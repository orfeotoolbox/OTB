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
// Contains class definition for ossimGeneralRasterInfo
//*******************************************************************
// $Id: ossimGeneralRasterInfo.cpp 21745 2012-09-16 15:21:53Z dburken $

#include <ossim/imaging/ossimGeneralRasterInfo.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/support_data/ossimEnviHeader.h>
#include <ossim/support_data/ossimFgdcXmlDoc.h>

#include <iostream>
#include <iomanip>

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
   theMetaData(),
   theImageFileList(),
   theInterleaveType(OSSIM_BIL),
   theRawImageRect(),
   theValidImageRect(),
   theImageRect(),
   theSubImageOffset(0,0),
   theHeaderSize(0),
   theSetNullsMode(NONE),
   thePixelsToChop(0),
   theImageDataByteOrder(OSSIM_LITTLE_ENDIAN)
{
   theRawImageRect.makeNan();
   theValidImageRect.makeNan();
   theImageRect.makeNan();
}

ossimGeneralRasterInfo::ossimGeneralRasterInfo(const std::vector<ossimFilename>& imageFileList,
                                               ossimScalarType     pixelType,
                                               ossimInterleaveType il_type,
                                               ossim_int32         numberOfBands,
                                               ossim_int32         lines,
                                               ossim_int32         samples,
                                               ossim_int32         headerSize,
                                               ossimFillMode       nullsMode,
                                               ossim_int32         pixelsToChop)
   :
   theMetaData(pixelType, numberOfBands),
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
   theRawImageRect.makeNan();
   theValidImageRect.makeNan();
   theImageRect.makeNan();
   
   loadState(kwl, prefix);
}

ossimGeneralRasterInfo::ossimGeneralRasterInfo( const ossimGeneralRasterInfo& obj )
   :
   theMetaData           ( obj.theMetaData ),
   theImageFileList      ( obj.theImageFileList ),
   theInterleaveType     ( obj.theInterleaveType ),
   theRawImageRect       ( obj.theRawImageRect ),
   theValidImageRect     ( obj.theValidImageRect ),
   theImageRect          ( obj.theImageRect ),
   theSubImageOffset     ( obj.theSubImageOffset ),
   theHeaderSize         ( obj.theHeaderSize ),
   theSetNullsMode       ( obj.theSetNullsMode ),
   thePixelsToChop       ( obj.thePixelsToChop ),
   theImageDataByteOrder ( obj.theImageDataByteOrder )
{
}

const ossimGeneralRasterInfo& ossimGeneralRasterInfo::operator=(
   const ossimGeneralRasterInfo& rhs )
{
   if ( this != &rhs )
   {
      theMetaData           = rhs.theMetaData;
      theImageFileList      = rhs.theImageFileList;
      theInterleaveType     = rhs.theInterleaveType;
      theRawImageRect       = rhs.theRawImageRect;
      theValidImageRect     = rhs.theValidImageRect;
      theImageRect          = rhs.theImageRect;
      theSubImageOffset     = rhs.theSubImageOffset;
      theHeaderSize         = rhs.theHeaderSize;
      theSetNullsMode       = rhs.theSetNullsMode;
      thePixelsToChop       = rhs.thePixelsToChop;
      theImageDataByteOrder = rhs.theImageDataByteOrder;
   }
   return *this;
}

ossimGeneralRasterInfo::~ossimGeneralRasterInfo()
{
}

void ossimGeneralRasterInfo::clear()
{
   theMetaData.clear();
   theImageFileList.clear();
   theInterleaveType = OSSIM_BIL;
   theRawImageRect.makeNan();
   theValidImageRect.makeNan();
   theImageRect.makeNan();
   theSubImageOffset.x = 0;
   theSubImageOffset.y = 0;
   theHeaderSize = 0;
   theSetNullsMode = NONE;
   thePixelsToChop = 0;
   theImageDataByteOrder = OSSIM_LITTLE_ENDIAN;
}

const ossimIrect& ossimGeneralRasterInfo::imageRect() const
{
   return theImageRect;
}

