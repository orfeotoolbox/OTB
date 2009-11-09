//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Eric Hirschorn
//
// Description:
//
// Contains class declaration for ossimVirtualImageWriter.
//*******************************************************************
//  $Id: ossimVirtualImageWriter.cpp 13312 2008-07-27 01:26:52Z gpotts $

#include <ossim/ossimConfig.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimImageTypeLut.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimVirtualImageHandler.h>
#include <ossim/imaging/ossimVirtualImageWriter.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>

static ossimTrace traceDebug("ossimVirtualImageWriter:debug");

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimVirtualImageWriter.cpp 13312 2008-07-27 01:26:52Z gpotts $";
#endif

RTTI_DEF3( ossimVirtualImageWriter,
           "ossimVirtualImageWriter",
           ossimOutputSource,
           ossimProcessInterface,
           ossimConnectableObjectListener )

ossimVirtualImageWriter::ossimVirtualImageWriter( const ossimFilename& file,
                                                  ossimImageHandler* inputSource,
                                                  bool overviewFlag )
 : ossimOutputSource(0,
                     1,
                     0,
                     true,
                     true),
     theOutputFile(file),
     theOutputFileTmp(""),
     theOutputSubdirectory("unknown"),
     theVirtualWriterType("unknown"),
     theOutputImageType(ossimImageTypeLut().getEntryString(OSSIM_IMAGE_TYPE_UNKNOWN)),
     theMajorVersion("1.00"), // for the ossimVirtualImageWriter class only
     theMinorVersion("0.00"), // for derived writers to set uniquely 
     theCompressType(1),
     theCompressQuality(75),
     theOutputTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
     theOutputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
     theInputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
     theBytesPerPixel(0),
     theBitsPerSample(0),
     theSampleFormat(0),
     theTileSizeInBytes(0),
     theNullDataBuffer(0),
     theProgressListener(0),
     theCopyAllFlag(false),
     theLimitedScopeUpdateFlag(false),
     theOverviewFlag(overviewFlag),
     theCurrentEntry(0),
     theInputMapProjection(0),
     theDirtyFrameList(0),
     theInputFrameInfoList(0),
     theInputFrameInfoQueue(0),
     theResampleType(ossimFilterResampler::ossimFilterResampler_BOX),
     theImageHandler(0),
     theInputConnection(0),
     theInputGeometry(0),
     theInputProjection(0),
     theAreaOfInterest(0,0,0,0)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualImageWriter::ossimVirtualImageWriter entered..."
         << std::endl;
   }

   ossim::defaultTileSize( theOutputTileSize );

   theImageHandler = inputSource;
   theInputConnection = new ossimImageSourceSequencer( 
                           static_cast<ossimImageSource*>( inputSource ) );
   theInputGeometry = theImageHandler.valid() ? theImageHandler->getImageGeometry() : 0;
   theInputProjection = theInputGeometry.valid() ? theInputGeometry->getProjection() : 0;

   // Retrieve the map projection object from the input image.
   theInputMapProjection = theInputProjection.valid() ? 
      PTR_CAST( ossimMapProjection, theInputProjection.get() ) : 0;

   // The current entry of the input image handler stays the same
   // throughout the lifetime of this virtual image writer.
   theCurrentEntry = theImageHandler->getCurrentEntry();

   // now map the sequencer to the same input
   connectMyInputTo(0, inputSource);
   initialize();

   theInputConnection->connectMyInputTo( 0, inputSource, false );
   theAreaOfInterest.makeNan();

   //---
   // Check for a listeners.  If the list is empty, add a standard out
   // listener so that command line apps like img2rr will get some 
   // progress.
   //---
   if (theListenerList.empty())
   {
      theProgressListener = new ossimStdOutProgress( 0, true );
      addListener( theProgressListener );
      setCurrentMessage(ossimString( "Starting virtual image write...") );
   }

   // See if the input image format is RPF. If it is, do some config.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, theImageHandler.get() );
   if ( pRpf != NULL )
   {
      theInputFrameSize.x = ossimCibCadrgTileSource::CIBCADRG_FRAME_WIDTH;
      theInputFrameSize.y = ossimCibCadrgTileSource::CIBCADRG_FRAME_HEIGHT;

      // Collect the frame file names of RPF image
      int nFramesFound = 0;

      const ossimRpfToc* pRpfToc = pRpf->getToc();

      // Number of directories of images
      unsigned long nEntries = pRpfToc->getNumberOfEntries();
      ossim_uint32 iE;
      for ( iE=0; iE<nEntries; ++iE )
      {
         // Retrieve directory info
         const ossimRpfTocEntry* pEntry = pRpfToc->getTocEntry( iE );
         if ( pEntry != NULL )
         {
            ossim_uint32 nRows = pEntry->getNumberOfFramesVertical();
            ossim_uint32 nCols = pEntry->getNumberOfFramesHorizontal();

            ossim_uint32 iV,iH;
            for ( iV=0; iV<nRows; ++iV )
            {
               for ( iH=0; iH<nCols; ++iH )
               {
                  ossimRpfFrameEntry frameEntry;
                  bool bResult = pEntry->getEntry( iV, iH, frameEntry );
                  if ( bResult == true )
                  {
                     ossimString framePath = frameEntry.getPathToFrameFileFromRoot();
                     if ( framePath.length() > 0 )
                     {
                        InputFrameInfo* info = new InputFrameInfo();
                        if ( info != 0 )
                        {
                           info->name  = framePath;
                           info->entry = iE;
                           info->row   = iV;
                           info->col   = iH;

                           theInputFrameInfoList.push_back( info );
                        }
                        if ( traceDebug() )
                        {
                           ossimNotify(ossimNotifyLevel_WARN)
                              << "Frame file(" << ++nFramesFound << "): "
                              << framePath
                              << " (entry: " << iE << ", row: " << iV << ", col: " << iH << ")" 
                              << std::endl;
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

ossimVirtualImageWriter::~ossimVirtualImageWriter()
{
   int numFrames = (int)theInputFrameInfoList.size();
   int i;
   for ( i=0; i<numFrames; ++i )
   {
      InputFrameInfo* info = theInputFrameInfoList[i];
      if ( info != 0 )
      {
         delete info;
      }
   }
   theInputFrameInfoList.clear();

   theInputConnection = 0;
   theInputGeometry   = 0;
   theInputProjection = 0;

   if ( theProgressListener )
   {
      setCurrentMessage( ossimString("Finished virtual image write...") );
      removeListener( theProgressListener );
      delete theProgressListener;
      theProgressListener = 0;
   }
}

void ossimVirtualImageWriter::initialize()
{
   if( theInputConnection.valid() )
   {
      theInputConnection->initialize();
      setAreaOfInterest( theInputConnection->getBoundingRect() );
   }
}

InputFrameInfo* ossimVirtualImageWriter::getInputFrameInfo( ossimFilename name ) const
{
   static const char MODULE[] = "ossimVirtualImageWriter::getInputFrameInfo";

   // First test: do an unfiltered name compare to find the InputFrameInfo.
   int numFrames = (int)theInputFrameInfoList.size();
   int i;
   for ( i=0; i<numFrames; ++i )
   {
      InputFrameInfo* info = theInputFrameInfoList[i];
      if ( info != 0 )
      {
         if ( (theCurrentEntry == info->entry) &&
              (name == info->name) )
         {
            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << MODULE << " DEBUG:"
                  << "\nRPF frame found: " << name
                  << " using full unfiltered name compare:"
                  << " info->entry=" << info->entry
                  << " info->row=" << info->row
                  << " info->col=" << info->col
                  << std::endl;
            }

            return info;
         }
      }
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nRPF frame not found: " << name
         << " in full unfiltered name compare."
         << "/nWill now try less stringent test."
         << std::endl;
   }

   // Second test: just check the filename and extension with no path check 
   // to find an InputFrameInfo candidate.
   ossimString driveStringIn;
   ossimString pathStringIn;
   ossimString fileStringIn;
   ossimString extStringIn;
   name.split( driveStringIn, 
               pathStringIn, 
               fileStringIn, 
               extStringIn );

   for ( i=0; i<numFrames; ++i )
   {
      InputFrameInfo* info = theInputFrameInfoList[i];
      if ( info != 0 )
      {
         if ( theCurrentEntry == info->entry )
         {
            ossimString driveStringRPF;
            ossimString pathStringRPF;
            ossimString fileStringRPF;
            ossimString extStringRPF;
            info->name.split( driveStringRPF, 
                              pathStringRPF, 
                              fileStringRPF, 
                              extStringRPF );

            if ( (fileStringRPF == fileStringIn) && 
                 (extStringRPF  == extStringIn) )
            {
               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << MODULE << " DEBUG:"
                     << "\nRPF frame candidate found: " << name
                     << " using stripped name compare:"
                     << " info->name=" << info->name
                     << " info->entry=" << info->entry
                     << " info->row=" << info->row
                     << " info->col=" << info->col
                     << std::endl;
               }

               return info;
            }
         }
      }
   }

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nRPF frame not found: " << name
         << std::endl;
   }

   return 0;
}

ossim_int32 ossimVirtualImageWriter::getNumberOfBuiltFrames( ossim_uint32 resLevel,
                                                              bool bPartialBuild ) const
{
   ossim_int32 numFrames = 0;

   if ( bPartialBuild )
   {
      // Since the input frames have potentially overlapping output
      // frame ranges, we make the count accurate by making use of the 
      // isFrameAlreadyDone() member function which allows us to discard
      // the duplicates. 
      ossim_uint32 nQ = (ossim_uint32)theInputFrameInfoQueue.size();
      ossim_uint32 idxQ;
      for ( idxQ=0; idxQ<nQ; ++idxQ )
      {
         InputFrameInfo* pInfo = theInputFrameInfoQueue[idxQ];
         if ( pInfo && pInfo->isValid(resLevel) )
         {
            ossim_int32 xRangeMin = pInfo->xRangeMin[resLevel];
            ossim_int32 xRangeMax = pInfo->xRangeMax[resLevel];
            ossim_int32 yRangeMin = pInfo->yRangeMin[resLevel];
            ossim_int32 yRangeMax = pInfo->yRangeMax[resLevel];

            //***
            // Frame loop in the line (height) direction.
            //***
            ossim_int32 iF;
            for( iF = yRangeMin; iF <= yRangeMax; ++iF )
            {
               //***
               // Frame loop in the sample (width) direction.
               //***
               ossim_int32 jF;
               for( jF = xRangeMin; jF <= xRangeMax; ++jF )
               {
                  // Add check here to see if (jF,iF) has already been processed for
                  // a previous input frame in the queue.
                  bool bIsFrameAlreadyDone = isFrameAlreadyDone( idxQ, resLevel, jF, iF );

                  if ( bIsFrameAlreadyDone == false )
                  {
                     ++numFrames;
                  }
               }
            }
         }
      }
   }
   else
   {
      ossim_int32 imageSamples = theImageHandler->getNumberOfSamples();
      ossim_int32 imageLines   = theImageHandler->getNumberOfLines();

      ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
      imageSamples /= resLevelPow;
      imageLines   /= resLevelPow;

      ossim_int32 frameSamples = theOutputFrameSize.x;
      ossim_int32 frameLines   = theOutputFrameSize.y;

      ossim_int32 framesWide = (imageSamples % frameSamples) ?
                               (imageSamples / frameSamples) + 1 : (imageSamples / frameSamples);
      ossim_int32 framesHigh = (imageLines % frameLines) ?
                               (imageLines / frameLines) + 1 : (imageLines / frameLines);

      numFrames = framesWide * framesHigh;
   }

   return numFrames;
}

ossimIpt ossimVirtualImageWriter::getNumberOfOutputFrames( ossim_uint32 resLevel ) const
{
   ossim_int32 imageSamples = theImageHandler->getNumberOfSamples();
   ossim_int32 imageLines   = theImageHandler->getNumberOfLines();

   ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
   imageSamples /= resLevelPow;
   imageLines   /= resLevelPow;

   ossim_int32 frameSamples = theOutputFrameSize.x;
   ossim_int32 frameLines   = theOutputFrameSize.y;

   ossim_int32 framesWide = (imageSamples % frameSamples) ?
                            (imageSamples / frameSamples) + 1 : (imageSamples / frameSamples);
   ossim_int32 framesHigh = (imageLines % frameLines) ?
                            (imageLines / frameLines) + 1 : (imageLines / frameLines);

   return ossimIpt( framesWide, framesHigh );
}

ossimIpt ossimVirtualImageWriter::getNumberOfOutputTiles( ossim_uint32 resLevel ) const
{
   ossim_int32 imageSamples = theImageHandler->getNumberOfSamples();
   ossim_int32 imageLines   = theImageHandler->getNumberOfLines();

   ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
   imageSamples /= resLevelPow;
   imageLines   /= resLevelPow;

   ossim_int32 tileSamples  = theOutputTileSize.x;
   ossim_int32 tileLines    = theOutputTileSize.y;

   ossim_int32 tilesWide    = (imageSamples % tileSamples) ?
                              (imageSamples / tileSamples) + 1 : 
                              (imageSamples / tileSamples);
   ossim_int32 tilesHigh    = (imageLines % tileLines) ?
                              (imageLines / tileLines) + 1 : 
                              (imageLines / tileLines);

   return ossimIpt( tilesWide, tilesHigh );
}

ossimIpt ossimVirtualImageWriter::getNumberOfTilesPerOutputFrame() const
{
   ossim_int32 frameSamples  = theOutputFrameSize.x;
   ossim_int32 frameLines    = theOutputFrameSize.y;

   ossim_int32 tileSamples   = theOutputTileSize.x;
   ossim_int32 tileLines     = theOutputTileSize.y;

   ossim_int32 tilesWide     = (frameSamples % tileSamples) ?
                               (frameSamples / tileSamples) + 1 : (frameSamples / tileSamples);
   ossim_int32 tilesHigh     = (frameLines % tileLines) ?
                               (frameLines / tileLines) + 1 : (frameLines / tileLines);

   return ossimIpt( tilesWide, tilesHigh );
}

ossimObject* ossimVirtualImageWriter::getObject()
{
   return this;
}

const ossimObject* ossimVirtualImageWriter::getObject() const
{
   return this;
}

void ossimVirtualImageWriter::setOutputImageType( const ossimString& type )
{
   theOutputImageType = type;
}

void ossimVirtualImageWriter::setOutputImageType( ossim_int32 type )
{
   ossimImageTypeLut lut;
   theOutputImageType = lut.getEntryString( type );
}

ossimString ossimVirtualImageWriter::getOutputImageTypeString() const
{
   return theOutputImageType;
}

ossim_int32 ossimVirtualImageWriter::getOutputImageType() const
{
   ossimImageTypeLut lut;
   return lut.getEntryNumber( theOutputImageType );
}

bool ossimVirtualImageWriter::getOverviewFlag() const
{
   return theOverviewFlag;
}

void ossimVirtualImageWriter::setOverviewFlag( bool flag )
{
   theOverviewFlag = flag;
}

ossimIrect ossimVirtualImageWriter::getAreaOfInterest() const
{
   return theAreaOfInterest;
}

void ossimVirtualImageWriter::setAreaOfInterest( const ossimIrect& inputRect )
{
   theAreaOfInterest = inputRect;
   if( theInputConnection.valid() )
   {
      theInputConnection->setAreaOfInterest( inputRect );
   }
}

void ossimVirtualImageWriter::setOutputFile( const ossimFilename& file )
{
   theOutputFile = file;
}

const ossimFilename& ossimVirtualImageWriter::getOutputFile()const
{
   return theOutputFile;
}

bool ossimVirtualImageWriter::canConnectMyInputTo( ossim_int32 inputIndex,
                                                   const ossimConnectableObject* object ) const
{
   return ( object &&
          ( (PTR_CAST(ossimImageSource, object) && (inputIndex == 0)) ) );
}

void ossimVirtualImageWriter::setOutputTileSize( const ossimIpt& tileSize )
{
   if ( theInputConnection.valid() )
   {
      theInputConnection->setTileSize( tileSize );
   }

   theOutputTileSize = tileSize;

   theTileSizeInBytes = theOutputTileSize.x * theOutputTileSize.y * theBytesPerPixel;

   //---
   // Make a buffer to pass to pass to the write tile methods when an image
   // handler returns a null tile.
   //---
   theNullDataBuffer.resize( theTileSizeInBytes );

   // Fill it with zeroes.
   std::fill( theNullDataBuffer.begin(), theNullDataBuffer.end(), 0 );
}

void ossimVirtualImageWriter::setOutputFrameSize( const ossimIpt& frameSize )
{
   theOutputFrameSize = frameSize;
}

bool ossimVirtualImageWriter::getCopyAllFlag() const
{
   return theCopyAllFlag;
}

void ossimVirtualImageWriter::setCopyAllFlag( bool flag )
{
   theCopyAllFlag = flag;
}

ossim_uint32 ossimVirtualImageWriter::getFinalResLevel( ossim_uint32 startRLevel,
                                                        bool bPartialBuild ) const
{
   // Note we always have one rset
   ossim_uint32 rLevel = startRLevel;

   ossimIpt nFrames = getNumberOfOutputFrames( rLevel );
   ossim_uint32 largest = ( nFrames.x > nFrames.y ) ? nFrames.x : nFrames.y;

   while( largest > 1 )
   {
      ++rLevel;

      nFrames = getNumberOfOutputFrames( rLevel );
      largest = (nFrames.x > nFrames.y) ? nFrames.x : nFrames.y;
   }

   return rLevel;
}

ossim_uint32 ossimVirtualImageWriter::getStartingResLevel() const
{
   return theCopyAllFlag ? 0 : 1;
}

// After 1 or more dirty frames are set, an update to the image will
// be of limited scope to areas of the RPF image that come from the 
// frame files found in theDirtyFrameList.
void ossimVirtualImageWriter::setDirtyFrame( const ossimString& name )
{
   // Add the name of a frame to the dirty frame list.
   theDirtyFrameList.push_back(name);
}

bool ossimVirtualImageWriter::isFrameAlreadyDone( ossim_uint32 mQ,
                                                  ossim_uint32 resLevel,
                                                  ossim_int32  frameX, 
                                                  ossim_int32  frameY ) const
{
   bool bFoundIt = false;
   ossim_uint32 nQ = (ossim_uint32)theInputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ && idxQ<mQ; ++idxQ )
   {
      InputFrameInfo* pInfo = theInputFrameInfoQueue[idxQ];
      if ( pInfo && pInfo->isValid() )
      {
         ossim_int32 xRangeMin = pInfo->xRangeMin[resLevel];
         ossim_int32 xRangeMax = pInfo->xRangeMax[resLevel];
         ossim_int32 yRangeMin = pInfo->yRangeMin[resLevel];
         ossim_int32 yRangeMax = pInfo->yRangeMax[resLevel];

         if ( xRangeMin <= frameX && frameX <= xRangeMax &&
              yRangeMin <= frameY && frameY <= yRangeMax )
         {
            bFoundIt = true;
            break;
         }
      }
   }

   return bFoundIt;
}

bool ossimVirtualImageWriter::execute()
{
   static const char MODULE[] = "ossimVirtualImageWriter::execute";

   // If the virtual header file already exists, 
   // let's try to stay consistent with its parameters.
   bool bExists = theOutputFile.exists();
   if ( bExists )
   {
      ossimRefPtr<ossimImageHandler> pHand = ossimImageHandlerRegistry::instance()->open( theOutputFile );
      if ( pHand.valid() )
      {
         ossimVirtualImageHandler* pVirt = PTR_CAST( ossimVirtualImageHandler, pHand.get() );
         if ( pVirt != NULL )
         {
            ossimIpt outputFrameSize;
            outputFrameSize.x = pVirt->getFrameWidth();
            outputFrameSize.y = pVirt->getFrameHeight();

            if ( outputFrameSize.x != theOutputFrameSize.x )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output frame size(x): " 
                  << theOutputFrameSize.x 
                  << " -> "
                  << outputFrameSize.x
                  << ", overridden by pre-existing value found in: "
                  << theOutputFile
                  << std::endl;

               theOutputFrameSize.x = outputFrameSize.x;
            }
            if ( outputFrameSize.y != theOutputFrameSize.y )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output frame size(y): " 
                  << theOutputFrameSize.y 
                  << " -> "
                  << outputFrameSize.y
                  << ", overridden by pre-existing value found in: "
                  << theOutputFile
                  << std::endl;

               theOutputFrameSize.y = outputFrameSize.y;
            }

            ossimIpt outputTileSize;
            outputTileSize.x = pVirt->getTileWidth();
            outputTileSize.y = pVirt->getTileHeight();

            if ( outputTileSize.x != theOutputTileSize.x )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output tile size(x): " 
                  << theOutputTileSize.x 
                  << " -> "
                  << outputTileSize.x
                  << ", overridden by pre-existing value found in: "
                  << theOutputFile
                  << std::endl;

               theOutputTileSize.x = outputTileSize.x;
            }
            if ( outputTileSize.y != theOutputTileSize.y )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output tile size(y): " 
                  << theOutputTileSize.y 
                  << " -> "
                  << outputTileSize.y
                  << ", overridden by pre-existing value found in: "
                  << theOutputFile
                  << std::endl;

               theOutputTileSize.y = outputTileSize.y;
            }
         }

         pHand = 0;
      }
   }

   // Adjust the x-dimensional output frame size to be an integer 
   // multiple of the output tile size (e.g. x1,x2,x3,...) if it isn't 
   // already.
   ossim_int32 modX = theOutputFrameSize.x % theOutputTileSize.x;
   if ( modX != 0 )
   {
      ossim_int32 mulX = (theOutputFrameSize.x / theOutputTileSize.x) + 1;
      if ( mulX == 0 )
      {
         mulX = 1;
      }

      ossimNotify(ossimNotifyLevel_WARN)
         << "Adjusting the requested output frame size(" 
         << theOutputFrameSize.x 
         << ") in x-dim to "
         << mulX << " times the output tile size("
         << theOutputTileSize.x
         << ")"
         << std::endl;

      theOutputFrameSize.x = theOutputTileSize.x * mulX;
   }

   // Adjust the y-dimensional output frame size to be an integer 
   // multiple of the output tile size (e.g. x1,x2,x3,...) if it isn't 
   // already.
   ossim_int32 modY = theOutputFrameSize.y % theOutputTileSize.y;
   if ( modY != 0 )
   {
      ossim_int32 mulY = (theOutputFrameSize.y / theOutputTileSize.y) + 1;
      if ( mulY== 0 )
      {
         mulY = 1;
      }

      ossimNotify(ossimNotifyLevel_WARN)
         << "Adjusting the requested output frame size(" 
         << theOutputFrameSize.y 
         << ") in y-dim to "
         << mulY << " times the output tile size("
         << theOutputTileSize.y
         << ")"
         << std::endl;

      theOutputFrameSize.y = theOutputTileSize.y * mulY;
   }

   // Zero base starting resLevel.
   ossim_uint32 startRLev = getStartingResLevel();

   // Zero base final resLevel.
   ossim_uint32 finalRLev = getFinalResLevel( startRLev );

   // Print out the dirty frames.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, theImageHandler.get() );
   if ( pRpf != NULL )
   {
      // Make sure the queue is empty before we start adding in.
      theInputFrameInfoQueue.clear();

      int nFrames = (int)theDirtyFrameList.size();
      if ( nFrames > 0 )
      {
         ossim_uint32 inputFrameNx  = theInputFrameSize.x;
         ossim_uint32 inputFrameNy  = theInputFrameSize.y;
         ossim_uint32 outputFrameNx = theOutputFrameSize.x;
         ossim_uint32 outputFrameNy = theOutputFrameSize.y;

         int idx;
         for( idx=0; idx<nFrames; ++idx )
         {
            ossimFilename frameTestName = theDirtyFrameList[idx];
            InputFrameInfo* info = getInputFrameInfo( frameTestName );
            if ( info )
            {
               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "\nRange processing input frame file(" << info->name << "): "
                     << "\n(entry: " << info->entry << ", row: " << info->row << ", col: " << info->col << ")" 
                     << std::endl;
               }

               ossim_uint32 inputFrameI  = info->col;
               ossim_uint32 inputFrameJ  = info->row;

               // set the output frame index ranges
               ossim_uint32 r;
               for ( r=0; r<=finalRLev; ++r )
               {
                  ossim_uint32 scale = (ossim_uint32)(1<<r);
                  ossim_uint32 scaledOutputFrameNx = scale * outputFrameNx;
                  ossim_uint32 scaledOutputFrameNy = scale * outputFrameNy;

                  ossim_uint32 inputNthFramePixelBeginX = inputFrameI * inputFrameNx;
                  ossim_uint32 inputNthFramePixelEndX   = inputNthFramePixelBeginX + inputFrameNx - 1;

                  ossim_uint32 inputNthFramePixelBeginY = inputFrameJ * inputFrameNy;
                  ossim_uint32 inputNthFramePixelEndY   = inputNthFramePixelBeginY + inputFrameNy - 1;

                  double xRangeMinD = (double)inputNthFramePixelBeginX / scaledOutputFrameNx;
                  double xRangeMaxD = (double)inputNthFramePixelEndX   / scaledOutputFrameNx;
                  double yRangeMinD = (double)inputNthFramePixelBeginY / scaledOutputFrameNy;
                  double yRangeMaxD = (double)inputNthFramePixelEndY   / scaledOutputFrameNy;

                  ossim_uint32 xRangeMin = (ossim_uint32)xRangeMinD;
                  ossim_uint32 xRangeMax = (ossim_uint32)xRangeMaxD;
                  ossim_uint32 yRangeMin = (ossim_uint32)yRangeMinD;
                  ossim_uint32 yRangeMax = (ossim_uint32)yRangeMaxD;

                  if ( traceDebug() )
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << "\nOutput frame ranges at resLevel: " << r 
                        << "\nxRangeMin: " << xRangeMin << ", xRangeMax: " << xRangeMax
                        << "\nyRangeMin: " << yRangeMin << ", yRangeMax: " << yRangeMax
                        << "\n"
                        << std::endl;
                  }

                  info->xRangeMin.push_back( xRangeMin );
                  info->xRangeMax.push_back( xRangeMax );
                  info->yRangeMin.push_back( yRangeMin );
                  info->yRangeMax.push_back( yRangeMax );
               }

               // add frames to process to queue
               theInputFrameInfoQueue.push_back( info );
            }

            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nFrame file(" << idx+1 << "): "
                  << theDirtyFrameList[idx]
                  << (info ? " will" : " will not")
                  << " be processed."
                  << std::endl;
            }
         }

         if ( theInputFrameInfoQueue.size() > 0 )
         {
            theLimitedScopeUpdateFlag = true;

            // If all the frames for the current input image handler entry have 
            // been entered, we don't have to set theLimitedScopeUpdateFlag to true. 
            // So a check to see if all frames have been added to the list or not
            // can be added.
         }
         else
         {
            // No frames have been found for the current entry of the
            // input image handler. So there's no work to be done and we
            // should bail...
            ossimNotify(ossimNotifyLevel_WARN)
               << "There is nothing to do, so exiting overview write of entry: "
               << theCurrentEntry
               << std::endl;
            return false;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Building entire image..."
            << std::endl;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "\nInput image (" << theImageHandler->getFilename() << ") not an RPF, "
         << "so the entire image is being rebuilt." 
         << std::endl;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nCurrent number of reduced res sets: "
         << theImageHandler->getNumberOfDecimationLevels()
         << "\nStarting reduced res set:    " << startRLev
         << "\nEnding reduced res sets:     " << finalRLev
         << "\nResampling type:             " << theResampleType
         << std::endl;
   }

   if ( ossimMpi::instance()->getRank() == 0 )
   {
      if ( startRLev == 0 )
      {       
         if ( !writeR0() )
         {
            // Set the error...
            ossimSetError( getClassName(),
                           ossimErrorCodes::OSSIM_ERROR,
                           "File %s line %d\nError copying image!",
                           __FILE__,
                           __LINE__ );
            return false;
         }
         else
         {
            ossimKeywordlist temp_header_kwl;
            saveHeaderInfo( temp_header_kwl, startRLev, startRLev );
            temp_header_kwl.write( theOutputFileTmp );
         }
      }
   }

   ossim_uint32 writeRnStartRLev = (startRLev==0) ? startRLev+1 : startRLev;
   for ( ossim_uint32 i = writeRnStartRLev; i <= finalRLev; ++i )
   {
      // Sync all processes...
      ossimMpi::instance()->barrier();

      if ( !writeRn( i ) )
      {
         // Set the error...
         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_WRITE_FILE_ERROR,
            "%s file %s line %d\nError creating reduced res set!",
            MODULE,
            __FILE__,
            __LINE__ );

         if ( theOutputFileTmp.exists() )
         {
            theOutputFileTmp.remove();
         }

         cout << "Writing header file for up to R" << i-1 << ": " << theOutputFile << endl;

         ossimKeywordlist error_header_kwl;
         saveHeaderInfo( error_header_kwl, startRLev, i-1 );
         error_header_kwl.write( theOutputFile );
         return false;
      }
      else
      {
         if ( theOutputFileTmp.exists() )
         {
            theOutputFileTmp.remove();
         }
         ossimKeywordlist temp_header_kwl;
         saveHeaderInfo( temp_header_kwl, startRLev, i );
         temp_header_kwl.write( theOutputFileTmp );
      }
   }

   if ( theOutputFileTmp.exists() )
   {
      theOutputFileTmp.remove();
   }

   cout << "Writing header file: " << theOutputFile << " for virtual image." << endl;

   ossimKeywordlist header_kwl;
   saveHeaderInfo( header_kwl, startRLev, finalRLev );
   return header_kwl.write( theOutputFile );
}

