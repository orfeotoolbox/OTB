//***************************************************************************
// FILE: ossimGeoTiff
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// Class definition for ossimGeoTiff which is designed to read and hold tag
// information.
//
//***************************************************************************
// $Id: ossimGeoTiff.cpp,v 1.34 2005/09/01 14:34:01 gpotts Exp $


#include <tiffio.h>
#include <string.h>
#include <fstream>
#include <iomanip>
using namespace std;

#include <support_data/tiff/ossimGeoTiff.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/context/ossimErrorContext.h>
#include <base/misc/lookup_tables/ossimGeoTiffCoordTransformsLut.h>
#include <base/misc/lookup_tables/ossimGeoTiffDatumLut.h>
#include <base/context/ossimNotifyContext.h>
#include <base/context/ossimNotifyContext.h>

static const ossimGeoTiffCoordTransformsLut COORD_TRANS_LUT;
static const ossimGeoTiffDatumLut DATUM_LUT;

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimGeoTiff.cpp,v 1.34 2005/09/01 14:34:01 gpotts Exp $";
#endif

//---
// Static trace for debugging
//---
static ossimTrace traceDebug("ossimGeoTiff:debug");

ossimGeoTiff::ossimGeoTiff(const ossimFilename& file)
   :
      theGeoKeyOffset(0),
      theGeoKeyLength(0),
      theTiffByteOrder(OSSIM_LITTLE_ENDIAN),
      theEndian(),
      theGeoKeysPresentFlag(false),
      theZone(0),
      theHemisphere("N"),
      theDoubleParamLength(0),
      theAsciiParamLength(0),
      theProjectionName("unknown"),
      theDatumName("unknown"),
      
      theScale(),
      theTiePoint(),
      theModelTransformation(),
      theDoubleParam(0),
      theAsciiParam(0),
      
      theWidth(0),
      theLength(0),
      theBitsPerSample(),
      theCompresionType(NOT_COMPRESSED),
      thePhotoInterpretation(PHOTO_MINISBLACK),
      theImageDescriptionString(),
      theSamplesPerPixel(0),
      thePlanarConfig(PLANARCONFIG_CONTIG),
      theSoftwareDescriptionString(),
      theDateDescriptionString(),
      theTileWidth(0),
      theTileLength(0),
      
      theModelType(UNKNOWN),
      theRasterType(UNDEFINED),
      theGcsCode(0),
      theDatumCode(0),
      theAngularUnits(0),
      thePcsCode(0),
      theSavePcsCodeFlag(false),
      thePcsCitation(),
      theProjGeoCode(0),
      theCoorTransGeoCode(0),
      theLinearUnitsCode(UNDEFINED),
      theStdPar1(0.0),
      theStdPar2(0.0),
      theOriginLon(0.0),
      theOriginLat(0.0),
      theFalseEasting(0.0),
      theFalseNorthing(0.0),
      theCenterLon(0.0),
      theCenterLat(0.0),
      theScaleFactor(0.0)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::ossimGeoTiff: Entered..." << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::ossimGeoTiff: OSSIM_ID =  "
         << OSSIM_ID << endl;
#endif      
   }
   
   if(readTags(file) == false)
   {
      if (traceDebug())
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::ossimGeoTiff: "
            << "Unable to reade tags."
            << std::endl;
      }
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimGeoTiff::ossimGeoTiff: "
         << "Unable to reade tags."
         << std::endl;
   }
}

ossimGeoTiff::~ossimGeoTiff()
{
   if (theAsciiParam)
   {
      delete [] theAsciiParam;
      theAsciiParam = 0;
   }
   if (theDoubleParam)
   {
      delete [] theDoubleParam;
      theDoubleParam = 0;
   }
}

