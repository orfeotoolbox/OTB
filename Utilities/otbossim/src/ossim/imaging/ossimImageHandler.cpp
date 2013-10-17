//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// This class defines an abstract image handler which all loaders should
// derive from.
//
//*******************************************************************
//  $Id: ossimImageHandler.cpp 22228 2013-04-12 14:11:45Z dburken $

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimContainerEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <algorithm>

RTTI_DEF1(ossimImageHandler, "ossimImageHandler", ossimImageSource)

// Static trace for debugging
static ossimTrace traceDebug("ossimImageHandler:debug");

// Property keywords.
static const char HAS_LUT_KW[]                 = "has_lut";
static const char OPEN_OVERVIEW_FLAG_KW[]      = "open_overview_flag";
static const char START_RES_LEVEL_KW[]         = "start_res_level";
static const char SUPPLEMENTARY_DIRECTORY_KW[] = "supplementary_directory";
static const char VALID_VERTICES_FILE_KW[]     = "valid_vertices_file";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageHandler.cpp 22228 2013-04-12 14:11:45Z dburken $";
#endif

// GARRETT! All of the decimation factors are scattered throughout. We want to fold that into 
// the image geometry object. The code is ready in ossimImageGeometry for this (more or less), but
// the ossimImageGeometry::m_decimationFactors is not being set nor referenced. Can you do this?
// I'm a little foggy on how we want to incorporate R-level scaling into the geometry object.
   
ossimImageHandler::ossimImageHandler()
:
ossimImageSource(0, 0, 0, true, false /* output list is not fixed */ ),
theImageFile(ossimFilename::NIL),
theOverviewFile(ossimFilename::NIL),
theSupplementaryDirectory(ossimFilename::NIL),
theOverview(0),
theValidImageVertices(0),
theMetaData(),
theGeometry(),
theLut(0),
theDecimationFactors(0),
theImageID(""),
theStartingResLevel(0),
theOpenOverviewFlag(true),
thePixelType(OSSIM_PIXEL_IS_POINT)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandler::ossimImageHandler() DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  "
         << OSSIM_ID
         << std::endl;
#endif      
   }
}

ossimImageHandler::~ossimImageHandler()
{
   theOverview = 0;
}

void ossimImageHandler::initialize()
{
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimImageHandler::saveState(ossimKeywordlist& kwl,
                                  const char* prefix) const
{
   static const char MODULE[] = "ossimImageHandler::saveState";

   ossimImageSource::saveState(kwl, prefix);
   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << " ERROR detected in keyword list!  State not saved."
         << std::endl;
      return false;
   }

   kwl.add(prefix, ossimKeywordNames::FILENAME_KW, theImageFile.c_str(), true);
   kwl.add(prefix, HAS_LUT_KW, (theLut.valid()?"true":"false"), true);
   kwl.add(prefix, ossimKeywordNames::IMAGE_ID_KW, theImageID, true);
   kwl.add(prefix, ossimKeywordNames::OVERVIEW_FILE_KW, theOverviewFile.c_str(), true);
   kwl.add(prefix, SUPPLEMENTARY_DIRECTORY_KW, theSupplementaryDirectory.c_str(), true);
   kwl.add(prefix, START_RES_LEVEL_KW, theStartingResLevel, true);
   kwl.add(prefix, OPEN_OVERVIEW_FLAG_KW, (theOpenOverviewFlag?"1":"0"), true);
   kwl.add(prefix, ossimKeywordNames::PIXEL_TYPE_KW, (ossim_uint16) thePixelType, true);

   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimImageHandler::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   static const char MODULE[] = "ossimImageHandler::loadState(kwl, prefix)";
   theDecimationFactors.clear();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: entered ..."
         << std::endl;
   }
   
   ossimImageSource::loadState(kwl, prefix);
   
   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << "WARNING: error detected in keyword list!  State not load."
         << std::endl;
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: "
            << "leaving with error in kwl ..." << std::endl;
      }
      return false;
   }
   
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if (!lookup)
   {
      // Deprecated...
      lookup = kwl.find(prefix, ossimKeywordNames::IMAGE_FILE_KW);
   }
   
   if (lookup)
   {
      theImageFile = lookup;
   }
   
   lookup = kwl.find(prefix, ossimKeywordNames::OVERVIEW_FILE_KW);
   if(lookup)
   {
      theOverviewFile = ossimFilename(lookup);
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:\n"
         << "filename = " << theImageFile << std::endl;
   }
   
   // Check for an external geometry file to initialize our image geometry object:
   lookup = kwl.find(prefix, ossimKeywordNames::GEOM_FILE_KW);
   if (lookup)
   {
      ossimKeywordlist geomKwl;
      if (geomKwl.addFile(lookup) == true)
      {
         theGeometry = new ossimImageGeometry;
         theGeometry->loadState(geomKwl);
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "Loaded external geometry file:  "
               << lookup
               << "\n" << geomKwl << std::endl;
         }
      }
   }
   
   // Check for an valid image vetices file.
   lookup = kwl.find(prefix, VALID_VERTICES_FILE_KW);
   if (lookup)
   {
      initVertices(lookup);
   }

   // Starting resolution level.
   lookup = kwl.find(prefix, START_RES_LEVEL_KW);
   if (lookup)
   {
      theStartingResLevel = ossimString(lookup).toUInt32();
   }

   // Open overview flag.
   lookup = kwl.find(prefix, OPEN_OVERVIEW_FLAG_KW);
   if (lookup)
   {
      setOpenOverviewFlag( ossimString(lookup).toBool() );
   }

   // The supplementary directory for finding the overview
   lookup = kwl.find(prefix, SUPPLEMENTARY_DIRECTORY_KW);
   if (lookup)
   {
      theSupplementaryDirectory = ossimFilename(lookup);
   }

   if(getNumberOfInputs())
   {
      theInputObjectList.clear();
   }
   theInputListIsFixedFlag = true;
   
   // Read image id if present:
   theImageID = kwl.find(prefix, ossimKeywordNames::IMAGE_ID_KW);

   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_TYPE_KW);
   if (lookup)
   {
      thePixelType = (ossimPixelType) atoi(lookup);
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: Leaving Ok ..."
         << std::endl;
   }
   return true;
}

