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

#include <sstream>

#include <ossim/ossimConfig.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/imaging/ossimVirtualOverviewSequencer.h>
#include <ossim/imaging/ossimVirtualImageHandler.h>
#include <ossim/imaging/ossimVirtualImageWriter.h>

static ossimTrace traceDebug("ossimVirtualImageWriter:debug");

// Virtual image identifier string, found on 1st line of header file:
static const char OSSIM_VIRTUAL_IMAGE_ID[] = "OSSIM_Virtual_Image";

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
     m_OutputFile(file),
     m_OutputFileTmp(""),
     m_OutputSubdirectory("unknown"),
     m_VirtualWriterType("ossim-virtual-image"),
     m_MajorVersion("1.00"),
     m_MinorVersion("1.00"),
     m_CompressType(1),
     m_CompressQuality(75),
     m_OutputTileSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
     m_OutputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
     m_InputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
     m_BytesPerPixel(0),
     m_BitsPerSample(0),
     m_TileSizeInBytes(0),
     m_NullDataBuffer(0),
     m_CopyAllFlag(false),
     m_LimitedScopeUpdateFlag(false),
     m_OverviewFlag(overviewFlag),
     m_CurrentEntry(0),
     m_ProgressListener(0),
     m_DirtyFrameList(0),
     m_InputFrameInfoList(0),
     m_InputFrameInfoQueue(0),
     m_ResampleType(ossimFilterResampler::ossimFilterResampler_BOX),
     m_ImageHandler(0),
     m_InputConnection(0),
     m_InputGeometry(0),
     m_InputProjection(0),
     m_FrameWriter(0),
     m_AreaOfInterest(0,0,0,0),
     m_CurrentFrameName(""),
     m_CurrentFrameNameTmp(""),
     m_FrameExt(""),
     m_NumberVerticalFrames(0),
     m_NumberHorizontalFrames(0)
{
   static const char MODULE[] = "ossimVirtualImageWriter::ossimVirtualImageWriter";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << std::endl;
   }

   ossim::defaultTileSize( m_OutputTileSize );

   m_ImageHandler = inputSource;

   switch( m_ImageHandler->getOutputScalarType() )
   {
      case OSSIM_UINT8:
         m_BitsPerSample = 8;
         m_BytesPerPixel = 1;
         break;

      case OSSIM_USHORT11:
      case OSSIM_UINT16:
         m_BitsPerSample = 16;
         m_BytesPerPixel = 2;
         break;

      case OSSIM_SINT16:
         m_BitsPerSample = 16;
         m_BytesPerPixel = 2;
         break;

      case OSSIM_UINT32:
         m_BitsPerSample = 32;
         m_BytesPerPixel = 4;
         break;

      case OSSIM_FLOAT32:
         m_BitsPerSample = 32;
         m_BytesPerPixel = 4;
         break;

      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         m_BitsPerSample = 64;
         m_BytesPerPixel = 8;
         break;

      default:
      {
         // Set the error...
         setErrorStatus();

         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nUnknown pixel type: "
            << ( ossimScalarTypeLut::instance()->getEntryString( 
            m_ImageHandler->getOutputScalarType() ) )
            << std::endl;

         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_ERROR,
            "Unknown pixel type!",
            __FILE__,
            __LINE__ );
      }
      break;
   }

   m_InputConnection = new ossimImageSourceSequencer( 
                           static_cast<ossimImageSource*>( inputSource ) );
   m_InputGeometry = m_ImageHandler.valid() ? m_ImageHandler->getImageGeometry() : 0;
   m_InputProjection = m_InputGeometry.valid() ? m_InputGeometry->getProjection() : 0;

   // The current entry of the input image handler stays the same
   // throughout the lifetime of this virtual image writer.
   m_CurrentEntry = m_ImageHandler->getCurrentEntry();

   // now map the sequencer to the same input
   connectMyInputTo(0, inputSource);
   initialize();

   m_InputConnection->connectMyInputTo( 0, inputSource, false );
   m_AreaOfInterest.makeNan();

   if ( m_OutputFile == ossimFilename::NIL )
   {
      initializeOutputFilenamFromHandler();
   }
   else
   {
      // Temporary header file used to help build Rn for n>1.
      m_OutputFileTmp = m_OutputFile + ".tmp";
   }

   ossimString driveString;
   ossimString pathString;
   ossimString fileString;
   ossimString extString;
   m_OutputFile.split( driveString, pathString, fileString, extString );

   m_OutputSubdirectory = fileString + "_cache";

   //---
   // Check for a listeners.  If the list is empty, add a standard out
   // listener so that command line apps like img2rr will get some 
   // progress.
   //---
   if (theListenerList.empty())
   {
      m_ProgressListener = new ossimStdOutProgress( 0, true );
      addListener( m_ProgressListener );
      setCurrentMessage(ossimString( "Starting virtual image write...") );
   }

   // See if the input image format is RPF. If it is, do some config.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, m_ImageHandler.get() );
   if ( pRpf != NULL )
   {
      m_InputFrameSize.x = ossimCibCadrgTileSource::CIBCADRG_FRAME_WIDTH;
      m_InputFrameSize.y = ossimCibCadrgTileSource::CIBCADRG_FRAME_HEIGHT;

      // Collect the frame file names of RPF image
      int nFramesFound = 0;

      const ossimRpfToc* pRpfToc = pRpf->getToc();

      // Retrieve directory info
      const ossimRpfTocEntry* pEntry = pRpfToc->getTocEntry( m_CurrentEntry );
      if ( pEntry != NULL )
      {
         m_NumberVerticalFrames   = pEntry->getNumberOfFramesVertical();
         m_NumberHorizontalFrames = pEntry->getNumberOfFramesHorizontal();

         ossim_uint32 iV,iH;
         for ( iV=0; iV<m_NumberVerticalFrames; ++iV )
         {
            for ( iH=0; iH<m_NumberHorizontalFrames; ++iH )
            {
               ossimRpfFrameEntry frameEntry;
               bool bResult = pEntry->getEntry( iV, iH, frameEntry );
               if ( bResult == true )
               {
                  ossimString framePath = frameEntry.getPathToFrameFileFromRoot();
                  if ( framePath.empty() == false )
                  {
                     InputFrameInfo* info = new InputFrameInfo();
                     if ( info != 0 )
                     {
                        info->name  = framePath;
                        info->entry = m_CurrentEntry;
                        info->row   = iV;
                        info->col   = iH;

                        m_InputFrameInfoList.push_back( info );
                     }
                     if ( traceDebug() )
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           << "Frame file(" << ++nFramesFound << "): "
                           << framePath
                           << " (entry: " << m_CurrentEntry << ", row: " << iV << ", col: " << iH << ")" 
                           << std::endl;
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
   // Make sure the image frame is destroyed.
   closeFrame();

   int numFrames = (int)m_InputFrameInfoList.size();
   int i;
   for ( i=0; i<numFrames; ++i )
   {
      InputFrameInfo* info = m_InputFrameInfoList[i];
      if ( info != 0 )
      {
         delete info;
      }
   }
   m_InputFrameInfoList.clear();

   m_InputConnection = 0;
   m_InputGeometry   = 0;
   m_InputProjection = 0;

   if ( m_ProgressListener )
   {
      setCurrentMessage( ossimString("Finished virtual image write...") );
      removeListener( m_ProgressListener );
      delete m_ProgressListener;
      m_ProgressListener = 0;
   }
}

void ossimVirtualImageWriter::setFrameWriter( ossimImageFileWriter* pWriter )
{
   static const char* MODULE = "ossimVirtualImageWriter::setFrameWriter";

   if ( pWriter != 0 )
   {
      m_FrameWriter = pWriter;

      ossim_uint32 extLength = (ossim_uint32)pWriter->getExtension().length();
      m_FrameExt = extLength>0 ? pWriter->getExtension() : "ext";
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL image writer given."
         << std::endl;
   }
}

ossimImageFileWriter* ossimVirtualImageWriter::getFrameWriter( void ) const
{
   return const_cast<ossimImageFileWriter*>(m_FrameWriter.get());
}

bool ossimVirtualImageWriter::setFrameName( int resLevel, int row, int col )
{
   static const char MODULE[] = "ossimVirtualImageWriter::setFrameName";

   bool bSuccess = false;

   // Make sure the previous frame is closed out.
   closeFrame();

   // Check for empty file name.
   if ( m_OutputFile.empty() == false )
   {
      ossimString driveString;
      ossimString pathString;
      ossimString fileString;
      ossimString extString;
      m_OutputFile.split( driveString, pathString, fileString, extString );

      // If the virtual image header filename is a.ovr, the current frame
      // name is e.g. ./a_cache/res0/row0/col0.tif

      ossimFilename pathFName( pathString );
      ossimFilename subdirFName1( "." );
      ossimFilename subdirFName2( subdirFName1.dirCat(m_OutputSubdirectory) );
      ossimFilename subdirFName3( subdirFName2.dirCat("res") );
      subdirFName3.append( ossimString::toString( resLevel ) );
      ossimFilename subdirFName4( subdirFName3.dirCat("row") );
      subdirFName4.append( ossimString::toString( row ) );
      ossimString newPathString( pathFName.dirCat( subdirFName4 ) );

      ossimFilename driveFName( driveString );
      ossimFilename newPathFName( newPathString );
      ossimFilename newDirFName( driveFName.dirCat( newPathFName ) );
      bSuccess = newDirFName.createDirectory(true);

      if ( bSuccess == true )
      {
         ossimString newFileString( "col" );
         newFileString.append( ossimString::toString(col) );

         m_CurrentFrameName.merge( driveString, newPathString, newFileString, m_FrameExt );

         m_CurrentFrameNameTmp = m_CurrentFrameName + ".tmp";

         m_FrameWriter->setFilename( m_CurrentFrameNameTmp );
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nFailed to create directory: "
            << newDirFName.c_str()
            << std::endl;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNo output file name found!"
         << " Cannot give the frame writer a file name to open."
         << std::endl;
   }
   return bSuccess;
}

void ossimVirtualImageWriter::closeFrame()
{
   // Close the image frame using the ossim image handler.
   m_FrameWriter->close();
}

bool ossimVirtualImageWriter::setFrameParameters( ossim_int32 resLevel,
                                                  const ossimIrect& outputRect )
{
   ossim_float64 minSampleValue  = m_ImageHandler->getMinPixelValue();
   ossim_float64 maxSampleValue  = m_ImageHandler->getMaxPixelValue();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualImageWriter::setFrameParameters DEBUG:"
         << "\nrrds_level:      " << resLevel
         << "\nminSampleValue:  " << minSampleValue
         << "\nmaxSampleValue:  " << maxSampleValue
         << std::endl;
   }

   /* Set geospatial-based tags */

   // Retrieve the map projection object from the input image.
   ossimMapProjection* mapProjection = m_InputProjection.valid() ? 
      PTR_CAST( ossimMapProjection, m_InputProjection.get() ) : 0;

   if ( mapProjection != 0 )
   {
      ossimDpt degPerPix;
      if ( resLevel > 0 )
      {
         ossim_uint32 pixelSizeScale = (ossim_uint32)(1<<resLevel);

         // Rescale pixel size based on the resolution level.
         degPerPix = mapProjection->getDecimalDegreesPerPixel();
         ossimDpt degPerPixScaled = degPerPix * pixelSizeScale;

         mapProjection->setDecimalDegreesPerPixel( degPerPixScaled );
      }

      ossimDrect areaOfInterest(outputRect);
      ossimRefPtr<ossimMapProjectionInfo> projInfo = 
         new ossimMapProjectionInfo( mapProjection, 
         areaOfInterest );

      if ( resLevel > 0 )
      {
         // reset to resLevel 0 value
         mapProjection->setDecimalDegreesPerPixel( degPerPix );
      }
   }

   return true;
}

ossimString ossimVirtualImageWriter::getOverviewType( 
   ossimFilterResampler::ossimFilterResamplerType resamplerType )
{
   ossimString overviewType("unknown");

   if ( resamplerType == 
      ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR )
   {
      overviewType = "ossim_virtual_nearest";
   }
   else
   {
      overviewType = "ossim_virtual_box";
   }

   return overviewType;
}

ossimFilterResampler::ossimFilterResamplerType 
ossimVirtualImageWriter::getResamplerType( const ossimString& type )
{
   ossimFilterResampler::ossimFilterResamplerType resamplerType =
      ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;

   if (type == "ossim_virtual_nearest")
   {
      resamplerType =
         ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;

   }
   else
      if (type == "ossim_virtual_box")
      {
         resamplerType = ossimFilterResampler::ossimFilterResampler_BOX;
      }

      return resamplerType;
}

bool ossimVirtualImageWriter::isOverviewTypeHandled( const ossimString& type )
{
   bool bIsHandled = false;
   if (type == "ossim_virtual_nearest")
   {
      bIsHandled = true;
   }
   else
      if (type == "ossim_virtual_box")
      {
         bIsHandled = true;
      }

      return bIsHandled;
}

void ossimVirtualImageWriter::getTypeNameList(
   std::vector<ossimString>& typeList )
{
   typeList.push_back( ossimString("ossim_virtual_box") );
   typeList.push_back( ossimString("ossim_virtual_nearest") );
}

void ossimVirtualImageWriter::initialize()
{
   if( m_InputConnection.valid() )
   {
      m_InputConnection->initialize();
      setAreaOfInterest( m_InputConnection->getBoundingRect() );
   }
}

// Rename the currently open frame image
void ossimVirtualImageWriter::renameFrame()
{
   m_CurrentFrameNameTmp.rename( m_CurrentFrameName );
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << "Wrote file:  " << m_CurrentFrameName.c_str() << std::endl;
   }
}

InputFrameInfo* ossimVirtualImageWriter::getInputFrameInfo( ossimFilename name ) const
{
   static const char MODULE[] = "ossimVirtualImageWriter::getInputFrameInfo";

   // First test: do an unfiltered name compare to find the InputFrameInfo.
   int numFrames = (int)m_InputFrameInfoList.size();
   int i;
   for ( i=0; i<numFrames; ++i )
   {
      InputFrameInfo* info = m_InputFrameInfoList[i];
      if ( info != 0 )
      {
         if ( (m_CurrentEntry == info->entry) &&
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
      InputFrameInfo* info = m_InputFrameInfoList[i];
      if ( info != 0 )
      {
         if ( m_CurrentEntry == info->entry )
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
      ossim_uint32 nQ = (ossim_uint32)m_InputFrameInfoQueue.size();
      ossim_uint32 idxQ;
      for ( idxQ=0; idxQ<nQ; ++idxQ )
      {
         InputFrameInfo* pInfo = m_InputFrameInfoQueue[idxQ];
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
                  // See if (jF,iF) has already been processed for
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
      ossim_int32 imageSamples = m_ImageHandler->getNumberOfSamples();
      ossim_int32 imageLines   = m_ImageHandler->getNumberOfLines();

      ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
      imageSamples /= resLevelPow;
      imageLines   /= resLevelPow;

      ossim_int32 frameSamples = m_OutputFrameSize.x;
      ossim_int32 frameLines   = m_OutputFrameSize.y;

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
   ossim_int32 imageSamples = m_ImageHandler->getNumberOfSamples();
   ossim_int32 imageLines   = m_ImageHandler->getNumberOfLines();

   ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
   imageSamples /= resLevelPow;
   imageLines   /= resLevelPow;

   ossim_int32 frameSamples = m_OutputFrameSize.x;
   ossim_int32 frameLines   = m_OutputFrameSize.y;

   ossim_int32 framesWide = (imageSamples % frameSamples) ?
                            (imageSamples / frameSamples) + 1 : (imageSamples / frameSamples);
   ossim_int32 framesHigh = (imageLines % frameLines) ?
                            (imageLines / frameLines) + 1 : (imageLines / frameLines);

   return ossimIpt( framesWide, framesHigh );
}

ossimIpt ossimVirtualImageWriter::getNumberOfOutputTiles( ossim_uint32 resLevel ) const
{
   ossim_int32 imageSamples = m_ImageHandler->getNumberOfSamples();
   ossim_int32 imageLines   = m_ImageHandler->getNumberOfLines();

   ossim_uint32 resLevelPow = (ossim_uint32)(1<<resLevel);
   imageSamples /= resLevelPow;
   imageLines   /= resLevelPow;

   ossim_int32 tileSamples  = m_OutputTileSize.x;
   ossim_int32 tileLines    = m_OutputTileSize.y;

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
   ossim_int32 frameSamples  = m_OutputFrameSize.x;
   ossim_int32 frameLines    = m_OutputFrameSize.y;

   ossim_int32 tileSamples   = m_OutputTileSize.x;
   ossim_int32 tileLines     = m_OutputTileSize.y;

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

bool ossimVirtualImageWriter::getOverviewFlag() const
{
   return m_OverviewFlag;
}

void ossimVirtualImageWriter::setOverviewFlag( bool flag )
{
   m_OverviewFlag = flag;
}

ossimIrect ossimVirtualImageWriter::getAreaOfInterest() const
{
   return m_AreaOfInterest;
}

void ossimVirtualImageWriter::setAreaOfInterest( const ossimIrect& inputRect )
{
   m_AreaOfInterest = inputRect;
   if( m_InputConnection.valid() )
   {
      m_InputConnection->setAreaOfInterest( inputRect );
   }
}

void ossimVirtualImageWriter::setOutputFile( const ossimFilename& file )
{
   m_OutputFile = file;
}

const ossimFilename& ossimVirtualImageWriter::getOutputFile()const
{
   return m_OutputFile;
}

bool ossimVirtualImageWriter::canConnectMyInputTo( ossim_int32 inputIndex,
                                                   const ossimConnectableObject* object ) const
{
   return ( object &&
          ( (PTR_CAST(ossimImageSource, object) && (inputIndex == 0)) ) );
}

void ossimVirtualImageWriter::setOutputTileSize( const ossimIpt& tileSize )
{
   ossim_int32 remainder_x = tileSize.x % 16;
   ossim_int32 remainder_y = tileSize.y % 16;
   if ( remainder_x!=0 || remainder_y!=0 )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimVirtualImageWriter::changeTileSize ERROR:"
            << "\nTile size must be a multiple of 16!"
            << std::endl;
      }

      // Make the size a multiple of 16
      m_OutputTileSize.x = (tileSize.x / 16);
      m_OutputTileSize.y = (tileSize.y / 16);

      m_OutputTileSize.x += (remainder_x!=0)?1:0;
      m_OutputTileSize.y += (remainder_y!=0)?1:0;

      m_OutputTileSize.x *= 16;
      m_OutputTileSize.y *= 16;
   }
   else
   {
      m_OutputTileSize = tileSize;
   }

   if ( m_InputConnection.valid() )
   {
      m_InputConnection->setTileSize( m_OutputTileSize );
   }

   m_TileSizeInBytes = m_OutputTileSize.x * m_OutputTileSize.y * m_BytesPerPixel;

   //---
   // Make a buffer to pass to pass to the write tile methods when an image
   // handler returns a null tile.
   //---
   m_NullDataBuffer.resize( m_TileSizeInBytes );

   // Fill it with zeroes.
   std::fill( m_NullDataBuffer.begin(), m_NullDataBuffer.end(), 0 );
}

ossimIpt ossimVirtualImageWriter::getOutputTileSize() const
{
   return m_OutputTileSize;
}

void ossimVirtualImageWriter::setOutputFrameSize( const ossimIpt& frameSize )
{
   m_OutputFrameSize = frameSize;
}

bool ossimVirtualImageWriter::getCopyAllFlag() const
{
   return m_CopyAllFlag;
}

void ossimVirtualImageWriter::setCopyAllFlag( bool flag )
{
   m_CopyAllFlag = flag;
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
   return m_CopyAllFlag ? 0 : 1;
}

// After 1 or more dirty frames are set, an update to the image will
// be of limited scope to areas of the RPF image that come from the 
// frame files found in theDirtyFrameList.
void ossimVirtualImageWriter::setDirtyFrame( const ossimString& name )
{
   // Add the name of a frame to the dirty frame list.
   m_DirtyFrameList.push_back(name);
}

bool ossimVirtualImageWriter::isFrameAlreadyDone( ossim_uint32 mQ,
                                                  ossim_uint32 resLevel,
                                                  ossim_int32  frameX, 
                                                  ossim_int32  frameY ) const
{
   bool bFoundIt = false;
   ossim_uint32 nQ = (ossim_uint32)m_InputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ && idxQ<mQ; ++idxQ )
   {
      InputFrameInfo* pInfo = m_InputFrameInfoQueue[idxQ];
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

   if ( !m_FrameWriter.valid() )
   {
      m_FrameWriter = new ossimTiffWriter;
      m_FrameExt    = m_FrameWriter->getExtension();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << "\nWARN: Missing image frame writer! Creating a default tiff writer."
         << std::endl;
   }

   // Configure the image frame writer
   m_FrameWriter->setTileSize( m_OutputTileSize );

   // Zero base starting resLevel.
   bool bFoundOldStartRLev = false;
   ossim_uint32 oldStartRLev = 0;

   // Zero base final resLevel.
   bool bFoundOldFinalRLev = false;
   ossim_uint32 oldFinalRLev = 0;

   // If the virtual header file already exists, 
   // let's try to stay consistent with its parameters.
   bool bExists = m_OutputFile.exists();
   if ( bExists )
   {
      ossimRefPtr<ossimImageHandler> pHand = ossimImageHandlerRegistry::instance()->open( m_OutputFile );
      if ( pHand.valid() )
      {
         ossimVirtualImageHandler* pVirt = PTR_CAST( ossimVirtualImageHandler, pHand.get() );
         if ( pVirt != NULL )
         {
            ossimIpt outputFrameSize;
            outputFrameSize.x = pVirt->getFrameWidth();
            outputFrameSize.y = pVirt->getFrameHeight();

            if ( outputFrameSize.x != m_OutputFrameSize.x )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output frame size(x): " 
                  << m_OutputFrameSize.x 
                  << " -> "
                  << outputFrameSize.x
                  << ", overridden by pre-existing value found in: "
                  << m_OutputFile
                  << std::endl;

               m_OutputFrameSize.x = outputFrameSize.x;
            }
            if ( outputFrameSize.y != m_OutputFrameSize.y )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output frame size(y): " 
                  << m_OutputFrameSize.y 
                  << " -> "
                  << outputFrameSize.y
                  << ", overridden by pre-existing value found in: "
                  << m_OutputFile
                  << std::endl;

               m_OutputFrameSize.y = outputFrameSize.y;
            }

            ossimIpt outputTileSize;
            outputTileSize.x = pVirt->getTileWidth();
            outputTileSize.y = pVirt->getTileHeight();

            if ( outputTileSize.x != m_OutputTileSize.x )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output tile size(x): " 
                  << m_OutputTileSize.x 
                  << " -> "
                  << outputTileSize.x
                  << ", overridden by pre-existing value found in: "
                  << m_OutputFile
                  << std::endl;

               m_OutputTileSize.x = outputTileSize.x;
            }
            if ( outputTileSize.y != m_OutputTileSize.y )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The requested output tile size(y): " 
                  << m_OutputTileSize.y 
                  << " -> "
                  << outputTileSize.y
                  << ", overridden by pre-existing value found in: "
                  << m_OutputFile
                  << std::endl;

               m_OutputTileSize.y = outputTileSize.y;
            }

            // Let's make sure we keep the number of input frames 
            // consistent with previous runs, in order to keep the
            // output row numbering from getting offset.
            m_NumberHorizontalFrames = pVirt->getNumberInputFramesX();
            m_NumberVerticalFrames   = pVirt->getNumberInputFramesY();

            // Zero base starting resLevel.
            bFoundOldStartRLev = true;
            oldStartRLev = pVirt->getStartingResLevel();

            // Zero base final resLevel.
            bFoundOldFinalRLev = true;
            oldFinalRLev = oldStartRLev + pVirt->getNumberOfDecimationLevels() - 1;
         }

         pHand = 0;
      }
   }

   // Adjust the x-dimensional output frame size to be an integer 
   // multiple of the output tile size (e.g. x1,x2,x3,...) if it isn't 
   // already.
   ossim_int32 modX = m_OutputFrameSize.x % m_OutputTileSize.x;
   if ( modX != 0 )
   {
      ossim_int32 mulX = (m_OutputFrameSize.x / m_OutputTileSize.x) + 1;
      if ( mulX == 0 )
      {
         mulX = 1;
      }

      ossimNotify(ossimNotifyLevel_WARN)
         << "Adjusting the requested output frame size(" 
         << m_OutputFrameSize.x 
         << ") in x-dim to "
         << mulX << " times the output tile size("
         << m_OutputTileSize.x
         << ")"
         << std::endl;

      m_OutputFrameSize.x = m_OutputTileSize.x * mulX;
   }

   // Adjust the y-dimensional output frame size to be an integer 
   // multiple of the output tile size (e.g. x1,x2,x3,...) if it isn't 
   // already.
   ossim_int32 modY = m_OutputFrameSize.y % m_OutputTileSize.y;
   if ( modY != 0 )
   {
      ossim_int32 mulY = (m_OutputFrameSize.y / m_OutputTileSize.y) + 1;
      if ( mulY== 0 )
      {
         mulY = 1;
      }

      ossimNotify(ossimNotifyLevel_WARN)
         << "Adjusting the requested output frame size(" 
         << m_OutputFrameSize.y 
         << ") in y-dim to "
         << mulY << " times the output tile size("
         << m_OutputTileSize.y
         << ")"
         << std::endl;

      m_OutputFrameSize.y = m_OutputTileSize.y * mulY;
   }

   // Zero base starting resLevel.
   ossim_uint32 startRLev = bFoundOldStartRLev ? 
         oldStartRLev : getStartingResLevel();

   // Zero base final resLevel.
   ossim_uint32 finalRLev = bFoundOldFinalRLev ? 
         oldFinalRLev : getFinalResLevel( startRLev );

   // Print out the dirty frames.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, m_ImageHandler.get() );
   if ( pRpf != NULL )
   {
      ossim_uint32 inputFrameNx  = m_InputFrameSize.x;
      ossim_uint32 inputFrameNy  = m_InputFrameSize.y;
      ossim_uint32 outputFrameNx = m_OutputFrameSize.x;
      ossim_uint32 outputFrameNy = m_OutputFrameSize.y;

      // Make sure the queue is empty before we start adding in.
      m_InputFrameInfoQueue.clear();

      int nFrames = (int)m_DirtyFrameList.size();
      if ( nFrames > 0 )
      {
         int idx;
         for( idx=0; idx<nFrames; ++idx )
         {
            ossimFilename frameTestName = m_DirtyFrameList[idx];
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

               if ( info->col < m_NumberHorizontalFrames &&
                    info->row < m_NumberVerticalFrames )
               {
                  ossim_uint32 inputFrameI = info->col;
                  ossim_uint32 inputFrameJ = m_NumberVerticalFrames - info->row - 1;

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
                  m_InputFrameInfoQueue.push_back( info );
               }
            }

            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nFrame file(" << idx+1 << "): "
                  << m_DirtyFrameList[idx]
                  << (info ? " will" : " will not")
                  << " be processed."
                  << std::endl;
            }
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Building entire image for current entry..."
            << std::endl;

         int numFrames = (int)m_InputFrameInfoList.size();
         int i;
         for ( i=0; i<numFrames; ++i )
         {
            InputFrameInfo* info = m_InputFrameInfoList[i];
            if ( info != 0 )
            {
               if ( m_CurrentEntry == info->entry )
               {
                  if ( traceDebug() )
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << "\nRange processing input frame file(" << info->name << "): "
                        << "\n(entry: " << info->entry << ", row: " << info->row << ", col: " << info->col << ")" 
                        << std::endl;
                  }

                  if ( info->col < m_NumberHorizontalFrames &&
                       info->row < m_NumberVerticalFrames )
                  {
                     ossim_uint32 inputFrameI = info->col;
                     ossim_uint32 inputFrameJ = m_NumberVerticalFrames - info->row - 1;

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
                     m_InputFrameInfoQueue.push_back( info );
                  }
               }
            }
         }
      }

      if ( m_InputFrameInfoQueue.size() > 0 )
      {
         m_LimitedScopeUpdateFlag = true;
      }
      else
      {
         // No frames have been found for the current entry of the
         // input image handler. So there's no work to be done and we
         // should bail...
         ossimNotify(ossimNotifyLevel_WARN)
            << "There is nothing to do, so exiting overview write of entry: "
            << m_CurrentEntry
            << std::endl;
         return false;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Input image (" << m_ImageHandler->getFilename() << ") not an RPF, "
         << "so the entire image is being rebuilt." 
         << std::endl;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nCurrent number of reduced res sets: "
         << m_ImageHandler->getNumberOfDecimationLevels()
         << "\nStarting reduced res set:    " << startRLev
         << "\nEnding reduced res sets:     " << finalRLev
         << "\nResampling type:             " << m_ResampleType
         << std::endl;
   }

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
         temp_header_kwl.write( m_OutputFileTmp, OSSIM_VIRTUAL_IMAGE_ID );
      }
   }

   ossim_uint32 writeRnStartRLev = (startRLev==0) ? startRLev+1 : startRLev;
   for ( ossim_uint32 i = writeRnStartRLev; i <= finalRLev; ++i )
   {
      if ( !writeRn( i ) )
      {
         // Set the error...
         ossimSetError( getClassName(),
            ossimErrorCodes::OSSIM_WRITE_FILE_ERROR,
            "%s file %s line %d\nError creating reduced res set!",
            MODULE,
            __FILE__,
            __LINE__ );

         if ( m_OutputFileTmp.exists() )
         {
            m_OutputFileTmp.remove();
         }

         cout << "Writing header file for up to R" << i-1 << ": " << m_OutputFile << endl;

         ossimKeywordlist error_header_kwl;
         saveHeaderInfo( error_header_kwl, startRLev, i-1 );
         error_header_kwl.write( m_OutputFile, OSSIM_VIRTUAL_IMAGE_ID );
         return false;
      }
      else
      {
         if ( m_OutputFileTmp.exists() )
         {
            m_OutputFileTmp.remove();
         }
         ossimKeywordlist temp_header_kwl;
         saveHeaderInfo( temp_header_kwl, startRLev, i );
         temp_header_kwl.write( m_OutputFileTmp, OSSIM_VIRTUAL_IMAGE_ID );
      }
   }

   if ( m_OutputFileTmp.exists() )
   {
      m_OutputFileTmp.remove();
   }

   cout << "Writing header file: " << m_OutputFile << " for virtual image." << endl;

   ossimKeywordlist header_kwl;
   saveHeaderInfo( header_kwl, startRLev, finalRLev );
   return header_kwl.write( m_OutputFile, OSSIM_VIRTUAL_IMAGE_ID );
}

bool ossimVirtualImageWriter::writeR0()
{
   if ( m_LimitedScopeUpdateFlag == true )
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
   if ( m_LimitedScopeUpdateFlag == true )
   {
      return writeRnPartial( resLevel );
   }
   else
   {
      return writeRnFull( resLevel );
   }
}

bool ossimVirtualImageWriter::writeR0Partial()
{
   static const char MODULE[] = "ossimVirtualImageWriter::writeR0Partial";

   ossimRefPtr<ossimVirtualOverviewSequencer> sequencer;
   sequencer = new ossimVirtualOverviewSequencer( m_ImageHandler.get() );

   sequencer->setSourceLevel( 0 );
   sequencer->setResampleType( m_ResampleType );
   sequencer->setTileSize( m_OutputTileSize );
   sequencer->setFrameSize( m_OutputFrameSize );
   sequencer->setDisableResamplingFlag( true );
   sequencer->initialize();

   // Give the frame writer the selected input source.
   m_FrameWriter->connectMyInputTo( 0, m_ImageHandler.get() );
   m_FrameWriter->changeSequencer( sequencer.get() );
   m_FrameWriter->initialize();

   ossim_int32 numberOfFrames = getNumberOfBuiltFrames( 0, true ); // Frames per image

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << 0
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   setCurrentMessage(ossimString("Copying r0..."));

   ossim_int32 frameNumber = 0;
   bool bIsFrameAlreadyDone = false;

   ossim_uint32 nQ = (ossim_uint32)m_InputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ; ++idxQ )
   {
      InputFrameInfo* pInfo = m_InputFrameInfoQueue[idxQ];
      if ( pInfo && pInfo->isValid() )
      {
         //***
         // Frame loop in the line (height) direction.
         //***
         ossim_int32 iF;
         ossim_int32 yRangeMin = pInfo->yRangeMin[0];
         ossim_int32 yRangeMax = pInfo->yRangeMax[0];
         for( iF = yRangeMin; iF <= yRangeMax; ++iF )
         {
             // Origin of a frame with respect to the entire image.
            ossimIpt originI(0, 0);
            originI.y = iF * m_OutputFrameSize.y;

            //***
            // Frame loop in the sample (width) direction.
            //***
            ossim_int32 jF;
            ossim_int32 xRangeMin = pInfo->xRangeMin[0];
            ossim_int32 xRangeMax = pInfo->xRangeMax[0];
            for( jF = xRangeMin; jF <= xRangeMax; ++jF )
            {
               // See if (jF,iF) has already been processed for
               // a previous input frame in the queue.
               bIsFrameAlreadyDone = isFrameAlreadyDone( idxQ, 0, jF, iF );

               if ( bIsFrameAlreadyDone == false )
               {
                  originI.x = jF * m_OutputFrameSize.x;

                  ossimIrect aoi( originI.x,
                                  originI.y,
                                  originI.x + m_OutputFrameSize.x - 1,
                                  originI.y + m_OutputFrameSize.y - 1 );

                  m_FrameWriter->setAreaOfInterest( aoi );

                  // Set the frame filename to the writer.
                  if ( setFrameName( 0, iF, jF ) == false )
                  {
                     abort();
                     setPercentComplete(100.0);
                     return false;
                  }

                  try
                  {
                     m_FrameWriter->execute();
                  }
                  catch(std::exception& e)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << MODULE
                        << " Caught exception!\n"
                        << e.what()
                        << std::endl;
                  }
                  catch(...)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << MODULE 
                        << " Unknown exception caught!\n"
                        << std::endl;
                  }

                  // Close and rename the new frame file.
                  closeFrame();
                  renameFrame();

                  ++frameNumber;

               } // is frame already done check

            } // End of frame loop in the sample (width) direction.

            if (needsAborting())
            {
               setPercentComplete(100.0);
               return true;
            }
            else
            if ( bIsFrameAlreadyDone == false )
            {
               double frame = frameNumber;
               setPercentComplete(frame / numberOfFrames * 100.0);
            }

         } // End of frame loop in the line (height) direction.

      } // End of pInfo NULL check

   } // End of queue loop

   return true;
}

