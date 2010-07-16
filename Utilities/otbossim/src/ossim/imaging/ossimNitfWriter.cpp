//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <tiffio.h>
#include <ossim/imaging/ossimNitfWriter.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <ossim/support_data/ossimNitfLocalCartographicTag.h>
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <ossim/support_data/ossimNitfNameConversionTables.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>

RTTI_DEF1(ossimNitfWriter, "ossimNitfWriter", ossimNitfWriterBase);

static ossimTrace traceDebug(ossimString("ossimNitfWriter:debug"));
                             
ossimNitfWriter::ossimNitfWriter(const ossimFilename& filename,
                                 ossimImageSource* inputSource)
   : ossimNitfWriterBase(filename, inputSource),
     theOutputStream(0),
     theFileHeader(),
     theImageHeader()
{
   //---
   // Since the internal nitf tags are not very accurate, write an external
   // geometry out as default behavior.  Users can disable this via the
   // property interface or keyword list.
   //---
   setWriteExternalGeometryFlag(true);
   
   theFileHeader      = new ossimNitfFileHeaderV2_1;
   theImageHeader     = new ossimNitfImageHeaderV2_1;
   theOutputImageType = "nitf_block_band_separate";
}

ossimNitfWriter::~ossimNitfWriter()
{
   //---
   // This looks like a leak but it's not as both of these are ossimRefPtr's.
   //---
   theFileHeader=NULL;
   theImageHeader=NULL;
   
   close();
}

bool ossimNitfWriter::isOpen()const
{
   return (theOutputStream != NULL);
}

bool ossimNitfWriter::open()
{
   if(isOpen())
   {
      close();
   }
   theOutputStream = new std::ofstream;
   theOutputStream->open(theFilename.c_str(), ios::out|ios::binary);
   
   return theOutputStream->good();
}

void ossimNitfWriter::close()
{
   if(theOutputStream)
   {
      theOutputStream->close();
      delete theOutputStream;
      theOutputStream = (std::ofstream*)NULL;
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
   writeGeometry(theImageHeader.get(), theInputConnection.get());

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
         theFileHeader->setProperties(propertyList);
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
         theImageHeader->setProperties(propertyList);
      }
   }
   else
   {
      ossimNitfWriterBase::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfWriter::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = NULL;
   
   if(name == "file_header")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimRefPtr<ossimProperty> > propertyList;
      
      theFileHeader->getPropertyList(propertyList);
      container->addChildren(propertyList);
      
      result = container;
   }
   else if(name == "image_header")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimRefPtr<ossimProperty> > propertyList;
      
      theImageHeader->getPropertyList(propertyList);
      container->addChildren(propertyList);
      
      result = container;
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
}