bool ossimGeoTiff::readTags(const ossimFilename& file)
{
   theCenterLon = OSSIM_DBL_NAN;
   theCenterLat = OSSIM_DBL_NAN;
   theOriginLon = OSSIM_DBL_NAN;
   theOriginLat = OSSIM_DBL_NAN;
   
   theSavePcsCodeFlag = false;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::readTags: Entered...\n";
   }

   //---
   // Open the tif file.
   //---
   ifstream str(file.c_str(), ios::binary|ios::in);
   if (!str) 
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimGeoTiff::readTags: "
         << "Cannot open file:  " << file
         << std::endl;
      
      return false;
   }
   
   //---
   // Get the byte order.  The data member "theTiffByteOrder" was set to
   // BIG_END in the initialization list.  This will be changed if
   // byte order is LITTLE_END(IBM, DEC).
   //---
   char byteOrder[3];
   str.read(byteOrder, 2); // Read the byte order.
   byteOrder[2] = '\0';  // Null terminate.

   //---
   // Change the byte order if it was not default of little endian.
   //---   
   if (byteOrder[0] == 'M')
   {
      theTiffByteOrder = OSSIM_BIG_ENDIAN;
   }
   
   ossim_uint16 version;
   readShort(version, str);
   
   if (byteOrder[0] != 'I' &&
       byteOrder[0] != 'M' &&
       version      != 42)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimGeoTiff::readTags:  Unacceptable image format."
         << "\nByte Order:  " << byteOrder
         << "\nversion:     " << dec << version << std::endl;
      str.close();
      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << " DEBUG ossimGeoTiff::readTags: "
         << "version:  " << dec << version << "\n";
      
      if (theTiffByteOrder == OSSIM_BIG_ENDIAN)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Byte Order:  BIG_ENDIAN" << std::endl;
      }
      else // OSSIM_LITTLE_ENDIAN
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Byte Order:  LITTLE_ENDIAN" << std::endl;
      }

      if (theEndian.getSystemEndianType() == OSSIM_BIG_ENDIAN)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "System Byte Order:  BIG_ENDIAN" << std::endl;
      }
      else // OSSIM_LITTLE_ENDIAN
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "System Byte Order:  LITTLE_ENDIAN" << std::endl;
      }   
   }

   //---
   // Variables used within the loop.
   //---
   ossim_uint32  ifdOffset   = 0; // Image File Directory.
   ossim_uint16  ndirs       = 0; // Number of directory entries.
   
   //---
   // Get the offset to the first Image File Directory (IFD).
   //---
   readLong(ifdOffset, str);
   if (!str)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimGeoTiff::readTags: "
         << "No offset to an image file directory found.\n"
         << "Returning with error."
         << std::endl;
      str.close();
      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::readTags: "
         << "Offset to first ifd:  " << dec << ifdOffset
         << "\n";
   }

   while(ifdOffset)
   {
      
      str.seekg(ifdOffset, ios::beg);  // Seek to the image file directory.

      //---
      // Get the number of directories within the IFD.
      //---
      readShort(ndirs, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimGeoTiff::readTags: "
            << "Error reading number of direcories."
            << std::endl;
         str.close();
         return false;
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::readTags:\n"
            << "ifd:  " << dec << ifdOffset
            << "\nndirs:  " << dec << ndirs << std::endl;
      }

      for (short dir = 0; dir < ndirs; ++dir)
      {
         // Variables used within the loop.
         ossim_uint16   tag         = 0; // Tag number
         ossim_uint16   type        = 0; // Type(short, long...)
         ossim_int32    length      = 0;
         ossim_uint32   offset      = 0; // Value or Offset to data.
         ossim_uint8    byteValue   = 0;
         ossim_uint16   shortValue  = 0;
         ossim_uint32   longValue   = 0;
         ossim_float32  floatValue  = 0.0;
         ossim_float64  doubleValue = 0.0;
         ossim_uint8*   byteArray   = 0;
         ossim_int8*    asciiArray  = 0;
         ossim_uint16*  shortArray  = 0;
         ossim_uint32*  longArray   = 0;
         ossim_float32* floatArray  = 0;
         ossim_float64* doubleArray = 0;
         
         //---
         // Get the tag.
         //---
         readShort(tag, str);
         if (!str)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimGeoTiff::readTags: "
               << "Error reading tag number."
               << std::endl;
            str.close();
            return false;
         }

         //---
         // Get the type (byte, ascii, short...)
         //---
         readShort(type, str);
         if (!str)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimGeoTiff::readTags: "
               << "Error reading type number."
               << std::endl;
            str.close();
            return false;
         }

         //---
         // Get the count.  This is not in bytes.  It is based on the
         // type.  So if the type is a short and the count is one then
         // read "sizeof(short"(2) bytes.
         //---
         readLong(length, str);
         if (!str)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimGeoTiff::readTags: "
               << "Error reading length."
               << std::endl;
            str.close();
            return false;
         }
         if(traceDebug())
	 {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DEBUG ossimGeoTiff::readTags:"
               << "\nDirectory index:  " << dir
               << "\ntag:              " << tag
               << "\ntype:             " << type
               << "\nlength:           " << length << std::endl;

	 }
         //---
         // Info is stored in the next four bytes if it will fit.  If not
         // the 4 bytes is an offset to the data.  Need to determine
         // if the data fits in next four bytes based on the type and count.
         //---
         switch (type)
         {
            case TIFF_BYTE:
            {
               if (length > 1) // Need the array.
               {
                  if (byteArray) delete [] byteArray;
                  byteArray = new unsigned char[length];
               }
            
               if (length == 1)
               {
                  str.read((char*)&byteValue, length);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Error reading data." << std::endl;
                     str.close();
                     return false;
                  }
               }
               else if (length < 5)
               {
                  str.read((char*)byteArray, length);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags:"
                        << "Error reading data." << std::endl;
                     str.close();
                     return false;
                  }
               }
               else if (length > 4)
               {
                  // Get the offset to the data.
                  readLong(offset, str);
               
                  streampos pos = str.tellg(); // Capture position
               
                  str.seekg(offset, ios::beg); // Seek to the string.

                  str.read((char*)byteArray, length);
               
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags:"
                        << "Unable to read byte array." << std::endl;
                     str.close();
                     return false;
                  }
               
                  str.seekg(pos, ios::beg); // Seek back.
               }
            
               if (length < 4) // Seek to the end of the directory entry.
               {
                  str.seekg(4 - length, ios::cur);
               }
            
               break;
            
            }  // End of case TIFF_BYTE
            
            case TIFF_ASCII:
            {
               if (asciiArray) delete [] asciiArray;
               asciiArray = new char[length];
            
               if (length < 5)
               {
                  str.read(asciiArray, length);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Unable to read data." << std::endl;
                     str.close();
                     return false;
                  }
               }
               else if (length > 4)
               {
                  // Get the offset to the data.
                  readLong(offset, str);
               
                  streampos pos = str.tellg(); // Capture position
               
                  str.seekg(offset, ios::beg); // Seek to the string.
                  
                  str.read(asciiArray, length);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Unable to read ascii array." << std::endl;
                     str.close();
                     return false;
                  }
               
                  str.seekg(pos, ios::beg); // Seek back.
               }
            
               if (length < 4) // Seek to the end of the directory entry.
               {
                  str.seekg(4 - length, ios::cur);
               }
            
               asciiArray[length-1] = '\0'; // Null terminate to be safe.
            
               break;
            
            }  // End of case TIFF_ASCII
         
            case TIFF_SHORT:
            {
               if (length > 1)
               {
                  if (shortArray) delete [] shortArray;
                  shortArray = new unsigned short[length];
               }
            
               if (length == 1)
               {
                  readShort(shortValue, str);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Unable to read SHORT data" << endl;
                     str.close();
                     return false;
                  }

		  str.ignore(2);
                  // Seek to next direcory entry.
		  //                  str.seekg(2, ios::cur);
               }
               else if (length == 2)
               {
                  // Get the first short.
                  readShort(shortArray[0], str);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Unable to read SHORT data" << std::endl;
                     str.close();
                     return false;
                  }
               
                  // Get the second short.
                  readShort(shortArray[1], str);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        <<"FATAL ossimGeoTiff::readTags: "
                        << "Unalbe to read SHORT data" << std::endl;
                     str.close();
                     return false;
                  }
               }
               else
               {
                  // Get the offset to the data.
                  readLong(offset, str);
               
                  streampos pos = str.tellg(); // Capture position
                  str.seekg(offset, ios::beg); // Seek to the array.
               
		  for (int i=0; i<length; ++i)
		  {
                     readShort(shortArray[i], str);
                     if (!str)
                     {
                        ossimNotify(ossimNotifyLevel_FATAL)
                           << "FATAL ossimGeoTiff::readTags: "
                           << "Unable to read short array." << endl;
                        str.close();
                        return false;
                     }
                   }
               
                  str.seekg(pos, ios::beg); // Seek back.
               }
            
               break;
            
            }  // End of case TIFF_SHORT
            
            case TIFF_LONG:
            {
               if (length > 1)
               {
                  if (longArray) delete [] longArray;
                  longArray = new ossim_uint32[length];
               }
            
               if (length == 1)
               {
                  readLong(longValue, str);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Unable to read LONG data." << std::endl;
                     str.close();
                     return false;
                  }
               }
               else
               {
                  // Get the offset to the data.
                  readLong(offset, str);
                  streampos pos = str.tellg(); // Capture position
                  
                  str.seekg(offset, ios::beg); // Seek to the array.

                  for (int i=0; i<length; ++i)
                  {
                     readLong(longArray[i], str);
                     if (!str)
                     {
                        ossimNotify(ossimNotifyLevel_FATAL)
                           << "FATAL ossimGeoTiff::readTags: "
                           << "Unable to read long array." << std::endl;
                        str.close();
                        return false;
                     }
                  }
                  
                  str.seekg(pos, ios::beg); // Seek back.
               }
               
               break;

            } // End of case TIFF_LONG

            case TIFF_DOUBLE:
            {
               if (doubleArray) delete [] doubleArray;
               doubleArray = new double[length];

               // Get the offset to the data.
               readLong(offset, str);
               
               streampos pos = str.tellg(); // Capture position
               
               str.seekg(offset, ios::beg); // Seek to the array.
               
               for (int i=0; i<length; ++i)
               {
                  readDouble(doubleArray[i], str);
                  if (!str)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << "FATAL ossimGeoTiff::readTags: "
                        << "Error reading double array." << std::endl;
                     str.close();
                     return false;
                  }
               }
                  
               str.seekg(pos, ios::beg); // Seek back.
               
               break;
            }

            case TIFF_RATIONAL:
            case TIFF_SBYTE:
            case TIFF_UNDEFINED:
            case TIFF_SSHORT:
            case TIFF_SLONG:
            case TIFF_SRATIONAL:
            case TIFF_FLOAT:
            {
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "DEBUG ossimGeoTiff::readTags:"
                     << " Type currently not handled "
                     << " type = " << dec << type
                     << std::endl;
               }
            }
            default:
            {
               // Get the offset to the data.
               readLong(offset, str);
               if (!str)
               {
                  ossimNotify(ossimNotifyLevel_FATAL)
                     << "FATAL ossimGeoTiff::readTags: "
                     << "Unable to read offset to next directory."
                     << std::endl;
                  str.close();
                  return false;
               }
               break;
            }
            
         } // End of switch on type.
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "\nbyteValue:        " << int(byteValue)
               << "\nshortValue:       " << shortValue
               << "\nlongValue:        " << longValue
               << "\nfloatValue:       " << floatValue
               << "\ndoubleValue:      " << doubleValue
               << "\noffset:           " << offset
               << std::endl;

