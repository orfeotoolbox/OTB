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
// Contains class definition for VirtualImageHandler.
//
//*******************************************************************
//  $Id: ossimVirtualImageHandler.cpp 14655 2009-06-05 11:58:56Z dburken $

#include <cstdlib> /* for abs(int) */

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimVirtualImageHandler.h>

RTTI_DEF1( ossimVirtualImageHandler, "ossimVirtualImageHandler", ossimImageHandler )

static ossimTrace traceDebug( "ossimVirtualImageHandler:debug" );

// Virtual image identifier string, found on 1st line of header file:
static const char OSSIM_VIRTUAL_IMAGE_ID[] = "OSSIM_Virtual_Image";

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimVirtualImageHandler::ossimVirtualImageHandler( ossim_uint32 srcResLevel )
   :
      ossimImageHandler(),
      m_Buffer(0),
      m_BufferSize(0),
      m_BufferRect(0, 0, 0, 0),
      m_NullBuffer(0),
      m_SampleFormatUnit(0),
      m_BitsPerSample(0),
      m_BytesPerPixel(0),
      m_ImageSubdirectory(""),
      m_VirtualWriterType(""),
      m_MajorVersion(""),
      m_MinorVersion(""),
      m_CompressType(1),
      m_CompressQuality(75),
      m_OverviewFlag(false),
      m_OpenedFlag(false),
      m_R0isFullRes(false),
      m_EntryIndex(-1),
      m_ResLevelStart(0),
      m_ResLevelEnd(0),
      m_SamplesPerPixel(0),
      m_NumberOfResLevels(0),
      m_InterleaveType(OSSIM_BIL),
      m_ScalarType(OSSIM_SCALAR_UNKNOWN),
      m_ImageFrameWidth(-1),
      m_ImageFrameLength(-1),
      m_R0NumberOfLines(-1),
      m_R0NumberOfSamples(-1),
      m_Tile(0),
      m_ImageWidth(0),
      m_ImageLength(0),
      m_CurrentFrameExt(""),
      m_FrameHandlerInfoList(0),
      m_CurrentTileWidth(0),
      m_CurrentTileHeight(0),
      m_CurrentResLevel(-1),
      m_CurrentRow(-1),
      m_CurrentCol(-1),
      m_CurrentImageHandler(0),
      m_SourceResLevel(srcResLevel),
      m_NumberInputFramesX(0),
      m_NumberInputFramesY(0)
{}

ossimVirtualImageHandler::~ossimVirtualImageHandler()
{
   close();
}

bool ossimVirtualImageHandler::open( const ossimFilename& image_file )
{
   theImageFile = image_file;
   return open();
}

void ossimVirtualImageHandler::close()
{
   m_OpenedFlag = false;

   m_ImageWidth.clear();
   m_ImageLength.clear();

   // Shutdown the list of image handlers.
   ossim_uint32 i;
   ossim_uint32 nSavedHandlers = (ossim_uint32)m_FrameHandlerInfoList.size();
   for ( i=0; i<nSavedHandlers; ++i )
   {
      FrameHandlerInfo* pInfo = m_FrameHandlerInfoList[i];
      if ( pInfo != 0 )
      {
         ossimImageHandler* pHandler = pInfo->pHandler;
         if ( pHandler != 0 )
         {
            pHandler->close();
         }
         delete pInfo;
      }
   }
   m_FrameHandlerInfoList.clear();

   if (m_Buffer)
   {
      delete [] m_Buffer;
      m_Buffer = 0;
      m_BufferSize = 0;
   }
   if (m_NullBuffer)
   {
      delete [] m_NullBuffer;
      m_NullBuffer = 0;
   }
   ossimImageHandler::close();
}

