/* /////////////////////////////////////////////////////////////////////////////
 * File:    unixem/unixem.h
 *
 * Purpose: Root header for the UNIXEm API.
 *
 * Created: 29th August 2005
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


/** \file unixem/unixem.h Root header for the UNIXEm API
 */

#ifndef UNIXEM_INCL_UNIXEM_H_UNIXEM
#define UNIXEM_INCL_UNIXEM_H_UNIXEM

/* /////////////////////////////////////////////////////////////////////////////
 * Version
 */

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define UNIXEM_VER_UNIXEM_H_UNIXEM_MAJOR       1
# define UNIXEM_VER_UNIXEM_H_UNIXEM_MINOR       0
# define UNIXEM_VER_UNIXEM_H_UNIXEM_REVISION    1
# define UNIXEM_VER_UNIXEM_H_UNIXEM_EDIT        1
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/** \def UNIXEM_VER_MAJOR
 * The major version number of UNIXEm
 */

/** \def UNIXEM_VER_MINOR
 * The minor version number of UNIXEm
 */

/** \def UNIXEM_VER_REVISION
 * The revision version number of UNIXEm
 */

/** \def UNIXEM_VER
 * The current composite version number of UNIXEm
 */

#define UNIXEM_VER_MAJOR       1
#define UNIXEM_VER_MINOR       6
#define UNIXEM_VER_REVISION    1

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define UNIXEM_VER_1_0_1       0x01000100
# define UNIXEM_VER_1_1_1       0x01010100
# define UNIXEM_VER_1_2_1       0x01020100
# define UNIXEM_VER_1_3_1       0x01030100
# define UNIXEM_VER_1_4_1       0x01040100
# define UNIXEM_VER_1_5_1       0x01050100
# define UNIXEM_VER_1_5_2       0x01050200
# define UNIXEM_VER_1_5_3       0x01050300
# define UNIXEM_VER_1_6_1       0x01060100

# define UNIXEM_VER             UNIXEM_VER_1_6_1
#else /* ? UNIXEM_DOCUMENTATION_SKIP_SECTION */
# define UNIXEM_VER             0x01060100
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* UNIXEM_INCL_UNIXEM_H_UNIXEM */

/* ////////////////////////////////////////////////////////////////////////// */
