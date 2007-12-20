// $Id: ossimOrthoIgen.cpp 11425 2007-07-27 17:03:29Z dburken $
#include <sstream>
#include <ossim/parallel/ossimOrthoIgen.h>
#include <ossim/parallel/ossimIgen.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimEastingNorthingCutter.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/parallel/ossimMpi.h>

static ossimTrace traceDebug("ossimOrthoIgen:debug");
static ossimTrace traceLog("ossimOrthoIgen:log");

ossimOrthoIgen::ossimOrthoIgenFilename::ossimOrthoIgenFilename(const ossimFilename& file, bool decodeEntry)
{
   if(decodeEntry)
   {
      setFilenameWithDecoding(file);
   }
   else
   {
      setFilenameAndEntry(file, -1);
   }
}

void ossimOrthoIgen::ossimOrthoIgenFilename::setFilenameWithDecoding(const ossimFilename& file)
{
   ossimFilename::size_type idx = file.rfind("|", file.length());
   ossimString actualFile = file;
   ossimString entry("-1");       
   if(idx != ossimFilename::npos)
   {
      actualFile = ossimString(file.begin(), file.begin()+idx);
      entry      = ossimString(file.begin()+idx+1,
                               file.end());
      entry = entry.trim();
      actualFile = actualFile.trim();
   }

   theFilename = ossimFilename(actualFile);
   theEntry    = entry.toInt32();
}

ossimOrthoIgen::ossimOrthoIgen()
   :
   theThumbnailRes(""),
   theThumbnailFlag(false),
   theDeltaPerPixelUnit(OSSIM_UNIT_UNKNOWN),
   theDeltaPerPixelOverride(ossim::nan(), ossim::nan()),
   theProjectionType(OSSIM_INPUT_PROJECTION),
   theProjectionName(""),
   theGeographicOriginOfLatitude(0.0),
   theCombinerType("ossimImageMosaic"),
   theResamplerType("nearest neighbor"),
   theTemplateView(""),
   theTilingTemplate(""),
   theTilingFilename(""),
   theChainTemplate(""),
   theCombinerTemplate(""),
   theAnnotationTemplate(""),
   theWriterTemplate(""),
   theSlaveBuffers("2"),
   theCutOriginType(ossimOrthoIgen::OSSIM_CENTER_ORIGIN),
   theCutOrigin(ossim::nan(), ossim::nan()),
   theCutDxDy(ossim::nan(), ossim::nan()),
   theCutOriginUnit(OSSIM_UNIT_UNKNOWN),
   theCutDxDyUnit(OSSIM_UNIT_UNKNOWN),
   theLowPercentClip(ossim::nan()),
   theHighPercentClip(ossim::nan()),
   theUseAutoMinMaxFlag(false),
   theScaleToEightBitFlag(false),
   theFilenames()
{
   // setDefaultValues();
}

void ossimOrthoIgen::addArguments(ossimArgumentParser& argumentParser)
{
   argumentParser.getApplicationUsage()->addCommandLineOption("--annotate", "annotation keyword list");

   argumentParser.getApplicationUsage()->addCommandLineOption("-t or --thumbnail", "thumbnail resolution");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--meters","Specifies an override for the meters per pixel");
   argumentParser.getApplicationUsage()->addCommandLineOption("--slave-buffers","number of slave tile buffers for mpi processing (default = 2)");
   argumentParser.getApplicationUsage()->addCommandLineOption("--view-template","Specify an external file that contains view information");
   argumentParser.getApplicationUsage()->addCommandLineOption("--chain-template","Specify an external file that contains chain information");
   argumentParser.getApplicationUsage()->addCommandLineOption("--combiner-template","Specify an external file that contains combiner information");
   argumentParser.getApplicationUsage()->addCommandLineOption("--tiling-template","Specify an external file that contains tiling information");
   argumentParser.getApplicationUsage()->addCommandLineOption("--writer-template","Specify an external file that contains tiling information");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--utm","Defaults to a utm image chain with GSD = to the input");

   argumentParser.getApplicationUsage()->addCommandLineOption("--geo","Defaults to a geographic image chain with GSD = to the input.  Origin of latitude is on the equator.");
   argumentParser.getApplicationUsage()->addCommandLineOption("--input-proj","Makes the view equal to the input.  If more than one file then the first is taken");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--geo-scaled","Takes a latitude as an argument for purpose of scaling.  Specifies that no spec file was defined.  Defaults to a scaled geographic image chain with GSD = to the input.");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--combiner-type","Specify what mosaic to use, ossimImageMosiac or ossimFeatherMosaic or osimBlendMosaic ... etc");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--resample-type","Specify what resampler to use, nearest neighbor, bilinear, bicubic");
   
   argumentParser.getApplicationUsage()->addCommandLineOption("--cut-center-ll","Specify the center cut in lat lon space.  Takes two argument <lat> <lon>");
   argumentParser.getApplicationUsage()->addCommandLineOption("--cut-radius-meters","Specify the cut distance in meters.  A bounding box for the cut will be produced");

   argumentParser.getApplicationUsage()->addCommandLineOption("--cut-bbox-ll","Specify the min lat and min lon and max lat and maxlon <minLat> <minLon> <maxLat> <maxLon>");
   argumentParser.getApplicationUsage()->addCommandLineOption("--cut-pixel-width-height","Specify cut box's width and height in pixels");

   argumentParser.getApplicationUsage()->addCommandLineOption("--hist-stretch","Specify in normalized percent the low clip and then the high clip value");

   argumentParser.getApplicationUsage()->addCommandLineOption("--hist-auto-minmax","uses the automatic search for the best min and max clip values");

   argumentParser.getApplicationUsage()->addCommandLineOption("--scale-to-8-bit","Scales output to eight bits if not already.");
}