bool ossimImageHandler::initVertices(const char* file)
{
   static const char MODULE[] = "ossimImageHandler::initVertices";

   ossimFilename f = file;
   if (!f.exists()) return false;

   ossimKeywordlist kwl(file);
   
   if (kwl.getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimImageHandler::initVertices NOTICE:"
            << "\nError reading valid vertice file:  " << file << std::endl;
      }
      return false;
   }

   // Clean out any old vertices...
   theValidImageVertices.clear();

   ossim_uint32 number_of_points = kwl.numberOf("point", "x");

   for (ossim_uint32 i=0; i<number_of_points; ++i)
   {
      ossimIpt pt;
      const char* lookup;
      ossimString p = "point";
      p += ossimString::toString(i);
      
      ossimString px = p + ".x";
      lookup = kwl.find(px.c_str());
      if (lookup)
      {
         pt.x = atoi(lookup);
      }
      else
      {
         if (traceDebug())
         {
            CLOG << " ERROR:"
                 << "\nlookup failed for:  " << px.c_str()
                 << "\nReturning..."
                 << std::endl;
         }
         return false;
      }
         
      ossimString py = p + ".y";
      lookup = kwl.find(py.c_str());
      if (lookup)
      {
         pt.y = atoi(lookup);
      }
      else
      {
         if (traceDebug())
         {
            CLOG << " ERROR:"
                 << "\nLookup failed for:  " << py.c_str()
                 << "\nReturning..."
                 << std::endl;
         }
         return false;
      }

      theValidImageVertices.push_back(pt);
   }

   
   if (traceDebug())
   {
      CLOG << " DEBUG:"
           << "\nVertices file:  " << f
           << "\nValid image vertices:"
           << std::endl;
      for (ossim_uint32 i=0; i<theValidImageVertices.size(); ++i)
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "\npoint[" << i << "]:  " << theValidImageVertices[i];
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << std::endl;
   }
   
   return true;
}

ossim_uint32 ossimImageHandler::getNumberOfDecimationLevels() const
{
   ossim_uint32 result = 1; // Add r0
   if (theOverview.valid())
   {
      result += theOverview->getNumberOfDecimationLevels();
   }
   return result;
}

ossim_uint32 ossimImageHandler::getNumberOfReducedResSets() const
{
   return getNumberOfDecimationLevels();
}

ossimIrect ossimImageHandler::getImageRectangle(ossim_uint32 resLevel) const
{
   ossimIrect result;
   
   if( isOpen() && isValidRLevel(resLevel) )
   {
      ossim_int32 lines   = getNumberOfLines(resLevel);
      ossim_int32 samples = getNumberOfSamples(resLevel);
      if( !lines || !samples )
      {
         result.makeNan();
      }
      else
      {
         result = ossimIrect(0, 0, samples-1, lines-1);
      }
   }
   else
   {
      result.makeNan();
   }

   return result;
}

ossimIrect ossimImageHandler::getBoundingRect(ossim_uint32 resLevel) const
{
   return getImageRectangle(resLevel);
}


void ossimImageHandler::getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const
{
   if (resLevel < theDecimationFactors.size())
      result = theDecimationFactors[resLevel];
   else
      result.makeNan();
}

void ossimImageHandler::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   decimations = theDecimationFactors;
}

