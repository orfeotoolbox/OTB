//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.cpp 11385 2007-07-25 13:56:38Z gpotts $

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <tiffio.h>
#include <ossim/imaging/ossimNitf20Writer.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <ossim/support_data/ossimNitfLocalCartographicTag.h>
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <ossim/support_data/ossimNitfNameConversionTables.h>
#include <ossim/support_data/ossimNitfBlockaTag.h>

RTTI_DEF1(ossimNitf20Writer, "ossimNitf20Writer", ossimImageFileWriter);

static ossimTrace traceDebug(ossimString("ossimNitfWriter:debug"));
     
static ossimString monthConversionTable[] = {"   ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

ossimNitf20Writer::ossimNitf20Writer(const ossimFilename& filename,
                                 ossimImageSource* inputSource)
   : ossimImageFileWriter(filename, inputSource, NULL),
     theOutputStream(NULL),
     theFileHeader(),
     theImageHeader(),
     theEnableRpcbTagFlag(false),
     theEnableBlockaTagFlag(true),
     theCopyFieldsFlag(false)
{
   //---
   // Since the internal nitf tags are not very accurate, write an external
   // geometry out as default behavior.  Users can disable this via the
   // property interface or keyword list.
   //---
   setWriteExternalGeometryFlag(true);
   
   theFileHeader      = new ossimNitfFileHeaderV2_0;
   theImageHeader     = new ossimNitfImageHeaderV2_0;
   theOutputImageType = "nitf20_block_band_separate";
}

ossimNitf20Writer::~ossimNitf20Writer()
{
   //---
   // This looks like a leak but it's not as both of these are ossimRefPtr's.
   //---
   theFileHeader=NULL;
   theImageHeader=NULL;
   
   close();
}

bool ossimNitf20Writer::isOpen()const
{
   return (theOutputStream != NULL);
}

bool ossimNitf20Writer::open()
{
   if(isOpen())
   {
      close();
   }
   theOutputStream = new std::ofstream;
   theOutputStream->open(theFilename.c_str(), ios::out|ios::binary);
   
   return theOutputStream->good();
}

void ossimNitf20Writer::close()
{
   if(theOutputStream)
   {
      theOutputStream->close();
      delete theOutputStream;
      theOutputStream = (std::ofstream*)NULL;
   }
}

bool ossimNitf20Writer::writeFile()
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
            << "ossimNitf20Writer::writeFile ERROR:"
            << " Could not open!  Returning..."
            << std::endl;
      }

      return false;
   }

   // Write out the geometry info.
   writeGeometry();
   addTags();
   
   bool result = false;
   if((theOutputImageType == "nitf20_block_band_separate")||
      (theOutputImageType == "image/nitf20"))
   {
      result =  writeBlockBandSeparate();
   }
   else if(theOutputImageType == "nitf20_block_band_sequential")
   {
      result =  writeBlockBandSequential();
   }
   
   close();
   
   return result;
}

void ossimNitf20Writer::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   imageTypeList.push_back(ossimString("nitf20_block_band_separate"));
   imageTypeList.push_back(ossimString("nitf20_block_band_sequential"));
}

void ossimNitf20Writer::setProperty(ossimRefPtr<ossimProperty> property)
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
   else if (name == "enable_rpcb_tag")
   {
      theEnableRpcbTagFlag = property->valueToString().toBool();
   }
   else if (name == "enable_blocka_tag")
   {
      theEnableBlockaTagFlag = property->valueToString().toBool();
   }
   else if(name == "copy_fields_flag")
   {
      theCopyFieldsFlag = property->valueToString().toBool();
   }
   else
   {
      // just in case it was an nitf specific tag we can pass it safely like this
      theFileHeader->setProperty(property);
      theImageHeader->setProperty(property);
      ossimImageFileWriter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitf20Writer::getProperty(const ossimString& name)const
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
   else if(name == "enable_rpcb_tag")
   {
      ossimBooleanProperty* boolProperty =
         new ossimBooleanProperty(name, theEnableRpcbTagFlag);

      result = boolProperty;
   }   
   else if(name == "enable_blocka_tag")
   {
      ossimBooleanProperty* boolProperty =
         new ossimBooleanProperty(name, theEnableBlockaTagFlag);

      result = boolProperty;
   }   
   else if(name == "copy_fields_flag")
   {
      ossimBooleanProperty* boolProperty =
      new ossimBooleanProperty(name, theCopyFieldsFlag);
      
      result = boolProperty;
   }
   else
   {
      return ossimImageFileWriter::getProperty(name);
   }

   return result;
}

