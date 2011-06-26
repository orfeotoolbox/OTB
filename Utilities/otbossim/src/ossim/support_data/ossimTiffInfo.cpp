//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: TIFF Info object.
// 
//----------------------------------------------------------------------------
// $Id: ossimTiffInfo.cpp 2673 2011-06-06 14:57:24Z david.burken $

#include <ossim/support_data/ossimTiffInfo.h>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimGeoTiffCoordTransformsLut.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

// Static trace for debugging
static ossimTrace traceDebug("ossimTiffInfo:debug");
static ossimTrace traceDump("ossimTiffInfo:dump"); // This will dump offsets.

static const std::string PHOTO_INTERP[] =
{
   "MINISWHITE",
   "MINISBLACK",
   "RGB",
   "PALETTE",
   "MASK",
   "SEPARATED",
   "YCBCR",
   "CIELAB"
};

static const std::string ANGULAR_UNITS_KW = "angular_units";
static const std::string CENTER_LATITUDE__KW = "center_latitude";
static const std::string CENTER_LONGITUDE_KW = "center_longitude";
static const std::string COORD_TRANS_CODE_KW = "coord_trans_code";
static const std::string FALSE_ORIGIN_LONGITUDE_KW = "false_origin_longitude";
static const std::string FALSE_ORIGIN_LATITUDE_KW = "false_origin_latitude";
static const std::string FALSE_ORIGIN_EASTING_KW = "false_origin_easting";
static const std::string FALSE_ORIGIN_NORTHING_KW = "false_origin_northing";
static const std::string GEODETIC_DATUM_KW = "geodetic_datum";
static const std::string IMAGE_LENGTH_KW = "image_length";
static const std::string IMAGE_WIDTH_KW = "image_width";
static const std::string LINEAR_UNITS_KW = "linear_units";
static const std::string MODEL_PIXEL_SCALE_KW = "model_pixel_scale";
static const std::string MODEL_TIE_POINT_KW = "model_tie_point";
static const std::string MODEL_TRANSFORM_KW = "model_transform";
static const std::string MODEL_TYPE_KW = "model_type";
static const std::string ORIGIN_LATITUDE_KW = "origin_latitude";
static const std::string ORIGIN_LONGITUDE_KW = "origin_longitude";
static const std::string RASTER_TYPE_KW = "raster_type";
static const std::string VERTICAL_UNITS_KW = "vertical_units";


ossimTiffInfo::ossimTiffInfo()
   : ossimInfoBase(),
     theFile(),
     theEndian(0)
{
}

ossimTiffInfo::~ossimTiffInfo()
{
   if (theEndian)
   {
      delete theEndian;
      theEndian = 0;
   }
}

bool ossimTiffInfo::open(const ossimFilename& file)
{
   bool result = false;

   //---
   // Open the tif file.
   //---
   std::ifstream str(file.c_str(), std::ios_base::binary|std::ios_base::in);
   if (str.good()) 
   {
      //---
      // Get the byte order.  First two byte should be "II" or "MM".
      //---
      char byteOrder[2];
      str.read(byteOrder, 2); // Read the byte order.
      ossimByteOrder sysByteOrder = ossim::byteOrder();
      ossimByteOrder tifByteOrder = OSSIM_LITTLE_ENDIAN;
      
      if (byteOrder[0] == 'M')
      {
         tifByteOrder = OSSIM_BIG_ENDIAN;
      }

      if (sysByteOrder != tifByteOrder)
      {
         if (!theEndian)
         {
            theEndian = new ossimEndian();
         }
      }
      else if (theEndian)
      {
         delete theEndian;
         theEndian = 0;
      }
      
      //--
      // Get the version. Note theEndian must be set/unset before calling
      // "readShort".
      //---
      ossim_uint16 version;
      readShort(version, str);

      if ( ( (byteOrder[0] == 'M') || (byteOrder[0] == 'I') ) &&
           ( (version == 42) || (version == 43) ) )
      {
         result = true;  // is a tif...
      }
   }

   if (result)
   {
      theFile = file;
   }
   else
   {
      theFile.clear();
      if (theEndian)
      {
         delete theEndian;
         theEndian = 0;
      }
   }

   return result;
}

std::ostream& ossimTiffInfo::print(std::ostream& out) const
{
   static const char MODULE[] = "ossimTiffInfo::print";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG Entered...\n";
   }

   //---
   // Open the tif file.
   //---
   std::ifstream str(theFile.c_str(), std::ios_base::binary|std::ios_base::in);
   if (!str) 
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " Cannot open file:  " << theFile << std::endl;
      }
      return out;
   }
   
   //---
   // Get the byte order.  First two byte should be "II" or "MM".
   //---
   char byteOrder[2];
   str.read(byteOrder, 2); // Read the byte order.
   ossimByteOrder sysByteOrder = ossim::byteOrder();
   ossimByteOrder tifByteOrder = OSSIM_LITTLE_ENDIAN;
   
   if (byteOrder[0] == 'M')
   {
      tifByteOrder = OSSIM_BIG_ENDIAN;
   }
   
   if (sysByteOrder != tifByteOrder)
   {
      if (!theEndian)
      {
         theEndian = new ossimEndian();
      }
   }
   else if (theEndian) // No swapping required.
   {
      delete theEndian;
      theEndian = 0;
   }

   //--
   // Get the version. Note theEndian must be set/unset before calling
   // "readShort".
   //---
   ossim_uint16 version;
   readShort(version, str);

   // Set the tag value length.
   ossim_uint64 tagValueLength;
   if (version == 42)
   {
      tagValueLength = 4;
   }
   else
   {
      tagValueLength = 8;
   }

   out << "tiff.version: " << int(version)
       << ((version==42)?"(classic)\n":"(big)\n")
       << "tiff.byte_order: ";
   
   if (byteOrder[0] == 'M')
   {
      out << "big_endian\n";
   }
   else // OSSIM_LITTLE_ENDIAN
   {
      out  << "little_endian\n";
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "system_byte_order: ";
      if (ossim::byteOrder() == OSSIM_BIG_ENDIAN)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<< "big_endian\n";
      }
      else // OSSIM_LITTLE_ENDIAN
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<< "little_endian\n";
      }
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "tiff.tag_value_length: " << tagValueLength << "\n";
   }

   //---
   // Variables used within the loop.
   //---
   std::streamoff seekOffset;      // used throughout
   std::streampos streamPosition;  // used throughout

   if (version == 43)
   {
      // We must skip the first four bytes.
      ossim_uint32 offsetSize;
      readLong(offsetSize, str);
   }

   // Get the offset.
   if (getOffset(seekOffset, str, version) == false)
   {
     ossimNotify(ossimNotifyLevel_WARN) 
        << MODULE << " FATAL ERROR - "
        << "No offset to an image file directory found.\n"
        << "Returning with error."
        << std::endl;
     str.close();
     return out;
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: "
         << "Offset to first ifd:  " << seekOffset
         << "\n";
   }

   // Capture the original flags then set float output to full precision.
   std::ios_base::fmtflags f = out.flags();
   out << setiosflags(std::ios::fixed) << std::setprecision(15);
   
   // Image File Directory (IFD) loop.
   ossim_int32 ifdIndex = 0;
   while(seekOffset)
   {
      out << "tiff.directory_offset: " << seekOffset << "\n";

      // Seek to the image file directory.
      str.seekg(seekOffset, std::ios_base::beg);  

      // directory prefix for prints.
      std::string prefix = "tiff.";
      getDirPrefix(ifdIndex, prefix);

      //---
      // Things we need to save for printGeoKeys:
      //---
      ossim_uint16*  geoKeyBlock     = 0;
      ossim_uint64   geoKeyLength    = 0;
      ossim_float64* geoDoubleBlock  = 0;
      ossim_uint64   geoDoubleLength = 0;
      ossim_int8*    geoAsciiBlock   = 0;
      ossim_uint64   geoAsciiLength  = 0;
      
      //---
      // Get the number of directories within the IFD.
      //---
      ossim_uint64 nTags; // Number of tags in an IFD.
      if (getValue(nTags, str, TWO_OR_EIGHT, version) == false)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " FATAL error reading number of direcories."
               << std::endl;
         }
         str.close();
         return out;
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:\n"
            << "ifd:  " << seekOffset
            << "\ntags in directory:  " << nTags<< "\n";
      }

      // Tag loop:
      for (ossim_uint64 tagIdx = 0; tagIdx < nTags; ++tagIdx)
      {
         // Variables used within the loop.
         ossim_uint16   tag              = 0; // Tag number
         ossim_uint16   type             = 0; // Type(short, long...)
         ossim_uint64   count            = 0;
         ossim_uint64   arraySizeInBytes = 0; // 
         ossim_uint8*   valueArray       = 0; // To hold value.
         
         //---
         // Get the tag.
         //---
         readShort(tag, str);
         if (!str)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading tag number."
                  << std::endl;
            }
            str.close();
            return out;
         }

         //---
         // Get the type (byte, ascii, short...)
         //---
         readShort(type, str);
         if (!str)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading type number."
                  << std::endl;
            }
            str.close();
            return out;
         }

         //---
         // Get the count.  This is not in bytes.  It is based on the
         // type.  So if the type is a short and the count is one then
         // read "sizeof(short"(2) bytes.
         //---
         getValue(count, str, FOUR_OR_EIGHT, version);
         if (!str)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading count."
                  << std::endl;
            }
            str.close();
            return out;
         }

         // Get the array size in bytes.
         arraySizeInBytes = getArraySizeInBytes(count, type);
         if (arraySizeInBytes == 0)
         {
            // Could be an unhandle type.  Gobble the value.
            eatValue(str, version);
         }
         else
         {
            // Allocate array.
            if (valueArray) delete [] valueArray;
            valueArray = new ossim_uint8[arraySizeInBytes];

            if (arraySizeInBytes <= tagValueLength)
            {
               // Read in the value(s).
               str.read((char*)valueArray, arraySizeInBytes);

               // Skip any byes left in the field.
               if (arraySizeInBytes < tagValueLength)
               {
                  // Skip these bytes.
                  str.ignore(tagValueLength-arraySizeInBytes);
               }
            }
            else // Data to big for field.  Stored elsewhere...
            {
               // Get the offset to the data.
               getOffset(seekOffset, str, version);

               // Capture the seek position to come back to.
               streamPosition = str.tellg();

               // Seek to the data.
               str.seekg(seekOffset, std::ios_base::beg);

               // Read in the value(s).
               str.read((char*)valueArray, arraySizeInBytes);

               // Seek back.
               str.seekg(streamPosition);
            }

            // Swap the bytes if needed.
            swapBytes(valueArray, type, count);
         }

         if( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\ntag[" << tagIdx << "]:" << tag
               << "\ntype:                " << type
               << "\ncount:        " << count
               << "\narray size in bytes: " << arraySizeInBytes
               << "\n";
	 }

         if (tag == OGEO_KEY_DIRECTORY_TAG)
         {
            // tag 34735 save for printGeoKeys
            geoKeyBlock = reinterpret_cast<ossim_uint16*>(valueArray);
            geoKeyLength = count;
         }
         else if (tag == OGEO_DOUBLE_PARAMS_TAG)
         {
            // tag 34736 save for printGeoKeys
            geoDoubleBlock  = reinterpret_cast<ossim_float64*>(valueArray);
            geoDoubleLength = count;
         }
         else if (tag == OGEO_ASCII_PARAMS_TAG)
         {
            // tag 34737 save for printGeoKeys
            geoAsciiBlock   = reinterpret_cast<ossim_int8*>(valueArray);
            geoAsciiLength  = count;
         }
         else
         {
            print(out,
                  prefix,
                  tagIdx,
                  tag,
                  type,
                  count,
                  arraySizeInBytes,
                  valueArray);
            
            // Free memory if allocated...
            if (valueArray)
            {
               delete [] valueArray;
               valueArray = 0;
            }
         }
         
      } // End of tag loop.

      //---
      // If Geotiff Keys read them.
      // This had to done last since the keys could
      // have references to tags GEO_DOUBLE_PARAMS_TAG and
      // GEO_ASCII_PARAMS_TAG.
      //---
      if (geoKeyBlock)
      {
         printGeoKeys(out, prefix, geoKeyLength, geoKeyBlock,
                      geoDoubleLength,geoDoubleBlock,
                      geoAsciiLength,geoAsciiBlock);

         delete [] geoKeyBlock;
         geoKeyBlock = 0;
      }

      if (geoDoubleBlock)
      {
         delete [] geoDoubleBlock;
         geoDoubleBlock = 0;
      }
      if (geoAsciiBlock)
      {
         delete [] geoAsciiBlock;
         geoAsciiBlock = 0;
      }
      geoKeyLength = 0;
      geoDoubleLength = 0;
      geoAsciiLength = 0;

      //---
      // Get the next IFD offset.  Continue this loop until the offset is
      // zero.
      //---
      if (getOffset(seekOffset, str, version) == false)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " No offset to an image file directory found.\n"
               << "Returning with error."
               << std::endl;
         }
         str.close();
         return out;
      }
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimTiffInfo::readTags: "
            << "Next Image File Directory(IFD) offset = "
            << seekOffset << "\n";
      }

      ++ifdIndex; // next ifd

      //---
      // Note this does NOT check to see if sub_file_type is '1' simply
      // skips all directories past the first if theOverviewFlag is false.
      //---
      if ( ifdIndex && !theOverviewFlag) // Don't process overviews...
      {
         break;
      }
      
   } // End of loop through the IFD's.
   
   out << std::endl;
    
   str.close();

   // Reset flags.
   out.setf(f);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG Exited..." << std::endl;
   }
   
   return out;
}