const ossimIrect& ossimGeneralRasterInfo::validImageRect() const
{
   return theValidImageRect;
}

const ossimIrect& ossimGeneralRasterInfo::rawImageRect() const
{
   return theRawImageRect;
}

const ossimIpt& ossimGeneralRasterInfo::subImageOffset() const
{
   return theSubImageOffset;
}

ossim_int32 ossimGeneralRasterInfo::headerSize() const
{
   return theHeaderSize;
}

ossim_uint32 ossimGeneralRasterInfo::fillToNullsMode() const
{
   return theSetNullsMode;
}
  
std::ostream& ossimGeneralRasterInfo::print(std::ostream& out) const
{
   //---
   // This will print in a keyword format that can be read by the constructor.
   // that takes a keyword list.
   //---
   ossimKeywordlist kwl;
   saveState( kwl, 0 );
   out << kwl << std::endl;
   return out;
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
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nmode out of bounds(0 - 2):  " << mode << std::endl
         << "\nmode has not been changed." << std::endl;
   }
}

ossim_int32 ossimGeneralRasterInfo::pixelsToChop() const
{
   return thePixelsToChop;
} 

ossimInterleaveType ossimGeneralRasterInfo::interleaveType() const
{
   return theInterleaveType;
}

const std::vector<ossimFilename>& ossimGeneralRasterInfo::getImageFileList() const
{
   return theImageFileList;
}

void ossimGeneralRasterInfo::setImageFileList(const std::vector<ossimFilename>& list)
{
   theImageFileList = list;
}

void ossimGeneralRasterInfo::setImageFile(const ossimFilename& file)
{
   theImageFileList.clear();
   theImageFileList.push_back( file );
}   

void ossimGeneralRasterInfo::setImageDataByteOrder(ossimByteOrder byteOrder)
{
   theImageDataByteOrder = byteOrder;
}

void ossimGeneralRasterInfo::setHeaderSize(ossim_int32 headerSize)
{
   theHeaderSize = headerSize;
}

void ossimGeneralRasterInfo::setInterleaveType(ossimInterleaveType il_type)
{
   theInterleaveType = il_type;
}

void ossimGeneralRasterInfo::setImageRect(const ossimIrect& imageRect)
{
   theImageRect = imageRect;
}

void ossimGeneralRasterInfo::setValidImageRect(const ossimIrect &imageRect)
{
   theValidImageRect = imageRect;
}

void ossimGeneralRasterInfo::setRawImageRect(const ossimIrect &imageRect)
{
   theRawImageRect = imageRect;
}

void ossimGeneralRasterInfo::setSubImageOffset(const ossimIpt& d)
{
   theSubImageOffset = d;
}