void ossimNitf20Writer::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageFileWriter::getPropertyNames(propertyNames);

   propertyNames.push_back("file_header");
   propertyNames.push_back("image_header");
   propertyNames.push_back("enable_rpcb_tag");
   propertyNames.push_back("enable_blocka_tag");
   propertyNames.push_back("copy_fields_flag");
}

bool ossimNitf20Writer::writeBlockBandSeparate()
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

   ossimNitfImageInfoRecordV2_0 imageInfoRecord;
   //imageInfoRecord.setSubheaderLength(439); // ok if no tags
   imageInfoRecord.setImageLength(bands*byteSize*blocksVertical*blockSize.y*blocksHorizontal*blockSize.x);

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
   theImageHeader->setActualBitsPerPixel(getActualBitsPerPixel());
   theImageHeader->setBitsPerPixel(getBitsPerPixel());
   theImageHeader->setPixelType(getNitfPixelType());
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

   ossimNitfImageBandV2_0 bandInfo;
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

   ossim_uint64 pos = theOutputStream->tellp();

   setComplexityLevel(pos);

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   theFileHeader->setFileLength(pos);
   theFileHeader->setHeaderLength(headerLength);
   theOutputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   theFileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream);
   
   return true;
}

bool ossimNitf20Writer::writeBlockBandSequential()
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
   
   ossimNitfImageInfoRecordV2_0 imageInfoRecord;
 //  imageInfoRecord.setSubheaderLength(439);
   imageInfoRecord.setImageLength(bands*byteSize*blocksHorizontal*blocksVertical*blockSize.x*blockSize.y);

   ossimDate currentDate;
   
   theFileHeader->setDate(currentDate);
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
   theImageHeader->setActualBitsPerPixel(getActualBitsPerPixel());
   theImageHeader->setBitsPerPixel(getBitsPerPixel());
   theImageHeader->setPixelType(getNitfPixelType());
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

   ossimNitfImageBandV2_0 bandInfo;
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
   
   ossim_uint64 pos = theOutputStream->tellp();

   setComplexityLevel(pos);

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   theFileHeader->setFileLength(pos);
   theFileHeader->setHeaderLength(headerLength);
   theOutputStream->seekp(0, ios::beg);
   imageInfoRecord.setSubheaderLength(imageHeaderSize);
   theFileHeader->replaceImageInfoRecord(0, imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream);
   
   return true;
}


void ossimNitf20Writer::addRegisteredTag(
   ossimRefPtr<ossimNitfRegisteredTag> registeredTag)
{
   ossimNitfTagInformation tagInfo;
   tagInfo.setTagData(registeredTag.get());
     theImageHeader->addTag(tagInfo);
}

void ossimNitf20Writer::writeGeometry()
{
   if ( (theImageHeader.valid() == false) || !theInputConnection )
   {
      return;
   }
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);
   ossimRefPtr<ossimProjection> proj =
      ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   
   if(proj.valid() == false)
   {
      // No projection info.
      return;
   }

   // Get the requested bounding rectangles.
   ossimIrect rect = theInputConnection->getBoundingRect();

   // See if it's a map projection; else, a sensor model.
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj.get());
   if (mapProj)
   {
      // Use map info to get the corners.
      ossimMapProjectionInfo mapInfo(mapProj, rect);
      mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
      
      // See if it's utm.
      ossimUtmProjection* utmProj = PTR_CAST(ossimUtmProjection, proj.get());
      if(utmProj)
      {
         ossimDpt ul = mapInfo.ulEastingNorthingPt();
         ossimDpt ur = mapInfo.urEastingNorthingPt();
         ossimDpt lr = mapInfo.lrEastingNorthingPt();
         ossimDpt ll = mapInfo.llEastingNorthingPt();
         
         if(utmProj->getHemisphere() == 'N')
         {
            theImageHeader->setUtmNorth(utmProj->getZone(), ul, ur, lr, ll);
         }
         else
         {
            theImageHeader->setUtmSouth(utmProj->getZone(), ul, ur, lr, ll);
         }
      }
      else
      {
         ossimGpt ul = mapInfo.ulGroundPt();
         ossimGpt ur = mapInfo.urGroundPt();
         ossimGpt lr = mapInfo.lrGroundPt();
         ossimGpt ll = mapInfo.llGroundPt();
         theImageHeader->setGeographicLocationDms(ul, ur, lr, ll);
      }

      if (theEnableBlockaTagFlag)
      {
         addBlockaTag(mapInfo);
      }
   }

   if (theEnableRpcbTagFlag)
   {
      addRpcbTag(rect, proj);
   }
}