bool ossimVirtualImageHandler::open()
{
   static const char MODULE[] = "ossimVirtualImageHandler::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Entered..."
         << "\nFile:  " << theImageFile.c_str() << std::endl;
   }

   if(isOpen())
   {
      close();
   }

   if ( theImageFile.empty() )
   {
      return false;
   }
   if ( theImageFile.isReadable() == false )
   {
      return false;
   }

   // If the file is not an overview, then it could be any format.
   // To reduce the chance that the ossimKeywordlist file parser will
   // get stuck or crash on it, let's check for the expected identifier 
   // on the first line.
   if ( theImageFile.ext() != "ovr" )
   {
      // Open the file and try to find the 
      // OSSIM_VIRTUAL_IMAGE_ID on the 1st line.
      std::ifstream imgStream;
      imgStream.open(theImageFile.c_str(), std::ios::in | std::ios::binary);

      if ( !imgStream.is_open() )
      {
         // If first you don't succeed with the given filename, 
         // try again with the results of a case insensitive search.
         ossimDirectory directory(theImageFile.path());
         ossimFilename filename(theImageFile.file());

         std::vector<ossimFilename> result;
         bool bSuccess = directory.findCaseInsensitiveEquivalents( filename, result );
         if ( bSuccess == true )
         {
            int numResults = (int)result.size();
            int i;
            for ( i=0; i<numResults && !imgStream.is_open(); ++i )
            {
               imgStream.open( result[i].c_str(), std::ios::in | std::ios::binary );

               // For now on use the name that worked opening the header.
               theImageFile = result[i];
            }
         }

         if ( !imgStream.is_open() )
         {
            if ( traceDebug() ) 
            {
               // report all errors that aren't existence problems.
               // we want to know about things like permissions, too many open files, etc.
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "Error opening file: " << theImageFile.c_str() << std::endl;
            }
            return false;
         }
      }

      // Read in a block.
      char buf[1024];

      memset(buf, '\0', 1024);
//      std::string line;
     // std::getline(imgStream, line);
      imgStream.read(buf, 1023);
      ossimString lineStr = ossimString(buf);
      bool bFoundID = lineStr.contains(OSSIM_VIRTUAL_IMAGE_ID);
      imgStream.close();
      if ( bFoundID == false )
      {
         // Could not find the OSSIM_VIRTUAL_IMAGE_ID
         // on the 1st line of the header file.
         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Could not find OSSIM_Virtual_Image identifier on 1st line of "
               << theImageFile.c_str() 
               << std::endl;
         }
         return false;
      }
   } // if ( theImageFile.ext() != "ovr" )

   ossimKeywordlist header_kwl( theImageFile );

   if ( header_kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << " Keywordlist open error detected." << endl;
      }
      return false;
   }

   m_OpenedFlag = loadHeaderInfo( header_kwl );
   m_OpenedFlag = m_OpenedFlag ? initializeBuffers() : false;

   return m_OpenedFlag;
}

bool ossimVirtualImageHandler::loadState( const ossimKeywordlist& kwl,
                                          const char* prefix )
{
   const char* MODULE = "ossimVirtualImageHandler::loadState";

   if(traceDebug())
   {
      CLOG << "Entering..." << endl;
   }
   bool result = ossimImageHandler::loadState(kwl, prefix);

   if(!result)
   {
      if(traceDebug())
      {
         CLOG << "Leaving..." << endl;
      }
      return false;
   }

   return ossimImageHandler::open(theImageFile);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimVirtualImageHandler::loadHeaderInfo( const ossimKeywordlist& kwl )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadHeaderInfo";

   bool bRetVal = true;

   // Virtual images currently can only have 1 entry.
   ossimString lookupStr = kwl.find( ossimKeywordNames::NUMBER_ENTRIES_KW );
   if ( lookupStr.empty() == false )
   {
      ossim_int16 numEntries = lookupStr.toInt16();
      if ( numEntries != 1 )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nERROR: Number of entries (" << numEntries << ") in virtual image header != 1."
            << std::endl;

         bRetVal = false;
      }
      else
      {
         std::vector<ossimString> keyList = kwl.findAllKeysThatContains(
            ossimKeywordNames::ENTRY_KW );

         int numKeys = (int)keyList.size();
         if ( numKeys != 1 )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nERROR: Number of entry lines (" << numKeys << ") in virtual image header != 1."
               << std::endl;

            bRetVal = false;
         }
         else
         {
            ossimString key = keyList[0];
            ossimString lookupStr = kwl.find( key );
            if ( lookupStr.empty() == false )
            {
               m_EntryIndex = lookupStr.toInt16();
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nERROR: No valid entries found in virtual image header."
                  << std::endl;

               bRetVal = false;
            }
         }
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE
            << "\nERROR: Number of entries not found in virtual image header."
            << std::endl;
      }

      bRetVal = false;
   }

   if ( m_EntryIndex > -1 )
   {
      ossimString prefix = "image";
      prefix += ossimString::toString( m_EntryIndex ) + ".";

      loadGeometryKeywordEntry( kwl, prefix );
      loadGeneralKeywordEntry ( kwl, prefix );
      loadNativeKeywordEntry  ( kwl, prefix );
   }

   return bRetVal;
}

