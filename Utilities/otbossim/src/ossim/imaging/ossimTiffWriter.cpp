//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerda@home.com)
//
//*******************************************************************
//  $Id: ossimTiffWriter.cpp 20026 2011-09-01 16:33:18Z dburken $

#include <ossim/ossimConfig.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/support_data/ossimGeoTiff.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
#include <ossim/imaging/ossimScalarRemapper.h>

#include <tiffio.h>
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
#    include <xtiffio.h>
#    include <geotiff.h>
#    include <geo_normalize.h>
#    include <geovalues.h>
#  endif
#endif

#include <algorithm>
#include <sstream>

static ossimTrace traceDebug("ossimTiffWriter:debug");
static const char* TIFF_WRITER_OUTPUT_TILE_SIZE_X_KW = "output_tile_size_x";
static const char* TIFF_WRITER_OUTPUT_TILE_SIZE_Y_KW = "output_tile_size_y";
static const int   PCS_BRITISH_NATIONAL_GRID = 27700;
static const long  DEFAULT_JPEG_QUALITY = 75;

RTTI_DEF1(ossimTiffWriter, "ossimTiffWriter", ossimImageFileWriter);

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimTiffWriter.cpp 20026 2011-09-01 16:33:18Z dburken $";
#endif

ossimTiffWriter::ossimTiffWriter()
   :
      ossimImageFileWriter(),
      theTif(NULL),
      theCompressionType("none"),
      theJpegQuality(DEFAULT_JPEG_QUALITY),
      theOutputGeotiffTagsFlag(true),
      theColorLutFlag(false),
      theProjectionInfo(NULL),
      theOutputTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
      theForceBigTiffFlag(false),
      theBigTiffFlag(false)
{
   theColorLut = new ossimNBandLutDataObject();
   ossim::defaultTileSize(theOutputTileSize);
   theOutputImageType = "tiff_tiled_band_separate";

   
#ifdef OSSIM_ID_ENABLED /* to quell unused variable warning. */
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "OSSIM_ID:  " << OSSIM_ID << endl;
   }
#endif
}

ossimTiffWriter::~ossimTiffWriter()
{
   if(isOpen())
   {
      closeTiff();
   }
}

