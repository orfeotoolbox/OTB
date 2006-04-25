/* /////////////////////////////////////////////////////////////////////////////
 * File:    dlfcn.h
 *
 * Purpose: Declaration of the dlopen, dlclose(), dlsym() and dlerror() API
 *          functions.
 *
 * Created: 1st January 2004
 * Updated: 19th September 2005
 *
 * Home:    http://synesis.com.au/software/
 *
 * Copyright (c) 2004-2005, Matthew Wilson and Synesis Software
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


/** \file dlfcn.h
 *
 * Contains the declarations for the dlopen() API.
 */

#ifndef SYNSOFT_UNIXEM_INCL_H_DLFCN
#define SYNSOFT_UNIXEM_INCL_H_DLFCN

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define SYNSOFT_UNIXEM_VER_H_DLFCN_MAJOR       2
# define SYNSOFT_UNIXEM_VER_H_DLFCN_MINOR       1
# define SYNSOFT_UNIXEM_VER_H_DLFCN_REVISION    2
# define SYNSOFT_UNIXEM_VER_H_DLFCN_EDIT        12
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

#include <sys/types.h>

/* ////////////////////////////////////////////////////////////////////////// */

/** \weakgroup unixem Synesis Software UNIX Emulation for Win32
 * \brief The UNIX emulation library
 */

/** \weakgroup unixem_dlopen dlopen() API
 * \ingroup UNIXem unixem
 * \brief This API provides facilities for manipulating dynamically loaded
 * executable modules
 * @{
 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WIN32
# error This file is only currently defined for compilation on Win32 systems
#endif /* _WIN32 */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants and definitions
 */

#define RTLD_LAZY           (0x00001)       /*!< Lazy function call binding. */
#define RTLD_NOW            (0x00002)       /*!< Immediate function call binding. */

/* /////////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** \brief Loads the given module, 
 *
 * \param moduleName The name of the module. The name is passed through to Win32's
 * LoadLibrary(), so the value of this parameter is interpreted according to Win32's
 * loading rules.
 * \param mode The loading mode. This is ignored on Win32 platforms, and the function
 * always behaves as if RTLD_NOW was specified
 * \note 
 */
void        *dlopen(char const *moduleName, int mode);

/** \brief Closes the given module
 *
 * \param hModule The handle of the executable module to close
 * \retval 0 success
 * \retval !0 failure. dlerror() will return an error string
 */
int         dlclose(void *hModule);

/** \brief Looks up a symbol
 *
 * \param hModule The handle of the executable module in which to search for the symbol
 * \param symbolName The name of the symbol
 * \return The pointer to the symbol, or NULL if no matching symbol was found
 * \retval NULL The symbol was not found
 */
void        *dlsym(void *hModule, char const *symbolName);

/** \brief Returns a descriptive string for the last error
 *
 * \return A pointer to a string describing the last error, or NULL.
 * \retval NULL No error occured, or dlerror() already called
 * \note Each call to this function resets it, so that a second call will always return NULL
 */
char const  *dlerror(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

/** @} // end of group unixem_dlopen */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* SYNSOFT_UNIXEM_INCL_H_DLFCN */

/* ////////////////////////////////////////////////////////////////////////// */
