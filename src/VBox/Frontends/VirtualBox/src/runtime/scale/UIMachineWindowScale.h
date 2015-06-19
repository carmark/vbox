/* $Id: UIMachineWindowScale.h 55401 2015-04-23 10:03:17Z vboxsync $ */
/** @file
 * VBox Qt GUI - UIMachineWindowScale class declaration.
 */

/*
 * Copyright (C) 2010-2012 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef __UIMachineWindowScale_h__
#define __UIMachineWindowScale_h__

/* Local includes: */
#include "UIMachineWindow.h"

/* Scale machine-window implementation: */
class UIMachineWindowScale : public UIMachineWindow
{
    Q_OBJECT;

protected:

    /* Constructor: */
    UIMachineWindowScale(UIMachineLogic *pMachineLogic, ulong uScreenId);

private:

    /* Prepare helpers: */
    void prepareMainLayout();
#ifdef Q_WS_MAC
    void prepareVisualState();
#endif /* Q_WS_MAC */
    void loadSettings();

    /* Cleanup helpers: */
    void saveSettings();
#ifdef Q_WS_MAC
    //void cleanupVisualState() {}
#endif /* Q_WS_MAC */
    //void cleanupMainLayout() {}

    /* Show stuff: */
    void showInNecessaryMode();

    /* Event handlers: */
    bool event(QEvent *pEvent);
#ifdef Q_WS_WIN
    bool winEvent(MSG *pMessage, long *pResult);
#endif /* Q_WS_WIN */

    /* Helpers: */
    bool isMaximizedChecked();

    /* Variables: */
    QRect m_normalGeometry;

    /* Factory support: */
    friend class UIMachineWindow;
};

#endif // __UIMachineWindowScale_h__