//*************************************************************************************************
// Method determines the decimation factors at each resolution level. This
// base class implementation computes the decimation by considering the ratios in image size
// between resolution levels, with fuzzy logic for rounding ratios to the nearest power of 2
// if possible. Derived classes need to override this method if the decimations are provided
// as part of the image metadata. In some cases (cf. ossimNitfTileSource), the derived class can
// set the first R-level (R0) decimation in the case that it is not unity, and then invoke this
// base class implementation to compute remaining R-levels, respecting the R0 decimation previously
// set by derived class.
//*************************************************************************************************
void ossimImageHandler::establishDecimationFactors() 
{
   double line_decimation, samp_decimation, decimation, ratio;
   ossim_uint32 num_lines, num_samps;
   
   // The error margin here is effectively the percent error tolerated between ideal number of 
   // pixels for given power-of-2 decimation, and the actual number of pixels found at this 
   // decimation level. Typically, the last level will have fewer pixels than expected, hence the 
   // need for this logic...
   static const double ERROR_MARGIN = 1.1;  // 10% allowance

   // NOTE -- Until the end of this method, all decimation values are actually inverse quantities,
   // i.e., a decimation of 0.5 typical for rlevel 1 is represented here as 2. This facilitates the
   // fuzzy logic for recognizing powers of 2 (as integers)

   // Default implementation assumes R0 is not decimated. Check for R0 defined by derived class
   // however, in case this is not the case:
   ossimDpt decimation_r0 (1.0, 1.0);
   if (theDecimationFactors.size() > 0)
   {
      decimation_r0.x = 1.0/theDecimationFactors[0].x; // note use of inverse decimation
      decimation_r0.y = 1.0/theDecimationFactors[0].y; 
   }
   else
      theDecimationFactors.push_back(decimation_r0);

   // Variables used in loop below:
   ossim_uint32 num_lines_r0 = getNumberOfLines(0);
   ossim_uint32 num_samps_r0 = getNumberOfSamples(0);
   ossim_uint32 power_of_2_decimation = (ossim_uint32) decimation_r0.x;

   // Remaining res levels are computed as a ratio of image size at R0 to image size at Rn:
   ossim_uint32 nRlevels = getNumberOfDecimationLevels();
   for(ossim_uint32 res_level = 1; res_level < nRlevels; ++res_level)
   {
      num_lines = getNumberOfLines(res_level);
      num_samps = getNumberOfSamples(res_level);
      
      if ((num_lines < 2) || (num_samps < 2)) 
         break;

      line_decimation = decimation_r0.y * num_lines_r0 / (double)num_lines;
      samp_decimation = decimation_r0.x * num_samps_r0 / (double)num_samps;
      decimation = line_decimation<samp_decimation ? line_decimation:samp_decimation;

      // Check for possible error due to inexact ratios.
      // Loop until reasonable effort was made to establish the corresponding power-of-2 decimation.
      // If close match is found, the exact integer value is assigned:
      do 
      {
         power_of_2_decimation *= 2;
         ratio = (double)power_of_2_decimation / decimation;
         if (ratio < 1.0)
            ratio = 1.0/ratio;
         if (ratio < ERROR_MARGIN)
            decimation = (double) power_of_2_decimation;

      } while ((double) power_of_2_decimation < decimation);

      // Convert the computed decimation back to fractional form before saving in the factors list:
      decimation = 1.0/decimation;
      theDecimationFactors.push_back(ossimDpt(decimation, decimation));
   }
}


bool ossimImageHandler::buildHistogram(int numberOfRLevels)
{
   if(isOpen())
   {
      ossimFilename file =
         getFilenameWithThisExtension(ossimString(".his"));
      
      ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
      ossimRefPtr<ossimHistogramWriter> writer = new ossimHistogramWriter;
      
      if(numberOfRLevels)
      {
         histoSource->setMaxNumberOfRLevels(numberOfRLevels);
      }
      else
      {
         histoSource->setMaxNumberOfRLevels(getNumberOfDecimationLevels());
      }
      histoSource->connectMyInputTo(0, this);
      histoSource->enableSource();
      writer->connectMyInputTo(0, histoSource.get());
      writer->setFilename(file);
      writer->addListener(&theStdOutProgress);
      writer->execute();
      histoSource->disconnect();
      writer->disconnect();
      histoSource = 0;
      writer = 0;
   }
   else
   {
      return false;
   }
   
   return true;
}

bool ossimImageHandler::buildAllHistograms(int numberOfRLevels)
{
   ossim_uint32 currentEntry = getCurrentEntry();
   std::vector<ossim_uint32> entryList;
   getEntryList(entryList);
   ossim_uint32 idx = 0;
   for(idx = 0; idx < entryList.size(); ++idx)
   {
      setCurrentEntry(entryList[idx]);
      if(!buildHistogram(numberOfRLevels))
      {
         setCurrentEntry(currentEntry);
         return false;
      }
   }
   
   setCurrentEntry(currentEntry);
   return true;
}

bool ossimImageHandler::buildOverview(ossimImageHandlerOverviewCompressionType compressionType,
                                      ossim_uint32 quality,
                                      ossimFilterResampler::ossimFilterResamplerType resampleType,
                                      bool includeFullResFlag)
{
   ossimFilename file = getFilenameWithThisExtension(ossimString(".ovr"));

   theOverviewFile = file;
   if(buildOverview(file, compressionType, quality, resampleType, includeFullResFlag))
   {
      openOverview();
   }

   return true;
}

bool ossimImageHandler::buildAllOverviews(ossimImageHandlerOverviewCompressionType compressionType,
                                          ossim_uint32 quality,
                                          ossimFilterResampler::ossimFilterResamplerType resampleType,
                                          bool includeFullResFlag)
{
   ossim_uint32 currentEntry = getCurrentEntry();
   std::vector<ossim_uint32> entryList;
   getEntryList(entryList);
   ossim_uint32 idx = 0;
   for(idx = 0; idx < entryList.size(); ++idx)
   {
      setCurrentEntry(entryList[idx]);
      if(!buildOverview(compressionType, quality, resampleType, includeFullResFlag))
      {
         setCurrentEntry(currentEntry);
         return false;
      }
   }
   
   setCurrentEntry(currentEntry);
   return true;
}


