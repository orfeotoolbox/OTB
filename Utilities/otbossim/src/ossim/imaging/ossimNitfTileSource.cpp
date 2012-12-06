//*******************************************************************
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
//  $Id: ossimNitfTileSource.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimNitfTileSource.h>
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
#include <ossim/base/ossimPackedBits.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimJpegMemSrc.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimJpegDefaultTable.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/support_data/ossimNitfVqCompressionHeader.h>


#if defined(JPEG_DUAL_MODE_8_12)
#undef JPEG_DUAL_MODE_8_12 
#endif

#if defined(JPEG_DUAL_MODE_8_12)
#include <ossim/imaging/ossimNitfTileSource_12.h>
#endif

#include <jerror.h>
#include <fstream>
#include <algorithm> /* for std::fill */

RTTI_DEF1_INST(ossimNitfTileSource, "ossimNitfTileSource", ossimImageHandler)

#ifdef OSSIM_ID_ENABLED
   static const char OSSIM_ID[] = "$Id: ossimNitfTileSource.cpp 21631 2012-09-06 18:10:55Z dburken $";
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
      theNitfBlockOffset(0),
      theNitfBlockSize(0),
      m_isJpeg12Bit(false),
      m_jpegOffsetsDirty(false)
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
   theNitfImageHeader.clear();

   if(theFileStr.is_open())
   {
      theFileStr.close();
   }

   theCacheTile = 0;
   theTile      = 0;
   theOverview  = 0;
 }

bool ossimNitfTileSource::isOpen()const
{
   return (theNitfImageHeader.size() > 0);
}

bool ossimNitfTileSource::open()
{
   bool result = false;
   
   if(isOpen())
   {
      close();
   }
   
   theErrorStatus = ossimErrorCodes::OSSIM_OK;

   if ( parseFile() )
   {
      result = allocate();
   }
   if (result)
   {
      completeOpen();
   }
   
   return result;
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
      ossimRefPtr<ossimNitfImageHeader> hdr = theNitfFile->getNewImageHeader(i);
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
         if(hdr.valid())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << "DEBUG:"
               << "\nImage header[" << i << "]:\n" << *hdr
               << endl;
         }
      }

      if( !hdr->isCompressed() )
      {
         // Skip cloud mask images??? (drb)
         if (hdr->getRepresentation() != "NODISPLY")
         {
            theEntryList.push_back(i);
            theNitfImageHeader.push_back(hdr);
         }
         else 
         {
            ossimString cat = hdr->getCategory().trim().downcase();
            // this is an NGA Highr Resoluion Digital Terrain Model NITF format
            if(cat == "dtem")
            {
               theEntryList.push_back(i);
               theNitfImageHeader.push_back(hdr);
            }
         }

      }
      else if ( canUncompress(hdr.get()) )
      {
         theEntryList.push_back(i);
         theCacheEnabledFlag = true;
         theNitfImageHeader.push_back(hdr);

         if (hdr->getBitsPerPixelPerBand() == 8)
         {
            m_isJpeg12Bit = false;
         }
         else if (hdr->getBitsPerPixelPerBand() == 12)
         {
           m_isJpeg12Bit = true;
         }
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
         return false;
      }
   }

   if(theEntryList.size()<1)
   {
      return false;
   }
   
   //### WHY IS THIS HERE? THIS CAUSED A BUG BECAUSE theCurrentEntry was previously initialized 
   //### in loadState() according to a KWL. Any entry index in the KWL was being ignored.
   //if(theEntryList.size()>0)
   //{
   //   theCurrentEntry = theEntryList[0];
   //}

   theNumberOfImages = (ossim_uint32)theNitfImageHeader.size();
   
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
   
   // Initialize the lut to the current entry if the current entry has a lut.
   initializeLut();
   
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

   // Clear buffers:
   theTile = 0;
   theCacheTile = 0;
   theCompressedBuf.clear();

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

   return true;
}

bool ossimNitfTileSource::allocateBuffers()
{
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

   //---
   // Make the output tile.
   //---
   initializeOutputTile();

   return true;
}

