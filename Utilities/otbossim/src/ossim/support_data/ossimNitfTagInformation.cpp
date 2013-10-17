//*******************************************************************
//
// LICENSE: LGPL  see top level LICENSE.txt
//
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagInformation.cpp 22418 2013-09-26 15:01:12Z gpotts $

#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/support_data/ossimNitfTagFactoryRegistry.h>
#include <ossim/support_data/ossimNitfUnknownTag.h>
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
      clearFields();
      theTagOffset = in.tellg();
      in.read(theTagName, 6);
      in.read(theTagLength, 5);
      theTagDataOffset = in.tellg();

      theTagData = ossimNitfTagFactoryRegistry::instance()->create(getTagName());

      if (theTagData.valid())
      {
         if (theTagData->getClassName() == "ossimNitfUnknownTag")
         {
            // Unknown tag doesn't know his tag name yet.
            theTagData->setTagName( getTagName() );
         }

         //---
         // Tags with dynamic tag length construct with 0 length.
         // Set if 0.
         //---
         if ( theTagData->getTagLength() == 0 )
         {
            theTagData->setTagLength( getTagLength() );
         }
         // Sanity check fixed length in code with length from CEL field:
         else if ( theTagData->getTagLength() != getTagLength() )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimNitfTagInformation::parseStream WARNING!"
               << "\nCEL field length does not match fixed tag length for tag: "
               << theTagData->getTagName().c_str()
               << "\nCEL: " << getTagLength()
               << "\nTag: " << theTagData->getTagLength()
               << std::endl;
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
   out << "theTagName:            " << theTagName
       << "\ntheTagLength:        " << theTagLength
       << "\theTagType:           " << theTagType
       << "\ntheTagOffset:        " << theTagOffset
       << "\ntheTagDataOffset:    " << theTagDataOffset
       << std::endl;

   return out;
}

void ossimNitfTagInformation::clearFields()
{
   memset(theTagName, ' ', 6);
   memset(theTagLength, '0', 5);
   strcpy(theTagType, "IXSHD ");

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
ossimString ossimNitfTagInformation::getTagType() const
{
   return ossimString(theTagType).trim();
}

void ossimNitfTagInformation::setTagType(const ossimString& tagType) const
{
   std::ostringstream out;
   out << std::setw(6)
       << std::setfill(' ')
       << std::setiosflags(std::ios::left)
       << tagType;
   memcpy(theTagType, out.str().c_str(), 6);
   theTagType[6] = 0;
}

