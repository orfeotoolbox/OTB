//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt for more details
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeader.cpp,v 1.10 2005/10/05 12:50:48 gpotts Exp $

#include <cmath> /* for fmod */

#include <support_data/nitf/ossimNitfImageHeader.h>
#include <base/context/ossimNotifyContext.h>

RTTI_DEF2(ossimNitfImageHeader,
          "ossimNitfImageHeader",
          ossimObject,
          ossimPropertyInterface)

ossimNitfImageHeader::ossimNitfImageHeader()
{
}

ossimNitfImageHeader::~ossimNitfImageHeader()
{
}

void ossimNitfImageHeader::addTag(ossimNitfTagInformation tag)
{
   removeTag(tag.getTagName());
   theTagList.push_back(tag);
}

bool ossimNitfImageHeader::getTag(ossimNitfTagInformation& tagInfo,
                                  const ossimString& tagName)const
{
   ossim_uint32 idx = 0;
   if(theTagList.size())
   {
      for(idx = 0; idx < theTagList.size(); ++idx)
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

int ossimNitfImageHeader::getNumberOfTags()const
{
   return theTagList.size();
}

void ossimNitfImageHeader::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimNitfImageHeader::getProperty(const ossimString& name)const
{
   return (ossimProperty*)NULL;
}

void ossimNitfImageHeader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
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

void ossimNitfImageHeader::printTags(std::ostream& out)const
{
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      out << "Tag name:   " << theTagList[i].getTagName() << std::endl;
      if(i+1 < theTagList.size())
      {
         out << "Tag Length: " << theTagList[i].getTagLength() << std::endl;
      }
      else
      {
         out << "Tag Length: " << theTagList[i].getTagLength();
      }
   }
}

bool ossimNitfImageHeader::hasLut()const
{
   return true;
}

ossimRefPtr<ossimNBandLutDataObject> ossimNitfImageHeader::createLut(ossim_uint32 bandIdx)const
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
