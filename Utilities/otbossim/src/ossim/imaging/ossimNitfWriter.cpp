//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.cpp 22420 2013-09-26 16:42:56Z gpotts $

#include <ossim/imaging/ossimNitfWriter.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <ossim/support_data/ossimNitfLocalCartographicTag.h>
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <ossim/support_data/ossimNitfNameConversionTables.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfImageDataMaskV2_1.h>
#include <tiffio.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

RTTI_DEF1(ossimNitfWriter, "ossimNitfWriter", ossimNitfWriterBase);

static ossimTrace traceDebug(ossimString("ossimNitfWriter:debug"));

// Maximum file size
static const ossim_uint64 KB = 1024;
static const ossim_uint64 MB = KB * KB;
static const ossim_uint64 MB50 = 50 * MB;
static const ossim_uint64 GB = KB * MB;
static const ossim_uint64 GB2 = 2 * GB;
static const ossim_uint64 GB10 = 10 * GB;
                            
ossimNitfWriter::ossimNitfWriter(const ossimFilename& filename,
                                 ossimImageSource* inputSource)
   : ossimNitfWriterBase(filename, inputSource),
     m_outputStream(0),
     m_fileHeader(0),
     m_imageHeader(0),
     m_textHeader(0),
     m_textEntry(),
     m_blockSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT)
{
   //---
   // Since the internal nitf tags are not very accurate, write an external
   // geometry out as default behavior.  Users can disable this via the
   // property interface or keyword list.
   //---
   setWriteExternalGeometryFlag(true);
   
   m_fileHeader       = new ossimNitfFileHeaderV2_1;
   m_imageHeader      = new ossimNitfImageHeaderV2_1;

   // m_textHeader is conditional so we will not new here.
   
   theOutputImageType = "nitf_block_band_separate";

   //---
   // The tile size can be set in the preferences via "tile_size" keyword.
   // This will get it if set; else, set to default.
   //---
   ossim::defaultTileSize(m_blockSize);

   // These are always set:
   m_fileHeader->setEncryption( ossimString("0") );
   m_imageHeader->setJustification( ossimString("R") );

   // Set any site defaults.
   initializeDefaultsFromConfigFile(
      dynamic_cast<ossimNitfFileHeaderV2_X*>(m_fileHeader.get()),
      dynamic_cast<ossimNitfImageHeaderV2_X*>(m_imageHeader.get()) );
}

ossimNitfWriter::~ossimNitfWriter()
{
   //---
   // This looks like a leak but it's not as both of these are ossimRefPtr's.
   //---
   m_fileHeader=0;
   m_imageHeader=0;
   m_textHeader=0;

   close();
}

bool ossimNitfWriter::isOpen()const
{
   return m_outputStream;
}

bool ossimNitfWriter::open()
{
   if(isOpen())
   {
      close();
   }
   m_outputStream = new ossimOFStream64(theFilename.c_str(), 
                                            ios::out|ios::binary);
   //new std::ofstream;
   //m_outputStream->open(theFilename.c_str(), ios::out|ios::binary);
   
   return m_outputStream->good();
}

void ossimNitfWriter::close()
{
   if(m_outputStream)
   {
      m_outputStream->close();
      delete m_outputStream;
      m_outputStream = 0;
   }
}

bool ossimNitfWriter::writeFile()
{
   if(!theInputConnection->isMaster())
   {
      theInputConnection->slaveProcessTiles();

      return true;
   }

   open();

   if (!isOpen())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfWriter::writeFile ERROR:"
            << " Could not open!  Returning..."
            << std::endl;
      }

      return false;
   }
   
   // Write out the geometry info.
   writeGeometry(m_imageHeader.get(), theInputConnection.get());

   // addStandardTags();
   
   bool result = false;
   if((theOutputImageType == "nitf_block_band_separate")||
      (theOutputImageType == "image/nitf"))
   {
      result =  writeBlockBandSeparate();
   }
   else if(theOutputImageType == "nitf_block_band_sequential")
   {
      result =  writeBlockBandSequential();
   }
   
   close();
   
   return result;
}

