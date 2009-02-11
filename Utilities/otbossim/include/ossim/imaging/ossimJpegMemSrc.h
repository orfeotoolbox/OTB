//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken, original code from Thomas G. Lane
//
// Description:
// Code interfaces to use with jpeg-6b library to read a jpeg image from
// memory.
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimJpegMemSrc_HEADER
#define ossimJpegMemSrc_HEADER

#include <ossim/base/ossimConstants.h> /** for OSSIM_DLL export macro */
extern "C"
{
#include <cstdio>                      /** for size_t  */
#include <csetjmp>                     /** for jmp_buf */
#include <jpeglib.h>                   /** for jpeg stuff */


/** @brief Extended error handler struct. */
struct OSSIM_DLL ossimJpegErrorMgr
{
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct ossimJpegErrorMgr* ossimJpegErrorPtr;

/**
 * @brief Error routine that will replace jpeg's standard error_exit method.
 */
OSSIM_DLL void ossimJpegErrorExit (j_common_ptr cinfo);

/**
 * @brief Method which uses memory instead of a FILE* to read from.
 * @note Used in place of "jpeg_stdio_src(&cinfo, infile)".
 */
OSSIM_DLL void ossimJpegMemorySrc (j_decompress_ptr cinfo,
                                   const JOCTET * buffer,
                                   std::size_t bufsize);
}
#endif /* #ifndef ossimJpegMemSrc_HEADER */
