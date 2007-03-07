//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimIgen.cpp,v 1.18 2005/12/25 17:33:14 dburken Exp $

#include <iterator>
#include <sstream>
#include <exception>

#include <ossimConfig.h> /* To pick up define OSSIM_HAS_MPI. */

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
#    include <mpi.h>
#  endif
#endif

#include <parallel/ossimIgen.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimStdOutProgress.h>

#include <init/ossimInit.h>
#include <projections/ossimProjection.h>
// #include "projections/map_projections/ossimUtmProjection.h>

#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <projections/factory/ossimMapProjectionFactory.h>
#include <imaging/ossimImageChain.h>
#include <base/data_types/ossimKeywordlist.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageChain.h>
#include <imaging/tile_sources/ossimImageRenderer.h>
#include <imaging/tile_sources/ossimRectangleCutFilter.h>
#include <imaging/formats/tiff/ossimTiffWriter.h>
#include <imaging/factory/ossimImageWriterFactoryRegistry.h>
#include <base/data_types/ossimDpt.h>
#include <base/factory/ossimDatumFactory.h>
#include <imaging/factory/ossimImageHandlerRegistry.h>
#include <imaging/formats/ossimImageHandler.h>
#include <projections/ossimImageViewProjectionTransform.h>
#include <base/factory/ossimObjectFactoryRegistry.h>
#include <imaging/annotation/ossimGeoAnnotationSource.h>
#include <base/common/ossimPreferences.h>
#include <base/common/ossimKeywordNames.h>
#include <parallel/ossimMpi.h>
#include <base/common/ossimArgumentParser.h>
#include <base/common/ossimApplicationUsage.h>

// include the Master implementation for the writer
// sequence.
//
#include <parallel/ossimImageMpiMWriterSequenceConnection.h>
#include <parallel/ossimImageMpiSWriterSequenceConnection.h>

// #include <parallel/ossimImageMpiSequenceConnection.h>

static ossimTrace traceDebug(ossimString("ossimIgen:debug"));

ossimIgen::ossimIgen()
   :
    theProductProjection(NULL),
    theOutputRect(),
    theBuildThumbnailFlag(false),
    theThumbnailResolution(0, 0),
    theNumberOfTilesToBuffer(2),
    theKwl(),
    theTilingEnabled(false),
    theTiling(),
    theProgressFlag(true)
{
   theOutputRect.makeNan();
}

ossimIgen::~ossimIgen()
{
   deleteAttributes();
   
}

void ossimIgen::initializeAttributes()
{
   theBuildThumbnailFlag  = false;
   theThumbnailResolution = ossimIpt(0,0);
   theTilingEnabled = false;

   if(ossimMpi::instance()->getRank() != 0)
   {
      ossimPreferences::instance()->addPreferences(theKwl,
                                                   "preferences.",
                                                   true);
   }

   const char* lookup = theKwl.find("igen.output_progress");
   if (lookup)
   {
      ossimString os = lookup;
      theProgressFlag = os.toBool();
   }
   
   const char* thumbnailStr = theKwl.find("igen.thumbnail");
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Thumbnail string = " << ossimString(thumbnailStr) << std::endl;
   }
   if(thumbnailStr)
   {
      theBuildThumbnailFlag= ossimString(thumbnailStr).toBool();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "Generate thumbnail attribute is set to "
            << theBuildThumbnailFlag << std::endl;
      }
      if(theBuildThumbnailFlag)
      {
         const char* resStr = theKwl.find("igen.thumbnail_res");
         if(resStr)
         {
            theThumbnailResolution = ossimIpt(0,0);
            std::istringstream in(resStr);
            in >> theThumbnailResolution.x >> theThumbnailResolution.y;

            if(theThumbnailResolution.x < 1)
            {
               theThumbnailResolution.x = 128;
            }
            if(theThumbnailResolution.y < 1)
            {
               theThumbnailResolution.y = theThumbnailResolution.x;
            }
//            theThumbnailResolution = ossimString(resStr).toInt32();
            
         }
         else
         {
            theThumbnailResolution = ossimIpt(128, 128);
         }
      }
   }
   const char* numberOfSlaveTileBuffersStr = theKwl.find("igen.slave_tile_buffers");
   if(numberOfSlaveTileBuffersStr)
   {
      theNumberOfTilesToBuffer = ossimString(numberOfSlaveTileBuffersStr).toLong();
   }

   const char* tilingKw = theKwl.find("igen.tiling.type");
   if(tilingKw)
   {
      theTilingEnabled = true;
      if(!theTiling.loadState(theKwl, "igen.tiling."))
      {
         theTilingEnabled = false;
      }
   }
}

