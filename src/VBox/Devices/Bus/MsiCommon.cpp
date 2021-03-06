/* $Id: MsiCommon.cpp 56292 2015-06-09 14:20:46Z vboxsync $ */
/** @file
 * MSI support routines
 */

/*
 * Copyright (C) 2010-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */
#define LOG_GROUP LOG_GROUP_DEV_PCI
/* Hack to get PCIDEVICEINT declare at the right point - include "PCIInternal.h". */
#define PCI_INCLUDE_PRIVATE
#include <VBox/pci.h>
#include <VBox/msi.h>
#include <VBox/vmm/pdmdev.h>
#include <VBox/log.h>

#include "MsiCommon.h"

/** @todo: use accessors so that raw PCI devices work correctly with MSI. */
DECLINLINE(uint16_t) msiGetMessageControl(PPCIDEVICE pDev)
{
    return PCIDevGetWord(pDev, pDev->Int.s.u8MsiCapOffset + VBOX_MSI_CAP_MESSAGE_CONTROL);
}

DECLINLINE(bool) msiIs64Bit(PPCIDEVICE pDev)
{
    return pciDevIsMsi64Capable(pDev);
}

DECLINLINE(uint32_t*) msiGetMaskBits(PPCIDEVICE pDev)
{
    uint8_t iOff = msiIs64Bit(pDev) ? VBOX_MSI_CAP_MASK_BITS_64 : VBOX_MSI_CAP_MASK_BITS_32;
    iOff += pDev->Int.s.u8MsiCapOffset;
    return (uint32_t*)(pDev->config + iOff);
}

DECLINLINE(uint32_t*) msiGetPendingBits(PPCIDEVICE pDev)
{
    uint8_t iOff = msiIs64Bit(pDev) ? VBOX_MSI_CAP_PENDING_BITS_64 : VBOX_MSI_CAP_PENDING_BITS_32;
    iOff += pDev->Int.s.u8MsiCapOffset;
    return (uint32_t*)(pDev->config + iOff);
}

DECLINLINE(bool) msiIsEnabled(PPCIDEVICE pDev)
{
    return (msiGetMessageControl(pDev) & VBOX_PCI_MSI_FLAGS_ENABLE) != 0;
}

DECLINLINE(uint8_t) msiGetMme(PPCIDEVICE pDev)
{
    return (msiGetMessageControl(pDev) & VBOX_PCI_MSI_FLAGS_QSIZE) >> 4;
}

DECLINLINE(RTGCPHYS) msiGetMsiAddress(PPCIDEVICE pDev)
{
    if (msiIs64Bit(pDev))
    {
        uint32_t lo = PCIDevGetDWord(pDev, pDev->Int.s.u8MsiCapOffset + VBOX_MSI_CAP_MESSAGE_ADDRESS_LO);
        uint32_t hi = PCIDevGetDWord(pDev, pDev->Int.s.u8MsiCapOffset + VBOX_MSI_CAP_MESSAGE_ADDRESS_HI);
        return RT_MAKE_U64(lo, hi);
    }
    else
    {
        return PCIDevGetDWord(pDev, pDev->Int.s.u8MsiCapOffset + VBOX_MSI_CAP_MESSAGE_ADDRESS_32);
    }
}

DECLINLINE(uint32_t) msiGetMsiData(PPCIDEVICE pDev, int32_t iVector)
{
    int16_t  iOff = msiIs64Bit(pDev) ? VBOX_MSI_CAP_MESSAGE_DATA_64 : VBOX_MSI_CAP_MESSAGE_DATA_32;
    uint16_t lo = PCIDevGetWord(pDev, pDev->Int.s.u8MsiCapOffset + iOff);

    // vector encoding into lower bits of message data
    uint8_t bits = msiGetMme(pDev);
    uint16_t uMask = ((1 << bits) - 1);
    lo &= ~uMask;
    lo |= iVector & uMask;

    return RT_MAKE_U32(lo, 0);
}

DECLINLINE(bool) msiBitJustCleared(uint32_t uOldValue,
                                   uint32_t uNewValue,
                                   uint32_t uMask)
{
    return (!!(uOldValue & uMask) && !(uNewValue & uMask));
}

