/** @file

  Copyright (c) 2011, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials are licensed and made available under
  the terms and conditions of the BSD License that accompanies this distribution.
  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

    NetBSD: fdopen.c,v 1.14 2003/08/07 16:43:22 agc Exp
    fdopen.c  8.1 (Berkeley) 6/4/93
**/
#include  <LibConfig.h>
#include  <sys/EfiCdefs.h>

#include  "namespace.h"
#include  <sys/types.h>

#include  <assert.h>
#include  <errno.h>
#include  <fcntl.h>
#include  <stdio.h>
#include  <unistd.h>

#include  "reentrant.h"
#include  "local.h"

#ifdef __weak_alias
__weak_alias(fdopen,_fdopen)
#endif

FILE *
fdopen(int fd, const char *mode)
{
  FILE *fp;
  int flags, oflags, fdflags, tmp;

  _DIAGASSERT(fd != -1);

  if ((flags = __sflags(mode, &oflags)) == 0)
    return (NULL);

  /* Make sure the mode the user wants is a subset of the actual mode. */
  if ((fdflags = fcntl(fd, F_GETFL, 0)) < 0)
    return (NULL);
  tmp = fdflags & O_ACCMODE;
  if (tmp != O_RDWR && (tmp != (oflags & O_ACCMODE))) {
    errno = EINVAL;
    return (NULL);
  }

  if (oflags & O_NONBLOCK) {
    struct stat st;
    if (fstat(fd, &st) == -1) {
      return (NULL);
    }
    if (!S_ISREG(st.st_mode)) {
      errno = EFTYPE;
      return (NULL);
    }
  }

  if ((fp = __sfp()) == NULL)
    return (NULL);
  fp->_flags = (unsigned short)flags;
  /*
   * If opened for appending, but underlying descriptor does not have
   * O_APPEND bit set, assert __SAPP so that __swrite() will lseek to
   * end before each write.
   */
  if ((oflags & O_APPEND) && !(fdflags & O_APPEND))
    fp->_flags |= __SAPP;
  fp->_file = (short)fd;
  fp->_cookie = fp;
  fp->_read = __sread;
  fp->_write = __swrite;
  fp->_seek = __sseek;
  fp->_close = __sclose;
  return (fp);
}
