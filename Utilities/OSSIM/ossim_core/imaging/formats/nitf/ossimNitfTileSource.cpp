//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Contains class definition for ossimNitfTileSource.
// 
//*******************************************************************
//  $Id: ossimNitfTileSource.cpp,v 1.88 2006/01/05 15:00:01 dburken Exp $

#include <imaging/formats/nitf/ossimNitfTileSource.h>

#include <support_data/nitf/ossimNitfPackedBits.h>
#include <support_data/nitf/ossimNitfFile.h>
#include <support_data/nitf/ossimNitfFileHeader.h>
#include <support_data/nitf/ossimNitfImageHeader.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_0.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_1.h>
#include <support_data/nitf/ossimNitfVqCompressionHeader.h>

#include <base/common/ossimConstants.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/misc/lookup_tables/ossimInterleaveTypeLut.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>
#include <base/misc/ossimEndian.h>
#include <base/property/ossimBooleanProperty.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <imaging/formats/tiff/ossimTiffTileSource.h>
RTTI_DEF1_INST(ossimNitfTileSource, "ossimNitfTileSource", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id: ossimNitfTileSource.cpp,v 1.88 2006/01/05 15:00:01 dburken Exp $";
#endif
   
//---
// NOTE:  This should match the enumerations for ReadMode.
//---
static const char* READ_MODE[] = { "READ_MODE_UNKNOWN",
                                   "READ_BIB_BLOCK",
                                   "READ_BIP_BLOCK",
                                   "READ_BIR_BLOCK",
                                   "READ_BSQ_BLOCK",
                                   "READ_BIB",
                                   "READ_BIP",
                                   "READ_BIR" };

//***
// Static trace for debugging
//***
static ossimTrace traceDebug("ossimNitfTileSource:debug");

// 64x64*12bits
// divide by 8 bits to get bytes gives you 6144 bytes
static const ossim_uint32   OSSIM_NITF_VQ_BLOCKSIZE = 6144;

ossimNitfTileSource::ossimNitfTileSource()
   :
      ossimImageHandler(),
      theTile(NULL),
      theCacheTile(NULL),
      theNitfFile(new ossimNitfFile()),
      theNitfImageHeader(0),
      theReadMode(READ_MODE_UNKNOWN),
      theScalarType(OSSIM_SCALAR_UNKNOWN),
      theSwapBytesFlag(false),
      theNumberOfInputBands(0),
      theNumberOfOutputBands(0),
      theBlockSizeInBytes(0),
      theReadBlockSizeInBytes(0),
      theNumberOfImages(0),
      theCurrentEntry(0),
      theImageRect(0,0,0,0),
      theFileStr(),
      theOutputBandList(),
      theCacheSize(0, 0),
      theCacheTileInterLeaveType(OSSIM_INTERLEAVE_UNKNOWN),
      theCacheEnabledFlag(false),
      theCacheId(-1),
      thePackedBitsFlag(false),
      theCompressedBuf(NULL)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::ossimNitfTileSource entered..." << endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID << endl;
#endif
   }

}

ossimNitfTileSource::~ossimNitfTileSource()
{
   destroy();
}

void ossimNitfTileSource::destroy()
{
   if (theCacheId != -1)
   {
      ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
      theCacheId = -1;
   }

   // Delete the list of image headers.
   vector<ossimNitfImageHeader*>::iterator i = theNitfImageHeader.begin();
   while (i != theNitfImageHeader.end())
   {
     if(*i)
       {
	 delete *i;
	 (*i) = 0;
       }
      ++i;
   }
   theNitfImageHeader.clear();

   if(theFileStr.is_open())
   {
      theFileStr.close();
   }
   if (theNitfFile)
   {
      delete theNitfFile;
      theNitfFile = NULL;
   }

   theCacheTile = NULL;
   theTile      = NULL;

   if(theOverview)
   {
      delete theOverview;
      theOverview = NULL;
   }
   if(theCompressedBuf)
   {
      delete [] theCompressedBuf;
      theCompressedBuf = NULL;
   }
}

bool ossimNitfTileSource::isOpen()const
{
   return (theNitfImageHeader.size() > 0);
}

bool ossimNitfTileSource::open()
{
   if(isOpen())
   {
      close();
   }
   
   theErrorStatus = ossimErrorCodes::OSSIM_OK;

   if ( !parseFile() )
   {
      return false;
   }
   return allocate();
}

void ossimNitfTileSource::close()
{
   destroy();
}

bool ossimNitfTileSource::parseFile()
{
   static const char MODULE[] = "ossimNitfTileSource::parseFile";
   
   ossimFilename file = getFilename();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: Nitf file =  " << file << endl;
   }

   if (file.empty())
   {
      setErrorStatus();
      return false;
   }

   if ( !theNitfFile )  // A close deletes "theNitfFile".
   {
      theNitfFile = new ossimNitfFile();
   }
   
   if ( !theNitfFile->parseFile(file) )
   {
      setErrorStatus();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "DEBUG:" << "\nError parsing file!" << endl;
      }

      return false;
   }

   // Get the number of images within the file.
   theNumberOfImages = theNitfFile->getHeader()->getNumberOfImages();

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "DEBUG:\nNumber of images "
         <<theNumberOfImages << std::endl; 
   }
   
   if ( theNumberOfImages == 0 )
   {
      setErrorStatus();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "DEBUG:\nNo images in file!" << endl;
      }
      
      return false;
   }
   theEntryList.clear();
   //---
   // Get image header pointers.  Note there can be multiple images in one
   // image file.
   //---
   for (ossim_uint32 i = 0; i < theNumberOfImages; ++i)
   {
      ossimNitfImageHeader* hdr = theNitfFile->getNewImageHeader(i);
      if (!hdr)
      {
         setErrorStatus();
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " ERROR:\nNull image header!" << endl;
         }
         
         return false;
      }
      if (traceDebug())
      {
         if(hdr)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << "DEBUG:"
               << "\nImage header[" << i << "]:\n" << *hdr
               << endl;
         }
      }
      if(!hdr->isCompressed())
      {
         theEntryList.push_back(i);
      }
      else
      {
         if(isVqCompressed(hdr->getCompressionCode())&&
            (hdr->getCompressionHeader().valid()))
         {
            // we will only support single band vq compressed NITFS
            // basically CIB and CADRG products are single band code words.
            //
            if(hdr->getNumberOfBands() == 1)
            {
               theEntryList.push_back(i);
            }
            theCacheEnabledFlag = true;
         }
         else
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Entry " << i
                  <<" has an unsupported compression code = "
                  << hdr->getCompressionCode() << std::endl;
            }
         }
      }
      theNitfImageHeader.push_back(hdr);
   }
   if(theEntryList.size()<1)
   {
      return false;
   }
   if(theNitfImageHeader.size() == 1)
   {
      theLut = theNitfImageHeader[0]->createLut(0);
   }
   if(theEntryList.size()>0)
   {
      theCurrentEntry = theEntryList[0];
   }

   theNumberOfImages = theNitfImageHeader.size();
   
   if (theNitfImageHeader.size() != theNumberOfImages)
   {
      setErrorStatus();
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE
            << "DEBUG:\nNumber of header not equal number of images!"
            << endl;
      }
      
      return false;
   }

   // Check the current entry range.
   if ( theCurrentEntry >= theNumberOfImages )
   {
      if(theEntryList.size())
      {
         theCurrentEntry = theEntryList[0];
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nCurrent entry:  " << theCurrentEntry
         << endl;
   }


   // Open up a stream to the file.
   theFileStr.open(file.c_str(), ios::in | ios::binary);
   if (!theFileStr)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " ERROR:"
            << "\nCannot open:  " << file.c_str() << endl;
      }
      return false;
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " leaving with true..." << endl;
      
   }
   
   return true;
}

