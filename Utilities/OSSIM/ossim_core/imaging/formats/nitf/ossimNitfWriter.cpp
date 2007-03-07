//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.cpp,v 1.41 2006/01/06 16:46:46 dburken Exp $
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <tiffio.h>
#include <imaging/formats/nitf/ossimNitfWriter.h>
#include <base/common/ossimDate.h>
#include <base/common/ossimPreferences.h>
#include <base/common/ossimRefPtr.h>
#include <base/common/ossimTrace.h>
#include <base/misc/ossimEndian.h>
#include <projections/ossimProjection.h>
#include <projections/ossimRpcSolver.h>
#include <projections/map_projections/ossimUtmProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <imaging/tile_sources/ossimRectangleCutFilter.h>
#include <base/property/ossimProperty.h>
#include <base/property/ossimContainerProperty.h>
#include <base/property/ossimStringProperty.h>
#include <base/property/ossimNumericProperty.h>
#include <base/property/ossimBooleanProperty.h>
#include <support_data/nitf/ossimNitfGeoPositioningTag.h>
#include <support_data/nitf/ossimNitfLocalGeographicTag.h>
#include <support_data/nitf/ossimNitfLocalCartographicTag.h>
#include <support_data/nitf/ossimNitfProjectionParameterTag.h>
#include <support_data/nitf/ossimNitfNameConversionTables.h>
#include <support_data/nitf/ossimNitfBlockaTag.h>

RTTI_DEF1(ossimNitfWriter, "ossimNitfWriter", ossimImageFileWriter);

static ossimTrace traceDebug(ossimString("ossimNitfWriter:debug"));
                             
ossimNitfWriter::ossimNitfWriter(const ossimFilename& filename,
                                 ossimImageSource* inputSource)
   : ossimImageFileWriter(filename, inputSource, NULL),
     theOutputStream(NULL),
     theFileHeader(),
     theImageHeader(),
     theEnableRpcbTagFlag(false),
     theEnableBlockaTagFlag(true)
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
   writeGeometry();

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
   else if (name == "enable_rpcb_tag")
   {
      ossimBooleanProperty* boolProperty = PTR_CAST(ossimBooleanProperty,
                                                    property.get());
      if (boolProperty)
      {
         theEnableRpcbTagFlag = boolProperty->getBoolean();
      }
   }
   else if (name == "enable_blocka_tag")
   {
      ossimBooleanProperty* boolProperty = PTR_CAST(ossimBooleanProperty,
                                                    property.get());
      if (boolProperty)
      {
         theEnableBlockaTagFlag = boolProperty->getBoolean();
      }
   }
   else
   {
      ossimImageFileWriter::setProperty(property);
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
   else
   {
      return ossimImageFileWriter::getProperty(name);
   }

   return result;
}

void ossimNitfWriter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageFileWriter::getPropertyNames(propertyNames);

   propertyNames.push_back("file_header");
   propertyNames.push_back("image_header");
   propertyNames.push_back("enable_rpcb_tag");
   propertyNames.push_back("enable_blocka_tag");
}

bool ossimNitfWriter::writeBlockBandSeparate()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint32 byteSize      = ossimGetScalarSizeInBytes(scalarType);
   ossimIrect rect            = theInputConnection->getBoundingRect();
   ossim_uint32 bands         = theInputConnection->getNumberOfOutputBands();
   ossim_uint32 idx           = 0;
   ossim_uint64 headerStart   = (ossim_uint64)theOutputStream->tellp();
   
   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439); // ok if no tags
   imageInfoRecord.setImageLength(bands*byteSize*rect.width()*rect.height());
   

   ossimString pixelType = "INT";
   ossim_uint32 bitsPerPixel = 8;
   ossim_uint32 actualBitsPerPixel = 8;
   switch(scalarType)
   {
      case OSSIM_UCHAR:
      {
         // already set to this type by default
         break;
      }
      case OSSIM_USHORT11:
      {
         actualBitsPerPixel = 11;
         bitsPerPixel = 16;
         break;
      }
      case OSSIM_USHORT16:
      {
         actualBitsPerPixel = 16;
         bitsPerPixel = 16;
         break;
      }
      case OSSIM_SSHORT16:
      {
         actualBitsPerPixel = 16;
         bitsPerPixel = 16;
         pixelType    = "SI";
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         actualBitsPerPixel = 32;
         bitsPerPixel = 32;
         pixelType    = "R";
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         actualBitsPerPixel = 64;
         bitsPerPixel = 64;
         pixelType    = "R";
         break;
      }
      default:
      {
         // ERROR
         return false;
      }
   }
   ossimDate currentDate;
   
   theFileHeader->setDate(currentDate);
   theFileHeader->addImageInfoRecord(imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream); // this makes space for the file header; it is written again at the end of this method with updated values

   // need a better way to get the length.  This should be queried on the header before writing
   //
   ossim_uint64 headerLength = ((ossim_uint64)theOutputStream->tellp() - headerStart) /* + 1 */;
   
   ossimString representation;
   theImageHeader->setActualBitsPerPixel(actualBitsPerPixel);
   theImageHeader->setBitsPerPixel(bitsPerPixel);
   theImageHeader->setPixelType(pixelType);
   theImageHeader->setNumberOfBands(bands);
   theImageHeader->setImageMode('B');// blocked
   theImageHeader->setNumberOfRows(rect.height());
   theImageHeader->setNumberOfCols(rect.width());

   ossimIpt blockSize(64, 64);
   ossim_uint32 blocksHorizontal = (ossim_uint32)ceil(((double)rect.width()/(double)blockSize.x));
   ossim_uint32 blocksVertical   = (ossim_uint32)ceil(((double)rect.height()/(double)blockSize.y));
   