bool ossimTiffWriter::openTiff()
{
   static const char* MODULE = "ossimTiffWriter::openTiff()";

   if (theTif) // Close the existing file pointer.
   {
      
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF 
      XTIFFClose( (TIFF*)theTif );
#  else
      TIFFClose( (TIFF*)theTif );
#  endif
#else
      TIFFClose( (TIFF*)theTif );   
#endif

   }

   // Check for empty file name.
   if (theFilename.empty())
   {
      return false;
   }

   ossimString openMode = "w";
   if(theBigTiffFlag||theForceBigTiffFlag)
   {
      openMode += "8";
   }
   // Open the new file.
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
   theTif = XTIFFOpen( theFilename.c_str(), openMode.c_str() );
#  else
   theTif = TIFFOpen( theFilename.c_str(), openMode.c_str() );
#  endif
#else
   theTif = TIFFOpen( theFilename.c_str(), openMode.c_str() );
#endif

   if (!theTif)
   {
      setErrorStatus(); // base class
      ossimSetError(getClassName().c_str(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "File %s line %d Module %s Error:\n\
Error opening file:  %s\n",
                    __FILE__,
                    __LINE__,
                    MODULE,
                    theFilename.c_str());

      return false;
   }
   return true;
}

bool ossimTiffWriter::closeTiff()
{
   if (theTif)
   {
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
      XTIFFClose( (TIFF*)theTif );
#  else
      TIFFClose( (TIFF*)theTif );
#  endif
#else
      TIFFClose( (TIFF*)theTif );
#endif
      theTif = NULL;
   }

   return true;
}

bool ossimTiffWriter::writeTiffTags()
{
   static const char MODULE[] = "ossimTiffWriter::writeTiffTags";
   TIFF* tiffPtr = (TIFF*)theTif;
   if (!tiffPtr)
   {
      setErrorStatus(); // base class
      ossimSetError(getClassName().c_str(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "File %s line %d %s\nError:  Tiff pointer is null!\n\
Call setFilename method.\n",
                    __FILE__,
                    __LINE__,
                    MODULE);
      return false;
   }

   //---
   // NOTE:
   // Since the tiff library uses the variable argument list function "va_arg"
   // it is important to use the correct data type.  If in doubt see the
   // code for libtiff's _TIFFVSetField in "tif_dir.c" in the libtiff package.
   //---

   int bitsPerSample  = 0;
   int sampleFormat   = 0;
   switch( theInputConnection->getOutputScalarType() )
   {
   case OSSIM_UINT8:
      bitsPerSample = 8;
      sampleFormat = SAMPLEFORMAT_UINT;
      break;

   case OSSIM_UINT16:
   case OSSIM_USHORT11:
      bitsPerSample = 16;
      sampleFormat = SAMPLEFORMAT_UINT;
      break;

   case OSSIM_SINT16:
      bitsPerSample = 16;
      sampleFormat = SAMPLEFORMAT_INT;
      break;

   case OSSIM_FLOAT32:
   case OSSIM_NORMALIZED_FLOAT:
      bitsPerSample = 32;
      sampleFormat = SAMPLEFORMAT_IEEEFP;
      break;

   case OSSIM_NORMALIZED_DOUBLE:
   case OSSIM_FLOAT64:
      bitsPerSample = 64;
      sampleFormat = SAMPLEFORMAT_IEEEFP;
      break;

   default:
      return false;
   }

   // Set the pixel type.
   TIFFSetField( (TIFF*)tiffPtr, TIFFTAG_BITSPERSAMPLE, bitsPerSample );
   TIFFSetField( (TIFF*)tiffPtr, TIFFTAG_SAMPLEFORMAT, sampleFormat );

   // Set the image dimensions.
   ossim_uint32  width  = theAreaOfInterest.width();
   ossim_uint32  height = theAreaOfInterest.height();
   TIFFSetField( tiffPtr, TIFFTAG_IMAGEWIDTH, width);
   TIFFSetField( tiffPtr, TIFFTAG_IMAGELENGTH, height);
   if (isTiled())
   {
      ossim_uint32 tileXSize = theOutputTileSize.x;
      ossim_uint32 tileYSize = theOutputTileSize.y;
      TIFFSetField(tiffPtr, TIFFTAG_TILEWIDTH,  tileXSize);
      TIFFSetField(tiffPtr, TIFFTAG_TILELENGTH, tileYSize);
   }
   else
   {
      TIFFSetField(tiffPtr, TIFFTAG_ROWSPERSTRIP, ossim_uint32(1));
   }

   ossim_uint32 numberOfBands = theInputConnection->getNumberOfOutputBands();

   // Set the min/max values.
   std::vector<ossim_float64> minBand(numberOfBands);
   std::vector<ossim_float64> maxBand(numberOfBands);
   for(ossim_uint32 idx = 0; idx < numberOfBands; ++idx)
   {
      maxBand[idx] = theInputConnection->getMaxPixelValue(idx);
      minBand[idx] = theInputConnection->getMinPixelValue(idx);
   }
   
   writeMinMaxTags(minBand, maxBand);
   
   // Set the planar configuration.
   if ( (theOutputImageType == "tiff_strip") ||
        (theOutputImageType == "tiff_tiled") ||
        (theOutputImageType == "image/tiff") ||
        (theOutputImageType == "image/tif") ||
        (theOutputImageType == "image/gtif") ||
        (theOutputImageType == "image/gtiff") )
   {
      TIFFSetField( tiffPtr, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
   }
   else
   {
      TIFFSetField( tiffPtr, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
   }

   // Set the compression type:
   uint16 tiffCompressType = COMPRESSION_NONE;
   theCompressionType.downcase();
   if( theCompressionType == "jpeg")
   {
      tiffCompressType  = COMPRESSION_JPEG;

      // Set the jpeg quality.
      TIFFSetField( tiffPtr, TIFFTAG_JPEGQUALITY,  theJpegQuality);
   }
   else if(theCompressionType == "packbits")
   {
      tiffCompressType = COMPRESSION_PACKBITS;
   }
   else if((theCompressionType == "deflate") ||
           (theCompressionType == "zip"))
   {
      tiffCompressType  = COMPRESSION_DEFLATE;
   }
   TIFFSetField( tiffPtr, TIFFTAG_COMPRESSION, tiffCompressType);
   TIFFSetField(tiffPtr, TIFFTAG_SAMPLESPERPIXEL, (int)theInputConnection->getNumberOfOutputBands());

   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   bool lutEnabled = (theColorLutFlag&&
                      ((scalarType == OSSIM_UINT8)||
                       (scalarType == OSSIM_UINT16)||
                       (scalarType == OSSIM_USHORT11))&&
                      (theColorLut->getNumberOfEntries() > 0)&&
                      (theInputConnection->getNumberOfOutputBands() == 1));
   if(lutEnabled)
   {
      TIFFSetField( tiffPtr, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE );
      TIFFSetField( tiffPtr, TIFFTAG_INDEXED, (ossim_uint16)1);

      if(scalarType == OSSIM_UINT8)
      {
         ossim_uint16 r[256], g[256], b[256];
         
         memset(r, '\0', sizeof(ossim_uint16)*256);
         memset(g, '\0', sizeof(ossim_uint16)*256);
         memset(b, '\0', sizeof(ossim_uint16)*256);
         
         for(ossim_uint32 i = 0; i < theColorLut->getNumberOfEntries(); i++)
         {
            r[i] = (ossim_uint16) (((*theColorLut)[i][0]/255.0)*65535);
            g[i] = (ossim_uint16) (((*theColorLut)[i][1]/255.0)*65535);
            b[i] = (ossim_uint16) (((*theColorLut)[i][2]/255.0)*65535);
         }
         TIFFSetField(tiffPtr, TIFFTAG_COLORMAP, r, g ,b);
      }
      else
      {
         ossim_uint16 r[65536], g[65536], b[65536];
         memset(r, '\0', sizeof(ossim_uint16)*65536);
         memset(g, '\0', sizeof(ossim_uint16)*65536);
         memset(b, '\0', sizeof(ossim_uint16)*65536);
         
         for(ossim_uint32 i = 0; i < theColorLut->getNumberOfEntries(); i++)
         {
            r[i] = (ossim_uint16) ((*theColorLut)[i][0]);
            g[i] = (ossim_uint16) ((*theColorLut)[i][1]);
            b[i] = (ossim_uint16) ((*theColorLut)[i][2]);
         }
         TIFFSetField(tiffPtr, TIFFTAG_COLORMAP, r, g ,b);
      }
   }
   else if( (theInputConnection->getNumberOfOutputBands() == 3 ||
             theInputConnection->getNumberOfOutputBands() == 4 ||
             (thePhotoMetric == "rgb"))&&
            (scalarType == OSSIM_UCHAR))
   {
      TIFFSetField( tiffPtr, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   }
   else
   {
      TIFFSetField( tiffPtr, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
   }

   return true;
}

bool ossimTiffWriter::writeGeotiffTags(ossimRefPtr<ossimMapProjectionInfo> projectionInfo)
{
   TIFF* tiffPtr = (TIFF*)theTif;
   bool result = false;
   if ( tiffPtr )
   {
      if ( projectionInfo.valid() )
      {
         result = ossimGeoTiff::writeTags(tiffPtr, projectionInfo);
      }
   }
   return result;
}

void ossimTiffWriter::checkColorLut()
{
   
   // this code appears to be wrong.  We can only do an outo lut if the immediate input to the sequencer is 
   // a handler with a lut or some kind of lut source.
   // 
   // I think that we should add a flag to enable auto setting of the lut instead of just doing it. This code causes core
   // dumps if one is to replicate bands with a band selector where a lut is a single band output like a CIB
   //
   // I currenlty have to disable
   //
   
   
#if 0
   bool needColorLut = false;
   bool needLoop = true;
   ossimRefPtr<ossimNBandLutDataObject> colorLut = 0;
   ossimConnectableObject::ConnectableObjectList imageInputs = theInputConnection->getInputList();
   if (imageInputs.size() > 0)
   {
      for (ossim_uint32 i = 0; i < imageInputs.size(); i++)
      {
         if (needLoop == false)
         {
            break;
         }
         ossimImageChain* source = PTR_CAST(ossimImageChain, imageInputs[i].get());
         if (source)
         {
            ossimConnectableObject::ConnectableObjectList imageChains = source->getInputList();
            for (ossim_uint32 j = 0; j < imageChains.size(); j++)
            {
               if (needLoop == false)
               {
                  break;
               }
               ossimImageChain* imageChain = PTR_CAST(ossimImageChain, imageChains[j].get());
               if (imageChain)
               {
                  ossimConnectableObject::ConnectableObjectList imageHandlers =
                     imageChain->findAllObjectsOfType(STATIC_TYPE_INFO(ossimImageHandler), false);
                  
                  for (ossim_uint32 h= 0; h < imageHandlers.size(); h++)
                  {
                     ossimImageHandler* handler =
                        PTR_CAST(ossimImageHandler, imageHandlers[h].get());
                     if (handler)
                     {
                        if (handler->getLut() != 0) //
                        {
                           colorLut = handler->getLut();
                           needColorLut = true;
                        }
                        else //if not all handlers have color luts, ignore the color lut.
                        {
                           needColorLut = false;
                           needLoop = false;
                           break;
                        }
                     }
                  }
               }
            }
         }
      }
   }
   
   if (needColorLut && colorLut != 0)
   {
      setLut(*colorLut.get());
   }
#endif
}

bool ossimTiffWriter::writeFile()

{
   static const char MODULE[] = "ossimTiffWriter::writeFile";

   if (traceDebug()) CLOG << "Entered..." << std::endl;

   //checkColorLut();

   if(isLutEnabled())
   {
      theNBandToIndexFilter = new ossimNBandToIndexFilter;
      theNBandToIndexFilter->connectMyInputTo(0, theInputConnection->getInput());
      theNBandToIndexFilter->setLut(*theColorLut.get());
      theNBandToIndexFilter->initialize();
      theInputConnection->disconnect();
      theInputConnection->connectMyInputTo(0, theNBandToIndexFilter.get());
      theInputConnection->initialize();
   }
   else
   {
      theNBandToIndexFilter = 0;
   }
   
   if (traceDebug() && theInputConnection.get())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "DEBUG:"
         << "\nnull:  " << theInputConnection->getNullPixelValue()
         << "\nmin:   " << theInputConnection->getMinPixelValue()
         << "\nmax:   " << theInputConnection->getMaxPixelValue()
         << std::endl;
   }
   
   if (isTiled())
   {
      if ( (theInputConnection->getTileWidth()  !=
            static_cast<ossim_uint32>(theOutputTileSize.x)) ||
           (theInputConnection->getTileHeight() !=
            static_cast<ossim_uint32>(theOutputTileSize.y)) )
      {
         theInputConnection->setTileSize(theOutputTileSize);
      }
   }
   
   if(!theInputConnection->isMaster())
   {
      theInputConnection->slaveProcessTiles();

      if(theNBandToIndexFilter.valid())
      {
         theInputConnection->connectMyInputTo(0, theNBandToIndexFilter->getInput());
         theNBandToIndexFilter = 0;
      }
      
      return true;
   }

   // this might be called from writeFile(projection infoamrtion) method
   // we will check to see if the tiff is open.  If not then call the open.
   //
   if(isOpen())
   {
      close();
   }
   ossim_uint64 threeGigs = (static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(1024)*
                            static_cast<ossim_uint64>(3));
   ossimIrect bounds = theInputConnection->getBoundingRect();
   ossim_uint64 byteCheck = (static_cast<ossim_uint64>(bounds.width())*
                             static_cast<ossim_uint64>(bounds.height())*
                             static_cast<ossim_uint64>(theInputConnection->getNumberOfOutputBands())*
                             static_cast<ossim_uint64>(ossim::scalarSizeInBytes(theInputConnection->getOutputScalarType())));
	
   if(byteCheck > threeGigs)
   {
      theBigTiffFlag = true;
   }
   else
   {
      theBigTiffFlag = false;
   }
   open();

   if (!isOpen())
   {
      if (traceDebug())
      {
         CLOG << " ERROR:  Could not open!  Returning..." << std::endl;
      }

      return false;
   }

   if(!theInputConnection)
   {
      if (traceDebug())
      {
         CLOG << " ERROR:  No input connection!  Returning..." << std::endl;
      }

      return false;
   }

   // First write the tiff tags.
   if (writeTiffTags() == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " ERROR:"
            << "\nError detected writing tiff tags.  Returning..." << std::endl;
         return false;
      }
   }

   // Write the geotiff tags.
   if (theOutputGeotiffTagsFlag)
   {
      if(theViewController.get()) // let this override
      {                     // found in ossimImageWriter base
         ossimMapProjection* proj = PTR_CAST(ossimMapProjection,
                                             theViewController->getView());
         if(proj)
         {
            ossimRefPtr<ossimMapProjectionInfo> projectionInfo
               = new ossimMapProjectionInfo(proj, theAreaOfInterest);
            projectionInfo->setPixelType(thePixelType);

            if (writeGeotiffTags(projectionInfo) == false)
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " ERROR:"
                     << "\nError detected writing geotiff tags.  Returning..."
                     << std::endl;
               }
               return false;
            }
         }
      }
      else if(theProjectionInfo.valid())
      {
         theProjectionInfo->setPixelType(thePixelType);
         if (writeGeotiffTags(theProjectionInfo) == false)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << MODULE << " ERROR:"
                  << "\nError detected writing geotiff tags.  Returning..."
                  << std::endl;
            }
            return false;
         }
      }
      else
      {
         // Fetch the map projection of the input image if it exists:
         ossimMapProjection* mapProj = 0;
         ossimRefPtr<ossimImageGeometry> imgGeom = theInputConnection->getImageGeometry();
         if ( imgGeom.valid() )
         {
            const ossimProjection* proj = imgGeom->getProjection();
            mapProj = PTR_CAST(ossimMapProjection, proj);
         }
         if(mapProj)
         {
            ossimRefPtr<ossimMapProjectionInfo> projectionInfo
               = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);

            projectionInfo->setPixelType(thePixelType);
            if (writeGeotiffTags(projectionInfo) == false)
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " ERROR:"
                     << "\nError detected writing geotiff tags.  Returning..."
                     << std::endl;
               }
               return false;
            }
         }
      }

   } // End of "if (theOutputGeotiffTagsFlag)"

   // Write the file with the image data.
   bool status = false;
   if(theOutputImageType == "tiff_strip")
   {
      status = writeToStrips();
   }
   else if(theOutputImageType == "tiff_strip_band_separate")
   {
      status = writeToStripsBandSep();
   }
   else if((theOutputImageType == "tiff_tiled")||
           (ossimString::downcase(theOutputImageType) == "image/tiff")||
           (ossimString::downcase(theOutputImageType) == "image/tif")||
           (ossimString::downcase(theOutputImageType) == "image/gtif")||
           (ossimString::downcase(theOutputImageType) == "image/gtiff"))
   {
      status = writeToTiles();
   }
   else if(theOutputImageType == "tiff_tiled_band_separate")
   {
      status = writeToTilesBandSep();
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " ERROR:"
            << "\nUnsupported output type:  " << theOutputImageType
            << std::endl;
      }
   }

   close();

   if (traceDebug()) CLOG << " Exited..." << std::endl;

   if(theNBandToIndexFilter.valid())
   {
      theInputConnection->connectMyInputTo(0, theNBandToIndexFilter->getInput());
      theNBandToIndexFilter = 0;
   }
   
   return status;
}