bool ossimNitfTileSource::allocate()
{
   // Clear out the cache if there was any.
   if (theCacheId != -1)
   {
      ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
      theCacheId = -1;
   }

   // Set the scalar type.
   initializeScalarType();
   if (theScalarType == OSSIM_SCALAR_UNKNOWN)
   {
      return false;
   }

   // Set the swap bytes flag.
   initializeSwapBytesFlag();
   
   // Set the read mode.
   initializeReadMode();
   if (theReadMode == READ_MODE_UNKNOWN)
   {
      return false;
   }
   
   // Set the number of bands.
   initializeBandCount();
   if (theNumberOfInputBands == 0)
   {
      return false;
   }

   // Initialize the image rectangle. before the cache size is done
   //
   initializeImageRect();
   if (theImageRect.hasNans())
   {
      return false;
   }
   
   // Initialize the cache size.  Must be done before
   // setting the blocksize.  Since bit encoded data may very
   // and we need to know if the nitf file needs to be accessed
   // like a general raster.
   //
   initializeCacheSize();
   if ( (theCacheSize.x == 0) || (theCacheSize.y == 0) )
   {
      return false;
   }
   
   // Initialize the block size.
   initializeBlockSize();
   if (theBlockSizeInBytes == 0)
   {
      return false;
   }


   // Initialize the cache tile interleave type.
   initializeCacheTileInterLeaveType();
   if (theCacheTileInterLeaveType == OSSIM_INTERLEAVE_UNKNOWN)
   {
      return false;
   }

   //---
   // Initialize the cache tile.  This will be used for a block buffer even
   // if the cache is disabled.
   //---
   initializeCacheTile();
   if (!theCacheTile.valid())
   {
      return false;
   }

   // Initialize the cache if enabled.
   if (theCacheEnabledFlag)
   {
      theCacheId = ossimAppFixedTileCache::instance()->
         newTileCache(theBlockImageRect, theCacheSize);
   }

   // Make the tiles.  Note the blank tile is intentionally left blank.
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();
   theTile = idf->create(this, this);
   theTile->initialize();

   // Call base class complete open.
   completeOpen();

   return true;
}

void ossimNitfTileSource::initializeReadMode()
{
   // Initialize the read mode.
   theReadMode = READ_MODE_UNKNOWN;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   ossim_uint32 numberOfBlocks = getNumberOfBlocks();
   ossimString imode           = hdr->getIMode();
   if (numberOfBlocks > 1)
   {
      if (imode == "B")
      {
         theReadMode = READ_BIB_BLOCK;
      }
      else if (imode == "P")
      {
         theReadMode = READ_BIP_BLOCK;
      }
      else if (imode == "R")
      {
         theReadMode = READ_BIR_BLOCK;
      }
      else if (imode == "S")
      {
         theReadMode = READ_BSQ_BLOCK;
      }
   }
   else // The entire image comprises one block.
   {
      if ( (imode == "B") || (imode == "S"))
      {
         theReadMode = READ_BIB;
      }
      else if (imode == "P")
      {
         theReadMode = READ_BIP;
      }
      else if (imode == "R")
      {
         theReadMode = READ_BIR;
      }
   }        
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeReadMode DEBUG:"
         << "\nnumberOfBlocks:  " << numberOfBlocks
         << "\nIMODE:           " << imode
         << "\nRead Mode:       " << READ_MODE[theReadMode]
         << endl;
   }
}