bool ossimVirtualImageWriter::writeR0()
{
   if ( theLimitedScopeUpdateFlag == true )
   {
      return writeR0Partial();
   }
   else
   {
      return writeR0Full();
   }
}

bool ossimVirtualImageWriter::writeRn( ossim_uint32 resLevel )
{
   if ( theLimitedScopeUpdateFlag == true )
   {
      return writeRnPartial( resLevel );
   }
   else
   {
      return writeRnFull( resLevel );
   }
}

bool ossimVirtualImageWriter::saveHeaderInfo( ossimKeywordlist& kwl,
                                              ossim_uint32 rLevBegin,
                                              ossim_uint32 rLevEnd )
{
   ossim_uint32 entryIdx = theImageHandler->getCurrentEntry();

   ossimString prefix = "image";
   prefix += ossimString::toString( entryIdx ) + ".";

   kwl.add( prefix,
            ossimKeywordNames::ENTRY_KW,
            entryIdx,
            true );

   saveGeometryKeywordEntry( kwl, prefix );
   saveGeneralKeywordEntry ( kwl, prefix );
   saveNativeKeywordEntry  ( kwl, prefix,
                             rLevBegin,
                             rLevEnd );

   kwl.add( ossimKeywordNames::NUMBER_ENTRIES_KW, 1, true );

   return true;
}

