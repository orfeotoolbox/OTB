//*******************************************************************
//
// LICENSE: LGPL  see top level LICENSE.txt
//
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagInformation.cpp 16997 2010-04-12 18:53:48Z dburken $

#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfUnknownTag.h>
#include <ossim/base/ossimCommon.h>
#include <sstream>
#include <iomanip>
#include <cstring> // for memset

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

      theTagData = ossimNitfTagFactoryRegistry::instance()->
         create(getTagName());

      if (theTagData.valid())
      {
         if (theTagData->getClassName() == "ossimNitfUnknownTag")
         {
            ossimNitfUnknownTag* tag = PTR_CAST(ossimNitfUnknownTag,
                                                theTagData.get());
            if (tag)
            {
               tag->setTagName(getTagName());
               tag->setTagLength(getTagLength());
            }
         }
                               
         theTagData->parseStream(in);
      }
      else
      {
         theTagData = (ossimNitfRegisteredTag*)NULL;
      }
   }
}

void ossimNitfTagInformation::writeStream(std::ostream &out)
{
   theTagOffset = out.tellp(); // Capture the offset.
   out.write(theTagName, 6);
   out.write(theTagLength, 5);
   if(theTagData.valid())
   {
      theTagDataOffset = out.tellp();
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
   out << "theTagName:          " << theTagName
       << "\ntheTagLength:        " << theTagLength
       << "\ntheTagOffset:        " << theTagOffset
       << "\ntheTagDataOffset:    " << theTagDataOffset
       << std::endl;

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
