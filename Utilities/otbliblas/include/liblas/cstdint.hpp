/******************************************************************************
 * $Id: cstdint.hpp 838 2008-08-18 21:40:44Z hobu $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Integer types definitions
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 * This file has been stolen from <boost/cstdint.hpp> and
 * modified for libLAS purposes.
 * 
 * (C) Copyright Mateusz Loskot 2007, mateusz@loskot.net
 * (C) Copyright Phil Vachon 2007, philippe@cowpig.ca
 * (C) Copyright John Maddock 2001
 * (C) Copyright Jens Mauer 2001
 * (C) Copyright Beman Dawes 1999
 * Distributed under the Boost  Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 * 
 ******************************************************************************
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/
 
#ifndef LIBLAS_CSTDINT_HPP_INCLUDED
#define LIBLAS_CSTDINT_HPP_INCLUDED

#ifdef LIBLAS_C_API
#  include <limits.h>
#  ifndef _MSC_VER
#    include <stdint.h>
#  endif /* _MSC_VER */
#else /* LIBLAS_C_API */
#  include <climits>
namespace liblas
{
#endif /* LIBLAS_C_API */

/*
//  These are fairly safe guesses for some 16-bit, and most 32-bit and 64-bit
//  platforms.  For other systems, they will have to be hand tailored.
//
//  Because the fast types are assumed to be the same as the undecorated types,
//  it may be possible to hand tailor a more efficient implementation.  Such
//  an optimization may be illusionary; on the Intel x86-family 386 on, for
//  example, byte arithmetic and load/stores are as fast as "int" sized ones.

//  8-bit types  ------------------------------------------------------------//
*/

#ifndef UINT8_C
# if UCHAR_MAX == 0xff
    typedef unsigned char   uint8_t;
    typedef signed char     int8_t;
# else
#  error defaults not correct; you must hand modify liblas/cstdint.hpp
# endif
#else
# ifndef LIBLAS_C_API
    typedef ::uint8_t uint8_t;
    typedef ::int8_t int8_t;
# endif

#endif /* UINT8_C */

/*
//  16-bit types  -----------------------------------------------------------//
*/

#ifndef UINT16_C
# if USHRT_MAX == 0xffff
    typedef unsigned short  uint16_t;
    typedef short           int16_t;
# else
#   error defaults not correct; you must hand modify liblas/cstdint.hpp
# endif
#else
# ifndef LIBLAS_C_API
    typedef ::uint16_t uint16_t;
    typedef ::int16_t int16_t;
# endif
#endif /* UINT16_C */

/*
//  32-bit types  -----------------------------------------------------------//
*/

#ifndef UINT32_C
# if ULONG_MAX == 0xffffffff
    typedef unsigned long   uint32_t;
# elif UINT_MAX == 0xffffffff
    typedef unsigned int    uint32_t;
# else
#   error defaults not correct; you must hand modify liblas/cstdint.hpp
# endif
#else
# ifndef LIBLAS_C_API
    typedef ::uint32_t uint32_t;
# endif
#endif /* UINT32_C */

#ifndef INT32_C
# if ULONG_MAX == 0xffffffff
    typedef long            int32_t;
# elif UINT_MAX == 0xffffffff
    typedef int             int32_t;
# else
#   error defaults not correct; you must hand modify liblas/cstdint.hpp
# endif
#else
# ifndef LIBLAS_C_API
    typedef ::int32_t int32_t;
# endif
#endif /* INT32_C */

/*
//  64-bit types + intmax_t and uintmax_t  ----------------------------------//
*/

#ifndef UINT64_C
# if ULONG_MAX != 0xffffffffu
#  if ULONG_MAX == 18446744073709551615u /* 2**64 - 1 */
    typedef long                int64_t;
    typedef unsigned long       uint64_t;
#  else
#   error defaults not correct; you must hand modify boost/cstdint.hpp
#  endif
# elif defined(__GNUC__) || defined(HAVE_LONG_LONG)
    __extension__ typedef long long            int64_t;
    __extension__ typedef unsigned long long   uint64_t;
# elif defined(_MSC_VER)
/*    // we have Borland/Intel/Microsoft __int64: */
    typedef __int64             int64_t;
    typedef unsigned __int64    uint64_t;
# else /* // assume no 64-bit integers */
#  define LIBLAS_NO_INT64_T
# endif
#endif /* UINT64_C */

#ifndef LIBLAS_C_API
}  /* namespace liblas */
#endif /* LIBLAS_C_API */

#endif /* LIBLAS_CSTDINT_HPP_INCLUDED */