void ossimNitfTileSource::initializeScalarType()
{
   thePackedBitsFlag = false;
   // Initialize the read mode.
   theScalarType = OSSIM_SCALAR_UNKNOWN;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   ossim_int32 bitsPerPixel = hdr->getActualBitsPerPixelPerBand();
   if (bitsPerPixel < 1)
   {
      bitsPerPixel = hdr->getBitsPerPixelPerBand();
   }

   ossimString pixelValueType = hdr->getPixelValueType().upcase();
   
   switch (bitsPerPixel)
   {
      case 8:
      {
         theScalarType = OSSIM_UCHAR;
         break;
      }
      case 11:
      {
         theScalarType = OSSIM_USHORT11;
         break;
      }
      case 16:
      {
         if(pixelValueType == "SI")
         {
            theScalarType = OSSIM_SSHORT16;
         }
         else
         {
            theScalarType = OSSIM_USHORT16;
         }
         break;
      }
      case 32:
      {
         if(pixelValueType == "R")
         {
            theScalarType = OSSIM_FLOAT;
         }
         
         break;
      }
      case 64:
      {
         if(pixelValueType == "R")
         {
            theScalarType = OSSIM_DOUBLE;
         }
         
         break;
      }
      default:
      {
         if(!hdr->isCompressed())
         {
            thePackedBitsFlag = true;
            if(bitsPerPixel < 8)
            {
               theScalarType = OSSIM_UCHAR;
            }
            else if(bitsPerPixel < 16)
            {
               theScalarType = OSSIM_USHORT16;
            }
            else if(bitsPerPixel < 32)
            {
               theScalarType = OSSIM_FLOAT;
            }
         }
         break;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeScalarType DEBUG:"
         << "\nScalar type:  "
         << (ossimScalarTypeLut::instance()->getEntryString(theScalarType))
         << "\nPacked bits:  " << (thePackedBitsFlag?"true":"false")
         << endl;
   }
}

void ossimNitfTileSource::initializeSwapBytesFlag()
{
   if ( (theScalarType != OSSIM_UCHAR) &&
        (ossimGetByteOrder() == OSSIM_LITTLE_ENDIAN) )
   {
      theSwapBytesFlag = true;
   }
   else
   {
     theSwapBytesFlag = false;
   }
}

void ossimNitfTileSource::initializeBandCount()
{
   // Initialize the read mode.
   theNumberOfInputBands = 0;
   theNumberOfOutputBands = 0;
   theOutputBandList.clear();
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   if(!isVqCompressed(hdr->getCompressionCode()))
   {
      theNumberOfInputBands = hdr->getNumberOfBands();
      theNumberOfOutputBands = hdr->getNumberOfBands();
   }
   else 
   {
      theNumberOfInputBands = 1;
      theNumberOfOutputBands = 3;
//       const ossimRefPtr<ossimNitfCompressionHeader> header = hdr->getCompressionHeader();

//       ossimNitfVqCompressionHeader* header = PTR_CAST(ossimNitfVqCompressionHeader,
//                                                       header.get());

//       if(header)
//       {
         
//       }
   }
   
   theOutputBandList.resize(theNumberOfOutputBands);
   
   for (ossim_uint32 i=0; i < theNumberOfOutputBands; ++i)
   {
      theOutputBandList[i] = i; // One to one for initial setup.
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeBandCount DEBUG:"
         << "\nInput Band count:  " << theNumberOfInputBands
         << "\nOutput Band count:  " << theNumberOfOutputBands
         << endl;
   }
}

void ossimNitfTileSource::initializeBlockSize()
{
   theBlockSizeInBytes     = 0;
   theReadBlockSizeInBytes = 0;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   
                                                              
  // This returns block size in bytes.
  //     ossim_uint32 bytesPerPixel =
  //        static_cast<ossim_uint32>(ceil(hdr->getBitsPerPixelPerBand()/8.0));

   ossim_uint32 bytesRowCol = 0;
   ossim_uint32 bytesRowColCacheTile = 0;

   if(isVqCompressed(hdr->getCompressionCode()))
   {
      bytesRowCol = OSSIM_NITF_VQ_BLOCKSIZE;
   }
   else
   {
      bytesRowCol = (hdr->getNumberOfPixelsPerBlockHoriz()*
                     hdr->getNumberOfPixelsPerBlockVert()*
                     hdr->getBitsPerPixelPerBand()) / 8;
   }
   
   bytesRowColCacheTile = (theCacheSize.x*
                           theCacheSize.y*
                           hdr->getBitsPerPixelPerBand())/8;

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG:"
         << "\ncompressionHeader:  " << compressionHeader
         << "\ngetNumberOfPixelsPerBlockHoriz():  "
         << hdr->getNumberOfPixelsPerBlockHoriz()
         << "\ngetNumberOfPixelsPerBlockVert():  "
         << hdr->getNumberOfPixelsPerBlockVert()
         << "\ngetBitsPerPixelPerBand():  "
         << hdr->getBitsPerPixelPerBand()
         << "\nbytesRowCol:  " << bytesRowCol
         << "\nbytesRowColCacheTile:  " << bytesRowColCacheTile
         << endl;
   }
#endif
   
   theBlockSizeInBytes = bytesRowCol;
   theReadBlockSizeInBytes = theBlockSizeInBytes;
   switch (theReadMode)
   {
      case READ_BSQ_BLOCK:
      case READ_BIB_BLOCK:
      {
         break;
      }
      case READ_BIB:
      {
         theReadBlockSizeInBytes = bytesRowColCacheTile;
         
         break;
      }
      
      case READ_BIP_BLOCK:
      case READ_BIR_BLOCK:
      {
         theBlockSizeInBytes     *= theNumberOfInputBands;
         theReadBlockSizeInBytes *= theNumberOfInputBands;
         break;
      }
      case READ_BIP:
      case READ_BIR:   
      {
         theBlockSizeInBytes *= theNumberOfInputBands;
         theReadBlockSizeInBytes = bytesRowColCacheTile*theNumberOfInputBands;
         break;
      }
      
      default:
      {
         break;
      }
   }

//#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeBlockSize DEBUG:"
         << "\nNumber of input bands:          " << theNumberOfInputBands
         << "\nNumber of output bands:          " << theNumberOfOutputBands
         << "\nBlock size in bytes:      " << theBlockSizeInBytes
         << "\nRead block size in bytes: " << theReadBlockSizeInBytes
         << endl;
   }
//#endif
}

void ossimNitfTileSource::initializeImageRect()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      theImageRect.makeNan();
      return;
   }

   theBlockImageRect = hdr->getBlockImageRect();
   theImageRect      = hdr->getImageRect();
   theSubImageOffset = theImageRect.ul();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeImageRect DEBUG:"
         << "\nImage Rect:        " << theImageRect
         << "\nSub image offset:  " << theSubImageOffset
         << endl;
   }
}

void ossimNitfTileSource::initializeCacheSize()
{
   theCacheSize.x = 0;
   theCacheSize.y = 0;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }
   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      case READ_BIP_BLOCK:
      case READ_BIR_BLOCK:
      case READ_BSQ_BLOCK:
         theCacheSize.x = hdr->getNumberOfPixelsPerBlockHoriz();
         theCacheSize.y = hdr->getNumberOfPixelsPerBlockVert();
         break;

      case READ_BIB:
      case READ_BIP:
      case READ_BIR:
         theCacheSize.x = getNumberOfSamples(0);
         theCacheSize.y = getTileHeight();
         if(theCacheSize.y > hdr->getNumberOfPixelsPerBlockVert())
         {
            theCacheSize.y = hdr->getNumberOfPixelsPerBlockVert();
         }
         break;

      default:
         break;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeCacheSize DEBUG:"
         << "\nCache size:  " << theCacheSize
         << endl;
   }
}
void ossimNitfTileSource::initializeCacheTileInterLeaveType()
{
   theCacheTileInterLeaveType = OSSIM_INTERLEAVE_UNKNOWN;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      case READ_BSQ_BLOCK:
      case READ_BIB:
         theCacheTileInterLeaveType = OSSIM_BSQ;
         break;

      case READ_BIP_BLOCK:
      case READ_BIP:
         theCacheTileInterLeaveType = OSSIM_BIP;
         break;

      case READ_BIR_BLOCK:
      case READ_BIR:
         theCacheTileInterLeaveType = OSSIM_BIL;
         break;

      default:
         break;
   }

   if (traceDebug())
   {
      ossimInterleaveTypeLut lut;
      
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeCacheTileInterLeaveType DEBUG:"
         << "\nCache tile interleave type:  "
         << lut.getEntryString(theCacheTileInterLeaveType)
         << endl;
   }
}