bool ossimImageHandler::buildOverview(const ossimFilename& filename,
                                      ossimImageHandlerOverviewCompressionType compressionType,
                                      ossim_uint32 quality,
                                      ossimFilterResampler::ossimFilterResamplerType resampleType,
                                      bool includeFullResFlag)
{
   closeOverview();
   if(!isOpen())
   {
      return false;
   }

   theOverviewFile = filename;
   ossimTiffOverviewBuilder tiffBuilder;
   if ( tiffBuilder.setInputSource(this) == false )
   {
      return false;
   }
   
   ossim_uint16 cType = COMPRESSION_NONE;
   switch(compressionType)
   {
      case OSSIM_OVERVIEW_COMPRESSION_NONE:
      {
         cType = COMPRESSION_NONE;
         break;
      }
      case OSSIM_OVERVIEW_COMPRESSION_JPEG:
      {
         cType = COMPRESSION_JPEG;
         break;
      }
      case OSSIM_OVERVIEW_COMPRESSION_LZW:
      {
         cType = COMPRESSION_LZW;
         break;
      }
      case OSSIM_OVERVIEW_COMPRESSION_DEFLATE:
      {

         cType = COMPRESSION_DEFLATE;
         break;
      }
      case OSSIM_OVERVIEW_COMPRESSION_PACKBITS:
      {
         cType = COMPRESSION_PACKBITS;
         break;
      }
   }
   tiffBuilder.setJpegCompressionQuality(quality);
   tiffBuilder.setCompressionType(cType);
   tiffBuilder.setResampleType(resampleType);
   tiffBuilder.buildOverview(filename, includeFullResFlag);

   return true;
}

//*****************************************************************************
//! Returns the image geometry object associated with this tile source or
//! NULL if non defined.
//! The geometry contains full-to-local image transform as well as projection
//! (image-to-world).
//*****************************************************************************
ossimRefPtr<ossimImageGeometry> ossimImageHandler::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check factory for external geom:
      //---
      theGeometry = getExternalImageGeometry();

      if ( !theGeometry )
      {
         //---
         // WARNING:
         // Must create/set the geometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---
         theGeometry = new ossimImageGeometry();

         //---
         // And finally allow factories to extend the internal geometry.
         // This allows plugins for tagged formats with tags not know in the base
         // to extend the internal geometry.
         //
         // Plugins can do handler->getImageGeometry() then modify/extend.
         //---
         if(!ossimImageGeometryRegistry::instance()->extendGeometry( this ))
         {
            //---
            // Check for internal, for geotiff, nitf and so on as last resort for getting some
            // kind of geometry loaded
            //---
            theGeometry = getInternalImageGeometry();
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }
   return theGeometry;
}

ossimRefPtr<ossimImageGeometry> ossimImageHandler::getExternalImageGeometry() const
{
   ossimRefPtr<ossimImageGeometry> geom = 0;

   // No geometry object has been set up yet. Check for external geometry file.
   // Try "foo.geom" if image is "foo.tif":
   ossimFilename filename = getFilenameWithThisExtension(ossimString(".geom"), false);
   if(!filename.exists())
   {
      // Try "foo_e0.tif" if image is "foo.tif" where "e0" is entry index.
      filename = getFilenameWithThisExtension(ossimString(".geom"), true);
   }
   if(!filename.exists())
   {
      // Try supplementary data directory for remote geometry:
      filename = getFilenameWithThisExtension(ossimString(".geom"), false);
      filename = theSupplementaryDirectory.dirCat(filename.file());
   }
   if(!filename.exists())
   {
      // Try supplementary data directory for remote geometry with entry index:
      filename = getFilenameWithThisExtension(ossimString(".geom"), true);
      filename = theSupplementaryDirectory.dirCat(filename.file());
   }

   if(filename.exists())
   {
      // Open the geom file as a KWL and initialize our geometry object:
      filename = filename.expand();
      ossimKeywordlist geomKwl(filename);

      // Try loadState with no prefix.
      std::string prefix = "";
      std::string key = ossimKeywordNames::TYPE_KW;
      std::string value = geomKwl.findKey(prefix, key);

      if ( value.empty() || (value != "ossimImageGeometry") )
      {
         // Try with "image0." type prefix.
         prefix += std::string("image") + ossimString::toString(getCurrentEntry()).string() +
            std::string(".");
         value = geomKwl.findKey(prefix, key);
         
         if ( value.empty() || (value != "ossimImageGeometry") )
         {
            // Try with "image0.geometry." prefix.
            prefix += std::string( "geometry." );
            value = geomKwl.findKey(prefix, key);
            
            if ( value.empty() || (value != "ossimImageGeometry") )
            {
               // Try with "geometry." prefix.
               prefix = std::string( "geometry." );
               value = geomKwl.findKey(prefix, key);
            }
         }
      }
      
      if ( value == "ossimImageGeometry" )
      {
         geom = new ossimImageGeometry;
         if( !geom->loadState(geomKwl, prefix.c_str()) )
         {
            geom = 0;
         }
      }
   }
   
   return geom;
}

ossimRefPtr<ossimImageGeometry> ossimImageHandler::getInternalImageGeometry() const
{
   // Default, derived classes should override.
   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();
   return geom;
}

void ossimImageHandler::setImageGeometry( ossimImageGeometry* geom)
{
   theGeometry = geom;
}

void ossimImageHandler::saveImageGeometry() const
{
   ossimFilename geometryFile = getFilenameWithThisExtension(ossimString(".geom"));
   saveImageGeometry(geometryFile);
}

void ossimImageHandler::saveImageGeometry(const ossimFilename& geometry_file) const
{
   if (theGeometry.valid())
   {
      ossimKeywordlist kwl;
      theGeometry->saveState(kwl);
      kwl.write(geometry_file.chars());
   }
}

void ossimImageHandler::closeOverview()
{
   theOverview = 0;
}

const ossimImageHandler* ossimImageHandler::getOverview() const
{
   return theOverview.get();
}

