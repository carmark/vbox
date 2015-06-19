/* $Id: VBoxX11Helper.h 55401 2015-04-23 10:03:17Z vboxsync $ */
/** @file
 * VBox Qt GUI - VBox X11 helper functions.
 */

/*
 * Copyright (C) 2006-2010 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef __VBoxX11Helpers_h__
#define __VBoxX11Helpers_h__

void X11ScreenSaverSettingsInit();
void X11ScreenSaverSettingsSave();
void X11ScreenSaverSettingsRestore();
bool X11IsWindowManagerKWin();

#endif

