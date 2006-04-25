/* /////////////////////////////////////////////////////////////////////////////
 * File:    time.h (sys/time.h)
 *
 * Purpose: Declaration of the gettimeofday() API function and supporting
 *          types for the Win32 platform.
 *
 * Created: 8th December 2002
 * Updated: 21st September 2005
 *
 * Home:    http://synesis.com.au/software/
 *
 * Copyright (c) 2002-2005, Matthew Wilson and Synesis Software
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


/** \file sys/time.h
 *
 * Contains time functions and types.
 */

#ifndef SYNSOFT_UNIXEM_INCL_SYS_H_TIME
#define SYNSOFT_UNIXEM_INCL_SYS_H_TIME

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define SYNSOFT_UNIXEM_VER_SYS_H_TIME_MAJOR    2
# define SYNSOFT_UNIXEM_VER_SYS_H_TIME_MINOR    0
# define SYNSOFT_UNIXEM_VER_SYS_H_TIME_REVISION	2
# define SYNSOFT_UNIXEM_VER_SYS_H_TIME_EDIT     14
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/** \weakgroup unixem Synesis Software UNIX Emulation for Win32
 * \brief The UNIX emulation library
 */

/** \weakgroup unixem_time time() API
 * \ingroup UNIXem unixem
 * \brief Time functions and types
 * @{
 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** \brief A representation of current time, used by gettimeofday().
 *
 * This structure represents the time, in seconds and microseconds, since Jan.
 * 1, 1970, in UTC. Returned by the gettimeofday() function.
 */

#if !defined(_WINSOCKAPI_) && \
	!defined(_WINSOCK2API_) && \
	!defined(_WINSOCK2_H) && \
	!defined(_TIMEVAL_DEFINED)

/*
	(	!defined(__GNUC__) || \
		!defined(_TIMEVAL_DEFINED))
*/

struct timeval
{
    long tv_sec;    /*!< The number of seconds, since Jan. 1, 1970, in the time value. */
    long tv_usec;   /*!< The number of microseconds in the time value. */
};

# if !defined(_TIMEVAL_DEFINED)
#  define _TIMEVAL_DEFINED
# endif /* __GNUC__ */

#endif /* _WINSOCKAPI_ / _TIMEVAL_DEFINED */

/** \brief
 *
 * This structure is NOT used in any meaningful way by the gettimeofday() function
 * implementation in this library.
 */
struct timezone
{
    int tz_minuteswest; /*!< minutes west of Greenwich. */
    int tz_dsttime;     /*!< type of dst corrections to apply. */
};

/** \brief Accesses the current UTC time
 *
 * Retrieves the current time and timezone information, relative to Greenwich Mean Time
 *
 * \param tv Pointer to a timeval structure to receive the time information
 * \param tz Pointer to a timezone structure to receive the timezone information
 */
extern void gettimeofday(struct timeval *tv, struct timezone *tz);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

/** @} // end of group unixem_time */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* SYNSOFT_UNIXEM_INCL_SYS_H_TIME */

/* ////////////////////////////////////////////////////////////////////////// */