ossimByteOrder ossimGeneralRasterInfo::getImageDataByteOrder() const
{
   return theImageDataByteOrder;
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
           ossimString::toString( rawLines() ), true);
   kwl.add(prefix,
           NUMBER_SAMPLES,
           ossimString::toString( rawSamples() ),
           true);
   kwl.add(prefix,
           HEADER_SIZE,
           ossimString::toString(theHeaderSize),
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
   if ( traceDebug() )
   {
      CLOG << "DEBUG: entered..."
           << "\nprefix:  " << (prefix ? prefix : "")
           << "\nInput keyword list:\n"
           << kwl
           << std::endl;
   }   

   bool result = false;

   //---
   // Look for required and option keyword.  Break from loop if required
   // keyword is not found.
   //---
   while( 1 )
   {
      // Check for errors in the ossimKeywordlist.
      if(kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << "Detected an error in the keywordlist:  " << kwl
            << std::endl;
         break;
      }

      std::string key;
      ossimString value; // Use for keyword list lookups.
      ossim_int32 lines = 0;
      ossim_int32 samples = 0;

      // Lines (required):
      key = NUMBER_LINES;
      value.string() = kwl.findKey( key );  // Required to have this.
      if ( value.size() )
      {
         lines = value.toInt32();
         if ( !lines )
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << " ERROR:\n"
                  << "Required number of lines is 0!" << std::endl;
            }
            break;
         } 
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << " ERROR:\n"
               << "Required keyword not found:  " << key << std::endl;
         }
         break;
      }

      // Samples (required):
      key = NUMBER_SAMPLES;
      value.string() = kwl.findKey( key );  // Required to have this.
      if ( value.size() )
      {
         samples = value.toInt32();
         if ( !samples )
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << " ERROR:\n"
                  << "Required number of samples is 0!" << std::endl;
            }
            break;
         }          
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << " ERROR:\n"
               << "Required keyword not found:  " << key << std::endl;
         }
         break;
      }
      
      // Bands ossimImageMetaData::loadState checks for required bands:
      if(!theMetaData.loadState(kwl, prefix))
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << " Error loading meta data!\n" << std::endl;
         }
         break;
      }

      // If we get here assign the rectangles:
      theRawImageRect   = ossimIrect( 0, 0, samples - 1, lines - 1 );
      theValidImageRect = theRawImageRect;
      theImageRect      = theRawImageRect;

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
      if (theInterleaveType != OSSIM_BSQ_MULTI_FILE)
      {
         // Look for "filename" first, then deprecated "image_file".
         key = ossimKeywordNames::FILENAME_KW;
         value.string() = kwl.findKey( key );
         if ( value.empty() )
         {
            // deprecated keyword...
            key = ossimKeywordNames::IMAGE_FILE_KW;
            value.string() = kwl.findKey( key );
         }
         
         if ( value.size() )
         {
            //---
            // omd (ossim metadata) files can have just the base filename, e.g. image.ras,
            // in which case open will fail if not in the image dir.  So only put it in
            // the list if it doesn't exits.
            //---
            ossimFilename f = value;
            if ( f.exists() )
            {
               theImageFileList.clear();
               theImageFileList.push_back(ossimFilename(value));
            }
         }

         if ( theImageFileList.empty() )
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ERROR:\n"
                  << "Required keyword not found:  "
                  << ossimKeywordNames::FILENAME_KW << std::endl;
            }
            break;
         }
      }
      else
      {
         // multiple file names.
         ossim_int32 count = 0;
         
         // look for image file key word with no number.
         // Required to have this.
         key = ossimKeywordNames::FILENAME_KW;
         value.string() = kwl.findKey( key );
         if ( value.empty() )
         {
            // deprecated keyword...
            key = ossimKeywordNames::IMAGE_FILE_KW;
            value.string() = kwl.findKey( key );
         }
         
         if ( value.size() )
         {
            theImageFileList.push_back(ossimFilename(value));
            ++count;
         }
         
         ossim_int32 i = 0;
         while ( (count < numberOfBands()) && (i < 1000) )
         {
            key = ossimKeywordNames::FILENAME_KW;
            key += ossimString::toString(i).string();
            value.string() = kwl.findKey( key );
            if ( value.empty() )
            {
               // Lookup for deprecated keyword.
               key = ossimKeywordNames::IMAGE_FILE_KW;
               key += ossimString::toString(i).string();
               value.string() = kwl.findKey( key );
            }
            
            if ( value.size() )
            {
               theImageFileList.push_back(ossimFilename(value));
               ++count;
            }
            ++i;
         }
         
         if (count != numberOfBands())  // Error, count should equal bands!
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << " ERROR:\n"
                  << "Required keyword not found:  "
                  << ossimKeywordNames::FILENAME_KW
                  << "\nInterleave type is multi file; however,"
                  << " not enough were pick up!"  << std::endl;
            }
            break;
         }
      }

      key = VALID_START_LINE;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         theValidImageRect.set_uly( value.toInt32() );
      }

      key = VALID_STOP_LINE;
      value.string() = kwl.findKey( key ); // Default is last line.
      if ( value.size() )
      {
         theValidImageRect.set_lry( value.toInt32() );
      }
      
      if (theValidImageRect.lr().y < theValidImageRect.ul().y)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << " ERROR:"
            << "\nValid stop line < start line."
            << "\nValid start line:  " << theValidImageRect.ul().y
            << "\nValid stop line:   " << theValidImageRect.lr().y
            << "\nError status has been set.  Returning." << std::endl;
         break;
      }

      key = VALID_START_SAMPLE;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         theValidImageRect.set_ulx( value.toInt32() );
      }

      key = VALID_STOP_SAMPLE;
      value.string() = kwl.findKey( key ); // Default is last sample.
      if ( value.size() ) 
      {
         theValidImageRect.set_lrx( value.toInt32() );
      }
      
      if (theValidImageRect.lr().x < theValidImageRect.ul().x)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << " ERROR:"
            << "\nValid stop samp < start samp."
            << "\nValid start samp:  " << theValidImageRect.ul().x
            << "\nValid stop samp:   " << theValidImageRect.lr().x
            << "\nError status has been set.  Returning." << std::endl;
         break;
      }
      
      theImageRect.set_lry(theValidImageRect.lr().y -
                           theValidImageRect.ul().y);
      theImageRect.set_lrx(theValidImageRect.lr().x -
                           theValidImageRect.ul().x);
      
      key = SUB_IMAGE_OFFSET_LINE;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         theSubImageOffset.line = value.toInt32();
      }

      key = SUB_IMAGE_OFFSET_SAMP;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         theSubImageOffset.samp = atoi(value);
      }

      key = HEADER_SIZE;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         theHeaderSize = value.toInt32();
      }

      key = SET_NULLS;
      value.string() = kwl.findKey( key ); // Default is 2.
      if ( value.size() )
      {
         int tmp;
         tmp = atoi(value);
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
               << "\nDefaulted to 2" << std::endl;
         }
      }

      key = PIXELS_TO_CHOP;
      value.string() = kwl.findKey( key ); // Default is zero.
      if ( value.size() )
      {
         thePixelsToChop = value.toInt32();
      }
      
      if (bytesPerPixel() > 1)
      {
         // get the byte order of the data.
         key = ossimKeywordNames::BYTE_ORDER_KW;
         value.string() = kwl.findKey( key );
         if ( value.size() )
         {
            ossimString s(value);
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

      // End of while forever loop.
      result = true;
      break;

   } // Matches: while (1)

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE  << " Exit status: " << (result?"true":"false") << std::endl;
   }      
   
   return result;
   
} // End: bool ossimGeneralRasterInfo::loadState

