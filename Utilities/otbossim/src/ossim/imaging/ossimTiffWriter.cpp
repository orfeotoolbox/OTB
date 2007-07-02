//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerda@home.com)
//
//*******************************************************************
//  $Id: ossimTiffWriter.cpp 9257 2006-07-14 15:31:02Z dburken $

#include <algorithm>
#include <sstream>

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
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimFilenameProperty.h>

static ossimTrace traceDebug("ossimTiffWriter:debug");

#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
#    include <xtiffio.h>
#    include <geotiff.h>
#    include <geo_normalize.h>
#    include <geovalues.h>
#  endif
#endif

static const char* TIFF_WRITER_OUTPUT_TILE_SIZE_X_KW = "output_tile_size_x";
static const char* TIFF_WRITER_OUTPUT_TILE_SIZE_Y_KW = "output_tile_size_y";
static const int   PCS_BRITISH_NATIONAL_GRID = 27700;
static const long  DEFAULT_JPEG_QUALITY = 75;

RTTI_DEF1(ossimTiffWriter, "ossimTiffWriter", ossimImageFileWriter);

ossimTiffWriter::ossimTiffWriter()
   :
      ossimImageFileWriter(),
      theTif(NULL),
      theCompressionType("none"),
      theJpegQuality(DEFAULT_JPEG_QUALITY),
      theOutputGeotiffTagsFlag(true),
      theImagineNad27Flag(false),
      theColorLutFlag(false),
      theProjectionInfo(NULL),
      theOutputTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT)
{
   ossimGetDefaultTileSize(theOutputTileSize);
   theOutputImageType = "tiff_tiled_band_separate";
}

ossimTiffWriter::~ossimTiffWriter()
{
   if(isOpen())
   {
      closeTiff();
   }
   if(theProjectionInfo)
   {
      delete theProjectionInfo;
      theProjectionInfo = NULL;
   }
}

bool ossimTiffWriter::openTiff()
{
   static const char* MODULE = "ossimTiffWriter::openTiff()";

   if (theTif) // Close the existing file pointer.
   {
      
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF 
      XTIFFClose( theTif );
#  else
      TIFFClose( theTif );
#  endif
#else
      TIFFClose( theTif );   
#endif

   }

   // Check for empty file name.
   if (theFilename.empty())
   {
      return false;
   }

   // Open the new file.
#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF
   theTif = XTIFFOpen( theFilename.c_str(), "w" );
#  else
   theTif = TIFFOpen( theFilename.c_str(), "w" );
#  endif
#else
   theTif = TIFFOpen( theFilename.c_str(), "w" );
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
      XTIFFClose( theTif );
#  else
      TIFFClose( theTif );
#  endif
#else
      TIFFClose( theTif );
#endif
      theTif = NULL;
   }

   return true;
}