void ossimTiffWriter::setLut(const ossimNBandLutDataObject& lut)
{
   theColorLutFlag = true;
   theColorLut = (ossimNBandLutDataObject*)lut.dup();
}

bool ossimTiffWriter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   kwl.add(prefix,
           "output_geotiff_flag",
           (int)theOutputGeotiffTagsFlag,
           true);

   kwl.add(prefix,
           TIFF_WRITER_OUTPUT_TILE_SIZE_X_KW,
           theOutputTileSize.x,
           true);

   kwl.add(prefix,
           TIFF_WRITER_OUTPUT_TILE_SIZE_Y_KW,
           theOutputTileSize.y,
           true);

   kwl.add(prefix,
           ossimKeywordNames::COMPRESSION_QUALITY_KW,
           theJpegQuality,
           true);

   kwl.add(prefix,
           ossimKeywordNames::COMPRESSION_TYPE_KW,
           theCompressionType,
           true);

   kwl.add(prefix,
           "color_lut_flag",
           (ossim_uint32)theColorLutFlag,
           true);

   if(theColorLutFlag)
   {
      if(theLutFilename != "")
      {
         kwl.add(prefix,
                 "lut_filename",
                 theLutFilename.c_str(),
                 true);
      }
      else
      {
         ossimString newPrefix = ossimString(prefix) + "lut.";
         theColorLut->saveState(kwl, newPrefix.c_str());
      }
   }


   return ossimImageFileWriter::saveState(kwl,
                                          prefix);
}