bool ossimGeneralRasterInfo::open( const ossimFilename& imageFile )
{
   static const char MODULE[] = "ossimGeneralRasterInfo::open";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..." << "\nimageFile: " << imageFile << std::endl;
   }

   bool result = false;

   // Wipe any previous state slick.
   clear();

   ossimFilename copyFile = imageFile;
   if ( !imageFile.exists() )
   {
      copyFile = imageFile.expand();
   }

   // Look for the headrer of omd file as they are written out by img2rr.
   ossimFilename hdr = copyFile;
   hdr.setExtension("hdr"); // image.hdr
   if ( !hdr.exists() )
   {
      hdr = imageFile;
      hdr.string() += ".hdr"; // image.ras.hdr
      if ( ! hdr.exists() )
      {
         hdr = imageFile;
         hdr.setExtension("xml"); // image.xml
      }
   }

   if ( hdr.exists() )
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "header file: " << hdr << std::endl;
      }
      
      ossimString ext = hdr.ext().downcase();
      
      if ( ext == "hdr" )
      {
         if ( ossimEnviHeader::isEnviHeader( hdr ) )
         {
            result = initializeFromEnviHdr( hdr );
         }
         else
         {
            result = initializeFromHdr( imageFile, hdr );
         }

         if ( !result )
         {
            // Could be an ossim meta data file:
            ossimKeywordlist kwl( hdr );
            result = loadState( kwl, 0 );
         }
      }
      else if ( ext == "xml" )
      {
         result = initializeFromXml( imageFile, hdr );
      }
   }
   
   //---
   // Set the file name.  Needed for ossimGeneralRasterTileSource::open.
   // Note set here above loadState call to stop loadState from returning
   // false if no image file found.
   //---
   if ( theImageFileList.empty() )
   {
      setImageFile( imageFile );
   }
   
   ossimFilename omd = imageFile;
   omd.setExtension("omd"); // image.omd
   if ( !omd.exists() )
   {
      omd.setExtension("kwl"); // image.kwl
   }

   if ( omd.exists() )
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "omd file: " << omd << std::endl;
      }

      ossimKeywordlist kwl( omd );

      if ( result && theMetaData.getNumberOfBands() )
      {
         //---
         // Just update the band info in case it has min/max values from
         // a compute min/max scan.
         //---
         theMetaData.updateMetaData( kwl, std::string("") );
      }
      else
      {
         // We're not initialized yet so do a loadState:
         result = loadState( kwl, 0 );
      }
   }
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE  << " Exit status: " << (result?"true":"false") << std::endl;
   }
   
   return result;
}