bool ossimImageHandler::hasOverviews() const
{
   return (getNumberOfDecimationLevels() > 1);
}

bool ossimImageHandler::openOverview(const ossimFilename& overview_file)
{
   bool result = false;
   
   closeOverview();

   if (overview_file != theImageFile) // Make sure we don't open ourselves.
   {
      //---
      // Get the number of level before the call to opening the overview so
      // the overview can be told what it's starting res level is.
      //---
      ossim_uint32 overviewStartingResLevel = getNumberOfDecimationLevels();


      // Try to open:
      theOverview = ossimImageHandlerRegistry::instance()->openOverview( overview_file );

      if (theOverview.valid())
      {
         result = true;
         
         //---
         // Set the owner in case the overview reader needs to get something
         // from the it like min/max/null.
         //---
         theOverview->changeOwner(this);
         
         // Set the starting res level of the overview.
         theOverview->setStartingResLevel(overviewStartingResLevel);
         
         // Capture the file name.
         theOverviewFile = overview_file;

         //---
         // Some overview handlers cannot store what the null is.  Like dted
         // null is -32767 not default -32768 so this allows passing this to the
         // overview reader provided it overrides setMin/Max/NullPixel value
         // methods. (drb)
         //---
         const ossim_uint32 BANDS = getNumberOfOutputBands();
         for (ossim_uint32 band = 0; band < BANDS; ++band)
         {
            theOverview->setMinPixelValue(band, getMinPixelValue(band));
            theOverview->setMaxPixelValue(band, getMaxPixelValue(band));
            theOverview->setNullPixelValue(band, getNullPixelValue(band));
         }

         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "overview starting res level: " << overviewStartingResLevel
               << "\noverview levels: "
               << theOverview->getNumberOfDecimationLevels()
               << "\nlevels: " << getNumberOfDecimationLevels()
               << endl;
         }
         
         //---
         // This is not really a container event; however, using for now.
         //---
         ossimContainerEvent event(this,
                                   OSSIM_EVENT_ADD_OBJECT_ID);
         event.setObjectList(theOverview.get());
         fireEvent(event);
      }
   }
   
   return result;
}

