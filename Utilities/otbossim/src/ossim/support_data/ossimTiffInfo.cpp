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
// $Id$
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ossim/support_data/ossimTiffInfo.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>

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

ossimTiffInfo::ossimTiffInfo()
   : ossimInfoBase(),
     theFile(),
     theEndian(0),
     theVersion(0)
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
      readShort(theVersion, str);

      if ( ( (byteOrder[0] == 'M') || (byteOrder[0] == 'I') ) &&
           ( (theVersion == 42) || (theVersion == 43) ) )
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
         out << MODULE << " Cannot open file:  " << theFile << std::endl;
      }
      return out;
   }
   
   //---
   // Get the byte order.  The data member "theTiffByteOrder" was set to
   // BIG_END in the initialization list.  This will be changed if
   // byte order is LITTLE_END(IBM, DEC).
   //---
   char byteOrder[2];
   str.read(byteOrder, 2); // Read the byte order.

   // Get the version:
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
   if (getOffset(seekOffset, str) == false)
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
      if (getValue(nTags, str, TWO_OR_EIGHT) == false)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " FATAL error reading number of direcories."
            << std::endl;
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
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " FATAL error reading tag number."
               << std::endl;
            str.close();
            return out;
         }

         //---
         // Get the type (byte, ascii, short...)
         //---
         readShort(type, str);
         if (!str)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " FATAL error reading type number."
               << std::endl;
            str.close();
            return out;
         }

         //---
         // Get the count.  This is not in bytes.  It is based on the
         // type.  So if the type is a short and the count is one then
         // read "sizeof(short"(2) bytes.
         //---
         getValue(count, str, FOUR_OR_EIGHT);
         if (!str)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " FATAL error reading count."
               << std::endl;
            str.close();
            return out;
         }

         // Get the array size in bytes.
         arraySizeInBytes = getArraySizeInBytes(count, type);
         if (arraySizeInBytes == 0)
         {
            // Could be an unhandle type.  Gobble the value.
            eatValue(str);
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
               getOffset(seekOffset, str);

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

         if (tag == GEO_KEY_DIRECTORY_TAG)
         {
            // tag 34735 save for printGeoKeys
            geoKeyBlock = reinterpret_cast<ossim_uint16*>(valueArray);
            geoKeyLength = count;
         }
         else if (tag == GEO_DOUBLE_PARAMS_TAG)
         {
            // tag 34736 save for printGeoKeys
            geoDoubleBlock  = reinterpret_cast<ossim_float64*>(valueArray);
            geoDoubleLength = count;
         }
         else if (tag == GEO_ASCII_PARAMS_TAG)
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
      if (getOffset(seekOffset, str) == false)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " No offset to an image file directory found.\n"
            << "Returning with error."
            << std::endl;
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



bool ossimTiffInfo::getOffset(std::streamoff& offset, std::ifstream& str) const
{
   bool status = true;
   if  (theVersion == 42)
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
                            WordType type) const
{
   bool status = true;
   if  (theVersion == 42)
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
   switch (type)
   {
      case TIFF_BYTE:
      case TIFF_ASCII:
      case TIFF_SBYTE:
      case TIFF_UNDEFINED:
         return 1;
         
      case TIFF_SHORT:
      case TIFF_SSHORT:
         return 2;
         
      case TIFF_LONG:
      case TIFF_SLONG:
      case TIFF_IFD:
         return 4;
         
      case TIFF_RATIONAL:
      case TIFF_SRATIONAL:
      case TIFF_FLOAT:
      case TIFF_DOUBLE:
      case 16:             // TIFF_LONG8 defined in big tiff only.
      case 17:             // TIFF_SLONG8 defined in big tiff only.
      case 18:             // TIFF_IFD8 defined in big tiff only.
         return 8;

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
   return 0;
}

void ossimTiffInfo::eatValue(std::ifstream& str) const
{
   if (theVersion == 42)
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
   v = p[position]; // +position);
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
      case TIFFTAG_SUBFILETYPE: // tag 254
      {
         out << prefix << "sub_file_type: ";
         printValue(out, type, valueArray);
         break;
      }
      case TIFFTAG_IMAGEWIDTH: // tag 256
      {
         out << prefix << "image_width: ";
         printValue(out, type, valueArray);
         break;
      }
      
      case TIFFTAG_IMAGELENGTH: // tag 257
      {
         out << prefix << "image length: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_BITSPERSAMPLE: // tag 258
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
         
      case TIFFTAG_COMPRESSION: // tag 259
      {
         if ( (count == 1) && (type == TIFF_SHORT) )
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

      case TIFFTAG_PHOTOMETRIC: // tag 262
      {
         out << prefix << "photo_interpretation: ";

         if ( (count == 1) && (type == TIFF_SHORT) )
         {
            ossim_uint16 s;
            getArrayValue(s, valueArray, 0);
            if (s <= PHOTO_CIELAB)
            {
               out << PHOTO_INTERP[s] << "\n";
            }
            else
            {
               out <<"range error!\n";
            }
            break;
         }
      }

      case TIFFTAG_IMAGEDESCRIPTION: // tag 270
      {
         out << prefix << "image_description: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case TIFFTAG_STRIPOFFSETS: // tag 273
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
      case TIFFTAG_ORIENTATION: // tag 274
      {
         ossim_uint16 code;
         getArrayValue(code, valueArray, 0);
         printOrientation(out, prefix, code);
         break;
      }
      
      case TIFFTAG_SAMPLESPERPIXEL: // tag 277
      {
         out << prefix << "samples_per_pixel: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_ROWSPERSTRIP: // tag 278
      {
         out << prefix << "rows_per_strip: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_STRIPBYTECOUNTS: // tag 279
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
      
      case TIFFTAG_MINSAMPLEVALUE: // tag 280
      {
         out << prefix << "min_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      case TIFFTAG_MAXSAMPLEVALUE: // tag 281
      {
         out << prefix << "max_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }
      
      case TIFFTAG_XRESOLUTION: // tag 282
      {
         out << prefix << "xresolution: ";
         printValue(out, type, valueArray);
         break;
      }
      case TIFFTAG_YRESOLUTION: // tag 283
      {
         out << prefix << "yresolution: ";
         printValue(out, type, valueArray);
         break;
      }
      
      case TIFFTAG_PLANARCONFIG: // tag 284
      {
         if ( (count == 1) && (type == TIFF_SHORT) )
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

      case TIFFTAG_RESOLUTIONUNIT: // tag 296
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
      case TIFFTAG_DATETIME: // tag 306
      {
         out << prefix << "date_time: ";
         printArray(out, type, count, valueArray);
         break;
      }
      case TIFFTAG_TILEWIDTH: // tag 322
      {
         out << prefix << "tile_width: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_TILELENGTH: // tag 323
      {
         out << prefix << "tile_length: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_TILEOFFSETS: // tag 324
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
      
      case TIFFTAG_TILEBYTECOUNTS: // tag 325
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

      case TIFFTAG_SUBIFD: // tag 330
      {
         if ( (count == 1) && (type == TIFF_IFD8) )
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
      
      case TIFFTAG_EXTRASAMPLES: // tag 338
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

      case TIFFTAG_SAMPLEFORMAT: // tag 339
      {
         if ( (count == 1) && (type == TIFF_SHORT) )
         {
            out << prefix << "sample_format: ";
            ossim_uint16 v;
            getArrayValue(v, valueArray, 0);
            switch (v)
            {
               case SAMPLEFORMAT_UINT:
                  out << "unsigned integer data\n";
                  break;
               case SAMPLEFORMAT_INT:
                  out << "signed integer data\n";
                  break;
               case SAMPLEFORMAT_IEEEFP:
                  out << "IEEE floating point data\n";
                  break;
               case SAMPLEFORMAT_COMPLEXINT:
                  out << "complex signed int\n";
                  break;
               case SAMPLEFORMAT_COMPLEXIEEEFP:
                  out << "complex ieee floating\n";
                  break;
               case SAMPLEFORMAT_VOID:
               default:
                  out << "untyped data\n";
                  break;
            }
         }
         break;
      }

      case TIFFTAG_SMINSAMPLEVALUE: // tag 340
      {
         out << prefix << "smin_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_SMAXSAMPLEVALUE: // tag 341
      {
         out << prefix << "smax_sample_value: ";
         printValue(out, type, valueArray);
         break;
      }

      case TIFFTAG_COPYRIGHT: // tag 33432
      {
         out << prefix << "copyright: ";
         printArray(out, type, count, valueArray);
         break;
      }
      
      case MODEL_PIXEL_SCALE_TAG: // tag 33550
      {
         out << prefix << "model_pixel_scale: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case MODEL_TIE_POINT_TAG: // tag 33992
      {
         out << prefix << "model_tie_point: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case MODEL_TRANSFORM_TAG: // tag 34264
      {
         out << prefix << "model_transform: ";
         printArray(out, type, count, valueArray);
         break;
      }

      case TIFFTAG_PHOTOSHOP:  // tag 34377
      {
         out << prefix << "photoshop_image_resource_blocks: found\n";
         break;
      }
      
      case GEO_DOUBLE_PARAMS_TAG: // tag 34736
      {
         out << prefix << "double_params: ";
         printArray(out, type, count, valueArray);
         break;
      }
      case GEO_ASCII_PARAMS_TAG: // tag 34737
      {
         out << prefix << "ascii_params: ";
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
      case TIFF_SHORT:
      {
         ossim_uint16 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case TIFF_SSHORT:
      {
         ossim_sint16 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case TIFF_LONG:
      {
         ossim_uint32 v;
         getArrayValue(v, valueArray, 0);
         out << v << "\n";
         break;
      }
      case TIFF_RATIONAL: // two longs first=numerator, second=denominator
      {
         ossim_uint32 num;
         ossim_uint32 den;
         
         getArrayValue(num, valueArray, 0);
         getArrayValue(den, valueArray, 1);
         
         // out << num << " / " << den << "\n";
         out << (num/den) << "\n";
         
         break;
         
      }
      case TIFF_SLONG:
      {
         ossim_sint32 v;
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
         case TIFF_BYTE:
         {
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << ((ossim_uint8)valueArray[i]);
            }
            out << "\n";
            break;
         }

         case TIFF_ASCII:
         {
            for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
            {
               out << ((char)valueArray[i]);
            }
            out << "\n";
            break;
         }
         case TIFF_SHORT:
          {
             ossim_uint16* p = (ossim_uint16*)valueArray;
             for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
             {
                out << p[i] << " ";
            }
            out << "\n";
            break;
         }
         case TIFF_LONG:
          {
             ossim_uint32* p = (ossim_uint32*)valueArray;
             for (ossim_uint64 i = 0; i < arraySizeInBytes; ++i)
             {
                out << p[i] << " ";
            }
            out << "\n";
            break;
         }
         case TIFF_DOUBLE:
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
            out
            << "DEBUG  ossimTiffInfo::readGeoKeys"
            << "\nKey index:  " << i
            << "\ngeo key:  " << key
            << "\ntag:      " << tag
            << "\ncount:    " << count
            << "\ncode:     " << code
            << "\n";

            if (geoAsciiBlock && geoAsciiLength)
            {
               out << "DEBUG: geo ascii block: ";
               std::string s(geoAsciiBlock,
                             static_cast<std::string::size_type>(
                                geoAsciiLength));
               out << s << "\n";
            }
         }

         switch (key)
         {
            case GT_MODEL_TYPE_GEO_KEY:  // key 1024 Section 6.3.1.1 Codes
            {
               printModelType(out, prefix, code);
               break;
            }               
            case GT_RASTER_TYPE_GEO_KEY:  // key 1025 Section 6.3.1.2 Code
            {
               printRasterType(out, prefix, code);
               break;
            }

            case GT_CITATION_GEO_KEY: // key 1026
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

            case GEOGRAPHIC_TYPE_GEO_KEY:  // key 2048  Section 6.3.2.1 Codes
            {
               out << prefix << "gcs_type: " << code << "\n";
               break;
            }

            case GEOG_CITATION_GEO_KEY: // key 2049
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

            case GEOG_GEODETIC_DATUM_GEO_KEY:// key 2050  Section 6.3.2.2 Codes
            {
               out << prefix << "geodetic_datum: " << code << "\n";
               break;
            }
            
            case GEOG_LINEAR_UNITS_GEO_KEY:// key 2052  Section 6.3.1.3 Codes
            {
               out << prefix << "linear_units_code: " << code << "\n";
               printLinearUnits(out, prefix, code);
               break;
            }
            
            case GEOG_ANGULAR_UNITS_GEO_KEY:// key 2054  Section 6.3.1.4 Codes
            {
               out << prefix << "angular_units_code: " << code << "\n";
               printAngularUnits(out, prefix, code);
               break;
            }
            
            case GEOG_ELLIPSOID_GEO_KEY:// key 2056  Section 6.3.23 Codes
            {
               out << prefix << "ellipsoid_code: " << code << "\n";
               break;
            }
            
            case GEOG_SEMI_MAJOR_AXIS: // key 2057
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "semi_major_axis: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            case GEOG_SEMI_MINOR_AXIS: // key 2058
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "semi_minor_axis: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case PROJECTED_CS_TYPE_GEO_KEY: // key 3072 Section 6.3.3.1 codes
            {
               out << prefix << "pcs_code: " << code << "\n";
               break;
            }

            case PCS_CITATION_GEO_KEY: // key 3073 ascii
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
      
            case PROJECTION_GEO_KEY: // key 3074 Section 6.3.3.2 codes
            {
               out << prefix << "proj_code: " << code << "\n";
               break;
            }
            
            case PROJ_COORD_TRANS_GEO_KEY:  // key 3075 Section 6.3.3.3 codes
            {
               out << prefix << "coord_trans_code: " << code << "\n";
               printCoordTrans(out, prefix, code);
               break;
            }
         
            case LINEAR_UNITS_GEO_KEY:  // key 3076 Section 6.3.1.3 codes
            {
               out << prefix << "linear_units_code: " << code << "\n";
               printLinearUnits(out, prefix, code);
               break;
            }

            case PROJ_STD_PARALLEL1_GEO_KEY:  // key 3078
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && (code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "std_parallel1: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
         
            case PROJ_STD_PARALLEL2_GEO_KEY:  // key 3079
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "std_parallel2: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
         
            case PROJ_NAT_ORIGIN_LONG_GEO_KEY:  // key 3080
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "origin_longitude: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
               
            case PROJ_NAT_ORIGIN_LAT_GEO_KEY:  // key 3081
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "origin_latitude: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case PROJ_FALSE_EASTING_GEO_KEY:  // key 3082
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "false_easting: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }

            case PROJ_FALSE_NORTHING_GEO_KEY:  // key 3083
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "false_northing: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
               break;
            }
            
            case PROJ_CENTER_LONG_GEO_KEY:  // key 3088
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

            case PROJ_CENTER_LAT_GEO_KEY:  // key 3089
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

            case PROJ_SCALE_AT_NAT_ORIGIN_GEO_KEY:  // key 3092
            {
               if (tag == 34736) // using double array
               {
                  // Code is index into array.
                  if ( geoDoubleBlock && ( code < geoDoubleLength) )
                  {
                     // Always count of one.
                     out << prefix << "scale_factor: "
                         << geoDoubleBlock[code] << "\n";
                  }
               }
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
      }
   }

   return out;
}

std::ostream& ossimTiffInfo::printModelType(std::ostream& out,
                                           const std::string& prefix,
                                           ossim_uint16 code) const
{
   // key 1024 Section 6.3.1.1 Codes
   out << prefix << "model_type: ";
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
   out << prefix << "raster_type: ";
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
   out << prefix << "angular_units: ";
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
                                             ossim_uint16 code) const
{
   // key 3076 Section 6.3.1.3 Codes
   out << prefix << "linear_units: ";
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