void ossimNitfTileSource::initializeCacheTile()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   if(theCompressedBuf)
   {
      delete [] theCompressedBuf;
      theCompressedBuf = NULL;
   }
   
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();
   theCacheTile = idf->create(this,
                              theScalarType,
                              theNumberOfOutputBands,
                              theCacheSize.x,
                              theCacheSize.y);
   theCacheTile->initialize();

   if(hdr->getRepresentation().upcase().contains("LUT"))
   {
      theCompressedBuf = new ossim_uint8[theReadBlockSizeInBytes];
      memset(theCompressedBuf, '\0', theReadBlockSizeInBytes);
   }
   else if(isVqCompressed(hdr->getCompressionCode()))
   {
      theCompressedBuf = new ossim_uint8[theReadBlockSizeInBytes];
      memset(theCompressedBuf, '\0', theReadBlockSizeInBytes);
   }
}

ossimRefPtr<ossimImageData> ossimNitfTileSource::getTile(
   const  ossimIrect& tileRect, ossim_uint32 resLevel)
{
   // This tile source bypassed, or invalid res level, return a blank tile.
   if(!isSourceEnabled() || !isOpen() || !isValidRLevel(resLevel))
   {
      return ossimRefPtr<ossimImageData>();
   }

   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         // Subtract any offsets since the overview doesn't know about them.
         ossimIrect rr_rect = tileRect - getSubImageOffset(resLevel);

         //---
         // If there is a sub image offset then we will not be requesting
         // the same rectangle as was requested from us.  For this reason,
         // we'll do a setOrigin, rather than just returning the getTile
         // result.
         //---
         ossimRefPtr<ossimImageData> tileData = theOverview->getTile(rr_rect,
                                                                     resLevel);
         if(tileData.valid())
         {
            tileData->setImageRectangle(tileRect);
         }
         
         if(getOutputScalarType() == OSSIM_USHORT11)
         {
            //---
            // Temp fix:
            // The overview handler could return a tile of OSSIM_UINT16 if
            // the max sample value was not set to 2047.  
            //---
            tileData->setScalarType(OSSIM_USHORT11);
         }
         
         return tileData;
      }
   }

   if (!theTile.valid())
   {
      return theTile;
   }
   
   theTile->setImageRectangle(tileRect);

   ossimIrect  cacheRect  = theCacheTile->getImageRectangle();
   
   //---
   // See if any point of the requested tile is in the image.
   //---
   if ( tileRect.intersects(theBlockImageRect) )
   {
      ossimIrect clipRect = tileRect.clipToRect(theImageRect);

      //---
      // See if the whole tile is going to be filled, if not, start out with
      // a blank tile so data from a previous load gets wiped out.
      //---
      if ( !tileRect.completely_within(clipRect) )
      {
         // Start with a blank tile.
         if (theTile->getDataObjectStatus() != OSSIM_EMPTY)
         {
            theTile->makeBlank();
         }
      }
            
      // See if the requested clip rect is already in the cache tile.
      bool status = true;
      if ( (clipRect.completely_within(theCacheTile->getImageRectangle()))&&
           (theCacheTile->getDataObjectStatus() != OSSIM_EMPTY)&&
           (theCacheTile->getBuf()))
      {
         theTile->loadTile(theCacheTile->getBuf(),
                           theCacheTile->getImageRectangle(),
                           theCacheTileInterLeaveType);
      }
      else
      {
         
         status = loadTile(clipRect);
      }

      if (status)
      {
         //---
         // Validate the tile.  This will set the status to full, partial
         // or empty.  Must be performed if any type of combining is to be
         // performed down the chain.
         //---
         theTile->validate();
         return theTile;
      }

   } // End of if ( tileRect.intersects(image_rect) )

   // No part of requested tile within the image rectangle or loadTile failed.
   return ossimRefPtr<ossimImageData>();   
}

bool ossimNitfTileSource::loadTile(const ossimIrect& clipRect)
{
   // Subtract any offsets and work in zero based image space.
   ossimIrect zbClipRect  = clipRect - theSubImageOffset;

   const ossim_uint32 BLOCK_HEIGHT = theCacheSize.y;
   const ossim_uint32 BLOCK_WIDTH  = theCacheSize.x;

   zbClipRect.stretchToTileBoundary(ossimIpt(BLOCK_WIDTH, BLOCK_HEIGHT));
   
   //---
   // Shift the upper left corner of the "clip_rect" to the an even nitf
   // block boundry.  
   //---
   ossimIpt nitfBlockOrigin = zbClipRect.ul();
//   adjustToStartOfBlock(nitfBlockOrigin, BLOCK_HEIGHT, BLOCK_WIDTH);
   
   // Vertical block loop.
   ossim_int32 y = nitfBlockOrigin.y;
   while (y < zbClipRect.lr().y)
   {
      // Horizontal block loop.
      ossim_int32 x = nitfBlockOrigin.x;
      while (x < zbClipRect.lr().x)
      {
         if (loadBlock(x, y))
         {
            theTile->loadTile(theCacheTile->getBuf(),
                              theCacheTile->getImageRectangle(),
                              theCacheTileInterLeaveType);
         }
         else
         {
            // Error loading...
            return false;
         }
         
         x += BLOCK_WIDTH; // Go to next block.
      }
      
      y += BLOCK_HEIGHT; // Go to next row of blocks.
   }

   return true;
}

bool ossimNitfTileSource::loadBlock(ossim_uint32 x, ossim_uint32 y)
{
#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::loadBlock DEBUG:"
         << "  x:  " << x << " y:  " << y << endl;
   }