bool ossimImageHandler::openOverview()
{
   closeOverview();
   
   // 1) ESH 03/2009 -- Use the overview file set e.g. using a .spec file.
   ossimFilename overviewFilename = getOverviewFile();
   
   if (overviewFilename.empty() || (overviewFilename.exists() == false) )
   {
      // 2) Generate the name from image name.
      overviewFilename = createDefaultOverviewFilename();
      
      if (overviewFilename.empty() || (overviewFilename.exists() == false) )
      {  
         // 3) For backward compatibility check if single entry and _e0.ovr
         overviewFilename = getFilenameWithThisExtension(ossimString(".ovr"), true);
         if (overviewFilename.empty() || (overviewFilename.exists() == false) )
         {
            // 4) For overviews built with gdal look for foo.tif.ovr
            overviewFilename = getFilename();
            overviewFilename += ".ovr";
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Looking for " << overviewFilename
         << " overview file..." << std::endl;
   }

   bool status = false;
   
   if ( overviewFilename.exists() )
   {
      status = openOverview( overviewFilename );
   }

   if ( !status  && traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandler::openOverview NOTICE:"
         << "\nCould not find an overview." << std::endl;
   }

   return status;
}


bool ossimImageHandler::writeValidImageVertices(const std::vector<ossimIpt>& vertices, const ossimFilename& file)
{
   ossimFilename tempFile = file;
   if(tempFile == "")
   {
      tempFile = createDefaultValidVerticesFilename();
   }
   ossim_uint32 i = 0;
   ossimKeywordlist tempKwl;
   ossimPolygon poly(vertices);
   
   poly.checkOrdering();

   // Capture the vertices.
   theValidImageVertices = vertices;
   
   if(poly.getOrdering() == OSSIM_COUNTERCLOCKWISE_ORDER)
   {
      std::reverse(theValidImageVertices.begin(), theValidImageVertices.end());
   }
   
   if(theValidImageVertices.size())
   {
      for(i = 0; i < theValidImageVertices.size(); ++i)
      {
         ossimString prefix = ossimString("point") + ossimString::toString(i) + ".";
         tempKwl.add(prefix.c_str(),
                     "x",
                     theValidImageVertices[i].x,
                     true);
	 tempKwl.add(prefix.c_str(),
                     "y",
                     theValidImageVertices[i].y,
                     true);
      }
      
      tempKwl.write(tempFile.c_str());
   }

   if ( tempFile.exists() )
   {
      return true;
   }

   return false; // Write of file failed.
}

bool ossimImageHandler::openValidVertices(const ossimFilename& vertices_file)
{
   return initVertices(vertices_file);
}

bool ossimImageHandler::openValidVertices()
{
   ossimFilename vertices_file;
   vertices_file = theImageFile.path();
   vertices_file = vertices_file.dirCat(theImageFile.fileNoExtension());
   if(getNumberOfEntries() > 1)
   {
      vertices_file += ossimString("_vertices") + "_e" + ossimString::toString(getCurrentEntry()) + ".kwl";
   }
   else
   {
      vertices_file += "_vertices.kwl";
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Looking for " << vertices_file
         << " vertices file..." << std::endl;
   }

   return openValidVertices(vertices_file);
}

bool ossimImageHandler::open(const ossimFilename& imageFile)
{
   if(isOpen())
   {
      close();
   }

   setFilename(imageFile);
   
   return open();
}

bool ossimImageHandler::open(const ossimFilename& imageFile,
                             ossim_uint32 entryIndex)
{
   if(isOpen())
   {
      close();
   }
   setFilename(imageFile);
   
   bool result = open();

   if(result)
   {
      return setCurrentEntry(entryIndex);
   }

   return result;
}

bool ossimImageHandler::isValidRLevel(ossim_uint32 resLevel) const
{
   bool result = false;
   
   const ossim_uint32 LEVELS = getNumberOfDecimationLevels();
   
   if ( !theStartingResLevel) // Not an overview.
   {
      result = (resLevel < LEVELS);
   }
   else  // Used as overview.
   {
      if (resLevel >= theStartingResLevel)
      {
         //---
         // Adjust the res level to be zero based for this overview before
         // checking.
         //---
         result = ( (resLevel - theStartingResLevel) < LEVELS);
      }
   }
   
   return result;
}

void ossimImageHandler::getValidImageVertices(vector<ossimIpt>& validVertices,
                                              ossimVertexOrdering ordering,
                                              ossim_uint32 resLevel) const
{
   ossimDpt decimation;
   getDecimationFactor(resLevel, decimation);
   if(decimation.hasNans())
   {
      return;
   }
   if (theValidImageVertices.size() != 0)
   {
      validVertices.clear();
      for (ossim_uint32 i=0; i<theValidImageVertices.size(); ++i)
      {
         ossimIpt pt(ossim::round<int>(theValidImageVertices[i].x*decimation.x),
                     ossim::round<int>(theValidImageVertices[i].y*decimation.y));
         validVertices.push_back(pt);
      }

      if(ordering == OSSIM_COUNTERCLOCKWISE_ORDER)
      {
         std::reverse(validVertices.begin()+1, validVertices.end());
      }
   }
   else
   {
      // Call the base class method which will set to bounding rectangle.
      ossimImageSource::getValidImageVertices(validVertices, ordering, resLevel);
   }
}

void ossimImageHandler::close()
{
   theOverview = 0;
   theGeometry = 0;
   theValidImageVertices.clear();
   theDecimationFactors.clear();
}

bool ossimImageHandler::isBandSelector() const
{
   return false;
}

bool ossimImageHandler::setOutputBandList(const std::vector<ossim_uint32>& /* band_list */)
{
   return false;
}

bool ossimImageHandler::setOutputToInputBandList()
{
   bool result = false;
   if ( isBandSelector() )
   {
      std::vector<ossim_uint32> bandList;
      ossimImageSource::getOutputBandList( bandList );
      result = setOutputBandList( bandList );
   }
   return result;
}

bool ossimImageHandler::isIdentityBandList( const std::vector<ossim_uint32>& bandList ) const
{
   bool result = false;
   const ossim_uint32 BANDS = bandList.size();
   if ( BANDS )
   {
      std::vector<ossim_uint32> inputList;
      ossimImageSource::getOutputBandList( inputList ); // This populates an identity band list.

      if ( inputList.size() == BANDS )
      {
         ossim_uint32 band = 0;
         while ( band < BANDS )
         {
            if ( inputList[band] != bandList[band] )
            {
               break;
            }
            ++band;
         }
         if ( band == BANDS )
         {
            result = true;
         }
      }
   }
   return result;
}

// Protected convenience method:
bool ossimImageHandler::setOutputBandList(const std::vector<ossim_uint32>& inBandList,
                                          std::vector<ossim_uint32>& outBandList)
{
   bool result = false;

   const ossim_uint32 INPUT_BANDS  = getNumberOfInputBands();
   const ossim_uint32 OUTPUT_BANDS = inBandList.size();

   if ( INPUT_BANDS && OUTPUT_BANDS )
   {
      result = true;
      outBandList.resize( OUTPUT_BANDS );
      for ( ossim_uint32 band = 0; band < OUTPUT_BANDS; ++band )
      {
         if ( inBandList[band] < INPUT_BANDS )
         {
            outBandList[band] = inBandList[band];
         }
         else // Out of range...
         {
            result = false;
            break;
         }
      }
      if ( result && theOverview.valid() )
      {
         result = theOverview->setOutputBandList( inBandList );
      }
   }

   if ( result == false )
   {
      ossimImageSource::getOutputBandList( outBandList ); // Set to identity.
   }

   return result;
}

bool ossimImageHandler::isImageTiled() const
{
   return (getImageTileWidth() && getImageTileHeight());
}

void ossimImageHandler::loadMetaData()
{
  theMetaData.clear();

  ossimFilename filename = getFilenameWithThisExtension(ossimString(".omd"), false);
  if ( filename.exists() == false )
  {
     filename = getFilenameWithThisExtension(ossimString(".omd"), true);
  }
  if(filename.exists())
  {
     ossimKeywordlist kwl;
     
     kwl.addFile(filename.c_str());
     
     theMetaData.loadState(kwl);
  }
  else
  {
     theMetaData.setScalarType(getOutputScalarType());
  }
}

double ossimImageHandler::getMinPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getMinPix(band);
   }
   return ossim::defaultMin(getOutputScalarType());
}

double ossimImageHandler::getMaxPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getMaxPix(band);
   }
   return ossim::defaultMax(getOutputScalarType());
}