bool ossimVirtualImageWriter::writeR0Full()
{
   static const char MODULE[] = "ossimVirtualImageWriter::writeR0Full";

   ossimRefPtr<ossimVirtualOverviewSequencer> sequencer;
   sequencer = new ossimVirtualOverviewSequencer( m_ImageHandler.get() );

   sequencer->setSourceLevel( 0 );
   sequencer->setResampleType( m_ResampleType );
   sequencer->setTileSize( m_OutputTileSize );
   sequencer->setFrameSize( m_OutputFrameSize );
   sequencer->setDisableResamplingFlag( true );
   sequencer->initialize();

   // Give the frame writer the selected input source.
   m_FrameWriter->connectMyInputTo( 0, m_ImageHandler.get() );
   m_FrameWriter->changeSequencer( sequencer.get() );
   m_FrameWriter->initialize();

   ossimIpt    framesPerImage = getNumberOfOutputFrames();
   ossim_int32 framesWide     = framesPerImage.x;
   ossim_int32 framesHigh     = framesPerImage.y;
   ossim_int32 numberOfFrames = framesWide * framesHigh; // Frames per image

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nframes per image (horz) : " << framesWide
         << "\nframes per image (vert) : " << framesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   setCurrentMessage(ossimString("Copying r0..."));

   ossim_int32 frameNumber = 0;

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 iF;
   for( iF = 0; iF < framesHigh; ++iF )
   {
      // Origin of a frame with respect to the entire image.
      ossimIpt originI(0, 0);
      originI.y = iF * m_OutputFrameSize.y;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 jF;
      for( jF = 0; jF < framesWide; ++jF )
      {
         originI.x = jF * m_OutputFrameSize.x;

         ossimIrect aoi( originI.x,
                         originI.y,
                         originI.x + m_OutputFrameSize.x - 1,
                         originI.y + m_OutputFrameSize.y - 1 );

         m_FrameWriter->setAreaOfInterest( aoi );

         // Set the frame filename to the writer.
         if ( setFrameName( 0, iF, jF ) == false )
         {
            abort();
            setPercentComplete(100.0);
            return false;
         }

         try
         {
            m_FrameWriter->execute();
         }
         catch(std::exception& e)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << MODULE
               << " Caught exception!\n"
               << e.what()
               << std::endl;
         }
         catch(...)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << MODULE 
               << " Unknown exception caught!\n"
               << std::endl;
         }

         // Close and rename the new frame file.
         closeFrame();
         renameFrame();

         ++frameNumber;

      } // End of frame loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double frame = frameNumber;
         setPercentComplete(frame / numberOfFrames * 100.0);
      }

   } // End of frame loop in the line (height) direction.

   return true;
}