bool ossimTiffWriter::writeTiffTags()
{
   static const char MODULE[] = "ossimTiffWriter::writeTiffTags";

   if (!theTif)
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
   TIFFSetField( theTif, TIFFTAG_BITSPERSAMPLE, bitsPerSample );
   TIFFSetField( theTif, TIFFTAG_SAMPLEFORMAT, sampleFormat );

   // Set the image dimensions.
   ossim_uint32  width  = theAreaOfInterest.width();
   ossim_uint32  height = theAreaOfInterest.height();
   TIFFSetField( theTif, TIFFTAG_IMAGEWIDTH, width);
   TIFFSetField( theTif, TIFFTAG_IMAGELENGTH, height);
   if (isTiled())
   {
      ossim_uint32 tileXSize = theOutputTileSize.x;
      ossim_uint32 tileYSize = theOutputTileSize.y;
      TIFFSetField(theTif, TIFFTAG_TILEWIDTH,  tileXSize);
      TIFFSetField(theTif, TIFFTAG_TILELENGTH, tileYSize);
   }
   else
   {
      TIFFSetField(theTif, TIFFTAG_ROWSPERSTRIP, ossim_uint32(1));
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
      TIFFSetField( theTif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
   }
   else
   {
      TIFFSetField( theTif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
   }

   // Set the compression type:
   uint16 tiffCompressType = COMPRESSION_NONE;
   theCompressionType.downcase();
   if( theCompressionType == "jpeg")
   {
      tiffCompressType  = COMPRESSION_JPEG;

      // Set the jpeg quality.
      TIFFSetField( theTif, TIFFTAG_JPEGQUALITY,  theJpegQuality);
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
   TIFFSetField( theTif, TIFFTAG_COMPRESSION, tiffCompressType);
   TIFFSetField(theTif, TIFFTAG_SAMPLESPERPIXEL, (int)theInputConnection->getNumberOfOutputBands());

   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   bool lutEnabled = (theColorLutFlag&&
                      ((scalarType == OSSIM_UINT8)||
                       (scalarType == OSSIM_UINT16)||
                       (scalarType == OSSIM_USHORT11))&&
                      (theColorLut.getNumberOfEntries() > 0)&&
                      (theInputConnection->getNumberOfOutputBands() == 1));
   if(lutEnabled)
   {
      TIFFSetField( theTif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE );
      TIFFSetField( theTif, TIFFTAG_INDEXED, (ossim_uint16)1);

      if(scalarType == OSSIM_UINT8)
      {
         ossim_uint16 r[256], g[256], b[256];
         
         memset(r, '\0', sizeof(ossim_uint16)*256);
         memset(g, '\0', sizeof(ossim_uint16)*256);
         memset(b, '\0', sizeof(ossim_uint16)*256);
         
         for(ossim_uint32 i = 0; i < theColorLut.getNumberOfEntries(); i++)
         {
            r[i] = (ossim_uint16) ((theColorLut[i][0]/255.0)*65535);
            g[i] = (ossim_uint16) ((theColorLut[i][1]/255.0)*65535);
            b[i] = (ossim_uint16) ((theColorLut[i][2]/255.0)*65535);
         }
         TIFFSetField(theTif, TIFFTAG_COLORMAP, r, g ,b);
      }
      else
      {
         ossim_uint16 r[65536], g[65536], b[65536];
         memset(r, '\0', sizeof(ossim_uint16)*65536);
         memset(g, '\0', sizeof(ossim_uint16)*65536);
         memset(b, '\0', sizeof(ossim_uint16)*65536);
         
         for(ossim_uint32 i = 0; i < theColorLut.getNumberOfEntries(); i++)
         {
            r[i] = (ossim_uint16) (theColorLut[i][0]);
            g[i] = (ossim_uint16) (theColorLut[i][1]);
            b[i] = (ossim_uint16) (theColorLut[i][2]);
         }
         TIFFSetField(theTif, TIFFTAG_COLORMAP, r, g ,b);
      }
   }
   else if( (theInputConnection->getNumberOfOutputBands() == 3 ||
             theInputConnection->getNumberOfOutputBands() == 4 ||
             (thePhotoMetric == "rgb"))&&
            (scalarType == OSSIM_UCHAR))
   {
      TIFFSetField( theTif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   }
   else
   {
      TIFFSetField( theTif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
   }

   return true;
}

bool ossimTiffWriter::writeGeotiffTags(ossimMapProjectionInfo* projectionInfo)
{
   static const char MODULE[] = "ossimTiffWriter::writeGeotiffTags";
	
   if (!theTif)
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

   if (!projectionInfo)
   {
      return false;
   }

#ifdef OSSIM_HAS_GEOTIFF
#  if OSSIM_HAS_GEOTIFF

   GTIF* gtif = GTIFNew(theTif);
   
   // Get a pointer to the projection.
   const ossimMapProjection* proj = projectionInfo->getProjection();
   if (!proj)
   {
      return false;
   }

   // Get some things we need thoughout.
   ossimGpt origin      = proj->origin();
   double falseEasting  =  proj->getFalseEasting();
   double falseNorthing =  proj->getFalseNorthing();
   
   ossimKeywordlist kwl;
   proj->saveState(kwl);
   const char* stdParallel1 = kwl.find(ossimKeywordNames::STD_PARALLEL_1_KW);
   const char* stdParallel2 = kwl.find(ossimKeywordNames::STD_PARALLEL_2_KW);
   const char* scaleFactor  = kwl.find(ossimKeywordNames::SCALE_FACTOR_KW);

   // Set the pixel type.  This will shift the tie point correctly for us.
   projectionInfo->setPixelType(thePixelType);

   //---
   // Since using a pcs code is the easiest way to go, look for that first.
   //---
   bool isStatePlane = false;
   ossim_int16 pcsCode = proj->getPcsCode();
   if (pcsCode)
   {
      isStatePlane = true;
   }
   else // Make pcs code from utm.
   {
      // Look in the pcs factory.
      pcsCode = ossimPcsCodeProjectionFactory::instance()->
         getPcsCodeFromProjection(proj);
   }

   //---
   // Get the units now.  If user has overriden pcs units then go user defined
   // projection by setting pcs code to 0.
   //---
   ossimString projName = proj->getClassName();
   UnitType units = getUnitType(pcsCode, projName);
   if (isStatePlane)
   {
      if (units != getPcsUnitType(pcsCode))
      {
         //---
         // State plane pcs codes imply units, so if user overroad with
         // theLinearUnits make the projection user defined by setting the
         // pcs code to 0.
         //---
         pcsCode = 0;
      }
   }

   if (units == UNDEFINED)
   {
      units = LINEAR_METER;
   }
   
   if (pcsCode)
   {
      GTIFKeySet(gtif,
                 ProjectedCSTypeGeoKey,
                 TYPE_SHORT,
                 1,
                 pcsCode);
   }
   
   if (traceDebug())
   {
      CLOG << " DEBUG:\n"
           << "Geotiff ProjectedCSTypeGeoKey:  "
           << pcsCode
           << std::endl;
   }
   
   //---
   // Set the model type and units.
   //---
   if (units == ANGULAR_DEGREES)
   {
      GTIFKeySet(gtif,
                 GTModelTypeGeoKey,
                 TYPE_SHORT,
                 1,
                 ModelTypeGeographic);

      // Set the units key.
      GTIFKeySet(gtif,
                 GeogAngularUnitsGeoKey,
                 TYPE_SHORT,
                 1,
                 units);
   }
   else
   {
      GTIFKeySet(gtif,
                 GTModelTypeGeoKey,
                 TYPE_SHORT,
                 1,
                 ModelTypeProjected);

      // Set the units key.
      GTIFKeySet(gtif,
                 ProjLinearUnitsGeoKey,
                 TYPE_SHORT,
                 1,
                 units);
   }
   

   // Set the pixel type.
   if (thePixelType == OSSIM_PIXEL_IS_POINT)
   {
      // Tie point relative to center of pixel.
      GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsPoint);
   }
   else
   {
      // Tie point relative to upper left corner of pixel
      GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
   }

   //---
   // Set the tie point and scale.
   //---
   double   tiePoints[6]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
   double   pixScale[3]   = { 0.0, 0.0, 0.0 };
   switch (units)
   {
      case LINEAR_FOOT:
      {
         tiePoints[3]  = mtrs2ft(projectionInfo->ulEastingNorthingPt().x);
         tiePoints[4]  = mtrs2ft(projectionInfo->ulEastingNorthingPt().y);
         pixScale[0]   = mtrs2ft(projectionInfo->getMetersPerPixel().x);
         pixScale[1]   = mtrs2ft(projectionInfo->getMetersPerPixel().y);
         falseEasting  = mtrs2ft(falseEasting);
         falseNorthing = mtrs2ft(falseNorthing);
         
         break;
      }
      case LINEAR_FOOT_US_SURVEY:
      {
         tiePoints[3]  = mtrs2usft(projectionInfo->ulEastingNorthingPt().x);
         tiePoints[4]  = mtrs2usft(projectionInfo->ulEastingNorthingPt().y);
         pixScale[0]   = mtrs2usft(projectionInfo->getMetersPerPixel().x);
         pixScale[1]   = mtrs2usft(projectionInfo->getMetersPerPixel().y);
         falseEasting  = mtrs2usft(falseEasting);
         falseNorthing = mtrs2usft(falseNorthing);
         break;
      }
      case ANGULAR_DEGREES:
      {
         tiePoints[3] = projectionInfo->ulGroundPt().lond();
         tiePoints[4] = projectionInfo->ulGroundPt().latd();
         pixScale[0]  = projectionInfo->getDecimalDegreesPerPixel().x;
         pixScale[1]  = projectionInfo->getDecimalDegreesPerPixel().y;
         break;
      }
      case LINEAR_METER:
      default:
      {
         tiePoints[3] = projectionInfo->ulEastingNorthingPt().x;
         tiePoints[4] = projectionInfo->ulEastingNorthingPt().y;
         pixScale[0]  = projectionInfo->getMetersPerPixel().x;
         pixScale[1]  = projectionInfo->getMetersPerPixel().y;
         break;
      }
         
   } // End of "switch (units)"
   
   TIFFSetField( theTif, TIFFTAG_GEOTIEPOINTS, 6, tiePoints );
   TIFFSetField( theTif, TIFFTAG_GEOPIXELSCALE, 3, pixScale );

   
   ossimString datumCode = "WGE";
   ossimString datumName = "WGE";
   // Attemp to get the datum code
   const ossimDatum* datum = proj->getDatum();
   if(datum)
   {
      datumCode = datum->code();
      datumName = datum->name();
   }

   short gcs = USER_DEFINED;

   if (datumCode == "WGE") gcs = GCS_WGS_84;
   else if (datumCode == "WGD") gcs = GCS_WGS_72;
   else if (datumCode == "NAR-C") gcs = GCS_NAD83;
   else if (datumCode == "NAR") gcs = GCS_NAD83;
   else if (datumCode == "NAS-C") gcs = GCS_NAD27;
   else if (datumCode == "NAS") gcs = GCS_NAD27;
   else if (datumCode == "ADI-M") gcs = GCS_Adindan;
   else if (datumCode == "ARF-M") gcs = GCS_Arc_1950;
   else if (datumCode == "ARS-M") gcs = GCS_Arc_1960;
   else if (datumCode == "EUR-7" || datumCode == "EUR-M") gcs = GCS_ED50;
   else if ((datumCode == "OGB-7") ||
            (datumCode == "OGB-M") ||
            (datumCode == "OGB-A") ||
            (datumCode == "OGB-B") ||
            (datumCode == "OGB-C") ||
            (datumCode == "OGB-D")) gcs = GCS_OSGB_1936;
   else if (datumCode == "TOY-M") gcs = GCS_Tokyo;
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DATUM = " << datumCode << " tag not written " << std::endl
            << "Please let us know so we can add it"          << std::endl;
      }
   }

   // ***
   // ERDAS Imagine < v8.7 has a NAD27 Conus Bug.  They are not using the
   // proper GCS code.  They use user-defined fields and Geog cititaion tag to
   // define.  Sucks!  It is an open issue at Leica.  This is a work around
   // flag for this issue.
   // ***
   if((datumCode == "NAS-C") && theImagineNad27Flag)
   {
      gcs = USER_DEFINED;

      std::ostringstream os;
      os << "IMAGINE GeoTIFF Support\nCopyright 1991 -  2001 by ERDAS, Inc. All Rights Reserved\n@(#)$RCSfile$ $Revision: 9257 $ $Date: 2006-07-14 17:31:02 +0200 (ven, 14 jui 2006) $\nUnable to match Ellipsoid (Datum) to a GeographicTypeGeoKey value\nEllipsoid = Clarke 1866\nDatum = NAD27 (CONUS)";

      GTIFKeySet(gtif,
                 GeogCitationGeoKey,
                 TYPE_ASCII,
                 1,
                 os.str().c_str());

      // User-Defined
      GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1,
                 KvUserDefined );
      // User-Defined
      GTIFKeySet(gtif, GeogEllipsoidGeoKey, TYPE_SHORT, 1,
                 KvUserDefined );
   }
   else
   {
      GTIFKeySet( gtif, GeographicTypeGeoKey, TYPE_SHORT, 1, gcs );
   }

   // Set the ellipsoid major/minor axis.
   GTIFKeySet(gtif,
              GeogSemiMajorAxisGeoKey,
              TYPE_DOUBLE,
              1,
              proj->getA());

   GTIFKeySet(gtif,
              GeogSemiMinorAxisGeoKey,
              TYPE_DOUBLE,
              1,
              proj->getB());

   // Write the projection parameters.

   bool setFalseEastingNorthingFlag = false;
   
   if ( (projName == "ossimUtmProjection") && !pcsCode )
   {
      //---
      // UTM tags needed example from the geo tiff spec page:
      // ModelTiepointTag       = (0, 0, 0,  350807.4, 5316081.3, 0.0)
      // ModelPixelScaleTag     = (100.0, 100.0, 0.0)
      // GeoKeyDirectoryTag:
      //       GTModelTypeGeoKey        =  1      (ModelTypeProjected)
      //       GTRasterTypeGeoKey       =  1      (RasterPixelIsArea)
      //       ProjectedCSTypeGeoKey    =  32660  (PCS_WGS84_UTM_zone_60N)
      //       PCSCitationGeoKey        =  "UTM Zone 60 N with WGS84"
      //
      // NOTE:
      // The "ProjectedCSTypeGeoKey" can be constructed using the map zone
      // and the datum.
      //---
      const ossimUtmProjection* utmProjection
         = PTR_CAST(ossimUtmProjection, proj);

      // Attempt to get the pcs key.
      int mapZone = utmProjection->getZone();
      ossimString hemisphere = utmProjection->getHemisphere();
      short projSysCode=0;

      //---
      // Use a projection code that does not imply a datum.
      // See section "6.3.3.2 Projection Codes" for definition.
      //---
      if (mapZone > 0) // Northern hemisphere.
      {
         projSysCode = 16000 + mapZone;
      }
      else if (mapZone < 0) // Southern hemisphere.
      {
         hemisphere = "S";
         projSysCode = 16100 + abs(mapZone);
      }
      
      // Set the Projected Coordinate System Type to be user defined.
      GTIFKeySet(gtif,
                 ProjectedCSTypeGeoKey,
                 TYPE_SHORT,
                 1,
                 USER_DEFINED);
      
      // Set the geographic type to be user defined.
      GTIFKeySet(gtif,
                 GeographicTypeGeoKey,
                 TYPE_SHORT,
                 1,
                 USER_DEFINED);
      
      // Set the ProjectionGeoKey in place of the ProjectedCSTypeGeoKey.
      GTIFKeySet(gtif,
                 ProjectionGeoKey,
                 TYPE_SHORT,
                 1,
                 projSysCode);
   
      if (traceDebug())
      {
         CLOG << " DEBUG:\n"
              << "Geotiff ProjectedCSTypeGeoKey:  "
              << pcsCode
              << std::endl;
      }
      
      std::ostringstream os;
      os << "UTM Zone " << dec << mapZone << hemisphere.c_str()
         << " with " << datumName << " datum";
      
      GTIFKeySet(gtif,
                 PCSCitationGeoKey,
                 TYPE_ASCII,
                 1,
                 os.str().c_str());
      
   } // End of "if ( (projName == "ossimUtmProjection") && !pcsCode )
   
   else if(projName == "ossimBngProjection")
   {
      // User-Defined
      GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
		 PCS_BRITISH_NATIONAL_GRID);//KvUserDefined );
      
      // User-Defined
      GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1,
		 KvUserDefined );
      
      GTIFKeySet(gtif,
                 PCSCitationGeoKey,
                 TYPE_ASCII,
                 26,
                 "PCS_British_National_Grid");
      
      GTIFKeySet(gtif,
                 ProjCoordTransGeoKey,
                 TYPE_SHORT,
                 1,
                 (uint16)CT_TransverseMercator);
      
      GTIFKeySet(gtif,
		 ProjNatOriginLongGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.lond());
      
      GTIFKeySet(gtif,
		 ProjNatOriginLatGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.latd());

      setFalseEastingNorthingFlag = true;

      double scale = ossimString(scaleFactor).toDouble();

      GTIFKeySet(gtif,
                 ProjScaleAtNatOriginGeoKey,
                 TYPE_DOUBLE,
                 1,
                 scale);
   }
   else if( (projName == "ossimEquDistCylProjection")||
            (projName == "ossimLlxyProjection"))
   {
      GTIFKeySet(gtif,
		 ProjNatOriginLongGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.lond());
      
      GTIFKeySet(gtif,
		 ProjNatOriginLatGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.latd());
   }
   else if ( ( (projName == "ossimLambertConformalConicProjection") &&
               (!pcsCode)) ||
             (projName == "ossimAlbersProjection") )
   {
      //---
      // Lambert Conformal Conic:
      // tags needed example from the geo tiff spec page:
      // ModelTiepointTag     = (  80,  100, 0,  200000,  1500000, 0)
      // ModelPixelScaleTag         = (1000, 1000, 0)
      // GeoKeyDirectoryTag:
      //       GTModelTypeGeoKey           =  1     (ModelTypeProjected)
      //       GTRasterTypeGeoKey          =  1     (RasterPixelIsArea)
      //       GeographicTypeGeoKey        =  4267  (GCS_NAD27)
      //       ProjectedCSTypeGeoKey       =  32767 (user-defined)
      //       ProjectionGeoKey            =  32767 (user-defined)
      //       ProjLinearUnitsGeoKey       =  9001     (Linear_Meter)
      //       ProjCoordTransGeoKey        =  8 (CT_LambertConfConic_2SP)
      //            ProjStdParallel1GeoKey     =  41.333
      //            ProjStdParallel2GeoKey     =  48.666
      //            ProjCenterLongGeoKey       =-120.0
      //            ProjNatOriginLatGeoKey     =  45.0
      //            ProjFalseEastingGeoKey,    = 200000.0
      //            ProjFalseNorthingGeoKey,   = 1500000.0
      //
      // NOTE: Albers Same as Lambert with the exception of the
      //       ProjCoordTransGeoKey which is:  CT_AlbersEqualArea.
      //---
      
      if (projName == "ossimLambertConformalConicProjection")
      {
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_LambertConfConic_2SP );
      }
      else // Albers
      {
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_AlbersEqualArea);
      }
      
      // User-Defined
      GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                 KvUserDefined );

      // User-Defined
      GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1,
		 KvUserDefined );

      double phi1 = ossimString(stdParallel1).toDouble();

      GTIFKeySet(gtif,
		 ProjStdParallel1GeoKey,
		 TYPE_DOUBLE,
		 1,
		 phi1);  // 1st parallel

      double phi2 = ossimString(stdParallel2).toDouble();

      GTIFKeySet(gtif,
		 ProjStdParallel2GeoKey,
		 TYPE_DOUBLE,
		 1,
		 phi2);  // 2nd parallel

      GTIFKeySet(gtif,
		 ProjCenterLongGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.lond());  // Longitude at the origin.

      GTIFKeySet(gtif,
		 ProjNatOriginLatGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.latd());  // Origin

      setFalseEastingNorthingFlag = true;

   }  // End of Lambert.


   else if ( (projName == "ossimTransMercatorProjection") && !pcsCode )
   {
      //---
      // Transverse Mercator ( no example in the geo tiff spec.
      // Requires:
      //    - latitude/longitude of the origin
      //    - easting/northing of some tie point(line/sample 0,0)
      //    - false easting/northing
      //    - The scale factor.
      //---

      //---
      // The easting/northing is the distance from the origin plus the
      // false easting/northing.  In other words if line 0 is 5,000
      // meters from the origin and the false northing is 5,000 meters,
      // then the northing would be 10,000.  The same goes for the easting.
      //---
      GTIFKeySet(gtif,
                 ProjCoordTransGeoKey,
                 TYPE_SHORT,
                 1,
                 (uint16)CT_TransverseMercator);
      
      // User-Defined
      GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1, KvUserDefined );
      
      // User-Defined
      GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1, KvUserDefined );
      
      GTIFKeySet(gtif,
		 ProjNatOriginLongGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.lond());

      GTIFKeySet(gtif,
		 ProjNatOriginLatGeoKey,
		 TYPE_DOUBLE,
		 1,
		 origin.latd());

      setFalseEastingNorthingFlag = true;

      double scale = ossimString(scaleFactor).toDouble();

      GTIFKeySet(gtif,
                 ProjScaleAtNatOriginGeoKey,
                 TYPE_DOUBLE,
                 1,
                 scale);
   } // End of TM

   if (setFalseEastingNorthingFlag == true)
   {

      GTIFKeySet(gtif,
		 ProjFalseEastingGeoKey,
		 TYPE_DOUBLE,
		 1,
		 falseEasting);

      GTIFKeySet(gtif,
		 ProjFalseNorthingGeoKey,
		 TYPE_DOUBLE,
		 1,
		 falseNorthing);
   }
   
 

   GTIFWriteKeys(gtif); // Write out geotiff tags.
   GTIFFree(gtif);

   return true;
