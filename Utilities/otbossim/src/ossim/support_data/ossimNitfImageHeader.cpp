//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeader.cpp 18443 2010-11-22 23:42:39Z gpotts $

#include <cmath> /* for fmod */
#include <iomanip>
#include <sstream>

#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF2(ossimNitfImageHeader,
          "ossimNitfImageHeader",
          ossimObject,
          ossimPropertyInterface)
static const char* TAGS_KW = "tags";
ossimNitfImageHeader::ossimNitfImageHeader()
{
}

ossimNitfImageHeader::~ossimNitfImageHeader()
{
}

bool ossimNitfImageHeader::getTagInformation(ossimNitfTagInformation& tagInfo,
                                             const ossimString& tagName)const
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            tagInfo = theTagList[idx];
            return true;
         }
      }
   }
   
   return false;
}

bool ossimNitfImageHeader::getTagInformation(ossimNitfTagInformation& tag,
                                             ossim_uint32 idx)const
{
   bool result = false;
   
   if(idx < theTagList.size())
   {
      tag = theTagList[idx];
      result = true;
   }
   
   return result;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfImageHeader::getTagData(
   const ossimString& tagName)
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            return theTagList[idx].getTagData();
         }
      }
   }

   return ossimRefPtr<ossimNitfRegisteredTag>();
}
   
const ossimRefPtr<ossimNitfRegisteredTag> ossimNitfImageHeader::getTagData(
   const ossimString& tagName) const
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            return theTagList[idx].getTagData();
         }
      }
   }
   
   return ossimRefPtr<ossimNitfRegisteredTag>();
}

ossim_uint32 ossimNitfImageHeader::getNumberOfTags()const
{
   return static_cast<ossim_uint32>(theTagList.size());
}

void ossimNitfImageHeader::addTag(const ossimNitfTagInformation& tag)
{
   removeTag(tag.getTagName());
   theTagList.push_back(tag);
}

void ossimNitfImageHeader::removeTag(const ossimString& tagName)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theTagList.size(); ++idx)
   {
      if(theTagList[idx].getTagName() == tagName)
      {
         theTagList.erase(theTagList.begin() + idx);
         return;
      }
   }
}

bool ossimNitfImageHeader::isSameAs(const ossimNitfImageHeader* hdr) const
{
   if (!hdr) return false;
   
   return ( (isCompressed() == hdr->isCompressed()) &&
            (getNumberOfRows() == hdr->getNumberOfRows()) &&
            (getNumberOfBands() == hdr->getNumberOfBands()) &&
            (getNumberOfCols() == hdr->getNumberOfCols()) &&
            (getNumberOfBlocksPerRow() == hdr->getNumberOfBlocksPerRow()) &&
            (getNumberOfBlocksPerCol() == hdr->getNumberOfBlocksPerCol()) &&
            (getNumberOfPixelsPerBlockHoriz() ==
             hdr->getNumberOfPixelsPerBlockHoriz()) &&
            (getNumberOfPixelsPerBlockVert() ==
             hdr->getNumberOfPixelsPerBlockVert()) &&
            (getBitsPerPixelPerBand() == hdr->getBitsPerPixelPerBand()) &&
            (getImageRect() == hdr->getImageRect()) &&
            (getIMode() == hdr->getIMode()) &&
            (getCoordinateSystem() == hdr->getCoordinateSystem()) &&
            (getGeographicLocation() == hdr->getGeographicLocation()) );
}


void ossimNitfImageHeader::setProperty(ossimRefPtr<ossimProperty> /* property */)
{
}

ossimRefPtr<ossimProperty> ossimNitfImageHeader::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;

   if(name == TAGS_KW)
   {
      ossim_uint32 idxMax = getNumberOfTags();
      if(idxMax > 0)
      {
         ossimContainerProperty* containerProperty = new ossimContainerProperty;
         containerProperty->setName(name);
         ossim_uint32 idx = 0;

         result = containerProperty;
         for(idx = 0; idx < idxMax; ++idx)
         {
            ossimNitfTagInformation tagInfo;
            getTagInformation(tagInfo, idx);
            
            const ossimRefPtr<ossimNitfRegisteredTag> tagData = tagInfo.getTagData();
            if(tagData.valid())
            {
               ossimContainerProperty* containerPropertyTag = new ossimContainerProperty;
               containerPropertyTag->setName(tagInfo.getTagName());
               std::vector<ossimRefPtr<ossimProperty> > propertyList;
               tagData->getPropertyList(propertyList);
               
               containerPropertyTag->addChildren(propertyList);
               containerProperty->addChild(containerPropertyTag);
            }
            else
            {
               containerProperty->addStringProperty(tagInfo.getTagName(), "", true);
            }
         }
      }
   }
   
   return result;
}

void ossimNitfImageHeader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TAGS_KW);
  
}