void ossimOrthoIgen::initialize(ossimArgumentParser& argumentParser)
{
   if(traceDebug())
   {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "Entered..... " 
            << std::endl;
   }
   double tempDouble;
   double tempDouble2;
   double tempDouble3;
   double tempDouble4;
   ossimString tempString;
   ossimArgumentParser::ossimParameter stringParam(tempString);
   ossimArgumentParser::ossimParameter doubleParam(tempDouble);
   ossimArgumentParser::ossimParameter doubleParam2(tempDouble2);
   ossimArgumentParser::ossimParameter doubleParam3(tempDouble3);
   ossimArgumentParser::ossimParameter doubleParam4(tempDouble4);
   theCutOriginType = ossimOrthoIgen::OSSIM_CENTER_ORIGIN;
   theCutOrigin.makeNan();
   theCutDxDy.makeNan();
   theCutOriginUnit   = OSSIM_UNIT_UNKNOWN;
   theCutDxDyUnit     = OSSIM_UNIT_UNKNOWN;
   theLowPercentClip  = ossim::nan();
   theHighPercentClip = ossim::nan();
   double minLat=ossim::nan(), minLon=ossim::nan(), maxLat=ossim::nan(), maxLon=ossim::nan();
   theUseAutoMinMaxFlag = false;
   theDeltaPerPixelOverride.makeNan();
   theDeltaPerPixelUnit = OSSIM_UNIT_UNKNOWN;
   if(argumentParser.read("--annotate", stringParam))
   {
      theAnnotationTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("-t", stringParam)   ||
      argumentParser.read("--thumbnail", stringParam))
   {
      theThumbnailRes  = tempString;
      theThumbnailFlag = true;
   }
   if(argumentParser.read("--slave-buffers", stringParam))
   {
      theSlaveBuffers = tempString;
   }
   if(argumentParser.read("--cut-center-ll", doubleParam, doubleParam2))
   {
      theCutOrigin.lat = tempDouble;
      theCutOrigin.lon = tempDouble2;
      theCutOriginUnit = OSSIM_DEGREES;
      theCutOriginType = ossimOrthoIgen::OSSIM_CENTER_ORIGIN;
   }
   if(argumentParser.read("--cut-radius-meters", doubleParam))
   {
      theCutDxDy.x = tempDouble;
      theCutDxDy.y = tempDouble;
      theCutDxDyUnit = OSSIM_METERS;
   }
   if(argumentParser.read("--cut-bbox-ll", doubleParam, doubleParam2, doubleParam3, doubleParam4))
   {
      minLat = tempDouble;
      minLon = tempDouble2;
      maxLat = tempDouble3;
      maxLon = tempDouble4;
      theCutOriginUnit = OSSIM_DEGREES;
      theCutOriginType = ossimOrthoIgen::OSSIM_UPPER_LEFT_ORIGIN;
      theCutOrigin.lat = maxLat;
      theCutOrigin.lon = minLon;
      theCutDxDy.lat   = (maxLat-minLat);
      theCutDxDy.lon   = (maxLon-minLon);
      theCutDxDyUnit   = theCutOriginUnit;
   }
   if(argumentParser.read("--cut-pixel-width-height", doubleParam, doubleParam2))
   {
      if((ossim::isnan(minLat) == false)&&
         (ossim::isnan(minLon) == false)&&
         (ossim::isnan(maxLat) == false)&&
         (ossim::isnan(maxLon) == false))
      {
         theDeltaPerPixelOverride = ossimDpt(theCutDxDy.x/(tempDouble-1),
                                             theCutDxDy.y/(tempDouble2-1));
         theDeltaPerPixelUnit     = theCutDxDyUnit;
      }
      else
      {
         theCutOrigin.makeNan();
         ossimNotify(ossimNotifyLevel_WARN) << "Can't have option --cut-pixel-width-height without --cut-bbox-ll" << std::endl;
      }
   }
   if(argumentParser.read("--hist-stretch", doubleParam, doubleParam2))
   {
      theLowPercentClip = tempDouble;
      theHighPercentClip = tempDouble2;
   }
   if(argumentParser.read("--hist-auto-minmax"))
   {
      theUseAutoMinMaxFlag = true;
   }
   if(argumentParser.read("--scale-to-8-bit"))
   {
      theScaleToEightBitFlag = true;
   }
   if(argumentParser.read("--meters", doubleParam))
   {
      theDeltaPerPixelUnit = OSSIM_METERS;
      theDeltaPerPixelOverride.x = tempDouble;
      theDeltaPerPixelOverride.y = tempDouble;
   }
   if(argumentParser.read("--writer-template",
                          stringParam))
   {
      theWriterTemplate = tempString;
   }
   if(argumentParser.read("--tiling-template",
                          stringParam))
   {
      theTilingTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("--chain-template", stringParam))
   {
      theChainTemplate = ossimFilename(tempString);
   }
   if(argumentParser.read("--combiner-template", stringParam))
   {
      theCombinerTemplate = ossimFilename(tempString);
   }
   if (argumentParser.read("--utm"))
   {
      theProjectionType = OSSIM_UTM_PROJECTION;
      theProjectionName = "ossimUtmProjection";
   }
   else if(argumentParser.read("--geo"))
   {
      theProjectionType = OSSIM_GEO_PROJECTION;
      theProjectionName = "ossimEquDistCylProjection";
   }
   else if(argumentParser.read("--input-proj"))
   {
      theProjectionType = OSSIM_INPUT_PROJECTION;
   }

   if(argumentParser.read("--view-template", stringParam))
   {
      theTemplateView = ossimFilename(tempString);
      theProjectionType = OSSIM_EXTERNAL_PROJECTION;
   }
   if(argumentParser.read("--geo-scaled", doubleParam))
   {
      theProjectionType = OSSIM_GEO_PROJECTION;
      theProjectionName = "ossimEquDistCylProjection";
      if ( (tempDouble < 90.0) && (tempDouble > -90.0) )
      {
         theGeographicOriginOfLatitude = tempDouble;
      }
      else
      {
         theGeographicOriginOfLatitude = 0.0;
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimOrthoIgen::initialize WARNING:"
            << "\nLatitude out  of range!  Must be between -90 and 90."
            << "\nRemains at zero."
            << std::endl;
      }
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "\ngeographicOriginOfLatitude:  " << theGeographicOriginOfLatitude
            << std::endl;
      }
   }
   if(argumentParser.read("--combiner-type", stringParam))
   {
      theCombinerType = tempString;
   }
   else
   {
      theCombinerType = "ossimImageMosaic";
   }
   if(argumentParser.read("--resample-type", stringParam))
   {
      theResamplerType = tempString;
   }
   if(traceDebug())
   {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::initialize DEBUG:"
            << "Leaving..... " 
            << std::endl;
   }
}

