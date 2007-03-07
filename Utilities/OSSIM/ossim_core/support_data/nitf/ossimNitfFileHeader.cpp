//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeader.cpp,v 1.8 2004/09/29 13:47:00 gpotts Exp $
#include "ossimNitfFileHeader.h"

#ifndef NULL
#include <stddef.h>
#endif

RTTI_DEF2(ossimNitfFileHeader, "ossimNitfFileHeader", ossimObject, ossimPropertyInterface);

std::ostream& ossimNitfImageOffsetInformation::print(std::ostream& out) const
{
   return out << "theImageHeaderOffset:          "
              << theImageHeaderOffset << endl
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
   return NULL;
}

void ossimNitfFileHeader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
}

