/* This file is from the proposed iomem_simple package found at
   http://code.trak.dk/

   ioapi_mem2.c -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API

   This version of ioapi is designed to access memory rather than files.
   We do use a region of memory to put data in to and take it out of. We do
   not have auto-extending buffers and do not inform anyone else that the
   data has been written. It is really intended for accessing a zip archive
   embedded in an application such that I can write an installer with no
   external files. Creation of archives has not been attempted, although
   parts of the framework are present.

   Based on Unzip ioapi.c version 0.22, May 19th, 2003

   Copyright (C) 1998-2003 Gilles Vollant
             (C) 2003 Justin Fletcher

   Dynamically allocated memory version. Troels K 2004
      mem_close deletes the data: file is single-session. No filenames.

   This file is under the same license as the Unzip tool it is distributed
   with.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unzip.h"
#include "ioapi.h"

#ifndef ZOFF_T
   #define ZOFF_T uLong /* bw compability is default */
#endif
#ifndef ZPOS_T
   #define ZPOS_T long /* bw compability is default */
#endif

#if defined(_INC_WINDOWS) || defined(_WINDOWS_H)
   #define _BOOL_DEFINED
#endif

#ifndef _BOOL_DEFINED
   #define _BOOL_DEFINED
   typedef signed  int BOOL;
#endif
#ifndef FALSE
   #define FALSE 0
#endif
#ifndef TRUE
   #define TRUE  1
#endif

static int fseek_calc(ZPOS_T offset, int origin, ZPOS_T* position, ZPOS_T size)
{
   BOOL bOK = TRUE;
   switch (origin)
   {
      case SEEK_SET :
         //bOK = (offset >= 0) && (offset <= size);
         if (bOK) *position = offset;
         break;
      case SEEK_CUR :
         bOK = ((offset + *position) >= 0) && (((offset + *position) <= size));
         if (bOK) *position = offset + *position;
         break;
      case SEEK_END:
         bOK = ((size - offset) >= 0) && (((size - offset) <= size));
         if (bOK) *position = offset + size - 0;
         break;
      default:
         bOK = FALSE;
         break;
   }
   return bOK ? 0 : -1;
}

static voidpf ZCALLBACK mem_open OF((
   voidpf opaque,
   const char* filename,
   int mode));

static uLong ZCALLBACK mem_read OF((
   voidpf opaque,
   voidpf stream,
   void* buf,
   uLong size));

static uLong ZCALLBACK mem_write OF((
   voidpf opaque,
   voidpf stream,
   const void* buf,
   uLong size));

static ZPOS_T ZCALLBACK mem_tell OF((
   voidpf opaque,
   voidpf stream));

static long ZCALLBACK mem_seek OF((
   voidpf opaque,
   voidpf stream,
   ZOFF_T offset,
   int origin));

static int ZCALLBACK mem_close OF((
   voidpf opaque,
   voidpf stream));

static int ZCALLBACK mem_error OF((
   voidpf opaque,
   voidpf stream));

typedef struct _MEMFILE
{
  void* buffer;    /* Base of the region of memory we're using */
  ZPOS_T length;   /* Size of the region of memory we're using */
  ZPOS_T position; /* Current offset in the area */
} MEMFILE;

static uLong ZCALLBACK mem_read (opaque, stream, buf, size)
   voidpf opaque;
   voidpf stream;
   void* buf;
   uLong size;
{
   MEMFILE* handle = (MEMFILE*) stream;

   /* It's possible for this function to be called with an invalid position.
    */
   if (handle->position < 0 || handle->position >= ZIP_MAX_UNCOMPRESSED_SIZE)
   {
     return 0;
   }

   if ( (handle->position + size) > handle->length)
   {
      /* There is a bug in this original code. It's possible for the position
       * to exceed the size, which results in memcpy being handed a negative
       * size. See libkml's src/kml/base/zip_file_test.cc for some overflow
       * tests that exercise this.
       * size = handle->length - handle->position;
      */
      int size_ = handle->length - handle->position;
      size = (size_ < 0) ? 0 : (uLong)size_;
   }

   memcpy(buf, ((char*)handle->buffer) + handle->position, size);
   handle->position+=size;

   return size;
}

static uLong ZCALLBACK mem_write (opaque, stream, buf, size)
   voidpf opaque;
   voidpf stream;
   const void* buf;
   uLong size;
{
   MEMFILE* handle = (MEMFILE*) stream;

   if ((handle->position + size) > handle->length)
   {
      handle->length = handle->position + size;
      handle->buffer = realloc(handle->buffer, handle->length);
   }

   memcpy(((char*)handle->buffer) + handle->position, buf, size);
   handle->position+=size;

   return size;
}

static ZPOS_T ZCALLBACK mem_tell (opaque, stream)
   voidpf opaque;
   voidpf stream;
{
   MEMFILE *handle = (MEMFILE *)stream;
   return handle->position;
}

static long ZCALLBACK mem_seek (opaque, stream, offset, origin)
   voidpf opaque;
   voidpf stream;
   ZOFF_T offset;
   int origin;
{
   MEMFILE* handle = (MEMFILE*)stream;
   return fseek_calc(offset, origin, &handle->position, handle->length);
}

int ZCALLBACK mem_close (opaque, stream)
   voidpf opaque;
   voidpf stream;
{
    MEMFILE *handle = (MEMFILE *)stream;

    /* Note that once we've written to the buffer we don't tell anyone
       about it here. Probably the opaque handle could be used to inform
       some other component of how much data was written.

       This, and other aspects of writing through this interface, has
       not been tested.
     */

    free (handle);
    return 0;
}

int ZCALLBACK mem_error (opaque, stream)
   voidpf opaque;
   voidpf stream;
{
    MEMFILE *handle = (MEMFILE *)stream;
    /* We never return errors */
    return 0;
}

ZEXTERN void* ZEXPORT mem_simple_create_file(zlib_filefunc_def* api, void* buffer, size_t buf_len)
{
    MEMFILE* handle = malloc(sizeof(*handle));
    api->zopen_file  = NULL;
    api->zread_file  = mem_read;
    api->zwrite_file = mem_write;
    api->ztell_file  = mem_tell;
    api->zseek_file  = mem_seek;
    api->zclose_file = mem_close;
    api->zerror_file = mem_error;
    api->opaque      = handle;
    handle->position = 0;
    handle->buffer   = buffer;
    handle->length   = buf_len;
    return handle;
}
