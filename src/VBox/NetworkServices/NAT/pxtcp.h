/* $Id: pxtcp.h 56300 2015-06-09 14:36:22Z vboxsync $ */
/** @file
 * NAT Network - TCP proxy, internal interface declarations.
 */

/*
 * Copyright (C) 2013-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef _pxtcp_h_
#define _pxtcp_h_

struct pxtcp;
struct fwspec;

struct pxtcp *pxtcp_create_forwarded(SOCKET);
void pxtcp_cancel_forwarded(struct pxtcp *);

void pxtcp_pcb_connect(struct pxtcp *, const struct fwspec *);

int pxtcp_pmgr_add(struct pxtcp *);
void pxtcp_pmgr_del(struct pxtcp *);

#endif  /* _pxtcp_h_ */