void ossimNitf20Writer::addTags()
{
   ossimDate currentDate;
   theFileHeader->setDate(currentDate);
   
   if(theCopyFieldsFlag)
   {
      ossimConnectableObject* obj = findObjectOfType("ossimImageHandler",
                                                     ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);
      ossimNitfTileSource* nitf = dynamic_cast<ossimNitfTileSource*>(obj);
      if(nitf)
      {
         ossimString value;
         ossimPropertyInterface* fileHeaderProperties = dynamic_cast<ossimPropertyInterface*>(theFileHeader.get());
         ossimPropertyInterface* imageHeaderProperties = dynamic_cast<ossimPropertyInterface*>(theImageHeader.get());
         bool nitf21Flag = false;
         const ossimNitfFileHeader*   header      = nitf->getFileHeader();
         const ossimNitfImageHeader*  imageHeader = nitf->getCurrentImageHeader();
         if(header)
         {
            ossimString version = header->getPropertyValueAsString("fhdr");
            nitf21Flag = version.contains("2.1");
            value = header->getPropertyValueAsString("stype");
            if(value.size())
            {
               fileHeaderProperties->setProperty("stype", value);
            }
            value = header->getPropertyValueAsString("ostaid");
            if(value.size())
            {
               fileHeaderProperties->setProperty("ostaid", value);
            }
            value = header->getPropertyValueAsString("ftitle");
            if(value.size())
            {
               fileHeaderProperties->setProperty("ftitle", value);
            }
            value = header->getPropertyValueAsString("fsclas");
            if(value.size())
            {
               fileHeaderProperties->setProperty("fsclas", value);
            }
            value = header->getPropertyValueAsString("oname");
            if(value.size())
            {
               fileHeaderProperties->setProperty("oname", value);
            }
            value = header->getPropertyValueAsString("ophone");
            if(value.size())
            {
               fileHeaderProperties->setProperty("ophone", value);
            }
            int idx = 0;
            for(idx = 0; idx < header->getNumberOfTags(); ++idx)
            {
               ossimNitfTagInformation info;
               header->getTagInformation(info,
                                         idx);
               theFileHeader->addTag(info);
            }
            value = header->getPropertyValueAsString("");
            if(value.size())
            {
               fileHeaderProperties->setProperty("", value);
            }
            value = header->getPropertyValueAsString("fdt");
            if(value.size()==14)
            {
               if(nitf21Flag)
               {
                  ossimString year(value.begin()+2, value.begin()+4);
                  ossimString mon(value.begin()+4, value.begin()+6);
                  ossimString day(value.begin()+6, value.begin()+8);
                  ossimString hour(value.begin()+8, value.begin()+10);
                  ossimString min(value.begin()+10, value.begin()+12);
                  ossimString sec(value.begin()+12, value.begin()+14);
                  if(mon.toUInt32() < 13)
                  {
                     mon = monthConversionTable[mon.toUInt32()];
                  }
                  fileHeaderProperties->setProperty("fdt", day+hour+min+sec+"Z"+mon+year);
               }
               else
               {
                  fileHeaderProperties->setProperty("fdt", value);
               }
            }
         }
         if(imageHeader)
         {
            ossim_uint32 idx = 0;
            for(idx = 0; idx < imageHeader->getNumberOfTags(); ++idx)
            {
               ossimNitfTagInformation info;
               imageHeader->getTagInformation(info,
                                              idx);
               theImageHeader->addTag(info);
            }
            value = imageHeader->getPropertyValueAsString("tgtid");
            if(value.size())
            {
               imageHeaderProperties->setProperty("tgtid", value);
            }
            value = imageHeader->getPropertyValueAsString("isclas");
            if(value.size())
            {
               imageHeaderProperties->setProperty("isclas", value);
            }
            value = imageHeader->getPropertyValueAsString("igeolo");
            if(value.size())
            {
               imageHeaderProperties->setProperty("igeolo", value);
            }
            value = imageHeader->getPropertyValueAsString("ititle");
            if(nitf21Flag)
            {
               value = imageHeader->getPropertyValueAsString("iid2");
            }
            if(value.size())
            {
               imageHeaderProperties->setProperty("ititle", value);
            }
            if(!nitf21Flag)
            {
               value = imageHeader->getPropertyValueAsString("iscaut");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("iscaut", value);
               }
               value = imageHeader->getPropertyValueAsString("iscode");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("iscode", value);
               }
               value = imageHeader->getPropertyValueAsString("isctlh");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("isctlh", value);
               }
               value = imageHeader->getPropertyValueAsString("isrel");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("isrel", value);
               }
               value = imageHeader->getPropertyValueAsString("isctln");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("isctln", value);
               }
               value = imageHeader->getPropertyValueAsString("isdwng");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("isdevt", value);
               }
               value = imageHeader->getPropertyValueAsString("isorce");
               if(value.size())
               {
                  imageHeaderProperties->setProperty("isorce", value);
               }
            }
            value = imageHeader->getPropertyValueAsString("idatim");
            if(value.size()==14)
            {
               if(nitf21Flag)
               {
                  ossimString year(value.begin()+2, value.begin()+4);
                  ossimString mon(value.begin()+4, value.begin()+6);
                  ossimString day(value.begin()+6, value.begin()+8);
                  ossimString hour(value.begin()+8, value.begin()+10);
                  ossimString min(value.begin()+10, value.begin()+12);
                  ossimString sec(value.begin()+12, value.begin()+14);
                  if(mon.toUInt32() < 13)
                  {
                     mon = monthConversionTable[mon.toUInt32()];
                  }
                  imageHeaderProperties->setProperty("idatim", day+hour+min+sec+"Z"+mon+year);
               }
               else
               {
                  imageHeaderProperties->setProperty("idatim", value);
               }
            }
         }
         
         // we will port over only a selected few 
      }
   }
}