//#if 0 /* Very serious debug only... */
            if (byteArray)
            {
               for (int i = 0; i < length; ++i)
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "byteArray[" << i << "]:  " << byteArray[i]
                     << std::endl;
               }
            }
            if (asciiArray)
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "asciiArray:    " << asciiArray << std::endl;
            }
            if (shortArray)
            {
               for (int i = 0; i < length; ++i)
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "shortArray[" << i << "]:  " << shortArray[i]
                     << std::endl;
               }
            }
            if (longArray)
            {
               for (int i = 0; i < length; ++i)
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "longArray[" << i << "]:  " << longArray[i]
                     << std::endl;
               }
            }
            if (doubleArray)
            {
               for (int i = 0; i < length; ++i)
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << setiosflags(ios::fixed) << setprecision(15)
                     << "doubleArray[" << i << "]:  " << doubleArray[i]
                     << std::endl;
               }
            }
            
// #endif  /* #if 0 */
            
         } // End of "if traceDebug()"
         

         switch(tag)
         {
            case TIFFTAG_IMAGEWIDTH: // tag 256
               if (type == TIFF_SHORT)
               {
                  theWidth = shortValue;
               }
               else if (type == TIFF_LONG)
               {
                  theWidth = longValue;
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "WARNING: "
                     << "Invalid type [ " << type << " ] for image width."
                     << std::endl;
               }
               break;
         
            case TIFFTAG_IMAGELENGTH: // tag 257
               if (type == TIFF_SHORT)
               {
                  theLength = shortValue;
               }
               else if (type == TIFF_LONG)
               {
                  theLength = longValue;
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "WARNING:\n"
                     << "Invalid type [ " << type << " ] for image length."
                     << std::endl;
               }
               break;
            
            case TIFFTAG_BITSPERSAMPLE: // tag 258
               // Clear out any old points.
               theBitsPerSample.clear();
               
               if (length == 1)
               {
                  theBitsPerSample.push_back(shortValue);
               }
               else if (shortArray)
               {
                  for (int i=0; i<length; ++i)
                  {
                     theBitsPerSample.push_back(shortArray[i]);
                  }
               }
               break;
         
            case TIFFTAG_COMPRESSION: // tag 259
               if (shortValue == 1)
               {
                  theCompresionType = NOT_COMPRESSED;
               }
               else
               {
                  theCompresionType = COMPRESSED;
               }
               break;

            case TIFFTAG_PHOTOMETRIC: // tag 262
               thePhotoInterpretation = (PhotoInterpretation)shortValue;
               break;
         
            case TIFFTAG_IMAGEDESCRIPTION: // tag 270
               if (asciiArray)
               {
                  theImageDescriptionString = asciiArray;
               }
               break;

            case TIFFTAG_SAMPLESPERPIXEL: // tag 277
               theSamplesPerPixel = shortValue;
               break;

            case TIFFTAG_PLANARCONFIG: // tag 284
               thePlanarConfig = shortValue;
               break;

            case TIFFTAG_SOFTWARE: // tag 305
               if (asciiArray)
               {
                  theSoftwareDescriptionString = asciiArray;
               }
               break;

            case TIFFTAG_DATETIME: // tag 306
               if (asciiArray)
               {
                  theDateDescriptionString = asciiArray;
               }
               break;

            case TIFFTAG_TILEWIDTH: // tag 322
               if (type == TIFF_SHORT)
               {
                  theTileWidth = shortValue;
               }
               else if (type == TIFF_LONG)
               {
                  theTileWidth = longValue;
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "WARNING:\n"
                     << "Invalid type [ " << type << " ] for tile width."
                     << std::endl;
               }
               break;

            case TIFFTAG_TILELENGTH: // tag 323
               if (type == TIFF_SHORT)
               {
                  theTileLength = shortValue;
               }
               else if (type == TIFF_LONG)
               {
                  theTileLength = longValue;
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "WARNING:\n"
                     << "Invalid type [ " << type << " ] for tile length."
                     << std::endl;
               }
               
               break;

            case MODEL_PIXEL_SCALE_TAG: // tag 33550
               if ( doubleArray )
               {
                  // Clear out any old points.
                  theScale.clear();

                  for (int i = 0; i < length; ++i)
                  {
                     theScale.push_back(doubleArray[i]);
                  }
               }

               break;

            case MODEL_TIE_POINT_TAG: // tag 33922
               if ( doubleArray )
               {
                  // Clear out any old points.
                  theTiePoint.clear();

                  for (int i=0; i<length; ++i)
                  {
                     theTiePoint.push_back(doubleArray[i]);
                  }
               }
               
               break;

            case MODEL_TRANSFORM_TAG: // tag 34264
               if ( doubleArray )
               {
                  // Clear out any old points.
                  theModelTransformation.clear();

                  for (int i=0; i<length; ++i)
                  {
                     theModelTransformation.push_back(doubleArray[i]);
                  }
               }
               break;

            case GEO_KEY_DIRECTORY_TAG: // tag 34735
               theGeoKeysPresentFlag = 1;

               // Store the offset and length for later.
               theGeoKeyOffset = offset;
               theGeoKeyLength = length;
               
               break;
         
            case GEO_DOUBLE_PARAMS_TAG: // tag 34736
               if ( doubleArray )
               {
                  //---
                  // This can contain multiple key values.  This
                  // will grap them all.  They will be sorted out in the method
                  // readGeoKeys.
                  //---
                  theDoubleParamLength = length;

                  if (theDoubleParam) delete [] theDoubleParam;
            
                  theDoubleParam = new double[theDoubleParamLength];

                  for (int i=0; i<length; ++i)
                  {
                     theDoubleParam[i] = doubleArray[i];
                  }
               }
               break;

            case GEO_ASCII_PARAMS_TAG: // tag 34737
               if ( asciiArray )
               {
                  //---
                  // This can contain multiple strings.  This
                  // will grap them all.  They will be sorted out in the method
                  // readGeoKeys.
                  //---
                  theAsciiParamLength = length;
            
                  if (theAsciiParam) delete [] theAsciiParam;
                  
                  theAsciiParam = new char[length];

                  for (int i=0; i<length; ++i)
                  {
                     theAsciiParam[i] = asciiArray[i];
                  }
               }
               break;
         
            default:
               break;
         } // End of switch.

         // Free memory if allocated...
         if (byteArray)
         {
            delete [] byteArray;
            asciiArray = 0;
         }
         if (asciiArray)
         {
            delete [] asciiArray;
            asciiArray = 0;
         }
         if (shortArray)
         {
            delete [] shortArray;
            shortArray = 0;
         }
         if (longArray)
         {
            delete [] longArray;
            longArray = 0;
         }
         if (floatArray)
         {
            delete [] floatArray;
            floatArray = 0;
         }
         if (doubleArray)
         {
            delete [] doubleArray;
            doubleArray = 0;
         }
         
      } // End of loop through directories

      //---
      // Get the next IFD offset.  Continue this loop until the offset is
      // zero.
      //---
      readLong(ifdOffset, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "FATAL ossimGeoTiff::readTags: "
            << "No offset to an image file directory found.\n"
            << "Returning with error."
            << std::endl;
         str.close();
         return false;
      }
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::readTags: "
            << "Next Image File Directory(IFD) offset = "
            << dec << ifdOffset << std::endl;
      }

      //---
      // Break out of the loop here so we only read the first image directory.
      //---
      break;
      
   } // End of loop through the IFD's.

   //---
   // If Geotiff Keys read them.  This had to done last since the keys could
   // have references to tags GEO_DOUBLE_PARAMS_TAG and GEO_ASCII_PARAMS_TAG.
   //---
   if (theGeoKeysPresentFlag)
   {      
      if (readGeoKeys(str, theGeoKeyOffset, theGeoKeyLength) == false)
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATALossimGeoTiff::readTags: "
            << "Unable to read geotiff keys."
            << std::endl;
         str.close();
         return false;
      }

      setOssimProjectionName();  // Initialize the ossim projection name.
      setOssimDatumName();       // Initialize the ossim datum name.
   }
   
   str.close();
   
   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimGeoTiff::readTags: Exited..." << std::endl;
   
   return true;
}