std::ostream& ossimTiffInfo::print(std::ifstream& inStr,
                                   std::ostream& outStr) const
{
   static const char MODULE[] = "ossimTiffInfo::print(std::ifstream&, std::ostream&)";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG Entered...\n";
   }

   std::streampos startPosition = inStr.tellg();
   
   //---
   // Get the byte order.  First two byte should be "II" or "MM".
   //---
   char byteOrder[2];
   inStr.read(byteOrder, 2); // Read the byte order.
   ossimByteOrder sysByteOrder = ossim::byteOrder();
   ossimByteOrder tifByteOrder = OSSIM_LITTLE_ENDIAN;
   
   if (byteOrder[0] == 'M')
   {
      tifByteOrder = OSSIM_BIG_ENDIAN;
   }
   
   if (sysByteOrder != tifByteOrder)
   {
      if (!theEndian)
      {
         theEndian = new ossimEndian();
      }
   }
   else if (theEndian) // No swapping required.
   {
      delete theEndian;
      theEndian = 0;
   }

   //--
   // Get the version. Note theEndian must be set/unset before calling
   // "readShort".
   //---
   ossim_uint16 version;
   readShort(version, inStr);

   // Set the tag value length.
   ossim_uint64 tagValueLength;
   if (version == 42)
   {
      tagValueLength = 4;
   }
   else
   {
      tagValueLength = 8;
   }

   outStr << "tiff.version: " << int(version)
          << ((version==42)?"(classic)\n":"(big)\n")
          << "tiff.byte_order: ";
   
   if (byteOrder[0] == 'M')
   {
      outStr << "big_endian\n";
   }
   else // OSSIM_LITTLE_ENDIAN
   {
      outStr  << "little_endian\n";
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "system_byte_order: ";
      if (ossim::byteOrder() == OSSIM_BIG_ENDIAN)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<< "big_endian\n";
      }
      else // OSSIM_LITTLE_ENDIAN
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<< "little_endian\n";
      }
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "tiff.tag_value_length: " << tagValueLength << "\n";
   }

   //---
   // Variables used within the loop.
   //---
   std::streamoff seekOffset;      // used throughout
   std::streampos streamPosition;  // used throughout

   if (version == 43)
   {
      // We must skip the first four bytes.
      ossim_uint32 offsetSize;
      readLong(offsetSize, inStr);
   }

   // Get the offset.
   if (getOffset(seekOffset, inStr, version) == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) 
            << MODULE << " FATAL ERROR - "
            << "No offset to an image file directory found.\n"
            << "Returning with error."
            << std::endl;
      }
      return outStr;
   }
   
   if (traceDebug())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: "
            << "Offset to first ifd:  " << seekOffset
            << "\n";
      }
   }

   // Capture the original flags then set float output to full precision.
   std::ios_base::fmtflags f = outStr.flags();
   outStr << setiosflags(std::ios::fixed) << std::setprecision(15);
   
   // Image File Directory (IFD) loop.
   ossim_int32 ifdIndex = 0;
   while(seekOffset)
   {
      outStr << "tiff.directory_offset: " << seekOffset << "\n";

      // Seek to the image file directory.
      inStr.seekg(startPosition+seekOffset, std::ios_base::beg);  

      // directory prefix for prints.
      std::string prefix = "tiff.";
      getDirPrefix(ifdIndex, prefix);

      //---
      // Things we need to save for printGeoKeys:
      //---
      ossim_uint16*  geoKeyBlock     = 0;
      ossim_uint64   geoKeyLength    = 0;
      ossim_float64* geoDoubleBlock  = 0;
      ossim_uint64   geoDoubleLength = 0;
      ossim_int8*    geoAsciiBlock   = 0;
      ossim_uint64   geoAsciiLength  = 0;
      
      //---
      // Get the number of directories within the IFD.
      //---
      ossim_uint64 nTags; // Number of tags in an IFD.
      if (getValue(nTags, inStr, TWO_OR_EIGHT, version) == false)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " FATAL error reading number of direcories."
               << std::endl;
         }
         return outStr;
      }

      if (traceDebug())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:\n"
               << "ifd:  " << seekOffset
               << "\ntags in directory:  " << nTags<< "\n";
         }
      }

      // Tag loop:
      for (ossim_uint64 tagIdx = 0; tagIdx < nTags; ++tagIdx)
      {
         // Variables used within the loop.
         ossim_uint16   tag              = 0; // Tag number
         ossim_uint16   type             = 0; // Type(short, long...)
         ossim_uint64   count            = 0;
         ossim_uint64   arraySizeInBytes = 0; // 
         ossim_uint8*   valueArray       = 0; // To hold value.
         
         //---
         // Get the tag.
         //---
         readShort(tag, inStr);
         if (!inStr)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading tag number."
                  << std::endl;
            }
            return outStr;
         }

         //---
         // Get the type (byte, ascii, short...)
         //---
         readShort(type, inStr);
         if (!inStr)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading type number."
                  << std::endl;
            }
            return outStr;
         }

         //---
         // Get the count.  This is not in bytes.  It is based on the
         // type.  So if the type is a short and the count is one then
         // read "sizeof(short"(2) bytes.
         //---
         getValue(count, inStr, FOUR_OR_EIGHT, version);
         if (!inStr)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " FATAL error reading count."
                  << std::endl;
            }
            return outStr;
         }

         // Get the array size in bytes.
         arraySizeInBytes = getArraySizeInBytes(count, type);
         if (arraySizeInBytes == 0)
         {
            // Could be an unhandle type.  Gobble the value.
            eatValue(inStr, version);
         }
         else
         {
            // Allocate array.
            if (valueArray) delete [] valueArray;
            valueArray = new ossim_uint8[arraySizeInBytes];

            if (arraySizeInBytes <= tagValueLength)
            {
               // Read in the value(s).
               inStr.read((char*)valueArray, arraySizeInBytes);

               // Skip any byes left in the field.
               if (arraySizeInBytes < tagValueLength)
               {
                  // Skip these bytes.
                  inStr.ignore(tagValueLength-arraySizeInBytes);
               }
            }
            else // Data to big for field.  Stored elsewhere...
            {
               // Get the offset to the data.
               getOffset(seekOffset, inStr, version);

               // Capture the seek position to come back to.
               streamPosition = inStr.tellg();

               // Seek to the data.
               inStr.seekg(startPosition+seekOffset, std::ios_base::beg);

               // Read in the value(s).
               inStr.read((char*)valueArray, arraySizeInBytes);

               // Seek back.
               inStr.seekg(streamPosition);
            }

            // Swap the bytes if needed.
            swapBytes(valueArray, type, count);
         }

         if( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\ntag[" << tagIdx << "]:" << tag
               << "\ntype:                " << type
               << "\ncount:        " << count
               << "\narray size in bytes: " << arraySizeInBytes
               << "\n";
         }

         if (tag == OGEO_KEY_DIRECTORY_TAG)
         {
            // tag 34735 save for printGeoKeys
            geoKeyBlock = reinterpret_cast<ossim_uint16*>(valueArray);
            geoKeyLength = count;
         }
         else if (tag == OGEO_DOUBLE_PARAMS_TAG)
         {
            // tag 34736 save for printGeoKeys
            geoDoubleBlock  = reinterpret_cast<ossim_float64*>(valueArray);
            geoDoubleLength = count;
         }
         else if (tag == OGEO_ASCII_PARAMS_TAG)
         {
            // tag 34737 save for printGeoKeys
            geoAsciiBlock   = reinterpret_cast<ossim_int8*>(valueArray);
            geoAsciiLength  = count;
         }
         else
         {
            print(outStr,
                  prefix,
                  tagIdx,
                  tag,
                  type,
                  count,
                  arraySizeInBytes,
                  valueArray);
            
            // Free memory if allocated...
            if (valueArray)
            {
               delete [] valueArray;
               valueArray = 0;
            }
         }
         
      } // End of tag loop.

      //---
      // If Geotiff Keys read them.
      // This had to done last since the keys could
      // have references to tags GEO_DOUBLE_PARAMS_TAG and
      // GEO_ASCII_PARAMS_TAG.
      //---
      if (geoKeyBlock)
      {
         printGeoKeys(outStr, prefix, geoKeyLength, geoKeyBlock,
                      geoDoubleLength,geoDoubleBlock,
                      geoAsciiLength,geoAsciiBlock);

         delete [] geoKeyBlock;
         geoKeyBlock = 0;
      }

      if (geoDoubleBlock)
      {
         delete [] geoDoubleBlock;
         geoDoubleBlock = 0;
      }
      if (geoAsciiBlock)
      {
         delete [] geoAsciiBlock;
         geoAsciiBlock = 0;
      }
      geoKeyLength = 0;
      geoDoubleLength = 0;
      geoAsciiLength = 0;

      //---
      // Get the next IFD offset.  Continue this loop until the offset is
      // zero.
      //---
      if (getOffset(seekOffset, inStr, version) == false)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " No offset to an image file directory found.\n"
               << "Returning with error."
               << std::endl;
         }
         return outStr;
      }
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimTiffInfo::readTags: "
            << "Next Image File Directory(IFD) offset = "
            << seekOffset << "\n";
      }

      ++ifdIndex; // next ifd

      //---
      // Note this does NOT check to see if sub_file_type is '1' simply
      // skips all directories past the first if theOverviewFlag is false.
      //---
      if ( ifdIndex && !theOverviewFlag) // Don't process overviews...
      {
         break;
      }
      
   } // End of loop through the IFD's.
   
   outStr << std::endl;
    
   // Reset flags.
   outStr.setf(f);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG Exited..." << std::endl;
   }
   
   return outStr;
}

