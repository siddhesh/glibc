/* System-specific extensions of <unistd.h>, Hurd version.
   Copyright (C) 2019-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _UNISTD_H
# error "Never include <bits/unistd_ext.h> directly; use <unistd.h> instead."
#endif

#ifdef __USE_GNU

/* Set the FD_CLOEXEC bit instead of closing the file descriptor.  */
#define CLOSE_RANGE_CLOEXEC (1U << 2)

/* Close the file descriptors from FIRST up to LAST, inclusive.
   If CLOSE_RANGE_CLOEXEC is set in FLAGS, set the FD_CLOEXEC flag
   instead of closing.  */
extern int close_range (unsigned int __first, unsigned int __last,
			int __flags) __THROW;

#endif /* __USE_GNU  */