void ossimNitfWriter::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   imageTypeList.push_back(ossimString("nitf_block_band_separate"));
   imageTypeList.push_back(ossimString("nitf_block_band_sequential"));
}

void ossimNitfWriter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   
   ossimString name = property->getName();
   
   if(name == "file_header")
   {
      ossimContainerProperty* containerProperty = PTR_CAST(ossimContainerProperty,
                                                           property.get());
      if(containerProperty)
      {
         std::vector<ossimRefPtr<ossimProperty> > propertyList;
         containerProperty->getPropertyList(propertyList);
         m_fileHeader->setProperties(propertyList);
      }
   }
   else if(name == "image_header")
   {
      ossimContainerProperty* containerProperty = PTR_CAST(ossimContainerProperty,
                                                           property.get());
      if(containerProperty)
      {
         std::vector<ossimRefPtr<ossimProperty> > propertyList;
         containerProperty->getPropertyList(propertyList);
         m_imageHeader->setProperties(propertyList);
      }
   }
   else if(name == "block_size")
   {
      ossimIpt blockSize;
      blockSize.x = property->valueToString().toInt32();
      blockSize.y = blockSize.x;
      setTileSize(blockSize);
   }
   else
   {
      ossimNitfWriterBase::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfWriter::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;
   
   if(name == "file_header")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimRefPtr<ossimProperty> > propertyList;
      
      m_fileHeader->getPropertyList(propertyList);
      container->addChildren(propertyList);
      
      result = container;
   }
   else if(name == "image_header")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimRefPtr<ossimProperty> > propertyList;
      
      m_imageHeader->getPropertyList(propertyList);
      container->addChildren(propertyList);
      
      result = container;
   }
   else if(name == "des_header")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimRefPtr<ossimProperty> > propertyList;

      // Create a temporary DES in order to populate propertyList.
      ossimNitfDataExtensionSegmentV2_1 des;
      des.getPropertyList(propertyList);
      container->addChildren(propertyList);

      result = container;
   }
   else if(name == "block_size")
   {
      ossimStringProperty* stringProp =
         new ossimStringProperty(name,
                                 ossimString::toString(m_blockSize.x),
                                 false); // editable flag
      stringProp->addConstraint(ossimString("64"));
      stringProp->addConstraint(ossimString("128"));
      stringProp->addConstraint(ossimString("256"));      
      stringProp->addConstraint(ossimString("512"));      
      stringProp->addConstraint(ossimString("1024"));      
      stringProp->addConstraint(ossimString("2048"));      
      return stringProp;
   }
   else
   {
      return ossimNitfWriterBase::getProperty(name);
   }

   return result;
}

void ossimNitfWriter::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   ossimNitfWriterBase::getPropertyNames(propertyNames);

   propertyNames.push_back("file_header");
   propertyNames.push_back("image_header");
   propertyNames.push_back("des_header");
   propertyNames.push_back("block_size");
}