ossim_uint32 ossimNitfImageHeader::getTotalTagLength()const
{
   ossim_uint32 tagLength = 0;
   
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      tagLength += theTagList[i].getTotalTagLength();
   }

   return tagLength;
}

std::ostream& ossimNitfImageHeader::printTags(std::ostream& out,
                                              const std::string& prefix)const
{
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      ossimRefPtr<ossimNitfRegisteredTag> tag = theTagList[i].getTagData();
      if (tag.valid())
      {
         tag->print(out, prefix);
      }
   }
   return out;
}

void ossimNitfImageHeader::getMetadata(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   kwl.add(prefix, "source",
           getImageSource().c_str(),
           false);
   kwl.add(prefix,
           "image_date",
           getAcquisitionDateMonthDayYear().c_str(),
           false);
   kwl.add(prefix,
           "image_title",
           getTitle().c_str(),
           false);
}

bool ossimNitfImageHeader::hasLut() const
{
   bool result = false;

   const ossim_uint32 BANDS = static_cast<ossim_uint32>(getNumberOfBands());
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const ossimRefPtr<ossimNitfImageBand> imgBand = getBandInformation(band);
      if(imgBand.valid())
      {
         ossim_uint32 luts = imgBand->getNumberOfLuts();
         if(luts > 0)
         {
            if(imgBand->getLut(0).valid())
            {
               result = true;
               break;
            }
         }
      }
   }
   
   return result;
}

ossimRefPtr<ossimNBandLutDataObject> ossimNitfImageHeader::createLut(
   ossim_uint32 bandIdx)const
{
   ossimRefPtr<ossimNBandLutDataObject> result;
   
   if(bandIdx < (ossim_uint32)getNumberOfBands())
   {
      const ossimRefPtr<ossimNitfImageBand> band = getBandInformation(bandIdx);
      if(band.valid())
      {
         ossim_uint32 bands = band->getNumberOfLuts();
         if(bands > 0)
         {
            if(band->getLut(0).valid())
            {
               ossim_uint32 entries = band->getLut(0)->getNumberOfEntries();
               
               result = new ossimNBandLutDataObject();
               result->create(entries, band->getNumberOfLuts());
               ossim_uint32 bIdx;
               ossim_uint32 entryIdx;

               for(bIdx = 0; bIdx < bands; ++bIdx)
               {
                  const ossimRefPtr<ossimNitfImageLut> lut = band->getLut(bIdx);
                  if(lut.valid())
                  {
                     for(entryIdx = 0; entryIdx < entries; ++entryIdx)
                     {
                        (*result)[entryIdx][bIdx] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(lut->getValue(entryIdx));
                     }
                  }
               }
            }
         }
      }
   }

   return result;
}


void ossimNitfImageHeader::checkForGeographicTiePointTruncation(
   const ossimDpt& tie) const
{
   // One arc second in decimal degrees.
   const ossim_float64 ARC_SECOND = 1.0/3600.0;

   // Very small number.
   const ossim_float64 FUDGE_FACTOR = 0.000000001;

   // Remainder portion of latitude divided by an arc second.
   ossim_float64 y = std::fmod(tie.y, ARC_SECOND);

   // Remainder portion of longitue divided by an arc second.
   ossim_float64 x = std::fmod(tie.x, ARC_SECOND);

   if ( (std::fabs(y) > FUDGE_FACTOR) || (std::fabs(x) > FUDGE_FACTOR) )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfImageHeader WARNING:\n"
         << "Tie point will be truncated in image header:  "
         << tie
         << std::endl;
   }
}

void ossimNitfImageHeader::getDecimationFactor(ossim_float64& result) const
{
   //---
   // Look for string like:
   // 
   // "/2" = 1/2
   // "/4  = 1/4
   // ...
   // "/16 = 1/16
   // 
   // If it is full resolution it should be "1.0"
   //
   // or
   //
   // "0.5" which is the same as "/2"
   // "0.25" which is the same as "/4"
   //---
   ossimString os = getImageMagnification();

   // Spec says to fill with spaces so strip them.
   os.trim(ossimString(" "));

   if (os.size())
   {
      if ( os.contains("/") )
      {
         os = os.after("/");
         result = os.toFloat64();
         if (result)
         {
            result = 1.0 / result;
         }
      }
      else
      {
         result = os.toFloat64();
      }
   }
   else
   {
      result = ossim::nan();
   }
}


bool ossimNitfImageHeader::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   if(!ossimObject::saveState(kwl, prefix)) return false;
   ossimString tagsPrefix = prefix;
   
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      ossimRefPtr<ossimNitfRegisteredTag> tag = theTagList[i].getTagData();
      if (tag.valid())
      {
         if(!tag->saveState(kwl, tagsPrefix))
         {
            return false;
         }
      }
   }
   return true;
}