bool ossimNitfTileSource::canUncompress(const ossimNitfImageHeader* hdr) const
{

   bool result = false;
   if (hdr)
   {
      ossimString code = hdr->getCompressionCode();

      if (code == "C3") // jpeg
      {
         if (hdr->getBitsPerPixelPerBand() == 8)
         {
            result = true;
         }
         else if (hdr->getBitsPerPixelPerBand() == 12)
         {
           result = true;
         }
         else
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Entry with jpeg compression (C3) has an unsupported "
                  << "JPEG data precision: " << hdr->getBitsPerPixelPerBand()
                  << std::endl;
            }
         }
      }
      else if(isVqCompressed( code ) &&
              (hdr->getCompressionHeader().valid()) )
      {
         // we will only support single band vq compressed NITFS
         // basically CIB and CADRG products are single band code words.
         //
         if(hdr->getNumberOfBands() == 1)
         {
            result = true;
         }
      }
   }
   return result;
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

   if ( (compressionCode == "C3") && ((imode == "B")||(imode == "P")) )
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
         if(pixelValueType == "SI")
         {
            theScalarType = OSSIM_SINT32;
         }
         else if(pixelValueType == "R")
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
         else
         {
            if(bitsPerPixel<8)
            {
               theScalarType = OSSIM_UINT8;
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
      if(hdr->getRepresentation().contains("LUT")&&(theNumberOfInputBands == 1))
      {
         theNumberOfOutputBands = 3;
      }
   }
   else 
   {
      ossimRefPtr<ossimNitfImageBand> bandInfo = hdr->getBandInformation(0);
      if ( bandInfo.valid() )
      {
         theNumberOfInputBands = 1;
         theNumberOfOutputBands = bandInfo->getNumberOfLuts();
      }
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
         theBlockSizeInBytes *= theNumberOfInputBands;
         ossimString code = hdr->getCompressionCode();
         if (code == "C3") // jpeg
         {
            m_jpegOffsetsDirty  = true;
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

//*************************************************************************************************
// Virtual method determines the decimation factors at each resolution level. 
// This implementation derives the R0 decimation from the image metadata if available, then hands
// off the computation of remaining R-levels to the base class implementation.
//*************************************************************************************************
void ossimNitfTileSource::establishDecimationFactors()
{
   theDecimationFactors.clear();
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (hdr)
   {
      double decimation;
      hdr->getDecimationFactor(decimation);
      if ((decimation != 0.0) && !ossim::isnan(decimation))
      {
         //---
         // Note: Commented out as other code is picking up the resolution and then we're applying
         // a decimation on top of that. (drb Aug. 2011)
         // ossimDpt dec_2d (decimation, decimation);
         //---
         ossimDpt dec_2d (1.0, 1.0);
         theDecimationFactors.push_back(dec_2d);
      }
   }

   // Just needed to set the first R level here, the base class can do the rest:
   ossimImageHandler::establishDecimationFactors();
}

#if 0
ossimImageGeometry* ossimNitfTileSource::getImageGeometry()
{
   //---
   // Call base class getImageGeometry which will check for external geometry
   // or an already set geometry.
   //---
   ossimImageGeometry* result = ossimImageHandler::getImageGeometry();

   if (result)
   {
      if ( !result->getTransform() )
      {
         ossimRefPtr<ossim2dTo2dTransform> transform = 0;
         
         const ossimNitfImageHeader* hdr = getCurrentImageHeader();
         if (hdr)
         {
            //---
            // Test for the ichipb tag and set the sub image if needed.
            // 
            // NOTE # 1:
            // 
            // There are nitf writers that set the ichipb offsets and only have
            // IGEOLO field present.  For these it has been determined
            // (but still in question) that we should not apply the sub image
            // offset.
            //
            // See trac # 1578
            // http://trac.osgeo.org/ossim/ticket/1578
            //
            // NOTE # 2:
            //
            // Let the ICHIPB have precedence over the STDIDC tag as we could
            // have a chip of a segment.
            //---
            ossimRefPtr<ossimNitfRegisteredTag> tag =
               hdr->getTagData(ossimString("ICHIPB"));
            if (tag.valid())
            {
               ossimNitfIchipbTag* ichipb =
                  PTR_CAST(ossimNitfIchipbTag, tag.get());
               if (ichipb)
               {
//                  const ossimRefPtr<ossimNitfRegisteredTag> blocka =
//                     hdr->getTagData(ossimString("BLOCKA"));
//                  const ossimRefPtr<ossimNitfRegisteredTag> rpc00a =
//                     hdr->getTagData(ossimString("RPC00A"));              
//                  const ossimRefPtr<ossimNitfRegisteredTag> rpc00b =
//                     hdr->getTagData(ossimString("RPC00B"));
                  
                  //---
                  // If any of these tags are present we will use the sub
                  // image from the ichipb tag.
                  //---
//                  if ( blocka.get() || rpc00a.get() || rpc00b.get() )
                  // ************************* THERE ARE PROBLEMS NOT SETTING THIS AT SITE.  GO AHEAD AND ALWAYS INIT THE SHIFT
                  {
                     transform = ichipb->newTransform();
                  }
               }
            }
   
            if ( !transform)
            {
               //---
               // Look for the STDIDC tag for a sub image (segment) offset.
               //
               // See: STDI-002 Table 7.3 for documentation.
               //---
               tag = hdr->getTagData(ossimString("STDIDC"));
               if (tag.valid() && (hdr->getIMode() == "B") )
               {
                  ossimDpt shift;
                  ossimNitfStdidcTag* stdidc =
                     PTR_CAST(ossimNitfStdidcTag, tag.get());
                  if (stdidc)
                  {
                     ossim_int32 startCol = stdidc->getStartColumn().toInt32();
                     ossim_int32 startRow = stdidc->getStartRow().toInt32();
                     if ( (startCol > 0) && (startRow > 0) )
                     {
                        
                        // field are one based; hence, the - 1.
                        shift.x = (startCol-1) *
                           hdr->getNumberOfPixelsPerBlockHoriz();
                        shift.y = (startRow-1) *
                           hdr->getNumberOfPixelsPerBlockVert();
                     }
                     if(shift.x > 0 ||
                        shift.y > 0)
                     {
                        transform = new ossim2dTo2dShiftTransform(shift);
                     }
                  }
               }
            }
            
         } // matches: if (hdr)

         if ( transform.valid() )
         {
            result->setTransform( transform.get() );
         }
         //else
         //{
         //   ossimImageGeometryRegistry::instance()->createTransform(this);
         //}
         
         
      } // matches: if ( !result->getTransform() )

      if ( !result->getProjection() )
      {
         ossimRefPtr<ossimProjection> proj =
            ossimProjectionFactoryRegistry::instance()->
               createProjection(this);
         if ( proj.valid() )
         {
            result->setProjection( proj.get() );
         }
         //else
         //{
         //   ossimImageGeometryRegistry::instance()->createProjection(this);
         //}
         
      }
      
   } // matches: if (result)

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::createImageGeometry DEBUG:\n";

      if (result)
      {
         result->print(ossimNotify(ossimNotifyLevel_DEBUG)) << "\n";
      }
   }

   return result;
}
#endif

bool ossimNitfTileSource::initializeImageRect()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr)
   {
      theImageRect.makeNan();
      return false;
   }
   
   theBlockImageRect = hdr->getBlockImageRect();
   theImageRect      = hdr->getImageRect();

   if (traceDebug())
   {
      ossimIpt iloc;
      hdr->getImageLocation(iloc); // for temp debug (drb)
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::initializeImageRect DEBUG:"
         << "\noffset from ILOC field:  " << iloc
         << "\nImage Rect:              " << theImageRect
         << "\nBlock rect:              " << theBlockImageRect
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
      case READ_JPEG_BLOCK:  
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

   if( (hdr->getRepresentation().upcase().contains("LUT")) ||
       ( isVqCompressed(hdr->getCompressionCode()) ) )
   {
      theCompressedBuf.resize(theReadBlockSizeInBytes);
      std::fill(theCompressedBuf.begin(), theCompressedBuf.end(), '\0');
   }
}

void ossimNitfTileSource::initializeOutputTile()
{
   //---
   // Make the output tile.  This implementation will use default tile size.
   ossimImageDataFactory* idf = ossimImageDataFactory::instance();
   theTile = idf->create(this, this);
   theTile->initialize();
}

void ossimNitfTileSource::initializeLut()
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (hdr)
   {
      if ( hdr->hasLut() )
      {
         //---
         // NOTE: Only band 0 ??? (drb)
         //---
         theLut = theNitfImageHeader[theCurrentEntry]->createLut(0);
      }
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

   if ( !theTile.valid() )
   {
      // First call to getTile:
      allocateBuffers();
      if ( !theTile.valid() )
      {
         return theTile;
      }
   }

   // Rectangle must be set prior to getOverviewTile call.
   theTile->setImageRectangle(tileRect);

   if (resLevel)
   {
      if ( getOverviewTile(resLevel, theTile.get() ) )
      {
         return theTile;
      }
   }
   
   ossim_uint32 level = resLevel;
   if (theStartingResLevel)  // Used as overview.
   {
      if (theStartingResLevel <= resLevel)
      {
         //---
         // Adjust the level to be relative to the reader using this as
         // overview.
         //---
         level -= theStartingResLevel; 
      }
   }

   //---
   // See if the whole tile is going to be filled, if not, start out with
   // a blank tile so data from a previous load gets wiped out.
   //---
   if ( !tileRect.completely_within(theImageRect) )
   {
      // Start with a blank tile.
      theTile->makeBlank();
   }

   //---
   // See if any point of the requested tile is in the image.
   //---
   if ( tileRect.intersects(theBlockImageRect) )
   {
      ossimIrect clipRect = tileRect.clipToRect(theImageRect);
            
      // See if the requested clip rect is already in the cache tile.
      if ( (clipRect.completely_within(theCacheTile->getImageRectangle()))&&
           (theCacheTile->getDataObjectStatus() != OSSIM_EMPTY)&&
           (theCacheTile->getBuf()))
      {
         //---
         // Note: Clip the cache tile(nitf block) to the image clipRect since
         // there are nitf blocks that go beyond the image dimensions, i.e.,
         // edge blocks.
         //---
         ossimIrect cr =
               theCacheTile->getImageRectangle().clipToRect(clipRect);
         theTile->loadTile(theCacheTile->getBuf(),
                           theCacheTile->getImageRectangle(),
                           cr,
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
            ossimNotify(ossimNotifyLevel_WARN)
               << __FILE__ << __LINE__
               << " loadTile failed!"
               << std::endl;
            theTile->makeBlank(); // loadTile failed...
         }
      }
   } // End of if ( tileRect.intersects(image_rect) )

   return theTile;   
}

bool ossimNitfTileSource::loadTile(const ossimIrect& clipRect)
{
   ossimIrect zbClipRect  = clipRect;

   const ossim_uint32 BLOCK_HEIGHT = theCacheSize.y;
   const ossim_uint32 BLOCK_WIDTH  = theCacheSize.x;

   zbClipRect.stretchToTileBoundary(ossimIpt(BLOCK_WIDTH, BLOCK_HEIGHT));
   
   //---
   // Shift the upper left corner of the "clip_rect" to the an even nitf
   // block boundry.  
   //---
   ossimIpt nitfBlockOrigin = zbClipRect.ul();

   // Vertical block loop.
   ossim_int32 y = nitfBlockOrigin.y;
   while (y < zbClipRect.lr().y)
   {
      // Horizontal block loop.
      ossim_int32 x = nitfBlockOrigin.x;
      while (x < zbClipRect.lr().x)
      {
         if ( loadBlockFromCache(x, y, clipRect) == false )
         {
            if ( loadBlock(x, y) )
            {
               //---
               // Note: Clip the cache tile(nitf block) to the image clipRect
               // since there are nitf blocks that go beyond the image
               // dimensions, i.e., edge blocks.
               //---    
               ossimIrect cr =
                  theCacheTile->getImageRectangle().clipToRect(clipRect);
               
               theTile->loadTile(theCacheTile->getBuf(),
                                 theCacheTile->getImageRectangle(),
                                 cr,
                                 theCacheTileInterLeaveType);
            }
            else
            {
               // Error loading...
               return false;
            }
         }
         
         x += BLOCK_WIDTH; // Go to next block.
      }
      
      y += BLOCK_HEIGHT; // Go to next row of blocks.
   }

   return true;
}

bool ossimNitfTileSource::loadBlockFromCache(ossim_uint32 x, ossim_uint32 y,
                                             const ossimIrect& clipRect)
{
   bool result = false;
   
   if (theCacheEnabledFlag)
   {
      //---
      // The origin set in the cache tile must have the sub image offset in it
      // since "theTile" is relative to any sub image offset.  This is so that
      // "theTile->loadTile(theCacheTile)" will work.
      //---
      ossimIpt origin(x, y);

      ossimRefPtr<ossimImageData> tempTile =
         ossimAppFixedTileCache::instance()->getTile(theCacheId, origin);
      if (tempTile.valid())
      {
         //---
         // Note: Clip the cache tile(nitf block) to the image clipRect since
         // there are nitf blocks that go beyond the image dimensions, i.e.,
         // edge blocks.
         //---    
         ossimIrect cr =
            tempTile->getImageRectangle().clipToRect(clipRect);

         theTile->loadTile(tempTile.get()->getBuf(),
                           tempTile->getImageRectangle(),
                           cr,
                           theCacheTileInterLeaveType);
         result = true;
      }
   }
   
   return result;
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
               buf = (ossim_uint8*)&(theCompressedBuf.front());
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
               else if(hdr->getCompressionCode() == "C4")
               {
                  vqUncompressC4(theCacheTile,
                                 (ossim_uint8*)&(theCompressedBuf.front()));
               }

               else if(hdr->getCompressionCode() == "M4")
               {
                  vqUncompressM4(theCacheTile,
                                 (ossim_uint8*)&(theCompressedBuf.front()));
               }
               else if(hdr->getRepresentation().upcase().contains("LUT"))
               {
                  lutUncompress(theCacheTile,
                                (ossim_uint8*)&(theCompressedBuf.front()));
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

   if ( !isVqCompressed(hdr->getCompressionCode()) )
   {
      convertTransparentToNull(theCacheTile);
   }

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
   ossimIpt tempOrigin = tile->getOrigin();
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

   ossim_uint64 blockNumber = getBlockNumber(ossimIpt(x,y));
   
#if 0
   cout << "ossimNitfTileSource::getPosition blockNumber:  "
        << blockNumber << endl;
#endif
   
   streamPosition = (std::streamoff)hdr->getDataLocation(); // Position to first block.
   if(hdr->hasBlockMaskRecords())
   {
      ossim_uint64 blockOffset = hdr->getBlockMaskRecordOffset(blockNumber,
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
            (std::streamoff)((ossim_uint64)blockNumber * 
                (ossim_uint64)getBlockOffset()) +
               ((ossim_uint64)getBandOffset() * band);
         }
         else
         {
            streamPosition += (std::streamoff)((ossim_uint64)getBandOffset() * (ossim_uint64)band);
            
         }
         break;
      }
      
      case READ_BIB:
      {
         streamPosition +=
         (std::streamoff) ((ossim_uint64)blockNumber * (ossim_uint64)theReadBlockSizeInBytes)+
            ((ossim_uint64)getBandOffset() * (ossim_uint64)band);
         break;
      }
      
      case READ_BSQ_BLOCK:
      {
         
         if(!hdr->hasBlockMaskRecords())
         {
            streamPosition += (std::streamoff)((ossim_uint64)blockNumber * 
                                               (ossim_uint64)getBlockOffset()) +
                                              ((ossim_uint64)getBandOffset() * 
                                               (ossim_uint64)band);
         }
         
         break;
      }
      case READ_JPEG_BLOCK:
      {
         streamPosition += (std::streamoff)((ossim_uint64)blockNumber * (ossim_uint64)theReadBlockSizeInBytes);
         break;
      }
      default:
      {
         if(!hdr->hasBlockMaskRecords())
         {
            streamPosition += (std::streamoff)((ossim_uint64)blockNumber*(ossim_uint64)getBlockOffset());
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
   ossim_uint32 result = 0;
   if(!theCacheSize.hasNans()&& theCacheSize.x > 0)
   {
      result = theCacheSize.x;
   }
   else
   {
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      result = static_cast<ossim_uint32>(tileSize.x);
   }
   return result;
}

ossim_uint32 ossimNitfTileSource::getTileHeight() const
{
   ossim_uint32 result = 0;
   if(!theCacheSize.hasNans()&& theCacheSize.y > 0)
   {
      result = theCacheSize.y;
   }
   else
   {
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
      result = static_cast<ossim_uint32>(tileSize.y);
   }
   return result;
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
   ossim_uint32 result = 0;
   if (resLevel == 0)
   {
      const ossimNitfImageHeader* hdr = getCurrentImageHeader();
      if (hdr)
      {
         result = hdr->getNumberOfRows();
      }
   }
   else if (theOverview.valid())
   {
      result = theOverview->getNumberOfLines(resLevel);
   }
   return result;
}

ossim_uint32 ossimNitfTileSource::getNumberOfSamples(ossim_uint32 resLevel) const
{
   ossim_uint32 result = 0;
   if (resLevel == 0)
   {
      const ossimNitfImageHeader* hdr = getCurrentImageHeader();
      if (hdr)
      {
         result = hdr->getNumberOfCols();
      }
   }
   else if (theOverview.valid())
   {
      result = theOverview->getNumberOfSamples(resLevel);
   }
   return result;
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

ossim_uint32 ossimNitfTileSource::getPartialReadSize(const ossimIpt& /* blockOrigin */)const
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

ossim_uint32 ossimNitfTileSource::getCurrentEntry() const
{
   return theCurrentEntry;
}

ossim_uint32 ossimNitfTileSource::getNumberOfEntries() const
{
   return (ossim_uint32)theEntryList.size();
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
   bool result = true;
   if (theCurrentEntry != entryIdx)
   {
      if ( isOpen() )
      {
         if ( entryIdx < theNumberOfImages )
         {
            // Clear the geometry.
            theGeometry = 0;
            
            // Must clear or openOverview will use last entries.
            theOverviewFile.clear();
            
            theCurrentEntry = entryIdx;
            
            //---
            // Since we were previously open and the the entry has changed we
            // need to reinitialize some things.
            //---
            result = allocate();
            if (result)
            {
               completeOpen();
            }
         }
         else
         {
            result = false; // Entry index out of range.
         }
      }
      else
      {
         //---
         // Not open.
         // Allow this knowing that the parseFile will check for out of range.
         //---
         theCurrentEntry = entryIdx;
      }
   }
   
   if(result)
   {
      if(theNitfImageHeader[theCurrentEntry]->getRepresentation().contains("LUT"))
      {
         theLut = theNitfImageHeader[theCurrentEntry]->createLut(0);
      }
      
      
   }
   return result;
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

const ossimNitfFileHeader* ossimNitfTileSource::getFileHeader()const
{
   if(theNitfFile.valid())
   {
      return theNitfFile->getHeader();
   }
   
   return 0;
}

ossimNitfFileHeader* ossimNitfTileSource::getFileHeader()
{
   if(theNitfFile.valid())
   {
      return theNitfFile->getHeader();
   }
   
   return 0;
}

const ossimNitfImageHeader* ossimNitfTileSource::getCurrentImageHeader() const
{
   if(theNitfImageHeader.size())
   {
      return theNitfImageHeader[theCurrentEntry].get();
   }
   
   return 0;
}

ossimNitfImageHeader* ossimNitfTileSource::getCurrentImageHeader()
{
   if(theNitfImageHeader.size())
   {
      return theNitfImageHeader[theCurrentEntry].get();
   }
   
   return 0;
}

void ossimNitfTileSource::setBoundingRectangle(const ossimIrect& imageRect)
{
   theImageRect = imageRect;
   // now shift the internal block rect as well
   theBlockImageRect = (theBlockImageRect - theBlockImageRect.ul());
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
      if(theNitfFile.valid())
      {
         if(theNitfFile->getHeader())
         {
            ossimRefPtr<ossimProperty> p = theNitfFile->getHeader()->getProperty(name);
            if(p.valid())
            {
               p->setReadOnlyFlag(true);
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
            p->setReadOnlyFlag(true);
            return p;
         }
      }
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
   if(theNitfFile->getHeader())
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

void ossimNitfTileSource::vqUncompressC4(
   ossimRefPtr<ossimImageData> destination, ossim_uint8* source)
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr||!destination)
   {
      return;
   }

   const ossim_uint32 BANDS = destination->getNumberOfBands();

   if( ( (BANDS != 1) && (BANDS!=3) ) ||
       (!destination->getBuf()) ||
       (destination->getScalarType()!=OSSIM_UINT8) ||
       !theLut.valid() ||
       (theLut->getNumberOfBands() != BANDS) )
   {
      return;
   }
   
   ossimNitfVqCompressionHeader* compressionHeader =
      PTR_CAST(ossimNitfVqCompressionHeader,
               hdr->getCompressionHeader().get());

   if(!compressionHeader)
   {
      return;
   }
   
   const std::vector<ossimNitfVqCompressionOffsetTableData>& table =
      compressionHeader->getTable();

   ossimRefPtr<ossimNitfImageBand> bandInfo = hdr->getBandInformation(0);
   
   if(!bandInfo.valid()) return;
   
   std::vector<ossimRefPtr<ossimNitfImageLut> > luts(BANDS);
   std::vector<ossim_uint8*> tempRows(BANDS);
   
   ossim_uint32 band;
   for (band =0; band<BANDS; ++band)
   {
      luts[band] = bandInfo->getLut(band);
      if ( luts[band].valid() )
      {
         tempRows[band] = (ossim_uint8*)destination->getBuf(band);
      }
      else
      {
         return;
      }
   }

   ossimPackedBits bits(source, compressionHeader->getImageCodeBitLength());


   const ossim_uint32 ROWS = static_cast<ossim_uint32>(table.size());
   const ossim_uint32 COLS =
      static_cast<ossim_uint32>(table[0].
                                theNumberOfValuesPerCompressionLookup);
   const ossim_uint32 COMPRESSION_HEIGHT =
      compressionHeader->getNumberOfImageRows();
   const ossim_uint32 COMPRESSION_WIDTH  =
      compressionHeader->getNumberOfImageCodesPerRow();
   ossim_uint32 DEST_WIDTH  = destination->getWidth();
   
   ossim_uint32 compressionIdx = 0;
   ossim_uint32 uncompressIdx  = 0;
   ossim_uint32 uncompressYidx = 0;
   ossim_uint8  lutValue = 0;
   ossim_uint8* data     = 0;
   ossim_uint32 codeWord = 0;
   
   for(ossim_uint32 compressionYidx = 0;
       compressionYidx < COMPRESSION_HEIGHT;
       ++compressionYidx)
   {
      uncompressYidx = compressionYidx * ROWS * DEST_WIDTH;
      
      for(ossim_uint32 compressionXidx = 0;
          compressionXidx < COMPRESSION_WIDTH;
          ++compressionXidx)
      {
         uncompressIdx = uncompressYidx + COLS * compressionXidx;

         codeWord = bits.getValueAsUint32(compressionIdx++);
         codeWord *= COLS;

         for(ossim_uint32 rowIdx = 0; rowIdx < ROWS; ++rowIdx)
         {
            data = &(table[rowIdx].theData[codeWord]);
            
            for(ossim_uint32 colIdx = 0; colIdx < COLS; ++colIdx)
            {
               lutValue = (*data)&0xff;

               for (band = 0; band < BANDS; ++band)
               {
                  ossim_uint8 p = (*theLut.get())[lutValue][band];
                  tempRows[band][uncompressIdx+colIdx] = p;
               }
               ++data;
               
            } // column loop

            uncompressIdx += DEST_WIDTH;
            
         } // row loop

      } // x compression loop
      
   } // y compression loop
}

void ossimNitfTileSource::vqUncompressM4(
   ossimRefPtr<ossimImageData> destination, ossim_uint8* source)
{
   const ossimNitfImageHeader* hdr = getCurrentImageHeader();
   if (!hdr||!destination)
   {
      return;
   }

   const ossim_uint32 BANDS = destination->getNumberOfBands();

   if(( (BANDS != 1)&&(BANDS!=3) ) ||
      (!destination->getBuf())||
      (destination->getScalarType()!=OSSIM_UINT8))
   {
      return;
   }
   
   ossimNitfVqCompressionHeader* compressionHeader =
      PTR_CAST(ossimNitfVqCompressionHeader,
               hdr->getCompressionHeader().get());

   if(!compressionHeader)
   {
      return;
   }

   const std::vector<ossimNitfVqCompressionOffsetTableData>& table =
      compressionHeader->getTable();

   ossimRefPtr<ossimNitfImageBand> bandInfo = hdr->getBandInformation(0);
   
   if(!bandInfo.valid()) return;
   
   std::vector<ossimRefPtr<ossimNitfImageLut> > luts(BANDS);
   std::vector<ossim_uint8*> tempRows(BANDS);

   ossim_uint32 band;
   for (band =0; band<BANDS; ++band)
   {
      luts[band] = bandInfo->getLut(band);
      if ( luts[band].valid() )
      {
         tempRows[band] = (ossim_uint8*)destination->getBuf(band);
      }
      else
      {
         return;
      }
   }

   const ossim_uint8 NI = 216; // null index (transparency index).
   const ossim_uint8 NP = 0;   // null pixel

   ossim_uint32 destWidth  = destination->getWidth();
   ossimPackedBits bits(source, compressionHeader->getImageCodeBitLength());

   ossim_uint32 compressionYidx   = 0;
   ossim_uint32 compressionXidx   = 0;
   ossim_uint32 compressionIdx    = 0;
   ossim_uint32 uncompressIdx     = 0;
   ossim_uint32 uncompressYidx    = 0;
   ossim_uint32 rows   = (ossim_uint32)table.size();
   ossim_uint32 cols   = 0;
   ossim_uint32 rowIdx = 0;
   ossim_uint32 colIdx = 0;
   if(rows)
   {
      cols = table[0].theNumberOfValuesPerCompressionLookup;
   }
   ossim_uint32 compressionHeight = compressionHeader->getNumberOfImageRows();
   ossim_uint32 compressionWidth  =
      compressionHeader->getNumberOfImageCodesPerRow();
   ossim_uint8 lutValue = 0;
   ossim_uint8* data=0;

   for(compressionYidx = 0;
       compressionYidx < compressionHeight;
       ++compressionYidx)
   {
      uncompressYidx = compressionYidx*rows*destWidth;

      for(compressionXidx = 0;
          compressionXidx < compressionWidth;
          ++compressionXidx)
      {
         uncompressIdx = uncompressYidx + cols*compressionXidx;
         ossim_uint32 codeWord = bits.getValueAsUint32(compressionIdx);
         
         bool transparent = false;
         if (codeWord == 4095)
         {
            //---
            // Check to see if the whole kernel is transparent.  If no, the
            // null index '216' could be used for valid pixels.
            //
            // For more see docs:
            // MIL-PRF-89041A 3.13.1.2 Transparent pixels
            // MIL-STD-2411
            //---
            codeWord *= cols;
            transparent = true;
            for(rowIdx = 0; rowIdx < rows; ++rowIdx)
            {
               data = &table[rowIdx].theData[codeWord];
               
               for(colIdx = 0; colIdx < cols; ++colIdx)
               {
                  lutValue = (*data)&0xff;
                  if (lutValue != NI)
                  {
                     // Not a kernel full of transparent pixels.
                     transparent = false;
                     break;
                  }
                  ++data;
               }
               if (!transparent)
               {
                  break;
               }
               uncompressIdx += destWidth;
            }
         }

         // Reset everyone for loop to copy pixel data from lut.
         uncompressIdx = uncompressYidx + cols*compressionXidx;
         codeWord = bits.getValueAsUint32(compressionIdx);
         codeWord *= cols;

         for(rowIdx = 0; rowIdx < rows; ++rowIdx)
         {
            data = &table[rowIdx].theData[codeWord];
            
            for(colIdx = 0; colIdx < cols; ++colIdx)
            {
               lutValue = (*data)&0xff;
               
               for (band = 0; band < BANDS; ++band)
               {
                  ossim_uint8 p = luts[band]->getValue(lutValue);
                  tempRows[band][uncompressIdx+colIdx] = (!transparent?p:NP);
               }
               ++data;
            }

            uncompressIdx += destWidth;
         }
         ++compressionIdx;
         
      } // x loop
      
   } // y loop
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
   // Get the totol blocks.
   // Note:  There can be more than one jpeg image in the nitf.  So after blocks
   // found equals total_blocks get out.
   //---
   ossim_uint32 total_blocks = hdr->getNumberOfBlocksPerRow()*hdr->getNumberOfBlocksPerCol();
   
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
      if (static_cast<ossim_uint8>(c) == 0xff) // Found FF byte.
      {
         //---
         // Loop to skip multiple 0xff's in cases like FF FF D8
         //---
         while ( theFileStr.get(c) )
         {
            ++blockSize;
            if (static_cast<ossim_uint8>(c) != 0xff)
            {
               break;
            }
         }
         
         if (static_cast<ossim_uint8>(c) == 0xd8) 
         {
            // At SOI 0xFFD8 marker...
            std::streamoff pos = theFileStr.tellg();
            theNitfBlockOffset.push_back(pos-2);
         }
         else if (static_cast<ossim_uint8>(c) == 0xd9)
         {
            // At EOI 0xD9marker...
            theNitfBlockSize.push_back(blockSize);
            blockSize = 0;
         }

         //---
         // Since there can be more than one jpeg entry in a file, breeak out of
         // loop when we hit block size.
         //---
         if ( (theNitfBlockOffset.size() == total_blocks) &&
              (theNitfBlockSize.size()   == total_blocks) )
         {
            break;
         }
      }
   }

   theFileStr.seekg(0, ios::beg);
   theFileStr.clear();
   
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

#if 0 /* Please leave for debug. (drb) */
   ossimNotify(ossimNotifyLevel_WARN) << "current entry: " << theCurrentEntry << "\n";
   for (ossim_uint32 i = 0; i < total_blocks; ++i)
   {
      cout << "theNitfBlockOffset[" << i << "]: " << theNitfBlockOffset[i]
           << "\ntheNitfBlockSize[" << i << "]: " << theNitfBlockSize[i]
           << "\n";
   }
#endif

   return true;
}

bool ossimNitfTileSource::uncompressJpegBlock(ossim_uint32 x, ossim_uint32 y)
{
   ossim_uint32 blockNumber = getBlockNumber( ossimIpt(x,y) );

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfTileSource::uncompressJpegBlock DEBUG:"
         << "\nblockNumber:  " << blockNumber
         << std::endl;
   }

   //---
   // Logic to hold off on scanning for offsets until a block is actually needed
   // to speed up loads for things like ossim-info that don't actually read
   // pixel data.
   //---
   if ( m_jpegOffsetsDirty )
   {
      if ( scanForJpegBlockOffsets() )
      {
         m_jpegOffsetsDirty = false;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "ossimNitfTileSource::uncompressJpegBlock scan for offsets error!"
            << "\nReturning error..." << endl;
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return false;
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\noffset to block: " << theNitfBlockOffset[blockNumber]
         << "\nblock size: " << theNitfBlockSize[blockNumber]
         << std::endl;
   }
   
   // Seek to the block.
   theFileStr.seekg(theNitfBlockOffset[blockNumber], ios::beg);
   
   // Read the block into memory.
   std::vector<ossim_uint8> compressedBuf(theNitfBlockSize[blockNumber]);
   if (!theFileStr.read((char*)&(compressedBuf.front()),
                        theNitfBlockSize[blockNumber]))
   {
      theFileStr.clear();
      ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimNitfTileSource::uncompressJpegBlock Read Error!"
         << "\nReturning error..." << endl;
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return false;
   }
   
   if (m_isJpeg12Bit)
   {
#if defined(JPEG_DUAL_MODE_8_12)
      return ossimNitfTileSource_12::uncompressJpeg12Block(x,y,theCacheTile, 
       getCurrentImageHeader(), theCacheSize, compressedBuf, theReadBlockSizeInBytes, 
       theNumberOfOutputBands);
#endif  
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
     return false;
   }

   /* Now we can initialize the JPEG decompression object. */
   jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, sizeof(cinfo));
   
   //---
   // Step 2: specify data source.  In this case we will uncompress from
   // memory so we will use "ossimJpegMemorySrc" in place of " jpeg_stdio_src".
   //---
   ossimJpegMemorySrc (&cinfo,
                       &(compressedBuf.front()),
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
        return false;
      }
   }

   /* Step 4: set parameters for decompression */
   
   /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

   /* Step 5: Start decompressor */
   jpeg_start_decompress(&cinfo);

#if 0 /* Please leave for debug. (drb) */
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "jpeg cinfo.output_width:  " << cinfo.output_width
         << "\njpeg cinfo.output_height: " << cinfo.output_height
         << "\n";
   }
#endif
   
   const ossim_uint32 SAMPLES = cinfo.output_width;

   //---
   // Note: Some nitf will be tagged with a given number of lines but the last
   // jpeg block may go beyond that to a complete block.  So it you clamp to
   // last line of the nitf you will get a libjpeg error:
   // 
   // "Application transferred too few scanlines"
   //
   // So here we will always read the full jpeg block even if it is beyond the
   // last line of the nitf.
   //---
   const ossim_uint32 LINES_TO_READ =
      min(static_cast<ossim_uint32>(theCacheSize.y), cinfo.output_height);

   /* JSAMPLEs per row in output buffer */
   const ossim_uint32 ROW_STRIDE = SAMPLES * cinfo.output_components;

   if ( (SAMPLES < theCacheTile->getWidth() ) ||
        (LINES_TO_READ < theCacheTile->getHeight()) )
   {
      theCacheTile->makeBlank();
   }

   if ( (SAMPLES > theCacheTile->getWidth()) ||
        (LINES_TO_READ > theCacheTile->getHeight()) )
   {
     // Error...
     jpeg_finish_decompress(&cinfo);
     jpeg_destroy_decompress(&cinfo);
     return false;
   }

   // Get pointers to the cache tile buffers.
   std::vector<ossim_uint8*> destinationBuffer(theNumberOfInputBands);
   for (ossim_uint32 band = 0; band < theNumberOfInputBands; ++band)
   {
     destinationBuffer[band] = theCacheTile->getUcharBuf(band);
   }

   std::vector<ossim_uint8> lineBuffer(ROW_STRIDE);
   JSAMPROW jbuf[1];
   jbuf[0] = (JSAMPROW) &(lineBuffer.front());

   while (cinfo.output_scanline < LINES_TO_READ)
   {
     // Read a line from the jpeg file.
     jpeg_read_scanlines(&cinfo, jbuf, 1);

     //---
     // Copy the line which if band interleaved by pixel the the band
     // separate buffers.
     //
     // Note:
     // Not sure if IMODE of 'B' is interleaved the same as image with
     // IMODE of 'P'.
     //
     // This works with all samples with IMODE of B and P but I only have
     // one band 'B' and three band 'P'.  So if we ever get a three band
     // 'B' it could be wrong here. (drb - 20090615)
     //---
     ossim_uint32 index = 0;
     for (ossim_uint32 sample = 0; sample < SAMPLES; ++sample)         
     {
       for (ossim_uint32 band = 0; band < theNumberOfInputBands; ++band)
       {
         destinationBuffer[band][sample] = lineBuffer[index];
         ++index;
       }
     }

     for (ossim_uint32 band = 0; band < theNumberOfInputBands; ++band)
     {
       destinationBuffer[band] += theCacheSize.x;         
     }
   }

   // clean up...

   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);

   return true;
}

//---
// Default JPEG quantization tables
// Values from: MIL-STD-188-198, APPENDIX A
//---
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

// Protected to disallow use...
ossimNitfTileSource::ossimNitfTileSource(const ossimNitfTileSource& /* obj */)
{
}

// Protected to disallow use...
ossimNitfTileSource& ossimNitfTileSource::operator=(
   const ossimNitfTileSource& /* rhs */)
{
   return *this;
}