void ossimIgen::slaveSetup()
{
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   int stringSize;
   MPI_Status status;
   int numberOfTimes = 0;

   // we first need to receive the size of the keyword list to load
   MPI_Recv(&stringSize,
            1,
            MPI_INT,
            0,    // source
            0,    // tag
            MPI_COMM_WORLD,
            &status);

   if(status.MPI_ERROR != MPI_SUCCESS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "SLAVE = "
         << ossimMpi::instance()->getRank()
         << "Had errors receiving!!!!" << std::endl;
      return;
   }
   
   char* buf = new char[stringSize+1];

   numberOfTimes = 0;

   // now lets get the keywordlist as a string so we can load it up
   MPI_Recv(buf,
            stringSize,
            MPI_CHAR,
            0, // source
            0, // tag
            MPI_COMM_WORLD,
            &status);
   
   if(status.MPI_ERROR != MPI_SUCCESS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "SLAVE = " << ossimMpi::instance()->getRank()
         << "Had errors receiving in ossimIgen::slaveCreate(!!!!"
         << std::endl;
      return;
   }
   buf[stringSize] = '\0';

   if(status.MPI_ERROR != MPI_SUCCESS)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "SLAVE = " << ossimMpi::instance()->getRank()
         << "Had errors receiving!!!!" << std::endl;
      return;
   }

   // now lets convert the received keywordlist into an actual
   // ossimKeywordlist by using the parsStream method.
   std::ostringstream kwlStream;

   kwlStream << buf << ends;
   
   istringstream kwlInStream(kwlStream.str());
   theKwl.clear();
   theKwl.parseStream(kwlInStream);
   initializeAttributes();
   delete buf;
   buf = NULL;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "****************** KWL ************************" << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << theKwl << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "**************** END KWL ************************" << std::endl;
      
   }
   loadProduct();
#  endif
#endif
}

void ossimIgen::initialize(const ossimKeywordlist& kwl)
{
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   if(ossimMpi::instance()->getNumberOfProcessors() > 0)
   {
      if(ossimMpi::instance()->getRank() != 0)
      {
         slaveSetup();
         return;
      }
   }
#  endif
#endif
   theKwl = kwl;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "The igen kewyord list  ==== \n" << theKwl << std::endl;
   }
   ossimKeywordlist kwlPrefs = ossimPreferences::instance()->preferencesKWL();
   
   kwlPrefs.addPrefixToAll("preferences.");
   theKwl.add(kwlPrefs);

   initializeAttributes();
   
   // now stream it to all slave processors
   //
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI

   if(ossimMpi::instance()->getNumberOfProcessors() > 0)
   {
      std::ostringstream outputKeywordlist;
      
      theKwl.writeToStream(outputKeywordlist);
      ossimString kwlString = outputKeywordlist.str();
      ossim_uint32 size = kwlString.size();
      
      for(long processor = 1;
          processor < ossimMpi::instance()->getNumberOfProcessors();
          ++processor)
      {
      // let's send the keywordlist argument.
      // This is two steps.  We send a message to
      // indicate the size and then we send the
      // string.
      //
         MPI_Send(&size,
                  1,
                  MPI_INT,
                  processor,
                  0,
                  MPI_COMM_WORLD);
         
         MPI_Send((void*)kwlString.c_str(),
                  size,
                  MPI_CHAR,
                  processor,
                  0,
                  MPI_COMM_WORLD);
      }
   }
#  endif
#endif
   loadProduct();
   
}

