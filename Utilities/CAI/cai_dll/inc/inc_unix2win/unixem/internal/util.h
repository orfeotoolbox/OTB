/* /////////////////////////////////////////////////////////////////////////////
 * File:    unixem/internal/util.h
 *
 * Purpose: Internal utility header for the UNIXEm API.
 *
 * Created: 2nd September 2005
 * Updated: 2nd September 2005
 *
 * Home:    http://synesis.com.au/software/
 *
 * Copyright 1994-2005, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the names of Matthew Wilson and Synesis Software nor the names of
 *   any contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/** \file unixem/internal/util Internal utility header for the UNIXEm API
 */

#ifndef UNIXEM_INCL_UNIXEM_INTERNAL_H_UTIL
#define UNIXEM_INCL_UNIXEM_INTERNAL_H_UTIL

/* /////////////////////////////////////////////////////////////////////////////
 * Version
 */

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define UNIXEM_VER_UNIXEM_INTERNAL_H_UTIL_MAJOR    1
# define UNIXEM_VER_UNIXEM_INTERNAL_H_UTIL_MINOR    0
# define UNIXEM_VER_UNIXEM_INTERNAL_H_UTIL_REVISION 1
# define UNIXEM_VER_UNIXEM_INTERNAL_H_UTIL_EDIT     1
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#include <unixem/unixem.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Macros
 */

#ifndef NUM_ELEMENTS
# define NUM_ELEMENTS(x)        (sizeof(x) / sizeof(0[x]))
#endif /* !NUM_ELEMENTS */

/* /////////////////////////////////////////////////////////////////////////////
 * Function declarations
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int		errno_from_Win32(unsigned long);

/** Gets the drive letter of the current working directory
 *
 * \retval Either an uppercase letter, or '\0' on failure;
 */
char	get_current_drive(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* UNIXEM_INCL_UNIXEM_INTERNAL_H_UTIL */

/* ////////////////////////////////////////////////////////////////////////// */
