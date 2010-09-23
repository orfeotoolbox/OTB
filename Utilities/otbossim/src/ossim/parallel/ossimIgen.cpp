//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimIgen.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <iterator>
#include <sstream>
#include <exception>

#include <ossim/ossimConfig.h> /* To pick up define OSSIM_HAS_MPI. */

#if OSSIM_HAS_MPI
#  include <mpi.h>
#  include <ossim/parallel/ossimImageMpiMWriterSequenceConnection.h>
#  include <ossim/parallel/ossimImageMpiSWriterSequenceConnection.h>
#endif

#include <ossim/parallel/ossimIgen.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/parallel/ossimMpi.h>

static ossimTrace traceDebug(ossimString("ossimIgen:debug"));
static ossimTrace traceLog(ossimString("ossimIgen:log"));

ossimIgen::ossimIgen()
:
theContainer(new ossimConnectableContainer()),
theProductProjection(0),
theBuildThumbnailFlag(false),
theThumbnailSize(0, 0),
theNumberOfTilesToBuffer(2),
theKwl(),
theTilingEnabled(false),
theTiling(),
theProgressFlag(true),
theStdoutFlag(false)
{
   theOutputRect.makeNan();
}

ossimIgen::~ossimIgen()
{
   theProductProjection = 0;
   theContainer->disconnect();
   theContainer->deleteAllChildren();
   theContainer = 0;
}

void ossimIgen::initializeAttributes()
{
   theBuildThumbnailFlag  = false;
   theThumbnailSize = ossimIpt(0,0);
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
            theThumbnailSize = ossimIpt(0,0);
            std::istringstream in(resStr);
            ossimString x,y;

            in >> x >> y;

            ossim_int32 ix = x.toInt32();
            ossim_int32 iy = y.toInt32();

            if (ix > 0)
            {
               theThumbnailSize.x = ix;
            }
            else
            {
               theThumbnailSize.x = 128;
            }

            if (iy > 0)
            {
               theThumbnailSize.y = iy;
            }
            else
            {
               theThumbnailSize.y = theThumbnailSize.x;
            }
         }
         else
         {
            theThumbnailSize = ossimIpt(128, 128);
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
#if OSSIM_HAS_MPI
   int stringSize;
   MPI_Status status;
   int numberOfTimes = 0;

   memset((void *)&status, 0, sizeof(MPI_Status));


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

   memset((void *)&status, 0, sizeof(MPI_Status));

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
   delete [] buf;
   buf = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "****************** KWL ************************" << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << theKwl << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "**************** END KWL ************************" << std::endl;

   }
   loadProductSpec();
#endif
}

void ossimIgen::initialize(const ossimKeywordlist& kwl)
{
#if OSSIM_HAS_MPI
   if(ossimMpi::instance()->getNumberOfProcessors() > 0)
   {
      if(ossimMpi::instance()->getRank() != 0)
      {
         slaveSetup();
         return;
      }
   }
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
#if OSSIM_HAS_MPI
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
#endif
   loadProductSpec();

}

bool ossimIgen::loadProductSpec()
{
   const char* MODULE = "ossimIgen::loadProductSpec";
   if(traceDebug())  CLOG << "entered..." << std::endl;

   // Clear out the overall container and initialize it with spec in KWL:
   theContainer->deleteAllChildren();
   theContainer->loadState(theKwl);

   // There should be a product chain defined in the container:
   ossimConnectableObject* obj = 
      theContainer->findFirstObjectOfType(STATIC_TYPE_NAME(ossimImageChain), false);
   theProductChain = PTR_CAST(ossimImageChain, obj);
   if (!theProductChain.valid())
   {
      // Search for a connectable container specified that contains the entire product chain:
      ossimConnectableObject* obj2 = 
         theContainer->findFirstObjectOfType(STATIC_TYPE_NAME(ossimImageFileWriter), true);
      ossimImageFileWriter* writer = PTR_CAST(ossimImageFileWriter, obj2);
      if (writer)
         theProductChain = PTR_CAST(ossimImageChain, writer->getInput());

      if (!theProductChain.valid())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE 
            << " -- No processing chain defined for generating product." << std::endl;
         return false; 
      }
   }

   // The output projection is specified separately in the KWL:
   ossimString prefix = "product.projection.";
   theProductProjection = PTR_CAST(ossimMapProjection, 
      ossimProjectionFactoryRegistry::instance()->createProjection(theKwl, prefix));

   const char* lookup = theKwl.find("igen.write_to_stdout");
   if (lookup && ossimString(lookup).toBool())
   {
      theStdoutFlag = true;
   }

   return true;
}