void ossimOrthoIgen::addFiles(ossimArgumentParser& argumentParser,
                              bool withDecoding,
                              ossim_uint32 startIdx)
{
   ossim_uint32 idx = startIdx;
   while(argumentParser.argv()[idx])
   {
      addFile(ossimFilename(argumentParser.argv()[idx]), withDecoding);
//      theFilenames.push_back(ossimFilename(argumentParser.argv()[idx]));
      ++idx;
   }
}

bool ossimOrthoIgen::execute()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::execute DEBUG: Entered ..."
         << std::endl;
   }
//   double start=0, stop=0;

   if(theFilenames.size() < 1)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOrthoIgen::execute WARNING: No filenames to process"
         << std::endl;
      return false;
   }

//    if(ossimMpi::instance()->getRank() == 0)
//    {
//       start = ossimMpi::instance()->getTime();
//    }
   ossimKeywordlist igenKwl;
   ossimKeywordlist inputGeom;
   if(ossimMpi::instance()->getRank() == 0)
   {
      if(!setupIgenKwl(igenKwl))
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::execute DEBUG: setupIgenKwl returned false..."
               << std::endl;
         }
         return false;
      }

      if (traceLog())
      {
         const char* lookup = igenKwl.find("object2.filename");
         if (lookup)
         {
            ossimFilename logFile = lookup;
            logFile.setExtension("log");
            igenKwl.write(logFile.c_str());
         }
      }
   }
   
   ossimIgen *igen = new ossimIgen;
   igen->initialize(igenKwl);
   igen->outputProduct();
   
//    if(ossimMpi::instance()->getRank() == 0)
//    {
//       stop = ossimMpi::instance()->getTime();
//       ossimNotify(ossimNotifyLevel_NOTICE)
//          << "Time elapsed: " << (stop-start)
//          << std::endl;
//    }
   delete igen;
   igen = NULL;

   return true;
}

void ossimOrthoIgen::clearFilenameList()
{
   theFilenames.clear();
}

void ossimOrthoIgen::addFile(const ossimFilename& file,
                             bool withEncodedEntry)
{
   ossimOrthoIgenFilename filename;
   if(withEncodedEntry)
   {
      filename.setFilenameWithDecoding(file);
   }
   else
   {
      filename.setFilenameAndEntry(file, -1);
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::addFile DEBUG:"
         << "\nAdded file: " << filename.theFilename
         << std::endl;
   }   

   theFilenames.push_back(filename);
}

void ossimOrthoIgen::setDefaultValues()
{
   theThumbnailRes = "";
   theThumbnailFlag = false;
   theDeltaPerPixelUnit = OSSIM_UNIT_UNKNOWN;
   theDeltaPerPixelOverride.makeNan();
   theTemplateView = "";
   theProjectionType = OSSIM_UNKNOWN_PROJECTION;
   theGeographicOriginOfLatitude = 0.0;
   theCombinerType = "ossimImageMosaic";
   theResamplerType = "nearest neighbor";
   theTilingTemplate = "";
   theTilingFilename = "";
   theSlaveBuffers = "2";
   clearFilenameList();
   theLowPercentClip = ossim::nan();
   theHighPercentClip = ossim::nan();
   theCutOrigin.makeNan();
   theCutDxDy.makeNan();
   theCutOriginUnit   = OSSIM_UNIT_UNKNOWN;
   theCutDxDyUnit     = OSSIM_UNIT_UNKNOWN;
}