bool ossimIgen::loadProduct()//ossimKeywordlist& kwl,
                            //bool thumbnail,
                            //long resolution)
{
   const char* MODULE = "ossimIgen::loadProduct";

   if(traceDebug())
   {
      CLOG << "entered..." << std::endl;
   }

   if(traceDebug())
   {
      CLOG << " Creating object1..." << std::endl;
   }

   theContainer.deleteAllChildren();

   theContainer.loadState(theKwl);

   theProductProjection = buildProductProjection(theKwl);

   if(theProductProjection)
   {
      vector<ossimImageViewTransform*> transforms;
      setView(&theContainer, theProductProjection);
      // if it's a thumbnail then adjust the GSD and
      // reset the view proj to the chain.
      if(theBuildThumbnailFlag)
      {
         buildThumbnail();
         delete theProductProjection;
         
         theProductProjection = buildProductProjection(theKwl);
         
         setView(&theContainer, theProductProjection);
      }
      
   }
   
   ossimKeywordlist proj;
   
   ossimDrect rect;
   ossimImageChain* chain = getChain();
   if(!getOutputRect(theKwl, rect)&&chain)
   {
      rect = chain->getBoundingRect();
   }
   else
   {
      rect.makeNan();
   }
   
   // Stretch the rectangle out to integer boundaries.
   if(!rect.hasNans())
   {
      rect.stretchOut();
   }
   theOutputRect = rect;
   
   if(theProductProjection)
   {
      theProductProjection->saveState(proj, "projection.");
         
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "bounding rect = " << theOutputRect << std::endl;
      CLOG << "leaving..." << std::endl;
   }
   
   return true;
}

void ossimIgen::outputProduct()
{
   ossimImageChain* chain = getChain();
   if(chain)
   {
      chain->initialize();
   }
   ossimImageSourceSequencer* sequencer = NULL;
   
#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
   // only allocate the slave connection if
   // the number of processors is larger than
   // 1
   //
   if(ossimMpi::instance()->getNumberOfProcessors() > 1)
   {
      if(ossimMpi::instance()->getRank()!=0)
      {
         sequencer = new ossimImageMpiSWriterSequenceConnection(NULL,
                                                                 theNumberOfTilesToBuffer);
      }
      else
      {
         sequencer = new ossimImageMpiMWriterSequenceConnection();
      }
   }
   else
   {
      sequencer = new ossimImageSourceSequencer();
   }
#  else

   // we will just load a serial connection if MPI is not supported.
   //
   sequencer = new ossimImageSourceSequencer(NULL);
   
#  endif
#else

   // we will just load a serial connection if MPI is not supported.
   sequencer = new ossimImageSourceSequencer(NULL);
   
#endif
   
   std::vector<ossimConnectableObject*> imageWriters = theContainer.findAllObjectsOfType(STATIC_TYPE_INFO(ossimImageFileWriter),
                                                                                         false);
   if(imageWriters.size() > 0)
   {
      ossimImageFileWriter* writer = PTR_CAST(ossimImageFileWriter, imageWriters[0]);

      
      writer->changeSequencer(sequencer);
      writer->connectMyInputTo(chain);
      writer->initialize();
   }
   else
   {
      delete sequencer;
      sequencer = NULL;
   }
   
   std::vector<ossimConnectableObject*> processObjects = theContainer.findAllObjectsOfType(STATIC_TYPE_INFO(ossimProcessInterface),
                                                                                           false);
   ossim_uint32 i = 0;
   ossimImageFileWriter* writer  = 0;
   for(i = 0; ((i < processObjects.size())&&(!writer)); ++i)
   {
      writer = PTR_CAST(ossimImageFileWriter,
                        processObjects[i]);
   }

   if(writer)
   {
      ossimRefPtr<ossimProjection> proj = buildProductProjection(theKwl);
      ossimMapProjection *mapProj = PTR_CAST(ossimMapProjection,proj.get());
      
      if(theTilingEnabled&&mapProj)
      {
         ossimRefPtr<ossimMapProjection> view;
         ossimIrect clipRect;
         theTiling.initialize(*mapProj,
                               chain->getBoundingRect());
         ossimRectangleCutFilter* cut = new ossimRectangleCutFilter;
         
         chain->addFirst(cut);
         ossim_uint32 idx = 0;
         ossimFilename tempFile = writer->getFilename();

         if(!tempFile.isDir())
         {
            tempFile = tempFile.path();
         }
         ossimString tileName;
         
         while(theTiling.next(view,
                              clipRect,
                              tileName))
         {
            ossimStdOutProgress* prog = NULL;
            if ( (ossimMpi::instance()->getRank() == 0) && theProgressFlag)
            {
               // Add a listener to master.
               prog = new ossimStdOutProgress(0, true);
               writer->addListener(prog);
            }
            
            setView(chain, view.get());
            chain->initialize();
            cut->setRectangle(clipRect);
            chain->initialize();
            writer->disconnect();
            writer->connectMyInputTo(chain);
            writer->setFilename(tempFile.dirCat(tileName));
            writer->initialize();

            try
            {
               writer->execute();
            }
            catch(std::exception& e)
            {
               ossimNotify(ossimNotifyLevel_FATAL)
                  << "ossimIgen::outputProduct ERROR:\n"
                  << "Caught exception!\n"
                  << e.what()
                  << std::endl;
            }
            catch(...)
            {
               ossimNotify(ossimNotifyLevel_FATAL)
                  << "ossimIgen::outputProduct ERROR:\n"
                  << "Unknown exception caught!\n"
                  << std::endl;
            }

            ++idx;

            if (prog)
            {
               writer->removeListener(prog);
               delete prog;
               prog = NULL;
            }
            
         }
      }
      else
      {
         ossimStdOutProgress* prog = NULL;
         if ( (ossimMpi::instance()->getRank() == 0) && theProgressFlag)
         {
            // Add a listener to master.
            prog = new ossimStdOutProgress(0, true);
            writer->addListener(prog);
         }

         try
         {
            writer->execute();
         }
         catch(std::exception& e)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "ossimIgen::outputProduct ERROR:\n"
               << "Caught exception!\n"
                  << e.what()
               << std::endl;
         }
         catch(...)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "ossimIgen::outputProduct ERROR:\n"
               << "Unknown exception caught!\n"
               << std::endl;
         }

         if (prog)
         {
            writer->removeListener(prog);
            delete prog;
            prog = NULL;
         }
      }
   }

   if(ossimMpi::instance()->getRank() == 0)
   {
      for(ossim_uint32 i = 0; i < processObjects.size(); ++i)
      {
         if(!PTR_CAST(ossimImageFileWriter,
                      processObjects[i]))
         {
            ossimProcessInterface* process = PTR_CAST(ossimProcessInterface,
                                                      processObjects[i]);
            if(process)
            {
               processObjects[i]->connectMyInputTo(chain);
               process->execute();
            }
         }
      }
   }
}