bool ossimTiffWriter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char *value;

   ossimIpt defaultTileSize;
   ossim::defaultTileSize(defaultTileSize);

   value = kwl.find(prefix,
                    TIFF_WRITER_OUTPUT_TILE_SIZE_X_KW);
   if(value)
   {
      theOutputTileSize.x = ossimString(value).toLong();
      if(theOutputTileSize.x<1)
      {
         theOutputTileSize.x = defaultTileSize.x;
      }
   }

   value = kwl.find(prefix,
                    TIFF_WRITER_OUTPUT_TILE_SIZE_Y_KW);
   if(value)
   {
      theOutputTileSize.y = ossimString(value).toLong();
      if(theOutputTileSize.y<1)
      {
         theOutputTileSize.y = defaultTileSize.y;
      }
   }


   value = kwl.find(prefix, ossimKeywordNames::COMPRESSION_TYPE_KW);
   if(value)
   {
      theCompressionType = ossimString(value).downcase();
   }
   else
   {
      theCompressionType = "none";
   }

   value = kwl.find(prefix, ossimKeywordNames::COMPRESSION_QUALITY_KW);
   if(value)
   {
      setJpegQuality(ossimString(value).toLong());
   }

   value = kwl.find(prefix, ossimKeywordNames::PHOTOMETRIC_KW);
   if(value)
   {
      thePhotoMetric = ossimString(value).downcase();
   }

   value = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(value)
   {
      setFilename(ossimFilename(value));
   }

   const char* flag = kwl.find(prefix, "output_geotiff_flag");
   if(flag)
   {
      theOutputGeotiffTagsFlag = ossimString(flag).toBool();
   }

   ossimString newPrefix = ossimString(prefix) + "lut.";

   const char* colorLutFlag = kwl.find(prefix, "color_lut_flag");
   if(colorLutFlag)
   {
      theColorLutFlag = ossimString(colorLutFlag).toBool();
   }
   else
   {
      theColorLutFlag = false;
   }
   theLutFilename = ossimFilename(kwl.find(prefix, "lut_filename"));

   theLutFilename = ossimFilename(theLutFilename.trim());
   if(theLutFilename != "")
   {
      theColorLut->open(theLutFilename);
   }
   else
   {
      theColorLut->loadState(kwl, newPrefix.c_str());
   }

   if(ossimImageFileWriter::loadState(kwl,
                                      prefix))
   {
      if((theOutputImageType!="tiff_tiled") &&
         (theOutputImageType!="tiff_tiled_band_separate") &&
         (theOutputImageType!="tiff_strip") &&
         (theOutputImageType!="tiff_strip_band_separate")&&
         (theOutputImageType!="image/tiff")&&
         (theOutputImageType!="image/tif")&&
         (theOutputImageType!="image/gtif")&&
         (theOutputImageType!="image/gtiff"))
      {

         theOutputImageType = "tiff_tiled_band_separate";;
      }
   }
   else
   {
      return false;
   }

   return true;
}

bool ossimTiffWriter::isTiled() const
{
   return ( theOutputImageType == "tiff_tiled" ||
            theOutputImageType == "image/tiff" ||
            theOutputImageType == "image/tif" ||
            theOutputImageType == "image/gtif" ||
            theOutputImageType == "image/gtiff" ||
            theOutputImageType == "tiff_tiled_band_separate" );
}