bool ossimGeneralRasterInfo::initializeFromHdr( const ossimFilename& imageFile,
                                                const ossimFilename& headerFile )
{
   bool result = false;

   ossimKeywordlist kwl;
   char delimeter = ' ';
   kwl.change_delimiter(delimeter);

   if ( kwl.addFile(headerFile) )
   {
      kwl.downcaseKeywords();
      ossimString value;
   
      while( 1 ) 
      {
         //---
         // Go through the data members in order.
         // If a required item is not found break from loop.
         //--
         theMetaData.clear();

         // scalar ( default ) - adjusted below :
         theMetaData.setScalarType( OSSIM_UINT8 );

         // Image file name:
         theImageFileList.clear();
         theImageFileList.push_back( imageFile );
         
         // interleave ( not required - default=BIL)
         theInterleaveType = OSSIM_BIL;
         value.string() = kwl.findKey( std::string("layout") );
         if ( value.size() )
         {
            ossimInterleaveTypeLut lut;
            ossim_int32 intrlv = lut.getEntryNumber( value.string().c_str(), true );
            if ( intrlv != ossimLookUpTable::NOT_FOUND )
            {
               theInterleaveType = static_cast<ossimInterleaveType>(intrlv);
            }
         }

         // bands ( required ):
         ossim_uint32 bands = 0;
         value.string() = kwl.findKey( std::string("nbands") );
         if ( value.size() )
         {
            bands = value.toUInt32();
         }
         if ( !bands )
         {
            break;
         }
         theMetaData.setNumberOfBands( bands );

         // lines ( required ):
         ossim_int32 lines = 0;
         value.string() = kwl.findKey( std::string("nrows") );
         if ( value.size() )
         {
            lines = value.toInt32();
         }
         if ( !lines )
         {
            break;
         }
         
         // samples ( required ):
         ossim_int32 samples = 0;
         value.string() = kwl.findKey( std::string("ncols") );
         if ( value.size() )
         {
            samples = value.toInt32();
         }
         if ( !samples )
         {
            break;
         }

         // nodata or null value ( not required )
         value.string() = kwl.findKey( std::string("nodata") );
         if ( value.empty() )
         {
            value.string() = kwl.findKey( std::string("nodata_value") );
         }
         if ( value.size() )
         {
            ossim_float64 nullValue = value.toUInt32();
            for ( ossim_uint32 band = 0; band < theMetaData.getNumberOfBands(); ++band )
            {
               theMetaData.setNullPix( band, nullValue );
            }
            theMetaData.setNullValuesValid(true);
         }

         // Set the rectangles:
         theRawImageRect   = ossimIrect( 0, 0, samples - 1, lines - 1 );
         theValidImageRect = theRawImageRect;
         theImageRect      = theRawImageRect;         

         // sample start ( not required ):
         theSubImageOffset.x = 0;

         // line start ( not required ):
         theSubImageOffset.y = 0;

         // header offset ( not required ):
         theHeaderSize = 0;

         // null mode:
         theSetNullsMode = ossimGeneralRasterInfo::NONE;

         // pixels to chop:
         thePixelsToChop = 0; 

         // Byte order, ( not required - defaulted to system if not found.
         theImageDataByteOrder = ossim::byteOrder();
         value.string() = kwl.findKey( std::string("byteorder") );
         if ( value.size() )
         {
            ossim_uint32 i = value.toUInt32();
            if ( i == 0 )
            {
               theImageDataByteOrder = OSSIM_LITTLE_ENDIAN;
            }
            else
            {
               theImageDataByteOrder = OSSIM_BIG_ENDIAN;
            }
         }

         // Pixel type used for scalar below:
         std::string pixelType = "N"; // not defined
         value.string() = kwl.findKey( std::string("pixeltype") );
         if ( value.size() )
         {
            pixelType = value.string();
         }
         
         ossim_int32 nbits = -1;
         value.string() = kwl.findKey( std::string("nbits") );
         if ( value.size() )
         {
            nbits = value.toInt32();
         }
         else
         {
            nbits = getBitsPerPixel( imageFile );
         }

         switch( nbits )
         {
            case 8:
            {
               theMetaData.setScalarType( OSSIM_UINT8 );
               break;
            }
            case 16:
            {
               if (pixelType == "S")
               {
                  theMetaData.setScalarType( OSSIM_SINT16 );
               }
               else
               {
                  theMetaData.setScalarType( OSSIM_UINT16 );
               }
               break;
            }
            case 32:
            {
               if( pixelType == "S")
               {
                  theMetaData.setScalarType( OSSIM_SINT32 );
               }
               else if( pixelType == "F")
               {
                  theMetaData.setScalarType( OSSIM_FLOAT32 );
               }
               else
               {
                  theMetaData.setScalarType( OSSIM_UINT32 );
               }
               break;
            }
            default:
            {
               if( (nbits < 8) && (nbits >= 1 ) )
               {
                  theMetaData.setScalarType( OSSIM_UINT8 );
               }
               break;
            }
         }

         result = true;
         break; // Trailing break to get out.
      }
   }

   return result;
   
} // End: ossimGeneralRasterInfo::initializeFromHdr