// Note be sure to theImageHandler->setCurrentEntry before calling.
bool ossimVirtualImageWriter::isExternalOverview() const
{
   bool result = false; // Have to prove it.

   ossimString s = "imag";
   ossimRefPtr<ossimProperty> prop = theImageHandler->getProperty( s );
   if ( prop.valid() )
   {
      ossimString s;
      prop->valueToString( s );
      if ( s.toFloat32() < 1.0 )
      {
         result = true;
      }
   }

   return result;
}

void ossimVirtualImageWriter::saveNativeKeywordEntry( ossimKeywordlist& kwl,
                                                      const ossimString& prefix,
                                                      ossim_uint32 rLevBegin,
                                                      ossim_uint32 rLevEnd ) const
{
   ossimString nativeStr = ossimString( "virtual" ) + ".";

   kwl.add( prefix,
            nativeStr+"subdirectory",
            theOutputSubdirectory,
            true );
   kwl.add( prefix,
            nativeStr+"writer_type",
            theVirtualWriterType,
            true );
   kwl.add( prefix,
            nativeStr+"frame_size_x",
            theOutputFrameSize.x,
            true );
   kwl.add( prefix,
            nativeStr+"frame_size_y",
            theOutputFrameSize.y,
            true );
   kwl.add( prefix,
            nativeStr+"tile_size_x",
            theOutputTileSize.x,
            true );
   kwl.add( prefix,
            nativeStr+"tile_size_y",
            theOutputTileSize.y,
            true );
   kwl.add( prefix,
            nativeStr+"version_major",
            theMajorVersion,
            true );
   kwl.add( prefix,
            nativeStr+"version_minor",
            theMinorVersion,
            true );
   kwl.add( prefix,
            nativeStr+"overview_flag",
            theOverviewFlag ? "true" : "false",
            true );
   kwl.add( prefix,
            nativeStr+"includes_r0",
            theCopyAllFlag ? "true" : "false",
            true );
   kwl.add( prefix,
            nativeStr+"bits_per_sample",
            theBitsPerSample,
            true );
   kwl.add( prefix,
            nativeStr+"bytes_per_pixel",
            theBytesPerPixel,
            true );
   kwl.add( prefix,
            nativeStr+"resolution_level_starting",
            rLevBegin,
            true );
   kwl.add( prefix,
            nativeStr+"resolution_level_ending",
            rLevEnd,
            true );

   nativeStr += ossimString( "resolution_level_" );

   ossim_uint32 r;
   for ( r=rLevBegin; r<=rLevEnd; ++r )
   {
      ossimString fullStr = nativeStr + ossimString::toString( r ) + ".";

      ossimIpt nFrames = getNumberOfOutputFrames( r );

      kwl.add( prefix,
               fullStr+"number_of_frames_x",
               nFrames.x,
               true );
      kwl.add( prefix,
               fullStr+"number_of_frames_y",
               nFrames.y,
               true );
   }
}