double ossimImageHandler::getNullPixelValue(ossim_uint32 band)const
{
   if(theMetaData.getNumberOfBands())
   {
      return theMetaData.getNullPix(band);
   }
   
   return ossim::defaultNull(getOutputScalarType());
}

void ossimImageHandler::setMinPixelValue(ossim_uint32 band,
                                         const ossim_float64& pix)
{
   theMetaData.setMinPix(band, pix);
}

void ossimImageHandler::setMaxPixelValue(ossim_uint32 band,
                                         const ossim_float64& pix)
{
   theMetaData.setMaxPix(band, pix); 
}

void ossimImageHandler::setNullPixelValue(ossim_uint32 band,
                                          const ossim_float64& pix)
{
   theMetaData.setNullPix(band, pix);  
}

ossim_uint32 ossimImageHandler::getCurrentEntry()const
{
   return 0;
}

bool ossimImageHandler::setCurrentEntry(ossim_uint32 /* entryIdx */)
{
   return true;
}

ossimFilename ossimImageHandler::getOverviewFile()const
{
   return theOverviewFile;
}

void ossimImageHandler::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   entryList.push_back(0);
}

void ossimImageHandler::getEntryStringList(
   std::vector<ossimString>& entryStringList) const
{
   std::vector<ossim_uint32> entryList;
   getEntryList(entryList);

   if (entryList.size())
   {
      entryStringList.resize(entryList.size());
      for (ossim_uint32 i = 0; i < entryList.size(); ++i)
      {
         entryStringList[i] = ossimString::toString(entryList[i]);
      }
   }
}

bool ossimImageHandler::useEntryIndex() const
{
   bool result = false;
   if ( isOpen() )
   {
      std::vector<ossim_uint32> entryList;
      getEntryList(entryList);

      if ( entryList.size() )
      {
         if ( (entryList.size() > 1) || (entryList[0] != 0) ) result = true;
      }
   }
   return result;
}

ossim_uint32 ossimImageHandler::getNumberOfEntries()const
{
   std::vector<ossim_uint32> tempList;
   getEntryList(tempList);
   
   return (ossim_uint32)tempList.size();
}

void ossimImageHandler::getEntryName( ossim_uint32 /* entryIdx */, std::string& name ) const
{
   //---
   // This implementation does nothing. Interface for multi entry image readers that have
   // named entries, e.g. HDF5 data.
   //---
   name.clear();
}

void ossimImageHandler::completeOpen()
{
   loadMetaData();
   if ( theOpenOverviewFlag )
   {
      openOverview();
   }
   establishDecimationFactors();
   openValidVertices();
}

bool ossimImageHandler::canConnectMyInputTo(ossim_int32 /* inputIndex */,
                                            const ossimConnectableObject* /* object */)const
{      
   return false;
}

bool ossimImageHandler::hasMetaData()const
{
   return theMetaData.isValid();
}

void ossimImageHandler::setFilename(const ossimFilename& filename)
{
   theImageFile = filename;
}

const ossimFilename& ossimImageHandler::getFilename()const
{
   return theImageFile;
}

void ossimImageHandler::setSupplementaryDirectory(const ossimFilename& dir)
{
   if (dir.isDir())
      theSupplementaryDirectory = dir;
   else
      theSupplementaryDirectory = dir.path();

   // A change in supplementary directory presents an opportunity to find the OVR that could not be
   // opened previously, as well as other support data items:
   if (!theOverview.valid())
   {
      if (theDecimationFactors.size() > 0)
      {
        theDecimationFactors.clear();
      }
      completeOpen();
   }
}

const ossimFilename& ossimImageHandler::getSupplementaryDirectory()const
{
   return theSupplementaryDirectory;
}

void ossimImageHandler::setProperty(ossimRefPtr<ossimProperty> property)
{
   if ( property.valid() )
   {
      if(property->getName() == ossimKeywordNames::ENTRY_KW)
      {
         setCurrentEntry(property->valueToString().toUInt32());
      }
      else if ( property->getName() == OPEN_OVERVIEW_FLAG_KW )
      {
         setOpenOverviewFlag( property->valueToString().toBool() );
      }
      else
      {
         ossimImageSource::setProperty(property);
      }
   }
}