bool ossimTiffInfo::getImageGeometry(ossimKeywordlist& geomKwl,
                                     ossim_uint32 entryIndex) const
{
   static const char MODULE[] = "ossimTiffInfo::getImageGeometry #1";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << " entered...\n";
   }
   
   bool result = false;

   // Open the file.
   std::ifstream str;
   str.open(theFile.c_str(), ios::in | ios::binary);
   
   if ( str.is_open() )
   {
      result = getImageGeometry(str, geomKwl, entryIndex);

      str.close();
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "geomKwl:\n"
         << geomKwl
         << MODULE << " exit status = " << (result?"true":"false") << "\n";
   }

   return result;
}

bool ossimTiffInfo::getImageGeometry(std::ifstream& inStr,
                                     ossimKeywordlist& geomKwl,
                                     ossim_uint32 entryIndex) const
{
   static const char M[] = "ossimTiffInfo::getImageGeometry #2";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << " entered...\n";
   }
   
   bool result = false;

   // Do a print to a memory stream.
   std::ostringstream out;
   print(inStr, out);

   // Open an input stream to pass to the keyword list.
   std::istringstream in( out.str() );

   // Since the print is in key:value format we can pass to a keyword list.
   ossimKeywordlist gtiffKwl;
   if ( gtiffKwl.parseStream(in) )      
   {
      result = getImageGeometry(gtiffKwl, geomKwl, entryIndex);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "geomKwl:\n"
         << geomKwl
         << M << " exit status = " << (result?"true":"false") << "\n";
   }

   return result;
}

void ossimTiffInfo::getImageGeometry(ossim_uint64   geoKeyLength,
                                     ossim_uint16*  geoKeyBlock,
                                     ossim_uint64   geoDoubleLength,
                                     ossim_float64* geoDoubleBlock,
                                     ossim_uint64   geoAsciiLength,
                                     ossim_int8*    geoAsciiBlock,
                                     ossimKeywordlist& geomKwl) const
{
   static const char M[] = "ossimTiffInfo::getImageGeometry #3";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }

   // Dump the geotiff keys to memory.
   ostringstream out;
   printGeoKeys(out, std::string("tiff.image0."),
                geoKeyLength, geoKeyBlock,
                geoDoubleLength,geoDoubleBlock,
                geoAsciiLength,geoAsciiBlock);

   // Open an input stream to pass to the keyword list.
   std::istringstream in( out.str() );

   // Since the print is in key:value format we can pass to a keyword list.
   ossimKeywordlist gtiffKwl;
   if ( gtiffKwl.parseStream(in) )
   {
      getImageGeometry(gtiffKwl, geomKwl, 0);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

// Private method:
bool ossimTiffInfo::getImageGeometry(const ossimKeywordlist& gtiffKwl,
                                     ossimKeywordlist& geomKwl,
                                     ossim_uint32 entryIndex) const
{
   static const char M[] = "ossimTiffInfo::getImageGeometry #4";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered...\n";
   }
   
   //---
   // Start with a return status of true and set to false if something bad
   // happens.
   //---
   bool result = true;
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "tiffinfo dump to kwl:\n" << gtiffKwl << "\n";
   }
   
   ossimString gtiffPrefix = "tiff.image";
   gtiffPrefix += ossimString::toString(entryIndex);
   gtiffPrefix += ".";
   ossimString geomPrefix = "image";
   geomPrefix += ossimString::toString(entryIndex);
   geomPrefix += ".";
   
   // Get the pixel type.
   ossimString pixelType;
   if ( getPixelType(gtiffPrefix, gtiffKwl, pixelType) == false )
   {
      pixelType = "pixel_is_point"; // Not an error we'll make assumption?
   }
   geomKwl.add(geomPrefix.c_str(),
               ossimKeywordNames::PIXEL_TYPE_KW,
               pixelType.c_str());
   
   // Get the lines.
   ossim_uint32 height = getLines(gtiffPrefix, gtiffKwl);
   if (height)
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::NUMBER_LINES_KW,
                  height);
   }
   else
   {
      result = false;
   }
   
   // Get the samples.
   ossim_uint32 width = getSamples(gtiffPrefix, gtiffKwl);
   if (width)
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::NUMBER_SAMPLES_KW,
                  width);
   }
   else
   {
      result = false;
   }
   
   // Add the pixel type.
   geomKwl.add(geomPrefix.c_str(),
               ossimKeywordNames::PIXEL_TYPE_KW,
               pixelType.c_str());
   
   // Set the projection type.
   ossimString pcsCode;
   ossimString ossimProjectionName = "";
   bool hasPcsCode = getPcsCode(gtiffPrefix, gtiffKwl, pcsCode);
   if (hasPcsCode)
   {
      // Add the pcs code.
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::PCS_CODE_KW,
                  pcsCode.c_str());
   }
   else
   {
      if ( getOssimProjectionName(gtiffPrefix, gtiffKwl, ossimProjectionName) == false )
         ossimProjectionName = "ossimEquDistCylProjection";
      geomKwl.add(geomPrefix.c_str(), ossimKeywordNames::TYPE_KW, ossimProjectionName);
   }
   
   // Get the units. 
   ossimString units = "";
   getUnits(gtiffPrefix, gtiffKwl, units);
   if (units.empty() || (units.contains("unknown")))
   {
      // HACK: Encountered JP2 with geotiff info that did not specify units, so using projection
      // type to discern units if none explicitly specified. (OLK 05/11)
      if (ossimProjectionName == "ossimEquDistCylProjection")
         units = "degrees";
   }
   
   // Get the pixel scale.
   ossimDpt scale;
   bool hasScale = getPixelScale(gtiffPrefix, gtiffKwl, scale);
   
   // Get the tie point.
   std::vector<ossim_float64> ties;
   getTiePoint(gtiffPrefix, gtiffKwl, ties);

   //---
   // Tie count:
   // NOTE: It takes six doubles to make one tie point ie:
   // x,y,z,longitude,latitude,height or x,y,z,easting,northing,height
   //--- 
   ossim_uint32 tieCount = (ossim_uint32)ties.size()/6;
   
   // Get the model transform.
   std::vector<ossim_float64> xfrm;
   getModelTransform(gtiffPrefix, gtiffKwl, xfrm);
   
   bool useXfrm = false;
   if ( xfrm.size() == 16 )
   {
      // Need at least 24 (which is four ties) to use bilinear.
      if ( !hasScale && ties.size() < 24 )
      {
         useXfrm = true;
      }
   }
   if (useXfrm)
   {
      std::ostringstream out;
      out << std::setprecision(15); // To avoid truncating.
      ossim_uint32 idx = 0;
      for(idx =0; idx < 16; ++idx)
      {
         out << xfrm[idx] << " ";
      }
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::IMAGE_MODEL_TRANSFORM_MATRIX_KW,
                  out.str().c_str(), true);
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::IMAGE_MODEL_TRANSFORM_UNIT_KW,
                  units.c_str(), true);
   }
   else // Use tie points.
   {
      if ( hasScale && (tieCount == 1) )
      {
         // Shift the tile to 0,0 pixel of image if not already there.
         ossimDpt tie;
         tie.x = ties[3] - ties[0] * scale.x;
         tie.y = ties[4] - ties[1] * scale.y;
         geomKwl.add(geomPrefix.c_str(),
                     ossimKeywordNames::TIE_POINT_XY_KW,
                     tie.toString().c_str());
         geomKwl.add(geomPrefix.c_str(),
                     ossimKeywordNames::TIE_POINT_UNITS_KW,
                     units.c_str());
         
         // Add the scale.
         geomKwl.add(geomPrefix.c_str(),
                     ossimKeywordNames::PIXEL_SCALE_XY_KW,
                     scale.toString().c_str());
         geomKwl.add(geomPrefix.c_str(),
                     ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
                     units.c_str());
      }
      else if (tieCount > 1) // four or better tie points.
      {
         ossimTieGptSet tieSet;
         getTieSets(ties, width, height, tieSet);
         
         if(tieCount >= 4)
         {
            ossimRefPtr<ossimBilinearProjection> proj =
               new ossimBilinearProjection;
            proj->optimizeFit(tieSet);
            proj->saveState(geomKwl, geomPrefix.c_str());
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Creating a bilinear projection\n";
            }
         }
         else  // Need at least four ties.
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
   } // matches: else Use tie points block.
   
   ossimString tmpStr;
   if ( getStdParallelOne(gtiffPrefix, gtiffKwl, tmpStr) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::STD_PARALLEL_1_KW,
                  tmpStr);
   }
   
   if ( getStdParallelTwo(gtiffPrefix, gtiffKwl, tmpStr) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::STD_PARALLEL_2_KW,
                  tmpStr);
   }
   
   ossimDpt eastingNorthing;
   if ( getFalseEastingNorthing(gtiffPrefix, gtiffKwl,
                                eastingNorthing) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
                  eastingNorthing.toString());
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::
                  FALSE_EASTING_NORTHING_UNITS_KW, units);
   }
   
   ossim_float64 tmpDbl;
   
   if ( getOriginLat(gtiffPrefix, gtiffKwl, tmpDbl) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::ORIGIN_LATITUDE_KW,
                  tmpDbl);
   }
   if ( getCentralMeridian(gtiffPrefix, gtiffKwl, tmpDbl) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                  tmpDbl);
   }
   
   if ( getScaleFactor(gtiffPrefix, gtiffKwl, tmpDbl) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::SCALE_FACTOR_KW,
                  tmpDbl);
   }
   
   if ( getDatumCode(gtiffPrefix, gtiffKwl, tmpStr) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  ossimKeywordNames::DATUM_KW,
                  tmpStr.c_str());
   }

   //---
   // Linear and vertical units not read by projection factories but added so external user could
   // query.
   //---
   if ( getLinearUnits(gtiffPrefix, gtiffKwl, tmpStr) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  LINEAR_UNITS_KW.c_str(),
                  tmpStr.c_str());
   }

   if ( getVerticalUnits(gtiffPrefix, gtiffKwl, tmpStr) )
   {
      geomKwl.add(geomPrefix.c_str(),
                  VERTICAL_UNITS_KW.c_str(),
                  tmpStr.c_str());
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "geomKwl:\n"
         << geomKwl << "\n"
         << M << " exit status = " << (result?"true":"false") << "\n";
   }
   
   return result;
}