bool ossimNitfWriter::writeBlockBandSeparate()
{
   ossimScalarType scalarType  = theInputConnection->getOutputScalarType();
   ossim_uint64    byteSize    = ossim::scalarSizeInBytes(scalarType);
   ossimIrect      rect        = theInputConnection->getBoundingRect();
   ossim_uint64    bands       = theInputConnection->getNumberOfOutputBands();
   ossim_uint64    idx         = 0;
   ossim_uint64    headerStart = (ossim_uint64)m_outputStream->tellp64();

   // Set the sequencer block size to be the same as output.
   theInputConnection->setTileSize(m_blockSize);
   
   ossim_uint64 blocksHorizontal = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 blocksVertical   = theInputConnection->getNumberOfTilesVertical();
   ossim_uint64 numberOfTiles    = theInputConnection->getNumberOfTiles();

   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439); // ok if no tags
   imageInfoRecord.setImageLength(bands*byteSize*blocksVertical*m_blockSize.y*blocksHorizontal*m_blockSize.x);

   m_fileHeader->setDate();
   m_fileHeader->addImageInfoRecord(imageInfoRecord);

   if ( m_textHeader.valid() )
   {
      // Add any text headers
      ossimNitfTextFileInfoRecordV2_1 textInfoRecord;
      textInfoRecord.setSubheaderLength(285); //default
      // Set length of text to be that of input text
      textInfoRecord.setTextLength(m_textEntry.length()); 
      m_fileHeader->addTextInfoRecord(textInfoRecord);
   }  

   // Get the overflow tags from the file header and the image subheader
   takeOverflowTags(true, true);
   takeOverflowTags(true, false);
   takeOverflowTags(false, true);
   takeOverflowTags(false, false);

   for (vector<ossimNitfDataExtensionSegmentV2_1>::iterator iter = m_dataExtensionSegments.begin();
      iter != m_dataExtensionSegments.end(); iter++)
   {
      ossimNitfDataExtSegInfoRecordV2_1 desInfoRecord;
      iter->setSecurityMarkings(*m_fileHeader);
      std::ostringstream headerOut;
      headerOut << std::setw(4)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << iter->getHeaderLength();
      strcpy(desInfoRecord.theDataExtSegSubheaderLength, headerOut.str().c_str());

      std::ostringstream dataOut;
      dataOut << std::setw(9)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << iter->getDataLength();
      strcpy(desInfoRecord.theDataExtSegLength, dataOut.str().c_str());

      m_fileHeader->addDataExtSegInfoRecord(desInfoRecord);
   }

   //---
   // This makes space for the file header; it is written again at the end of
   // this method with updated values
   // need a better way to get the length.  This should be queried on the
   // header before writing
   //---
   m_fileHeader->writeStream(*m_outputStream); 
   ossim_uint64 headerLength = ((ossim_uint64)m_outputStream->tellp64() - headerStart) /* + 1 */;
   
   ossimString representation;
   m_imageHeader->setActualBitsPerPixel(ossim::getActualBitsPerPixel(scalarType));
   m_imageHeader->setBitsPerPixel(ossim::getBitsPerPixel(scalarType));
   m_imageHeader->setPixelType(ossimNitfCommon::getNitfPixelType(scalarType));
   m_imageHeader->setNumberOfBands(bands);
   m_imageHeader->setImageMode('B');// blocked

   bool masked = (m_imageHeader->getCompressionCode() == "NM");
   ossimNitfImageDataMaskV2_1 datamask;
   datamask.setBlockCount(blocksVertical * blocksHorizontal);
   ossim_uint64 blockLength = bands * byteSize * m_blockSize.x * m_blockSize.y;
   datamask.setBlockLengthInBytes(blockLength);
   std::vector<char> blockZeros;


   if((bands == 3)&&
      (scalarType == OSSIM_UCHAR))
   {
      m_imageHeader->setRepresentation("RGB");
      m_imageHeader->setCategory("VIS");
   }
   else if(bands == 1)
   {
      m_imageHeader->setRepresentation("MONO");
      m_imageHeader->setCategory("MS");
   }
   else
   {
      m_imageHeader->setRepresentation("MULTI");
      m_imageHeader->setCategory("MS");
   }
   
   m_imageHeader->setBlocksPerRow(blocksHorizontal);
   m_imageHeader->setBlocksPerCol(blocksVertical);
   m_imageHeader->setNumberOfPixelsPerBlockRow(m_blockSize.x);
   m_imageHeader->setNumberOfPixelsPerBlockCol(m_blockSize.y);
   m_imageHeader->setNumberOfRows(rect.height());
   m_imageHeader->setNumberOfCols(rect.width());

   ossimNitfImageBandV2_1 bandInfo;
   for(idx = 0; idx < bands; ++idx)
   {
      std::ostringstream out;
      
      out << std::setfill('0')
          << std::setw(2)
          << idx;
      
      bandInfo.setBandRepresentation(out.str().c_str());
      m_imageHeader->setBandInfo(idx, bandInfo);
   }

   ossim_uint64 imageHeaderStart = m_outputStream->tellp64();
   m_imageHeader->writeStream(*m_outputStream);
   ossim_uint64 imageHeaderEnd = m_outputStream->tellp64();
   ossim_uint64 imageHeaderSize = imageHeaderEnd - imageHeaderStart;

   // Start the sequence through tiles:
   theInputConnection->setToStartOfSequence();
   
   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint64 tileNumber = 1;
   ossimEndian endian;
   
   // write out mask if needed
   if(masked)
   {
      blockZeros.resize(blockLength);
      memset(&blockZeros.front(), '\0', blockLength);
      datamask.writeStream(*m_outputStream);
   }
   while( data.valid() && !needsAborting())
   {
      bool write = true;
      if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
      {
         switch(data->getScalarType())
         {
            case OSSIM_USHORT16:
            case OSSIM_USHORT11:
            {
               endian.swap((ossim_uint16*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_SSHORT16:
            {
               endian.swap((ossim_sint16*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_FLOAT:
            case OSSIM_NORMALIZED_FLOAT:
            {
               endian.swap((ossim_float32*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_DOUBLE:
            case OSSIM_NORMALIZED_DOUBLE:
            {
               endian.swap((ossim_float64*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            default:
               break;
         }
      }
      
      if (masked)
      {
         if (memcmp(data->getBuf(), &blockZeros.front(), blockLength) == 0)
         {
            write = false;
            datamask.setIncludeBlock(tileNumber-1, false);
         }
      }
      if(write)
      {
         m_outputStream->write((char*)(data->getBuf()), data->getSizeInBytes());         
      }
      
      setPercentComplete(((double)tileNumber / (double)numberOfTiles) * 100);
      
      if(!needsAborting())
      {
         data = theInputConnection->getNextTile();
      }
      ++tileNumber;
   }
   ossim_uint64 imageSegmentEnd = m_outputStream->tellp64();

   // Let's write our text header
   if ( m_textHeader.valid() )
   {
      m_textHeader->writeStream(*m_outputStream); 
      //Now write the text
      m_outputStream->write((char*)(m_textEntry.c_str()), m_textEntry.length());
   }   


   for (vector<ossimNitfDataExtensionSegmentV2_1>::iterator iter = m_dataExtensionSegments.begin();
      iter != m_dataExtensionSegments.end(); iter++)
   {
      iter->writeStream(*m_outputStream);
   }

   if (masked)
   {
      m_outputStream->seekp(imageHeaderEnd);
      datamask.writeStream(*m_outputStream);
      //delete [] blockZeros;
   }

   std::streamoff pos = m_outputStream->tellp64();

   setComplexityLevel(pos, m_fileHeader.get());

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   m_fileHeader->setFileLength(static_cast<ossim_uint64>(pos));
   m_fileHeader->setHeaderLength(headerLength);
   m_outputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   m_fileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   m_fileHeader->writeStream(*m_outputStream);
   
   return true;
}

bool ossimNitfWriter::writeBlockBandSequential()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint64    byteSize   = ossim::scalarSizeInBytes(scalarType);
   ossimIrect      rect       = theInputConnection->getBoundingRect();
   ossim_uint64    bands      = theInputConnection->getNumberOfOutputBands();
   ossim_uint64    idx        = 0;
   ossim_uint64    headerStart   = (ossim_uint64)m_outputStream->tellp64();

   // Set the sequencer block size to be the same as output.
   theInputConnection->setTileSize(m_blockSize);
   
   ossim_uint64 blocksHorizontal = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 blocksVertical   = theInputConnection->getNumberOfTilesVertical();
   ossim_uint64 numberOfTiles    = theInputConnection->getNumberOfTiles();

   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439);
   imageInfoRecord.setImageLength(bands*byteSize*blocksHorizontal*blocksVertical*m_blockSize.x*m_blockSize.y);

   m_fileHeader->setDate();
   m_fileHeader->addImageInfoRecord(imageInfoRecord);

   if ( m_textHeader.valid() )
   {
      // Add any text headers
      ossimNitfTextFileInfoRecordV2_1 textInfoRecord;
      textInfoRecord.setSubheaderLength(285); //default
      // Set length of text to be that of input text
      textInfoRecord.setTextLength(m_textEntry.length()); 
      m_fileHeader->addTextInfoRecord(textInfoRecord);
   }

   //---
   // This makes space for the file header; it is written again at the end of
   // this method with updated values
   // need a better way to get the length.  This should be queried on the
   // header before writing
   //---  
   m_fileHeader->writeStream(*m_outputStream);
   ossim_uint64 headerLength = ((ossim_uint64)m_outputStream->tellp64() - headerStart) /* + 1 */;
   
   ossimString representation;
   m_imageHeader->setActualBitsPerPixel(ossim::getActualBitsPerPixel(scalarType));
   m_imageHeader->setBitsPerPixel(ossim::getBitsPerPixel(scalarType));
   m_imageHeader->setPixelType(ossimNitfCommon::getNitfPixelType(scalarType));
   m_imageHeader->setNumberOfBands(bands);
   m_imageHeader->setImageMode('S');// blocked
   
   if((bands == 3)&&
      (scalarType == OSSIM_UCHAR))
   {
      m_imageHeader->setRepresentation("RGB");
      m_imageHeader->setCategory("VIS");
   }
   else if(bands == 1)
   {
      m_imageHeader->setRepresentation("MONO");
      m_imageHeader->setCategory("MS");
   }
   else
   {
      m_imageHeader->setRepresentation("MULTI");
      m_imageHeader->setCategory("MS");
   }
   m_imageHeader->setBlocksPerRow(blocksHorizontal);
   m_imageHeader->setBlocksPerCol(blocksVertical);
   m_imageHeader->setNumberOfPixelsPerBlockRow(m_blockSize.x);
   m_imageHeader->setNumberOfPixelsPerBlockCol(m_blockSize.y);
   m_imageHeader->setNumberOfRows(rect.height());
   m_imageHeader->setNumberOfCols(rect.width());

   ossimNitfImageBandV2_1 bandInfo;
   for(idx = 0; idx < bands; ++idx)
   {
      std::ostringstream out;
      
      out << std::setfill('0')
          << std::setw(2)
          << idx;
      
      bandInfo.setBandRepresentation(out.str().c_str());
      m_imageHeader->setBandInfo(idx, bandInfo);
   }

   ossim_uint64 imageHeaderStart = m_outputStream->tellp64();
   m_imageHeader->writeStream(*m_outputStream);
   ossim_uint64 imageHeaderEnd = m_outputStream->tellp64();
   ossim_uint64 imageHeaderSize = imageHeaderEnd - imageHeaderStart;

   // ossimIpt ul = rect.ul();

   // Start the sequence through tiles:
   theInputConnection->setToStartOfSequence();

   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint64 tileNumber = 0;
   ossimEndian endian;

   // get the start to the first band of data block
   //
   ossim_uint64 streamOffset = m_outputStream->tellp64();
   
   // holds the total pixels to the next band

   ossim_uint64 blockSizeInBytes = m_blockSize.x*m_blockSize.y*ossim::scalarSizeInBytes(data->getScalarType());
   ossim_uint64 bandOffsetInBytes = (blockSizeInBytes*blocksHorizontal*blocksVertical);

   bool needSwapping = endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN;
   while(data.valid() && !needsAborting())
   {
      if(needSwapping)
      {
         switch(data->getScalarType())
         {
            case OSSIM_USHORT16:
            case OSSIM_USHORT11:
            {
               endian.swap((ossim_uint16*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_SSHORT16:
            {
               endian.swap((ossim_sint16*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_FLOAT:
            case OSSIM_NORMALIZED_FLOAT:
            {
               endian.swap((ossim_float32*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            case OSSIM_DOUBLE:
            case OSSIM_NORMALIZED_DOUBLE:
            {
               endian.swap((ossim_float64*)data->getBuf(),
                           data->getWidth()*data->getHeight()*data->getNumberOfBands());
               break;
            }
            default:
               break;
         }
      }

      for(idx = 0; idx < bands; ++idx)
      {
         m_outputStream->seekp(streamOffset+ // start of image stream
                               tileNumber*blockSizeInBytes + // start of block for band separate output
                               bandOffsetInBytes*idx, // which band offset is it
                               ios::beg); 
         
         m_outputStream->write((char*)(data->getBuf(idx)),
                               blockSizeInBytes);
      }
      ++tileNumber;
      
      setPercentComplete(((double)tileNumber / (double)numberOfTiles) * 100);
      
      if(!needsAborting())
      {
         data = theInputConnection->getNextTile();
      }
   }

   // Let's write our text header
   if ( m_textHeader.valid() )
   {
      m_textHeader->writeStream(*m_outputStream); 
      //Now write the text
      m_outputStream->write((char*)(m_textEntry.c_str()), m_textEntry.length());
   }   

   ossim_uint64 pos = m_outputStream->tellp64();

   setComplexityLevel(pos, m_fileHeader.get());

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   m_fileHeader->setFileLength(static_cast<ossim_uint64>(pos));
   m_fileHeader->setHeaderLength(headerLength);
   m_outputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   m_fileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   m_fileHeader->writeStream(*m_outputStream);
   
   return true;
}

void ossimNitfWriter::addRegisteredTag(ossimRefPtr<ossimNitfRegisteredTag> registeredTag,
   bool unique)
{
   addRegisteredTag(registeredTag, unique, 1, ossimString("IXSHD"));
}

void ossimNitfWriter::addRegisteredTag(ossimRefPtr<ossimNitfRegisteredTag> registeredTag,
   bool unique, const ossim_uint32& ownerIndex, const ossimString& tagType)
{
   ossimNitfTagInformation tagInfo;
   tagInfo.setTagData(registeredTag.get());
   tagInfo.setTagType(tagType);

   switch (ownerIndex)
   {
      case 0:
      {
         m_fileHeader->addTag(tagInfo, unique);
         break;
      }

      case 1:
      {
         m_imageHeader->addTag(tagInfo, unique);
         break;
      }

      default:
      {
         // Do nothing
      }
   }
}

bool ossimNitfWriter::addTextToNitf(std::string &inputText)
{
   // Initialize the m_textHeader
   m_textEntry = inputText;
   if ( m_textHeader.valid() == false )
   {
      // Only created if we need it.
      m_textHeader = new ossimNitfTextHeaderV2_1;
   }
   return true;
}
void ossimNitfWriter::getTileSize(ossimIpt& size) const
{
   size = m_blockSize;
}

void ossimNitfWriter::setTileSize(const ossimIpt& tileSize)
{
   const ossim_int32 MIN_BLOCK_SIZE = 64;
   if ( (tileSize.x % MIN_BLOCK_SIZE) || (tileSize.y % MIN_BLOCK_SIZE) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfWriter::setTileSize ERROR:"
            << "\nBlock size must be a multiple of " << MIN_BLOCK_SIZE
            << "\nSize remains:  " << m_blockSize
            << std::endl;
      }
   }
   else
   {
      m_blockSize = tileSize;
   }
}

#if 0
void ossimNitfWriter::addStandardTags()
{

   if(!theInputConnection)
   {
      return;
   }
   
   // commenting this out for now.  For some reason the pixels
   // are off when I add this tag.  I checked the parsing and it
   // appears to be the correct length???  So, I am not sure
   // why we are off when reading the output back in.
   //
   // first lets do the projection tag.  I REALLY need to add
   // parameter support soon or this tag is useless.
   //
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);
   ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   ossimNitfNameConversionTables table;
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj.get());
   ossimNitfTagInformation tagInfo;
   
   if(mapProj)
   {

      if(!PTR_CAST(ossimUtmProjection, mapProj))
      {
         ossimRefPtr<ossimNitfProjectionParameterTag> parameterTag = new ossimNitfProjectionParameterTag;
         
         ossimString nitfCode = table.convertMapProjectionNameToNitfCode(proj->getClassName());
         ossimString nitfName = table.convertNitfCodeToNitfProjectionName(nitfCode);
         
         parameterTag->setName(nitfName);
         parameterTag->setCode(nitfCode);
         parameterTag->setFalseX(mapProj->getFalseEasting());
         parameterTag->setFalseY(mapProj->getFalseNorthing());
         
         tagInfo.setTagData(parameterTag.get());
         m_imageHeader->addTag(tagInfo);
      }
   }
}
#endif

bool ossimNitfWriter::saveState(ossimKeywordlist& kwl,
                                const char* prefix) const
{
   return ossimNitfWriterBase::saveState(kwl, prefix);
}

bool ossimNitfWriter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   return ossimNitfWriterBase::loadState(kwl, prefix);
}

ossimNitfImageHeaderV2_1 *ossimNitfWriter::getImageHeader()
{
   return m_imageHeader.get();
}

ossimNitfFileHeaderV2_1 *ossimNitfWriter::getFileHeader()
{
   return m_fileHeader.get();
}

void ossimNitfWriter::addDataExtensionSegment(const ossimNitfDataExtensionSegmentV2_1& des, bool allowTreOverflow)
{
   if (allowTreOverflow == false)
   {
      ossimRefPtr<ossimProperty> pId = des.getProperty(ossimNitfDataExtensionSegmentV2_1::DESID_KW);
      if (pId == NULL || pId->valueToString() == "TRE_OVERFLOW" ||
         pId->valueToString() == "REGISTERED EXTENSIONS" || pId->valueToString() == "CONTROLLED EXTENSIONS")
      {
         return;
      }
   }

   m_dataExtensionSegments.push_back(des);
}
void ossimNitfWriter::takeOverflowTags(bool useFileHeader, bool userDefinedTags)
{
   ossimString itemIndex;
   std::vector<ossimNitfTagInformation> overflowTags;
   const ossim_uint32 potentialDesIndex = m_dataExtensionSegments.size() + 1;

   if (useFileHeader)
   {
      m_fileHeader->takeOverflowTags(overflowTags, potentialDesIndex, userDefinedTags);
      itemIndex = "0";
   }
   else
   {
      m_imageHeader->takeOverflowTags(overflowTags, potentialDesIndex, userDefinedTags);
      itemIndex = "1";
   }

   if (overflowTags.empty() == false)
   {
      ossimNitfDataExtensionSegmentV2_1 des;
      ossimRefPtr<ossimProperty> pDe =
         new ossimStringProperty(ossimNitfDataExtensionSegmentV2_1::DE_KW, "DE");
      des.setProperty(pDe);

      ossimRefPtr<ossimProperty> pId =
         new ossimStringProperty(ossimNitfDataExtensionSegmentV2_1::DESID_KW, "TRE_OVERFLOW");
      des.setProperty(pId);

      ossimRefPtr<ossimProperty> pVersion =
         new ossimStringProperty(ossimNitfDataExtensionSegmentV2_1::DESVER_KW, "1");
      des.setProperty(pVersion);

      ossimRefPtr<ossimProperty> pOverflow =
         new ossimStringProperty(ossimNitfDataExtensionSegmentV2_1::DESOFLW_KW, overflowTags[0].getTagType());
      des.setProperty(pOverflow);

      ossimRefPtr<ossimProperty> pItem =
         new ossimStringProperty(ossimNitfDataExtensionSegmentV2_1::DESITEM_KW, itemIndex);
      des.setProperty(pItem);

      des.setTagList(overflowTags);
      addDataExtensionSegment(des, true);
   }
}