bool ossimTiffWriter::writeToTiles()
{
   static const char* const MODULE = "ossimTiffWriter::writeToTiles";
   TIFF* tiffPtr = (TIFF*)theTif;

   if (traceDebug()) CLOG << " Entered." << std::endl;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossimRefPtr<ossimImageData> tempTile = 0;

   if(theColorLutFlag)
   {
      tempTile = ossimImageDataFactory::instance()->create(this, 1, theInputConnection.get());
   }
   else
   {
      tempTile = ossimImageDataFactory::instance()->create(this, theInputConnection.get());
   }
   if(tempTile.valid())
   {
      tempTile->initialize();
   }
   ossim_uint32 tilesWide       = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint32 tilesHigh       = theInputConnection->getNumberOfTilesVertical();
   ossim_uint32 tileWidth       = theInputConnection->getTileWidth();
   ossim_uint32 tileHeight      = theInputConnection->getTileHeight();
   ossim_uint32 numberOfTiles   = theInputConnection->getNumberOfTiles();

   // Tile loop in the height direction.
   ossim_uint32 tileNumber = 0;
   vector<ossim_float64> minBands;
   vector<ossim_float64> maxBands;
   for(ossim_uint32 i = 0; ((i < tilesHigh)&&!needsAborting()); i++)
   {
      ossimIpt origin(0,0);
      origin.y = i * tileHeight;

      // Tile loop in the sample (width) direction.
      for(ossim_uint32 j = 0; ((j < tilesWide)&&!needsAborting()); j++)
      {
         origin.x = j * tileWidth;

         // Grab the tile.
         ossimRefPtr<ossimImageData> id = theInputConnection->getNextTile();
         if (!id)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "Error returned writing tiff tile:  " << tileNumber
               << "\nNULL Tile encountered"
               << std::endl;
            return false;
         }

         ossimDataObjectStatus  tileStatus      = id->getDataObjectStatus();
         ossim_uint32           tileSizeInBytes = id->getSizeInBytes();
         if (tileStatus != OSSIM_FULL)
         {
            // Clear out the buffer since it won't be filled all the way.
            tempTile->setImageRectangle(id->getImageRectangle());
            tempTile->makeBlank();
         }

         if ((tileStatus == OSSIM_PARTIAL || tileStatus == OSSIM_FULL))
         {
            // Stuff the tile into the tileBuffer.
            id->unloadTile(tempTile->getBuf(),
                           id->getImageRectangle(),
                           OSSIM_BIP);
            tempTile->setDataObjectStatus(id->getDataObjectStatus());
            if(!theColorLutFlag&&!needsAborting())
            {
               id->computeMinMaxPix(minBands, maxBands);
            }
         }

         //---
         // Write the tile to disk.
         //---
         ossim_uint32 bytesWritten = 0;
         bytesWritten = TIFFWriteTile(tiffPtr,
                                      tempTile->getBuf(),
                                      origin.x,
                                      origin.y,
                                      0,            // z
                                      0);           // s

         if (bytesWritten != tileSizeInBytes)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << tileSizeInBytes
                  << "\nBytes written:  " << bytesWritten
                  << std::endl;
            }
            setErrorStatus();
            return false;
         }

         ++tileNumber;

      } // End of tile loop in the sample (width) direction.

      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);

   } // End of tile loop in the line (height) direction.

   if(!theColorLutFlag&&!needsAborting())
   {
      writeMinMaxTags(minBands, maxBands);
   }

   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

bool ossimTiffWriter::writeToTilesBandSep()
{
   static const char* const MODULE = "ossimTiffWriter::writeToTilesBandSep";
   TIFF* tiffPtr = (TIFF*)theTif;
   if (traceDebug()) CLOG << " Entered." << std::endl;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossim_uint32 bands     = theInputConnection->getNumberOfOutputBands();
   ossim_uint32 tilesWide = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint32 tilesHigh = theInputConnection->getNumberOfTilesVertical();
   ossim_uint32 tileWidth     = theInputConnection->getTileWidth();
   ossim_uint32 tileHeight    = theInputConnection->getTileHeight();
   ossim_uint32 numberOfTiles = theInputConnection->getNumberOfTiles();

#if 0
   if(traceDebug())
   {
      ossimIrect   boundingRect  = theInputConnection->getBoundingRect();
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "Bounding rect = " << boundingRect
         << "\nBands         = " << bands
         << "\ntilesWide     = " << tilesWide
         << "\ntilesHigh     = " << tilesHigh
         << "\ntileWidth     = " << tileWidth
         << "\ntileHeight    = " << tileHeight << std::endl;
   }
#endif

   ossim_uint32 tileNumber = 0;
   vector<ossim_float64> minBands;
   vector<ossim_float64> maxBands;
   for(ossim_uint32 i = 0; ((i < tilesHigh)&&!needsAborting()); ++i)
   {
      ossimIpt origin;
      origin.y = i * tileHeight;

      //---
      // Tile loop in the sample (width) direction.
      //---
      for(ossim_uint32 j = 0; ((j < tilesWide)&&!needsAborting()); ++j)
      {
         origin.x = j * tileWidth;

         ossimRefPtr<ossimImageData> id = theInputConnection->getNextTile();
	 if(!id)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "Error returned writing tiff tile:  " << i
               << "\nNULL Tile encountered"
               << std::endl;
            return false;
         }
	 ossim_int32 tileSizeInBytes = id->getSizePerBandInBytes();

         if(!theColorLutFlag)
         {
            id->computeMinMaxPix(minBands, maxBands);
         }

         //---
         // Band loop.
         //---
         for (ossim_uint32 band=0; ((band<bands)&&(!needsAborting())); ++band)
         {
            // Grab a pointer to the tile for the band.
            tdata_t* data = (tdata_t*)id->getBuf(band);
            // Write the tile.
            tsize_t bytesWritten = 0;
            if(data)
            {
               bytesWritten = TIFFWriteTile(tiffPtr,
                                            data,
                                            (ossim_uint32)origin.x,
                                            (ossim_uint32)origin.y,
                                            (ossim_uint32)0,        // z
                                            (tsample_t)band);    // sample
            }
            if ( ( bytesWritten != tileSizeInBytes ) && !needsAborting() )
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " ERROR:"
                     << "Error returned writing tiff tile:  " << i
                     << "\nExpected bytes written:  " << tileSizeInBytes
                     << "\nBytes written:  " << bytesWritten
                     << std::endl;
               }
               setErrorStatus();
               return false;
            }
            
         } // End of band loop.

         ++tileNumber;

      } // End of tile loop in the sample (width) direction.

      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);
      if(needsAborting())
      {
         setPercentComplete(100);
      }

   } // End of tile loop in the line (height) direction.

   if(!theColorLutFlag&&!needsAborting())
   {
      writeMinMaxTags(minBands, maxBands);
   }
    
   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