void ossimTiffInfo::readShort(ossim_uint16& s, std::ifstream& str) const
{
   str.read((char*)&s, sizeof(s));
   if (theEndian)
   {
      theEndian->swap(s);
   }
}

void ossimTiffInfo::readLong(ossim_uint32& l, std::ifstream& str) const
{
   str.read((char*)&l, sizeof(l));
   if (theEndian)
   {
      theEndian->swap(l);
   }
}

void ossimTiffInfo::readLongLong(ossim_uint64& l, std::ifstream& str) const
{
   str.read((char*)&l, sizeof(l));
   if (theEndian)
   {
      theEndian->swap(l);
   }
}



bool ossimTiffInfo::getOffset(
   std::streamoff& offset, std::ifstream& str, ossim_uint16 version) const
{
   bool status = true;
   if  (version == 42)
   {
      ossim_uint32   littleOffset;
      readLong(littleOffset, str);
      offset = littleOffset;
   }
   else
   {
      ossim_uint64 bigOffset;
      readLongLong(bigOffset, str);
      offset = bigOffset;
   }
   if (!str)
   {
      status = false;
   }
   return status;
}

bool ossimTiffInfo::getValue(ossim_uint64& value,
                             std::ifstream& str,
                             WordType type,
                             ossim_uint16 version) const
{
   bool status = true;
   if  (version == 42)
   {
      if (type == TWO_OR_EIGHT)
      {
         ossim_uint16 i;
         readShort(i, str);
         value = i;
      }
      else
      {
         ossim_uint32 i;
         readLong(i, str);
         value = i;
      }
   }
   else
   {
      ossim_uint64 i;
      readLongLong(i, str);
      value = i;
   }
   if (!str)
   {
      status = false;
   }
   return status;
}

ossim_uint64 ossimTiffInfo::getArraySizeInBytes(ossim_uint64 length,
                                                ossim_uint16 type) const
{
   return length * getTypeByteSize(type);
}

ossim_uint16 ossimTiffInfo::getTypeByteSize(ossim_uint16 type) const
{
   ossim_uint16 result = 0;
   
   switch (type)
   {
      case OTIFF_BYTE:
      case OTIFF_ASCII:
      case OTIFF_SBYTE:
      case OTIFF_UNDEFINED:
      {
         result = 1;
         break;
      }
         
      case OTIFF_SHORT:
      case OTIFF_SSHORT:
      {
         result = 2;
         break;
      }
         
      case OTIFF_LONG:
      case OTIFF_SLONG:
      case OTIFF_IFD:
      case OTIFF_FLOAT:
      {
         result = 4;
         break;
      }
         
      case OTIFF_RATIONAL:
      case OTIFF_SRATIONAL:
      case OTIFF_DOUBLE:
      case 16:             // TIFF_LONG8 defined in big tiff only.
      case 17:             // TIFF_SLONG8 defined in big tiff only.
      case 18:             // TIFF_IFD8 defined in big tiff only.
      {
         result = 8;
         break;
      }

      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimTiffInfo::getTypeByteSize DEBUG:"
               << "\nUnhandled type: " << int(type) << "\n";
         }
         break;
      }
   }
   return result;
}

void ossimTiffInfo::eatValue(std::ifstream& str, ossim_uint16 version) const
{
   if (version == 42)
   {
      ossim_uint32 dummy;
      readLong(dummy, str);
   }
   else
   {
      ossim_uint64 dummy;
      readLongLong(dummy, str);
   }
}

void ossimTiffInfo::swapBytes(ossim_uint8* v,
                              ossim_uint16 type,
                              ossim_uint64 count) const
{
   if (theEndian)
   {
      ossim_uint32 byteSize = getTypeByteSize(type);
      switch(byteSize)
      {
         case 2:
            theEndian->swapTwoBytes(v, count);
            break;
         case 4:
            theEndian->swapFourBytes(v, count);
            break;
         case 8:
            theEndian->swapEightBytes(v, count);
            break;
         default:
            break;
      }
   }
}

template <class T> void ossimTiffInfo::getArrayValue(
   T& v,
   ossim_uint8* array,
   ossim_uint64 position) const
{
   T* p = (T*)array;
   v = p[position];
}