#endif
   
   //---
   // The origin set in the cache tile must have the sub image offset in it
   // since "theTile" is relative to any sub image offset.  This is so that
   // "theTile->loadTile(theCacheTile)" will work.
   //---
   ossimIpt origin(x, y);
   origin = origin + theSubImageOffset;
   
   if (theCacheEnabledFlag)
   {
      // See if it's in the cache already.
      ossimRefPtr<ossimImageData> tempTile = NULL;
      tempTile = ossimAppFixedTileCache::instance()->
         getTile(theCacheId, origin);
      if (tempTile.valid())
      {
         theTile->loadTile(tempTile.get());
         return true;
      }
   }
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   theCacheTile->setOrigin(origin);
   ossim_uint32 readSize = theReadBlockSizeInBytes;
   if(!theCacheTile->getImageRectangle().completely_within(theBlockImageRect))
   {
      readSize = getPartialReadSize(origin);
   }
   if((hdr->hasBlockMaskRecords())||
      (readSize != theReadBlockSizeInBytes))
   {
      theCacheTile->makeBlank();
   }

   switch (theReadMode)
   {
      case READ_BIR:  
      case READ_BIR_BLOCK:
      case READ_BIP:  
      case READ_BIP_BLOCK:
      {
         std::streampos p;
         if(getPosition(p, x, y, 0))
         {
            theFileStr.seekg(p, ios::beg);
            char* buf = (char*)(theCacheTile->getBuf());
            if (!theFileStr.read(buf, readSize))
            {
               theFileStr.clear();
               ossimNotify(ossimNotifyLevel_FATAL)
                  << "ossimNitfTileSource::loadBlock BIP Read Error!"
                  << "\nReturning error..." << endl;
               theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
               
               return false;
            }
         }
         break;
      }
      case READ_BSQ_BLOCK:
      case READ_BIB_BLOCK:
      case READ_BIB:
      {
         //---
         // NOTE:
         // With some of these types we could do one read and get all bands.
         // The reads are done per for future enabling on band selection
         // at the image handler level.
         //---
         for (ossim_uint32 band = 0; band < theNumberOfInputBands; ++band)
         {
            ossim_uint8* buf =0;
            if(isVqCompressed(hdr->getCompressionCode())||
               hdr->getRepresentation().upcase().contains("LUT"))
            {
               buf = theCompressedBuf;
            }
            else
            {
               buf = (ossim_uint8*)(theCacheTile->getBuf(band));
            }
            std::streampos p;
            if(getPosition(p, x, y, band))
            {
               theFileStr.seekg(p, ios::beg);
               if (!theFileStr.read((char*)buf, readSize))
               {
                  theFileStr.clear();
                  ossimNotify(ossimNotifyLevel_FATAL)
                     << "ossimNitfTileSource::loadBlock Read Error!"
                     << "\nReturning error..." << endl;
                  theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                  return false;
               }
               else if(isVqCompressed(hdr->getCompressionCode()))
               {
                  vqUncompress(theCacheTile, theCompressedBuf);
               }
               else if(hdr->getRepresentation().upcase().contains("LUT"))
               {
                  lutUncompress(theCacheTile, theCompressedBuf);
               }
            }
         }
         break;
      }

      default:
         break;
   }
   
   if(thePackedBitsFlag)
   {
      explodePackedBits(theCacheTile);
   }
   // Check for swap bytes.
   if (theSwapBytesFlag)
   {
      ossimEndian swapper;
      swapper.swap(theScalarType,
                   theCacheTile->getBuf(),
                   theCacheTile->getSize());
   }
   convertTransparentToNull(theCacheTile);

   // Set the origin of the cache tile.
   theCacheTile->validate();
   if (theCacheEnabledFlag)
   {
      // Add it to the cache for the next time.
      ossimAppFixedTileCache::instance()->addTile(theCacheId, theCacheTile);
   }
   
   return true;
}

void ossimNitfTileSource::explodePackedBits(ossimRefPtr<ossimImageData> packedBuffer)const
{
   ossim_uint8* tempBuf = new ossim_uint8[packedBuffer->getSizePerBandInBytes()];
   ossim_uint32 idx      = 0;
   ossim_uint32 bandIdx  = 0;
   ossim_uint32 h = packedBuffer->getHeight();
   ossim_uint32 w = packedBuffer->getWidth();
   ossim_uint32 maxIdx = w*h;
   ossim_uint32 bandCount = packedBuffer->getNumberOfBands();
   switch(packedBuffer->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         
         ossim_uint8* outputBuf = (ossim_uint8*)tempBuf;
         for(bandIdx = 0; bandIdx < bandCount; ++bandIdx)
         {
            ossimNitfPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
                                           getCurrentImageHeader()->getBitsPerPixelPerBand());
            for(idx = 0; idx < maxIdx; ++idx)
            {
               *outputBuf = (ossim_uint8)packedBits.getValueAsUint32(idx);
               ++outputBuf;
            }
            
            memcpy((char*)packedBuffer->getBuf(bandIdx),
                   (char*)tempBuf,
                   theCacheTile->getSizePerBandInBytes()*bandCount);
         }
         break;
      }
      case OSSIM_USHORT16:
      {
         
         ossim_uint16* outputBuf = (ossim_uint16*)tempBuf;
         for(bandIdx = 0; bandIdx < bandCount; ++bandIdx)
         {
            ossimNitfPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
                                           getCurrentImageHeader()->getBitsPerPixelPerBand());
            for(idx = 0; idx < maxIdx; ++idx)
            {
               *outputBuf = (ossim_uint16)packedBits.getValueAsUint32(idx);
               ++outputBuf;
            }
            
            memcpy((char*)packedBuffer->getBuf(bandIdx),
                   (char*)tempBuf,
                   theCacheTile->getSizePerBandInBytes()*bandCount);
         }
         break;
      }
      case OSSIM_FLOAT:
      {
         ossim_float32* outputBuf = (ossim_float32*)tempBuf;
         for(bandIdx = 0; bandIdx < bandCount; ++bandIdx)
         {
            ossimNitfPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
                                           getCurrentImageHeader()->getBitsPerPixelPerBand());
            for(idx = 0; idx < maxIdx; ++idx)
            {
               *outputBuf = (ossim_float32)packedBits.getValueAsUint32(idx);
               ++outputBuf;
            }
            
            memcpy((char*)packedBuffer->getBuf(bandIdx),
                   (char*)tempBuf,
                   theCacheTile->getSizePerBandInBytes()*bandCount);
         }
         break;
      }
      default:
      {
         break;
      }
   }
   delete [] tempBuf;
}

void ossimNitfTileSource::convertTransparentToNull(ossimRefPtr<ossimImageData> tile)const
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();

   if(!hdr||!tile) return;

   if(!tile->getBuf()) return;
   ossimIpt tempOrigin = tile->getOrigin()-theSubImageOffset;
   ossim_uint32 blockNumber = getBlockNumber(tempOrigin);
   ossim_uint32 numberOfBands = tile->getNumberOfBands();
   ossim_uint32 band = 0;
   if(hdr->hasPadPixelMaskRecords())
   {
      if(hdr->hasTransparentCode())
      {
         ossim_uint32 idx = 0;
         ossim_uint32 maxIdx = tile->getWidth()*tile->getHeight();
         
         for (band = 0; band < numberOfBands; ++band)
         {
            if(hdr->getPadPixelMaskRecordOffset(blockNumber,
                                                band)!=0xffffffff)
            {
               switch(tile->getScalarType())
               {
                  case OSSIM_UCHAR:
                  {
                     ossim_uint8 transparentValue = hdr->getTransparentCode();
                     ossim_uint8* buf = (ossim_uint8*)tile->getBuf(band);
                     ossim_uint8 nullPix = (ossim_uint8)tile->getNullPix(band);
                     for(idx = 0; idx < maxIdx; ++idx)
                     {
                        if(*buf == transparentValue)
                        {
                           *buf = nullPix;
                        }
                        ++buf;
                     }
                     break;
                  }
                  case OSSIM_USHORT11:
                  case OSSIM_USHORT16:
                  {
                     ossim_uint16 transparentValue = hdr->getTransparentCode();
                     ossim_uint16* buf = (ossim_uint16*)tile->getBuf(band);
                     ossim_uint16 nullPix = (ossim_uint16)tile->getNullPix(band);
                     for(idx = 0; idx < maxIdx; ++idx)
                     {
                        if(*buf == transparentValue)
                        {
                           *buf = nullPix;
                        }
                        ++buf;
                     }
                     break;
                  }
                  case OSSIM_SSHORT16:
                  {
                     ossim_sint16 transparentValue = hdr->getTransparentCode();
                     ossim_sint16* buf = (ossim_sint16*)tile->getBuf(band);
                     ossim_sint16 nullPix = (ossim_sint16)tile->getNullPix(band);
                     for(idx = 0; idx < maxIdx; ++idx)
                     {
                        if(*buf == transparentValue)
                        {
                           *buf = nullPix;
                        }
                        ++buf;
                     }
                     break;
                  }
				  default:
				  {
					  break;
				  }
               }
            }
         }
      }
   }
}


