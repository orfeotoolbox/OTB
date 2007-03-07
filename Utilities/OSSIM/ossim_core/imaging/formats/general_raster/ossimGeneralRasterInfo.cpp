//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
//*******************************************************************
//  $Id: ossimGeneralRasterInfo.cpp,v 1.26 2005/06/15 19:44:03 gpotts Exp $

#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include <imaging/formats/general_raster/ossimGeneralRasterInfo.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimString.h>
#include <base/misc/lookup_tables/ossimInterleaveTypeLut.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>

static const ossimKeyword
NUMBER_LINES("number_lines",
             "Number of lines in the raster image.");

static const ossimKeyword
NUMBER_SAMPLES("number_samples",
              "Number of samples in the raster image.");

static const ossimKeyword
VALID_START_LINE("valid_start_line",
                 "First valid line of raster image(zero based).");

static const ossimKeyword
VALID_STOP_LINE("valid_stop_line",
                "Last valid line of raster image(zero based).");

static const ossimKeyword
VALID_START_SAMPLE("valid_start_sample",
                   "First valid sample of raster image(zero based).");

static const ossimKeyword
VALID_STOP_SAMPLE("valid_stop_sample",
                  "Last valid sample of raster image(zero based).");

static const ossimKeyword
SUB_IMAGE_OFFSET_LINE(
   "sub_image_offset_line",
   "Pixel line offset of sub-image in the full-image pixel space.");

static const ossimKeyword
SUB_IMAGE_OFFSET_SAMP(
   "sub_image_offset_samp",
   "Pixel sample offset of sub-image in the full-image pixel space."); 

static const ossimKeyword
HEADER_SIZE("header_size",
            "Header size in bytes.");

static const ossimKeyword
SET_NULLS("set_fill_to_nulls_mode",
          "0 = do nothing to pixels,\n1 = all zeroes to min values,\
\n2 = zeroes to null on edges only.");

static const ossimKeyword
PIXELS_TO_CHOP("pixels_to_chop",
               "Ammount of pixels to chop from edge.");

static const ossimInterleaveTypeLut INTERLEAVE_TYPE_LUT;

static ossimTrace traceDebug("ossimGeneralRasterInfo:debug");

ossimGeneralRasterInfo::ossimGeneralRasterInfo()
   :
      theImageFileList(),
      theInterleaveType(OSSIM_BIL),
      theRawImageRect(0,0,0,0),
      theValidImageRect(0,0,0,0),
      theImageRect(0,0,0,0),
      theSubImageOffset(0,0),
      theHeaderSize(0),
      theSetNullsMode(NONE),
      thePixelsToChop(0),
      theImageDataByteOrder(OSSIM_LITTLE_ENDIAN)
{}

ossimGeneralRasterInfo::ossimGeneralRasterInfo(const vector<ossimFilename>& imageFileList,
                                               ossimScalarType     pixelType,
                                               ossimInterleaveType il_type,
                                               ossim_uint32        numberOfBands,
                                               ossim_uint32        lines,
                                               ossim_uint32        samples,
                                               ossim_uint32        headerSize,
                                               ossimFillMode       nullsMode,
                                               ossim_uint32        pixelsToChop)
   :
  theMetaData(pixelType,
	      numberOfBands),
      theImageFileList(imageFileList),
      theInterleaveType(il_type),
      theRawImageRect(0,0,0,0),
      theValidImageRect(0,0,0,0),
      theImageRect(0,0,0,0),
      theSubImageOffset(0,0),
      theHeaderSize(headerSize),
      theSetNullsMode(nullsMode),
      thePixelsToChop(pixelsToChop),
      theImageDataByteOrder(OSSIM_LITTLE_ENDIAN)
{
   theRawImageRect.set_lry(lines - 1);
   theRawImageRect.set_lrx(samples - 1);
   theValidImageRect = theRawImageRect;
   theImageRect = theRawImageRect;
}

ossimGeneralRasterInfo::ossimGeneralRasterInfo(const ossimKeywordlist& kwl,
                     const char* prefix)
   :
      theImageFileList(),
      theInterleaveType(OSSIM_BIL),
      theRawImageRect(0,0,0,0),
      theValidImageRect(0,0,0,0),
      theImageRect(0,0,0,0),
      theSubImageOffset(0,0),
      theHeaderSize(0),
      theSetNullsMode(NONE),
      thePixelsToChop(0),
      theImageDataByteOrder(OSSIM_LITTLE_ENDIAN)
{
   loadState(kwl, prefix);
}

ossimGeneralRasterInfo::~ossimGeneralRasterInfo()
{
}
  