std::ostream& ossimTiffInfo::print(std::ostream& out,
                                   const std::string& prefix,
                                   ossim_uint64 tagIdx,
                                   ossim_uint16 tag,
                                   ossim_uint16 type,
                                   ossim_uint64 count,
                                   ossim_uint64 arraySizeInBytes,
                                   ossim_uint8* valueArray) const
{
   switch(tag)
   {
      case OTIFFTAG_SUBFILETYPE: // tag 254
      {
         out << prefix << "sub_file_type: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_IMAGEWIDTH: // tag 256
      {
         out << prefix << IMAGE_WIDTH_KW << ": ";
         printValue(out, type, valueArray);
         break;
      }
      
      case OTIFFTAG_IMAGELENGTH: // tag 257
      {
         out << prefix << IMAGE_LENGTH_KW << ": ";
         printValue(out, type, valueArray);
         break;
      }

      case OTIFFTAG_BITSPERSAMPLE: // tag 258
      {
         out << prefix << "bits_per_sample: ";
         if (count == 1)
         {
            printValue(out, type, valueArray);
         }
         else if (valueArray)
         {
            printArray(out, type, count, valueArray);
         }
         break;
      }
         
      case OTIFFTAG_COMPRESSION: // tag 259
      {
         if ( (count == 1) && (type == OTIFF_SHORT) )
         {
            out << prefix << "compression: ";
            
            ossim_uint16 s;
            getArrayValue(s, valueArray, 0);
            if (s == 1)
            {
               out << "false\n";
            }
            else
            {
               out << "true\n";
            }
         }
         break;
      }

      case OTIFFTAG_PHOTOMETRIC: // tag 262
      {
         out << prefix << "photo_interpretation: ";

         if ( (count == 1) && (type == OTIFF_SHORT) )
         {
            ossim_uint16 s;
            getArrayValue(s, valueArray, 0);
            if (s <= OPHOTO_CIELAB)
            {
               out << PHOTO_INTERP[s] << "\n";
            }
            else
            {
               out <<"range error!\n";
            }
         }
         break;
      }

      case OTIFFTAG_FILLORDER: // tag 266
      {
         if ( (count == 1) && (type == OTIFF_SHORT) )
         {
            out << prefix << "fill_order: ";
            ossim_uint16 s;
            getArrayValue(s, valueArray, 0);
            out << s << "\n";

         }
         break;
      }

      case OTIFFTAG_IMAGEDESCRIPTION: // tag 270
      {
         out << prefix << "image_description: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case OTIFFTAG_STRIPOFFSETS: // tag 273
      {
         if (traceDump())
         {
            out << prefix << "bytes_per_strip: ";
            
            if (count == 1)
            {
               printValue(out, type, valueArray);
            }
            else
            {
               printArray(out, type, count, valueArray);
            }
         }
         break;
      }
      case OTIFFTAG_ORIENTATION: // tag 274
      {
         ossim_uint16 code;
         getArrayValue(code, valueArray, 0);
         printOrientation(out, prefix, code);
         break;
      }
      
      case OTIFFTAG_SAMPLESPERPIXEL: // tag 277
      {
         out << prefix << "samples_per_pixel: ";
         printValue(out, type, valueArray);
         break;
      }

      case OTIFFTAG_ROWSPERSTRIP: // tag 278
      {
         out << prefix << "rows_per_strip: ";
         printValue(out, type, valueArray);
         break;
      }

      case OTIFFTAG_STRIPBYTECOUNTS: // tag 279
      {
         if (traceDump())
         {
            out << prefix << "bytes_per_strip: ";
            
            if (count == 1)
            {
               printValue(out, type, valueArray);
            }
            else
            {
               printArray(out, type, count, valueArray);
            }
         }
         break;
      }
      
      case OTIFFTAG_MINSAMPLEVALUE: // tag 280
      {
         out << prefix << "min_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_MAXSAMPLEVALUE: // tag 281
      {
         out << prefix << "max_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      
      case OTIFFTAG_XRESOLUTION: // tag 282
      {
         out << prefix << "xresolution: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_YRESOLUTION: // tag 283
      {
         out << prefix << "yresolution: ";
         printValue(out, type, valueArray);
         break;
      }
      
      case OTIFFTAG_PLANARCONFIG: // tag 284
      {
         if ( (count == 1) && (type == OTIFF_SHORT) )
         {
            out << prefix << "planar_configuration: ";
            ossim_uint16 v;
            getArrayValue(v, valueArray, 0);
            if (v == 1)
            {
               out << "single image plane\n";
            }
            else if (v == 2)
            {
               out << "separate image planes\n";
            }
            else
            {
               out << "unknown planar value!\n";
            }
         }
         break;
      }

      case OTIFFTAG_RESOLUTIONUNIT: // tag 296
      {
         out << prefix << "resolution_units: ";
         ossim_uint16 v;
         getArrayValue(v, valueArray, 0);
         if (v == 2) 
         {
            out << "inch\n";
         }
         else if (v == 3)
         {
            out << "cm\n";
         }
         else
         {
            out << "none\n";
         }
         break;
      }

      case OTIFFTAG_PAGENUMBER: // tag 297
      {
         if ( (count == 2) && (type == OTIFF_SHORT) )
         {
            out << prefix << "page_number: ";
            ossim_uint16 s;
            getArrayValue(s, valueArray, 0);
            out << s << "\n";
            out << prefix << "total_pages: ";
            getArrayValue(s, valueArray, 1);
            out << s << "\n";
         }
         break;
      }
      
      case OTIFFTAG_SOFTWARE: // tag 305
      {
         out << prefix << "software: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case OTIFFTAG_DATETIME: // tag 306
      {
         out << prefix << "date_time: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case OTIFFTAG_ARTIST: // tag 315
      {
         out << prefix << "artist: ";
         printArray(out, type, count, valueArray);
         break;
      }
      
      case OTIFFTAG_TILEWIDTH: // tag 322
      {
         out << prefix << "tile_width: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_TILELENGTH: // tag 323
      {
         out << prefix << "tile_length: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_TILEOFFSETS: // tag 324
      {
         if (traceDump())
         {
            out << prefix << "tile_offsets: ";
            if (count == 1)
            {
               printValue(out, type, valueArray);
            }
            else
            {
               printArray(out, type, count, valueArray);
            }
         }
         break;
      }
      case OTIFFTAG_TILEBYTECOUNTS: // tag 325
      {
         if (traceDump())
         {
            out << prefix << "tile_byte_counts: ";
            if (count == 1)
            {
               printValue(out, type, valueArray);
            }
            else
            {
               printArray(out, type, count, valueArray);
            }
         }
         break;
      }
      case OTIFFTAG_SUBIFD: // tag 330
      {
         if ( (count == 1) && (type == OTIFF_IFD8) )
         {
            out << prefix << "subimage_descriptor: ";
            ossim_uint64 v;
            getArrayValue(v, valueArray, 0);
            out << v << "\n";
         }
         else
         {
            out << prefix << "tag 330 unhandled condition.\n";
         }
         break;
      }
      case OTIFFTAG_EXTRASAMPLES: // tag 338
      {
         out << prefix << "extra_samples: ";
         ossim_uint16 v;
         getArrayValue(v, valueArray, 0);
         switch (v)
         {
            case 1:
            {
               out << "associated_alpha_data\n";
               break;
            }
            case 2:
            {
               out << "unassociated_alpha_data\n";
               break;
            }
            default:
            {
               out << "unspecified_data\n";
               break;
            }
         }
         break;
      }
      case OTIFFTAG_SAMPLEFORMAT: // tag 339
      {
         out << prefix << "sample_format: ";
         
         if (count == 1)
         {
            printValue(out, type, valueArray);
         }
         else if (valueArray)
         {
            printArray(out, type, count, valueArray);
         }
         for (ossim_uint64 i = 0; i < count; ++i)
         {
            std::ostringstream s;
            s << "sample_format_string";
            if (count > 1)
            {
               s << i;
            }
            out << prefix << s.str() << ": ";
            
            ossim_uint16 v;
            getArrayValue(v, valueArray, i);
            switch (v)
            {
               case OSAMPLEFORMAT_UINT:
                  out << "unsigned integer data\n";
                  break;
               case OSAMPLEFORMAT_INT:
                  out << "signed integer data\n";
                  break;
               case OSAMPLEFORMAT_IEEEFP:
                  out << "IEEE floating point data\n";
                  break;
               case OSAMPLEFORMAT_COMPLEXINT:
                  out << "complex signed int\n";
                  break;
               case OSAMPLEFORMAT_COMPLEXIEEEFP:
                  out << "complex ieee floating\n";
                  break;
               case OSAMPLEFORMAT_VOID:
               default:
                  out << "untyped data\n";
                  break;
            }
         }
         break;
      }
      case OTIFFTAG_SMINSAMPLEVALUE: // tag 340
      {
         out << prefix << "smin_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_SMAXSAMPLEVALUE: // tag 341
      {
         out << prefix << "smax_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      case OTIFFTAG_COPYRIGHT: // tag 33432
      {
         out << prefix << "copyright: ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OMODEL_PIXEL_SCALE_TAG: // tag 33550
      {
         out << prefix << MODEL_PIXEL_SCALE_KW << ": ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OMODEL_TIE_POINT_TAG: // tag 33992
      {
         out << prefix << MODEL_TIE_POINT_KW << ": ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OMODEL_TRANSFORM_TAG: // tag 34264
      {
         out << prefix << MODEL_TRANSFORM_KW << ": ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OTIFFTAG_PHOTOSHOP:  // tag 34377
      {
         out << prefix << "photoshop_image_resource_blocks: found\n";
         break;
      }
      case OGEO_DOUBLE_PARAMS_TAG: // tag 34736
      {
         out << prefix << "double_params: ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OGEO_ASCII_PARAMS_TAG: // tag 34737
      {
         out << prefix << "ascii_params: ";
         printArray(out, type, count, valueArray);
         break;
      }
      case OGDAL_METADATA_TAG: // tag  42112
      {
         printGdalMetadata(out, prefix, count, valueArray);
         break;
      }
      case OGDAL_NODATA: // tag 42113
      {
         out << prefix << "gdal_nodata: ";
         printArray(out, type, count, valueArray);
         break;
      }
      default:
      {
         out << prefix << "unhandled_tag: " << tag << "\n";
         if (traceDebug())
         {
             ossimNotify(ossimNotifyLevel_DEBUG)
                << "generic:"
                << "\ntag[" << tagIdx << "]:         " << tag
                << "\ntype:                " << type
                << "\ncount:        " << count
                << "\narray size in bytes: " << arraySizeInBytes
                << "\n";
            printArray(out, type, count, valueArray);
         }
         break;
      }

   } // end of switch on tag...

   return out;

} // end of print

std::ostream& ossimTiffInfo::printValue(std::ostream& out,
                                        ossim_uint16 type,
                                        ossim_uint8* valueArray) const
{
   switch (type)
   {
      case OTIFF_BYTE:
      {
         ossim_uint8 v;
         getArrayValue(v, valueArray, 0);
         out << (ossim_uint16)v << "\n";
         break;
      }
      case OTIFF_SHORT:
      {
         ossim_uint16 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case OTIFF_SSHORT:
      {
         ossim_sint16 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case OTIFF_LONG:
      {
         ossim_uint32 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case OTIFF_RATIONAL: // two longs first=numerator, second=denominator
      {
         ossim_uint32 num;
         ossim_uint32 den;
         
         getArrayValue(num, valueArray, 0);
         getArrayValue(den, valueArray, 1);
         
         // out << num << " / " << den << "\n";
         out << (num/den) << "\n";
         
         break;
         
      }
      case OTIFF_SLONG:
      {
         ossim_sint32 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case OTIFF_FLOAT:
      {
         ossim_float32 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case OTIFF_DOUBLE:
      {
         ossim_float64 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      default:
      {
         out << "print_value_unhandled type: " << type << "\n";
         break;
      }
   }
   return out;
}

std::ostream& ossimTiffInfo::printArray(std::ostream& out,
                                        ossim_uint16 type,
                                        ossim_uint64 arraySizeInBytes,
                                        ossim_uint8* valueArray) const
{
   if (valueArray)
   {
      switch (type)
      {
         case OTIFF_BYTE:
         {
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << ((ossim_uint8)valueArray[i]);
            }
            out << "\n";
            break;
         }
         case OTIFF_ASCII:
         {
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               //---
               // Test to avoid putting nulls in the ascii string out.  Added to fix
               // ossimKeywordlist::parseStream returning false on trailing null Where array was
               // tagged as 11 bytes and ascii string was OrthoVista(10 bytes) and 11 byte was
               // ascii NUL '\0'.
               //---
               if ( valueArray[i] != 0 )
               {
                  out << ((char)valueArray[i]);
               }
            }
            out << "\n";
            break;
         }
         case OTIFF_SHORT:
         {
            ossim_uint16* p = (ossim_uint16*)valueArray;
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << p[i] << " ";
            }
            out << "\n";
            break;
         }
         case OTIFF_LONG:
         {
            ossim_uint32* p = (ossim_uint32*)valueArray;
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << p[i] << " ";
            }
            out << "\n";
            break;
         }
         case OTIFF_DOUBLE:
         {
            ossim_float64* p = (ossim_float64*)valueArray;
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << p[i] << " ";
            }
            out << "\n";
            break;
         }
         default:
         {
            out << "print_array_unhandled type: " << type << "\n";
            break;
         }
      }
   }
   else
   {
      out << "null array passed to ossimTiffInfo::printArray method." << "\n";
   }
   return out;
}

std::ostream& ossimTiffInfo::printGdalMetadata(std::ostream& out,
                                               const std::string& prefix,
                                               ossim_uint64 count,
                                               ossim_uint8* valueArray) const
{
   ossimString xmlString(valueArray, valueArray+count);
   ossimRefPtr<ossimXmlNode> xmlNode = new ossimXmlNode();
   std::istringstream in(xmlString);
   if(xmlNode->read(in))
   {
      const ossimXmlNode::ChildListType& children = xmlNode->getChildNodes();
      
      ossim_uint32 idx = 0;
      for(idx = 0; idx < children.size(); ++idx)
      {
         out << prefix << "gdalmetadata."
             << ossimString(children[idx]->getAttributeValue("name")).downcase()
             << ":" << children[idx]->getText() << std::endl;
      }
   }
   return out;
}

std::ostream& ossimTiffInfo::printGeoKeys(
   std::ostream& out,
   const std::string&  prefix,
   ossim_uint64   geoKeyLength,
   ossim_uint16*  geoKeyBlock,
   ossim_uint64   geoDoubleLength,
   ossim_float64* geoDoubleBlock,
   ossim_uint64   geoAsciiLength,
   ossim_int8*    geoAsciiBlock) const
{
   if (geoKeyLength && geoKeyBlock)
   {
      //---
      // Length passed in is the total number of shorts in the geo key
      // directory.  Each key has four short values; hence, "length/4".
      //---
      ossim_int32 index = 0;
      ossim_int32 tagCount = static_cast<ossim_int32>(geoKeyLength/4);
      for (ossim_int32 i = 0; i < tagCount; ++i)
      {
         //---
         // Each key contains four unsigned shorts:
         // GeoKey ID
         // TIFF Tag ID or 0
         // GeoKey value count
         // value or tag offset
         //---
         ossim_uint16 key   = geoKeyBlock[index++];
         ossim_uint16 tag   = geoKeyBlock[index++];
         ossim_uint16 count = geoKeyBlock[index++];
         ossim_uint16 code  = geoKeyBlock[index++];

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG  ossimTiffInfo::printGeoKeys"
               << "\nKey index:  " << i
               << "\ngeo key:  " << key
               << "\ntag:      " << tag
               << "\ncount:    " << count
               << "\ncode:     " << code
               << "\n";
         }

         switch (key)
         {
            case OGT_MODEL_TYPE_GEO_KEY:  // key 1024 Section 6.3.1.1 Codes
            {
               printModelType(out, prefix, code);
               break;
            }               
            case OGT_RASTER_TYPE_GEO_KEY:  // key 1025 Section 6.3.1.2 Code
            {
               printRasterType(out, prefix, code);
               break;
            }

            case OGT_CITATION_GEO_KEY: // key 1026
            {
               if (tag == 34737) // using ascii array
               {
                  if (geoAsciiBlock && ( (code+count) <= geoAsciiLength ) )
                  {
                     std::string s;
                     int i = 0;
                     while (i < count)
                     {
                        s.push_back(geoAsciiBlock[code+i]);
                        ++i;
                     }
                     out << prefix << "citation: " << s << "\n";
                  }
               }
               break;
            }

            case OGEOGRAPHIC_TYPE_GEO_KEY:  // key 2048  Section 6.3.2.1 Codes
            {
               out << prefix << ossimKeywordNames::GCS_CODE_KW << ": "
                   << code << "\n";
               break;
            }

            case OGEOG_CITATION_GEO_KEY: // key 2049
            {
               if (tag == 34737) // using ascii array
               {
                  if (geoAsciiBlock && ( (code+count) <= geoAsciiLength ) )
                  {
                     std::string s;
                     int i = 0;
                     while (i < count)
                     {
                        s.push_back(geoAsciiBlock[code+i]);
                        ++i;
                     }
                     out << prefix << "geographic_citation: " << s << "\n";
                  }
               }
               break;
            }

            case OGEOG_GEODETIC_DATUM_GEO_KEY:// key 2050 Section 6.3.2.2 Codes
            {
               out << prefix << GEODETIC_DATUM_KW << ": " << code << "\n";
               break;
            }

            case OGEOG_PRIME_MERIDIAN_GEOKEY:// key 2051 Section 6.3.2.4 Codes
            {
               out << prefix << "prime_meridian_code: " << code << "\n";
               break;
            }
            
            case OGEOG_LINEAR_UNITS_GEO_KEY:// key 2052  Section 6.3.1.3 Codes
            {
               out << prefix << "linear_units_code: " << code << "\n";
               printLinearUnits(out, prefix, LINEAR_UNITS_KW, code);
               break;
            }
            
            case OGEOG_ANGULAR_UNITS_GEO_KEY:// key 2054  Section 6.3.1.4 Codes
            {
               out << prefix << "angular_units_code: " << code << "\n";
               printAngularUnits(out, prefix, code);
               break;
            }

            case OGEOG_ANGULAR_UNIT_SIZE_GEO_KEY:// key 2055 Size in radians Section 6.2.2
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "angular_units_size_radians: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OGEOG_ELLIPSOID_GEO_KEY:// key 2056  Section 6.3.23 Codes
            {
               out << prefix << "ellipsoid_code: " << code << "\n";
               break;
            }
            
            case OGEOG_SEMI_MAJOR_AXIS: // key 2057
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "semi_major_axis: " << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OGEOG_SEMI_MINOR_AXIS: // key 2058
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "semi_minor_axis: "  << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OGEOG_INV_FLATTENING_GEO_KEY: // key 2059 ratio Section 6.2.2
            {
               if (tag == 34736) // using double array
               {  
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "inverse_flattening_ratio: " << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OGEOG_PRIME_MERIDIAN_LONG_GEO_KEY: // key 2061 GeogAngularUnit Section 6.2.2
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "prime_meridian_longitude: " << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJECTED_CS_TYPE_GEO_KEY: // key 3072 Section 6.3.3.1 codes
            {
               out << prefix << "pcs_code: " << code << "\n";
               break;
            }

            case OPCS_CITATION_GEO_KEY: // key 3073 ascii
            {
               if (tag == 34737) // using ascii array
               {
                  if (geoAsciiBlock && ( (code+count) <= geoAsciiLength ) )
                  {
                     std::string s;
                     int i = 0;
                     while (i < count)
                     {
                        s.push_back(geoAsciiBlock[code+i]);
                        ++i;
                     }
                     out << prefix << "pcs_citation: " << s << "\n";
                  }
               }
               
               break;
            }
      
            case OPROJECTION_GEO_KEY: // key 3074 Section 6.3.3.2 codes
            {
               out << prefix << "proj_code: " << code << "\n";
               break;
            }
            
            case OPROJ_COORD_TRANS_GEO_KEY:  // key 3075 Section 6.3.3.3 codes
            {
               out << prefix << COORD_TRANS_CODE_KW << ": " << code << "\n";
               printCoordTrans(out, prefix, code);
               break;
            }
         
            case OPROJ_LINEAR_UNITS_GEO_KEY:  // key 3076 Section 6.3.1.3 codes
            {
               out << prefix << "linear_units_code: " << code << "\n";
               printLinearUnits(out, prefix, std::string("linear_units"), code);
               break;
            }

            case OPROJ_LINEAR_UNIT_SIZE_GEO_KEY:  // key 3077 meters Section 6.2.3
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "linear_units_size: " << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJ_STD_PARALLEL1_GEO_KEY:  // key 3078
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << ossimKeywordNames::STD_PARALLEL_1_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
         
            case OPROJ_STD_PARALLEL2_GEO_KEY:  // key 3079
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << ossimKeywordNames::STD_PARALLEL_2_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
         
            case OPROJ_NAT_ORIGIN_LONG_GEO_KEY:  // key 3080
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << ORIGIN_LONGITUDE_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
               
            case OPROJ_NAT_ORIGIN_LAT_GEO_KEY:  // key 3081
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << ORIGIN_LATITUDE_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJ_FALSE_EASTING_GEO_KEY:  // key 3082
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << ossimKeywordNames::FALSE_EASTING_KW<< ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJ_FALSE_NORTHING_GEO_KEY:  // key 3083
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << ossimKeywordNames::FALSE_NORTHING_KW
                         << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OPROJ_FALSE_ORIGIN_LONG_GEO_KEY:  // key 3084
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << FALSE_ORIGIN_LONGITUDE_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OPROJ_FALSE_ORIGIN_LAT_GEO_KEY:  // key 3085
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         << FALSE_ORIGIN_LATITUDE_KW << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OPROJ_FALSE_ORIGIN_EASTING_GEO_KEY:  // key 3086
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         <<  FALSE_ORIGIN_EASTING_KW<< ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case OPROJ_FALSE_ORIGIN_NORTHING_GEO_KEY:  // key 3087
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix
                         <<  FALSE_ORIGIN_NORTHING_KW<< ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
           
            case OPROJ_CENTER_LONG_GEO_KEY:  // key 3088
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "center_longitude: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJ_CENTER_LAT_GEO_KEY:  // key 3089
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "center_latitude: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OPROJ_SCALE_AT_NAT_ORIGIN_GEO_KEY:  // key 3092
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << ossimKeywordNames::SCALE_FACTOR_KW
                         << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            case OPROJ_SCALE_AT_CENTER_GEO_KEY:  // key 3093
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << ossimKeywordNames::SCALE_FACTOR_KW
                         << ": "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case OVERTICAL_CITATION_GEO_KEY: // key 4097
            {               
               if (tag == 34737) // using ascii array
               {
                  if (geoAsciiBlock && ( (code+count) <= geoAsciiLength ) )
                  {
                     std::string s;
                     int i = 0;
                     while (i < count)
                     {
                        s.push_back(geoAsciiBlock[code+i]);
                        ++i;
                     }
                     out << prefix << "vertical_citation: " << s << "\n";
                  }
               }
               break;
            }

            case OVERTICAL_UNITS_GEO_KEY: // key 4099  Section 6.3.1.3 Codes
            {
               out << prefix << "vertical_units_code: " << code << "\n";
               printLinearUnits(out, prefix, VERTICAL_UNITS_KW, code);
               break;
            }
 
            default:
            {
               if (key > 1)
               {
                  if (tag == 34737) // using ascii array
                  {
                     if (geoAsciiBlock && ( (code+count) <= geoAsciiLength ) )
                     {
                        std::string s;
                        int i = 0;
                        while (i < count)
                        {
                           s.push_back(geoAsciiBlock[code+i]);
                           ++i;
                        }
                        out << prefix << "key_" << key << ": " << s << "\n";
                     }
                  }
                  else
                  {
                     out << prefix << "unhandle_key: " << key << "\n";
                  }
               }
               break;
            }
            
         } // matches: switch(key)

      } //  for (ossim_int32 i = 0; i < tagCount; ++i)
      
      if (traceDebug())
      {
         if (geoAsciiBlock && geoAsciiLength)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimTiffInfo::printGeoKeys DEBUG: geo ascii block: ";
            std::string s(geoAsciiBlock, static_cast<std::string::size_type>(geoAsciiLength));
            ossimNotify(ossimNotifyLevel_DEBUG)<< s << "\n";
         }
      } 
      
   } // if (geoKeyLength && geoKeyBlock)
   
   return out;
}


std::ostream& ossimTiffInfo::printModelType(std::ostream& out,
                                            const std::string& prefix,
                                            ossim_uint16 code) const
{
   // key 1024 Section 6.3.1.1 Codes
   out << prefix << MODEL_TYPE_KW << ": ";
   if (code == 1)
   {
      out << "projected\n";
   }
   else if (code == 2)
   {
      out << "geographic\n";
   }
   else if (code == 2)
   {
      out << "geocentric\n";
   }
   else
   {
      out << "unknown\n";
   }
   return out;
}

std::ostream& ossimTiffInfo::printRasterType(std::ostream& out,
                                             const std::string& prefix,
                                             ossim_uint16 code) const
{
   // key 1025 Section 6.3.1.2 Codes
   out << prefix << RASTER_TYPE_KW << ": ";
   if (code == 1)
   {
      out << "pixel_is_area\n";
   }
   else if (code == 2)
   {
      out << "pixels_is_point\n";
   }
   else
   {
      out << "unknown\n";
   }
   return out;
}

std::ostream& ossimTiffInfo::printAngularUnits(std::ostream& out,
                                               const std::string& prefix,
                                               ossim_uint16 code) const
{
   // key 2054 Section 6.3.1.4 Codes
   out << prefix << ANGULAR_UNITS_KW << ": ";
   switch (code)
   {
      case 9101:
      {
         out << "radians\n";
         break;
      }
      case 9102:
      {
         out << "degrees\n";
         break;
      }
      case 9103:
      {
         out << "arc_minutes\n";
         break;
      }
      case 9104:
      {
         out << "arc_seconds\n";
         break;
      }
      case 9105:
      {
         out << "grad\n";
         break;
      }
      case 9106:
      {
         out << "gon\n";
         break;
      }
      case 9107:
      {
         out << "dms\n";
         break;
      }
      case 9108:
      {
         out << "dms_hemisphere\n";
         break;
      }
      default:
      {
         out << "unknown\n";
         break;
      }
   }
   return out;
}

std::ostream& ossimTiffInfo::printCoordTrans(std::ostream& out,
                                             const std::string& prefix,
                                             ossim_uint16 code) const
{
   // key 3075 Section 6.3.3.3 Codes
   out << prefix << "coord_trans: ";
   switch (code)
   {
      case 1:
      {
         out << "TransverseMercator\n";
         break;
      }
      case 2:
      {
         out << "TransvMercator_Modified_Alaska\n";
         break;
      }
      case 3:
      {
         out << "ObliqueMercator\n";
         break;
      }
      case 4:
      {
         out << "ObliqueMercator_Laborde\n";
         break;
      }
      case 5:
      {
         out << "ObliqueMercator_Rosenmund\n";
         break;
      }
      case 6:
      {
         out << "ObliqueMercator_Spherical\n";
         break;
      }
      case 7:
      {
         out << "Mercator\n";
         break;
      }
      case 8:
      {
         out << "LambertConfConic_2SP\n";
         break;
      }
      case 9:
      {
         out << "LambertConfConic_Helmert\n";
         break;
      }
      case 10:
      {
         out << "LambertAzimEqualArea\n";
         break;
      }
      case 11:
      {
         out << "AlbersEqualArea\n";
         break;
      }
      case 12:
      {
         out << "AzimuthalEquidistant\n";
         break;
      }
      case 13:
      {
         out << "EquidistantConic\n";
         break;
      }
      case 14:
      {
         out << "Stereographic\n";
         break;
      }
      case 15:
      {
         out << "PolarStereographic\n";
         break;
      }
      case 16:
      {
         out << "ObliqueStereographic\n";
         break;
      }
      case 17:
      {
         out << "Equirectangular\n";
         break;
      }
      case 18:
      {
         out << "CassiniSoldner\n";
         break;
      }
      case 19:
      {
         out << "Gnomonic\n";
         break;
      }
      case 20:
      {
         out << "MillerCylindrical\n";
         break;
      }
      case 21:
      {
         out << "Orthographic\n";
         break;
      }
      case 22:
      {
         out << "Polyconic\n";
         break;
      }
      case 23:
      {
         out << "Robinson\n";
         break;
      }
      case 24:
      {
         out << "Sinusoidal\n";
         break;
      }
      case 25:
      {
         out << "VanDerGrinten\n";
         break;
      }
      case 26:
      {
         out << "NewZealandMapGrid\n";
         break;
      }
      case 27:
      {
         out << "TransvMercator_SouthOriented\n";
         break;
      }
      default:
      {
         out << code << " unknown\n";
         break;
      }
   }
   return out;
}

std::ostream& ossimTiffInfo::printLinearUnits(std::ostream& out,
                                              const std::string& prefix,
                                              const std::string& key,
                                              ossim_uint16 code) const
{
   // key 3076 Section 6.3.1.3 Codes
   out << prefix << key << ": ";
   switch (code)
   {
      case 9001:
      {
         out << "meters\n";
         break;
      }
      case 9002:
      {
         out << "feet\n";
         break;
      }
      case 9003:
      {
         out << "us_survey_feet\n";
         break;
      }
      case 9004:
      {
         out << "foot_modified_american\n";
         break;
      }
      case 9005:
      {
         out << "foot_clarke\n";
         break;
      }
      case 9006:
      {
         out << "foot_indian\n";
         break;
      }
      case 9007:
      {
         out << "link\n";
         break;
      }
      case 9008:
      {
         out << "link_benoit\n";
         break;
      }
      case 9009:
      {
         out << "link_sears\n";
         break;
      }
      case 9010:
      {
         out << "chain_benoit\n";
         break;
      }
      case 9011:
      {
         out << "chain_sears\n";
         break;
      }
      case 9012:
      {
         out << "yard_sears\n";
         break;
      }
      case 9013:
      {
         out << "yard_indian\n";
         break;
      }
      case 9014:
      {
         out << "fathom\n";
         break;
      }
      case 9015:
      {
         out << "mile_international_nautical\n";
         break;
      }
      default:
      {
         out << code << " unknown\n";
         break;
      }
   }
   return out;
}

std::ostream& ossimTiffInfo::printOrientation(std::ostream& out,
                                              const std::string& prefix,
                                              ossim_uint16 code) const
{
   // Tag 274:
   out << prefix << "orientation: ";
   switch (code)
   {
      case 1:
      {
         out << "top_left\n";
         break;
      }
      case 2:
      {
         out << "top_right\n";
         break;
      }
      case 3:
      {
         out << "bottom_right\n";
         break;
      }
      case 4:
      {
         out << "bottom_left\n";
         break;
      }
      case 5:
      {
         out << "left_top\n";
         break;
      }
      case 6:
      {
         out << "right_top\n";
         break;
      }
      case 7:
      {
         out << "right_bottom\n";
         break;
      }
      case 8:
      {
         out << "left_bottom\n";
         break;
      }
      default:
      {
         out << code << " unknown\n";
         break;
      }
   }
   return out;
}

void ossimTiffInfo::getDirPrefix(ossim_int32 index,
                                 std::string& prefix) const
{
   prefix += "image";
   std::ostringstream s;
   s << index;
   prefix += s.str();
   prefix += ".";
}

bool ossimTiffInfo::getPixelScale(const ossimString& gtiffPrefix,
                                  const ossimKeywordlist& gtiffKwl,
                                  ossimDpt& scale) const
{
   bool result = false;

   std::vector<ossim_float64> floats;
   if ( getFloats(gtiffPrefix, MODEL_PIXEL_SCALE_KW, gtiffKwl, floats) )
   {
      if ( floats.size() == 3 )
      {
         scale.x = floats[0];
         scale.y = floats[1];
         result = true;
      }
   }

   return result;
}

bool ossimTiffInfo::getTiePoint(const ossimString& gtiffPrefix,
                                const ossimKeywordlist& gtiffKwl,
                                std::vector<ossim_float64>& ties) const
{
   return getFloats(gtiffPrefix, MODEL_TIE_POINT_KW, gtiffKwl, ties);
}

bool ossimTiffInfo::getModelTransform(const ossimString& gtiffPrefix,
                                      const ossimKeywordlist& gtiffKwl,
                                      std::vector<ossim_float64>& xfrm) const
{
   return getFloats(gtiffPrefix, MODEL_TRANSFORM_KW, gtiffKwl, xfrm);
}

bool ossimTiffInfo::getFloats(const ossimString& gtiffPrefix,
                              const ossimString& key,
                              const ossimKeywordlist& gtiffKwl,
                              std::vector<ossim_float64>& floats) const
{
   bool result = false;
   
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), key.c_str());
   if ( lookup )
   {
      ossimString line = lookup;
      result  = getFloats(line, floats);
   }

   return result; 
}

bool ossimTiffInfo::getFloats(const ossimString& line,
                              std::vector<ossim_float64>& floats) const
{
   bool result = false;
   
   floats.clear();
   
   if (line.size())
   {
      ossim_float64 f;
      std::istringstream is(line);

      is >> f; // Get the first double.
      while ( ! is.fail() )
      {
         floats.push_back(f);
         is >> f;
      }
   }

   if ( floats.size() )
   {
      result = true;
   }

   return result;
}

bool ossimTiffInfo::getDatumCode(const ossimString& gtiffPrefix,
                                 const ossimKeywordlist& gtiffKwl,
                                 ossimString& datumCode) const
{
   bool result = false;
   
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), GEODETIC_DATUM_KW.c_str());

   // There is some ambiguity in the definition of EPSG GCS code. Here both the datum code (6000-
   // series) and projection code (4000-series) are tested to get to the datum's native ascii code:

   // Look for GEODETIC_DATUM_KW code first:
   if (lookup)
   {
      ossim_int32 code = ossimString(lookup).toInt32();
      result = getDatumCode( code, datumCode );
   }

   if ( !result )
   {
      // Try GCS_CODE_KW:
      lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::GCS_CODE_KW);
      if ( lookup )
      {
         ossim_int32 code = ossimString(lookup).toInt32();
         result = getDatumCode( code, datumCode );
      }
   }
   return result;
}

bool ossimTiffInfo::getDatumCode(ossim_int32 code, ossimString& datumCode) const
{
   bool result = false;
   datumCode.clear();
   switch(code)
   {
      case 4267:
      case 6267:   
      {
         datumCode = "NAS-C";
         break;
      }
      case 4269:
      case 6269:
      {
         datumCode = "NAR-C";
         break;
      }
      case 4322:
      case 6322:
      {
         datumCode = "WGD";
         break;
      }
      case 4326:
      case 6326:
      {
         datumCode = "WGE";
         break;
      }
      
   } // matches: switch(code)

   if ( datumCode.size() )
   {
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getPcsCode(const ossimString& gtiffPrefix,
                               const ossimKeywordlist& gtiffKwl,
                               ossimString& pcsCode) const
{
   bool result = false;

   // Check for key "pcs_code":
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::PCS_CODE_KW);
   if (lookup)
   {
      pcsCode = lookup;
      
      ossim_uint32 code = pcsCode.toUInt32();
      if ( code != 32767 )
      {
         // See if we handle this code in our projection factories.
         ossimRefPtr<ossimProjection>  proj = 
            ossimEpsgProjectionFactory::instance()->createProjection(pcsCode);
         if (proj.valid())
         {
            proj = 0;
            result = true;
         }
      }
      
   }

   if (result == false)
   {
      // Check for key "pcs_citation":
      lookup = gtiffKwl.find(gtiffPrefix.c_str(), "pcs_citation");
      if ( lookup )
      {
         ossimString spec = lookup;

         // Strip commonly found or bar '|' from end if present.
         spec.trim(ossimString("|"));

         // See if we handle this code in our projection factories.
         ossimRefPtr<ossimProjection>  proj =
            ossimEpsgProjectionFactory::instance()->createProjection(spec);
         if (proj.valid())
         {
            ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(proj.get());
            if ( mapProj )
            {
               ossim_uint32 intCode = mapProj->getPcsCode();
               if ( intCode != 32767 )
               {
                  proj = 0;
                  pcsCode = ossimString::toString(intCode);
                  result = true;
               }
            }
         }
      }
   }

   return result;
}

bool ossimTiffInfo::getUnits(const ossimString& gtiffPrefix,
                             const ossimKeywordlist& gtiffKwl,
                             ossimString& units) const
{
   bool result = true;
   
   ossimString linearUnits = "";
   getLinearUnits(gtiffPrefix, gtiffKwl, linearUnits);
   
   ossimString angularUnits = "";
   getAngularUnits(gtiffPrefix, gtiffKwl, angularUnits);

   ossimString modelType;
   getModelType(gtiffPrefix, gtiffKwl, modelType);
   
   if (modelType == "geographic")
   {
      if (angularUnits.size())
      {
         units = angularUnits;
      }
      else
      {
         units = "degrees";
      }
   }
   else if (modelType == "projected")
   {
      if (linearUnits.size())
      {
         units = linearUnits;
      }
      else
      {
         units = "meters";
      }
   }
   else
   {
      units = "meters";
   }

   return result;
}


bool ossimTiffInfo::getLinearUnits(const ossimString& gtiffPrefix,
                                   const ossimKeywordlist& gtiffKwl,
                                   ossimString& linearUnits) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), LINEAR_UNITS_KW.c_str());
   if (lookup)
   {
      linearUnits = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getVerticalUnits(const ossimString& gtiffPrefix,
                                     const ossimKeywordlist& gtiffKwl,
                                     ossimString& verticalUnits) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), VERTICAL_UNITS_KW.c_str());
   if (lookup)
   {
      verticalUnits = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getAngularUnits(const ossimString& gtiffPrefix,
                                    const ossimKeywordlist& gtiffKwl,
                                    ossimString& angularUnits) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ANGULAR_UNITS_KW.c_str());
   if (lookup)
   {
      angularUnits = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getPixelType(const ossimString& gtiffPrefix,
                                 const ossimKeywordlist& gtiffKwl,
                                 ossimString& pixelType) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), RASTER_TYPE_KW.c_str());
   if (lookup)
   {
      pixelType = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getModelType(const ossimString& gtiffPrefix,
                                 const ossimKeywordlist& gtiffKwl,
                                 ossimString& modelType) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), MODEL_TYPE_KW.c_str());
   if (lookup)
   {
      modelType = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getOssimProjectionName(const ossimString& gtiffPrefix,
                                           const ossimKeywordlist& gtiffKwl,
                                           ossimString& ossimProj) const
{
   bool result = false;
   ossimProj.clear();
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), COORD_TRANS_CODE_KW.c_str());
   if (lookup)
   {
      ossim_uint32 code = ossimString(lookup).toUInt32();

      ossimGeoTiffCoordTransformsLut lut;
      
      ossimProj = lut.getEntryString(code);
      
      if (ossimProj.size())
      {
         result = true;
      }
   }
   return result;
}

ossim_uint32 ossimTiffInfo::getLines(const ossimString& gtiffPrefix,
                                     const ossimKeywordlist& gtiffKwl) const
{
   ossim_uint32 result = 0;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), IMAGE_LENGTH_KW.c_str());
   if (lookup)
   {
      result = ossimString(lookup).toUInt32();
   }
   return result;
}

ossim_uint32 ossimTiffInfo::getSamples(const ossimString& gtiffPrefix,
                                       const ossimKeywordlist& gtiffKwl) const
{
   ossim_uint32 result = 0;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), IMAGE_WIDTH_KW.c_str());
   if (lookup)
   {
      result = ossimString(lookup).toUInt32();
   }   
   return result;
}

bool ossimTiffInfo::getStdParallelOne(const ossimString& gtiffPrefix,
                                      const ossimKeywordlist& gtiffKwl,
                                      ossimString& value) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::STD_PARALLEL_1_KW);
   if (lookup)
   {
      value = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getStdParallelTwo(const ossimString& gtiffPrefix,
                                      const ossimKeywordlist& gtiffKwl,
                                      ossimString& value) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::STD_PARALLEL_2_KW);
   if (lookup)
   {
      value = lookup;
      result = true;
   }
   return result;
}

bool ossimTiffInfo::getFalseEasting(const ossimString& gtiffPrefix,
                                    const ossimKeywordlist& gtiffKwl,
                                    ossimString& value) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::FALSE_EASTING_KW);
   if (lookup)
   {
      value = lookup;
      result = true;
   }
   return result; 
}

