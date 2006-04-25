/* /////////////////////////////////////////////////////////////////////////////
 * File:    unistd.h
 *
 * Purpose: Declaration of the .
 *
 * Created: 1st November 2003
 * Updated: 21st September 2005
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


/** \file unistd.h
 *
 * Contains standard limits and declarations.
 */

#ifndef SYNSOFT_UNIXEM_INCL_H_UNISTD
#define SYNSOFT_UNIXEM_INCL_H_UNISTD

#ifndef UNIXEM_DOCUMENTATION_SKIP_SECTION
# define SYNSOFT_UNIXEM_VER_H_UNISTD_MAJOR      2
# define SYNSOFT_UNIXEM_VER_H_UNISTD_MINOR      3
# define SYNSOFT_UNIXEM_VER_H_UNISTD_REVISION   4
# define SYNSOFT_UNIXEM_VER_H_UNISTD_EDIT       26
#endif /* !UNIXEM_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#include <stddef.h>     /* for size_t */
#include <sys/types.h>  /* for mode_t */

/* ////////////////////////////////////////////////////////////////////////// */

/** \weakgroup unixem Synesis Software UNIX Emulation for Win32
 * \brief The UNIX emulation library
 */

/** \weakgroup unixem_unistd unistd.h
 * \ingroup UNIXem unixem
 * \brief Standard limits and declarations
 * @{
 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WIN32
# error This file is only currently defined for compilation on Win32 systems
#endif /* _WIN32 */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants and definitions
 */

//#ifndef PATH_MAX
//# define PATH_MAX   (260)   /*!< \brief The maximum number of characters (including null terminator) in a directory entry name */
//#endif /* !PATH_MAX */

enum
{
        _PC_LINK_MAX                    /*!< The maximum number of links to the file. */
#define _PC_LINK_MAX            _PC_LINK_MAX
    ,   _PC_MAX_CANON                   /*!< Maximum number of bytes in canonical input line. Applicable only to terminal devices. */
#define _PC_MAX_CANON           _PC_MAX_CANON
    ,   _PC_MAX_INPUT                   /*!< Maximum number of bytes allowed in an input queue. Applicable only to terminal devices. */
#define _PC_MAX_INPUT           _PC_MAX_INPUT
    ,   _PC_NAME_MAX                    /*!< Maximum number of bytes in a file name, not including a nul terminator. This number can range from 14 through 255. This value is applicable only to a directory file. */
#define _PC_NAME_MAX            _PC_NAME_MAX
    ,   _PC_PATH_MAX                    /*!< Maximum number of bytes in a path name, including a nul terminator. */
#define _PC_PATH_MAX            _PC_PATH_MAX
 
    ,   _PC_PIPE_BUF                    /*!< Maximum number of bytes guaranteed to be written atomically. This value is applicable only to a first-in-first-out (FIFO). */
#define _PC_PIPE_BUF            _PC_PIPE_BUF
    ,   _PC_CHOWN_RESTRICTED            /*!< Returns 0 if the use of the chown subroutine is restricted to a process with appropriate privileges, and if the chown subroutine is restricted to changing the group ID of a file only to the effective group ID of the process or to one of its supplementary group IDs. */
#define _PC_CHOWN_RESTRICTED    _PC_CHOWN_RESTRICTED
    ,   _PC_NO_TRUNC                    /*!< Returns 0 if long component names are truncated. This value is applicable only to a directory file. */
#define _PC_NO_TRUNC            _PC_NO_TRUNC
    ,   _PC_VDISABLE                    /*!< This is always 0. No disabling character is defined. This value is applicable only to a terminal device. */
#define _PC_VDISABLE            _PC_VDISABLE
    ,   _PC_AIX_DISK_PARTITION          /*!< Determines the physical partition size of the disk. 
Note:
The _PC_AIX_DISK_PARTITION variable is available only to the root user. */
#define _PC_AIX_DISK_PARTITION    _PC_AIX_DISK_PARTITION
    ,   _PC_AIX_DISK_SIZE               /*!< Determines the disk size in megabytes. 
Note:
The _PC_AIX_DISK_SIZE variable is available only to the root user.
Note:
The _PC_FILESIZEBITS and PC_SYNC_IO flags apply to AIX 4.3 and later releases. */
#define _PC_AIX_DISK_SIZE           _PC_AIX_DISK_SIZE
    ,   _PC_FILESIZEBITS                /*!< Returns the minimum number of bits required to hold the file system's maximum file size as a signed integer. The smallest value returned is 32. */
#define _PC_FILESIZEBITS            _PC_FILESIZEBITS
    ,   _PC_SYNC_IO                     /*!< Returns -1 if the file system does not support the Synchronized Input and Output option. Any value other than -1 is returned if the file system supports the option. */
#define _PC_SYNC_IO                 _PC_SYNC_IO
};

/* /////////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** \brief Creates a hardlink.
 *
 * This function creates a link from \c originalFile to \c linkName.
 *
 * \param originalFile Path of the original file
 * \param linkName Path of the link
 *
 * \return O on success, or -1 if there is an error
 *
 * \note Hardlink support is only available on Windows 2000 and later, and only
 *        works within a single drive.
 */
int link(const char *originalFile, const char *linkName);


/** \brief Unlinks a file or directory
 *
 * \param path The path of the file or directory to unlink
 *
 * \return O on success, or -1 if there is an error
 */
int unlink(const char *path);

/** \brief Change the current working directory.
 *
 * This function changes the current working directory to the directory
 * specified by dirName. dirName must refer to an existing directory.
 *
 * \param dirName Path of new working directory
 * \return O on success, or -1 if there is an error
 */
int chdir(const char *dirName);

/** \brief Get the current working directory
 *
 * This function gets the full path of the current working directory
 * and stores it in buffer.
 *
 * \param buffer Storage location for the current working directory
 * \param max_len Maximum length of path (in characters)
 * \return buffer on success, or NULL to indicate error.
 */
char *getcwd(char *buffer, size_t max_len);

#if !defined(__MWERKS__)

/** \brief Creates the given directory
 *
 * This function creates the named directory.
 *
 * \param dirName Path of directory to remove
 * \param mode The access permissions of the directory
 *
 * \return O on success, or -1 if there is an error
 */
int mkdir(const char *dirName, unsigned mode);

#endif /* !__MWERKS__ */

/** \brief Removes the given directory
 *
 * This function removes the named directory.
 *
 * \param dirName Path of directory to remove
 * \return O on success, or -1 if there is an error
 */
int rmdir(const char *dirName);

/** \brief Closes a file
 *
 * \param handle The handle of the file to be closed
 * \return 0 on success, or -1 if there is an error
 */
int close(int handle);

/** \brief Creates a pipe
 *
 * \param handles An array of two handles. handles[0] will be set to the
 * read stream. handels[1] will be set to the write stream
 * \return 0 on success, or -1 if there is an error
 */
/* int pipe(int handles[2]); */

/** \brief Returns the size, in bytes, of the page size
 */
int getpagesize(void);

/** \brief Provides access to various system limits not available at compile time
 */
long pathconf(char const *path, int name);


/** \brief Turns \c path into a fully qualified path, resolving all symbolic 
 * links, multiple /, /./ and /../
 *
 * \param path The relative path to be converted into absolute form
 * \param resolvedPath Pointer to a buffer to receive the path. This must contain
 *  sufficient storage for a valid path
 */
char *realpath(char const *path, char resolvedPath[]);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

/** @} // end of group unixem_unistd */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* SYNSOFT_UNIXEM_INCL_H_UNISTD */

/* ////////////////////////////////////////////////////////////////////////// */