ossimRefPtr<ossimProperty> ossimImageHandler::getProperty(const ossimString& name)const
{

   if(name == ossimKeywordNames::ENTRY_KW)
   {
      std::vector<ossim_uint32> entryList;
      getEntryList(entryList);
      
      ossimStringProperty* stringProperty = new ossimStringProperty(name,
                                                                    ossimString::toString(getCurrentEntry()));
      ossim_uint32 idx = 0;
      for(idx = 0; idx < entryList.size(); ++idx)
      {
         stringProperty->addConstraint(ossimString::toString(entryList[idx]));
      }
      stringProperty->setFullRefreshBit();
      stringProperty->setEditableFlag(false);

      return stringProperty;
   }
   else if(name == ossimKeywordNames::NUMBER_SAMPLES_KW)
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(getNumberOfSamples()));
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   else if(name == ossimKeywordNames::NUMBER_LINES_KW)
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(getNumberOfLines()));
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   else if(name == ossimKeywordNames::NUMBER_BANDS_KW)
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(getNumberOfInputBands()));
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   else if(name == ossimKeywordNames::NUMBER_REDUCED_RES_SETS_KW)
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name, ossimString::toString(getNumberOfDecimationLevels()));
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   else if(name == ossimKeywordNames::SCALAR_TYPE_KW)
   {
      ossimStringProperty* prop = new ossimStringProperty(name, ossimScalarTypeLut::instance()->getEntryString(getOutputScalarType()));
      prop->setReadOnlyFlag(true);
      
      return prop;
   }
   else if(name == "histogram_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultHistogramFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   else if(name == "geometry_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultGeometryFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   else if(name == "valid_vertices_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultValidVerticesFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   else if(name == "metadata_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultMetadataFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   else if(name == "overview_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultOverviewFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   else if(name == "filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, getFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setFullRefreshBit();
      
      return filenameProp;
   }
   else if ( name == OPEN_OVERVIEW_FLAG_KW)
   {
      ossimRefPtr<ossimProperty> result =
         new ossimBooleanProperty(ossimString(OPEN_OVERVIEW_FLAG_KW), theOpenOverviewFlag); 
      return result;
   }
   
   return ossimImageSource::getProperty(name);
}

void ossimImageHandler::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSource::getPropertyNames(propertyNames);
   propertyNames.push_back(ossimKeywordNames::ENTRY_KW);
   propertyNames.push_back(ossimKeywordNames::NUMBER_SAMPLES_KW);
   propertyNames.push_back(ossimKeywordNames::NUMBER_LINES_KW);
   propertyNames.push_back(ossimKeywordNames::NUMBER_BANDS_KW);
   propertyNames.push_back(ossimKeywordNames::NUMBER_REDUCED_RES_SETS_KW);
   propertyNames.push_back(ossimKeywordNames::SCALAR_TYPE_KW);
   propertyNames.push_back(OPEN_OVERVIEW_FLAG_KW);
}

ossimFilename ossimImageHandler::getFilenameWithThisExtension(
   const ossimString& ext,
   bool set_e0_prefix) const
{
   // Get the image file.
   ossimFilename f = getFilename();

   // If the supplementary directory is set, find the extension
   // at that location instead of at the default.
   if ( theSupplementaryDirectory.size() )
   {
      
      ossimString drivePart;
      ossimString pathPart;
      ossimString filePart;
      ossimString extPart;

      f.split(drivePart,
              pathPart,
              filePart,
              extPart);

      ossimFilename newDrivePart = theSupplementaryDirectory.drive();
      ossimFilename newPathPart  = theSupplementaryDirectory.after(newDrivePart);

      f.merge( newDrivePart, newPathPart, filePart, extPart );
   }

   // Wipe out the extension.
   f.setExtension("");

   if (set_e0_prefix || (getNumberOfEntries() > 1))
   {
      f += "_e";
      f += ossimString::toString(getCurrentEntry());
   }
   if (ext.size())
   {
      if (ext[static_cast<std::string::size_type>(0)] != '.')
      {
         f += ".";
      }
      f += ext;
   }
   return f;
}

bool ossimImageHandler::getOverviewTile(ossim_uint32 resLevel,
                                        ossimImageData* result)
{
   bool status = false;
   
   if (theOverview.valid())
   {
      if (theOverview->isValidRLevel(resLevel) && result &&
          (result->getNumberOfBands() == getNumberOfOutputBands()) )
      {
         status = theOverview->getTile(result, resLevel);
      }
   }
   
   return status;
}

ossimRefPtr<ossimNBandLutDataObject> ossimImageHandler::getLut()const
{
   return theLut;
}

bool ossimImageHandler::hasLut() const
{
   return theLut.valid();
}

ossimFilename ossimImageHandler::createDefaultOverviewFilename() const
{
   return getFilenameWithThisExtension("ovr");
}
   
ossimFilename ossimImageHandler::createDefaultGeometryFilename() const
{
   return getFilenameWithThisExtension("geom");
}

ossimFilename ossimImageHandler::createDefaultMetadataFilename() const
{
   return getFilenameWithThisExtension("omd");
}

ossimFilename ossimImageHandler::createDefaultHistogramFilename() const
{
   return getFilenameWithThisExtension("his");
}

ossimFilename ossimImageHandler::createDefaultValidVerticesFilename() const
{
   ossimFilename result;
   
   result = theImageFile.path();
   result = result.dirCat(theImageFile.fileNoExtension());
   result += "_vertices";
   if(getNumberOfEntries() > 1)
   {
      result += "_e";
      result += ossimString::toString(getCurrentEntry());
   }
   result += ".kwl";
   
   return result;
}

ossim_uint32 ossimImageHandler::getStartingResLevel() const
{
   return theStartingResLevel;
}

void ossimImageHandler::setStartingResLevel(ossim_uint32 level)
{
   theStartingResLevel = level;
}

bool ossimImageHandler::getOpenOverviewFlag() const
{
   return theOpenOverviewFlag;
}
   
void ossimImageHandler::setOpenOverviewFlag(bool flag)
{
   theOpenOverviewFlag = flag;

   // If false close overview if open??? (drb)
}

void ossimImageHandler::initImageParameters(ossimImageGeometry* geom) const
{
   if ( geom )
   {
      // Set decimation levels
      ossim_uint32 num_levels = getNumberOfDecimationLevels();
      if ((num_levels > 0) && (num_levels != geom->getNumberOfDecimations()))
      {
         geom->setDiscreteDecimation(theDecimationFactors);
      }

      // Set image size.
      if(geom->getImageSize().hasNans())
      {
         ossimIrect rect = getBoundingRect();
         geom->setImageSize(ossimIpt(rect.width(), rect.height()));
      } 
   }
}