bool ossimVirtualImageWriter::writeRnPartial( ossim_uint32 resLevel )
{
   static const char MODULE[] = "ossimVirtualImageWriter::writeRnPartial";

   if ( resLevel == 0 )
   {
      return false;
   }

   ossimRefPtr<ossimImageHandler> imageHandler = 0;

   // If same rlevel is available from the src overview data, set to true.
   bool bUsingSrcAtSameRLevel = false;

   // If same rlevel is available from the dst overview data, set to true.
   bool bUsingDstAtSameRLevel = false;

   ossim_uint32 highestSrcRLevel = m_ImageHandler->getNumberOfDecimationLevels() - 1;
   bool bSrcHasSameRLevel = (resLevel <= highestSrcRLevel) ? true:false;
   bool bSrcHasOneRLevelDown = (resLevel-1 <= highestSrcRLevel) ? true:false;
   bool bDstHasInternalOverviews = m_FrameWriter->getOutputHasInternalOverviews();
   if ( bSrcHasSameRLevel )
   {
      // If the source image handler has the required rLevel available use it
      // since we don't need to decimate it, which is an expensive operation.
      bUsingSrcAtSameRLevel = true;
      imageHandler = m_ImageHandler.get();
   }
   else
   if ( bDstHasInternalOverviews )
   {
      bUsingDstAtSameRLevel = true;
      // Note that we're telling the constructor to use an overview 1 level up.
      imageHandler = m_OutputFileTmp.exists() ? new ossimVirtualImageHandler( 1 ) : 0;
      bool bSuccess = imageHandler.valid() ? imageHandler->open( m_OutputFileTmp ) : false;
      if ( !bSuccess )
      {
         imageHandler = 0;
         bUsingDstAtSameRLevel = false;
      }
   }

   if ( imageHandler.get()==0 )
   {
      if ( bSrcHasOneRLevelDown )
      {
         // Since the source image handler has the rLevel 1 level 
         // down data available, we'll make do and decimate it.
         imageHandler = m_ImageHandler.get();
      }
      else
      {
         // Final recourse: since the virtual overview we're building has the 
         // rLevel 1 level down data available, we'll make do and decimate it.
         imageHandler = m_OutputFileTmp.exists() ? new ossimVirtualImageHandler() : 0;
         bool bSuccess = imageHandler.valid() ? imageHandler->open( m_OutputFileTmp ) : false;
         if ( !bSuccess )
         {
            imageHandler = 0;
            // Set the error...
            ossimSetError( getClassName(),
               ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
               "%s file %s line %d\nCannot open file:  %s",
               MODULE,
               __FILE__,
               __LINE__,
               m_OutputFileTmp.c_str() );   
            return false;
         }
      }
   }

   // If the image handler was created in this member function, 
   // make sure we delete it on return.
   bool bLocalImageHandler = (m_ImageHandler.get() != imageHandler.get());

   //---
   // If the source image had built in overviews then we must subtract from the
   // resLevel given to the sequencer or it will get the wrong image rectangle
   // for the area of interest.
   //---
   ossim_uint32 sourceResLevel = bUsingSrcAtSameRLevel ? 
                                    resLevel : resLevel - 1;

   ossimRefPtr<ossimVirtualOverviewSequencer> sequencer;
   sequencer = new ossimVirtualOverviewSequencer(imageHandler.get());

   sequencer->setSourceLevel(sourceResLevel);
   sequencer->setResampleType(m_ResampleType);
   sequencer->setTileSize(m_OutputTileSize);
   sequencer->setFrameSize(m_OutputFrameSize);
   sequencer->setDisableResamplingFlag( bUsingSrcAtSameRLevel || 
                                        bUsingDstAtSameRLevel );
   sequencer->initialize();

   // Give the frame writer the selected input source.
   m_FrameWriter->connectMyInputTo(0, imageHandler.get());
   m_FrameWriter->changeSequencer(sequencer.get());
   m_FrameWriter->initialize();

   setProcessStatus( ossimProcessInterface::PROCESS_STATUS_EXECUTING );
   setPercentComplete( 0.0 );

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage( os.str() );

   ossim_int32 numberOfFrames = getNumberOfBuiltFrames( resLevel, true ); // partial build

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << resLevel
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   ossim_int32 frameNumber = 0;
   bool bIsFrameAlreadyDone = false;

   ossim_uint32 nQ = (ossim_uint32)m_InputFrameInfoQueue.size();
   ossim_uint32 idxQ;
   for ( idxQ=0; idxQ<nQ; ++idxQ )
   {
      InputFrameInfo* pInfo = m_InputFrameInfoQueue[idxQ];
      if ( pInfo && pInfo->isValid(resLevel) )
      {
         //***
         // Frame loop in the line (height) direction.
         //***
         ossim_int32 iF;
         ossim_int32 yRangeMin = pInfo->yRangeMin[resLevel];
         ossim_int32 yRangeMax = pInfo->yRangeMax[resLevel];
         for( iF = yRangeMin; iF <= yRangeMax; ++iF )
         {
            // Origin of a frame with respect to the entire image.
            ossimIpt originI(0, 0);
            originI.y = iF * m_OutputFrameSize.y;

            //***
            // Frame loop in the sample (width) direction.
            //***
            ossim_int32 jF;
            ossim_int32 xRangeMin = pInfo->xRangeMin[resLevel];
            ossim_int32 xRangeMax = pInfo->xRangeMax[resLevel];
            for( jF = xRangeMin; jF <= xRangeMax; ++jF )
            {
               // See if (jF,iF) has already been processed for
               // a previous input frame in the queue for this resolution level.
               bIsFrameAlreadyDone = isFrameAlreadyDone( idxQ, resLevel, jF, iF );

               if ( bIsFrameAlreadyDone == false )
               {
                  originI.x = jF * m_OutputFrameSize.x;

                  ossimIrect aoi( originI.x,
                                  originI.y,
                                  originI.x + m_OutputFrameSize.x - 1,
                                  originI.y + m_OutputFrameSize.y - 1 );

                  m_FrameWriter->setAreaOfInterest( aoi );

                  // Set the frame filename to the writer.
                  if ( setFrameName( resLevel, iF, jF ) == false )
                  {
                     abort();
                     setPercentComplete(100.0);
                     return false;
                  }

                  try
                  {
                     m_FrameWriter->execute();
                  }
                  catch(std::exception& e)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << MODULE
                        << " Caught exception!\n"
                        << e.what()
                        << std::endl;
                  }
                  catch(...)
                  {
                     ossimNotify(ossimNotifyLevel_FATAL)
                        << MODULE 
                        << " Unknown exception caught!\n"
                        << std::endl;
                  }

                  // Close and rename the new frame file.
                  closeFrame();
                  renameFrame();

                  ++frameNumber;

               } // is output frame already done check

            } // End of frame loop in the sample (width) direction.

            if (needsAborting())
            {
               setPercentComplete(100.0);
               if ( bLocalImageHandler == true )
               {
                  imageHandler = 0;
               }
               return true;
            }
            else
            if ( bIsFrameAlreadyDone == false )
            {
               double frame = frameNumber;
               setPercentComplete(frame / numberOfFrames * 100.0);
            }

         } // End of frame loop in the line (height) direction.

      } // End of pInfo NULL check

   } // End of queue loop

   if ( bLocalImageHandler == true )
   {
      imageHandler = 0;
   }

   return true;
}