void ossimGeoTiff::readShort(ossim_uint16& s, ifstream& str)
{
   str.read((char*)&s, sizeof(s));
   if (theTiffByteOrder != theEndian.getSystemEndianType())
   {
      theEndian.swap(s);
   }
}

void ossimGeoTiff::readLong(ossim_int32& l, ifstream& str)
{
   str.read((char*)&l, sizeof(l));
   if (theTiffByteOrder != theEndian.getSystemEndianType())
   {
      theEndian.swap(l);
   }
}

void ossimGeoTiff::readLong(ossim_uint32& l, ifstream& str)
{
   str.read((char*)&l, sizeof(l));
   if (theTiffByteOrder != theEndian.getSystemEndianType())
   {
      theEndian.swap(l);
   }
}

void ossimGeoTiff::readDouble(ossim_float64& d, ifstream& str)
{
   str.read((char*)&d, sizeof(d));
   if (theTiffByteOrder != theEndian.getSystemEndianType())
   {
      theEndian.swap(d);
   }
}

bool ossimGeoTiff::readGeoKeys(ifstream& str, streampos offset, int length)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::readGeoKeys: Entered..." << std::endl;
   }

   //---
   // Each key contains four unsigned shorts:
   // GeoKey ID
   // TIFF Tag ID or 0
   // GeoKey value count
   // value or tag offset
   //---
   unsigned short key;
   unsigned short tag;
   unsigned short count;
   unsigned short code;

   //---
   // Save the original file position.
   //---
   streampos pos = str.tellg();

   str.seekg(offset, ios::beg);  // Seek to the Key directory.

   //---
   // Length passed in is the total number of shorts in the geo key directory.
   // Each key has four short values; hence, "length/4".
   //---
   for (int i = 0; i < length/4; i++)
   {
      //---
      // Get the key.
      //---
      readShort(key, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "FATAL ossimGeoTiff::readGeoKeys: "
            << "Unable to read tag number."
            << std::endl;
         str.close();
         return false;
      }

      //---
      // Get the tiff tag.  This will either be a tag of zero.
      //---
      readShort(tag, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimGeoTiff::readGeoKeys: "
            << "Unalbe to read type number."
            << std::endl;
         str.close();
         return false;
      }

      //---
      // Get the count.  
      //---
      readShort(count, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL  ossimGeoTiff::readGeoKeys: "
            << "Unable to read length."
            << std::endl;
         str.close();
         return false;
      }

      //---
      // Get the value.
      //---
      readShort(code, str);
      if (!str)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimGeoTiff::readGeoKeys: "
            << "Unable to read length."
            << std::endl;
         str.close();
         return false;
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG  ossimGeoTiff::readGeoKeys"
            << "\nKey index:  " << i
            << "\ngeo key:  " << key
            << "\ntag:      " << tag
            << "\ncount:    " << count
            << "\ncode:     " << code
            << std::endl;
      }

      switch (key)
      {
         case GT_MODEL_TYPE_GEO_KEY:  // key 1024 Section 6.3.1.1 Codes
            theModelType = (ModelType)code;
            break;
         
         case GT_RASTER_TYPE_GEO_KEY:  // key 1025 Section 6.3.1.2 Code
            theRasterType = code;
            break;
         
         case GEOGRAPHIC_TYPE_GEO_KEY:  // key 2048  Section 6.3.2.1 Codes
            theGcsCode = code;
            break;

         case GEOG_GEODETIC_DATUM_GEO_KEY:  // key 2050  Section 6.3.2.2 Codes
            theDatumCode = code;
            break;
         
         case GEOG_ANGULAR_UNITS_GEO_KEY:  // key 2054  Section 6.3.1.4 Codes
            theAngularUnits = code;
            break;
         
         case PROJECTED_CS_TYPE_GEO_KEY: // key 3072 Section 6.3.3.1 codes
            thePcsCode = code;
            parsePcsCode(thePcsCode);
            break;
         
         case PCS_CITATION_GEO_KEY: // key 3073 ascii
         {
            if(theAsciiParamLength)
            {
               //---
               // Ensure the code + count does not bust the string length.
               //---
               if( (code + count) > theAsciiParamLength)
               {
                  ossimNotify(ossimNotifyLevel_FATAL)
                     << "FATAL ossimGeoTiff::readGeoKeys: "
                     << "Attempting to copy past the end of string."
                     << "\nString will not be copied." << std::endl;
               }
               else
               {
                  // Limit the scope of tmp.
                  char* tmp = new char[count];
                  strncpy(tmp, &theAsciiParam[code], count);
                  tmp[count-1] = '\0'; // Null terminate.
                  thePcsCitation = tmp;
                  delete [] tmp;
               }
            }
         }
         break;
      
         case PROJECTION_GEO_KEY: // key 3074 Section 6.3.3.2 codes
            theProjGeoCode = code;
            parseProjGeoCode(theProjGeoCode);
            break;
         
         case PROJ_COORD_TRANS_GEO_KEY:  // key 3075 Section 6.3.3.3 codes
            theCoorTransGeoCode = code;
            break;
         
         case LINEAR_UNITS_GEO_KEY:  // key 3076 Section 6.3.1.3 codes
            theLinearUnitsCode = code;
            break;
         
         case PROJ_STD_PARALLEL1_GEO_KEY:  // key 3078 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theStdPar1 = theDoubleParam[code];
            }
         
            break;
         
         case PROJ_STD_PARALLEL2_GEO_KEY:  // key 3079 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theStdPar2 = theDoubleParam[code];
            }
         
            break;
         
         case PROJ_NAT_ORIGIN_LONG_GEO_KEY:  // key 3080 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theOriginLon = theDoubleParam[code];
            }
         
            break;
         case PROJ_NAT_ORIGIN_LAT_GEO_KEY:  // key 3081 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theOriginLat = theDoubleParam[code];
            }
         
            break;

         case PROJ_FALSE_EASTING_GEO_KEY:  // key 3082 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theFalseEasting = theDoubleParam[code];
            }
         
            break;

         case PROJ_FALSE_NORTHING_GEO_KEY:  // key 3083 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theFalseNorthing = theDoubleParam[code];
            }
         
            break;

         case PROJ_CENTER_LONG_GEO_KEY:  // key 3088 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theCenterLon = theDoubleParam[code];
            }
         
            break;

         case PROJ_CENTER_LAT_GEO_KEY:  // key 3089 GeogAngularUnit
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theCenterLat = theDoubleParam[code];
            }
         
            break;

         case PROJ_SCALE_AT_NAT_ORIGIN_GEO_KEY:  // key 3092 ratio 
            if( (tag == GEO_DOUBLE_PARAMS_TAG) &&
                (code <= (theDoubleParamLength - 1)) )
            {
               //---
               // Grab the value from the theDoubleParam
               //---
               theScaleFactor = theDoubleParam[code];
            }
         
            break;

         default:
         {
            break;
            
         }
      }
   }

   str.seekg(pos, ios::beg); // Seek back to next directory entry.

   return true;
}