void ossimOrthoIgen::setCombinerType(const ossimString& combinerType)
{
   theCombinerType = combinerType;
}

void ossimOrthoIgen::setProjectionName(const ossimString& projName)
{
   // for now we will only support utm or geographic for this.
   theProjectionName = projName;

   if(projName == "ossimUtmProjection")
   {
      theProjectionType = OSSIM_UTM_PROJECTION;
   }
   else if(projName == "ossimEquDistCylProjection")
   {
      theProjectionType = OSSIM_GEO_PROJECTION;
   }
}

void ossimOrthoIgen::setResamplerType(const ossimString& resamplerType)
{
   theResamplerType = resamplerType;
}

void ossimOrthoIgen::setCutCenter(const ossimDpt& dpt,
                                  ossimUnitType unit)
{
   theCutOriginType = ossimOrthoIgen::OSSIM_CENTER_ORIGIN;
   theCutOrigin     = dpt;
   theCutOriginUnit = unit;
}

void ossimOrthoIgen::setCutDxDy(const ossimDpt& dpt,
                                ossimUnitType unit)
{
   theCutDxDy     = dpt;
   theCutDxDyUnit = unit;
}

bool ossimOrthoIgen::setupIgenKwl(ossimKeywordlist& kwl)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupIgenKwl DEBUG: Entered ..."
         << std::endl;
   }
   setupTiling(kwl);
   kwl.add("igen.thumbnail",
           theThumbnailFlag,
           true);
   if((theThumbnailRes != "")&&
      theThumbnailFlag)
   {
      kwl.add("igen.thumbnail_res",
              theThumbnailRes.c_str(),
              true);
   }
   if(theSlaveBuffers == "")
   {
      kwl.add("igen.slave_tile_buffers",
              2,
              true);
   }
   else
   {
      kwl.add("igen.slave_tile_buffers",
              theSlaveBuffers.c_str(),
              true);
   }
   ossim_uint32 inputFileIdx = 0;
   if(!setupView(kwl))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimOrthoIgen::execute DEBUG: setupView returned false..."
            << std::endl;
      }
       return false;
   }

   if(theFilenames.size() > 1)
   {
      ossimKeywordlist tempKwl;
      ossimRefPtr<ossimObject> obj;
      
      ossimRefPtr<ossimConnectableObject> mosaicObject;
      tempKwl.clear();
      if(theCombinerTemplate.exists())
      {
         tempKwl.clear();
         tempKwl.addFile(theCombinerTemplate);
      }
      mosaicObject = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(tempKwl);
      if(!PTR_CAST(ossimImageCombiner,
                  mosaicObject.get()))
      {
         mosaicObject = 0;
      }
      if(!mosaicObject.valid())
      {
         mosaicObject = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(theCombinerType);
      }
      if(!mosaicObject)
      {
         mosaicObject = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(ossimString("ossimImageMosaic"));
      }
      
      if(!mosaicObject)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Unable to create a mosaic object" << std::endl;
         return false;
      }
      bool orthoMosaic = (PTR_CAST(ossimOrthoImageMosaic, mosaicObject.get())!=0);

      tempKwl.clear();
      if(theChainTemplate.exists())
      {
         tempKwl.addFile(theChainTemplate);
         if(tempKwl.find("type"))
         {
            obj = ossimObjectFactoryRegistry::instance()->createObject(tempKwl);
         }
         else if(tempKwl.find("object1.type"))
         {
            obj = ossimObjectFactoryRegistry::instance()->createObject(tempKwl, "object1.");  
         }
      }
      ossimRefPtr<ossimImageChain> chain = PTR_CAST(ossimImageChain, obj.get());
      
      if(!chain)  // then create a default rendering chain
      {
         ossimImageChain*    tempChain = new ossimImageChain;
         if(theProjectionType != OSSIM_UNKNOWN_PROJECTION)
         {
            if(!orthoMosaic)
            {
               ossimImageRenderer* tempObject   = new ossimImageRenderer;
               if(tempObject->getResampler())
               {
                  tempObject->getResampler()->setFilterType(theResamplerType);
               }
               if(tempObject)
               {
                  tempChain->addChild(tempObject);
               }
            }
         }
         obj   = tempChain;
         chain = tempChain;
      }
      
      ossim_uint32 fileSize = theFilenames.size()-1;
      ossim_uint32 idx;
      ossim_uint32 chainIdx = 1;
      ossimRefPtr<ossimImageChain> rootChain = new ossimImageChain;
      chain->saveState(kwl, "object1.object1.");
      for(idx = inputFileIdx;idx < fileSize; ++idx)
      {
         // first lets add an input to the chain
         //
         ossimFilename input  = theFilenames[idx].theFilename;
         ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(input);
         ossimHistogramRemapper* histRemapper = 0;
         if(handler.valid())
         {
            std::vector<ossim_uint32> entryList;
            if(theFilenames[idx].theEntry >-1)
            {
               entryList.push_back(theFilenames[idx].theEntry);
            }
            else
            {
               handler->getEntryList(entryList);
            }

            ossim_uint32 entryIdx = 0;
            for(entryIdx = 0; entryIdx < entryList.size(); ++entryIdx)
            {
               ossimImageHandler* h = (ossimImageHandler*)handler->dup();
               h->setCurrentEntry(entryList[entryIdx]);
               ossimImageChain* tempChain = (ossimImageChain*)chain->dup();
               tempChain->addLast(h);
               if( ( (ossim::isnan(theHighPercentClip) == false)  &&
                     (ossim::isnan(theLowPercentClip) == false) ) ||
                   theUseAutoMinMaxFlag )
               {
                  ossimFilename inputHisto = handler->createHistogramFilename();
                  if(inputHisto.exists())
                  {
                     tempChain->addLast(histRemapper);
                     histRemapper = new ossimHistogramRemapper;
                     tempChain->insertRight(histRemapper, h);
                     histRemapper->openHistogram(inputHisto);
                     if((ossim::isnan(theHighPercentClip) == false) &&
                        (ossim::isnan(theLowPercentClip) == false) )
                     {
                        histRemapper->setHighNormalizedClipPoint(1.0-theHighPercentClip);
                        histRemapper->setLowNormalizedClipPoint(theLowPercentClip);
                     }
                     else
                     {
                        histRemapper->setStretchMode(ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX);
                        histRemapper->buildTable();
                     }
                  }
                  else
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        <<"Histogram file " << inputHisto
                        << " not found!"
                        << std::endl;
                  }
               }
               tempChain->makeUniqueIds();
               tempChain->saveState(kwl, (ossimString("object1.object")+ossimString::toString(chainIdx)+".").c_str());
               rootChain->add(tempChain);
               ++chainIdx;
               mosaicObject->connectMyInputTo(tempChain);
            }
         }
         else if(input == "NULL_FILE")
         {
            ossimImageChain* tempChain = (ossimImageChain*)chain->dup();
            tempChain->makeUniqueIds();
            tempChain->saveState(kwl, (ossimString("object1.object")+ossimString::toString(chainIdx)+".").c_str());
            rootChain->add(tempChain);
            ++chainIdx;
            mosaicObject->connectMyInputTo(tempChain);
         }
      }
      mosaicObject->saveState(kwl, (ossimString("object1.object")+
                                    ossimString::toString(chainIdx)+".").c_str());

      ossimRefPtr<ossimConnectableObject> input = mosaicObject;
      ++chainIdx;
      if(orthoMosaic)
      {
         ossimRefPtr<ossimImageRenderer> tempObject   = new ossimImageRenderer;
         tempObject->getResampler()->setFilterType(theResamplerType);
         tempObject->connectMyInputTo(input.get());
         tempObject->saveState(kwl,
                              (ossimString("object1.object")+
                               ossimString::toString(chainIdx)+".").c_str());
         input = tempObject.get();
      }

      // Annotation setup...
      ossimRefPtr<ossimConnectableObject> annotator =
         setupAnnotation(kwl, input.get());
      if (annotator.valid())
      {
         annotator->saveState(
            kwl,
            (ossimString("object1.object")+
             ossimString::toString(chainIdx)+".").c_str());
         input = annotator;
         ++chainIdx;
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::setupIgenKwl DEBUG:\n"
               << "setupAnnotation failed..."
               << std::endl;
         }
         // return false; Not sure if we want this to be an error???
      }
      
      ossimRefPtr<ossimConnectableObject> cutter = setupCutter(kwl,
                                                               input.get());
      
      if(cutter.valid())
      {
         cutter->saveState(kwl, (ossimString("object1.object")+
                                 ossimString::toString(chainIdx)+".").c_str());
         input = cutter;
      }
                  
      rootChain = 0;
      kwl.add("object1.type",
              "ossimImageChain",
              true);

      if(!setupWriter(kwl, input.get()))
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::execute DEBUG: setupWriter returned false..."
               << std::endl;
         }
         return false;
      }
      
   }

   return true;
}