double ossimNitfTileSource::getMinPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getMinPixelValue(band);

   if(thePackedBitsFlag)
   {
      if(result < 1.0) result = 1.0;
   }
   
   return result;
}

double ossimNitfTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getMaxPixelValue(band);
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();

   if(hdr)
   {
      if(thePackedBitsFlag)
      {
         double test = 1<<(hdr->getBitsPerPixelPerBand());
         
         if(result > test) result = test;
      }
   }

   return result;
}

double ossimNitfTileSource::getNullPixelValue(ossim_uint32 band)const
{
   double result = ossimImageHandler::getNullPixelValue(band);

   if(thePackedBitsFlag)
   {
      if((result < 0) ||
         (result > getMaxPixelValue(band)))
         {
            result = 0.0;
         }
   }


   return result;
}


bool ossimNitfTileSource::getPosition(std::streampos& streamPosition,
                                      ossim_uint32 x,
                                      ossim_uint32 y,
                                      ossim_uint32 band) const
{
   //
   // NOTE:  "theCacheSize is always relative to a block size except in
   // the case where a block is the entire image.
   //
   streamPosition = 0;
   
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return streamPosition;
   }

   ossim_uint32 blockNumber = getBlockNumber(ossimIpt(x,y));

#if 0
   cout << "ossimNitfTileSource::getPosition blockNumber:  "
        << blockNumber << endl;
#endif
   
   streamPosition = hdr->getDataLocation(); // Position to first block.
   if(hdr->hasBlockMaskRecords())
   {
      ossim_uint32 blockOffset = hdr->getBlockMaskRecordOffset(blockNumber,
                                                               band);
      if(blockOffset == 0xffffffff)
      {
         return false;
      }
      streamPosition += blockOffset;
   }
   
   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      {
         if(!hdr->hasBlockMaskRecords())
         {
            streamPosition +=
               (blockNumber * getBlockOffset()) +
               (getBandOffset() * band);
         }
         else
         {
            streamPosition += (getBandOffset() * band);
            
         }
         break;
      }
   
      case READ_BIB:
      {
         streamPosition +=
            (blockNumber * theReadBlockSizeInBytes)+
            (getBandOffset() * band);
         break;
      }

      case READ_BSQ_BLOCK:
      {
         
         if(!hdr->hasBlockMaskRecords())
         {
            streamPosition += (blockNumber * getBlockOffset()) +
               (getBandOffset() * band);
         }
         
         break;
      }
      default:
      {
         if(!hdr->hasBlockMaskRecords())
         {
            streamPosition += (blockNumber*getBlockOffset());
         }
         
         break;
      }
   }

   return true;
}

std::streampos ossimNitfTileSource::getBandOffset() const
{
   std::streampos bandOffset = 0;

   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      case READ_BIP_BLOCK:
      case READ_BIR_BLOCK:
      case READ_BIB:
      case READ_BIP:
      case READ_BIR:
         bandOffset = theBlockSizeInBytes;
         break;
         
      case READ_BSQ_BLOCK:
         bandOffset = getNumberOfBlocks() * theBlockSizeInBytes;
         break;

      default:
         break;
   }

   return bandOffset;
}

std::streampos ossimNitfTileSource::getBlockOffset() const
{
   std::streampos blockOffset = 0;
   std::streampos blockSizeInBytes = 0;
   if (getNumberOfBlocks() == 1)
   {
      blockSizeInBytes = theReadBlockSizeInBytes;
   }
   else
   {
      blockSizeInBytes = theBlockSizeInBytes;
   }
   
   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      case READ_BIB:
         // Band interleaved by block.
         blockOffset = blockSizeInBytes * theNumberOfInputBands;
         break;
         
      case READ_BIR_BLOCK:
      case READ_BSQ_BLOCK:
      case READ_BIP_BLOCK:
      case READ_BIP:
      case READ_BIR:
         // Blocks side by side.
         blockOffset = blockSizeInBytes;
         break;
      
      default:
         break;
   }

   return blockOffset;
}
   
ossim_uint32 ossimNitfTileSource::getNumberOfBlocks() const
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return 0;
   }

   return static_cast<ossim_uint32>( hdr->getNumberOfBlocksPerRow() *
                                     hdr->getNumberOfBlocksPerCol() );
}

bool ossimNitfTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if ( !ossimImageHandler::loadState(kwl, prefix) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfTileSource::loadState(kwl, prefix) DEBUG:"
            << "\nUnable to load, exiting..." << std::endl;
      }
      return false;
   }
   
   const char* lookup = kwl.find(prefix, "entry");
   if (lookup)
   {
      ossimString s(lookup);
      theCurrentEntry = s.toUInt32();
   }

   lookup = kwl.find(prefix,ossimKeywordNames::ENABLE_CACHE_KW);
   if (lookup)
   {
      ossimString s(lookup);
      theCacheEnabledFlag = s.toBool();
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::loadState(kwl, prefix) DEBUG:"
         << "\nCurrent entry:      " << theCurrentEntry
         << "\nCache enable flag:  " << theCacheEnabledFlag
         << std::endl;
   }
   
   return open();
}

bool ossimNitfTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   // Add the entry number.
   kwl.add(prefix, "entry", theCurrentEntry, true);

   // Add the cache_enable flag.
   kwl.add(prefix, ossimKeywordNames::ENABLE_CACHE_KW, theCacheEnabledFlag, true);

   // Call the base class save state.
   return ossimImageHandler::saveState(kwl, prefix);
}

ossimScalarType ossimNitfTileSource::getOutputScalarType() const
{
   return theScalarType;
}

ossim_uint32 ossimNitfTileSource::getTileWidth() const
{
   if (theTile.valid())
   {
      return theTile->getWidth();
   }

   ossimIpt tileSize;
   ossimGetDefaultTileSize(tileSize);
   return static_cast<ossim_uint32>(tileSize.x);
}

ossim_uint32 ossimNitfTileSource::getTileHeight() const
{
   if (theTile.valid())
   {
      return theTile->getHeight();
   }

   ossimIpt tileSize;
   ossimGetDefaultTileSize(tileSize);
   return static_cast<ossim_uint32>(tileSize.y);
}