bool ossimGeoTiff::addImageGeometry(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   // Sanity check...
   if (getTiePoint().size() < 5 || getScale().size() < 2 )
   {
      return false;
   }

   ossimString copyPrefix = prefix;

   //---
   // Shift the tie point to the (0, 0) position if it's not already.
   //
   // Note:
   // Some geotiff writers like ERDAS IMAGINE set the "GTRasterTypeGeoKey"
   // key to RasterPixelIsArea, then set the tie point to (0.5, 0.5).
   // This really means "RasterPixelIsPoint" with a tie point of (0.0, 0.0).
   // Anyway we will check for this blunder and attempt to do the right
   // thing...
   //---
   double x_tie_point = getTiePoint()[3] - getScale()[0]*getTiePoint()[0];
   double y_tie_point = getTiePoint()[4] + getScale()[1]*getTiePoint()[1];
   
   if (theRasterType == OSSIM_PIXEL_IS_AREA)
   {
      //---
      // Since the internal pixel representation is "point", shift the
      // tie point to be relative to the center of the pixel.
      //---
      x_tie_point += (getScale()[0])/2.0;
      y_tie_point -= (getScale()[1])/2.0;
   }

   if(theSavePcsCodeFlag)
   {
      kwl.add(prefix,
              "pcs_code",
              thePcsCode,
              true);

      // tie point
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              convert2meters(x_tie_point));
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              convert2meters(y_tie_point));

      // scale or gsd
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              convert2meters(getScale()[0]));
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              convert2meters(getScale()[1]));
      
      return true;
   }

   //---
   // Get the projection type.  If unknown no point going on, so get out.
   //---
   if (getOssimProjectionName() == "unknown")
   {
      return false;
   }

   // Add these for all projections.
   kwl.add(copyPrefix.c_str(),
           ossimKeywordNames::TYPE_KW,
           getOssimProjectionName().c_str());
   
   kwl.add(copyPrefix.c_str(),
           ossimKeywordNames::DATUM_KW,
           getOssimDatumName().c_str());

   //---
   // Add the tie point and scale.  Convert to either meters or decimal
   // degrees if needed.
   //---
   if (theModelType == MODEL_TYPE_GEOGRAPHIC)
   {
      if (!theAngularUnits && traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << " WARNING ossimGeoTiff::addImageGeometry:"
            << "The GeogAngularUnitsGeoKey (key 2054) is not set!"
            << "\nAssuming \"Angular_Degree\"..." << std::endl;
      }
      
      switch (theAngularUnits)
      {
         case ANGULAR_ARC_MINUTE:
         case ANGULAR_ARC_SECOND:
         case ANGULAR_GRAD:
         case ANGULAR_GON:
         case ANGULAR_DMS:
         case ANGULAR_DMS_HEMISPHERE:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "WARNING ossimGeoTiff::addImageGeometry:"
               << "\nNot coded yet for unit type:  "
               << theAngularUnits << endl;
            return false;
         }
         case ANGULAR_DEGREE:
         default:
         {
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::TIE_POINT_LON_KW,
                    x_tie_point);
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::TIE_POINT_LAT_KW,
                    y_tie_point);
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
                    getScale()[0]);
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
                    getScale()[1]);

            if(theOriginLat == OSSIM_DBL_NAN)
            {
               theOriginLat = 0.0;
            }
            if(theOriginLon == OSSIM_DBL_NAN)
            {
               theOriginLon = 0.0;
            }
            double olat = theOriginLat;
            double olon = theOriginLon;
            if((theOriginLat == 0.0)&&
               (theOriginLon == 0.0)&&
               (getScale()[0] !=
                getScale()[1] ))
            {
               double centerX = theWidth/2.0;
               double centerY = theLength/2.0;
               
               olat = getScale()[1]*centerY + y_tie_point;
               olon = getScale()[0]*centerX + x_tie_point;
               
            }
            // origin
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::ORIGIN_LATITUDE_KW,
                    olat,
                    true);
            
            kwl.add(copyPrefix.c_str(),
                    ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                    olon,
                    true);
         }
      }  // End of switch on theAngularUnits.
   }
   else if (theModelType == MODEL_TYPE_PROJECTED)
   {
      // tie point
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              convert2meters(x_tie_point));
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              convert2meters(y_tie_point));

      // scale or gsd
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              convert2meters(getScale()[0]));
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              convert2meters(getScale()[1]));

      if(theOriginLat != OSSIM_DBL_NAN)
      {
         // origin
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::ORIGIN_LATITUDE_KW,
                 theOriginLat);
      }
      else if(theCenterLat != OSSIM_DBL_NAN)
      {
         // origin
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::ORIGIN_LATITUDE_KW,
                 theCenterLat);
      }

      if(theOriginLon != OSSIM_DBL_NAN)
      {
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                 theOriginLon);
      }
      if(theCenterLon != OSSIM_DBL_NAN)
      {
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                 theCenterLon);
      }

      // std paralles for conical projections
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::STD_PARALLEL_1_KW,
              theStdPar1);
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::STD_PARALLEL_2_KW,
              theStdPar2);

      // false easting and northing.
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::FALSE_EASTING_KW,
              convert2meters(theFalseEasting));
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::FALSE_NORTHING_KW,
              convert2meters(theFalseNorthing));
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING ossimGeoTiff::addImageGeometry:"
         << " Unsupported model type..." << std::endl;
      return false;
   }

   //---
   // Based on projection type, override/add the appropriate info.
   //---
   if (getOssimProjectionName() == "ossimUtmProjection")
   {
      // Check the zone before adding...
      if (theZone > 0 && theZone < 61)
      {
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::ZONE_KW,
                 theZone);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimGeoTiff::addImageGeometry: "
            << "UTM zone " << theZone << " out of range!\n"
            << "Valid range:  1 to 60" << endl;
         return false;
      }

      // Check the hemisphere before adding.
      if (theHemisphere == "N" || theHemisphere == "S")
      {
         kwl.add(copyPrefix.c_str(),
                 ossimKeywordNames::HEMISPHERE_KW,
                 theHemisphere);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimGeoTiff::addImageGeometry: "
            << "UTM hemisphere " << theHemisphere << " is invalid!\n"
            << "Valid hemisphere:  N or S" << std::endl;
         return false;
      }

      //---
      // Must set the central meridian even though the zone should do it.
      // (in decimal degrees)
      //---
      double central_meridian = ( 6.0 * abs(theZone) ) - 183.0;
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              central_meridian,
              true);  // overwrite keyword if previously added...
      
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              0.0,
              true);  // overwrite keyword if previously added...
 
   } // End of "if (UTM)"

   else if (getOssimProjectionName() == "ossimUtmProjection")
   {
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::SCALE_FACTOR_KW,
              theScaleFactor,
              true);  // overwrite keyword if previously added...
   }
   
   //---
   // Get the model transformation info if it's present.
   //---
   if (getModelTransformation().size() == 16)
   {
      vector<double> v = getModelTransformation();
      kwl.add(copyPrefix.c_str(),
              "m11",
              v[0], 
              true);
      kwl.add(copyPrefix.c_str(),
              "m12",
              v[1],
              true);
      kwl.add(copyPrefix.c_str(),
              "m13",
              v[2],
              true);
      kwl.add(copyPrefix.c_str(),
              "m14",
              v[3],
              true);
      kwl.add(copyPrefix.c_str(),
              "m21",
              v[4],
              true);
      kwl.add(copyPrefix.c_str(),
              "m22",
              v[5],
              true);
      kwl.add(copyPrefix.c_str(),
              "m23",
              v[6],
              true);
      kwl.add(copyPrefix.c_str(),
              "m24",
              v[7],
              true);
      kwl.add(copyPrefix.c_str(),
              "m31",
              v[8],
              true);
      kwl.add(copyPrefix.c_str(),
              "m32",
              v[9],
              true);
      kwl.add(copyPrefix.c_str(),
              "m33",
              v[10],
              true);
      kwl.add(copyPrefix.c_str(),
              "m34",
              v[11],
              true);
      kwl.add(copyPrefix.c_str(),
              "m41",
              v[12],
              true);
      kwl.add(copyPrefix.c_str(),
              "m42",
              v[13],
              true);
      kwl.add(copyPrefix.c_str(),
              "m43",
              v[14]);
      kwl.add(copyPrefix.c_str(),
              "m44",
              v[15],
              true);
   } // End of "if (gt.getModelTransformation().size() == 16)"

   if(theScaleFactor > 0.0)
   {
      kwl.add(copyPrefix.c_str(),
              ossimKeywordNames::SCALE_FACTOR_KW,
              theScaleFactor,
              true);
   }      
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::addImageGeometry: Keyword list dump:\n"
         << kwl << std::endl;
   }
   
   return true;
}