DECLINLINE(bool) msiBitJustSet(uint32_t uOldValue,
                               uint32_t uNewValue,
                               uint32_t uMask)
{
    return (!(uOldValue & uMask) && !!(uNewValue & uMask));
}

#ifdef IN_RING3
void     MsiPciConfigWrite(PPDMDEVINS pDevIns, PCPDMPCIHLP pPciHlp, PPCIDEVICE pDev,
                           uint32_t u32Address, uint32_t val, unsigned len)
{
    int32_t iOff = u32Address - pDev->Int.s.u8MsiCapOffset;
    Assert(iOff >= 0 && (pciDevIsMsiCapable(pDev) && iOff < pDev->Int.s.u8MsiCapSize));

    Log2(("MsiPciConfigWrite: %d <- %x (%d)\n", iOff, val, len));

    uint32_t uAddr = u32Address;
    bool f64Bit = msiIs64Bit(pDev);

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t reg = i + iOff;
        uint8_t u8Val = (uint8_t)val;
        switch (reg)
        {
            case 0: /* Capability ID, ro */
            case 1: /* Next pointer,  ro */
                break;
            case VBOX_MSI_CAP_MESSAGE_CONTROL:
                /* don't change read-only bits: 1-3,7 */
                u8Val &= UINT8_C(~0x8e);
                pDev->config[uAddr] = u8Val | (pDev->config[uAddr] & UINT8_C(0x8e));
                break;
            case VBOX_MSI_CAP_MESSAGE_CONTROL + 1:
                /* don't change read-only bit 8, and reserved 9-15 */
                break;
            default:
                if (pDev->config[uAddr] != u8Val)
                {
                    int32_t maskUpdated = -1;

                    /* If we're enabling masked vector, and have pending messages
                       for this vector, we have to send this message now */
                    if (    !f64Bit
                         && (reg >= VBOX_MSI_CAP_MASK_BITS_32)
                         && (reg < VBOX_MSI_CAP_MASK_BITS_32 + 4)
                        )
                    {
                        maskUpdated = reg - VBOX_MSI_CAP_MASK_BITS_32;
                    }
                    if (    f64Bit
                         && (reg >= VBOX_MSI_CAP_MASK_BITS_64)
                         && (reg < VBOX_MSI_CAP_MASK_BITS_64 + 4)
                       )
                    {
                        maskUpdated = reg - VBOX_MSI_CAP_MASK_BITS_64;
                    }

                    if (maskUpdated != -1 && msiIsEnabled(pDev))
                    {
                        uint32_t*  puPending = msiGetPendingBits(pDev);
                        for (int iBitNum = 0; iBitNum < 8; iBitNum++)
                        {
                            int32_t iBit = 1 << iBitNum;
                            uint32_t uVector = maskUpdated*8 + iBitNum;

                            if (msiBitJustCleared(pDev->config[uAddr], u8Val, iBit))
                            {
                                Log(("msi: mask updated bit %d@%x (%d)\n", iBitNum, uAddr, maskUpdated));

                                /* To ensure that we're no longer masked */
                                pDev->config[uAddr] &= ~iBit;
                                if ((*puPending & (1 << uVector)) != 0)
                                {
                                    Log(("msi: notify earlier masked pending vector: %d\n", uVector));
                                    MsiNotify(pDevIns, pPciHlp, pDev, uVector, PDM_IRQ_LEVEL_HIGH, 0 /*uTagSrc*/);
                                }
                            }
                            if (msiBitJustSet(pDev->config[uAddr], u8Val, iBit))
                            {
                                Log(("msi: mask vector: %d\n", uVector));
                            }
                        }
                    }

                    pDev->config[uAddr] = u8Val;
                }
        }
        uAddr++;
        val >>= 8;
    }
}

uint32_t MsiPciConfigRead (PPDMDEVINS pDevIns, PPCIDEVICE pDev, uint32_t u32Address, unsigned len)
{
    int32_t iOff = u32Address - pDev->Int.s.u8MsiCapOffset;

    Assert(iOff >= 0 && (pciDevIsMsiCapable(pDev) && iOff < pDev->Int.s.u8MsiCapSize));
    uint32_t rv = 0;

    switch (len)
    {
        case 1:
            rv = PCIDevGetByte(pDev,  u32Address);
            break;
        case 2:
            rv = PCIDevGetWord(pDev,  u32Address);
            break;
        case 4:
            rv = PCIDevGetDWord(pDev, u32Address);
            break;
        default:
            Assert(false);
    }

    Log2(("MsiPciConfigRead: %d (%d) -> %x\n", iOff, len, rv));

    return rv;
}