bool ossimTiffInfo::getFalseNorthing(const ossimString& gtiffPrefix,
                                     const ossimKeywordlist& gtiffKwl,
                                     ossimString& value) const
{
   bool result = false;
   const char* lookup = gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::FALSE_NORTHING_KW);
   if (lookup)
   {
      value = lookup;
      result = true;
   }
   return result; 
}

bool ossimTiffInfo::getFalseEastingNorthing(const ossimString& gtiffPrefix,
                                            const ossimKeywordlist& gtiffKwl,
                                            ossimDpt& eastingNorthing) const
{
   bool result = false;
   ossimString value;
   if ( getFalseEasting(gtiffPrefix, gtiffKwl, value) )
   {
      eastingNorthing.x = value.toFloat64();

      if ( getFalseNorthing(gtiffPrefix, gtiffKwl, value) )
      {
         eastingNorthing.y = value.toFloat64();
         result = true;
      }
   }
   return result;
}

bool ossimTiffInfo::getScaleFactor(const ossimString& gtiffPrefix,
                                   const ossimKeywordlist& gtiffKwl,
                                   ossim_float64& value) const
{
   bool result = false;

   const char* lookup =
      gtiffKwl.find(gtiffPrefix.c_str(), ossimKeywordNames::SCALE_FACTOR_KW);
   
   if (lookup)
   {
      value = ossimString(lookup).toFloat64();
      result = true;
   }

   return result; 
}

