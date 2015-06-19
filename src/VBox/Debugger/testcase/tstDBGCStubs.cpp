/* $Id: tstDBGCStubs.cpp 56296 2015-06-09 14:30:56Z vboxsync $ */
/** @file
 * DBGC Testcase - Command Parser, VMM Stub Functions.
 */

/*
 * Copyright (C) 2006-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#include <VBox/err.h>
#include <VBox/vmm/vm.h>
#include <iprt/string.h>



#include <VBox/vmm/dbgf.h>
VMMR3DECL(PDBGFADDRESS) DBGFR3AddrFromFlat(PUVM pUVM, PDBGFADDRESS pAddress, RTGCUINTPTR FlatPtr)
{
    return NULL;
}

VMMR3DECL(int) DBGFR3AddrFromSelOff(PUVM pUVM, VMCPUID idCpu, PDBGFADDRESS pAddress, RTSEL Sel, RTUINTPTR off)
{
    /* bad:bad -> provke error during parsing. */
    if (Sel == 0xbad && off == 0xbad)
        return VERR_OUT_OF_SELECTOR_BOUNDS;

    /* real mode conversion. */
    pAddress->FlatPtr = (uint32_t)(Sel << 4) | off;
    pAddress->fFlags |= DBGFADDRESS_FLAGS_FLAT;
    pAddress->Sel     = DBGF_SEL_FLAT;
    pAddress->off     = pAddress->FlatPtr;
    return VINF_SUCCESS;
}