ossim_uint32 ossimNitfTileSource::getNumberOfInputBands() const
{
   return theNumberOfInputBands;
}

ossim_uint32 ossimNitfTileSource::getNumberOfOutputBands() const
{
   return theNumberOfOutputBands;
}

ossim_uint32 ossimNitfTileSource::getNumberOfLines(ossim_uint32 resLevel) const
{
   if (resLevel == 0)
   {
      const ossimNitfImageHeader* hdr = getCurrentImageHeader();
      if (!hdr)
      {
         return 0;
      }
      return  hdr->getNumberOfRows();
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfLines(resLevel);
   }
   return 0;
}

ossim_uint32 ossimNitfTileSource::getNumberOfSamples(ossim_uint32 resLevel) const
{
   if (resLevel == 0)
   {
      const ossimNitfImageHeader* hdr = getCurrentImageHeader();
      if (!hdr)
      {
         return 0;
      }
      return  hdr->getNumberOfCols();
   }
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(resLevel);
   }
   return 0;
}

void ossimNitfTileSource::adjustToStartOfBlock(ossimIpt& pt,
                                               ossim_int32 tile_height,
                                               ossim_int32 tile_width) const
{
   if (pt.x > 0)
   {
      pt.x = pt.x/tile_width*tile_width;
   }
   else if (pt.x < 0)
   {
      pt.x = (pt.x-tile_width)/tile_width*tile_width;
   }

   if (pt.y > 0)
   {
      pt.y = pt.y/tile_height*tile_height;
   }
   else if (pt.y < 0)
   {
      pt.y = (pt.y-tile_height)/tile_height*tile_height;
   }
}

ossim_uint32 ossimNitfTileSource::getBlockNumber(const ossimIpt& block_origin) const
{
   ossim_uint32 blockNumber = 0;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return blockNumber;
   }

   ossim_uint32 blockY;
   ossim_uint32 blockX;
   blockX  = (block_origin.x /
              theCacheSize.x);
   blockY= (block_origin.y /
            theCacheSize.y);

   switch (theReadMode)
   {
      case READ_BIB_BLOCK:
      case READ_BIP_BLOCK:
      case READ_BIR_BLOCK:
      case READ_BSQ_BLOCK:
         
         blockNumber = ((blockY*hdr->getNumberOfBlocksPerRow()) +
                        blockX);
         break;
      case READ_BIB:
      case READ_BIP:
      case READ_BIR:
         //---
         // These read modes are for a single block image.  The cache size will
         // be set to the width of the image (block) by the height of one tile.
         //
         // This is to avoid reading an entire large image with a single block
         // into memory.
         //---
         blockNumber = blockY;
         break;

      default:
         break;
   }
   return blockNumber;
}

ossim_uint32 ossimNitfTileSource::getPartialReadSize(const ossimIpt& blockOrigin)const
{
   ossim_uint32 result = 0;
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return result;
   }
   
   if(theCacheTile->getImageRectangle().completely_within(theBlockImageRect))
   {
      return theReadBlockSizeInBytes;
   }
   ossimIrect clipRect = theCacheTile->getImageRectangle().clipToRect(theBlockImageRect);
   
   result = (theCacheSize.x*
             clipRect.height()*
             hdr->getBitsPerPixelPerBand())/8;
   
   switch (theReadMode)
   {
      case READ_BSQ_BLOCK:
      case READ_BIB_BLOCK:
      case READ_BIB:
      {
         // purposely left blank.  only hear for clarity.
         break;
      }

      case READ_BIP_BLOCK:
      case READ_BIR_BLOCK:
      case READ_BIP:
      case READ_BIR:   
      {
         result *= theNumberOfInputBands;
         break;
      }
      default:
      {
         break;
      }
   }
   return result;
}

bool ossimNitfTileSource::isVqCompressed(const ossimString& compressionCode)const
{
   return((compressionCode == "C4")||
          (compressionCode == "M4"));
}


ossim_uint32 ossimNitfTileSource::getImageTileWidth() const
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return 0;
   }
   return hdr->getNumberOfPixelsPerBlockHoriz();
}

ossim_uint32 ossimNitfTileSource::getImageTileHeight() const
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return 0;
   }
   return hdr->getNumberOfPixelsPerBlockVert();
}

ossimString ossimNitfTileSource::getShortName()const
{
   return ossimString("nitf");
}

ossimString ossimNitfTileSource::getLongName()const
{
   return ossimString("nitf reader");
}

ossimString ossimNitfTileSource::className()const
{
   return ossimString("ossimNitfTileSource");
}

ossim_uint32 ossimNitfTileSource::getCurrentEntry() const
{
   return theCurrentEntry;
}

ossim_uint32 ossimNitfTileSource::getNumberOfEntries() const
{
   return theEntryList.size();
}

void ossimNitfTileSource::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   entryList = theEntryList;
//    entryList.resize(theNumberOfImages);
//    for (ossim_uint32 i = 0; i < theNumberOfImages; ++i)
//    {
//       entryList[i] = i;
//    }
}

bool ossimNitfTileSource::setCurrentEntry(ossim_uint32 entryIdx)
{
   if (theCurrentEntry == entryIdx)
   {
      return true; // Nothing to change.
   }

   if ( isOpen() )
   {
      if ( entryIdx < theNumberOfImages )
      {
         theCurrentEntry = entryIdx;
         //---
         // Since we were previously open and the the entry has changed we
         // need to reinitialize some things.
         //---
         allocate();
      }
      else
      {
         return false;
      }
   }
   else
   {
      //---
      // Not open.
      // Allow this know that the parseFile will check for out of range.
      //---
      theCurrentEntry = entryIdx;
   }

   return true;
}

bool ossimNitfTileSource::getCacheEnabledFlag() const
{
   return theCacheEnabledFlag;
}

void ossimNitfTileSource::setCacheEnabledFlag(bool flag)
{
   if (flag != theCacheEnabledFlag)
   {
      // State of caching has changed...

      theCacheEnabledFlag = flag;

      if ( theCacheEnabledFlag) // Cache enabled.
      {
         theCacheId = ossimAppFixedTileCache::instance()->
            newTileCache(theBlockImageRect, theCacheSize);
      }
      else // Cache disabled...
      {
         // Clean out the cache if there was one.
         if (theCacheId != -1)
         {
            ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
            theCacheId = -1;
         }
      }
   }
}

const ossimNitfImageHeader* ossimNitfTileSource::getCurrentImageHeader() const
{
   return theNitfImageHeader[theCurrentEntry];
}