bool ossimVirtualImageWriter::writeRnFull( ossim_uint32 resLevel )
{
   static const char MODULE[] = "ossimVirtualImageWriter::writeRnFull";

   if ( resLevel == 0 )
   {
      return false;
   }

   ossimRefPtr<ossimImageHandler> imageHandler = 0;

   // If same rlevel is available from the src overview data, set to true.
   bool bUsingSrcAtSameRLevel = false;

   // If same rlevel is available from the dst overview data, set to true.
   bool bUsingDstAtSameRLevel = false;

   ossim_uint32 highestSrcRLevel = m_ImageHandler->getNumberOfDecimationLevels() - 1;
   bool bSrcHasSameRLevel = (resLevel <= highestSrcRLevel) ? true:false;
   bool bSrcHasOneRLevelDown = (resLevel-1 <= highestSrcRLevel) ? true:false;
   bool bDstHasInternalOverviews = m_FrameWriter->getOutputHasInternalOverviews();
   if ( bSrcHasSameRLevel )
   {
      // If the source image handler has the required rLevel available use it
      // since we don't need to decimate it, which is an expensive operation.
      bUsingSrcAtSameRLevel = true;
      imageHandler = m_ImageHandler.get();
   }
   else
   if ( bDstHasInternalOverviews )
   {
      bUsingDstAtSameRLevel = true;
      // Note that we're telling the constructor to use an overview 1 level up.
      imageHandler = m_OutputFileTmp.exists() ? new ossimVirtualImageHandler( 1 ) : 0;
      bool bSuccess = imageHandler.valid() ? imageHandler->open( m_OutputFileTmp ) : false;
      if ( !bSuccess )
      {
         imageHandler = 0;
         bUsingDstAtSameRLevel = false;
      }
   }

   if ( imageHandler.get()==0 )
   {
      if ( bSrcHasOneRLevelDown )
      {
         // Since the source image handler has the rLevel 1 level 
         // down data available, we'll make do and decimate it.
         imageHandler = m_ImageHandler.get();
      }
      else
      {
         // Final recourse: since the virtual overview we're building has the 
         // rLevel 1 level down data available, we'll make do and decimate it.
         imageHandler = m_OutputFileTmp.exists() ? new ossimVirtualImageHandler() : 0;
         bool bSuccess = imageHandler.valid() ? imageHandler->open( m_OutputFileTmp ) : false;
         if ( !bSuccess )
         {
            imageHandler = 0;
            // Set the error...
            ossimSetError( getClassName(),
               ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
               "%s file %s line %d\nCannot open file:  %s",
               MODULE,
               __FILE__,
               __LINE__,
               m_OutputFileTmp.c_str() );   
            return false;
         }
      }
   }

   // If the image handler was created in this member function, 
   // make sure we delete it on return.
   bool bLocalImageHandler = (m_ImageHandler.get() != imageHandler.get());

   //---
   // If the source image had built in overviews then we must subtract from the
   // resLevel given to the sequencer or it will get the wrong image rectangle
   // for the area of interest.
   //---
   ossim_uint32 sourceResLevel = bUsingSrcAtSameRLevel ? 
                  resLevel : resLevel - 1;

   ossimRefPtr<ossimVirtualOverviewSequencer> sequencer;
   sequencer = new ossimVirtualOverviewSequencer(imageHandler.get());

   sequencer->setSourceLevel(sourceResLevel);
   sequencer->setResampleType(m_ResampleType);
   sequencer->setTileSize(m_OutputTileSize);
   sequencer->setFrameSize(m_OutputFrameSize);
   sequencer->setDisableResamplingFlag( bUsingSrcAtSameRLevel || 
                                        bUsingDstAtSameRLevel );
   sequencer->initialize();

   // Give the frame writer the selected input source.
   m_FrameWriter->connectMyInputTo(0, imageHandler.get());
   m_FrameWriter->changeSequencer(sequencer.get());
   m_FrameWriter->initialize();

   setProcessStatus( ossimProcessInterface::PROCESS_STATUS_EXECUTING );
   setPercentComplete( 0.0 );

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage( os.str() );

   ossimIpt    framesPerImage = getNumberOfOutputFrames( resLevel ); // full build
   ossim_int32 framesWide     = framesPerImage.x;
   ossim_int32 framesHigh     = framesPerImage.y;
   ossim_int32 numberOfFrames = framesWide * framesHigh; // Frames per image

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nresolution level        : " << resLevel
         << "\nframes per image (horz) : " << framesWide
         << "\nframes per image (vert) : " << framesHigh
         << "\nframes per image (total): " << numberOfFrames
         << std::endl;
   }

   ossim_int32 frameNumber = 0;

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 iF;
   for( iF = 0; iF < framesHigh; ++iF )
   {
       // Origin of a frame with respect to the entire image.
      ossimIpt originI(0, 0);
      originI.y = iF * m_OutputFrameSize.y;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 jF;
      for( jF = 0; jF < framesWide; ++jF )
      {
         originI.x = jF * m_OutputFrameSize.x;

         ossimIrect aoi( originI.x,
                         originI.y,
                         originI.x + m_OutputFrameSize.x - 1,
                         originI.y + m_OutputFrameSize.y - 1 );

         m_FrameWriter->setAreaOfInterest( aoi );

         // Set the frame filename to the writer.
         if ( setFrameName( resLevel, iF, jF ) == false )
         {
            abort();
            setPercentComplete(100.0);
            return false;
         }

         try
         {
            m_FrameWriter->execute();
         }
         catch(std::exception& e)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << MODULE
               << " Caught exception!\n"
               << e.what()
               << std::endl;
         }
         catch(...)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << MODULE 
               << " Unknown exception caught!\n"
               << std::endl;
         }

         // Close and rename the new frame file.
         closeFrame();
         renameFrame();

         ++frameNumber;

      } // End of frame loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double frame = frameNumber;
         setPercentComplete(frame / numberOfFrames * 100.0);
      }

   } // End of frame loop in the line (height) direction.

   if ( bLocalImageHandler == true )
   {
      imageHandler = 0;
   }

   return true;
}