#  endif
#endif // #ifdef OSSIM_HAS_GEOTIFF

   return true;
}

bool ossimTiffWriter::writeFile()

{
   static const char MODULE[] = "ossimTiffWriter::writeFile";

   if (traceDebug()) CLOG << "Entered..." << std::endl;

   if(isLutEnabled())
   {
      theNBandToIndexFilter = new ossimNBandToIndexFilter;
      theNBandToIndexFilter->connectMyInputTo(0, theInputConnection->getInput());
      theNBandToIndexFilter->setLut(theColorLut);
      theNBandToIndexFilter->initialize();
      theInputConnection->disconnect();
      theInputConnection->connectMyInputTo(0, theNBandToIndexFilter.get());
      theInputConnection->initialize();
   }
   else
   {
      theNBandToIndexFilter = 0;
   }
   
   if (traceDebug() && theInputConnection)
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
      if(theViewController) // let this override
      {                     // found in ossimImageWriter base
         ossimMapProjection* proj = PTR_CAST(ossimMapProjection,
                                             theViewController->getView());
         if(proj)
         {
            ossimMapProjectionInfo* projectionInfo
               = new ossimMapProjectionInfo(proj, theAreaOfInterest);

            if (writeGeotiffTags(projectionInfo) == false)
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " ERROR:"
                     << "\nError detected writing geotiff tags.  Returning..."
                     << std::endl;
               }
               delete projectionInfo;
               return false;
            }

            delete projectionInfo;
         }
      }
      else if(theProjectionInfo)
      {
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
         ossimKeywordlist kwl;
         theInputConnection->getImageGeometry(kwl);
         
         ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->
            createProjection(kwl);
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);
         if(mapProj)
         {
            ossimMapProjectionInfo* projectionInfo
               = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);

            if (writeGeotiffTags(projectionInfo) == false)
            {
               if(traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " ERROR:"
                     << "\nError detected writing geotiff tags.  Returning..."
                     << std::endl;
               }
               if(proj)
               {
                  delete proj;
               }
               delete projectionInfo;
               return false;
            }

            delete projectionInfo;

         }
         if(proj)
         {
            delete proj;
            proj = NULL;
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
   theColorLut = lut;
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
         theColorLut.saveState(kwl, newPrefix.c_str());
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
   ossimGetDefaultTileSize(defaultTileSize);

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

   const char* flag              = kwl.find(prefix, "output_geotiff_flag");
   const char* img_nad27_flag    = kwl.find(prefix, "imagine_nad27_flag");

   if(flag)
   {
      theOutputGeotiffTagsFlag = ossimString(flag).toBool();
   }

   if(img_nad27_flag)
   {
      theImagineNad27Flag = ossimString(img_nad27_flag).toBool();
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
      theColorLut.open(theLutFilename);
   }
   else
   {
      theColorLut.loadState(kwl, newPrefix.c_str());
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

   if (traceDebug()) CLOG << " Entered." << std::endl;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossimRefPtr<ossimImageData> tempTile = 0;

   if(theColorLutFlag)
   {
      tempTile = ossimImageDataFactory::instance()->create(this, 1, theInputConnection);
   }
   else
   {
      tempTile = ossimImageDataFactory::instance()->create(this, theInputConnection);
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
   for(ossim_uint32 i = 0; i < tilesHigh; i++)
   {
      ossimIpt origin(0,0);
      origin.y = i * tileHeight;

      // Tile loop in the sample (width) direction.
      for(ossim_uint32 j = 0; j < tilesWide; j++)
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
            if(!theColorLutFlag)
            {
               id->computeMinMaxPix(minBands, maxBands);
            }
         }

         //---
         // Write the tile to disk.
         //---
         ossim_uint32 bytesWritten = 0;
         bytesWritten = TIFFWriteTile(theTif,
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

   if(!theColorLutFlag)
   {
      writeMinMaxTags(minBands, maxBands);
   }

   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

bool ossimTiffWriter::writeToTilesBandSep()
{
   static const char* const MODULE = "ossimTiffWriter::writeToTilesBandSep";

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
      for(ossim_uint32 j = 0; ((j < tilesWide)&!needsAborting()); ++j)
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
            bytesWritten = TIFFWriteTile(theTif,
                                         data,
                                         (ossim_uint32)origin.x,
                                         (ossim_uint32)origin.y,
                                         (ossim_uint32)0,        // z
                                         (tsample_t)band);    // sample

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

   if(!theColorLutFlag)
   {
      writeMinMaxTags(minBands, maxBands);
   }
    
   if (traceDebug()) CLOG << " Exited." << std::endl;

   return true;
}

bool ossimTiffWriter::writeToStrips()
{
   static const char* const MODULE = "ossimTiffWriter::writeToStrips";

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
      ossimGetScalarSizeInBytes(theInputConnection->getOutputScalarType()) *
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
        ossim_int32 status = TIFFWriteScanline(theTif,
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
      ossimGetScalarSizeInBytes(theInputConnection->getOutputScalarType()) *
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
           ossim_int32 status = TIFFWriteScanline(theTif,
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
   if (tileSize.x % 32 || tileSize.y % 32)
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
            TIFFSetField( theTif, TIFFTAG_MINSAMPLEVALUE,
                          static_cast<ossim_sint16>(0) );
            TIFFSetField( theTif, TIFFTAG_MAXSAMPLEVALUE,
                          static_cast<ossim_sint16>(2047) );
            break;
         }
         case OSSIM_UINT8:
         case OSSIM_UINT16:
         {
            TIFFSetField( theTif, TIFFTAG_MINSAMPLEVALUE,
                          static_cast<ossim_sint16>(minValue) );
            TIFFSetField( theTif, TIFFTAG_MAXSAMPLEVALUE,
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
            TIFFSetField( theTif, TIFFTAG_SMINSAMPLEVALUE,
                          static_cast<ossim_float32>(minValue) );
            TIFFSetField( theTif, TIFFTAG_SMAXSAMPLEVALUE,
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
      ossimNumericProperty* numericProperty = PTR_CAST(ossimNumericProperty,
                                                       property.get());
      if (numericProperty)
      {
         setJpegQuality( numericProperty->asInt32() );
      }
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
      theColorLut.open(theLutFilename);
   }
   else if(property->getName() == "color_lut_flag")
   {
      theColorLutFlag = property->valueToString().toBool();
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
   if(theProjectionInfo)
   {
      delete theProjectionInfo;
   }
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

ossimTiffWriter::UnitType ossimTiffWriter::getPcsUnitType(
   ossim_int32 pcsCode) const
{
   UnitType pcsUnits = UNDEFINED;
   
   if (!pcsCode)
   {
      return pcsUnits;
   }
   
   const ossimStatePlaneProjectionInfo* info =
      ossimStatePlaneProjectionFactory::instance()->getInfo(pcsCode);
   if (info)
   {
      ossimUnitType type = info->getUnitType();
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