std::ostream& ossimGeneralRasterInfo::print(std::ostream& out) const
{
   //***
   // This will print in a keyword format that can be read by the constructor.
   // that takes a keyword list.
   //***
   out << setiosflags(ios::left);
   ossim_uint32 i;
   for (i=0; i<theImageFileList.size(); ++i)
   {
      out << ossimKeywordNames::FILENAME_KW << (i+1) << ":  "
          << theImageFileList[i].c_str() << endl;
   }
            
   out  << ossimScalarTypeLut::instance()->getKeyword().key() << ":  "
        << ossimScalarTypeLut::instance()->getEntryString(getScalarType())
        << "\n" << INTERLEAVE_TYPE_LUT.getKeyword().key() << ":  "
        << INTERLEAVE_TYPE_LUT.getEntryString(theInterleaveType)
        << "\n" << ossimKeywordNames::NUMBER_BANDS_KW << ":  "
        << numberOfBands()
        << "\n" << NUMBER_LINES.key() << ":  "
        << rawLines()
        << "\n" << NUMBER_SAMPLES.key() << ":  "
        << rawSamples()
        << "\n" << VALID_START_LINE.key() << ":  "
        << theValidImageRect.ul().y
        << "\n" << VALID_STOP_LINE.key() << ":  "
        << theValidImageRect.lr().y
        << "\n" << VALID_START_SAMPLE.key() << ":  "
        << theValidImageRect.ul().x
        << "\n" << VALID_STOP_SAMPLE.key() << ":  "
        << theValidImageRect.lr().x
        << "\n" << SUB_IMAGE_OFFSET_LINE.key() << ":  "
        << theSubImageOffset.line
        << "\n" << SUB_IMAGE_OFFSET_SAMP.key() << ":  "
        << theSubImageOffset.samp
        << "\n" << HEADER_SIZE.key() << ":  "
        << theHeaderSize
        << "\n" << SET_NULLS.key() << ":  "
        << (ossim_uint32)theSetNullsMode
        << "\n" << PIXELS_TO_CHOP.key() << ":  "
        << thePixelsToChop
        << endl;
   
   if (bytesPerPixel() > 1)
   {
      out << ossimKeywordNames::BYTE_ORDER_KW << ":  "
          << (theImageDataByteOrder == OSSIM_LITTLE_ENDIAN ? "little_endian" :
              "big_endian")
          << endl;
   }
   for (i=0; i<numberOfBands(); ++i)
   {
      out << "band" << i+1 << ".min_value: "  << theMetaData.getMinPix(i)
          << "\nband" << i+1 << ".max_value: "  << theMetaData.getMaxPix(i)
          << "\nband" << i+1 << ".null_value: " << theMetaData.getNullPix(i)
          << endl;
   }
   
   return ossimErrorStatusInterface::print(out);
}

void ossimGeneralRasterInfo::setFillToNullsMode(ossim_uint32 mode)
{
   static const char MODULE[] = "ossimGeneralRasterInfo::setFillToNullMode";
   if(mode < 3)
   {
      theSetNullsMode = (ossimFillMode)mode;
   }
   else
   {
      cerr << MODULE << " ERROR:"
           << "\nmode out of bounds(0 - 2):  " << mode << endl
           << "\nmode has not been changed." << endl;
   }
}

ossim_uint32 ossimGeneralRasterInfo::bytesPerPixel(ossimScalarType type) const
{
   ossim_uint32 bpp;
   
   switch (type)
   {
   case OSSIM_UCHAR:
      bpp = 1;
      break;
   case OSSIM_USHORT16:
   case OSSIM_SSHORT16:
   case OSSIM_USHORT11:
      bpp = 2;
      break;
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
      bpp = 4;
      break;
   case OSSIM_NORMALIZED_DOUBLE:
   case OSSIM_DOUBLE:
      bpp = 8;
      break;
   default:
      bpp = 0;
      break;
   }

   return bpp;
}

void ossimGeneralRasterInfo::setImageDataByteOrder(ossimByteOrder byteOrder)
{
   theImageDataByteOrder = byteOrder;
}