bool ossimNitfWriter::writeBlockBandSeparate()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint64 byteSize      = ossim::scalarSizeInBytes(scalarType);
   ossimIrect rect            = theInputConnection->getBoundingRect();
   ossim_uint64 bands         = theInputConnection->getNumberOfOutputBands();
   ossim_uint64 idx           = 0;
   ossim_uint64 headerStart   = (ossim_uint64)theOutputStream->tellp();

   ossimIpt blockSize(64, 64);
   ossim_uint64 blocksHorizontal = (ossim_uint32)ceil(((double)rect.width()/(double)blockSize.x));
   ossim_uint64 blocksVertical   = (ossim_uint32)ceil(((double)rect.height()/(double)blockSize.y));

   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439); // ok if no tags
   imageInfoRecord.setImageLength(bands*byteSize*blocksVertical*blockSize.y*blocksHorizontal*blockSize.x);

   theFileHeader->setDate();
   theFileHeader->addImageInfoRecord(imageInfoRecord);

   //---
   // This makes space for the file header; it is written again at the end of
   // this method with updated values
   // need a better way to get the length.  This should be queried on the
   // header before writing
   //---
   theFileHeader->writeStream(*theOutputStream); 
   ossim_uint64 headerLength = ((ossim_uint64)theOutputStream->tellp() - headerStart) /* + 1 */;
   
   ossimString representation;
   theImageHeader->setActualBitsPerPixel(
      ossim::getActualBitsPerPixel(scalarType));
   theImageHeader->setBitsPerPixel(ossim::getBitsPerPixel(scalarType));
   theImageHeader->setPixelType(ossimNitfCommon::getNitfPixelType(scalarType));
   theImageHeader->setNumberOfBands(bands);
   theImageHeader->setImageMode('B');// blocked

   if((bands == 3)&&
      (scalarType == OSSIM_UCHAR))
   {
      theImageHeader->setRepresentation("RGB");
      theImageHeader->setCategory("VIS");
   }
   else if(bands == 1)
   {
      theImageHeader->setRepresentation("MONO");
      theImageHeader->setCategory("MS");
   }
   else
   {
      theImageHeader->setRepresentation("MULTI");
      theImageHeader->setCategory("MS");
   }
   
   theImageHeader->setBlocksPerRow(blocksHorizontal);
   theImageHeader->setBlocksPerCol(blocksVertical);
   theImageHeader->setNumberOfPixelsPerBlockRow(blockSize.x);
   theImageHeader->setNumberOfPixelsPerBlockCol(blockSize.y);
   theImageHeader->setNumberOfRows(rect.height());
   theImageHeader->setNumberOfCols(rect.width());

   ossimNitfImageBandV2_1 bandInfo;
   for(idx = 0; idx < bands; ++idx)
   {
      std::ostringstream out;
      
      out << std::setfill('0')
          << std::setw(2)
          << idx;
      
      bandInfo.setBandRepresentation(out.str().c_str());
      theImageHeader->setBandInfo(idx,
                              bandInfo);
   }

   ossim_uint64 imageHeaderStart = theOutputStream->tellp();
   theImageHeader->writeStream(*theOutputStream);
   ossim_uint64 imageHeaderEnd = theOutputStream->tellp();
   ossim_uint64 imageHeaderSize = imageHeaderEnd - imageHeaderStart;

   theInputConnection->setTileSize(blockSize);
   ossim_uint64 numberOfTiles   = theInputConnection->getNumberOfTiles();
   theInputConnection->setToStartOfSequence();
   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint64 tileNumber = 1;
   ossimEndian endian;

   
   while( data.valid() && !needsAborting())
   {
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
      
      theOutputStream->write((char*)(data->getBuf()),
                             data->getSizeInBytes());
      
      setPercentComplete(((double)tileNumber / (double)numberOfTiles) * 100);
      
      if(!needsAborting())
      {
         data = theInputConnection->getNextTile();
      }
      ++tileNumber;
   }

   std::streamoff pos = theOutputStream->tellp();

   setComplexityLevel(pos, theFileHeader.get());

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   theFileHeader->setFileLength(static_cast<ossim_uint64>(pos));
   theFileHeader->setHeaderLength(headerLength);
   theOutputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   theFileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream);
   
   return true;
}