bool ossimVirtualImageWriter::saveHeaderInfo( ossimKeywordlist& kwl,
                                              ossim_uint32 rLevBegin,
                                              ossim_uint32 rLevEnd )
{
   ossim_uint32 entryIdx = m_ImageHandler->getCurrentEntry();

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

// Note be sure to m_ImageHandler->setCurrentEntry before calling.
bool ossimVirtualImageWriter::isExternalOverview() const
{
   bool result = false; // Have to prove it.

   ossimString s = "imag";
   ossimRefPtr<ossimProperty> prop = m_ImageHandler->getProperty( s );
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
            m_OutputSubdirectory,
            true );
   kwl.add( prefix,
            nativeStr+"writer_type",
            m_VirtualWriterType,
            true );
   kwl.add( prefix,
            nativeStr+"frame_size_x",
            m_OutputFrameSize.x,
            true );
   kwl.add( prefix,
            nativeStr+"frame_size_y",
            m_OutputFrameSize.y,
            true );
   kwl.add( prefix,
            nativeStr+"number_input_frames_x",
            m_NumberHorizontalFrames,
            true );
   kwl.add( prefix,
            nativeStr+"number_input_frames_y",
            m_NumberVerticalFrames,
            true );
   kwl.add( prefix,
            nativeStr+"version_major",
            m_MajorVersion,
            true );
   kwl.add( prefix,
            nativeStr+"version_minor",
            m_MinorVersion,
            true );
   kwl.add( prefix,
            nativeStr+"overview_flag",
            m_OverviewFlag ? "true" : "false",
            true );
   kwl.add( prefix,
            nativeStr+"includes_r0",
            m_CopyAllFlag ? "true" : "false",
            true );
   kwl.add( prefix,
            nativeStr+"bits_per_sample",
            m_BitsPerSample,
            true );
   kwl.add( prefix,
            nativeStr+"bytes_per_pixel",
            m_BytesPerPixel,
            true );
   kwl.add( prefix,
            nativeStr+"resolution_level_starting",
            rLevBegin,
            true );
   kwl.add( prefix,
            nativeStr+"resolution_level_ending",
            rLevEnd,
            true );
}