ossimRefPtr<ossimConnectableObject> ossimOrthoIgen::setupCutter(
   ossimKeywordlist& kwl, ossimConnectableObject* input)
{
   ossimRefPtr<ossimConnectableObject> result;
   if(theCutDxDy.hasNans())
   {
      return result;
   }
   ossimRefPtr<ossimProjection> viewProj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, "product.projection.");
   
   if(viewProj.valid())
   {
      ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, viewProj.get());
      if(mapProj)
      {
         ossimGpt originLatLon(theCutOrigin.lat,
                               theCutOrigin.lon,
                               0.0,
                               mapProj->getDatum());

         if(!mapProj->isGeographic())  // projection in meters...
         {
            // Convert the cut radius to meters if not already.
            ossimDpt dxdy;
            if (theCutDxDyUnit == OSSIM_METERS)
            {
               dxdy = theCutDxDy;
            }
            else
            {
               dxdy.x = ossimUnitConversionTool(
                  theCutDxDy.x, theCutDxDyUnit).getValue(OSSIM_METERS);
               dxdy.y = ossimUnitConversionTool(
                  theCutDxDy.y, theCutDxDyUnit).getValue(OSSIM_METERS);
            }

            ossimEastingNorthingCutter* cutter = new ossimEastingNorthingCutter;
            ossimDpt pt, originEn;
            mapProj->worldToLineSample(originLatLon,
                                       pt);
            pt = ossimIpt(pt);
            mapProj->lineSampleToEastingNorthing(pt, originEn);
            
            ossimDpt ul, lr;
            
            switch(theCutOriginType)
            {
               case ossimOrthoIgen::OSSIM_CENTER_ORIGIN:  
               {
                  ul = originEn + ossimDpt(-dxdy.x,
                                           dxdy.y);
                  lr = originEn + ossimDpt(dxdy.x,
                                           -dxdy.y);
                  break;
               }
               case ossimOrthoIgen::OSSIM_UPPER_LEFT_ORIGIN:
               {
                  ul = originEn;
                  lr = originEn + ossimDpt(dxdy.x,
                                           -dxdy.y);
                  
                  break;
               }
               default:
               {
                  break;
               }
            }
            
            cutter->setEastingNorthingRectangle(ul, lr);
            
            result = cutter;
         }
         else // geographic projection, units = decimal degrees.
         {
            // Convert the cut radius to meters if not already.
            ossimDpt dxdy;
            if (theCutDxDyUnit == OSSIM_DEGREES)
            {
               dxdy = theCutDxDy;
            }
            else
            {
               dxdy.x = ossimUnitConversionTool(
                  theCutDxDy.x, theCutDxDyUnit).getValue(OSSIM_DEGREES);
               dxdy.y = ossimUnitConversionTool(
                  theCutDxDy.y, theCutDxDyUnit).getValue(OSSIM_DEGREES);
            }
            
            ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
            std::vector<ossimGpt> polygon;
               
            switch(theCutOriginType)
            {
               case ossimOrthoIgen::OSSIM_CENTER_ORIGIN:  
               {
                  ossimGpt ul(originLatLon.latd() + dxdy.y,
                              originLatLon.lond() - dxdy.x);
                  ossimGpt ur(originLatLon.latd() + dxdy.y,
                              originLatLon.lond() + dxdy.x);
                  ossimGpt lr(originLatLon.latd() - dxdy.y,
                              originLatLon.lond() + dxdy.x);
                  ossimGpt ll(originLatLon.latd() - dxdy.y,
                              originLatLon.lond() - dxdy.x);
                  
                  polygon.push_back(ul);
                  polygon.push_back(ur);
                  polygon.push_back(lr);
                  polygon.push_back(ll);
                  break;
               }
               case ossimOrthoIgen::OSSIM_UPPER_LEFT_ORIGIN:
               {
                  ossimGpt ul(originLatLon.latd(),
                              originLatLon.lond());
                  ossimGpt ur(originLatLon.latd(),
                              originLatLon.lond() + dxdy.x);
                  ossimGpt lr(originLatLon.latd() - dxdy.y,
                              originLatLon.lond() + dxdy.x);
                  ossimGpt ll(originLatLon.latd() - dxdy.y,
                              originLatLon.lond());
                  
                  polygon.push_back(ul);
                  polygon.push_back(ur);
                  polygon.push_back(lr);
                  polygon.push_back(ll);
                  
                  break;
               }
               default:
               {
                  break;
               }
            }
            cutter->setNumberOfPolygons(1);
            cutter->setPolygon(polygon);
            result = cutter;
         }
      }
   }

   if(result.valid())
   {
      result->connectMyInputTo(input);
   }
   
   return result;
}