#if 0
   
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);
   ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   

//    while((ceil(((double)rect.width()/(double)blockSize.x)) < 2) &&
//          (blockSize.x > 4))
//    {
//       blockSize.x >>= 1;
//       blockSize.y >>= 1;
//    }
   
   /*
    * Geo encode the image using the simple igeolo field for NITF.   
    */
   if(proj.valid())
   {
      ossimUtmProjection* utmProj = PTR_CAST(ossimUtmProjection, proj.get());
      if(utmProj)
      {
         ossimMapProjectionInfo mapInfo(utmProj,
                                        rect);
         mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
         
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
         ossimMapProjection* tempProj = PTR_CAST(ossimMapProjection,
                                             proj.get());
         if(tempProj)
         {
            ossimMapProjectionInfo mapInfo(tempProj,
                                           rect);
            
            mapInfo.setPixelType(OSSIM_PIXEL_IS_AREA);
            ossimGpt ul = mapInfo.ulGroundPt();
            ossimGpt ur = mapInfo.urGroundPt();
            ossimGpt lr = mapInfo.lrGroundPt();
            ossimGpt ll = mapInfo.llGroundPt();
            theImageHeader->setGeographicLocationDms(ul, ur, lr, ll);
         }
         else
         {
            ossimGpt ul;
            ossimGpt ur;
            ossimGpt lr;
            ossimGpt ll;
            proj->lineSampleToWorld(rect.ul(), ul);
            proj->lineSampleToWorld(rect.ur(), ur);
            proj->lineSampleToWorld(rect.lr(), lr);
            proj->lineSampleToWorld(rect.ll(), ll);
            theImageHeader->setGeographicLocationDms(ul, ur, lr, ll);
         }
      }
   }

#endif
   
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
//    theImageHeader->setNumberOfRows(blockSize.y*blocksVertical);
//    theImageHeader->setNumberOfCols(blockSize.x*blocksHorizontal);
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

   theInputConnection->setTileSize(blockSize);
   ossim_uint32 numberOfTiles   = theInputConnection->getNumberOfTiles();
   theInputConnection->setToStartOfSequence();
   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint32 tileNumber = 1;
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

bool ossimNitfWriter::writeBlockBandSequential()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint32 byteSize      = ossimGetScalarSizeInBytes(scalarType);
   ossimIrect rect            = theInputConnection->getBoundingRect();
   ossim_uint32 bands         = theInputConnection->getNumberOfOutputBands();
   ossim_uint32 idx           = 0;
   ossim_uint64 headerStart   = (ossim_uint64)theOutputStream->tellp();

   ossimIpt blockSize(64, 64);