void ossimIgen::deleteAttributes()
{
   if(theProductProjection)
   {
      delete theProductProjection;
      theProductProjection = NULL;
   }
   theContainer.deleteAllChildren();
}

ossimProjection* ossimIgen::createImageProjection(const ossimKeywordlist& kwl)
{
   // here we need to modify the factory to produce the correct
   // projector so we can seemlessly use sensors as well.  For
   // now we hardcode the Map projector
   ossimProjection* imageProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   
   if(!imageProjection)
   {
      
//       cerr << "Unable to create Image projection: "
//            << ossimString(kwl.find("projection.type"))
//            << " in ossimIgen::createImageProjection" << endl;
      // ERROR
   }

   return imageProjection;   
}

ossimProjection* ossimIgen::buildProductProjection(const ossimKeywordlist& kwl,
                                                   const char* prefix)
{
   ossimKeywordlist kwlProj;

   ossimString projPrefix = ossimString(prefix) + "projection.";
   const char* ul_lat = kwl.find(prefix, ossimKeywordNames::UL_LAT_KW);
   const char* ul_lon = kwl.find(prefix, ossimKeywordNames::UL_LON_KW);
   const char* lr_lat = kwl.find(prefix, ossimKeywordNames::LR_LAT_KW);
   const char* lr_lon = kwl.find(prefix, ossimKeywordNames::LR_LON_KW);
   const char* origin_latitude    = kwl.find(projPrefix.c_str(),
                                             ossimKeywordNames::ORIGIN_LATITUDE_KW);
   const char* central_meridian   = kwl.find(projPrefix.c_str(),
                                             ossimKeywordNames::CENTRAL_MERIDIAN_KW);
   double      originLat = 0.0;
   double      originLon = 0.0;
   
   ossimProjection* projection = NULL;
   projection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, "product.projection.");
   
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, projection);
   

   if(mapProj)
   {
      ossimKeywordlist projKwl;
      mapProj->saveState(projKwl);

      // allow one to override
      if(origin_latitude&&central_meridian)
      {
         originLat = ossimString(origin_latitude).toDouble();
         originLon = ossimString(central_meridian).toDouble();
      }
      // default
      else if(ul_lat&&ul_lon&&lr_lat&&lr_lon)
      {
         double ulLat = ossimString(ul_lat).toDouble();
         double ulLon = ossimString(ul_lon).toDouble();
         double lrLat = ossimString(lr_lat).toDouble();
         double lrLon = ossimString(lr_lon).toDouble();

         originLat = (ulLat + lrLat)*.5;
         originLon = (ulLon + lrLon)*.5;
               
         if (origin_latitude)
         {
            originLat = ossimString(origin_latitude).toDouble();
         }
         if (central_meridian)
         {
            originLon = ossimString(central_meridian).toDouble();
         }
      }

      if(origin_latitude)
      {
         projKwl.add(ossimKeywordNames::ORIGIN_LATITUDE_KW,
                     originLat,
                     true);
      }

      if(central_meridian)
      {
         projKwl.add(ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                     originLon,
                     true);
      }
      delete mapProj;
      
      projection = ossimProjectionFactoryRegistry::instance()->createProjection(projKwl);

   }
   else
   {
      
//       cout << "ERROR: not all product information was supplied. \n"
//            << "     need:  ul_lat, ul_lon, lr_lat lr_lon (area of coverage)\n"
//            << "            projection datum and projection type";
   }

   
   return projection;
}

