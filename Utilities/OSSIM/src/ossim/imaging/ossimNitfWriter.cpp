//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfWriter.cpp 10234 2007-01-12 14:36:32Z gpotts $

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <tiffio.h>
#include <ossim/imaging/ossimNitfWriter.h>
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
   ossim_uint64 byteSize      = ossimGetScalarSizeInBytes(scalarType);
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

bool ossimNitfWriter::writeBlockBandSequential()
{
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   ossim_uint64    byteSize   = ossimGetScalarSizeInBytes(scalarType);
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

   ossim_uint64 blockSizeInBytes = blockSize.x*blockSize.y*ossimGetScalarSizeInBytes(data->getScalarType());
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

void ossimNitfWriter::setComplexityLevel(ossim_uint64 endPosition)
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
   
   theFileHeader->setComplexityLevel(ossimString("05"));
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

ossim_uint32 ossimNitfWriter::getActualBitsPerPixel() const
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

ossim_uint32 ossimNitfWriter::getBitsPerPixel() const
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

ossimString ossimNitfWriter::getNitfPixelType() const
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