void ossimNitf20Writer::setComplexityLevel(ossim_uint64 endPosition)
{
   //---
   // See MIL-STD-2500C, Table A-10:
   //
   // Lots of rules here, but for now we will key off of file size.
   //---

   if (!theFileHeader)
   {
      return;
   }

   const ossim_uint64 MB   = 1024 * 1024;
   const ossim_uint64 MB50 = 50   * MB;
   const ossim_uint64 GIG  = 1000 * MB;
   const ossim_uint64 GIG2 = 2    * GIG;
   
   ossimString complexity = "03"; // Less than 50 mb.
   

   if ( (endPosition >= MB50) && (endPosition < GIG) )
   {
      complexity = "05";
   }
   else if ( (endPosition >= GIG) && (endPosition < GIG2) )
   {
      complexity = "06";
   }
   else if (endPosition >= GIG2)
   {
      complexity = "07";
   }
   
   theFileHeader->setComplexityLevel(complexity);
}


void ossimNitf20Writer::addBlockaTag(ossimMapProjectionInfo& mapInfo)
{
   if (!theImageHeader)
   {
      return;
   }

   // Capture the current pixel type.
   ossimPixelType originalPixelType = mapInfo.getPixelType();

   // This tag wants corners as area:
   mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
   
   // Stuff the blocka tag which has six digit precision.
   ossimNitfBlockaTag* blockaTag = new ossimNitfBlockaTag();
   
   // Set the block number.
   blockaTag->setBlockInstance(1);

   // Set the number of lines.
   blockaTag->setLLines(mapInfo.linesPerImage());

   // Set first row, first column.
   blockaTag->setFrfcLoc(ossimDpt(mapInfo.ulGroundPt()));
   
   // Set first row, last column.
   blockaTag->setFrlcLoc(ossimDpt(mapInfo.urGroundPt()));
   
   // Set last row, last column.
   blockaTag->setLrlcLoc(ossimDpt(mapInfo.lrGroundPt()));
   
   // Set last row, first column.
   blockaTag->setLrfcLoc(ossimDpt(mapInfo.llGroundPt()));
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitf20Writer::writeGeometry DEBUG:"
         << "\nBLOCKA Tag:" << *((ossimObject*)(blockaTag))
         << std::endl;
   }
   
   // Add the tag to the header.
   ossimRefPtr<ossimNitfRegisteredTag> blockaTagRp = blockaTag;
   ossimNitfTagInformation blockaTagInfo(blockaTagRp);
   theImageHeader->addTag(blockaTagInfo);

   // Reset the pixel type to original value
   mapInfo.setPixelType(originalPixelType);
}