// Modified from image_info.cpp outputGeometryEntry() routine
void ossimVirtualImageWriter::saveGeometryKeywordEntry( ossimKeywordlist& kwl,
                                                        const ossimString& prefix )
{
   if( theInputProjection.valid() )
   {
      ossimDrect outputRect = theImageHandler->getBoundingRect();
      theInputProjection->saveState( kwl, prefix );

      ossimGpt ulg,llg,lrg,urg;
      theInputProjection->lineSampleToWorld( outputRect.ul(), ulg );
      theInputProjection->lineSampleToWorld( outputRect.ll(), llg );
      theInputProjection->lineSampleToWorld( outputRect.lr(), lrg );
      theInputProjection->lineSampleToWorld( outputRect.ur(), urg );

      kwl.add( prefix, "ul_lat", ulg.latd(), true );
      kwl.add( prefix, "ul_lon", ulg.lond(), true );
      kwl.add( prefix, "ll_lat", llg.latd(), true );
      kwl.add( prefix, "ll_lon", llg.lond(), true );
      kwl.add( prefix, "lr_lat", lrg.latd(), true );
      kwl.add( prefix, "lr_lon", lrg.lond(), true );
      kwl.add( prefix, "ur_lat", urg.latd(), true );
      kwl.add( prefix, "ur_lon", urg.lond(), true );

      // ESH 01/2009:  The following are still needed by EW 4.4.
      if( !kwl.find( ossimKeywordNames::TIE_POINT_LAT_KW ) )
      {
         kwl.add( prefix, 
                  ossimKeywordNames::TIE_POINT_LAT_KW,
                  ulg.latd(), 
                  true );
         kwl.add( prefix, 
                  ossimKeywordNames::TIE_POINT_LON_KW,
                  ulg.lond(), 
                  true );

         if ( outputRect.height()-1.0 > DBL_EPSILON )
         {
            kwl.add( prefix, 
                     ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
                     fabs( ulg.latd()-llg.latd() ) / (outputRect.height()-1.0),
                     true );
         }
         if ( outputRect.width()-1.0 > DBL_EPSILON )
         {
            kwl.add( prefix, 
                     ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
                     fabs( ulg.lond()-urg.lond()) / (outputRect.width()-1.0),
                     true );
         }
      }

      ossimDpt gsd = theInputProjection->getMetersPerPixel();
      kwl.add( prefix,
               ossimKeywordNames::METERS_PER_PIXEL_X_KW,
               gsd.x,
               true );
      kwl.add( prefix,
               ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
               gsd.y,
               true );
   }
   else
   {
      cerr << "No projection geometry for file " << theImageHandler->getFilename() << endl;
   }
}