bool ossimNitfWriter::writeBlockBandSequential()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint64    byteSize   = ossim::scalarSizeInBytes(scalarType);
   ossimIrect      rect       = theInputConnection->getBoundingRect();
   ossim_uint64    bands      = theInputConnection->getNumberOfOutputBands();
   ossim_uint64    idx        = 0;
   ossim_uint64    headerStart   = (ossim_uint64)theOutputStream->tellp();

   ossimIpt blockSize(64, 64);

   ossim_uint64 blocksHorizontal = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 blocksVertical   = theInputConnection->getNumberOfTilesVertical();
   theInputConnection->setTileSize(blockSize);
   ossim_uint64 numberOfTiles   = theInputConnection->getNumberOfTiles();
   theInputConnection->setToStartOfSequence();
   
   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439);
   imageInfoRecord.setImageLength(bands*byteSize*blocksHorizontal*blocksVertical*blockSize.x*blockSize.y);

   theFileHeader->setDate();
   theFileHeader->addImageInfoRecord(imageInfoRecord);

   //---
   // This makes space for the file header; it is written again at the end of
   // this method with updated values
   // need a better way to get the length.  This should be queried on the
   // header before writing
   //---  
   theFileHeader->writeStream(*theOutputStream);
   ossim_uint64 headerLength = ((ossim_uint64)theOutputStream->tellp() - headerStart) /* + 1 */;
   
   ossimString representation;
   theImageHeader->setActualBitsPerPixel(
      ossim::getActualBitsPerPixel(scalarType));
   theImageHeader->setBitsPerPixel(ossim::getBitsPerPixel(scalarType));
   theImageHeader->setPixelType(ossimNitfCommon::getNitfPixelType(scalarType));
   theImageHeader->setNumberOfBands(bands);
   theImageHeader->setImageMode('S');// blocked
   
   if((bands == 3)&&
      (scalarType == OSSIM_UCHAR))
   {
      theImageHeader->setRepresentation("RGB");
      theImageHeader->setCategory("VIS");
   }
   else if(bands == 1)
   {
      theImageHeader->setRepresentation("MONO");
      theImageHeader->setCategory("MS");
   }
   else
   {
      theImageHeader->setRepresentation("MULTI");
      theImageHeader->setCategory("MS");
   }
   theImageHeader->setBlocksPerRow(blocksHorizontal);
   theImageHeader->setBlocksPerCol(blocksVertical);
   theImageHeader->setNumberOfPixelsPerBlockRow(blockSize.x);
   theImageHeader->setNumberOfPixelsPerBlockCol(blockSize.y);
   theImageHeader->setNumberOfRows(rect.height());
   theImageHeader->setNumberOfCols(rect.width());

   ossimNitfImageBandV2_1 bandInfo;
   for(idx = 0; idx < bands; ++idx)
   {
      std::ostringstream out;
      
      out << std::setfill('0')
          << std::setw(2)
          << idx;
      
      bandInfo.setBandRepresentation(out.str().c_str());
      theImageHeader->setBandInfo(idx,
                              bandInfo);
   }

   int imageHeaderStart = theOutputStream->tellp();
   theImageHeader->writeStream(*theOutputStream);
   int imageHeaderEnd = theOutputStream->tellp();
   int imageHeaderSize = imageHeaderEnd - imageHeaderStart;

   ossimIpt ul = rect.ul();
   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint64 tileNumber = 0;
   ossimEndian endian;

   // get the start to the first band of data block
   //
   ossim_uint64 streamOffset = theOutputStream->tellp();
   
   // holds the total pixels to the next band

   ossim_uint64 blockSizeInBytes = blockSize.x*blockSize.y*ossim::scalarSizeInBytes(data->getScalarType());
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
         theOutputStream->seekp(streamOffset+ // start of image stream
                                tileNumber*blockSizeInBytes + // start of block for band separate output
                                bandOffsetInBytes*idx, // which band offset is it
                                ios::beg); 
         
         theOutputStream->write((char*)(data->getBuf(idx)),
                                blockSizeInBytes);
      }
      ++tileNumber;
      
      setPercentComplete(((double)tileNumber / (double)numberOfTiles) * 100);

      if(!needsAborting())
      {
         data = theInputConnection->getNextTile();
      }
   }
   
   std::streamoff pos = theOutputStream->tellp();

   setComplexityLevel(pos, theFileHeader.get());

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   theFileHeader->setFileLength(static_cast<ossim_uint64>(pos));
   theFileHeader->setHeaderLength(headerLength);
   theOutputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   theFileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream);
   
   return true;
}


void ossimNitfWriter::addRegisteredTag(
   ossimRefPtr<ossimNitfRegisteredTag> registeredTag)
{
   ossimNitfTagInformation tagInfo;
   tagInfo.setTagData(registeredTag.get());
     theImageHeader->addTag(tagInfo);
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
         theImageHeader->addTag(tagInfo);
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