void ossimIgen::setView(ossimConnectableObject* obj,
                        ossimObject* viewObj)
{
   if(!viewObj||!obj)
   {
      return;
   }
   vector<ossimConnectableObject*> result;
   obj->findAllInputsOfType(result, STATIC_TYPE_INFO(ossimViewInterface), true, true);

   for(ossim_uint32 i = 0; i < result.size();++i)
   {
      ossimViewInterface* viewInterface = PTR_CAST(ossimViewInterface,
                                                   result[i]);
      if(viewInterface)
      {
         ossimObject* obj = viewObj->dup();
         viewInterface->setView(obj, true);
      }
   }
}


bool ossimIgen::getOutputRect(const ossimKeywordlist& kwl,
                              ossimDrect& rect)
{
   ossimProjection* viewProj = buildProductProjection(kwl);
   ossimImageChain* chain = getChain();
   
   
   
   if(chain)
   {
      setView(chain, viewProj);
      chain->initialize();
      rect = chain->getBoundingRect();
      
      return true;
   }
   if(viewProj)
   {
      delete viewProj;
   }

   return false;
}

void ossimIgen::buildThumbnail()
{
   double resolution = ossimMax(theThumbnailResolution.x, theThumbnailResolution.y);
   ossimProjection* viewProj   = buildProductProjection(theKwl);
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, viewProj);
   
   
   ossimDrect rect;

   if(mapProj && getOutputRect(theKwl, rect))
   {
      double xScale = rect.width()  / (double)resolution;
      double yScale = rect.height() / (double)resolution;

      //---
      // Maintain aspect ratio - adjust based on largest Scale.
      // 
      // Note the second argument to applyScale is to
      // "not recenter the tie point".  This was added to fix a resampler
      // runaway split problem. (drb 20051225)
      //---
      if (xScale > yScale)
      {
         mapProj->applyScale(ossimDpt(xScale, xScale), false);
      }
      else
      {
         mapProj->applyScale(ossimDpt(yScale, yScale), false);
      }
      mapProj->saveState(theKwl, "product.projection.");
   }
   
   delete viewProj;
}

ossimImageChain* ossimIgen::getChain()
{
   ossimConnectableObject* obj = theContainer.findFirstObjectOfType(STATIC_TYPE_NAME(ossimImageChain),
                                                                    false);

   return PTR_CAST(ossimImageChain, obj);
}