VMMR3DECL(int)  DBGFR3AddrToPhys(PUVM pUVM, VMCPUID idCpu, PDBGFADDRESS pAddress, PRTGCPHYS pGCPhys)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(int) DBGFR3Attach(PUVM pUVM)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(int) DBGFR3BpClear(PUVM pUVM, RTUINT iBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpDisable(PUVM pUVM, RTUINT iBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpEnable(PUVM pUVM, RTUINT iBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpEnum(PUVM pUVM, PFNDBGFBPENUM pfnCallback, void *pvUser)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpSet(PUVM pUVM, PCDBGFADDRESS pAddress, uint64_t iHitTrigger, uint64_t iHitDisable, PRTUINT piBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpSetReg(PUVM pUVM, PCDBGFADDRESS pAddress, uint64_t iHitTrigger, uint64_t iHitDisable,
                              uint8_t fType, uint8_t cb, PRTUINT piBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3BpSetREM(PUVM pUVM, PCDBGFADDRESS pAddress, uint64_t iHitTrigger, uint64_t iHitDisable, PRTUINT piBp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3QueryWaitable(PUVM pUVM)
{
    return VINF_SUCCESS;
}
VMMR3DECL(int) DBGFR3Detach(PUVM pUVM)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3DisasInstrEx(PUVM pUVM, VMCPUID idCpu, RTSEL Sel, RTGCPTR GCPtr, uint32_t fFlags,
                                  char *pszOutput, uint32_t cchOutput, uint32_t *pcbInstr)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3EventWait(PUVM pUVM, RTMSINTERVAL cMillies, PCDBGFEVENT *ppEvent)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3Halt(PUVM pUVM)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3Info(PUVM pUVM, const char *pszName, const char *pszArgs, PCDBGFINFOHLP pHlp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3InfoEx(PUVM pUVM, VMCPUID idCpu, const char *pszName, const char *pszArgs, PCDBGFINFOHLP pHlp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(bool) DBGFR3IsHalted(PUVM pUVM)
{
    return true;
}
VMMR3DECL(int) DBGFR3LogModifyDestinations(PUVM pUVM, const char *pszDestSettings)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3LogModifyFlags(PUVM pUVM, const char *pszFlagSettings)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3LogModifyGroups(PUVM pUVM, const char *pszGroupSettings)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(RTDBGCFG) DBGFR3AsGetConfig(PUVM pUVM)
{
    return NIL_RTDBGCFG;
}
VMMR3DECL(int) DBGFR3AsLoadImage(PUVM pUVM, RTDBGAS hAS, const char *pszFilename, const char *pszModName, RTLDRARCH enmArch,
                                 PCDBGFADDRESS pModAddress, RTDBGSEGIDX iModSeg, uint32_t fFlags)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3AsLoadMap(PUVM pUVM, RTDBGAS hAS, const char *pszFilename, const char *pszModName, PCDBGFADDRESS pModAddress, RTDBGSEGIDX iModSeg, RTGCUINTPTR uSubtrahend, uint32_t fFlags)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3AsUnlinkModuleByName(PUVM pUVM, RTDBGAS hDbgAs, const char *pszModName)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(RTDBGAS) DBGFR3AsResolveAndRetain(PUVM pUVM, RTDBGAS hAlias)
{
    return NIL_RTDBGAS;
}
VMMR3DECL(int) DBGFR3AsLineByAddr(PUVM pUVM, RTDBGAS hDbgAs, PCDBGFADDRESS pAddress,
                                  PRTGCINTPTR poffDisp, PRTDBGLINE pLine, PRTDBGMOD phMod)
{
    return VERR_DBG_LINE_NOT_FOUND;
}
VMMR3DECL(int) DBGFR3Resume(PUVM pUVM)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3StackWalkBegin(PUVM pUVM, VMCPUID idCpu, DBGFCODETYPE enmCodeType, PCDBGFSTACKFRAME *ppFirstFrame)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(PCDBGFSTACKFRAME) DBGFR3StackWalkNext(PCDBGFSTACKFRAME pCurrent)
{
    return NULL;
}
VMMR3DECL(void) DBGFR3StackWalkEnd(PCDBGFSTACKFRAME pFirstFrame)
{
}
VMMR3DECL(int) DBGFR3Step(PUVM pUVM, VMCPUID idCpu)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3AsSymbolByAddr(PUVM pUVM, RTDBGAS hDbgAs, PCDBGFADDRESS pAddress, uint32_t fFlags, PRTGCINTPTR poffDisplacement, PRTDBGSYMBOL pSymbol, PRTDBGMOD phMod)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3AsSymbolByName(PUVM pUVM, RTDBGAS hDbgAs, const char *pszSymbol, PRTDBGSYMBOL pSymbol, PRTDBGMOD phMod)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3MemScan(PUVM pUVM, VMCPUID idCpu, PCDBGFADDRESS pAddress, RTGCUINTPTR cbRange, RTGCUINTPTR uAlign, const void *pabNeedle, size_t cbNeedle, PDBGFADDRESS pHitAddress)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3MemRead(PUVM pUVM, VMCPUID idCpu, PCDBGFADDRESS pAddress, void *pvBuf, size_t cbRead)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3MemReadString(PUVM pUVM, VMCPUID idCpu, PCDBGFADDRESS pAddress, char *pszBuf, size_t cchBuf)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3MemWrite(PUVM pUVM, VMCPUID idCpu, PCDBGFADDRESS pAddress, const void *pvBuf, size_t cbRead)
{
    return VERR_INTERNAL_ERROR;
}
VMMDECL(int) DBGFR3PagingDumpEx(PUVM pUVM, VMCPUID idCpu, uint32_t fFlags, uint64_t cr3, uint64_t u64FirstAddr,
                                uint64_t u64LastAddr, uint32_t cMaxDepth, PCDBGFINFOHLP pHlp)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegNmValidate(PUVM pUVM, VMCPUID idDefCpu, const char *pszReg)
{
    if (   !strcmp(pszReg, "ah")
        || !strcmp(pszReg, "ax")
        || !strcmp(pszReg, "eax")
        || !strcmp(pszReg, "rax"))
        return VINF_SUCCESS;
    return VERR_DBGF_REGISTER_NOT_FOUND;
}
VMMR3DECL(int) DBGFR3RegCpuQueryU8(  PUVM pUVM, VMCPUID idCpu, DBGFREG enmReg, uint8_t     *pu8)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegCpuQueryU16( PUVM pUVM, VMCPUID idCpu, DBGFREG enmReg, uint16_t    *pu16)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegCpuQueryU32( PUVM pUVM, VMCPUID idCpu, DBGFREG enmReg, uint32_t    *pu32)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegCpuQueryU64( PUVM pUVM, VMCPUID idCpu, DBGFREG enmReg, uint64_t    *pu64)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegNmQuery(PUVM pUVM, VMCPUID idDefCpu, const char *pszReg, PDBGFREGVAL pValue, PDBGFREGVALTYPE penmType)
{
    if (idDefCpu == 0 || idDefCpu == DBGFREG_HYPER_VMCPUID)
    {
        if (!strcmp(pszReg, "ah"))
        {
            pValue->u16 = 0xf0;
            *penmType   = DBGFREGVALTYPE_U8;
            return VINF_SUCCESS;
        }
        if (!strcmp(pszReg, "ax"))
        {
            pValue->u16 = 0xbabe;
            *penmType   = DBGFREGVALTYPE_U16;
            return VINF_SUCCESS;
        }
        if (!strcmp(pszReg, "eax"))
        {
            pValue->u32 = 0xcafebabe;
            *penmType   = DBGFREGVALTYPE_U32;
            return VINF_SUCCESS;
        }
        if (!strcmp(pszReg, "rax"))
        {
            pValue->u64 = UINT64_C(0x00beef00feedface);
            *penmType   = DBGFREGVALTYPE_U32;
            return VINF_SUCCESS;
        }
    }
    return VERR_DBGF_REGISTER_NOT_FOUND;
}
VMMR3DECL(int) DBGFR3RegPrintf(PUVM pUVM, VMCPUID idCpu, char *pszBuf, size_t cbBuf, const char *pszFormat, ...)
{
    return VERR_INTERNAL_ERROR;
}
VMMDECL(ssize_t) DBGFR3RegFormatValue(char *pszBuf, size_t cbBuf, PCDBGFREGVAL pValue, DBGFREGVALTYPE enmType, bool fSpecial)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3RegNmSet(PUVM pUVM, VMCPUID idDefCpu, const char *pszReg, PCDBGFREGVAL pValue, DBGFREGVALTYPE enmType)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(PDBGFADDRESS) DBGFR3AddrFromPhys(PUVM pUVM, PDBGFADDRESS pAddress, RTGCPHYS PhysAddr)
{
    return NULL;
}
VMMR3DECL(int)  DBGFR3AddrToHostPhys(PUVM pUVM, VMCPUID idCpu, PDBGFADDRESS pAddress, PRTHCPHYS pHCPhys)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int)  DBGFR3AddrToVolatileR3Ptr(PUVM pUVM, VMCPUID idCpu, PDBGFADDRESS pAddress, bool fReadOnly, void **ppvR3Ptr)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(int) DBGFR3OSRegister(PUVM pUVM, PCDBGFOSREG pReg)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3OSDetect(PUVM pUVM, char *pszName, size_t cchName)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) DBGFR3OSQueryNameAndVersion(PUVM pUVM, char *pszName, size_t cchName, char *pszVersion, size_t cchVersion)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(void *) DBGFR3OSQueryInterface(PUVM pUVM, DBGFOSINTERFACE enmIf)
{
    return NULL;
}

VMMR3DECL(int) DBGFR3SelQueryInfo(PUVM pUVM, VMCPUID idCpu, RTSEL Sel, uint32_t fFlags, PDBGFSELINFO pSelInfo)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(CPUMMODE) DBGFR3CpuGetMode(PUVM pUVM, VMCPUID idCpu)
{
    return CPUMMODE_INVALID;
}
VMMR3DECL(VMCPUID) DBGFR3CpuGetCount(PUVM pUVM)
{
    return 1;
}
VMMR3DECL(bool) DBGFR3CpuIsIn64BitCode(PUVM pUVM, VMCPUID idCpu)
{
    return false;
}

VMMR3DECL(int) DBGFR3CoreWrite(PUVM pUVM, const char *pszFilename, bool fReplaceFile)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(int)  DBGFR3PlugInLoad(PUVM pUVM, const char *pszPlugIn, char *pszActual, size_t cbActual, PRTERRINFO pErrInfo)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int)  DBGFR3PlugInUnload(PUVM pUVM, const char *pszName)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(void) DBGFR3PlugInLoadAll(PUVM pUVM)
{
}


//////////////////////////////////////////////////////////////////////////
// The rest should eventually be replaced by DBGF calls and eliminated. //
/////////////////////////////////////////////////////////////////////////

#include <VBox/vmm/cpum.h>

VMMDECL(uint64_t) CPUMGetGuestCR3(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(uint64_t) CPUMGetGuestCR4(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(RTSEL) CPUMGetGuestCS(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(PCCPUMCTXCORE) CPUMGetGuestCtxCore(PVMCPU pVCpu)
{
    return NULL;
}

VMMDECL(uint32_t) CPUMGetGuestEIP(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(uint64_t) CPUMGetGuestRIP(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(RTGCPTR) CPUMGetGuestIDTR(PVMCPU pVCpu, uint16_t *pcbLimit)
{
    return 0;
}

VMMDECL(CPUMMODE) CPUMGetGuestMode(PVMCPU pVCpu)
{
    return CPUMMODE_INVALID;
}

VMMDECL(RTSEL) CPUMGetHyperCS(PVMCPU pVCpu)
{
    return 0xfff8;
}

VMMDECL(uint32_t) CPUMGetHyperEIP(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(PCPUMCTX) CPUMQueryGuestCtxPtr(PVMCPU pVCpu)
{
    return NULL;
}

VMMDECL(bool) CPUMIsGuestIn64BitCode(PVMCPU pVCpu)
{
    return false;
}

VMMDECL(uint32_t) CPUMGetGuestEFlags(PVMCPU pVCpu)
{
    return 2;
}

#include <VBox/vmm/hm.h>
VMMR3DECL(bool) HMR3IsEnabled(PUVM pUVM)
{
    return true;
}


#include <VBox/vmm/pgm.h>

VMMDECL(RTHCPHYS) PGMGetHyperCR3(PVMCPU pVCpu)
{
    return 0;
}

VMMDECL(PGMMODE) PGMGetShadowMode(PVMCPU pVCpu)
{
    return PGMMODE_INVALID;
}

VMMR3DECL(int) PGMR3DbgR3Ptr2GCPhys(PUVM pUVM, RTR3PTR R3Ptr, PRTGCPHYS pGCPhys)
{
    return VERR_INTERNAL_ERROR;
}

VMMR3DECL(int) PGMR3DbgR3Ptr2HCPhys(PUVM pUVM, RTR3PTR R3Ptr, PRTHCPHYS pHCPhys)
{
    return VERR_INTERNAL_ERROR;
}
VMMR3DECL(int) PGMR3DbgHCPhys2GCPhys(PUVM pUVM, RTHCPHYS HCPhys, PRTGCPHYS pGCPhys)
{
    return VERR_INTERNAL_ERROR;
}


#include <VBox/vmm/vmm.h>

VMMR3DECL(PVMCPU) VMMR3GetCpuByIdU(PUVM pUVM, RTCPUID idCpu)
{
    return NULL;
}


VMMR3DECL(PVM) VMR3GetVM(PUVM pUVM)
{
    return NULL;
}
