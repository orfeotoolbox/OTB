//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
//
// Description:
// 
// This class defines an abstract image handler which all loaders should
// derive from.
//
//*******************************************************************
//  $Id: ossimImageHandler.cpp 12199 2007-12-16 14:18:04Z dburken $

#include <algorithm>

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimContainerEvent.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>

RTTI_DEF1(ossimImageHandler, "ossimImageHandler", ossimImageSource)

//***
// Static trace for debugging
//***
static ossimTrace traceDebug("ossimImageHandler:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageHandler.cpp 12199 2007-12-16 14:18:04Z dburken $";
#endif

   
ossimImageHandler::ossimImageHandler()
      :
      ossimImageSource(0,
                       0,
                       0,
                       true,
                       false),// output list is not fixed
      theImageFile(ossimFilename::NIL),
      theOverview(0),
      theSubImageOffset(0, 0),
      theValidImageVertices(0),
      theMetaData(),
      theGeometryKwl()
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
   if (theOverview)
   {
      delete theOverview;
      theOverview = 0;
   }
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
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:\n"
         << "filename = " << theImageFile << std::endl;
   }
   
   // Check for an external geometry file.
   lookup = kwl.find(prefix, ossimKeywordNames::GEOM_FILE_KW);
   if (lookup)
   {
      ossimKeywordlist geomKwl;
      if (geomKwl.addFile(lookup) == true)
      {
         theGeometryKwl = geomKwl;
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
   return (theOverview?theOverview->getNumberOfDecimationLevels():1);
}

ossim_uint32 ossimImageHandler::getNumberOfReducedResSets() const
{
   return getNumberOfDecimationLevels();
}

ossimIrect ossimImageHandler::getImageRectangle(ossim_uint32 resLevel) const
{
   ossimIrect result;
   
   if( !isOpen() )
   {
      result.makeNan();
      return result;
   }
   
   if( isValidRLevel(resLevel) )
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
   
   //---
   // Special case to handle overviews with no R0.
   //---
   else if( isValidRLevel(resLevel+1) )
   {
      ossim_int32 lines   = getNumberOfLines(resLevel+1);
      ossim_int32 samples = getNumberOfSamples(resLevel+1);
      if( lines && samples )
      {
         result = ossimIrect(0, 0, samples*2-1, lines*2-1);
      }
      else
      {
         result.makeNan();
      }
   }
   
   return result;
}

ossimIrect ossimImageHandler::getBoundingRect(ossim_uint32 resLevel) const
{
   ossimIrect result = getImageRectangle(resLevel);

   if (result.isNan() == false)
   {
      ossimIpt offset = getSubImageOffset(resLevel);
      if (offset.isNan() == false)
      {
         result = result + offset;
      }
   }

   return result;
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
      result.x = 1.0 / pow((double)2, (double)resLevel);
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
      
      ossimImageHistogramSource* histoSource = new ossimImageHistogramSource;
      ossimHistogramWriter* writer = new ossimHistogramWriter;
      
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
      writer->connectMyInputTo(0, histoSource);
      writer->setFilename(file);
      writer->addListener(&theStdOutProgress);
      writer->execute();
      
      delete histoSource;
      delete writer;
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

   ossimTiffOverviewBuilder tiffBuilder;
   if ( tiffBuilder.setInputSource(this, false) == false )
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

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimImageHandler::getImageGeometry(ossimKeywordlist& kwl,
                                         const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandler::getImageGeometry DEBUG: entered..."
         << std::endl;
   }
   
   if (theGeometryKwl.getSize())
   {
      kwl.add(prefix, theGeometryKwl);
      //kwl = theGeometryKwl;
      return true;
   }
   
   bool result = false;
   ossimRefPtr<ossimProjection> proj = 0;
   ossimFilename filename = getFilenameWithThisExtension(ossimString(".geom"));
   if(!filename.exists())
   {
      // Try tacking on the entry like "foo_e0.hdf".
      filename = getFilenameWithThisExtension(ossimString(".geom"), true);
   }
   if(filename.exists())
   {
      if(kwl.addFile(filename))
      {
         ossimString tempPrefix =
            "image"+ossimString::toString(getCurrentEntry())+".";
         kwl.stripPrefixFromAll(tempPrefix);
         
         if(kwl.find(ossimKeywordNames::TYPE_KW))
         {
            ossimKeywordlist kwlTemp(kwl);
            
            kwlTemp.add(ossimKeywordNames::GEOM_FILE_KW,
                        filename.c_str(),
                        true);

            proj = ossimProjectionFactoryRegistry::instance()->
               createProjection(kwlTemp);
            if(proj.valid())
            {
               result = true;
               kwl.add(prefix, kwlTemp);//kwl = kwlTemp;
            }
         }
      }
   }

   
   if(!result)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimImageHandler::getImageGeometry DEBUG:"
            << "\nCalling createProjection"
            << "\ngetFilename():      " << getFilename()
            << "\ngetCurrentEntry():  " << getCurrentEntry()
            << std::endl;
      }
      
      proj = ossimProjectionFactoryRegistry::instance()->
         createProjection(getFilename(),
                          getCurrentEntry());
      if(proj.valid())
      {
         result = proj->saveState(kwl, prefix);
         
         result = true;
      }
   }
   
   if (result == true)
   {
      theGeometryKwl.clear();
      // Capture the geometry for next time.
//      theGeometryKwl = kwl;
      theGeometryKwl.add(kwl, prefix, true);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandler::getImageGeometry DEBUG:"
         << "boolean return:  " << result
         << std::endl;
   }
   
   return result;
}