void ossimVirtualImageHandler::loadNativeKeywordEntry( const ossimKeywordlist& kwl,
                                                       const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadNativeKeywordEntry";

   ossimString extPrefix = prefix + ossimString( "virtual" ) + ".";

   ossimString lookupStr = kwl.find( extPrefix, "subdirectory" );
   if ( lookupStr.empty() == false )
   {
      m_ImageSubdirectory = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "writer_type" );
   if ( lookupStr.empty() == false )
   {
      m_VirtualWriterType = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "frame_size_x" );
   if ( lookupStr.empty() == false )
   {
      m_ImageFrameWidth = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "frame_size_y" );
   if ( lookupStr.empty() == false )
   {
      m_ImageFrameLength = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "number_input_frames_x" );
   if ( lookupStr.empty() == false )
   {
      m_NumberInputFramesX = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "number_input_frames_y" );
   if ( lookupStr.empty() == false )
   {
      m_NumberInputFramesY = lookupStr.toInt32();
   }
   lookupStr = kwl.find( extPrefix, "version_major" );
   if ( lookupStr.empty() == false )
   {
      m_MajorVersion = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "version_minor" );
   if ( lookupStr.empty() == false )
   {
      m_MinorVersion = lookupStr;
   }
   lookupStr = kwl.find( extPrefix, "overview_flag" );
   if ( lookupStr.empty() == false )
   {
      m_OverviewFlag = lookupStr.toBool();
      setStartingResLevel( m_OverviewFlag ? 1 : 0 );
   }
   lookupStr = kwl.find( extPrefix, "includes_r0" );
   if ( lookupStr.empty() == false )
   {
      m_R0isFullRes = lookupStr.toBool();
   }
   lookupStr = kwl.find( extPrefix, "bits_per_sample" );
   if ( lookupStr.empty() == false )
   {
      m_BitsPerSample = lookupStr.toUInt16();
   }
   lookupStr = kwl.find( extPrefix, "bytes_per_pixel" );
   if ( lookupStr.empty() == false )
   {
      m_BytesPerPixel = lookupStr.toUInt32();
   }
   lookupStr = kwl.find( extPrefix, "resolution_level_starting" );
   if ( lookupStr.empty() == false )
   {
      m_ResLevelStart = lookupStr.toUInt16();
   }
   lookupStr = kwl.find( extPrefix, "resolution_level_ending" );
   if ( lookupStr.empty() == false )
   {
      m_ResLevelEnd = lookupStr.toUInt16();
   }

   // number of resolution levels available in the virtual image
   m_NumberOfResLevels = m_ResLevelEnd - m_ResLevelStart + 1;

   m_ImageWidth.resize(m_NumberOfResLevels);
   m_ImageLength.resize(m_NumberOfResLevels);

   extPrefix += ossimString( "resolution_level_" );

   ossim_uint32 r;
   for ( r=m_ResLevelStart; r<=m_ResLevelEnd; ++r )
   {
      ossim_int32 samples = m_R0NumberOfSamples >> r;
      ossim_int32 lines   = m_R0NumberOfLines   >> r;

      ossim_uint32 idx = r - m_ResLevelStart;
      m_ImageWidth [idx] = samples;
      m_ImageLength[idx] = lines;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE
         << "\nVirtual image information:"
         << "\nSubdirectory for frames:  " << m_ImageSubdirectory
         << "\nWriter type:              " << m_VirtualWriterType
         << "\nFrame size (x):           " << m_ImageFrameWidth
         << "\nFrame size (y):           " << m_ImageFrameLength
         << "\nMajor version:            " << m_MajorVersion
         << "\nMinor version:            " << m_MinorVersion
         << "\nOverview flag (boolean):  " << m_OverviewFlag
         << "\nStarting reduced res set: " << m_ResLevelStart
         << "\nEnding reduced res sets:  " << m_ResLevelEnd
         << std::endl;

         for ( r=m_ResLevelStart; r<=m_ResLevelEnd; ++r )
         {
            ossim_uint32 idx = r - m_ResLevelStart;
            ossimNotify(ossimNotifyLevel_WARN)
               << "Virtual image width [" << r << "]: " << m_ImageWidth [idx] << "\n"
               << "Virtual image length[" << r << "]: " << m_ImageLength[idx]
               << std::endl;
         }
   }
}

void ossimVirtualImageHandler::loadGeometryKeywordEntry( const ossimKeywordlist& kwl,
                                                         const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadGeometryKeywordEntry";

   ossimKeywordlist tempKwl(kwl);
   tempKwl.stripPrefixFromAll( prefix );

   const char* lookup = tempKwl.find(ossimKeywordNames::TYPE_KW);
   if ( lookup )
   {
      getInternalImageGeometry()->loadState( tempKwl );
   }
   else
   {
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nNo projection type found in: " 
            << theImageFile
            << std::endl;
      }
   }
}

void ossimVirtualImageHandler::loadGeneralKeywordEntry( const ossimKeywordlist& kwl,
                                                        const ossimString& prefix )
{
   static const char MODULE[] = "ossimVirtualImageHandler::loadGeneralKeywordEntry";

   /* Get the nul, min, max values, as a function of band index */
   loadMetaData( kwl, prefix );

   ossimString lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_INPUT_BANDS_KW );
   if ( lookupStr.empty() == false )
   {
      m_SamplesPerPixel = lookupStr.toUInt16();
   }

   lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_LINES_KW );
   if ( lookupStr.empty() == false )
   {
      m_R0NumberOfLines = lookupStr.toInt32();
   }

   lookupStr = kwl.find( prefix, ossimKeywordNames::NUMBER_SAMPLES_KW );
   if ( lookupStr.empty() == false )
   {
      m_R0NumberOfSamples = lookupStr.toInt32();
   }

   lookupStr = kwl.find( prefix, "radiometry" );
   m_ScalarType = OSSIM_SCALAR_UNKNOWN;
   if ( lookupStr.empty() == false )
   {
      if ( lookupStr.contains("8-bit") )
      {
         m_ScalarType = OSSIM_UINT8;
      }
      else
      if ( lookupStr.contains("11-bit") )
      {
         m_ScalarType = OSSIM_USHORT11;
      }
      else
      if ( lookupStr.contains("16-bit unsigned") )
      {
         m_ScalarType =  OSSIM_UINT16;
      }
      else
      if ( lookupStr.contains("16-bit signed") )
      {
         m_ScalarType =  OSSIM_SINT16;
      }
      else
      if ( lookupStr.contains("32-bit unsigned") )
      {
         m_ScalarType =  OSSIM_UINT32;
      }
      else
      if ( lookupStr.contains("float") )
      {
         m_ScalarType =  OSSIM_FLOAT32;
      }
      else
      if ( lookupStr.contains("normalized float") )
      {
         m_ScalarType =  OSSIM_FLOAT32;
      }
      else
      {
         /* Do nothing */

         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nERROR: Unrecognized pixel scalar type description: " 
            <<  lookupStr
            << std::endl;
      }
   }
}

