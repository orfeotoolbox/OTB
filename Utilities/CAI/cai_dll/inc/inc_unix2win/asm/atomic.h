/* /////////////////////////////////////////////////////////////////////////////
 * File:    atomic.h
 *
 * Purpose: Declaration of Linux like atomic functions for the
 *          Win32 platform.
 *
 * Created: 21st November 2003
 * Updated: 19th September 2005
 *
 * Home:    http://synesis.com.au/software/
 *
 * Copyright (c) 2003-2005, Matthew Wilson and Synesis Software
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


#ifndef SYNSOFT_UNIXEM_INCL_SYS_H_ATOMIC
#define SYNSOFT_UNIXEM_INCL_SYS_H_ATOMIC

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define SYNSOFT_UNIXEM_VER_SYS_H_ATOMIC_MAJOR		2
# define SYNSOFT_UNIXEM_VER_SYS_H_ATOMIC_MINOR		1
# define SYNSOFT_UNIXEM_VER_SYS_H_ATOMIC_REVISION	1
# define SYNSOFT_UNIXEM_VER_SYS_H_ATOMIC_EDIT		14
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WIN32
# error This file is only currently defined for compilation on Win32 systems
#endif /* _WIN32 */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants and definitions
 */


/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 */

typedef signed long		atomic_t;

/* /////////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ATOMIC_INIT(i)		(i)

void    atomic_set(atomic_t volatile *v, int i);
int     atomic_write(atomic_t volatile *v, int i);
int     atomic_read(atomic_t volatile *v);

void    atomic_add(int i, atomic_t volatile *v);
void    atomic_sub(int i, atomic_t volatile *v);

void    atomic_inc(atomic_t volatile *v);
void    atomic_dec(atomic_t volatile *v);

/** Returns the previous value */
int     atomic_inc_and_test(atomic_t volatile *v);
/** Returns the previous value */
int     atomic_dec_and_test(atomic_t volatile *v);

/** Returns the previous value */
int     atomic_add_and_test(int i, atomic_t volatile *v);
/** Returns the previous value */
int     atomic_sub_and_test(int i, atomic_t volatile *v);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* SYNSOFT_UNIXEM_INCL_SYS_H_ATOMIC */

/* ////////////////////////////////////////////////////////////////////////// */