bool ossimTiffWriter::writeToStrips()
{
   static const char* const MODULE = "ossimTiffWriter::writeToStrips";
   TIFF* tiffPtr = (TIFF*)theTif;

   if (traceDebug()) CLOG << " Entered." << std::endl;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossim_uint32 bands = theInputConnection->getNumberOfOutputBands();
   ossim_uint32 tilesWide = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint32 tilesHigh = theInputConnection->getNumberOfTilesVertical();
   ossim_uint32 tileHeight = theInputConnection->getTileHeight();
   ossim_uint32 numberOfTiles = theInputConnection->getNumberOfTiles();
   ossim_uint32 width = theAreaOfInterest.width();
   ossim_uint32 bytesInLine =
      ossim::scalarSizeInBytes(theInputConnection->getOutputScalarType()) *
      width * bands;

   //---
   // Buffer to hold one line x tileHeight
   //---
   ossim_uint32 bufferSizeInBytes = bytesInLine * tileHeight;
   unsigned char* buffer = new unsigned char[bufferSizeInBytes];

   int tileNumber = 0;
   vector<ossim_float64> minBands;
   vector<ossim_float64> maxBands;
   for(ossim_uint32 i = 0; ((i < tilesHigh)&&(!needsAborting())); ++i)
   {
      // Clear the buffer.
      memset(buffer, 0, bufferSizeInBytes);

      // Set the buffer rectangle.
      ossimIrect bufferRect(theAreaOfInterest.ul().x,
                            theAreaOfInterest.ul().y + i * tileHeight,
                            theAreaOfInterest.ul().x + width - 1,
                            theAreaOfInterest.ul().y + i * tileHeight +
                            tileHeight - 1);

      // Tile loop in the sample (width) direction.
      for(ossim_uint32 j = 0; ((j < tilesWide)&&(!needsAborting())); ++j)
      {
         // Get the tile and copy it to the buffer.
         ossimRefPtr<ossimImageData> id = theInputConnection->getNextTile();
         if (!id)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "Error returned writing tiff tile:  " << tileNumber
               << "\nNULL Tile encountered"
               << std::endl;
            delete [] buffer;
            return false;
         }
         id->unloadTile(buffer, bufferRect, OSSIM_BIP);
         if(!theColorLutFlag&&!needsAborting())
         {
            id->computeMinMaxPix(minBands, maxBands);
         }
         ++tileNumber;
      }

      // Get the number of lines to write from the buffer.
      ossim_uint32 linesToWrite = min(tileHeight, static_cast<ossim_uint32>(theAreaOfInterest.lr().y - bufferRect.ul().y + 1));

      // Write the buffer out to disk.
      ossim_uint32 row = static_cast<ossim_uint32>(bufferRect.ul().y -
                                                   theAreaOfInterest.ul().y);
      ossim_uint8* buf = buffer;
      for (ossim_uint32 ii=0; ((ii<linesToWrite)&&(!needsAborting())); ++ii)
      {
         ossim_int32 status = TIFFWriteScanline(tiffPtr,
                                                buf,
                                                row,
                                                0);
         if (status == -1)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "Error returned writing tiff scanline:  " << row
               << std::endl;
            setErrorStatus();
            delete [] buffer;
            return false;
         }

         ++row;  // Increment the line number.
         buf += bytesInLine;

      } // End of loop to write lines from buffer to tiff file.

      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);
      if(needsAborting())
      {
         setPercentComplete(100);
      }

   } // End of loop in the line (height) direction.

   if(!theColorLutFlag)
   {
      writeMinMaxTags(minBands, maxBands);
   }
   
   // Free the memory.
   delete [] buffer;

   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

bool ossimTiffWriter::writeToStripsBandSep()
{
   static const char* const MODULE = "ossimTiffWriter::writeToStripsBandSep";
   TIFF* tiffPtr = (TIFF*)theTif;

   if (traceDebug()) CLOG << " Entered." << std::endl;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossim_uint32 bands = theInputConnection->getNumberOfOutputBands();
   ossim_uint32 tilesWide = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint32 tilesHigh = theInputConnection->getNumberOfTilesVertical();
   ossim_uint32 tileHeight      = theInputConnection->getTileHeight();
   ossim_uint32 numberOfTiles   = theInputConnection->getNumberOfTiles();
   ossim_uint32 width           = theAreaOfInterest.width();
   ossim_uint32 bytesInLine     =
      ossim::scalarSizeInBytes(theInputConnection->getOutputScalarType()) *
      width;

   //---
   // Buffer to hold one line x tileHeight
   //---
   ossim_uint32 bufferSizeInBytes = bytesInLine * tileHeight * bands;

   unsigned char* buffer = new unsigned char[bufferSizeInBytes];

   // Tile loop in height direction.
   ossim_uint32 tileNumber = 0;
   vector<ossim_float64> minBands;
   vector<ossim_float64> maxBands;
   for(ossim_uint32 i = 0; ((i < tilesHigh)&&(!needsAborting())); ++i)
   {
      // Clear the buffer.
      memset(buffer, 0, bufferSizeInBytes);

      // Set the buffer rectangle.
      ossimIrect bufferRect(theAreaOfInterest.ul().x,
                            theAreaOfInterest.ul().y + i * tileHeight,
                            theAreaOfInterest.ul().x + width - 1,
                            theAreaOfInterest.ul().y + i * tileHeight +
                            tileHeight - 1);

      // Tile loop in the sample (width) direction.
      for(ossim_uint32 j = 0; ((j < tilesWide)&&(!needsAborting())); ++j)
      {
         // Get the tile and copy it to the buffer.
         ossimRefPtr<ossimImageData> id = theInputConnection->getNextTile();
         if (!id)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "Error returned writing tiff tile:  " << tileNumber
               << "\nNULL Tile encountered"
               << std::endl;
            delete [] buffer;
            return false;
         }
         id->unloadTile(buffer, bufferRect, OSSIM_BIL);
         if(!theColorLutFlag)
         {
            id->computeMinMaxPix(minBands, maxBands);
         }
         ++tileNumber;
      }

      // Get the number of lines to write from the buffer.
      ossim_uint32 linesToWrite = min(tileHeight, static_cast<ossim_uint32>(theAreaOfInterest.lr().y - bufferRect.ul().y + 1));

      // Write the buffer out to disk.
      ossim_uint32 row = static_cast<ossim_uint32>(bufferRect.ul().y -
                                       theAreaOfInterest.ul().y);
      ossim_uint8* buf = buffer;
      for (ossim_uint32 ii=0; ((ii<linesToWrite)&&(!needsAborting())); ++ii)
      {
         for (ossim_uint32 band =0; ((band<bands)&&(!needsAborting())); ++band)
         {
            ossim_int32 status = TIFFWriteScanline(tiffPtr,
                                                   buf,
                                                   row,
                                                   band);
            if (status == -1)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff scanline:  " << row
                  << std::endl;
               delete [] buffer;
               return false;
            }
            buf += bytesInLine;
         }

         ++row;  // Increment the line number.

      } // End of loop to write lines from buffer to tiff file.

      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);
      if(needsAborting())
      {
         setPercentComplete(100);
      }
   } // End of loop in the line (height) direction.

   if(!theColorLutFlag)
   {
      writeMinMaxTags(minBands, maxBands);
   }

   // Free the memory.
   delete [] buffer;

   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