void ossimVirtualImageHandler::loadMetaData( const ossimKeywordlist& kwl,
                                             const char* prefix )
{
   theMetaData.clear();
   theMetaData.loadState( kwl, prefix );
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfLines( ossim_uint32 resLevel ) const
{
   ossim_uint32 result = 0;

   if ( m_OpenedFlag && isValidRLevel(resLevel) )
   {
      //---
      // If we have r0 our reslevels are the same as the callers so
      // no adjustment necessary.
      //---
      if ( !getStartingResLevel() || m_R0isFullRes ) // not an overview or has r0.
      {
         //---
         // If we have r0 our reslevels are the same as the callers so
         // no adjustment necessary.
         //---
         if (resLevel < m_NumberOfResLevels)
         {
            result = m_ImageLength[resLevel];
         }
      }
      else // this is an overview without r0
      if (resLevel >= getStartingResLevel())
      {
         //---
         // Adjust the level to be relative to the reader using this as
         // overview.
         //---
         ossim_uint32 level = resLevel - getStartingResLevel();
         if (level < m_NumberOfResLevels)
         {
            result = m_ImageLength[level];
         }
      }
   }
   else
   if ( resLevel < getStartingResLevel() &&
        getStartingResLevel() > 0 &&
       !m_R0isFullRes &&
        m_NumberOfResLevels > 0 )
   {
      result = m_ImageLength[0] * (1<<(getStartingResLevel()-resLevel));
   }

   return result;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfSamples( ossim_uint32 resLevel ) const
{
   ossim_uint32 result = 0;

   if ( m_OpenedFlag && isValidRLevel(resLevel) )
   {
      //---
      // If we have r0 our reslevels are the same as the callers so
      // no adjustment necessary.
      //---
      if ( !getStartingResLevel() || m_R0isFullRes ) // not an overview or has r0.
      {
         //---
         // If we have r0 our reslevels are the same as the callers so
         // no adjustment necessary.
         //---
         if (resLevel < m_NumberOfResLevels)
         {
            result = m_ImageWidth[resLevel];
         }
      }
      else // this is an overview without r0
      if (resLevel >= getStartingResLevel())
      {
         //---
         // Adjust the level to be relative to the reader using this as
         // overview.
         //---
         ossim_uint32 level = resLevel - getStartingResLevel();
         if (level < m_NumberOfResLevels)
         {
            result = m_ImageWidth[level];
         }
      }
   }
   else
   if ( resLevel < getStartingResLevel() &&
        getStartingResLevel() > 0  && 
       !m_R0isFullRes && 
        m_NumberOfResLevels > 0 )
   {
      result = m_ImageWidth[0] * (1<<(getStartingResLevel()-resLevel));
   }

   return result;
}

ossimIrect ossimVirtualImageHandler::getImageRectangle(ossim_uint32 resLevel) const
{
   ossimIrect result;

   if( m_OpenedFlag && isValidRLevel(resLevel) )
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
   if ( resLevel < getStartingResLevel() &&
        getStartingResLevel() > 0 &&
       !m_R0isFullRes &&
        m_NumberOfResLevels > 0 )
   {
      ossim_uint32 scale   = (1<<(getStartingResLevel()-resLevel));
      ossim_uint32 lines   = m_ImageLength[0] * scale;
      ossim_uint32 samples = m_ImageWidth[0]  * scale;

      result = ossimIrect(0, 0, samples-1, lines-1);
   }
   else
   {
      result.makeNan();
   }

   return result;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfDecimationLevels() const
{
   ossim_uint32 result = m_NumberOfResLevels;

   if ( m_OverviewFlag && m_R0isFullRes )
   {
      // Don't count r0.
      --result;
   }

   return result;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimVirtualImageHandler::getOutputScalarType() const
{
   return m_ScalarType;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getNumberInputFramesX() const
{
   return isOpen() ? m_NumberInputFramesX : 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getNumberInputFramesY() const
{
   return isOpen() ? m_NumberInputFramesY : 0;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getFrameWidth() const
{
   if( isOpen() )
   {
      return m_ImageFrameWidth;
   }

   return 0;
}  

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimVirtualImageHandler::getFrameHeight() const
{
   if( isOpen() )
   {
      return m_ImageFrameLength;
   }

   return 0;
}

ossimRefPtr<ossimImageData> ossimVirtualImageHandler::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel )
{
   if (m_Tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_Tile->setImageRectangle(tile_rect);

      if ( getTile( *(m_Tile.get()), resLevel ) == false )
      {
         if (m_Tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_Tile->makeBlank();
         }
      }
   }

   m_Tile->setImageRectangle(tile_rect);
   return m_Tile;
}

bool ossimVirtualImageHandler::getTile( ossimImageData& result,
                                        ossim_uint32 resLevel )
{
   static const char MODULE[] ="ossimVirtualImageHandler::getTile";

   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) )
   {
      ossimIrect tile_rect = result.getImageRectangle();

      // This should be the zero base image rectangle for this res level.
      ossimIrect image_rect = getImageRectangle(resLevel);

      //---
      // See if any point of the requested tile is in the image.
      //---
      if ( tile_rect.intersects(image_rect) )
      {
         // Initialize the tile if needed as we're going to stuff it.
         if (result.getDataObjectStatus() == OSSIM_NULL)
         {
            result.initialize();
         }

         if ( (tile_rect.width()  != m_CurrentTileWidth) ||
              (tile_rect.height() != m_CurrentTileHeight) )
         {
            // Current tile size must be set prior to allocateBuffer call.
            m_CurrentTileWidth  = tile_rect.width();
            m_CurrentTileHeight = tile_rect.height();

            // NOTE: Using this buffer will be a thread issue. (drb) 
            allocateBuffer();
         }
         ossimIrect clip_rect = tile_rect.clipToRect(image_rect);

         if ( !tile_rect.completely_within(clip_rect) )
         {
            //---
            // We're not going to fill the whole tile so start with a
            // blank tile.
            //---
            result.makeBlank();
         }

         // Load the tile buffer with data from the tif.
         if ( loadTile( tile_rect, result, resLevel ) )
         {
            result.validate();
            status = true;
         }
         else
         {
            // Would like to change this to throw ossimException.(drb)
            status = false;
            if(traceDebug())
            {
               // Error in filling buffer.
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << " Error filling buffer. Return status = false..."
                  << std::endl;
            }
         }
      } // matches:  if ( tile_rect.intersects(image_rect) )
      else 
      {
         // No part of requested tile within the image rectangle.
         status = true; // Not an error.
         result.makeBlank();
      }

   } // matches: if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) )

   return status;
}

bool ossimVirtualImageHandler::loadTile( const ossimIrect& virtual_clip_rect,
                                         ossimImageData& result,
                                         ossim_uint32 resLevel )
{
   ossim_int32 virtual_minx, virtual_miny, virtual_maxx, virtual_maxy;
   virtual_clip_rect.getBounds( virtual_minx, virtual_miny, 
                                virtual_maxx, virtual_maxy );

   // If we're using overviews (m_SourceResLevel>0), the 
   // effective frame sizes get cut down by factors of 2.
   ossim_int32 frameWidth  = m_ImageFrameWidth;
   ossim_int32 frameLength = m_ImageFrameLength;

   ossim_uint32 resLevelPow = (ossim_uint32)(1<<m_SourceResLevel);
   frameWidth  /= resLevelPow;
   frameLength /= resLevelPow;

   // Get the indices of the frame that contains the top-left corner
   ossim_int32 rowFrameIdxI = virtual_miny / frameLength;
   ossim_int32 colFrameIdxI = virtual_minx / frameWidth;

   // Get the indices of the frame that contains the bottom-right corner
   ossim_int32 rowFrameIdxF = virtual_maxy / frameLength;
   ossim_int32 colFrameIdxF = virtual_maxx / frameWidth;

   // Get the virtual line,sample of the frame origin
   ossimIpt frame_shiftI( colFrameIdxI * frameWidth, 
                          rowFrameIdxI * frameLength );

   ossimIrect clip_rectI( virtual_clip_rect );
   clip_rectI -= frame_shiftI;

   result.setImageRectangle( clip_rectI );

   //***
   // Frame loop in the line (height) direction.
   //***
   ossim_int32 rowFrameIdx;
   for( rowFrameIdx = rowFrameIdxI; rowFrameIdx <= rowFrameIdxF; ++rowFrameIdx )
   {
      // Origin of a tile within a single output frame.
      ossimIpt originOF(0, 0);
      originOF.y = (rowFrameIdx-rowFrameIdxI) * frameLength;

      //***
      // Frame loop in the sample (width) direction.
      //***
      ossim_int32 colFrameIdx;
      for( colFrameIdx = colFrameIdxI; colFrameIdx <= colFrameIdxF; ++colFrameIdx )
      {
         originOF.x = (colFrameIdx-colFrameIdxI) * frameWidth;

         ossimIrect frame_rectOF( originOF.x,
                                  originOF.y,
                                  originOF.x + frameWidth  - 1,
                                  originOF.y + frameLength - 1 );

         if ( frame_rectOF.intersects(clip_rectI) )
         {
            // clip area that intersects with current frame
            ossimIrect frame_clip_rectOF = frame_rectOF.clipToRect(clip_rectI);

            // shift coords of clip area be wrt the current frame
            ossimIrect frame_clip_rectIF(frame_clip_rectOF);
            frame_clip_rectIF -= originOF;

            // Open a single frame file for reading.
            ossimImageHandler* pHandler = openFrame( resLevel, rowFrameIdx, colFrameIdx );

            // Read a tile out of the opened image frame using the ossim image handler.
            ossimRefPtr<ossimImageData> t = pHandler ? 
               pHandler->getTile( frame_clip_rectIF, m_SourceResLevel ) : 0;

            ossimDataObjectStatus doStatus = t.valid() ? 
               t->getDataObjectStatus() : OSSIM_STATUS_UNKNOWN;

            bool bFoundData = ( doStatus == OSSIM_PARTIAL || doStatus == OSSIM_FULL );

            for ( ossim_uint32 band=0; band<m_SamplesPerPixel; ++band )
            {
               if ( bFoundData )
               {
                  // Grab a pointer to the tile for the band.
                  void* buffer = t->getBuf(band);

                  result.loadBand( buffer,
                                   frame_clip_rectOF,
                                   frame_clip_rectOF,
                                   band );
               }
               else
               {
                  // fill with NULLs
                  result.loadBand( m_NullBuffer,
                                   frame_clip_rectOF,
                                   frame_clip_rectOF,
                                   band );
               }
            }
         }
      }
   }

   return true;
}

bool ossimVirtualImageHandler::isValidRLevel( ossim_uint32 resLevel ) const
{
   bool result = false;

   //---
   // If we have r0 our reslevels are the same as the callers so
   // no adjustment necessary.
   //---
   if ( !getStartingResLevel() || m_R0isFullRes) // Not an overview or has r0.
   {
      result = (resLevel < m_NumberOfResLevels);
   }
   else if (resLevel >= getStartingResLevel()) // Used as overview.
   {
      result = ( (resLevel - getStartingResLevel()) < m_NumberOfResLevels);
   }

   return result;
}

bool ossimVirtualImageHandler::allocateBuffer()
{
   //***
   // Allocate memory for a buffer to hold data grabbed from the frame file.
   //***
   ossim_uint32 buffer_size=0;

   // The interleave type is always assumed to be BIL.
   buffer_size = m_ImageFrameWidth  *
                 m_ImageFrameLength *
                 m_BytesPerPixel;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimVirtualImageHandler::allocateBuffer DEBUG:"
         << "\nbuffer_size:  " << buffer_size
         << endl;
   }

   m_BufferRect.makeNan();

   bool bSuccess = true;
   if ( buffer_size != m_BufferSize )
   {
      m_BufferSize = buffer_size;
      if ( m_Buffer )
      {
         delete [] m_Buffer;
      }
      if ( m_NullBuffer )
      {
         delete [] m_NullBuffer;
      }

      // ESH 05/2009 -- Fix for ticket #738:  
      // image_info crashing on aerial_ortho image during ingest
      try
      {
         m_Buffer = new ossim_uint8[buffer_size];
         m_NullBuffer = new ossim_uint8[buffer_size];
      }
      catch(...)
      {
         if ( m_Buffer )
         {
            delete [] m_Buffer;
            m_Buffer = 0;
         }
         if ( m_NullBuffer )
         {
            delete [] m_NullBuffer;
            m_NullBuffer = 0;
         }

         bSuccess = false;
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimVirtualImageHandler::allocateBuffer WARN:"
               << "\nNot enough memory: buffer_size:  " << buffer_size
               << endl;
         }
      }
   }

   // initialize the NULL buffer
   ossim_uint32 b;
   for ( b=0; b<buffer_size; ++b )
   {
      m_NullBuffer[b] = 0;
   }

   return bSuccess;
}

ossim_uint32 ossimVirtualImageHandler::getImageTileWidth() const
{
   return m_ImageFrameWidth;
}

ossim_uint32 ossimVirtualImageHandler::getImageTileHeight() const
{
   return m_ImageFrameLength;
}

ossimString ossimVirtualImageHandler::getLongName() const
{
   return ossimString( "Virtual Image Handler" );
}

ossimString ossimVirtualImageHandler::getShortName() const
{
   return ossimString( "Virtual Image Handler" );
}

std::ostream& ossimVirtualImageHandler::print( std::ostream& os ) const
{
   //***
   // Use a keyword format.
   //***
   os << "image_file:                    " << theImageFile
      << "\nsamples_per_pixel:           " << m_SamplesPerPixel
      << "\nbits_per_sample:             " << m_BitsPerSample
      << "\nsample_format_unit:          " << m_SampleFormatUnit
      << "\nm_NumberOfResLevels:         " << m_NumberOfResLevels
      << "\nr0_is_full_res:              " << m_R0isFullRes;

   for ( ossim_uint32 i=0; i<m_NumberOfResLevels; ++i )
   {
      os << "\ndirectory[" << i << "]"
         << "\nimage width:     " << m_ImageWidth[i]
         << "\nimage_length:    " << m_ImageLength[i];
      os << endl;
   }

   if ( m_Tile.valid() )
   {
      os << "\nOutput tile dump:\n" << *m_Tile << endl;
   }

   os << endl;
   
   return ossimSource::print( os );
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfInputBands() const
{
   return m_SamplesPerPixel;
}

ossim_uint32 ossimVirtualImageHandler::getNumberOfOutputBands () const
{
   return getNumberOfInputBands();
}

bool ossimVirtualImageHandler::isOpen() const
{
   return m_OpenedFlag;
}

bool ossimVirtualImageHandler::hasR0() const
{
   return m_R0isFullRes;
}

bool ossimVirtualImageHandler::initializeBuffers()
{
   if( m_Buffer )
   {
      delete [] m_Buffer;
      m_Buffer = 0;
   }
   if( m_NullBuffer )
   {
      delete [] m_NullBuffer;
      m_NullBuffer = 0;
   }

   ossimImageDataFactory* idf = ossimImageDataFactory::instance();

   m_Tile = idf->create( this, this );

   // The tile size should by default be twice the frame size in width 
   // and height for the common case where the virtual reader is driven by 
   // the virtual overview builder. The builder grabs from the 
   // next layer down and decimates it down by 2 into the frame size.
   m_CurrentTileWidth  = m_ImageFrameWidth  * 2;
   m_CurrentTileHeight = m_ImageFrameLength * 2;

   // The width and height must be set prior to call to allocateBuffer.
   m_Tile->setWidth (m_CurrentTileWidth);
   m_Tile->setHeight(m_CurrentTileHeight);

   // Tiles are constructed with no buffer storage.  Call initialize for
   // "m_Tile" to allocate memory.  Leave "m_BlankTile" with a
   // ossimDataObjectStatus of OSSIM_NULL since no data will ever be
   // stuffed in it.
   m_Tile->initialize();

   return allocateBuffer();
}


void ossimVirtualImageHandler::setProperty( ossimRefPtr<ossimProperty> property )
{
   if( !property.valid() )
   {
      return;
   }

   ossimImageHandler::setProperty( property );
}

ossimRefPtr<ossimProperty> ossimVirtualImageHandler::getProperty( const ossimString& name )const
{
   return ossimImageHandler::getProperty( name );
}

void ossimVirtualImageHandler::getPropertyNames( std::vector<ossimString>& propertyNames )const
{
   ossimImageHandler::getPropertyNames( propertyNames );
	propertyNames.push_back( "file_type" );
}

ossimImageHandler* ossimVirtualImageHandler::openFrame( int resLevel, int row, int col )
{
   static const char* MODULE = "ossimVirtualImageHandler::openFrame";

   // Check for empty file name.
   if (theImageFile.empty())
   {
      return 0;
   }

   if ( resLevel==m_CurrentResLevel && 
        row==m_CurrentRow && 
        col==m_CurrentCol )
   {
      return m_CurrentImageHandler;
   }

   ossimString driveString;
   ossimString pathString;
   ossimString fileString;
   ossimString extString;
   theImageFile.split( driveString, pathString, fileString, extString );

   // If the virtual image header filename is a.ovr, the current frame
   // name is e.g. ./a_cache/res0/row0/col0.tif

   ossimFilename pathFName( pathString );
   ossimFilename subdirFName1( "." );
   ossimFilename subdirFName2( subdirFName1.dirCat(m_ImageSubdirectory) );
   ossimFilename subdirFName3( subdirFName2.dirCat("res") );
   subdirFName3.append( ossimString::toString( resLevel ) );
   ossimFilename subdirFName4( subdirFName3.dirCat("row") );
   subdirFName4.append( ossimString::toString( row ) );
   ossimString newPathString( pathFName.dirCat( subdirFName4 ) );

   ossimFilename driveFName( driveString );
   ossimFilename newPathFName( newPathString );
   ossimFilename newDirFName( driveFName.dirCat( newPathFName ) );

   ossimString newFileString( "col" );
   newFileString.append( ossimString::toString(col) );

   ossimImageHandler* pHandler = 0;
   bool bHaveExt = (m_CurrentFrameExt.empty() == false);
   if ( bHaveExt )
   {
      // Try the extension from the previously found frame file.
      ossimFilename currentFrameName;
      currentFrameName.merge( driveString, newPathString, newFileString, m_CurrentFrameExt );

      if ( currentFrameName.exists() )
      {
         FrameHandlerInfo* pInfo = resolveFrameHandler( currentFrameName );
         if ( pInfo != 0 )
         {
            pHandler = pInfo->pHandler;
         }
      }
   }

   // If using the extension from the previously found frame file didn't work,
   // let's search for the file with the extension as a wildcard.
   if ( pHandler == 0 )
   {
      m_CurrentFrameExt.clear();

      ossimString newExtString( "*" );
      ossimFilename currentFrameName;
      currentFrameName.merge( driveString, newPathString, newFileString, newExtString );

      // Find the first image file that is consistent with the currentFrameName.
      ossimString frameName( currentFrameName.file() );
      ossimFilename framePath( currentFrameName.path() );

      if ( framePath.exists() && framePath.isDir() && (frameName.empty() == false) )
      {
         ossimDirectory currentDir( framePath );
         std::vector<ossimFilename> result;
         currentDir.findAllFilesThatMatch( result, 
                                           frameName, 
                                           ossimDirectory::OSSIM_DIR_FILES );

         ossim_uint32 nCandidates = (ossim_uint32)result.size();
         if( nCandidates > 0 )
         {
            ossim_uint32 idx;
            for( idx = 0; idx < nCandidates; ++idx )
            {
               ossimFilename filenameCandidate = result[idx];

               // Since findAllFilesThatMatch() sometimes gives false
               // positives, let's do an additional confirmation that
               // the filenames w/o the extension match:
               ossimString driveStringCandidate;
               ossimString pathStringCandidate;
               ossimString fileStringCandidate;
               ossimString extStringCandidate;
               filenameCandidate.split( driveStringCandidate, 
                                        pathStringCandidate, 
                                        fileStringCandidate, 
                                        extStringCandidate );

               if ( newFileString == fileStringCandidate )
               {
                  // Open the frame image using an ossim image handler.
                  FrameHandlerInfo* pInfo = resolveFrameHandler( filenameCandidate );
                  if ( pInfo != 0 )
                  {
                     pHandler = pInfo->pHandler;
                     m_CurrentFrameExt = pInfo->ext;
                     break;
                  }
               }
            }
         }
      }

      if ( traceDebug() && pHandler==0 )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE 
            << "\nFile not found: " 
            << currentFrameName.c_str()
            << std::endl;
      }
   }

   m_CurrentImageHandler = pHandler;
   m_CurrentResLevel = resLevel;
   m_CurrentRow = row;
   m_CurrentCol = col;

   return pHandler;
}

FrameHandlerInfo* ossimVirtualImageHandler::resolveFrameHandler( const ossimFilename& f )
{
   static const char* MODULE = "ossimVirtualImageHandler::resolveFrameHandler";

   bool bFoundHandler = false;
   FrameHandlerInfo* pInfo = 0;

   // See if we already have an image handler to use.
   std::vector<FrameHandlerInfo*>::iterator infoIt = m_FrameHandlerInfoList.begin();
   ossim_uint32 nSavedHandlers = (ossim_uint32)m_FrameHandlerInfoList.size();
   while ( infoIt != m_FrameHandlerInfoList.end() )
   {
      pInfo = *infoIt;
      if ( pInfo != 0 )
      {
         ossimImageHandler* pHandler = pInfo->pHandler;
         if ( pHandler != 0 )
         {
            if ( f == pHandler->getFilename() && pHandler->isOpen() )
            {
               bFoundHandler = true;
            }
            else
            {
               bFoundHandler = pHandler->open( f );
            }

            if ( bFoundHandler )
            {
               break;
            }
            else
            if ( f.ext() == pInfo->ext )
            {
               std::vector<FrameHandlerInfo*>::iterator saveIt = infoIt;

               pHandler->close();
               delete pHandler;

               // Remove a failed FrameHandlerInfo from the list.
               infoIt = m_FrameHandlerInfoList.erase( saveIt );

               // Make sure we don't return a bad FrameHandlerInfo.
               pInfo = 0;

               --nSavedHandlers;
            }
            else
            {
               ++infoIt;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE 
               << "\nERROR: Image handler not found for extension: " 
               << pInfo->ext.c_str()
               << "and for requested file: "
               << f.c_str()
               << std::endl;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE 
            << "\nERROR: Empty iterator found for extension: " 
            << pInfo->ext.c_str()
            << "and for requested file: "
            << f.c_str()
            << std::endl;
      }
   }

   if ( bFoundHandler == false )
   {
      // Throw all the ossim image handlers at the file.
      ossimImageHandler* pHandler = 
         ossimImageHandlerRegistry::instance()->open( f );

      if ( pHandler != 0 && pHandler->isOpen() )
      {
         bFoundHandler = true;

         pInfo = new FrameHandlerInfo();
         if ( pInfo != 0 )
         {
            pInfo->pHandler = pHandler;
            pInfo->ext = f.ext();
            m_FrameHandlerInfoList.push_back( pInfo );
         }
      }
   }

   return pInfo;
}