//*************************************************************************************************
//! Writes the output product image. Throws an ossimException if error encountered.
//*************************************************************************************************
void ossimIgen::outputProduct()
{
   // Verify that all vitals have been initialized:
   if (!theProductChain.valid())
   {
      std::string err = "ossimIgen::outputProduct() ERROR:  No product processing chain has yet"
         " been established. Nothing to output!";
      throw(ossimException(err));
   }
   if (!theProductProjection.valid())
   {
      std::string err = "ossimIgen::outputProduct() ERROR:  No product projection has yet"
         " been established. Nothing to output!";
      throw(ossimException(err));
   }

   // Update the chain with the product view specified:
   setView();
   initializeChain();

   // if it's a thumbnail then adjust the GSD and reset the view proj to the chain.
   if(theBuildThumbnailFlag)
      initThumbnailProjection();

   ossimRefPtr<ossimImageSourceSequencer> sequencer = 0;

#if OSSIM_HAS_MPI
   // only allocate the slave connection if the number of processors is larger than 1
   if(ossimMpi::instance()->getNumberOfProcessors() > 1)
   {
      if(ossimMpi::instance()->getRank()!=0)
         sequencer = new ossimImageMpiSWriterSequenceConnection(0, theNumberOfTilesToBuffer);
      else
         sequencer = new ossimImageMpiMWriterSequenceConnection();
   }
#endif

   // we will just load a serial connection if MPI is not supported.
   if (!sequencer.valid())
      sequencer = new ossimImageSourceSequencer(0);

   // Look for the first writer (should be the only writer) in our list of objects:
   ossimRefPtr<ossimImageFileWriter> writer  = 0;
   ossimConnectableObject::ConnectableObjectList imageWriters =
      theContainer->findAllObjectsOfType(STATIC_TYPE_INFO(ossimImageFileWriter), false);
   if (imageWriters.size() == 0)
   {
      sequencer = 0;
      std::string err = "ossimIgen::outputProduct() ERROR:  No image writer object was found in "
         " processing chain.";
      throw(ossimException(err));
   }

   writer = PTR_CAST(ossimImageFileWriter, imageWriters[0].get());
   writer->changeSequencer(sequencer.get());
   writer->connectMyInputTo(theProductChain.get());
   
   // Check for writing to standard output flag. Not all writers support this so check and 
   // throw an error if not supported.
   if (theStdoutFlag)
   {
      if ( writer->setOutputStream(std::cout) == false )
      {
         std::string err = "ERROR:  The write to standard out flag is set; however, writer does "
            "not support going to standard out. Bummer...";
         throw(ossimException(err));
      }
   }
   writer->initialize();

   // If multi-file tiled output is not desired perform simple output, handle special:
   if(theTilingEnabled && theProductProjection.valid())
   {
      theTiling.initialize(*(theProductProjection.get()), theOutputRect);
      ossimRectangleCutFilter* cut = new ossimRectangleCutFilter;
      theProductChain->addFirst(cut);
      
      ossimFilename tempFile = writer->getFilename();
      if(!tempFile.isDir())
         tempFile = tempFile.path();

      ossimString tileName;
      ossimIrect clipRect;

      // 'next' method modifies the mapProj which is the same instance as theProductProjection,
      // so this data member is modified here, then later accessed by setView:
      while(theTiling.next(theProductProjection, clipRect, tileName))
      {
         setView();
         cut->setRectangle(clipRect);
         initializeChain();
         writer->disconnect();
         writer->connectMyInputTo(theProductChain.get());
         writer->setFilename(tempFile.dirCat(tileName));
         writer->initialize();

         if (!writeToFile(writer.get()))
            break;
      }
   }
   else
   {
      // No multi-file tiling, just conventional write to single file:
      writeToFile(writer.get());
   }
}

