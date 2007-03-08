//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeader.cpp 10173 2007-01-03 18:21:26Z gpotts $
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/base/ossimContainerProperty.h>
#ifndef NULL
#include <stddef.h>
#endif

static const char* TAGS_KW = "tags";

RTTI_DEF2(ossimNitfFileHeader, "ossimNitfFileHeader", ossimObject, ossimPropertyInterface);

std::ostream& ossimNitfImageOffsetInformation::print(std::ostream& out) const
{
   return out << "theImageHeaderOffset:          "
              << theImageHeaderOffset << std::endl
              << "theImageDataOffset:            "
              << theImageDataOffset;
}

ossimNitfFileHeader::ossimNitfFileHeader()
{
}

ossimNitfFileHeader::~ossimNitfFileHeader()
{
}


bool ossimNitfFileHeader::getTag(ossimNitfTagInformation& tagInfo,
                                 const ossimString& tagName)const
{
   if(theTagList.size())
   {
      for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
      {
         if(theTagList[i].getTagName() == tagName)
         {
            tagInfo = theTagList[i];
            return true;
         }
      }
   }

   return false;
}

bool  ossimNitfFileHeader::hasImages()const
{
   return (getNumberOfImages() > 0);
}

bool  ossimNitfFileHeader::hasSymbols()const
{
   return (getNumberOfSymbols() > 0);
}

bool  ossimNitfFileHeader::hasGraphics()const
{
   return (getNumberOfGraphics() > 0);
}

bool  ossimNitfFileHeader::hasText()const
{
   return (getNumberOfTextSegments() > 0);
}

bool  ossimNitfFileHeader::hasLabels()const
{
   return (getNumberOfLabels() > 0);
}
bool ossimNitfFileHeader::hasDataExtSegments()const
{
   return (getNumberOfDataExtSegments() > 0);
}

bool ossimNitfFileHeader::getTagInformation(ossimNitfTagInformation& tag,
                                            int idx) const
{
   bool result = false;
   
   if(static_cast<ossim_uint32>(idx) < theTagList.size())
   {
      tag = theTagList[idx];
      result = true;
   }
   
   return result;
}

int ossimNitfFileHeader::getNumberOfTags()const
{
   return theTagList.size();
}

ossim_uint32 ossimNitfFileHeader::getTotalTagLength()const
{
   ossim_uint32 tagLength = 0;
   
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      tagLength += theTagList[i].getTotalTagLength();
   }
   
   return tagLength;
}


void ossimNitfFileHeader::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimNitfFileHeader::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;

   if(name == TAGS_KW)
   {
      ossim_uint32 idxMax = theTagList.size();
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

void ossimNitfFileHeader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TAGS_KW);
}