void ossimTiffWriter::setTileSize(const ossimIpt& tileSize)
{
   if ( (tileSize.x % 16) || (tileSize.y % 16) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTiffWriter::changeTileSize ERROR:"
            << "\nTile size must be a multiple of 32!"
            << "\nSize remains:  " << theOutputTileSize
            << std::endl;
      }
      return;
   }

   theOutputTileSize = tileSize;
}

void ossimTiffWriter::writeMinMaxTags(const vector<ossim_float64>& minBand,
                                      const vector<ossim_float64>& maxBand)
{
   TIFF* tiffPtr = (TIFF*)theTif;
   if(minBand.size() && maxBand.size())
   {
      ossim_float64 minValue =
         *std::min_element(minBand.begin(), minBand.end());
      ossim_float64 maxValue =
         *std::max_element(maxBand.begin(), maxBand.end());

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTiffWriter::writeMinMaxTags DEBUG:"
            << "\nminValue:  " << minValue
            << "\nmaxValue:  " << maxValue
            << std::endl;
      }

      switch( theInputConnection->getOutputScalarType() )
      {
         case OSSIM_USHORT11:
         {
            TIFFSetField( tiffPtr, TIFFTAG_MINSAMPLEVALUE,
                          static_cast<ossim_sint16>(0) );
            TIFFSetField( tiffPtr, TIFFTAG_MAXSAMPLEVALUE,
                          static_cast<ossim_sint16>(2047) );
            break;
         }
         case OSSIM_UINT8:
         case OSSIM_UINT16:
         {
            TIFFSetField( tiffPtr, TIFFTAG_MINSAMPLEVALUE,
                          static_cast<ossim_sint16>(minValue) );
            TIFFSetField( tiffPtr, TIFFTAG_MAXSAMPLEVALUE,
                          static_cast<ossim_sint16>(maxValue) );
            break;
         }
         
         case OSSIM_SINT16: 
         case OSSIM_UINT32:
         case OSSIM_FLOAT32:
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            TIFFSetField( tiffPtr, TIFFTAG_SMINSAMPLEVALUE,
                          static_cast<ossim_float32>(minValue) );
            TIFFSetField( tiffPtr, TIFFTAG_SMAXSAMPLEVALUE,
                          static_cast<ossim_float32>(maxValue) );
            break;
         }
         default:
         {
            break;
         }
      }
   }
}

void ossimTiffWriter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property)
   {
      return;
   }

   if(property->getName() == ossimKeywordNames::COMPRESSION_QUALITY_KW)
   {
//       ossimNumericProperty* numericProperty = PTR_CAST(ossimNumericProperty,
//                                                        property.get());
//       if (numericProperty)
//       {
      setJpegQuality( property->valueToString().toInt32() );
//       }
   }
   else if (property->getName() == ossimKeywordNames::COMPRESSION_TYPE_KW)
   {
      ossimStringProperty* stringProperty = PTR_CAST(ossimStringProperty,
                                                     property.get());
      if (stringProperty)
      {
         ossimString s;
         stringProperty->valueToString(s);
         setCompressionType(s);
      } 
   }
   else if(property->getName() == "lut_file")
   {
      theLutFilename = ossimFilename(property->valueToString());
      theColorLut->open(theLutFilename);
   }
   else if(property->getName() == "color_lut_flag")
   {
      theColorLutFlag = property->valueToString().toBool();
   }
   else if(property->getName() == "big_tiff_flag")
   {
      theForceBigTiffFlag = property->valueToString().toBool();
   }
   else if(property->getName() == ossimKeywordNames::OUTPUT_TILE_SIZE_KW)
   {
      theOutputTileSize.x = property->valueToString().toInt32();
      theOutputTileSize.y =  theOutputTileSize.x;
   }
   else
   {
      ossimImageFileWriter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimTiffWriter::getProperty(const ossimString& name)const
{
   if(name == "Filename")
   {
      ossimRefPtr<ossimProperty> tempProp = ossimImageFileWriter::getProperty(name);
      if(tempProp.valid())
      {
         ossimFilenameProperty* filenameProp = PTR_CAST(ossimFilenameProperty,
                                                        tempProp.get());

         if(filenameProp)
         {
            filenameProp->addFilter("*.tif");
         }

         return tempProp;
      }
   }
   else if (name == ossimKeywordNames::COMPRESSION_QUALITY_KW)
   {
      ossimNumericProperty* numericProp =
         new ossimNumericProperty(name,
                                  ossimString::toString(theJpegQuality),
                                  1.0,
                                  100.0);
      numericProp->
         setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
      return numericProp;
   }
   else if (name == ossimKeywordNames::COMPRESSION_TYPE_KW)
   {
      ossimStringProperty* stringProp =
         new ossimStringProperty(name,
                                 getCompressionType(),
                                 false); // editable flag
      stringProp->addConstraint(ossimString("none"));
      stringProp->addConstraint(ossimString("jpeg"));
      stringProp->addConstraint(ossimString("packbits"));
      stringProp->addConstraint(ossimString("deflate"));
      stringProp->addConstraint(ossimString("zip"));      
      return stringProp;
   }
   else if (name == "lut_file")
   {
      ossimFilenameProperty* property = new ossimFilenameProperty(name, theLutFilename);
      property->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);

      return property;
   }
   else if (name == "color_lut_flag")
   {
      ossimBooleanProperty* boolProperty = new ossimBooleanProperty(name,
                                                                    theColorLutFlag);
      return boolProperty;
   }
   else if(name == "big_tiff_flag")
   {
       ossimBooleanProperty* boolProperty = new ossimBooleanProperty(name,
                                                                    theForceBigTiffFlag);
      return boolProperty;     
   }
   else if(name == "output_tile_size")
   {
      ossimStringProperty* stringProp =
         new ossimStringProperty(name,
                                 ossimString::toString(theOutputTileSize.x),
                                 false); // editable flag
      stringProp->addConstraint(ossimString("16"));
      stringProp->addConstraint(ossimString("32"));
      stringProp->addConstraint(ossimString("64"));
      stringProp->addConstraint(ossimString("128"));
      stringProp->addConstraint(ossimString("256"));      
      stringProp->addConstraint(ossimString("512"));      
      stringProp->addConstraint(ossimString("1024"));      
      stringProp->addConstraint(ossimString("2048"));      
      return stringProp;
     
   }
   return ossimImageFileWriter::getProperty(name);
}

