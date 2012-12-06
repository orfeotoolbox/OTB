//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer, original code from Thomas G. Lane
//
// Most of code and comments below are from jpeg-6b "example.c" file. See 
// http://www4.cs.fau.de/Services/Doc/graphics/doc/jpeg/libjpeg.html
//
// Description:
// Code interfaces to use with jpeg-6b library to write a jpeg image from memory.
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimJpegMemSrc_HEADER
#define ossimJpegMemSrc_HEADER

#include <fstream>
#include <ossim/base/ossimConstants.h> /** for OSSIM_DLL export macro */
#include <cstdio>                      /** for size_t  */
#include <csetjmp>                     /** for jmp_buf */
#include <jpeglib.h>                   /** for jpeg stuff */

/**
 * @brief Method which uses memory instead of a FILE* to write to.
 * @note Used in place of "jpeg_stdio_dest(&cinfo, outfile)".
 */
OSSIM_DLL void jpeg_cpp_stream_dest (j_compress_ptr cinfo, std::ostream& stream);

#endif 