bool ossimGeneralRasterInfo::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   for (ossim_uint32 i=0; i<theImageFileList.size(); ++i)
   {
      ossimString kw = ossimKeywordNames::FILENAME_KW;
      kw += ossimString::toString(i+1);
      kwl.add(prefix, theImageFileList[i].c_str());
   }

   theMetaData.saveState(kwl, prefix);

   kwl.add(prefix,
           NUMBER_LINES,
           rawLines(),
           true);
   kwl.add(prefix,
           NUMBER_SAMPLES,
           rawSamples(),
           true);
   kwl.add(prefix,
           HEADER_SIZE,
           theHeaderSize,
           true);
   kwl.add(prefix,
           SUB_IMAGE_OFFSET_LINE,
           theSubImageOffset.line,
           true);
   kwl.add(prefix,
           SUB_IMAGE_OFFSET_SAMP,
           theSubImageOffset.samp,
           true);
   kwl.add(prefix,
           VALID_START_LINE,
           theValidImageRect.ul().y,
           true);
   kwl.add(prefix,
           VALID_STOP_LINE,
           theValidImageRect.lr().y,
           true);
   kwl.add(prefix,
           VALID_START_SAMPLE,
           theValidImageRect.ul().x,
           true);
   kwl.add(prefix,
           VALID_STOP_SAMPLE,
           theValidImageRect.ur().x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::INTERLEAVE_TYPE_KW,
           INTERLEAVE_TYPE_LUT.getEntryString(theInterleaveType),
           true);

   kwl.add(prefix,
           PIXELS_TO_CHOP,
           thePixelsToChop,
           true);

   kwl.add(prefix,
           SET_NULLS,
           theSetNullsMode,
           true);


   if (bytesPerPixel() > 1)
   {
      kwl.add(prefix,
              ossimKeywordNames::BYTE_ORDER_KW,
              (theImageDataByteOrder == OSSIM_LITTLE_ENDIAN ? "little_endian" :
               "big_endian"),
              true);
   }

   return true;
}