bool ossimTiffInfo::getOriginLat(const ossimString& gtiffPrefix,
                                 const ossimKeywordlist& gtiffKwl,
                                 ossim_float64& value) const
{
   bool result = false;

   //---
   // Not sure of the order of precedence here.
   //---
   const char* projOriginLatGeoKey =
      gtiffKwl.find(gtiffPrefix.c_str(), ORIGIN_LATITUDE_KW.c_str());

   //---
   // Go for this key first as it is used in geotiff spec example:
   // http://www.remotesensing.org/geotiff/spec/geotiff3.html#3.1.3.
   //---
   if (projOriginLatGeoKey)
   {
      value = ossimString(projOriginLatGeoKey).toFloat64();
      result = true;
   }
   else
   {
      const char* projCenterLatGeoKey =
         gtiffKwl.find(gtiffPrefix.c_str(), CENTER_LATITUDE__KW.c_str());

      if (projCenterLatGeoKey)
      {
         value = ossimString(projCenterLatGeoKey).toFloat64();
         result = true;
      }
      else
      {
         const char* projFalseOriginLatGeoKey =
            gtiffKwl.find(gtiffPrefix.c_str(),
                          FALSE_ORIGIN_LATITUDE_KW.c_str());

         if (projFalseOriginLatGeoKey)
         {
            //---
            // Seems the term "false" implies it should be added to the real
            // origin?
            //---
            value = ossimString(projFalseOriginLatGeoKey).toFloat64();
            result = true;
         }
      }
   }
   
   return result;
}

