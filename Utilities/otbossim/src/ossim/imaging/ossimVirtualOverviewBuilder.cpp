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
// Contains class definition for VirtualOverviewBuilder
// 
//*******************************************************************
//  $Id: ossimVirtualOverviewBuilder.cpp 14780 2009-06-25 19:32:32Z dburken $

#include <algorithm> /* for std::fill */
using namespace std;

#include <tiffio.h>
#include <xtiffio.h>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimVirtualImageTiffWriter.h>
#include <ossim/imaging/ossimVirtualOverviewBuilder.h>

const char* ossimVirtualOverviewBuilder::OUTPUT_FRAME_SIZE_KW =
   "output_frame_size";

RTTI_DEF1(ossimVirtualOverviewBuilder,
          "ossimVirtualOverviewBuilder",
          ossimOverviewBuilderBase)

static ossimTrace traceDebug("ossimVirtualOverviewBuilder:debug");

// Property keywords.
static const char COPY_ALL_KW[] = "copy_all_flag";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimVirtualOverviewBuilder.cpp 14780 2009-06-25 19:32:32Z dburken $";
#endif

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimVirtualOverviewBuilder::ossimVirtualOverviewBuilder()
   :
      ossimOverviewBuilderBase(),
      theImageHandler(0),
      theOutputFile(ossimFilename::NIL),
      theOutputTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
      theOutputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
      theResamplerType(ossimFilterResampler::ossimFilterResampler_BOX),
      theCopyAllFlag(false),
      theCompressType(""),
      theCompressQuality(0),
      theProgressListener(0),
      theWriterType(WRITER_TYPE_UNKNOWN)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualOverviewBuilder::ossimVirtualOverviewBuilder DEBUG:\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  "
         << OSSIM_ID
         << "\n";
#endif
   }

   //---
   // Check for a listeners. If the list is empty, add a standard out
   // listener so that command line apps like img2rr will get some 
   // progress.
   //---
   if ( theListenerList.empty() )
   {
      theProgressListener = new ossimStdOutProgress( 0, true );
      addListener(theProgressListener);
   }

   ossim::defaultTileSize(theOutputTileSize);
}

ossimVirtualOverviewBuilder::~ossimVirtualOverviewBuilder()
{
   theImageHandler = 0;

   if (theProgressListener)
   {
      removeListener(theProgressListener);
      delete theProgressListener;
      theProgressListener = 0;
   }
}

void ossimVirtualOverviewBuilder::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType )
{
   theResamplerType = resampleType;
}

bool ossimVirtualOverviewBuilder::buildOverview(
   const ossimFilename& overview_file, bool copy_all )
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualOverviewBuilder::buildOverview DEBUG:"
         << "\noverview file: " << overview_file.c_str()
         << "\ncopy_all flag: " << (copy_all?"true":"false")
         << std::endl;
   }

   theOutputFile  = overview_file;
   theCopyAllFlag = copy_all;

   return execute();
}

bool ossimVirtualOverviewBuilder::execute()
{
   static const char MODULE[] = "ossimVirtualOverviewBuilder::execute";

   if (theErrorStatus == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError status has been previously set!  Returning..."
         << std::endl;
      return false;
   }

   if ( !theImageHandler.valid() )
   {
      return false;
   }

   // Let's initialize our helper class...
   ossimRefPtr<ossimVirtualImageWriter> pWriter = 0;
   switch( theWriterType )
   {
      case WRITER_TYPE_TIFF:
         pWriter = new ossimVirtualImageTiffWriter( theOutputFile,
                                                    theImageHandler.get(),
                                                    true );
         break;

      case WRITER_TYPE_UNKNOWN:
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nError The overview type has not been set to a known value..."
            << std::endl;
         return false;
   }

   // Configure the writer
   pWriter->setOutputTileSize    ( theOutputTileSize );
   pWriter->setOutputFrameSize   ( theOutputFrameSize );
   pWriter->setResampleType      ( theResamplerType );
   pWriter->setCompressionType   ( theCompressType );
   pWriter->setCompressionQuality( theCompressQuality );

   // The setCopyAllFlag() call forces the building of R0 in the virtual
   // image. Keep in ming that OSSIM does not count of number of decimation 
   // levels correctly when the virtual image is labeled as an overview in 
   // the header .ovr file but is loaded directly into OSSIM (i.e. not 
   // used as an overview but as a standalone image). 
   pWriter->setCopyAllFlag( theCopyAllFlag );

   // Set the RPF frames to update to the virtual writer.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, theImageHandler.get() );
   int nFrames = (int)theDirtyFrameList.size();
   if ( pRpf != NULL && nFrames > 0 )
   {
      int idx;
      for( idx=0; idx<nFrames; ++idx )
      {
         // Add the name of a frame to the dirty frame list of 
         // the virtual image writer.
         pWriter->setDirtyFrame( theDirtyFrameList[idx] );
      }
   }

   // Check the output filename.  Disallow same file overview building.
   theOutputFile = pWriter->getOutputFile();
   if ( theImageHandler->getFilename() == theOutputFile )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Source image file and overview file cannot be the same!"
         << std::endl;
      return false;
   }

   // Have the writer build the virtual overview
   bool bResult = pWriter->execute();

   /* Let's delete our helper class... */
   pWriter = 0;

   return bResult;
}

bool ossimVirtualOverviewBuilder::getCopyAllFlag() const
{
   return theCopyAllFlag;
}

void ossimVirtualOverviewBuilder::setCopyAllFlag( bool flag )
{
   theCopyAllFlag = flag;
}