bool ossimOrthoIgen::setupWriter(ossimKeywordlist& kwl, ossimConnectableObject* input)
{
   ossimFilename outputFilename = theFilenames[theFilenames.size()-1].theFilename;
   ossimImageFileWriter* writer = 0;
   if(theTilingFilename!="")
   {
      if(!outputFilename.isDir())
      {
         outputFilename = outputFilename.path();
      }
      outputFilename = outputFilename.dirCat(theTilingFilename);
   }

   ossimRefPtr<ossimImageFileWriter> outputObj = 0;
   if((theWriterTemplate == "")||
      (!theWriterTemplate.exists()))
   {
      if(theTilingFilename == "%SRTM%")
      {
         ossimKeywordlist kwlWriter;
         
         kwlWriter.add("type",
                       "ossimGeneralRasterWriter",
                       true);
         kwlWriter.add("byte_order",
                       "big_endian");
         outputObj = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlWriter);
         outputFilename = outputFilename.path();
      }
      else if(outputFilename.ext() != "")
      {
         ossimFilename ext = outputFilename.ext();
         outputObj = ossimImageWriterFactoryRegistry::instance()->createWriterFromExtension(ext);
         if(!outputObj.valid())
         {
            outputObj = new ossimTiffWriter;
            writer = (ossimImageFileWriter*)outputObj.get();
            outputFilename.setExtension("tif");
         }
      }
      else
      {
         // just do a default just so we can set the keywords for now
         //
         outputObj = new ossimTiffWriter;
      }
      if(outputObj.valid())
      {
         writer = (ossimImageFileWriter*)outputObj.get();
         writer->setFilename(outputFilename);

         if(theScaleToEightBitFlag)
         {
            writer->setScaleToEightBitFlag(theScaleToEightBitFlag);
         }
         
         writer->connectMyInputTo(0, input);
         writer->saveState(kwl, "object2.");
      }
      kwl.add("object2.",
              ossimKeywordNames::FILENAME_KW,
              outputFilename.c_str(),
              true);
      
      if(!kwl.find("object2.type"))
      {
         kwl.add("object2.",
                 "type",
                 "ossimImageFileWriter",
                 true);
      }
   }
   else
   {
      ossimKeywordlist kwlTemp;

      kwlTemp.addFile(theWriterTemplate);
      outputObj = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlTemp);
      if(!outputObj.valid())
      {
         outputObj = ossimImageWriterFactoryRegistry::instance()->createWriter(kwlTemp, "object2.");
      }
      if(outputObj.valid())
      {
         outputObj->setFilename(outputFilename);

         if(theScaleToEightBitFlag)
         {
            writer->setScaleToEightBitFlag(theScaleToEightBitFlag);
         }
         
         outputObj->connectMyInputTo(0, input);
         outputObj->saveState(kwl, "object2.");
         kwl.add("object2.",
                 ossimKeywordNames::FILENAME_KW,
                 outputFilename.c_str(),
                 true);
      }
      else
      {
         return false;
      }
   }
   
   return true;
}