bool ossimTiffInfo::getCentralMeridian(const ossimString& gtiffPrefix,
                                       const ossimKeywordlist& gtiffKwl,
                                       ossim_float64& value) const
{
   bool result = false;

   //---
   // Not sure of the order of precedence here.
   //---
   const char* projCenterLongGeoKey =
      gtiffKwl.find(gtiffPrefix.c_str(), CENTER_LONGITUDE_KW.c_str());

   //---
   // Go for this key first as it is used in geotiff spec example:
   // http://www.remotesensing.org/geotiff/spec/geotiff3.html#3.1.3.
   //---
   if (projCenterLongGeoKey)
   {
      value = ossimString(projCenterLongGeoKey).toFloat64();
      result = true;
   }
   else
   {
      const char* projOriginLongGeoKey =
         gtiffKwl.find(gtiffPrefix.c_str(), ORIGIN_LONGITUDE_KW.c_str());

      if (projOriginLongGeoKey)
      {
         value = ossimString(projOriginLongGeoKey).toFloat64();
         result = true;
      }
      else
      {
         const char* projFalseOriginLongGeoKey =
            gtiffKwl.find(gtiffPrefix.c_str(),
                          FALSE_ORIGIN_LONGITUDE_KW.c_str());

         if (projFalseOriginLongGeoKey)
         {
            //---
            // Seems the term "false" implies it should be added to the real
            // origin?
            //---
            value = ossimString(projFalseOriginLongGeoKey).toFloat64();
            result = true;
         }
      }
   }
   
   return result;
}

void ossimTiffInfo::getTieSets(const std::vector<ossim_float64>& ties,
                               ossim_uint32 width,
                               ossim_uint32 height,
                               ossimTieGptSet& tieSet) const
{
   ossim_uint32 idx = 0;
   ossim_uint32 tieCount = (ossim_uint32)ties.size()/6;
   const double* tiePointsPtr = &ties.front();
   double offset = 0;
   if (hasOneBasedTiePoints(ties, width, height))
   {
      offset = -1.0;
   }
   
   for(idx = 0; idx < tieCount; ++idx)
   {
      ossimDpt xyPixel(tiePointsPtr[0]+offset, tiePointsPtr[1]+offset);
      // tie[3] = x, tie[4]
      ossimGpt gpt(tiePointsPtr[4], tiePointsPtr[3], tiePointsPtr[5]);
      
      tieSet.addTiePoint(new ossimTieGpt(gpt, xyPixel, .5));
      tiePointsPtr+=6;
   }
}

bool ossimTiffInfo::hasOneBasedTiePoints(
   const std::vector<ossim_float64>& ties,
   ossim_uint32 width, ossim_uint32 height) const
{
   bool result = false;
   
   // Assuming ties of (x,y,z,lat,lon,hgt) so size should be divide by 3.
   if (ties.size()%6)
   {
      return result;
   }
   
   ossim_float64 minX = 999999.0;
   ossim_float64 minY = 999999.0;
   ossim_float64 maxX = 0.0;
   ossim_float64 maxY = 0.0;

   const ossim_uint32 SIZE = (ossim_uint32)ties.size();
   ossim_uint32 tieIndex = 0;

   while (tieIndex < SIZE)
   {
      if ( ties[tieIndex]   < minX ) minX = ties[tieIndex];
      if ( ties[tieIndex]   > maxX ) maxX = ties[tieIndex];
      if ( ties[tieIndex+1] < minY ) minY = ties[tieIndex+1];
      if ( ties[tieIndex+1] > maxY ) maxY = ties[tieIndex+1];
      tieIndex += 6;
   }

   if ( (minX == 1) && (maxX == width) &&
        (minY == 1) && (maxY == height) )
   {
      result = true;
   }

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeoTiff::hasOneBasedTiePoints DEBUG:"
         << "\nminX:       " << minX
         << "\nmaxX:       " << maxX
         << "\nminY:       " << minY
         << "\nmaxY:       " << maxY
         << "\ntheWidth:   " << theWidth
         << "\ntheLength:  " << theLength
         << "\none based:  " << (result?"true":"false")
         << std::endl;
   }
#endif
   
   return result;
}