void ossimImageHandler::setImageGeometry(const ossimKeywordlist& kwl)
{
   theGeometryKwl = kwl;
}

void ossimImageHandler::saveImageGeometry() const
{
   ossimFilename geometryFile =
      getFilenameWithThisExtension(ossimString(".geom"));
   saveImageGeometry(geometryFile);
}

void ossimImageHandler::saveImageGeometry(
   const ossimFilename& geometry_file) const
{
   theGeometryKwl.write(geometry_file.c_str());
}

void ossimImageHandler::closeOverview()
{
   if(theOverview)
   {
      delete theOverview;
      theOverview = 0;
   }
}

// const ossimFilename& ossimImageHandler::getOverviewFile()const
// {
//    return theOverviewFile;
// }

bool ossimImageHandler::hasOverviews() const
{
   return (getNumberOfDecimationLevels() > 1);
}

bool ossimImageHandler::openOverview(const ossimFilename& overview_file)
{
   closeOverview();

   // make sure we don't open ourselves.
   if(theImageFile == overview_file) return false;
   
   theOverview = new ossimTiffTileSource;

   if ( theOverview->open(overview_file) )
   {
      //---
      // This is not really a container event; however, using for now.
      //---
      ossimContainerEvent event(this, theOverview, OSSIM_EVENT_ADD_OBJECT_ID);
      fireEvent(event);

      return true;
   }

   delete theOverview;
   theOverview = 0;
   return false;
}

bool ossimImageHandler::openOverview()
{
   closeOverview();

   ossimFilename overviewFilename = createOverviewFilename();
   
   // 1) If the current over file exists see if if can be opened.
   if( overviewFilename.exists() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Looking for " << overviewFilename
            << " overview file..." << std::endl;
      }

      if ( openOverview(overviewFilename) )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Opened overview " << overviewFilename
               << std::endl;
         }
         return true;
      }
   }
   
   // 2) For backward compatibility check if single entry and _e0.ovr
   overviewFilename = getFilenameWithThisExtension(ossimString(".ovr"), true);
   if ( overviewFilename.exists() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Looking for " << overviewFilename
            << " overview file..." << std::endl;
      }
   
      if ( openOverview(overviewFilename) )
      {
         return true;
      }
   }

   // 3) For overviews built with gdal look for foo.tif.ovr
   overviewFilename = getFilename();
   overviewFilename += ".ovr";
   if ( overviewFilename.exists() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Looking for " << overviewFilename
            << " overview file..." << std::endl;
      }
   
      if ( openOverview(overviewFilename) )
      {
         return true;
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandler::openOverview NOTICE:"
         << "\nCould not find an overview." << std::endl;
   }

   return false;
}


