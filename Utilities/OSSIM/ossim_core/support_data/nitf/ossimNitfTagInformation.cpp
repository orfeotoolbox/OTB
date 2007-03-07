//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL  see top level LICENSE.txt
//
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagInformation.cpp,v 1.7 2004/11/18 17:09:27 gpotts Exp $
#include "ossimNitfTagInformation.h"
#include <base/common/ossimCommon.h>
#include <sstream>
#include <iomanip>
#include <string.h> // for memset

ossimNitfTagInformation::ossimNitfTagInformation(ossimRefPtr<ossimNitfRegisteredTag> tagData)
{
   clearFields();
   setTagData(tagData);
}

ossimNitfTagInformation::~ossimNitfTagInformation()
{
}

void ossimNitfTagInformation::parseStream(std::istream &in)
{
   if(in)
   {
      theTagOffset = in.tellg();
      in.read(theTagName, 6);
      in.read(theTagLength, 5);
      theTagDataOffset = in.tellg();
      
      // we will not read tag information in here.
      // This is for speed.  No need to read tags if not used.
      // But we will save offsets so they can be read later.
      //
      theTagData = (ossimNitfRegisteredTag*)NULL;
   }
   else
   {
      //ERROR
   }
}


void ossimNitfTagInformation::writeStream(std::ostream &out)
{
   out.write(theTagName, 6);
   out.write(theTagLength, 5);
   if(theTagData.valid())
   {
      theTagData->writeStream(out);
   }
}

ossim_uint32 ossimNitfTagInformation::getTotalTagLength()const
{
   return (getTagLength() + (ossim_uint32)11);
}

ossim_uint32 ossimNitfTagInformation::getTagLength()const
{
   return ossimString(theTagLength).toUInt32();
}

ossim_uint64 ossimNitfTagInformation::getTagOffset()const
{
   return theTagOffset;
}

ossim_uint64 ossimNitfTagInformation::getTagDataOffset()const
{
   return theTagDataOffset;
}

ossimString ossimNitfTagInformation::getTagName()const
{
   return ossimString(theTagName).trim();
}

std::ostream& ossimNitfTagInformation::print(std::ostream& out)const
{
   out << "theTagName:          " << theTagName   << endl
       << "theTagLength:        " << theTagLength << endl
       << "theTagOffset:        " << theTagOffset << endl
       << "theTagDataOffset:    " << theTagDataOffset;

   return out;
}

void ossimNitfTagInformation::clearFields()
{
   memset(theTagName, ' ', 6);
   memset(theTagLength, '0', 5);

   theTagName[6]    = '\0';
   theTagLength[5]  = '\0';
   
   theTagOffset     = 0;
   theTagDataOffset = 0;
}

void ossimNitfTagInformation::setTagName(const ossimString& tagName)
{
   memset(theTagName, ' ', 6);

   std::ostringstream out;

   out << std::setw(6)
       << std::setfill(' ')
       << tagName;
   memcpy(theTagName, out.str().c_str(), 6);
}

void ossimNitfTagInformation::setTagLength(ossim_uint32 tagLength)
{
   memset(theTagLength, '0', 5);
   
   if(tagLength > 99999)
   {
      tagLength = 99999;
   }

   std::ostringstream out;

   out << std::setw(5)
       << std::setfill('0')
       << tagLength;
   
   memcpy(theTagLength, out.str().c_str(), 5);
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfTagInformation::getTagData()
{
   return theTagData;
}

const ossimRefPtr<ossimNitfRegisteredTag> ossimNitfTagInformation::getTagData()const
{
   return theTagData;
}

void ossimNitfTagInformation::setTagData(ossimRefPtr<ossimNitfRegisteredTag> tagData)
{
   theTagData = tagData;

   memset(theTagName, ' ', 6);
   memset(theTagLength, ' ', 5);
   
   if(theTagData.valid())
   {
      setTagName(theTagData->getRegisterTagName());
      setTagLength(theTagData->getSizeInBytes());
   }
}