void ossimNitf20Writer::addRpcbTag(const ossimIrect& rect,
                                 ossimRefPtr<ossimProjection> proj)
{
   if (!proj.valid())
   {
      return;
   }

   bool useElevation = false;

   if (PTR_CAST(ossimMapProjection, proj.get()))
   {
      // If we're already map projected turn the elevation off.
      useElevation = false;
   }

   // Make an rpc solver.
   ossimRpcSolver rs(useElevation);
   
   // Compute the coefficients.
   rs.solveCoefficients(ossimDrect(rect),
                        *proj.get(),
                        64,
                        64);
   
   // Add the tag.
   ossimRefPtr<ossimNitfRegisteredTag> tag = rs.getNitfRpcBTag();
   ossimNitfTagInformation tagInfo(tag);
   theImageHeader->addTag(tagInfo);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitf20Writer::addRpcbTag DEBUG:"
         << "\nRPCB Tag:" << *((ossimObject*)(tag.get()))
         << "\nProjection:" << std::endl;
      
      proj->print(ossimNotify(ossimNotifyLevel_DEBUG));
      
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nRect: " << rect << std::endl;
   }
}

ossim_uint32 ossimNitf20Writer::getActualBitsPerPixel() const
{
   ossim_uint32 actualBitsPerPixel = 0;
   if (theInputConnection)
   {
      ossimScalarType scalarType = theInputConnection->getOutputScalarType();
      switch(scalarType)
      {
         case OSSIM_UCHAR:
         {
            actualBitsPerPixel = 8;
            break;
         }
         case OSSIM_USHORT11:
         {
            actualBitsPerPixel = 11;
            break;
         }
         case OSSIM_USHORT16:
         case OSSIM_SSHORT16:
         {
            actualBitsPerPixel = 16;
            break;
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            actualBitsPerPixel = 32;
            break;
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            actualBitsPerPixel = 64;
            break;
         }
         default:
         {
            break;
         }
      }
   }
   
   return actualBitsPerPixel;
}

ossim_uint32 ossimNitf20Writer::getBitsPerPixel() const
{
   ossim_uint32 bitsPerPixel = 0;

   if (theInputConnection)
   {
      ossimScalarType scalarType = theInputConnection->getOutputScalarType();
      switch(scalarType)
      {
         case OSSIM_UCHAR:
         {
            bitsPerPixel = 8;
            break;
         }
         case OSSIM_USHORT11:
         {
            bitsPerPixel = 16;
            break;
         }
         case OSSIM_USHORT16:
         case OSSIM_SSHORT16:
         {
            bitsPerPixel = 16;
            break;
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            bitsPerPixel = 32;
            break;
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            bitsPerPixel = 64;
            break;
         }
         default:
         {
            break;
         }
      }
   }
   return bitsPerPixel;
}

ossimString ossimNitf20Writer::getNitfPixelType() const
{
   ossimString pixelType;
   if (theInputConnection)
   {
      ossimScalarType scalarType = theInputConnection->getOutputScalarType();
      switch(scalarType)
      {
         case OSSIM_UCHAR:
         case OSSIM_USHORT11:
         case OSSIM_USHORT16:
         {
            pixelType = "INT";
            break;
         }
         case OSSIM_SSHORT16:
         {
            pixelType    = "SI";
            break;
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            pixelType    = "R";
            break;
         }
         default:
         {
            break;
         }
      }
   }
   return pixelType;
}

bool ossimNitf20Writer::saveState(ossimKeywordlist& kwl,
                                const char* prefix) const
{
   kwl.add(prefix, "enable_rpcb_tag", theEnableRpcbTagFlag, true);
   kwl.add(prefix, "enable_blocka_tag", theEnableBlockaTagFlag, true);

   return ossimImageFileWriter::saveState(kwl, prefix);
}

bool ossimNitf20Writer::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   // Look for the rpcb enable flag keyword.
   const char* lookup = kwl.find(prefix, "enable_rpcb_tag");
   if(lookup)
   {
      ossimString os = lookup;
      theEnableRpcbTagFlag = os.toBool();
   }

   // Look for the blocka enable flag keyword.
   lookup = kwl.find(prefix, "enable_blocka_tag");
   if(lookup)
   {
      ossimString os = lookup;
      theEnableBlockaTagFlag = os.toBool();
   }

   return ossimImageFileWriter::loadState(kwl, prefix);
}