bool ossimGeneralRasterInfo::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "ossimGeneralRasterInfo::loadState";
   // Check for errors in the ossimKeywordlist.
   if(kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      cerr << MODULE << " ERROR:\n"
           << "Detected an error in the keywordlist:  " << kwl
           << endl;
      return false;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nprefix:  " << (prefix ? prefix : "")
           << "\nInput keyword list\n"
           << kwl
           << endl;
   }

   const char* kwlLookup = 0; // Use for keyword list lookups.

   if(!theMetaData.loadState(kwl, prefix))
     {
       return false;
     }

   int tmp = INTERLEAVE_TYPE_LUT.getEntryNumber(kwl);
   if (tmp == ossimLookUpTable::NOT_FOUND)
   {
      theInterleaveType = OSSIM_BIL;
   }
   else
   {
      theInterleaveType = static_cast<ossimInterleaveType>(tmp);
   }
   
    // Get the image files.
   
   theImageFileList.clear(); // Start with a clean list.
   
   if (theInterleaveType != OSSIM_BSQ_MULTI_FILE)
   {
      // Look for "filename" first, then deprecated "image_file".      
      kwlLookup = kwl.find(ossimKeywordNames::FILENAME_KW);
      if (!kwlLookup)
      {
         // deprecated keyword...
         kwlLookup = kwl.find(ossimKeywordNames::IMAGE_FILE_KW);
      }
      
      if (kwlLookup)
      {
         theImageFileList.push_back(ossimFilename(kwlLookup));
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         if (traceDebug())
         {
            cerr << "ERROR:\n"
                 << "Required keyword not found:  "
                 << ossimKeywordNames::FILENAME_KW << endl;
         }
         return false;
      }
   }
   else
   {
      // multiple file names.
      ossim_uint32 count = 0;

      // look for image file key word with no number.
      // Required to have this.
      kwlLookup = kwl.find(ossimKeywordNames::FILENAME_KW);
      if (!kwlLookup)
      {
         // deprecated keyword...
         kwlLookup = kwl.find(ossimKeywordNames::IMAGE_FILE_KW);
      }
      
      if (kwlLookup)
      {
         theImageFileList.push_back(ossimFilename(kwlLookup));
         ++count;
      }
      
      ossim_int32 i = 0;
      while ( (count < numberOfBands()) && (i < 1000) )
      {
         ossimString kw = ossimKeywordNames::FILENAME_KW;
         kw += ossimString::toString(i);
         kwlLookup = kwl.find(kw);
         if (!kwlLookup)
         {
            // Lookup for deprecated keyword.
            kw = ossimKeywordNames::IMAGE_FILE_KW;
            kw += ossimString::toString(i);
            kwlLookup = kwl.find(kw);
         }
         
         if (kwlLookup)
         {
            theImageFileList.push_back(ossimFilename(kwlLookup));
            ++count;
         }
         ++i;
      }

      if (count != numberOfBands())  // Error, count should equal bands!
      {
         if (traceDebug())
         {
            cerr << " ERROR:\n"
                 << "Required keyword not found:  "
                 << ossimKeywordNames::FILENAME_KW
                 << "\nInterleave type is multi file; however,"
                 << " not enough were pick up!"  << endl;
         }
         return false; 
      }
   }

   kwlLookup = kwl.find(NUMBER_LINES);  // Required to have this.
   if (kwlLookup)
   {
      theRawImageRect.set_lry(atoi(kwlLookup) - 1);
   }
   else
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceDebug())
      {
         cerr << " ERROR:\n"
              << "Required keyword not found:  " << NUMBER_LINES << endl;
      }
      return false;
   }

   kwlLookup = kwl.find(NUMBER_SAMPLES);  // Required to have this.
   if (kwlLookup)
   {
      theRawImageRect.set_lrx(atoi(kwlLookup) - 1);
   }
   else
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceDebug())
      {
         cerr << " ERROR:\n"
              << "Required keyword not found:  " << NUMBER_SAMPLES << endl;
      }
      return false;
   }

   kwlLookup = kwl.find(VALID_START_LINE); // Default is zero.
   if (kwlLookup)
   {
      theValidImageRect.set_uly(atoi(kwlLookup));
   }

   kwlLookup = kwl.find(VALID_STOP_LINE); // Default is last line.
   if (kwlLookup)
   {
      theValidImageRect.set_lry(atoi(kwlLookup));
   }
   else
   {
      theValidImageRect.set_lry(theRawImageRect.lr().y);
   }

   if (theValidImageRect.lr().y < theValidImageRect.ul().y)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      cerr << " ERROR:"
           << "\nValid stop line < start line."
           << "\nValid start line:  " << theValidImageRect.ul().y
           << "\nValid stop line:   " << theValidImageRect.lr().y
           << "\nError status has been set.  Returning." << endl;
      return false;
   }

   kwlLookup = kwl.find(VALID_START_SAMPLE); // Default is zero.
   if (kwlLookup)
   {
     theValidImageRect.set_ulx(atoi(kwlLookup));
   }

   kwlLookup = kwl.find(VALID_STOP_SAMPLE); // Default is last sample.
   if (kwlLookup) 
   {
      theValidImageRect.set_lrx(atoi(kwlLookup));
   }
   else
   {
      theValidImageRect.set_lrx(theRawImageRect.lr().x); 
   }

   if (theValidImageRect.lr().x < theValidImageRect.ul().x)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      cerr << " ERROR:"
           << "\nValid stop samp < start samp."
           << "\nValid start samp:  " << theValidImageRect.ul().x
           << "\nValid stop samp:   " << theValidImageRect.lr().x
           << "\nError status has been set.  Returning." << endl;
      return false;
   }

   theImageRect.set_lry(theValidImageRect.lr().y -
                        theValidImageRect.ul().y);
   theImageRect.set_lrx(theValidImageRect.lr().x -
                        theValidImageRect.ul().x);

   kwlLookup = kwl.find(SUB_IMAGE_OFFSET_LINE); // Default is zero.
   if (kwlLookup)
      theSubImageOffset.line = atoi(kwlLookup);

   kwlLookup = kwl.find(SUB_IMAGE_OFFSET_SAMP); // Default is zero.
   if (kwlLookup)
      theSubImageOffset.samp = atoi(kwlLookup);
      
   kwlLookup = kwl.find(HEADER_SIZE); // Default is zero.
   if (kwlLookup)
   {
      theHeaderSize = atoi(kwlLookup);
   }

   kwlLookup = kwl.find(SET_NULLS); // Default is 2.
   if (kwlLookup)
   {
      int tmp;
      tmp = atoi(kwlLookup);
      if ((tmp < 3) && (tmp > -1))
      {
         theSetNullsMode = (ossimFillMode)tmp;
      }
      else
      {
         theSetNullsMode = ZEROES_TO_NULL_EDGES_ONLY;  // 2
         ossimNotify(ossimNotifyLevel_WARN)
            << " WARNING:"
            << "\nset_fill_to_nulls_mode value out of range."
            << "\nDefaulted to 2" << endl;
      }
   }

   kwlLookup = kwl.find(PIXELS_TO_CHOP); // Default is zero.
   if (kwlLookup)
   {
      thePixelsToChop = atoi(kwlLookup);
   }

   if (bytesPerPixel() > 1)
   {
      // get the byte order of the data.
      kwlLookup = kwl.find(ossimKeywordNames::BYTE_ORDER_KW);
      if (kwlLookup)
      {
         ossimString s(kwlLookup);
         if (s.trim() != "")  // Check for empty string.
         {
            s.downcase();
            if (s.contains("big"))
            {
               theImageDataByteOrder = OSSIM_BIG_ENDIAN;
            }
            else if(s.contains("little"))
            {
               theImageDataByteOrder = OSSIM_LITTLE_ENDIAN;
            }
         }
      }
   }
   if (traceDebug())
   {
     CLOG << "Exiting....." << endl;
   }
   return true;
}