void ossimTiffWriter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(ossimString(
                              ossimKeywordNames::COMPRESSION_QUALITY_KW));
   propertyNames.push_back(ossimString(
                              ossimKeywordNames::COMPRESSION_TYPE_KW));
   propertyNames.push_back(ossimString("lut_file"));
   propertyNames.push_back(ossimString("color_lut_flag"));
   propertyNames.push_back(ossimString("big_tiff_flag"));
   propertyNames.push_back(ossimString("output_tile_size"));
  
   ossimImageFileWriter::getPropertyNames(propertyNames);
}

bool ossimTiffWriter::isOpen()const
{
   return (theTif!=NULL);
}

bool ossimTiffWriter::open()
{
   if(theTif)
   {
      closeTiff();
   }
   return openTiff();
}

void ossimTiffWriter::close()
{
   closeTiff();
}

void ossimTiffWriter::setJpegQuality(ossim_int32 quality)
{
   // Range 1 to 100 with 100 being best.
   if (quality > 0 && quality < 101)
   {
      theJpegQuality = quality;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimTiffWriter::setJpegQuality DEBUG:"
            << "\nquality out of range:  " << quality
            << "\nquality has been set to default:  " 
            << DEFAULT_JPEG_QUALITY
            << "\nvalid range:  1 to 100 with 100 being best."
            << std::endl;
      }
      
      theJpegQuality = DEFAULT_JPEG_QUALITY;
   }
}

ossim_int32 ossimTiffWriter::getJpegQuality()const
{
   
   return theJpegQuality;
}

void ossimTiffWriter::setCompressionType(const ossimString& type)
{
   theCompressionType = type;
}

ossimString ossimTiffWriter::getCompressionType()const
{
   return theCompressionType;
}

bool ossimTiffWriter::getGeotiffFlag()const
{
   return theOutputGeotiffTagsFlag;
}

void ossimTiffWriter::setGeotiffFlag(bool flag)
{
   theOutputGeotiffTagsFlag = flag;
}

ossimIpt ossimTiffWriter::getOutputTileSize()const
{
   return theOutputTileSize;
}

ossim_int32 ossimTiffWriter::setProjectionInfo(const ossimMapProjectionInfo& proj)
{
   theProjectionInfo = new ossimMapProjectionInfo(proj);

   return ossimErrorCodes::OSSIM_OK;
}


void ossimTiffWriter::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   imageTypeList.push_back(ossimString("tiff_strip"));
   imageTypeList.push_back(ossimString("tiff_strip_band_separate"));
   imageTypeList.push_back(ossimString("tiff_tiled"));
   imageTypeList.push_back(ossimString("tiff_tiled_band_separate"));
}

ossimString ossimTiffWriter::getExtension() const
{
   return ossimString("tif");
}

bool ossimTiffWriter::hasImageType(const ossimString& imageType) const
{
   // check for non image type list types
   // We will support mime type
   //
   if((imageType == "image/tiff")||
      (imageType == "image/gtiff")||
      (imageType == "image/tif")||
      (imageType == "image/gtif"))
   {
      return true;
   }

   return ossimImageFileWriter::hasImageType(imageType);
}

bool ossimTiffWriter::isLutEnabled()const
{
   return (theColorLutFlag);
}

ossimTiffWriter::UnitType ossimTiffWriter::getUnitType(
   ossim_int32 pcsCode,
   const ossimString& projName) const
{


   if ( ( projName == "ossimCylEquAreaProjection" ) ||
        ( projName == "ossimEquDistCylProjection" ) ||
        ( projName == "ossimLlxyProjection" ) )
   {
      return ANGULAR_DEGREES;
   } 

   UnitType pcsUnits = getPcsUnitType(pcsCode);

   UnitType type = UNDEFINED;
   
   switch (theLinearUnits)
   {
      case OSSIM_METERS:
      {
         type = LINEAR_METER;
         break;
      }
      
      case OSSIM_FEET:
      {
         type = LINEAR_FOOT;
         break;
      }
      
      case OSSIM_US_SURVEY_FEET:
      {
         type = LINEAR_FOOT_US_SURVEY;
         break;
      }
      default:
      {
         break;
      }
   }
   if (type == UNDEFINED)
   {
      return pcsUnits;
   }
   return type;
}

ossimTiffWriter::UnitType ossimTiffWriter::getPcsUnitType(ossim_int32 pcsCode) const
{
   UnitType pcsUnits = UNDEFINED;
   
   ossimRefPtr<ossimMapProjection> proj = PTR_CAST(ossimMapProjection, 
      ossimEpsgProjectionDatabase::instance()->findProjection((ossim_uint32) pcsCode));
   
   if (proj.valid())
   {
      ossimUnitType type = proj->getProjectionUnits();
      if (type == OSSIM_METERS)
      {
         pcsUnits = LINEAR_METER;
      }
      else
      {
         pcsUnits = LINEAR_FOOT_US_SURVEY;
      }
   }
   return pcsUnits;
}

//*************************************************************************************************
// Will take an ossimIMageData tile and write it to disk as a general raster file.
//*************************************************************************************************
void ossimTiffWriter::dumpTileToFile(ossimRefPtr<ossimImageData> t,  const ossimFilename& f)
{
   ossimRefPtr<ossimMemoryImageSource> tile = new ossimMemoryImageSource;
   tile->setImage(t);
   ossimRefPtr<ossimScalarRemapper> remapper = new ossimScalarRemapper(tile.get(), OSSIM_UINT8);
   ossimRefPtr<ossimTiffWriter> writer = new ossimTiffWriter();
   writer->connectMyInputTo(0, remapper.get());
   writer->setFilename(f);
   writer->setGeotiffFlag(false);
   writer->execute();
   writer=0;
   tile=0;
}