bool ossimImageHandler::writeValidImageVertices(const std::vector<ossimIpt>& vertices, const ossimFilename& file)
{
   ossimFilename tempFile = file;
   if(tempFile == "")
   {
      tempFile = createValidVerticesFilename();
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
   if (resLevel < getNumberOfDecimationLevels())
   {
      return true;
   }

   return false;
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
   if (theOverview)
   {
      delete theOverview;
      theOverview = 0;
   }

   theValidImageVertices.clear();
}

bool ossimImageHandler::isBandSelector() const
{
   return false;
}

bool ossimImageHandler::setOutputBandList(const std::vector<ossim_uint32>& band_list)
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


ossim_uint32 ossimImageHandler::getCurrentEntry()const
{
   return 0;
}

bool ossimImageHandler::setCurrentEntry(ossim_uint32 /* entryIdx */)
{
   return true;
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
   
   return tempList.size();
}


void ossimImageHandler::completeOpen()
{
  loadMetaData();
  openOverview();
  openValidVertices();
}

bool ossimImageHandler::canConnectMyInputTo(ossim_int32 inputIndex,
                                            const ossimConnectableObject* /* object */)const
{      
   return false;
}

bool ossimImageHandler::hasMetaData()const
{
   return theMetaData.isValid();
}

ossimIpt ossimImageHandler::getSubImageOffset(ossim_uint32 resLevel) const
{
   if (resLevel == 0)
   {
      return theSubImageOffset;
   }

   ossimIpt offset;
   offset.makeNan();
   if (isValidRLevel(resLevel))
   {
      ossimDpt decimation;
      getDecimationFactor(resLevel, decimation);
      if(!decimation.hasNans()) // Should never have nans...
      {
         offset.x = ossim::round<int>(theSubImageOffset.x*decimation.x);
         offset.y = ossim::round<int>(theSubImageOffset.y*decimation.y);
      }
   }
   return offset;
}

void ossimImageHandler::setSubImageOffset(const ossimIpt& p)
{
   theSubImageOffset = p;
}

void ossimImageHandler::setFilename(const ossimFilename& filename)
{
   theImageFile = filename;
}

const ossimFilename& ossimImageHandler::getFilename()const
{
   return theImageFile;
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
         new ossimFilenameProperty(name, createHistogramFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   if(name == "geometry_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createGeometryFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
      
   }
   if(name == "valid_vertices_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createValidVerticesFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   if(name == "metadata_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createMetadataFilename());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      
      return filenameProp;
   }
   if(name == "overview_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, createOverviewFilename());
      
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

   // Wipe out the extension.
   f.setExtension("");

   if (set_e0_prefix || (getNumberOfEntries() > 1))
   {
      f += "_e";
      f += ossimString::toString(getCurrentEntry());
   }
   if (ext.size())
   {
      if (ext[0] != '.')
      {
         f += ".";
      }
      f += ext;
   }
   return f;
}

ossimRefPtr<ossimNBandLutDataObject> ossimImageHandler::getLut()const
{
   return theLut;
}

ossimFilename ossimImageHandler::createOverviewFilename() const
{
   return getFilenameWithThisExtension("ovr");
}
   
ossimFilename ossimImageHandler::createGeometryFilename() const
{
   return getFilenameWithThisExtension("geom");
}

ossimFilename ossimImageHandler::createMetadataFilename() const
{
   return getFilenameWithThisExtension("omd");
}

ossimFilename ossimImageHandler::createHistogramFilename() const
{
   return getFilenameWithThisExtension("his");
}

ossimFilename ossimImageHandler::createValidVerticesFilename() const
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
