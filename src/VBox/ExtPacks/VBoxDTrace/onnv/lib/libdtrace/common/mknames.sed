#$Id: mknames.sed 53658 2015-01-02 12:31:02Z vboxsync $
## @file
# pure sed version of mknames.sh.
#

1b heading
$b footer
/DIF_SUBR_MAX/b end
s/^#[[:space:]]*define[[:space:]][[:space:]]*\(DIF_SUBR_[a-zA-Z0-9_]*\).*$/        case \1: \
            return "\L\1";/p
b end

:heading
i\/* Autogenerated source file - DO NOT EDIT! */
i\/*
i\ * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
i\ * Use is subject to license terms.
i\ */
i\#include <dtrace.h>
i\const char *dtrace_subrstr(dtrace_hdl_t *dtp, int subr)
i\{
i\    switch (subr)
i\    {
b end

:footer
i\        default:
i\            return "unknown";
i\    }
i\}
b end




:end