double ossimGeoTiff::convert2meters(double d) const
{
   switch(theLinearUnitsCode)
   {
   case LINEAR_METER:
      return d;
   case LINEAR_FOOT:
      return ft2mtrs(d);
   case LINEAR_FOOT_US_SURVEY:
      return usft2mtrs(d);
   default:
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::convert2meters: " 
            << "Linear units code was undefined!\n"
            << "No conversion was performed." << std::endl;
      }
      break;
   }
   
   return d;
}

ossimString ossimGeoTiff::getOssimProjectionName() const
{
   return theProjectionName;
}

void ossimGeoTiff::setOssimProjectionName()
{
   //---
   // The "parsePcsCode" method can also set the projection name.  So check
   // it prior to looking in the lookup table.
   //---
   if (theProjectionName == "unknown")
   {
      ossimString name =  COORD_TRANS_LUT.getEntryString(theCoorTransGeoCode);
      
      if (name.size())
      {
         theProjectionName = name;
      }
   }

   // If still unknown check for the model type.
   if (theProjectionName == "unknown")
   {
      if (theModelType == MODEL_TYPE_GEOGRAPHIC)
      {
         theProjectionName = "ossimEquDistCylProjection";
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::setOssimProjectionName: "
         << "theProjectionName:  "
         << theProjectionName
         << std::endl;
   }
}

ossimString ossimGeoTiff::getOssimDatumName() const
{
   return theDatumName;
}

void ossimGeoTiff::setOssimDatumName()
{
   //---
   // The "parsePcsCode" method can also set the datum name.  So check
   // it prior to trying to assign.
   //---
   if (theDatumName == "unknown")
   {
      ossimString name = DATUM_LUT.getEntryString(theDatumCode);
      
      if (name.size())
      {
         theDatumName = name;
      }
      else
      {
         // Try the GCS code.
         name = DATUM_LUT.getEntryString(theGcsCode);
         if (name.size())
         {
            theDatumName = name;
         }
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::setOssimDatumName: "
         << "theDatumName:  "
         << theDatumName
         << std::endl;
   } 
}

void ossimGeoTiff::parsePcsCode(int code)
{
   // key 3072 Section 6.3.3.1 codes
   
   if (code == USER_DEFINED)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::parsePcsCode: "
            << "Projection coordinate system is user defined."
            << std::endl;
      }
      return;
   }

   if(code == PCS_BRITISH_NATIONAL_GRID)
   {
      theProjectionName = "ossimBngProjection";
      theDatumName = "OGB-M";
      theFalseEasting = 400000.0;
      theFalseNorthing = -100000.0;
      theScaleFactor   = .9996012717;
      theOriginLat     = 49.0;
      theOriginLon     = -2.0;
      theHemisphere    = "N";
      return;
   }

   //---
   // Divide the code by 100. Then check for a known type.  If it is a
   // utm projection the last two digits represent the zone.
   //---
   int type = code/100;
   int zone = code%100;

   switch (type)
   {
      case 322:
         //---
         // utm, WGS72 (WGD), northern hemisphere
         // All 60 zones handled.
         //---
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "N";
         theDatumName      = "WGD";
         break;
         
      case 323:
         //---
         // utm, WGS72 (WGD), southern hemisphere
         // All 60 zones handled.
         //---
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "S";
         theDatumName      = "WGD";
         break;
         
      case 326:
         //---
         // utm, WGS84 (WGE), northern hemisphere
         // All 60 zones hadled.
         //---
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "N";
         theDatumName      = "WGE";
         break;
         
      case 327:
         //---
         // utm, WGS84 (WGE), southern hemisphere
         // All 60 zones handled.
         //---
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "S";
         theDatumName      = "WGE";
         break;
         
      case 267:
         //---
         // utm, "NAS-C", northern hemisphere
         // Only UTM NAD27 North zones 3 to 22 are in the 267xx range...
         // 26729 through 26803 handled by state plane factory.
         //---
         if ( (code > 26702) && (code < 26723) )
         {
            theProjectionName = "ossimUtmProjection";
            theZone           = zone;
            theHemisphere     = "N";
            theDatumName      = "NAS-C";
         }
         break;
         
      case 269: // utm, "NAR-C", northern hemisphere
         //---
         // Only UTM NAD83 North zones 3 to 23 are in the 269xx range...
         // 26929 through 26998 handled by state plane factory.
         //---
         if ( (code > 26902) && (code < 26924) )
         {
            theProjectionName = "ossimUtmProjection";
            theZone           = zone;
            theHemisphere     = "N";
            theDatumName      = "NAR-C";
         }
         break;
         
      case 248:
         //---
         // Provisional S. American 1956
         // 24818 through 24880
         //---
         if ( (code > 24817) && (code < 24881) )
         {
            theProjectionName = "ossimUtmProjection";
            if (zone > 60)
            {
               theZone = (zone - 60);
               theHemisphere     = "S";
            }
            else
            {
               theZone = zone;
               theHemisphere     = "N";
            }
            theDatumName = "PRP-M";
         }
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::parsePcsCode:"
         << "\nprojection type:  " << theProjectionName
         << "\nzone number:      " << theZone
         << "\nhemisphere:       " << theHemisphere
         << "\ndatum:            " << theDatumName
         << std::endl;
   }

   //---
   // Check for codes that the ossimStatePlaneProjectionFactory can create a
   // projection from. This probably should be moved to a method in 
   // ossimStatePlaneProjectionFactory.
   //---
   if(((code >= 26929) && (code <= 26998)) ||
      ((code >= 32100) && (code <= 32158)) ||
      ((code >= 32001) && (code <= 32060)) ||
      ((code >= 26729) && (code <= 26803)) ||
      (code == 32161)
      )
   {
      theSavePcsCodeFlag = true;
   }

   if ( (theSavePcsCodeFlag == true) &&
        (theProjectionName == "ossimUtmProjection") )
   {
      //---
      // Setting theSavePcsCodeFlag to true causes the getImageGeometry to
      // return just the pcs code and the tie point.  Since the
      // ossimStatePlaneProjectionFactory is used to return a projection
      // from the pcs code and it does NOT handle utm, set the 
      // theSavePcsCodeFlag back to false to avoid this.
      //---
      theSavePcsCodeFlag = false;
   }
}