// Modified from image_info.cpp outputGeometryEntry() routine
void ossimVirtualImageWriter::saveGeometryKeywordEntry( ossimKeywordlist& kwl,
                                                        const ossimString& prefix )
{
   if( m_InputProjection.valid() )
   {
      ossimDrect outputRect = m_ImageHandler->getBoundingRect();
      m_InputProjection->saveState( kwl, prefix );

      ossimGpt ulg,llg,lrg,urg;
      m_InputProjection->lineSampleToWorld( outputRect.ul(), ulg );
      m_InputProjection->lineSampleToWorld( outputRect.ll(), llg );
      m_InputProjection->lineSampleToWorld( outputRect.lr(), lrg );
      m_InputProjection->lineSampleToWorld( outputRect.ur(), urg );

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

      ossimDpt gsd = m_InputProjection->getMetersPerPixel();
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
      cerr << "No projection geometry for file " << m_ImageHandler->getFilename() << endl;
   }
}

// Modified from image_info.cpp outputGeneralImageInfo() routine
void ossimVirtualImageWriter::saveGeneralKeywordEntry( ossimKeywordlist& kwl,
                                                       const ossimString& prefix ) const
{
   ossimDrect boundingRect = m_ImageHandler->getBoundingRect();

   kwl.add( prefix, ossimKeywordNames::UL_X_KW, boundingRect.ul().x, true );
   kwl.add( prefix, ossimKeywordNames::UL_Y_KW, boundingRect.ul().y, true );
   kwl.add( prefix, ossimKeywordNames::LR_X_KW, boundingRect.lr().x, true );
   kwl.add( prefix, ossimKeywordNames::LR_Y_KW, boundingRect.lr().y, true );

   kwl.add( prefix,
            ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
            m_ImageHandler->getNumberOfInputBands(),
            true );
   kwl.add( prefix,
            ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
            m_ImageHandler->getNumberOfOutputBands(),
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
   for( i = 0; i < (int)m_ImageHandler->getNumberOfInputBands(); ++i )
   {
      ossimString band = ossimString( "band" ) + ossimString::toString( i ) + ".";

      kwl.add( prefix,
               band+"null_value",
               m_ImageHandler->getNullPixelValue( i ),
               true );
      kwl.add( prefix,
               band+"min_value",
               m_ImageHandler->getMinPixelValue( i ),
               true );
      kwl.add( prefix,
               band+"max_value",
               m_ImageHandler->getMaxPixelValue( i ),
               true );
   }

   // Output Radiometry.
   ossimString scalarStr("unknown");
   ossimScalarType scalar = m_ImageHandler->getOutputScalarType();
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

bool ossimVirtualImageWriter::initializeOutputFilenamFromHandler()
{
   if ( !m_ImageHandler.valid() )
   {
      return false;
   }

   // Set the output filename to a default.
   m_OutputFile = m_ImageHandler->getFilename();
   if( m_ImageHandler->getNumberOfEntries() > 1 )
   {
      ossim_uint32 currentEntry = m_ImageHandler->getCurrentEntry();
      m_OutputFile.setExtension( "" );
      m_OutputFile += "_e";
      m_OutputFile += ossimString::toString( currentEntry );

      // .ovr: file extension for all overview files in OSSIM.
      // .ovi: "ossim virtual image" for non-overview output images.
      m_OutputFile += m_OverviewFlag ? ".ovr" : "ovi";
   }
   else
   {
      // .ovr: file extension for all overview files in OSSIM.
      // .ovi: "ossim virtual image" for non-overview output images.
      m_OutputFile.setExtension( m_OverviewFlag ? ".ovr" : "ovi" );
   }

   if ( m_OutputFile == m_ImageHandler->getFilename() )
   {
      // Don't allow this.
      m_OutputFile = ossimFilename::NIL;
   }

   // Temporary header file used to help build Rn for n>1.
   m_OutputFileTmp = m_OutputFile + ".tmp";

   if ( m_OutputFile.empty() )
   {
      return false;
   }

   return true;
}
