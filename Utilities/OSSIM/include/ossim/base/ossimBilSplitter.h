//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimBilSplitter.
// 
// Utility class for splitting an image that is band interleaved by line into
// separate files.
//*******************************************************************
//  $Id: ossimBilSplitter.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimBilSplitter_HEADER
#define ossimBilSplitter_HEADER

#include <fstream>
using namespace std;

#include <ossim/base/ossimConstants.h>

class OSSIMDLLEXPORT ossimBilSplitter
{
public:
   ossimBilSplitter(const char* file_to_split,
                    ossim_uint32      header_size_in_bytes,
                    ossim_uint32      bytes_per_pixel,
                    ossim_uint32      samples_per_line,
                    ossim_uint32      number_of_channels);

   ~ossimBilSplitter();

   enum ossimStatus
   {
      OSSIM_OK    = 0,
      OSSIM_ERROR = 1
   };

   /*!
    *  Splits source image into separate bands and writes to output
    *  directory as channel_1.ras, channel_2.ras, ...
    *  Returns true on success, false on error.
    */
   bool output_multi_files(const char* output_dir);

   int errorStatus() const { return theErrorStatus; }

private:

   ifstream      theSourceFileStr;
   ossim_uint32  theHeaderSizeInBytes;
   ossim_uint32  theBytesPerLine;
   ossim_uint32  theLinesToWrite;
   ossim_uint32  theNumberOfChannels;
   ossimStatus   theErrorStatus;
};

#endif