void ossimGeoTiff::parseProjGeoCode(int code)
{
   //---
   // Currently only handles UTM 160xx and 161xx.
   // Note: No datum with this!
   //---
   
   if (code == USER_DEFINED)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::parseProjGeoCode: "
            << "Projection Geo Key is user defined."
            << std::endl;
      }
      return;
   }

   //---
   // Divide the code by 100. Then check for a known type.  If it is a
   // utm projection the last two digits represent the zone.
   //---
   int type = code/100;
   int zone = code%100;

   switch (type)
   {
      case 160: // utm, northern hemisphere
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "N";
         break;
         
      case 161: // utm, "WGD", southern hemisphere
         theProjectionName = "ossimUtmProjection";
         theZone           = zone;
         theHemisphere     = "S";
         break;

      default:
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::parseProjGeoCode:"
         << "\nprojection type:  " << theProjectionName
         << "\nzone number:      " << theZone
         << "\nhemisphere:       " << theHemisphere
         << std::endl;
   }
}

int ossimGeoTiff::mapZone() const
{
   return theZone;
}

ossimByteOrder ossimGeoTiff::getByteOrder() const
{
   return theTiffByteOrder;
}

void ossimGeoTiff::getScale(vector<double>& scale) const
{
   scale = theScale;
}

void ossimGeoTiff::getTiePoint(vector<double>& tie_point) const
{
   tie_point = theTiePoint;
}

void ossimGeoTiff::getModelTransformation(vector<double>& transform) const
{
   transform = theModelTransformation;
}

const vector<double>& ossimGeoTiff::getScale() const
{
   return theScale;
}

const vector<double>& ossimGeoTiff::getTiePoint() const
{
   return theTiePoint;
}

const vector<double>& ossimGeoTiff::getModelTransformation() const
{
   return theModelTransformation;
}

int ossimGeoTiff::getWidth() const
{
   return theWidth;
}

int ossimGeoTiff::getLength() const
{
   return theLength;
}

int ossimGeoTiff::getSamplesPerPixel() const
{
   return theSamplesPerPixel;
}