int MsiInit(PPCIDEVICE pDev, PPDMMSIREG pMsiReg)
{
    if (pMsiReg->cMsiVectors == 0)
         return VINF_SUCCESS;

    /* We cannot init MSI on raw devices yet. */
    Assert(!pciDevIsPassthrough(pDev));

    uint16_t   cVectors    = pMsiReg->cMsiVectors;
    uint8_t    iCapOffset  = pMsiReg->iMsiCapOffset;
    uint8_t    iNextOffset = pMsiReg->iMsiNextOffset;
    bool       f64bit      = pMsiReg->fMsi64bit;
    uint16_t   iFlags      = 0;
    int        iMmc;

    /* Compute multiple-message capable bitfield */
    for (iMmc = 0; iMmc < 6; iMmc++)
    {
        if ((1 << iMmc) >= cVectors)
            break;
    }

    if ((cVectors > VBOX_MSI_MAX_ENTRIES) || (1 << iMmc) < cVectors)
        return VERR_TOO_MUCH_DATA;

    Assert(iCapOffset != 0 && iCapOffset < 0xff && iNextOffset < 0xff);

    /* We always support per-vector masking */
    iFlags |= VBOX_PCI_MSI_FLAGS_MASKBIT | iMmc;
    if (f64bit)
        iFlags |= VBOX_PCI_MSI_FLAGS_64BIT;
    /* How many vectors we're capable of */
    iFlags |= iMmc;

    pDev->Int.s.u8MsiCapOffset = iCapOffset;
    pDev->Int.s.u8MsiCapSize   = f64bit ? VBOX_MSI_CAP_SIZE_64 : VBOX_MSI_CAP_SIZE_32;

    PCIDevSetByte(pDev,  iCapOffset + 0, VBOX_PCI_CAP_ID_MSI);
    PCIDevSetByte(pDev,  iCapOffset + 1, iNextOffset); /* next */
    PCIDevSetWord(pDev,  iCapOffset + VBOX_MSI_CAP_MESSAGE_CONTROL, iFlags);

    *msiGetMaskBits(pDev)    = 0;
    *msiGetPendingBits(pDev) = 0;

    pciDevSetMsiCapable(pDev);

    return VINF_SUCCESS;
}

#endif /* IN_RING3 */


bool     MsiIsEnabled(PPCIDEVICE pDev)
{
    return pciDevIsMsiCapable(pDev) && msiIsEnabled(pDev);
}

void MsiNotify(PPDMDEVINS pDevIns, PCPDMPCIHLP pPciHlp, PPCIDEVICE pDev, int iVector, int iLevel, uint32_t uTagSrc)
{
    AssertMsg(msiIsEnabled(pDev), ("Must be enabled to use that"));

    uint32_t   uMask = *msiGetMaskBits(pDev);
    uint32_t*  puPending = msiGetPendingBits(pDev);

    LogFlow(("MsiNotify: %d pending=%x mask=%x\n", iVector, *puPending, uMask));

    /* We only trigger MSI on level up */
    if ((iLevel & PDM_IRQ_LEVEL_HIGH) == 0)
    {
        /* @todo: maybe clear pending interrupts on level down? */
#if 0
        *puPending &= ~(1<<iVector);
        LogFlow(("msi: clear pending %d, now %x\n", iVector, *puPending));
#endif
        return;
    }

    if ((uMask & (1<<iVector)) != 0)
    {
        *puPending |= (1<<iVector);
        LogFlow(("msi: %d is masked, mark pending, now %x\n", iVector, *puPending));
        return;
    }

    RTGCPHYS   GCAddr = msiGetMsiAddress(pDev);
    uint32_t   u32Value = msiGetMsiData(pDev, iVector);

    *puPending &= ~(1<<iVector);

    Assert(pPciHlp->pfnIoApicSendMsi != NULL);
    pPciHlp->pfnIoApicSendMsi(pDevIns, GCAddr, u32Value, uTagSrc);
}