// Modified from image_info.cpp outputGeneralImageInfo() routine
void ossimVirtualImageWriter::saveGeneralKeywordEntry( ossimKeywordlist& kwl,
                                                       const ossimString& prefix ) const
{
   ossimDrect boundingRect = theImageHandler->getBoundingRect();

   kwl.add( prefix, ossimKeywordNames::UL_X_KW, boundingRect.ul().x, true );
   kwl.add( prefix, ossimKeywordNames::UL_Y_KW, boundingRect.ul().y, true );
   kwl.add( prefix, ossimKeywordNames::LR_X_KW, boundingRect.lr().x, true );
   kwl.add( prefix, ossimKeywordNames::LR_Y_KW, boundingRect.lr().y, true );

   kwl.add( prefix,
            ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
            theImageHandler->getNumberOfInputBands(),
            true );
   kwl.add( prefix,
            ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
            theImageHandler->getNumberOfOutputBands(),
            true );
   kwl.add( prefix,
            ossimKeywordNames::NUMBER_LINES_KW,
            boundingRect.height(),
            true );
   kwl.add( prefix,
            ossimKeywordNames::NUMBER_SAMPLES_KW,
            boundingRect.width(),
            true );

   int i;
   for( i = 0; i < (int)theImageHandler->getNumberOfInputBands(); ++i )
   {
      ossimString band = ossimString( "band" ) + ossimString::toString( i ) + ".";

      kwl.add( prefix,
               band+"null_value",
               theImageHandler->getNullPixelValue( i ),
               true );
      kwl.add( prefix,
               band+"min_value",
               theImageHandler->getMinPixelValue( i ),
               true );
      kwl.add( prefix,
               band+"max_value",
               theImageHandler->getMaxPixelValue( i ),
               true );
   }

   // Output Radiometry.
   ossimString scalarStr("unknown");
   ossimScalarType scalar = theImageHandler->getOutputScalarType();
   switch( scalar )
   {
      case OSSIM_UINT8:
      {
         scalarStr = "8-bit";
         break;
      }
      case OSSIM_USHORT11:
      {
         scalarStr = "11-bit";
         break;
      }
      case OSSIM_UINT16:
      {
         scalarStr = "16-bit unsigned";
         break;
      }
      case OSSIM_SINT16:
      {
         scalarStr = "16-bit signed";
         break;
      }
      case OSSIM_UINT32:
      {
         scalarStr = "32-bit unsigned";
         break;
      }
      case OSSIM_FLOAT32:
      {
         scalarStr = "float";
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         scalarStr = "normalized float";
         break;
      }
      default:
      {
         /* Do nothing */
         break;
      }
   }

   kwl.add( prefix, "radiometry", scalarStr, true );
}
