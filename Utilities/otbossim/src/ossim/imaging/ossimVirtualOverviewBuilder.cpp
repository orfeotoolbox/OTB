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

#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimVirtualImageWriter.h>
#include <ossim/imaging/ossimVirtualImageHandler.h>
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
      m_ImageHandler(0),
      m_OutputFile(ossimFilename::NIL),
      m_OutputTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
      m_OutputFrameSize(OSSIM_DEFAULT_FRAME_WIDTH, OSSIM_DEFAULT_FRAME_HEIGHT),
      m_ResamplerType(ossimFilterResampler::ossimFilterResampler_BOX),
      m_CopyAllFlag(false),
      m_ProgressListener(0),
      m_DirtyFrameList(0),
      m_OutputWriter(0)
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
      m_ProgressListener = new ossimStdOutProgress( 0, true );
      addListener(m_ProgressListener);
   }

   ossim::defaultTileSize(m_OutputTileSize);
}

ossimVirtualOverviewBuilder::~ossimVirtualOverviewBuilder()
{
   if (m_ProgressListener)
   {
      removeListener(m_ProgressListener);
      delete m_ProgressListener;
      m_ProgressListener = 0;
   }
}

void ossimVirtualOverviewBuilder::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType )
{
   m_ResamplerType = resampleType;
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

   m_OutputFile  = overview_file;
   m_CopyAllFlag = copy_all;

   return execute();
}

bool ossimVirtualOverviewBuilder::execute()
{
   static const char MODULE[] = "ossimVirtualOverviewBuilder::execute";

   if (getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError status has been previously set!  Returning..."
         << std::endl;
      return false;
   }

   if ( !m_ImageHandler.valid() )
   {
      return false;
   }

   // Let's initialize our helper class...
   ossimRefPtr<ossimVirtualImageWriter> pWriter = 
      new ossimVirtualImageWriter( m_OutputFile,
                                   m_ImageHandler.get(),
                                   true );

   // Configure the virtual writer
   pWriter->setOutputTileSize ( m_OutputTileSize );
   pWriter->setOutputFrameSize( m_OutputFrameSize );
   pWriter->setResampleType   ( m_ResamplerType );
   pWriter->setFrameWriter    ( m_OutputWriter.get() );

   // The setCopyAllFlag() call forces the building of R0 in the virtual
   // image. Keep in ming that OSSIM does not count of number of decimation 
   // levels correctly when the virtual image is labeled as an overview in 
   // the header .ovr file but is loaded directly into OSSIM (i.e. not 
   // used as an overview but as a standalone image). 
   pWriter->setCopyAllFlag( m_CopyAllFlag );

   // Set the RPF frames to update to the virtual writer.
   ossimCibCadrgTileSource* pRpf = PTR_CAST( ossimCibCadrgTileSource, 
                                             m_ImageHandler.get() );
   int nFrames = (int)m_DirtyFrameList.size();
   if ( pRpf != NULL && nFrames > 0 )
   {
      int idx;
      for( idx=0; idx<nFrames; ++idx )
      {
         // Add the name of a frame to the dirty frame list of 
         // the virtual image writer.
         pWriter->setDirtyFrame( m_DirtyFrameList[idx] );
      }
   }

   // Check the output filename.  Disallow same file overview building.
   m_OutputFile = pWriter->getOutputFile();
   if ( m_ImageHandler->getFilename() == m_OutputFile )
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
   return m_CopyAllFlag;
}

void ossimVirtualOverviewBuilder::setCopyAllFlag( bool flag )
{
   m_CopyAllFlag = flag;
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
   return m_OutputFile;
}

void ossimVirtualOverviewBuilder::setOutputFile( const ossimFilename& file )
{
   m_OutputFile = file;
}

void ossimVirtualOverviewBuilder::setOutputTileSize( const ossimIpt& tileSize )
{
   m_OutputTileSize = tileSize;
}

void ossimVirtualOverviewBuilder::setOutputFrameSize( const ossimIpt& frameSize )
{
   m_OutputFrameSize = frameSize;
}

bool ossimVirtualOverviewBuilder::setOutputWriter( ossimImageFileWriter* outputWriter )
{
   static const char MODULE[] =
   "ossimVirtualOverviewBuilder::setOutputWriter";

   m_OutputWriter = outputWriter;

   if ( !m_OutputWriter.valid() )
   {
      // Set the error...
      setErrorStatus();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image writer pointer passed to overview builder! Returning..."
         << std::endl;
      ossimSetError(getClassName(),
         ossimErrorCodes::OSSIM_ERROR,
         "%s File %s line %d\nNULL pointer passed to overview builder!",
         MODULE,
         __FILE__,
         __LINE__);
      return false;
   }
   else
   if ( m_OutputWriter->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR )
   {
      // Set the error...
      setErrorStatus();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError detected in overview builder!  Returning..."
         << std::endl;
      ossimSetError(getClassName(),
         ossimErrorCodes::OSSIM_ERROR,
         "%s file %s line %d\nImageFileWriter error detected!",
         MODULE,
         __FILE__,
         __LINE__);
      return false;
   }

   return true;
}

bool ossimVirtualOverviewBuilder::setInputSource( ossimImageHandler* imageSource )
{
   static const char MODULE[] =
      "ossimVirtualOverviewBuilder::setInputSource";

   m_ImageHandler = imageSource;

   if ( !m_ImageHandler.valid() )
   {
      // Set the error...
      setErrorStatus();
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image handler pointer passed to overview builder! Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to overview builder!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }
   else
   if ( m_ImageHandler->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR )
   {
      // Set the error...
      setErrorStatus();
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

   const ossimImageHandler* pOverview = m_ImageHandler->getOverview();
   if ( pOverview != 0 )
   {
      // If the overview is virtual, let's close it. It might be the same
      // virtual image that we're trying to build or add on to, which would
      // create confusion down in the virtual writer.
      ossimVirtualImageHandler* pVirtual = 
         PTR_CAST( ossimVirtualImageHandler, pOverview );
      if ( pVirtual != 0 )
      {
         m_ImageHandler->closeOverview();
      }
   }

   return true;
}

bool ossimVirtualOverviewBuilder::setOverviewType( const ossimString& type )
{
   bool bResult = true;
   if ( ossimVirtualImageWriter::isOverviewTypeHandled(type) )
   {
      m_ResamplerType = ossimVirtualImageWriter::getResamplerType(type);
   }
   else
   {
      bResult = false;
   }

   return bResult;
}

ossimString ossimVirtualOverviewBuilder::getOverviewType() const
{
   return ossimVirtualImageWriter::getOverviewType( m_ResamplerType );
}

void ossimVirtualOverviewBuilder::getTypeNameList(
                                    std::vector<ossimString>& typeList ) const
{
   ossimVirtualImageWriter::getTypeNameList( typeList );
}

void ossimVirtualOverviewBuilder::setProperty( ossimRefPtr<ossimProperty> prop )
{
   if( prop->getName() == COPY_ALL_KW )
   {
      m_CopyAllFlag = prop->valueToString().toBool();
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
   propNames.push_back( COPY_ALL_KW );
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
   m_DirtyFrameList.push_back(name);
}