bool ossimOrthoIgen::setupView(ossimKeywordlist& kwl)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupView DEBUG:"
         << "Entered...."
         << std::endl;
   }
   ossimKeywordlist tempKwl;
   if(theTemplateView != "")
   {
      tempKwl.addFile(theTemplateView);
   }
   ossimRefPtr<ossimObject> productObj = ossimObjectFactoryRegistry::instance()->createObject(tempKwl, "product.projection.");
   if(!productObj.valid())
   {
      productObj = ossimObjectFactoryRegistry::instance()->createObject(tempKwl);
      if(!PTR_CAST(ossimProjection, productObj.get()))
      {
         productObj = 0;
      }
      else
      {
         if(!theDeltaPerPixelOverride.hasNans())
         {
            ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, productObj.get());

            if(mapProj)
            {
               if(mapProj->isGeographic())
               {
                  mapProj->setDecimalDegreesPerPixel(ossimDpt(ossimUnitConversionTool(theDeltaPerPixelOverride.x,
                                                                                      theDeltaPerPixelUnit).getValue(OSSIM_DEGREES),
                                                              ossimUnitConversionTool(theDeltaPerPixelOverride.y,
                                                                                      theDeltaPerPixelUnit).getValue(OSSIM_DEGREES)));
               }
               else
               {
                  mapProj->setMetersPerPixel(ossimDpt(ossimUnitConversionTool(theDeltaPerPixelOverride.x,
                                                                              theDeltaPerPixelUnit).getValue(OSSIM_METERS),
                                                      ossimUnitConversionTool(theDeltaPerPixelOverride.y,
                                                                              theDeltaPerPixelUnit).getValue(OSSIM_METERS)));
               }
            }
         }
      }
   }
   if(!productObj.valid() && (theTemplateView!=""))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimOrthoIgen::setupView WARNING: File template "
         << theTemplateView << " did not have a valid view" << std::endl;
   }
   if(productObj.valid())
   {
      productObj->saveState(kwl, "product.projection.");
   }
   else
   {
      ossimKeywordlist inputGeom;
      // Open up the image.
      ossimFilename input  = theFilenames[0].theFilename;
      ossimRefPtr<ossimImageHandler> handler =
         ossimImageHandlerRegistry::instance()->open(input);
      if(!handler)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::setupView DEBUG:"
               << "\nCould not open: " << input.c_str()
               << std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::setupView DEBUG:"
               << "Leaving...." << __LINE__
               << std::endl;
         }
         return false;
      }
      handler->getImageGeometry(inputGeom);
      
      // Get the input projection.
      ossimRefPtr<ossimProjection> inputProj =
         ossimProjectionFactoryRegistry::instance()->createProjection(
            inputGeom);
      if(inputProj.valid()&&
         (theProjectionType !=OSSIM_UNKNOWN_PROJECTION))
      {
         // Get the input resolution.
         ossimDpt metersPerPixel = inputProj->getMetersPerPixel();

         double gsdTemp = (metersPerPixel.x+metersPerPixel.y)*.5;
         ossimUnitType gsdUnits = OSSIM_METERS;
         ossimDpt gsd(gsdTemp, gsdTemp);
         if(!theDeltaPerPixelOverride.hasNans())
         {
            gsd      = theDeltaPerPixelOverride;
            gsdUnits = theDeltaPerPixelUnit;
         }
         
         // Get the bounding rect.
         ossimIrect rect = handler->getBoundingRect();
         
         // Create an output projection.
         ossimRefPtr<ossimMapProjection> outputProjection;
         if (theProjectionType == OSSIM_GEO_PROJECTION)
         {
            // Assuming WGS 84 for now.
            outputProjection = new ossimEquDistCylProjection();
            ossimGpt gpt(theGeographicOriginOfLatitude, 0.0);
            
            // Set the origin.
            outputProjection->setOrigin(gpt);
            // Set the resolution.
//             outputProjection->setMetersPerPixel(ossimDpt(gsd, gsd));
            outputProjection->setDecimalDegreesPerPixel(ossimDpt(ossimUnitConversionTool(gsd.x,
                                                                                         gsdUnits).getValue(OSSIM_DEGREES),
                                                                 ossimUnitConversionTool(gsd.y,
                                                                                         gsdUnits).getValue(OSSIM_DEGREES)));
            // Save the state to keyword list.
            outputProjection->saveState(kwl, "product.projection.");
         }
         else if((theProjectionType == OSSIM_UTM_PROJECTION)||
                 ((theProjectionType == OSSIM_INPUT_PROJECTION) &&
                  !PTR_CAST(ossimMapProjection, inputProj.get())))
            
         {
            // Default to UTM.
            ossimUtmProjection* utm = new ossimUtmProjection;
            ossimGpt midGpt;
            
            
            inputProj->lineSampleToWorld(ossimDpt(rect.midPoint()),
                                         midGpt);
            
            utm->setZone(midGpt);
            
            ossimDpt eastingNorthing;
            
            utm->setMetersPerPixel(ossimDpt(ossimUnitConversionTool(gsd.x,
                                                                    gsdUnits).getValue(OSSIM_METERS),
                                            ossimUnitConversionTool(gsd.y,
                                                                    gsdUnits).getValue(OSSIM_METERS)));
            
            eastingNorthing = utm->forward(utm->origin());
            
            utm->setUlEastingNorthing(eastingNorthing);
            outputProjection = utm;
            // Set the resolution.
//             outputProjection->setMetersPerPixel(ossimDpt(gsd, gsd));
            // Save the state to keyword list.
            outputProjection->saveState(kwl, "product.projection.");
         }
         else // input is a map projection so just use that if we are type INPUT
         {
            inputProj->saveState(kwl, "product.projection.");
         }
      }
      else if(theProjectionType == OSSIM_UNKNOWN_PROJECTION)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimOrthoIgen::setupView DEBUG:"
               << "UNKNOWN PROJECTION"
               << std::endl;
         }
      }
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoIgen::setupView DEBUG:"
         << "Leaving...." << __LINE__
         << std::endl;
   }
   return true;
}