//    while((ceil(((double)rect.width()/(double)blockSize.x)) < 2) &&
//          (blockSize.x > 4))
//    {
//       blockSize.x >>= 1;
//       blockSize.y >>= 1;
//    }
   ossim_uint32 blocksHorizontal = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint32 blocksVertical   = theInputConnection->getNumberOfTilesVertical();
   theInputConnection->setTileSize(blockSize);
   ossim_uint32 numberOfTiles   = theInputConnection->getNumberOfTiles();
   theInputConnection->setToStartOfSequence();
   
   ossimNitfImageInfoRecordV2_1 imageInfoRecord;
   imageInfoRecord.setSubheaderLength(439);
   imageInfoRecord.setImageLength(bands*byteSize*rect.width()*rect.height());
   

   ossimString pixelType = "INT";
   ossim_uint32 bitsPerPixel = 8;
   switch(scalarType)
   {
      case OSSIM_UCHAR:
      {
         // already set to this type by default
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         bitsPerPixel = 16;
         break;
      }
      case OSSIM_SSHORT16:
      {
         bitsPerPixel = 16;
         pixelType    = "SI";
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {
         bitsPerPixel = 32;
         pixelType    = "R";
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         bitsPerPixel = 64;
         pixelType    = "R";
         break;
      }
      default:
      {
         // ERROR
         return false;
      }
   }
   ossimDate currentDate;
   
   theFileHeader->setDate(currentDate);
   theFileHeader->addImageInfoRecord(imageInfoRecord);
   theFileHeader->writeStream(*theOutputStream);

   // need a better way to get the length.  This should be queried on the header before writing
   //
   ossim_uint64 headerLength = ((ossim_uint64)theOutputStream->tellp() - headerStart) /* + 1 */;
   
   ossimString representation;
   theImageHeader->setBitsPerPixel(bitsPerPixel);
   theImageHeader->setPixelType(pixelType);
   theImageHeader->setNumberOfBands(bands);
   theImageHeader->setImageMode('S');// blocked
   theImageHeader->setNumberOfRows(rect.height());
   theImageHeader->setNumberOfCols(rect.width());

#if 0
   
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);
   ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);

   /*
    * Geo encode the image using the simple igeolo field for NITF.   
    */
   if(proj.valid())
   {
      ossimUtmProjection* utmProj = PTR_CAST(ossimUtmProjection, proj.get());
      if(utmProj)
      {
         ossimDpt ul;
         ossimDpt ur;
         ossimDpt lr;
         ossimDpt ll;

         utmProj->lineSampleToEastingNorthing(rect.ul(), ul);
         utmProj->lineSampleToEastingNorthing(rect.ur(), ur);
         utmProj->lineSampleToEastingNorthing(rect.lr(), lr);
         utmProj->lineSampleToEastingNorthing(rect.ll(), ll);
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
         ossimGpt ul;
         ossimGpt ur;
         ossimGpt lr;
         ossimGpt ll;
         proj->lineSampleToWorld(rect.ul(), ul);
         proj->lineSampleToWorld(rect.ur(), ur);
         proj->lineSampleToWorld(rect.lr(), lr);
         proj->lineSampleToWorld(rect.ll(), ll);
         theImageHeader->setGeographicLocationDms(ul, ur, lr, ll);
      }
   }

#endif
   
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
   theImageHeader->setNumberOfRows(blockSize.y*blocksVertical);
   theImageHeader->setNumberOfCols(blockSize.x*blocksHorizontal);

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
   
   theImageHeader->writeStream(*theOutputStream);
   ossimIpt ul = rect.ul();
   ossimRefPtr<ossimImageData> data = theInputConnection->getNextTile();
   ossim_uint32 tileNumber = 0;
   ossimEndian endian;

   // get the start to the first band of data block
   //
   ossim_uint64 streamOffset = theOutputStream->tellp();
   
   // holds the total pixels to the next band

   ossim_uint32 blockSizeInBytes = blockSize.x*blockSize.y*ossimGetScalarSizeInBytes(data->getScalarType());
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

   /*
    * Need to change the way I compute file length and header length later
    * We need to figure out a better way to compute.
    */
   theFileHeader->setFileLength(pos);
   theFileHeader->setHeaderLength(headerLength);
   theOutputStream->seekp(0, ios::beg);
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

void ossimNitfWriter::writeGeometry()
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

void ossimNitfWriter::addBlockaTag(ossimMapProjectionInfo& mapInfo)
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
         << "ossimNitfWriter::writeGeometry DEBUG:"
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

void ossimNitfWriter::addRpcbTag(const ossimIrect& rect,
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
         << "ossimNitfWriter::addRpcbTag DEBUG:"
         << "\nRPCB Tag:" << *((ossimObject*)(tag.get()))
         << "\nProjection:" << std::endl;
      
      proj->print(ossimNotify(ossimNotifyLevel_DEBUG));
      
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nRect: " << rect << std::endl;
   }
}

bool ossimNitfWriter::saveState(ossimKeywordlist& kwl,
                                const char* prefix) const
{
   kwl.add(prefix, "enable_rpcb_tag", theEnableRpcbTagFlag, true);
   kwl.add(prefix, "enable_blocka_tag", theEnableBlockaTagFlag, true);

   return ossimImageFileWriter::saveState(kwl, prefix);
}

bool ossimNitfWriter::loadState(const ossimKeywordlist& kwl,
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
