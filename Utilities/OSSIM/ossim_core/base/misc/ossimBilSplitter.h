//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for ossimBilSplitter.
// 
// Utility class for splitting an image that is band interleaved by line into
// separate files.
//*******************************************************************
//  $Id: ossimBilSplitter.h,v 1.3 2003/05/13 11:31:03 dburken Exp $

#ifndef ossimBilSplitter_HEADER
#define ossimBilSplitter_HEADER

#include <fstream>
using namespace std;

#include "base/common/ossimConstants.h"

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