//*************************************************************************************************
//! Consolidates job of actually writing to the output file.
//*************************************************************************************************
bool ossimIgen::writeToFile(ossimImageFileWriter* writer)
{
   ossimStdOutProgress* prog = 0;
   if ( (ossimMpi::instance()->getRank() == 0) && theProgressFlag)
   {
      // Add a listener to master.
      prog = new ossimStdOutProgress(0, true);
      writer->addListener(prog);
   }

   if (traceLog() && (ossimMpi::instance()->getRank() == 0))
   {
      ossimFilename logFile = writer->getFilename();
      logFile.setExtension(ossimString("log"));

      ossimKeywordlist logKwl;
      writer->saveStateOfAllInputs(logKwl);
      logKwl.write(logFile.c_str());
   }

   try
   {
      writer->execute();
   }

   // Catch internal exceptions:
   catch(std::exception& e)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimIgen::outputProduct ERROR:\n"
         << "Caught exception!\n"
         << e.what()
         << std::endl;
      return false;
   }
   catch(...)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimIgen::outputProduct ERROR:\n"
         << "Unknown exception caught!\n"
         << std::endl;
      return false;
   }

   if (prog)
   {
      writer->removeListener(prog);
      delete prog;
      prog = 0;
   }

   return true;
}

//*************************************************************************************************
//! Initializes all clients of the view projection to the current product projection.
//*************************************************************************************************
void ossimIgen::setView()
{
   if(!theProductChain.valid() || !theProductProjection.valid())
      return;

   // Find all view clients in the chain, and notify them of the new view:
   ossimConnectableObject::ConnectableObjectList clientList;
   theProductChain->findAllInputsOfType(clientList, STATIC_TYPE_INFO(ossimViewInterface), true, true);
   for(ossim_uint32 i = 0; i < clientList.size();++i)
   {
      ossimViewInterface* viewClient = PTR_CAST(ossimViewInterface, clientList[i].get());
      if (viewClient)
         viewClient->setView(theProductProjection->dup());
   }
}

//*************************************************************************************************
//! Modifies the production chain to output redused-resolution thumbnail image.
//*************************************************************************************************
void ossimIgen::initThumbnailProjection()
{
   double thumb_size = ossim::max(theThumbnailSize.x, theThumbnailSize.y);
   ossimMapProjection* mapProj = PTR_CAST (ossimMapProjection, theProductProjection.get());

   if(mapProj && !theOutputRect.hasNans())
   {
      double xScale = theOutputRect.width()  / (double)thumb_size;
      double yScale = theOutputRect.height() / (double)thumb_size;
      double scale = ossim::max(xScale, yScale);
      mapProj->applyScale(ossimDpt(scale, scale), true);
   }

   // Need to change the view in the product chain:
   setView();
   initializeChain();
}

//*************************************************************************************************
// This method is called after a change is made to the product chain. It recomputes the bounding
// rectangle.
//*************************************************************************************************
void ossimIgen::initializeChain()
{
   // Force initialization of the chain to recompute parameters:
   theProductChain->initialize();
   theOutputRect = theProductChain->getBoundingRect();

   // Stretch the rectangle out to integer boundaries.
   if(!theOutputRect.hasNans())
      theOutputRect.stretchOut();
}