ossimObject* ossimVirtualOverviewBuilder::getObject()
{
   return this;
}

const ossimObject* ossimVirtualOverviewBuilder::getObject() const
{
   return this;
}

ossimFilename ossimVirtualOverviewBuilder::getOutputFile() const
{
   return theOutputFile;
}

void ossimVirtualOverviewBuilder::setOutputFile( const ossimFilename& file )
{
   theOutputFile = file;
}

void ossimVirtualOverviewBuilder::setOutputTileSize( const ossimIpt& tileSize )
{
   theOutputTileSize = tileSize;
}

void ossimVirtualOverviewBuilder::setOutputFrameSize( const ossimIpt& frameSize )
{
   theOutputFrameSize = frameSize;
}

bool ossimVirtualOverviewBuilder::setInputSource( ossimImageHandler* imageSource )
{
   static const char MODULE[] =
      "ossimVirtualOverviewBuilder::setInputSource";

   theImageHandler = imageSource;

   if ( !theImageHandler.valid() )
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image handler pointer passed to constructor! Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to constructor!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }
   else if (theImageHandler->getErrorStatus() ==
            ossimErrorCodes::OSSIM_ERROR)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError detected in image handler!  Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s file %s line %d\nImageHandler error detected!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nTile Width:   " << theOutputTileSize.x
           << "\nTile Height:  " << theOutputTileSize.y
           << "\nFrame Width:  " << theOutputFrameSize.x
           << "\nFrame Height: " << theOutputFrameSize.y
           << "\nSource image is tiled:  "
           << (theImageHandler->isImageTiled()?"true":"false")
           << "\ntheImageHandler->getTileWidth():  "
           << theImageHandler->getTileWidth()
           << "\ntheImageHandler->getTileHeight():  "
           << theImageHandler->getTileHeight()
           << "\ntheImageHandler->getImageTileWidth():  "
           << theImageHandler->getImageTileWidth()
           << "\ntheImageHandler->getImageTileHeight():  "
           << theImageHandler->getImageTileHeight()
           << std::endl;
   }

   return true;
}

bool ossimVirtualOverviewBuilder::setOverviewType( const ossimString& type )
{
   bool result = true;
   if ( ossimVirtualImageTiffWriter::isOverviewTypeHandled(type) )
   {
      theWriterType = WRITER_TYPE_TIFF;
      theResamplerType = ossimVirtualImageTiffWriter::getResamplerType(type);
   }
   else
   {
      result = false;
   }

   return result;
}

ossimString ossimVirtualOverviewBuilder::getOverviewType() const
{
   static const char MODULE[] = "ossimVirtualOverviewBuilder::getOverviewType";

   ossimString overviewType("unknown");

   // Let's initialize our helper class...
   switch( theWriterType )
   {
      case WRITER_TYPE_TIFF:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "\nError The tiff-based virtual overview type has not been implemented yet..."
               << std::endl;
         }
         overviewType = ossimVirtualImageTiffWriter::getOverviewType( theResamplerType );
         break;

      case WRITER_TYPE_UNKNOWN:
      default:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << " ERROR:"
               << "\nError The overview type has not been set to a known value..."
               << std::endl;
         }
         break;
   }

   return overviewType;
}

void ossimVirtualOverviewBuilder::getTypeNameList(
                                    std::vector<ossimString>& typeList )const
{
   ossimVirtualImageTiffWriter::getTypeNameList( typeList );
}

void ossimVirtualOverviewBuilder::setProperty( ossimRefPtr<ossimProperty> prop )
{
   if( prop->getName() == ossimKeywordNames::COMPRESSION_QUALITY_KW )
   {
      theCompressQuality = prop->valueToString().toInt32();
   }
   else 
   if( prop->getName() == ossimKeywordNames::COMPRESSION_TYPE_KW )
   {
      ossimString tempStr = prop->valueToString();
      theCompressType = tempStr.downcase();
   }
   else
   if( prop->getName() == COPY_ALL_KW )
   {
      theCopyAllFlag = prop->valueToString().toBool();
   }
   else 
   if( prop->getName() == ossimKeywordNames::OUTPUT_TILE_SIZE_KW )
   {
      ossimIpt ipt;

      ipt.toPoint( prop->valueToString() );

      setOutputTileSize( ipt );
   }
   else
   if( prop->getName() == ossimVirtualOverviewBuilder::OUTPUT_FRAME_SIZE_KW)
   {
      ossimIpt ipt;

      ipt.toPoint( prop->valueToString() );

      setOutputFrameSize(ipt);
   }
}

void ossimVirtualOverviewBuilder::getPropertyNames( std::vector<ossimString>& propNames )const
{
   propNames.push_back( ossimKeywordNames::COMPRESSION_QUALITY_KW );
   propNames.push_back( ossimKeywordNames::COMPRESSION_TYPE_KW );
   propNames.push_back( COPY_ALL_KW );
   propNames.push_back( ossimOverviewBuilderBase::OVERVIEW_STOP_DIMENSION_KW );
}

bool ossimVirtualOverviewBuilder::canConnectMyInputTo(
                                    ossim_int32 index,
                                    const ossimConnectableObject* obj ) const
{
   return ( ( index == 0 ) && PTR_CAST( ossimImageHandler, obj ) ) ? true : false;
}

void ossimVirtualOverviewBuilder::setDirtyFrame( const ossimString& name )
{
   // Add the name of a frame to the dirty frame list.
   theDirtyFrameList.push_back(name);
}
