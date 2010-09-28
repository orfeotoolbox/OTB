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
#ifndef ossimJpegStdIOSrc_HEADER
#define ossimJpegStdIOSrc_HEADER

#include <ossim/base/ossimConstants.h> /** for OSSIM_DLL export macro */
extern "C"
{
#include <cstdio>                      /** for size_t  */
#include <csetjmp>                     /** for jmp_buf */
#include <jpeglib.h>                   /** for jpeg stuff */
   //#include <jinclude.h>
   //#include <jerror.h>
   /**
    * @brief Method which uses memory instead of a FILE* to read from.
    * @note Used in place of "jpeg_stdio_src(&cinfo, infile)".
    */
   OSSIM_DLL void ossimJpegStdIOSrc ( j_decompress_ptr cinfo,
                                     FILE* infile);
}
#endif /* #ifndef ossimJpegMemSrc_HEADER */