bool ossimGeneralRasterInfo::initializeFromEnviHdr( const ossimFilename& headerFile )
{
   bool result = false;
   ossimEnviHeader hdr;
   if( hdr.open( headerFile ) )
   {
      result = initializeFromEnviHdr( hdr );
   }
   return result;
}

bool ossimGeneralRasterInfo::initializeFromEnviHdr( const ossimEnviHeader& enviHdr )
{
   bool result = false;

   while( 1 ) 
   {
      //---
      // Go through the data members in order.
      // If a required item is not found break from loop.
      //--
      theMetaData.clear();
      
      // scalar ( required ) :
      if( enviHdr.getOssimScalarType() != OSSIM_SCALAR_UNKNOWN )
      {
         theMetaData.setScalarType( enviHdr.getOssimScalarType() );
      }
      else
      {
         break;
      }
      
      theImageFileList.clear();
      
      // interleave ( required ):
      theInterleaveType = enviHdr.getOssimInterleaveType();
      if ( theInterleaveType == OSSIM_INTERLEAVE_UNKNOWN )
      {
         break;
      }
      
      // bands ( required ):
      if ( !enviHdr.getBands() )
      {
         break;
      }
      theMetaData.setNumberOfBands( enviHdr.getBands() );
      
      // lines ( required ):
      ossim_uint32 lines = enviHdr.getLines();
      if ( !lines )
      {
         break;
      }
      
      // samples ( required ):
      ossim_uint32 samples = enviHdr.getSamples();
      if ( !samples )
      {
         break;
      }
      
      // Set the rectangles:
      theRawImageRect   = ossimIrect( 0, 0, samples - 1, lines - 1 );
      theValidImageRect = theRawImageRect;
      theImageRect      = theRawImageRect;         

      // sample start ( not required ):
      theSubImageOffset.x = enviHdr.getXStart();
      
      // line start ( not required ):
      theSubImageOffset.y = enviHdr.getYStart();
      
      // header offset ( not required ):
      theHeaderSize = enviHdr.getHeaderOffset();
      
      // null mode:
      theSetNullsMode = ossimGeneralRasterInfo::NONE;
      
      // pixels to chop:
      thePixelsToChop = 0; 
      
      // Byte order, this will be system if not found.
      theImageDataByteOrder = enviHdr.getByteOrder();
      
      result = true;
      break; // Trailing break to get out.
   }
   return result;
   
} // End: ossimGeneralRasterInfo::initializeFromEnviHdr( const ossimEnviHeader& )

