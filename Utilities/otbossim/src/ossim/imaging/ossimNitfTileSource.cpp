//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Contains class definition for ossimNitfTileSource.
// 
//*******************************************************************
//  $Id: ossimNitfTileSource.cpp 12988 2008-06-04 16:49:43Z gpotts $
#include <jerror.h>

#include <ossim/imaging/ossimNitfTileSource.h>

#include <ossim/base/ossimPackedBits.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfVqCompressionHeader.h>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimJpegMemSrc.h>
#include <ossim/imaging/ossimTiffTileSource.h>


#include <ossim/base/ossimContainerProperty.h>

RTTI_DEF1_INST(ossimNitfTileSource, "ossimNitfTileSource", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id: ossimNitfTileSource.cpp 12988 2008-06-04 16:49:43Z gpotts $";
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
                                   "READ_BIR",
                                   "READ_JPEG_BLOCK" };

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
      theTile(0),
      theCacheTile(0),
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
      theCompressedBuf(0),
      theDecimationFactor(1.0),
      theNitfBlockOffset(0),
      theNitfBlockSize(0)
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
      theNitfFile = 0;
   }

   theCacheTile = 0;
   theTile      = 0;

   if(theOverview)
   {
      delete theOverview;
      theOverview = 0;
   }
   if(theCompressedBuf)
   {
      delete [] theCompressedBuf;
      theCompressedBuf = 0;
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
      if( !hdr->isCompressed() )
      {
         theEntryList.push_back(i);
      }
      else if ( canUncompress(hdr) )
      {
         theEntryList.push_back(i);
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

   // Set the decimation factor.
   initializeDecimationFactor();

   // Initialize the sub image offset.
   if (initializeSubImageOffset() == false)
   {
      return false;
   }
   
   // Initialize the image rectangle. before the cache size is done
   if (initializeImageRect() == false)
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
   if (initializeBlockSize() == false)
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

   //---
   // Initialize the compressed buffer if needed.
   //---
   initializeCompressedBuf();

   // Make the tiles.  Note the blank tile is intentionally left blank.
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();
   theTile = idf->create(this, this);
   theTile->initialize();

   // Call base class complete open.
   completeOpen();

   return true;
}

bool ossimNitfTileSource::canUncompress(const ossimNitfImageHeader* hdr) const
{
   if (!hdr)
   {
      return false;
   }
   if (hdr->getCompressionCode() == "C3") // jpeg
   {
      return true;
   }
   else if(isVqCompressed(hdr->getCompressionCode())&&
           (hdr->getCompressionHeader().valid()))
   {
      // we will only support single band vq compressed NITFS
      // basically CIB and CADRG products are single band code words.
      //
      if(hdr->getNumberOfBands() == 1)
      {
         return true;
      }
   }
   return false;
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
   ossimString compressionCode = hdr->getCompressionCode();

   if ( (imode == "B") && (compressionCode == "C3") )
   {
      theReadMode = READ_JPEG_BLOCK; 
   }
   else if (numberOfBlocks > 1)
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
      if (imode == "B")
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
      else if (imode == "S")
      {
         theReadMode = READ_BSQ_BLOCK;
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
         theScalarType = OSSIM_UINT8;
         break;
      }
      case 11:
      {
         theScalarType = OSSIM_USHORT11;
         break;
      }
      case 12:
      {
         theScalarType = OSSIM_UINT16;
         break;
      }
      case 16:
      {
         if(pixelValueType == "SI")
         {
            theScalarType = OSSIM_SINT16;
         }
         else
         {
            theScalarType = OSSIM_UINT16;
         }
         break;
      }
      case 32:
      {
         if(pixelValueType == "R")
         {
            theScalarType = OSSIM_FLOAT32;
         }
         
         break;
      }
      case 64:
      {
         if(pixelValueType == "R")
         {
            theScalarType = OSSIM_FLOAT64;
         }
         
         break;
      }
      default:
      {
         if(hdr->isCompressed())
         {
            thePackedBitsFlag = true;
            if(bitsPerPixel < 8)
            {
               theScalarType = OSSIM_UINT8;
            }
            else if(bitsPerPixel < 16)
            {
               theScalarType = OSSIM_UINT16;
            }
            else if(bitsPerPixel < 32)
            {
               theScalarType = OSSIM_FLOAT32;
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
   if ( (theScalarType != OSSIM_UINT8) &&
        (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN) )
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

bool ossimNitfTileSource::initializeBlockSize()
{
   theBlockSizeInBytes     = 0;
   theReadBlockSizeInBytes = 0;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return false;
   }

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
      case READ_JPEG_BLOCK:
      {
         theBlockSizeInBytes    *= theNumberOfInputBands;
         if (scanForJpegBlockOffsets() == false)
         {
            return false;
         }
         break;
      }
      default:
      {
         return false;
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

   return true;
}

void ossimNitfTileSource::initializeDecimationFactor()
{
   theDecimationFactor = 1.0;
   
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   //---
   // Look for string like:
   // "/2" = 1/2
   // "/4  = 1/4
   // ...
   // "/16 = 1/16
   // If it is full resolution it should be "1.0"
   //---
   ossimString os = hdr->getImageMagnification();
   if ( os.contains("/") )
   {
      os = os.after("/");
      ossim_float64 d = os.toFloat64();
      if (d)
      {
         theDecimationFactor = 1.0 / d;
      }
   }
}

bool ossimNitfTileSource::initializeSubImageOffset()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      theSubImageOffset.makeNan();
      return false;
   }

   // Set to upper left corner (typically 0,0)
   theSubImageOffset = hdr->getImageRect().ul();

   //---
   // Test for the ichipb tag and set the sub image if needed.
   // 
   // NOTE:
   // 
   // There are nitf writers that set the ichipb offsets and only have IGEOLO
   // field present.  For these it has been determined (but still in question)
   // that we should not apply the sub image offset.
   //
   // See trac # 1578
   // http://trac.osgeo.org/ossim/ticket/1578
   //---
   
   const ossimRefPtr<ossimNitfRegisteredTag> tag =
      hdr->getTagData(ossimString("ICHIPB"));
   if (tag.valid())
   {
      ossimNitfIchipbTag* ichipb = PTR_CAST(ossimNitfIchipbTag, tag.get());
      if (ichipb)
      {
         const ossimRefPtr<ossimNitfRegisteredTag> blocka =
            hdr->getTagData(ossimString("BLOCKA"));
         const ossimRefPtr<ossimNitfRegisteredTag> rpc00a =
            hdr->getTagData(ossimString("RPC00A"));              
         const ossimRefPtr<ossimNitfRegisteredTag> rpc00b =
            hdr->getTagData(ossimString("RPC00B"));

         //---
         // If any of these tags are present we will use the sub image from
         // the ichipb tag.
         //---
         if ( blocka.get() || rpc00a.get() || rpc00b.get() )
         {
            ossimDpt pt;
            ichipb->getSubImageOffset(pt);
            theSubImageOffset = pt;
         }
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeSubImageOffset DEBUG:"
         << "\nSub image offset:  " << theSubImageOffset
         << endl;
   }
   return true;
}

bool ossimNitfTileSource::initializeImageRect()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      theImageRect.makeNan();
      return false;
   }

   bool initialized = false;

   const ossimRefPtr<ossimNitfRegisteredTag> tag =
      hdr->getTagData(ossimString("ICHIPB"));
   if (tag.valid())
   {
      ossimNitfIchipbTag* ichipb = PTR_CAST(ossimNitfIchipbTag, tag.get());
      if (ichipb)
      {
         // This has the sub image offset applied...
         ossimDrect rect;
         ichipb->getFullImageRect(rect);
         
         theBlockImageRect = rect;
         theImageRect      = rect;
         initialized       = true;
      }
   }

   if (!initialized)
   {
      theBlockImageRect = hdr->getBlockImageRect();
      theImageRect      = hdr->getImageRect();
      
      theBlockImageRect += theSubImageOffset;
      theImageRect      += theSubImageOffset;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeImageRect DEBUG:"
         << "\nImage Rect:        " << theImageRect
         << "\nBlock rect:        " << theBlockImageRect
         << endl;
   }
   return true;
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
      case READ_JPEG_BLOCK:
         theCacheSize.x = hdr->getNumberOfPixelsPerBlockHoriz();
         theCacheSize.y = hdr->getNumberOfPixelsPerBlockVert();
         break;

      case READ_BIB:
      case READ_BIP:
      case READ_BIR:
         theCacheSize.x = hdr->getNumberOfPixelsPerBlockHoriz();
         theCacheSize.y = hdr->getNumberOfPixelsPerBlockVert();
//          theCacheSize.x = getNumberOfSamples(0);
//          theCacheSize.y = getTileHeight();
//          if(theCacheSize.y > hdr->getNumberOfPixelsPerBlockVert())
//          {
//             theCacheSize.y = hdr->getNumberOfPixelsPerBlockVert();
//          }
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
      case READ_JPEG_BLOCK:
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
   theCacheTile = ossimImageDataFactory::instance()->create(
      this,
      theScalarType,
      theNumberOfOutputBands,
      theCacheSize.x,
      theCacheSize.y);

   theCacheTile->initialize();
}

void ossimNitfTileSource::initializeCompressedBuf()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return;
   }

   if(theCompressedBuf)
   {
      delete [] theCompressedBuf;
      theCompressedBuf = 0;
   }

   if( (hdr->getRepresentation().upcase().contains("LUT")) ||
       ( isVqCompressed(hdr->getCompressionCode()) ) )
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
         
            if(getOutputScalarType() == OSSIM_USHORT11)
            {
               //---
               // Temp fix:
               // The overview handler could return a tile of OSSIM_UINT16 if
               // the max sample value was not set to 2047.  
               //---
               tileData->setScalarType(OSSIM_USHORT11);
            }
         }
         
         return tileData;
      }
   }

   if (!theTile.valid())
   {
      return theTile;
   }
   
   theTile->setImageRectangle(tileRect);
   
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
         theTile->makeBlank();
      }
            
      // See if the requested clip rect is already in the cache tile.
      if ( (clipRect.completely_within(theCacheTile->getImageRectangle()))&&
           (theCacheTile->getDataObjectStatus() != OSSIM_EMPTY)&&
           (theCacheTile->getBuf()))
      {
         theTile->loadTile(theCacheTile->getBuf(),
                           theCacheTile->getImageRectangle(),
                           theCacheTileInterLeaveType);
         //---
         // Validate the tile.  This will set the status to full, partial
         // or empty.  Must be performed if any type of combining is to be
         // performed down the chain.
         //---
         theTile->validate();
      }
      else
      {
         if (loadTile(clipRect) == true)
         {
            //---
            // Validate the tile.  This will set the status to full, partial
            // or empty.  Must be performed if any type of combining is to be
            // performed down the chain.
            //---
            theTile->validate();
         }
         else
         {
            theTile->makeBlank(); // loadTile failed...
         }
      }
   } // End of if ( tileRect.intersects(image_rect) )
   else
   {
      // No part of requested tile within the image rectangle.
      theTile->makeBlank();
   }

   return theTile;   
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
      ossimRefPtr<ossimImageData> tempTile = 0;
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
         std::streamoff p;
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
            std::streamoff p;
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
      case READ_JPEG_BLOCK:
      {
         if (uncompressJpegBlock(x, y) == false)
         {
            theFileStr.clear();
            ossimNotify(ossimNotifyLevel_FATAL)
               << "ossimNitfTileSource::loadBlock Read Error!"
               << "\nReturning error..." << endl;
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            return false;
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
      case OSSIM_UINT8:
      {
         
         ossim_uint8* outputBuf = (ossim_uint8*)tempBuf;
         for(bandIdx = 0; bandIdx < bandCount; ++bandIdx)
         {
            ossimPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
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
      case OSSIM_UINT16:
      {
         
         ossim_uint16* outputBuf = (ossim_uint16*)tempBuf;
         for(bandIdx = 0; bandIdx < bandCount; ++bandIdx)
         {
            ossimPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
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
            ossimPackedBits packedBits((ossim_uint8*)packedBuffer->getBuf(bandIdx),
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
                  case OSSIM_UINT8:
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
                  case OSSIM_UINT16:
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
                  case OSSIM_SINT16:
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
      else
      {
         ossim_int32 bitsPerPixel = hdr->getActualBitsPerPixelPerBand();
         switch (bitsPerPixel)
         {
            case 11:
            {
               if (result > 2047.0)
               {
                  result = 2047.0;
               }
               break;
            }
            case 12:
            {
               if (result > 4095.0)
               {
                  result = 4095.0;
               }
               break;
            }
            default:
               break;
         }
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


bool ossimNitfTileSource::getPosition(std::streamoff& streamPosition,
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
      case READ_JPEG_BLOCK:
      {
         streamPosition += blockNumber * theReadBlockSizeInBytes;
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
      case READ_JPEG_BLOCK:
        blockSizeInBytes = theReadBlockSizeInBytes;
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
   ossim::defaultTileSize(tileSize);
   return static_cast<ossim_uint32>(tileSize.x);
}

ossim_uint32 ossimNitfTileSource::getTileHeight() const
{
   if (theTile.valid())
   {
      return theTile->getHeight();
   }

   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
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
      case READ_JPEG_BLOCK:
      {
         blockNumber = ((blockY*hdr->getNumberOfBlocksPerRow()) +
                        blockX);
         break;
      }
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

void ossimNitfTileSource::getDecimationFactor(ossim_uint32 resLevel,
                                              ossimDpt& result) const
{
   if (resLevel == 0)
   {
      result.x = theDecimationFactor;
      result.y = theDecimationFactor;
   }
   else
   {
      result.x = theDecimationFactor /
         pow( static_cast<ossim_float64>(2.0),
              static_cast<ossim_float64>(resLevel) );
      result.y = result.x;
   }
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
   else 
   {
      if(theNitfFile)
      {
         if(theNitfFile->getHeader().valid())
         {
            ossimRefPtr<ossimProperty> p = theNitfFile->getHeader()->getProperty(name);
				if(p.valid())
				{
					return p;
				}
         }
      }
		const ossimNitfImageHeader* imageHeader = getCurrentImageHeader();
		if(imageHeader)
      {
         ossimRefPtr<ossimProperty> p = imageHeader->getProperty(name);
			if(p.valid())
			{
				return p;
			}
		}
																		
      return 0;
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
   ossimImageHandler::getPropertyNames(propertyNames);
   propertyNames.push_back(ossimKeywordNames::ENABLE_CACHE_KW);
	if(theNitfFile->getHeader().valid())
	{
		theNitfFile->getHeader()->getPropertyNames(propertyNames);
	}
	const ossimNitfImageHeader* imageHeader = getCurrentImageHeader();
	if(imageHeader)
	{
		imageHeader->getPropertyNames(propertyNames);
	}

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
      (destination->getScalarType()!=OSSIM_UINT8)||
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
      (destination->getScalarType()!=OSSIM_UINT8))
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
   ossimPackedBits bits(source, compressionHeader->getImageCodeBitLength());
//    ossimPackedBits bits(source,
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

bool ossimNitfTileSource::scanForJpegBlockOffsets()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();

   if ( !hdr || (theReadMode != READ_JPEG_BLOCK) || !theFileStr )
   {
      return false;
   }

   theNitfBlockOffset.clear();
   theNitfBlockSize.clear();

   //---
   // NOTE:
   // SOI = 0xffd8 Start of image
   // EOI = 0xffd9 End of image
   // DHT = 0xffc4 Define Huffman Table(s)
   // DQT = 0xffdb Define Quantization Table(s)
   //---
   char c;

   // Seek to the first block.
   theFileStr.seekg(hdr->getDataLocation(), ios::beg);
   if (theFileStr.fail())
   {
      return false;
   }
   
   // Read the first two bytes and verify it is SOI; if not, get out.
   theFileStr.get( c );
   if (static_cast<ossim_uint8>(c) != 0xff)
   {
      return false;
   }
   theFileStr.get(c);
   if (static_cast<ossim_uint8>(c) != 0xd8)
   {
      return false;
   }

   ossim_uint32 blockSize = 2;  // Read two bytes...

   // Add the first offset.
   theNitfBlockOffset.push_back(hdr->getDataLocation());

   // Find all the SOI markers.
   while ( theFileStr.get(c) ) 
   {
      ++blockSize;
      if (static_cast<ossim_uint8>(c) == 0xff)
      {
         if ( theFileStr.get(c) )
         {
            ++blockSize;

            if (static_cast<ossim_uint8>(c) == 0xd8) // At SOI marker...
            {
               std::streamoff pos = theFileStr.tellg();
               theNitfBlockOffset.push_back(pos-2);
            }
            else if (static_cast<ossim_uint8>(c) == 0xd9) // At EOI marker...
            {
               theNitfBlockSize.push_back(blockSize);
               blockSize = 0;
            }
         }
      }
   }

   theFileStr.seekg(0, ios::beg);
   theFileStr.clear();

   // We should have the same amount of offsets as we do blocks...
   ossim_uint32 total_blocks =
      hdr->getNumberOfBlocksPerRow()*hdr->getNumberOfBlocksPerCol();
   
   if (theNitfBlockOffset.size() != total_blocks)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG:"
            << "\nBlock offset count wrong!"
            << "\nblocks:  " << total_blocks
            << "\noffsets:  " << theNitfBlockOffset.size()
            << std::endl;
      }
      
      return false;
   }
   if (theNitfBlockSize.size() != total_blocks)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG:"
            << "\nBlock size count wrong!"
            << "\nblocks:  " << total_blocks
            << "\nblock size array:  " << theNitfBlockSize.size()
            << std::endl;
      }

      return false;
   }

   return true;
}

bool ossimNitfTileSource::uncompressJpegBlock(ossim_uint32 x, ossim_uint32 y)
{
   ossim_uint32 blockNumber = getBlockNumber(ossimIpt(x,y));

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::uncompressJpegBlock DEBUG:"
         << "\nblockNumber:  " << blockNumber
         << "\noffset to block: " << theNitfBlockOffset[blockNumber]
         << "\nblock size: " << theNitfBlockSize[blockNumber]
         << std::endl;
   }
   
   // Seek to the block.
   theFileStr.seekg(theNitfBlockOffset[blockNumber], ios::beg);
   
   // Read the block into memory.
   ossim_uint8* compressedBuf = new ossim_uint8[theNitfBlockSize[blockNumber]];
   
   if (!theFileStr.read((char*)compressedBuf, theNitfBlockSize[blockNumber]))
   {
      theFileStr.clear();
      ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimNitfTileSource::loadBlock Read Error!"
         << "\nReturning error..." << endl;
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      delete [] compressedBuf;
      compressedBuf = 0;
      return false;
   }

   //---
   // Most of comments below from jpeg-6b "example.c" file.
   //---
   
   /* This struct contains the JPEG decompression parameters and pointers
    * to working space (which is allocated as needed by the JPEG library).
    */
   jpeg_decompress_struct cinfo;

   /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
   ossimJpegErrorMgr jerr;

   /* Step 1: allocate and initialize JPEG decompression object */
   
   /* We set up the normal JPEG error routines, then override error_exit. */
   cinfo.err = jpeg_std_error(&jerr.pub);
   jerr.pub.error_exit = ossimJpegErrorExit;

   /* Establish the setjmp return context for my_error_exit to use. */
   if (setjmp(jerr.setjmp_buffer))
   {
      /* If we get here, the JPEG code has signaled an error.
       * We need to clean up the JPEG object, close the input file, and return.
       */
     jpeg_destroy_decompress(&cinfo);
     delete [] compressedBuf;
     compressedBuf = 0;
     return false;
   }

   /* Now we can initialize the JPEG decompression object. */
   jpeg_create_decompress(&cinfo);

   //---
   // Step 2: specify data source.  In this case we will uncompress from
   // memory so we will use "ossimJpegMemorySrc" in place of " jpeg_stdio_src".
   //---
   ossimJpegMemorySrc (&cinfo,
                       compressedBuf,
                       static_cast<size_t>(theReadBlockSizeInBytes));

   /* Step 3: read file parameters with jpeg_read_header() */
   jpeg_read_header(&cinfo, TRUE);

   // Check for Quantization tables.
   if (cinfo.quant_tbl_ptrs[0] == NULL)
   {
      // This will load table specified in COMRAT field.
      if (loadJpegQuantizationTables(cinfo) == false)
      {
         jpeg_destroy_decompress(&cinfo);
         delete [] compressedBuf;
         compressedBuf = 0;
         return false;
      }
   }

   // Check for huffman tables.
   if (cinfo.ac_huff_tbl_ptrs[0] == NULL)
   {
      // This will load default huffman tables into .
      if (loadJpegHuffmanTables(cinfo) == false)
      {
         jpeg_destroy_decompress(&cinfo);
         delete [] compressedBuf;
         compressedBuf = 0;
         return false;
      }
   }

   /* Step 4: set parameters for decompression */
   
   /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

   /* Step 5: Start decompressor */
   jpeg_start_decompress(&cinfo);
   
   /* JSAMPLEs per row in output buffer */
   int row_stride = cinfo.output_width * cinfo.output_components;
   
   // Get pointers to the cache tile buffers.
   std::vector<ossim_uint8*> destinationBuffer(theNumberOfInputBands);
   ossim_uint32 band = 0;
   for (band = 0; band < theNumberOfInputBands; ++band)
   {
      destinationBuffer[band] = theCacheTile->getUcharBuf(band);
   }
   
   // Make a temp line buffer
   ossim_uint8* lineBuffer = new ossim_uint8[row_stride];
   JSAMPROW jbuf[1];
   jbuf[0] = (JSAMPROW) lineBuffer;

   ossim_uint32 linesToRead =
      min(static_cast<ossim_uint32>(theCacheSize.y),
          (getNumberOfLines()-y) );
                                 
   while (cinfo.output_scanline < linesToRead)
   {
      // Read a line from the jpeg file.
      jpeg_read_scanlines(&cinfo, jbuf, 1);
      
      //---
      // Copy the line which if band interleaved by pixel the the band
      // separate buffers.
      //---
      ossim_int32 index = 0;
      for (ossim_int32 sample = 0; sample < row_stride; ++sample)
      {
         for (band = 0; band < theNumberOfInputBands; ++band)
         {
            destinationBuffer[band][sample] = lineBuffer[index];
            ++index;
         }
      }
      
      for (band = 0; band < theNumberOfInputBands; ++band)
      {
         destinationBuffer[band] += row_stride;
      }
   }

   // clean up...
   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);

   delete [] compressedBuf;
   compressedBuf = 0;
   
   // Delete the line buffer.
   delete [] lineBuffer;
   lineBuffer = 0;
   return true;
}

//---
// Default JPEG quantization tables
// Values from: MIL-STD-188-198, APPENDIX A
//---
const static int Q1table[64] = 
{
   8,    72,  72,  72,  72,  72,  72,  72, // 0 - 7
   72,   72,  78,  74,  76,  74,  78,  89, // 8 - 15
   81,   84,  84,  81,  89, 106,  93,  94, // 16 - 23
   99,   94,  93, 106, 129, 111, 108, 116, // 24 - 31
   116, 108, 111, 129, 135, 128, 136, 145, // 32 - 39
   136, 128, 135, 155, 160, 177, 177, 160, // 40 - 47
   155, 193, 213, 228, 213, 193, 255, 255, // 48 - 55
   255, 255, 255, 255, 255, 255, 255, 255  // 56 - 63
};

const static int Q2table[64] = 
{ 
   8,   36,  36,  36,  36,  36,  36,  36, // 0 - 7
   36,   36,  39,  37,  38,  37,  39,  45, // 8 - 15
   41,   42,  42,  41,  45,  53,  47,  47, // 16 -23
   50,   47,  47,  53,  65,  56,  54,  59, // 24 - 31
   59,   54,  56,  65,  68,  64,  69,  73, // 32 - 39
   69,   64,  68,  78,  81,  89,  89,  81, // 40 - 47
   78,   98, 108, 115, 108,  98, 130, 144, // 48 - 55
   144, 130, 178, 190, 178, 243, 243, 255  // 56 - 63
};

const static int Q3table[64] = 
{ 
   8,  10, 10, 10, 10, 10, 10, 10,  // 0 - 7
   10, 10, 11, 10, 11, 10, 11, 13,  // 8 - 15
   11, 12, 12, 11, 13, 15, 13, 13,  // 16 - 23
   14, 13, 13, 15, 18, 16, 15, 16,  // 24 - 31
   16, 15, 16, 18, 19, 18, 19, 21,  // 32 - 39
   19, 18, 19, 22, 23, 25, 25, 23,  // 40 - 47
   22, 27, 30, 32, 30, 27, 36, 40,  // 48 - 56
   40, 36, 50, 53, 50, 68, 68, 91   // 57 - 63
};

const static int Q4table[64] = 
{
   8,   7,  7,  7,  7,  7,  7,  7, // 0 - 7
   7,   7,  8,  7,  8,  7,  8,  9, // 8 - 15
   8,   8,  8,  8,  9, 11,  9,  9, // 16 - 23
   10,  9,  9, 11, 13, 11, 11, 12, // 24 - 31
   12, 11, 11, 13, 14, 13, 14, 15, // 32 - 39
   14, 13, 14, 16, 16, 18, 18, 16, // 40 - 47
   16, 20, 22, 23, 22, 20, 26, 29, // 48 - 55
   29, 26, 36, 38, 36, 49, 49, 65  // 56 - 63
};

const static int Q5table[64] = 
{
   4,   4,  4,  4,  4,  4,  4,  4, // 0 - 7
   4,   4,  4,  4,  4,  4,  4,  5, // 8 - 15
   5,   5,  5,  5,  5,  6,  5,  5, // 16 - 23
   6,   5,  5,  6,  7,  6,  6,  6, // 24 - 31
   6,   6,  6,  7,  8,  7,  8,  8, // 32 - 39
   8,   7,  8,  9,  9, 10, 10,  9, // 40 - 47
   9,  11, 12, 13, 12, 11, 14, 16, // 48 - 55
   16, 14, 20, 21, 20, 27, 27, 36  // 56 - 63
};

const static int* QTABLE_ARRAY[5]={Q1table,Q2table,Q3table,Q4table,Q5table};

bool ossimNitfTileSource::loadJpegQuantizationTables(
   jpeg_decompress_struct& cinfo) const
{
   //---
   // Check to see if table is present.  We will only look at the first table
   // in the array of arrays.
   // 
   // NOTE:  There are four tables in the array "cinfo.quant_tbl_ptrs".  It
   // looks like the standard is to use the first table. (not sure though)
   //---
   if (cinfo.quant_tbl_ptrs[0] != NULL)
   {
      return false;
   }

   // Get the COMRAT (compression rate code) from the header:
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      return false;
   }
   
   ossimString comrat = hdr->getCompressionRateCode();
   ossim_uint32 tableIndex = 0;
   if (comrat.size() >= 4)
   {
      // COMRAT string like: "00.2" = use table 2. (between 1 and 5).
      ossimString s;
      s.push_back(comrat[static_cast<std::string::size_type>(3)]);
      ossim_int32 comTbl = s.toInt32();
      if ( (comTbl > 0) && (comTbl < 6) )
      {
         tableIndex = comTbl-1;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimNitfTileSource::loadJpegQuantizationTables WARNING\n"
            << "\nNo quantization tables specified!"
            << endl;
         return false;  
      }
   }

   cinfo.quant_tbl_ptrs[0] = jpeg_alloc_quant_table((j_common_ptr) &cinfo);
   
   JQUANT_TBL* quant_ptr = cinfo.quant_tbl_ptrs[0]; // quant_ptr is JQUANT_TBL*

   for (ossim_int32 i = 0; i < 64; ++i)
   {
      /* Qtable[] is desired quantization table, in natural array order */
      quant_ptr->quantval[i] = QTABLE_ARRAY[tableIndex][i];
   }
   return true;
}

//---
// Default JPEG Huffman tables
// Values from: MIL-STD-188-198, APPENDIX B
//---
static const int AC_BITS[16] = 
{
   0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125
};

static const int DC_BITS[16] = 
{
   0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
};

static const int AC_HUFFVAL[256] =
{
   0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,          
   0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
   0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08,
   0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
   0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16,
   0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
   0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
   0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
   0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
   0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
   0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
   0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
   0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
   0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
   0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6,
   0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
   0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4,
   0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
   0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA,
   0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
   0xF9, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const int DC_HUFFVAL[256] =
{
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
   0x08, 0x09, 0x0A, 0x0B, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

bool ossimNitfTileSource::loadJpegHuffmanTables(
   jpeg_decompress_struct& cinfo) const
{
   if ( (cinfo.ac_huff_tbl_ptrs[0] != NULL) &&
        (cinfo.dc_huff_tbl_ptrs[0] != NULL) )
   {
      return false;
   }

   cinfo.ac_huff_tbl_ptrs[0] = jpeg_alloc_huff_table((j_common_ptr)&cinfo);
   cinfo.dc_huff_tbl_ptrs[0] = jpeg_alloc_huff_table((j_common_ptr)&cinfo);
   
   ossim_int32 i;
   JHUFF_TBL* huff_ptr;
   
   // Copy the ac tables.
   huff_ptr = cinfo.ac_huff_tbl_ptrs[0]; /* huff_ptr is JHUFF_TBL* */     
   for (i = 0; i < 16; ++i) 
   {
      // huff_ptr->bits is array of 17 bits[0] is unused; hence, the i+1
      huff_ptr->bits[i+1] = AC_BITS[i]; 
   }
   
   for (i = 0; i < 256; ++i)
   {
      huff_ptr->huffval[i] = AC_HUFFVAL[i];
   }
   
   // Copy the dc tables.
   huff_ptr = cinfo.dc_huff_tbl_ptrs[0]; /* huff_ptr is JHUFF_TBL* */
   for (i = 0; i < 16; ++i)
   {
      // huff_ptr->bits is array of 17 bits[0] is unused; hence, the i+1
      huff_ptr->bits[i+1] = DC_BITS[i];
   }
   
   for (i = 0; i < 256; i++)
   {
      /* symbols[] is the list of Huffman symbols, in code-length order */
      huff_ptr->huffval[i] = DC_HUFFVAL[i];
   }
   return true;
}