void ossimNitfTileSource::setBoundingRectangle(const ossimIrect& imageRect)
{
   theImageRect = imageRect;
   theSubImageOffset = theImageRect.ul();
   
   // now shift the internal block rect as well
   theBlockImageRect = (theBlockImageRect - theBlockImageRect.ul()) + theSubImageOffset;
}

ossimRefPtr<ossimProperty> ossimNitfTileSource::getProperty(const ossimString& name)const
{
   if (name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      ossimProperty* p = new ossimBooleanProperty(name, theCacheEnabledFlag);
      return ossimRefPtr<ossimProperty>(p);
   }

   return ossimImageHandler::getProperty(name);
}

void ossimNitfTileSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property) return;
   
   ossimString name = property->getName();

   if (name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      ossimBooleanProperty* obj = PTR_CAST(ossimBooleanProperty,
                                           property.get());
      if (obj)
      {
         setCacheEnabledFlag(obj->getBoolean());
      }
   }
   else
   {
      ossimImageHandler::setProperty(property);
   }
}

void ossimNitfTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(ossimKeywordNames::ENABLE_CACHE_KW);

   ossimImageHandler::getPropertyNames(propertyNames);
}

ossimString ossimNitfTileSource::getSecurityClassification() const
{
   if(getCurrentImageHeader())
   {
      return getCurrentImageHeader()->getSecurityClassification();
   }
   
   return "U";
}

void ossimNitfTileSource::lutUncompress(ossimRefPtr<ossimImageData> destination, ossim_uint8* source)
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr||!destination)
   {
      return;
   }
   if((destination->getNumberOfBands()<3)||
      (!destination->getBuf())||
      (destination->getScalarType()!=OSSIM_UCHAR)||
      (!theLut.valid()))
   {
      return;
   }

   if(destination->getNumberOfBands()!=theLut->getNumberOfBands())
   {
      return;
   }
   
   ossim_uint8* tempRows[3];
   tempRows[0] = (ossim_uint8*)destination->getBuf(0);
   tempRows[1] = (ossim_uint8*)destination->getBuf(1);
   tempRows[2] = (ossim_uint8*)destination->getBuf(2);
   
   ossim_uint8* srcPtr = source;
   ossim_uint32 compressionYidx   = 0;
   ossim_uint32 compressionXidx   = 0;
   ossim_uint32 uncompressIdx     = 0;
   ossim_uint32 h = destination->getHeight();
   ossim_uint32 w = destination->getWidth();
   
   for(compressionYidx = 0; compressionYidx < h; ++compressionYidx)
   {
      for(compressionXidx = 0; compressionXidx < w; ++compressionXidx)
      {
         tempRows[0][uncompressIdx] = (*theLut)[*srcPtr][0];
         tempRows[1][uncompressIdx] = (*theLut)[*srcPtr][1];
         tempRows[2][uncompressIdx] = (*theLut)[*srcPtr][2];
         ++srcPtr;
         ++uncompressIdx;
      }
   }
}

void ossimNitfTileSource::vqUncompress(ossimRefPtr<ossimImageData> destination, ossim_uint8* source)
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr||!destination)
   {
      return;
   }
   if((destination->getNumberOfBands()<3)||
      (!destination->getBuf())||
      (destination->getScalarType()!=OSSIM_UCHAR))
   {
      return;
   }
   ossimNitfVqCompressionHeader* compressionHeader = PTR_CAST(ossimNitfVqCompressionHeader,
                                                              hdr->getCompressionHeader().get());

   if(!compressionHeader)
   {
      return;
   }
   const std::vector<ossimNitfVqCompressionOffsetTableData>& table = compressionHeader->getTable();
   ossimRefPtr<ossimNitfImageBand> bandInfo = hdr->getBandInformation(0);

//    if(!bandInfo.valid()) return;

//    if(!theLut.valid()) return;
//    if(theLut->getNumberOfBands() != 3) return;
   std::vector<ossimRefPtr<ossimNitfImageLut> > luts(destination->getNumberOfBands());

   if(bandInfo->getNumberOfLuts() == 1)
   {
      luts[0] = bandInfo->getLut(0);
      luts[1] = bandInfo->getLut(0);
      luts[2] = bandInfo->getLut(0);
   }
   else
   {
      luts[0] = bandInfo->getLut(0);
      luts[1] = bandInfo->getLut(1);
      luts[2] = bandInfo->getLut(2);
   }

   if(!luts[0].valid()||
      !luts[1].valid()||
      !luts[2].valid())
   {
      return;
   }
   ossim_uint8* tempRows[3];
   tempRows[0] = (ossim_uint8*)destination->getBuf(0);
   tempRows[1] = (ossim_uint8*)destination->getBuf(1);
   tempRows[2] = (ossim_uint8*)destination->getBuf(2);
   
   ossim_uint32 destWidth  = destination->getWidth();
   ossimNitfPackedBits bits(source,
                            compressionHeader->getImageCodeBitLength());
//    ossimNitfPackedBits bits(source,
//                             12); // vq is 12 bits

   ossim_uint32 compressionYidx   = 0;
   ossim_uint32 compressionXidx   = 0;
   ossim_uint32 compressionIdx    = 0;
   ossim_uint32 uncompressIdx     = 0;
   ossim_uint32 uncompressYidx    = 0;
   ossim_uint32 rows   = table.size();
   ossim_uint32 cols   = 0;
   ossim_uint32 rowIdx = 0;
   ossim_uint32 colIdx = 0;
   if(rows)
   {
      cols = table[0].theNumberOfValuesPerCompressionLookup;
   }
   ossim_uint32 compressionHeight = compressionHeader->getNumberOfImageRows();
   ossim_uint32 compressionWidth  = compressionHeader->getNumberOfImageCodesPerRow();
   ossim_uint8 lutValue = 0;
   ossim_uint8* data=0;
   
   for(compressionYidx = 0; compressionYidx < compressionHeight; ++compressionYidx)
   {
      uncompressYidx = compressionYidx*rows*destWidth;
      for(compressionXidx = 0; compressionXidx < compressionWidth; ++compressionXidx)
      {
         uncompressIdx = uncompressYidx + cols*compressionXidx;
         ossim_uint32 codeWord = bits.getValueAsUint32(compressionIdx);
         
         codeWord *= cols;
         for(rowIdx = 0; rowIdx < rows; ++rowIdx)
         {
            data = &table[rowIdx].theData[codeWord];
            for(colIdx = 0; colIdx < cols; ++colIdx)
            {
               lutValue = (*data)&0xff;
               tempRows[0][uncompressIdx+colIdx] = (*theLut)[lutValue][0];
               tempRows[1][uncompressIdx+colIdx] = (*theLut)[lutValue][1];
               tempRows[2][uncompressIdx+colIdx] = (*theLut)[lutValue][2];
               ++data;
            }
            uncompressIdx += destWidth;
         }
         ++compressionIdx;
      }
   }
}