bool ossimGeneralRasterInfo::initializeFromXml( const ossimFilename& imageFile,
                                                const ossimFilename& headerFile )
{
   bool result = false;

   ossimFgdcXmlDoc file;
   if (file.open( headerFile ))
   {
      while( 1 ) 
      {
         //---
         // Go through the data members in order.
         // If a required item is not found break from loop.
         //--
         theMetaData.clear();

         // scalar ( default ) - adjusted below :
         theMetaData.setScalarType( OSSIM_UINT8 );

         // Image file name:
         theImageFileList.clear();
         theImageFileList.push_back( imageFile );

         // interleave ( defaulted ):
         theInterleaveType = OSSIM_BIL;

         // bands ( required ):
         if ( !file.getNumberOfBands() )
         {
            break;
         }
         theMetaData.setNumberOfBands( file.getNumberOfBands() );
         
         ossimIpt size;
         if ( file.getImageSize(size) ) // Lines, samples not image file size.
         {
            // lines, samples ( required ):
            if ( !size.x || !size.y )
            {
               break;
            }
         }
         else
         {
            break;
         }

         // Set the rectangles:
         theRawImageRect   = ossimIrect( 0, 0, size.x - 1, size.y - 1 );
         theValidImageRect = theRawImageRect;
         theImageRect      = theRawImageRect;         

         // sample start ( not required ):
         theSubImageOffset.x = 0;

         // line start ( not required ):
         theSubImageOffset.y = 0;

         // header offset ( not required ):
         theHeaderSize = 0;

         // null mode:
         theSetNullsMode = ossimGeneralRasterInfo::NONE;

         // pixels to chop:
         thePixelsToChop = 0; 

         // Byte order *** need this ***, defaulting to system for now:
         theImageDataByteOrder = ossim::byteOrder();

         // Adjust scalar if needed, note defaulted to 8 bit above:
         ossimString eainfo;
         file.getPath("/metadata/eainfo/detailed/enttyp/enttypd", eainfo);
         ossim_int32 numBits = 0;
         ossim_int64 fileSize = imageFile.fileSize(); // Image file size.
         ossim_int32 numBytes = fileSize / size.x / size.y / numberOfBands();
         if( numBytes > 0 && numBytes != 3 )
         {
            numBits = numBytes*8;
         }
         if( numBits == 16 )
         {
            theMetaData.setScalarType( OSSIM_UINT16 );
         }
         else if( numBits == 32 )
         {
            if(eainfo.contains("float"))
            {
               theMetaData.setScalarType( OSSIM_FLOAT32 );
            }
            else
            {
               theMetaData.setScalarType( OSSIM_UINT32 );
            }
         }

         result = true;
         break; // Trailing break to get out.
      }
   }

   return result;
   
} // End: ossimGeneralRasterInfo::initializeFromXml

ossim_int32 ossimGeneralRasterInfo::getBitsPerPixel( const ossimFilename& imageFile ) const
{
   // Note currently does not consider header size.
   ossim_int32 result = 0;
   
   ossim_int64 fileSize = imageFile.size();
   ossimIpt rectSize = theRawImageRect.size();
   if ( fileSize && rectSize.x && rectSize.y && numberOfBands() )
   {
      result = ( fileSize / rectSize.x / rectSize.y / numberOfBands() ) * 8;
   }
   return result;
}

