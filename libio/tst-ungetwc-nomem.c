/* Test ungetwc behavior with malloc failures.
   Copyright The GNU Toolchain Authors.
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

#include <stdio.h>
#include <wchar.h>
#include <support/check.h>
#include <support/support.h>
#include <support/temp_file.h>

extern void *__libc_malloc (size_t)
     __attribute__ ((malloc)) __attribute__ ((alloc_size (1)));

static bool fail = false;

void *
malloc (size_t sz)
{
  if (fail)
    return NULL;

  return __libc_malloc (sz);
}

static int
do_test (void)
{
  char *filename = NULL;
  struct stat props = {};
  size_t bufsz = 0;

  create_temp_file ("tst-ungetwc-nomem.", &filename);
  if (stat (filename, &props) != 0)
    FAIL_EXIT1 ("Could not get file status: %m\n");

  FILE *fp = fopen (filename, "w");

  /* The libio buffer sizes are the same as block size.  */
  bufsz = (props.st_blksize + 2);

  wchar_t *buf = xmalloc (bufsz * sizeof (wchar_t));
  wmemset (buf, L'a', bufsz);

  size_t remaining = bufsz;
  while (remaining > 0)
    {
      size_t done = fwrite (buf, sizeof (wchar_t), remaining, fp);
      if (done == 0)
	break;
      remaining -= done;
    }
  fclose (fp);

  /* Begin test.  */
  fp = fopen (filename, "r");


  /* The standard requires the first ungetwc to always work.  */
  fail = true;
  TEST_COMPARE (ungetwc(L'y', fp), L'y');

  /* Now let the buffers get allocated to allow for subsequent tests.  */
  fail = false;
  TEST_COMPARE (fgetwc (fp), L'y');
  TEST_COMPARE (ungetwc(L'y', fp), L'y');
  TEST_COMPARE (fgetwc (fp), L'y');

  while (!feof (fp))
    {
      fail = true;
      TEST_COMPARE (ungetwc(L'y', fp), L'y');
      fail = false;
      TEST_COMPARE (fgetwc (fp), L'y');
      if (fgetwc (fp) != L'a')
	TEST_COMPARE (ferror (fp), 0);
    }
  return 0;
}

#include <support/test-driver.c>