ossimRefPtr<ossimConnectableObject> ossimOrthoIgen::setupAnnotation(
   ossimKeywordlist& kwl, ossimConnectableObject* input) const
{
   ossimRefPtr<ossimConnectableObject> result = NULL;
   
   if(theAnnotationTemplate.exists() == false)
   {
      return result;
   }
   
   ossimKeywordlist tempKwl;
   if (tempKwl.addFile(theAnnotationTemplate) == false)
   {
      return result;
   }
   
   ossimRefPtr<ossimObject> obj = ossimObjectFactoryRegistry::instance()->
      createObject(tempKwl, "object1.");
   if (obj.valid())
   {
      ossimGeoAnnotationSource* oga = PTR_CAST(ossimGeoAnnotationSource,
                                               obj.get());
      if (oga)
      {
         // Connect to input.
         oga->connectMyInputTo(input);
         
         // Set the view.
         ossimRefPtr<ossimProjection> viewProj =
            ossimProjectionFactoryRegistry::instance()->createProjection(
               kwl, "product.projection.");
         if (viewProj.valid())
         {
            oga->setProjection(viewProj.get(),
                               false); // False for doesn't own projection.
         }

         result = oga;
      }
   }
   
   return result;
}

bool ossimOrthoIgen::setupTiling(ossimKeywordlist& kwl)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Entered......" << std::endl;
      
   }
   ossimKeywordlist tempKwl;
   ossimFilename outputFilename = theFilenames[theFilenames.size()-1].theFilename;

   if((theTilingTemplate == "")||
      (!tempKwl.addFile(theTilingTemplate)))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Leaving......" << __LINE__ << std::endl;
         
      }
      return false;
   }

   if(outputFilename.isDir())
   {
      if(tempKwl.find("igen.tiling.type"))
      {
         kwl.add(tempKwl);
         theTilingFilename = tempKwl.find("igen.tiling.tile_name_mask");
      }
      else if(tempKwl.find("type"))
      {
         kwl.add("igen.tiling.", tempKwl);
         theTilingFilename = tempKwl.find("tile_name_mask");
      }
   }
   else
   {
      bool tilingEnabled = false;
      theTilingFilename = outputFilename.file();
      if(tempKwl.find("igen.tiling.type"))
      {
         tempKwl.add("igen.tiling.tile_name_mask",
                     theTilingFilename.c_str(),
                     true);
         kwl.add(tempKwl);
         tilingEnabled = true;
      }
      else if(tempKwl.find("type"))
      {
         tempKwl.add("tile_name_mask",
                     theTilingFilename.c_str(),
                     true);
         kwl.add("igen.tiling.", tempKwl);
         tilingEnabled = true;
      }
      if(tilingEnabled) // check for tile mask override
      {
         theFilenames[theFilenames.size()-1].theFilename = theFilenames[theFilenames.size()-1].theFilename.path();
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimOrthoIgen::setupTiling: tempKwl = \n" << tempKwl << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimOrthoIgen::setupTiling: Leaving......" << __LINE__ << std::endl;
   }
   
   return true;
}

void ossimOrthoIgen::setMetersPerPixel(const ossimDpt& mpp)
{
   theDeltaPerPixelOverride = mpp;
   theDeltaPerPixelUnit = OSSIM_METERS;
}

void ossimOrthoIgen::setThumbnailResolution(const ossimIpt& res)
{
   std::ostringstream out;

   out << res.x << " " << res.y;

   theThumbnailRes = out.str().c_str();
}

void ossimOrthoIgen::setThumbnailFlag(bool thumbnailFlag)
{
   theThumbnailFlag = thumbnailFlag;
}
