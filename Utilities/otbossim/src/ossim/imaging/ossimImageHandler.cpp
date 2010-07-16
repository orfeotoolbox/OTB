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
//  $Id: ossimImageHandler.cpp 17108 2010-04-15 21:08:06Z dburken $

#include <algorithm>

#include <ossim/imaging/ossimImageHandler.h>

#include <ossim/base/ossimContainerEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>

#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>

#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
RTTI_DEF1(ossimImageHandler, "ossimImageHandler", ossimImageSource)

//***
// Static trace for debugging
//***
static ossimTrace traceDebug("ossimImageHandler:debug");

// Property keywords.
static const char SUPPLEMENTARY_DIRECTORY_KW[] = "supplementary_directory";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageHandler.cpp 17108 2010-04-15 21:08:06Z dburken $";
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
//theSubImageOffset(0, 0),
theValidImageVertices(0),
theMetaData(),
theStartingResLevel(0)
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

   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theImageFile.c_str(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::OVERVIEW_FILE_KW,
           theOverviewFile.c_str(),
           true);
   kwl.add(prefix,
           SUPPLEMENTARY_DIRECTORY_KW,
           theSupplementaryDirectory.c_str(),
           true);

   kwl.add(prefix, "start_res_level", theStartingResLevel, true);
   
   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimImageHandler::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   static const char MODULE[] = "ossimImageHandler::loadState(kwl, prefix)";
   
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
   lookup = kwl.find(prefix, "valid_vertices_file");
   if (lookup)
   {
      initVertices(lookup);
   }

   // Starting resolution level.
   lookup = kwl.find(prefix, "start_res_level");
   if (lookup)
   {
      theStartingResLevel = ossimString(lookup).toUInt32();
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


void ossimImageHandler::getDecimationFactor(ossim_uint32 resLevel,
                                            ossimDpt& result) const
{
   if (resLevel == 0)
   {
      result.x = 1.0;
      result.y = 1.0;
   }
   else
   {
      /*
         ESH 02/2009 -- No longer assume powers of 2 reduction
         in linear size from resLevel 0 (Tickets # 467,529).
      */
      ossim_int32 x  = getNumberOfLines(resLevel);
      ossim_int32 x0 = getNumberOfLines(0);

      if ( x > 0 && x0 > 0 ) 
      {
         result.x = ((double)x) / x0; 
      }
      else 
      {
         result.x = 1.0 / (1<<resLevel);
      }
      result.y = result.x;
   }
}

void ossimImageHandler::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   const ossim_uint32 LEVELS = getNumberOfDecimationLevels();
   decimations.resize(LEVELS);
   for (ossim_uint32 level = 0; level < LEVELS; ++level)
   {
      getDecimationFactor(level, decimations[level]);
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
ossimImageGeometry* ossimImageHandler::getImageGeometry()
{
   if (theGeometry.valid())
   {
      if(getNumberOfDecimationLevels() !=
         theGeometry->getNumberOfDecimations())
      {
         std::vector<ossimDpt> decimationList;
         getDecimationFactors(decimationList);
         theGeometry->setDiscreteDecimation(decimationList);
      }
      return theGeometry.get();
   }
   
   //---
   // Check factory for external geom:
   //---
   theGeometry = getExternalImageGeometry();
   if (theGeometry.valid())
   {
      return theGeometry.get();  // We should return here.
   }
   // ok,  now let's start with an empty geometry and then call the extension plugins.
   //
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
      // Check for internal, for geotiff, nitf and so on as last resort for getting some kind of geometry
      // loaded
      //
      theGeometry = getInternalImageGeometry();
   }
   
   return theGeometry.get();
}

ossimImageGeometry* ossimImageHandler::getExternalImageGeometry()
{
   // If already defined, return it:
   if (theGeometry.valid())
   {
      return theGeometry.get();
   }

   //---
   // No geometry object has been set up yet. Check for external geometry file.
   //---
   // Try "foo.geom" if image is "foo.tif":
   //
   ossimFilename filename = getFilenameWithThisExtension(ossimString(".geom"), false);
   
   if(!filename.exists())
   {
      // Try "foo_e0.tif" if image is "foo.tif" where "e0" is entry index.
      filename = getFilenameWithThisExtension(ossimString(".geom"), true);
   }
   ossimRefPtr<ossimImageGeometry> geom = 0;
   
   if(filename.exists())
   {
      // Open the geom file as a KWL and initialize our geometry object:
      ossimKeywordlist geomKwl(filename);
      
      ossimString prefix = "";
      // Try loadState with no prefix.
      const char* lookup = geomKwl.find(prefix.c_str(),
                                        ossimKeywordNames::TYPE_KW);

      if (!lookup)
      {
         // Try with "image0 type prefix.
         prefix = "image"+ossimString::toString(getCurrentEntry()) + ".";
         lookup = geomKwl.find(prefix.c_str(), ossimKeywordNames::TYPE_KW);
      }
      
      if (lookup)
      {
         geom = new ossimImageGeometry;
         if(!geom->loadState(geomKwl, prefix.c_str()))
         {
            geom = 0;
         }
#if 0
         ossimString type = lookup;
         if(type == "ossimImageGeometry")
         {
            // Try it with no prefix.
            geom = new ossimImageGeometry;
            if(!geom->loadState(geomKwl, prefix.c_str()))
            {
               geom = 0;
            }
         }
#endif
      }
   }
   
   theGeometry = geom.get();
   return theGeometry.get();
}

ossimImageGeometry* ossimImageHandler::getInternalImageGeometry()
{
   // Default, derived classes should override.
   if ( !theGeometry )
   {
      // allocate an empty geometry if nothing present
      theGeometry = new ossimImageGeometry();
   }
   
   return theGeometry.get();
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
      result = true;
      
      //---
      // Get the number of level before the call to opening the overview so
      // the overview can be told what it's starting res level is.
      //---
      ossim_uint32 overviewStartingResLevel =
         getNumberOfDecimationLevels();
      
      // Try to open:
      theOverview = ossimImageHandlerRegistry::instance()->open(overview_file);

      if (theOverview.valid())
      {
         // Set the starting res level of the overview.
         theOverview->setStartingResLevel(overviewStartingResLevel);
         
         // Capture the file name.
         theOverviewFile = overview_file;

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
                                   theOverview.get(),
                                   OSSIM_EVENT_ADD_OBJECT_ID);
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
      status = openOverview(overviewFilename);
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
}

bool ossimImageHandler::isBandSelector() const
{
   return false;
}

bool ossimImageHandler::setOutputBandList(const std::vector<ossim_uint32>& /* band_list */)
{
   return false;
}

bool ossimImageHandler::isImageTiled() const
{
   return (getImageTileWidth() && getImageTileHeight());
}

void ossimImageHandler::loadMetaData()
{
  theMetaData.clear();

  ossimFilename filename = getFilenameWithThisExtension(ossimString(".omd"));
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

ossim_uint32 ossimImageHandler::getNumberOfEntries()const
{
   std::vector<ossim_uint32> tempList;
   getEntryList(tempList);
   
   return (ossim_uint32)tempList.size();
}


void ossimImageHandler::completeOpen()
{
   loadMetaData();
   openOverview();
   openValidVertices();

   if ( theOverview.valid() )
   {
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
   }
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
   theSupplementaryDirectory = dir;
}

const ossimFilename& ossimImageHandler::getSupplementaryDirectory()const
{
   return theSupplementaryDirectory;
}

void ossimImageHandler::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(property->getName() == ossimKeywordNames::ENTRY_KW)
   {
      setCurrentEntry(property->valueToString().toUInt32());
   }
   else
   {
      ossimImageSource::setProperty(property);
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
   if(name == "histogram_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultHistogramFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   if(name == "geometry_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultGeometryFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   if(name == "valid_vertices_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultValidVerticesFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   if(name == "metadata_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultMetadataFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   if(name == "overview_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createDefaultOverviewFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   if(name == "filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, getFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setFullRefreshBit();
      
      return filenameProp;
   }
   
   return ossimImageSource::getProperty(name);
}

void ossimImageHandler::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSource::getPropertyNames(propertyNames);
   propertyNames.push_back(ossimKeywordNames::ENTRY_KW);
}

ossimFilename ossimImageHandler::getFilenameWithThisExtension(
   const ossimString& ext,
   bool set_e0_prefix) const
{
   // Get the image file.
   ossimFilename f = getFilename();

   // If the supplementary directory is set, find the extension
   // at that location instead of at the default.
   if ( theSupplementaryDirectory.empty() == false )
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

   
