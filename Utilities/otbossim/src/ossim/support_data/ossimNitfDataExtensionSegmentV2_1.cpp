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
// $Id: ossimNitfDataExtensionSegmentV2_1.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>

#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_1.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>

#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimBinaryDataProperty.h>
RTTI_DEF1(ossimNitfDataExtensionSegmentV2_1, "ossimNitfDataExtensionSegmentV2_1",ossimNitfDataExtensionSegment)


const ossimString ossimNitfDataExtensionSegmentV2_1::DESID_KW = "desid";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESCLSY_KW = "desclsy";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESDCTP_KW = "desdctp";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESDCDT_KW = "desdcdt";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESDCXM_KW = "desdcxm";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESDG_KW = "desdg";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESDGDT_KW = "desdgdt";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESCLTX_KW = "descltx";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESCATP_KW = "descatp";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESCRSN_KW = "descrsn";
const ossimString ossimNitfDataExtensionSegmentV2_1::DESSRDT_KW = "dessrdt";

std::ostream& operator <<(std::ostream& out,
                          const ossimNitfDataExtIdentSecurityChunkV2_1& data)
{
   out << "theFilePartType:                  " << data.theFilePartType
       << "\ntheUniqueDataExtTypeId:         " << data.theUniqueDataExtTypeId
       << "\ntheVersionDataFieldDef:         " << data.theVersionDataFieldDef
       << "\ntheDataExtSecurityClass:        " << data.theDataExtSecurityClass
       << "\ntheDataExtClassSystem:          " << data.theDataExtClassSystem
       << "\ntheDataExtCodewords:            " << data.theDataExtCodewords
       << "\ntheDataExtControlAndHand:       " << data.theDataExtControlAndHand
       << "\ntheDataExtReleasingInst:        " << data.theDataExtReleasingInst
       << "\ntheDataExtDeclassType:          " << data.theDataExtDeclassType
       << "\ntheDataExtDeclassDate:          " << data.theDataExtDeclassDate
       << "\ntheDataExtDeclassExempt:        " << data.theDataExtDeclassExempt
       << "\ntheDataExtSecurityDowngrade:    " << data.theDataExtSecurityDowngrade
       << "\ntheDataExtSecurityDowngradeDate:" << data.theDataExtSecurityDowngradeDate
       << "\ntheDataExtSecurityClassText:    " << data.theDataExtSecurityClassText
       << "\ntheDataExtClassAuthType:        " << data.theDataExtClassAuthType
       << "\ntheDataExtClassAuthority:       " << data.theDataExtClassAuthority
       << "\ntheDataExtClassReason:          " << data.theDataExtClassReason
       << "\ntheDataExtSecuritySourceDate:   " << data.theDataExtSecuritySourceDate
       << "\ntheDataExtSecurityConNum:       " << data.theDataExtSecurityConNum
       << std::endl;
   
   return out;
}
   
ossimNitfDataExtensionSegmentV2_1::ossimNitfDataExtensionSegmentV2_1() :
   theUserDefinedSubheaderFields(NULL),
   theData(NULL)
{
   clearFields();
}

ossimNitfDataExtensionSegmentV2_1::~ossimNitfDataExtensionSegmentV2_1()
{

}

void ossimNitfDataExtensionSegmentV2_1::parseStream(std::istream &in, ossim_uint64 dataLength)
{
   if(in)
   {
      clearFields();

      in.read(theIdentSecurityChunk.theFilePartType, 2); // 2 byte alpha
      in.read(theIdentSecurityChunk.theUniqueDataExtTypeId, 25); // 25 byte alpha numeric
      in.read(theIdentSecurityChunk.theVersionDataFieldDef, 2); // 2 byte numeric
      in.read(theIdentSecurityChunk.theDataExtSecurityClass, 1); // 1 byte alpha
      in.read(theIdentSecurityChunk.theDataExtClassSystem, 2);
      in.read(theIdentSecurityChunk.theDataExtCodewords, 11);    // 11 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtControlAndHand, 2); // 2 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtReleasingInst, 20); // 20 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtDeclassType, 2); // 2 byte alph num
      in.read(theIdentSecurityChunk.theDataExtDeclassDate, 8); // 8 byte alph num
      in.read(theIdentSecurityChunk.theDataExtDeclassExempt, 4); // 4 byte alpha num
      in.read(theIdentSecurityChunk.theDataExtSecurityDowngrade, 1); // 1 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtSecurityDowngradeDate, 8); // 8 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtSecurityClassText, 43); // 43 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtClassAuthType, 1); // 1 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtClassAuthority, 40); // 40 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtClassReason, 1); // 1 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtSecuritySourceDate, 8); // 8 byte alpha num      
      in.read(theIdentSecurityChunk.theDataExtSecurityConNum, 15); // 15 byte alpha num      

      bool overflow = ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId).trim().upcase() == "TRE_OVERFLOW";
      if (overflow)
      {
         in.read(theOverflowedHeaderType, 6);
         in.read(theDataItemOverflowed, 3);
      }

      in.read(theLengthOfUserDefinedSubheaderFields, 4);

      // look to see if there is any data and if so read the tag
      // to see what it is
      long length = ossimString(theLengthOfUserDefinedSubheaderFields).toLong();
      if (length > 0)
      {
         theUserDefinedSubheaderFields.resize(length);
         in.read(reinterpret_cast<char*>(&theUserDefinedSubheaderFields.front()), theUserDefinedSubheaderFields.size());
      }

      if (dataLength != 0)
      {
         if (overflow)
         {
            istream::pos_type dataBegin = in.tellg();
            while (in.tellg() < dataBegin + static_cast<istream::pos_type>(dataLength))
            {
               ossimNitfTagInformation tag;
               tag.clearFields();
               tag.parseStream(in);
               theTagList.push_back(tag);
            }

            ossimIFStream64::seekg64(in, dataBegin, ios::beg);
         }

         theData.resize(dataLength);
         in.read(reinterpret_cast<char*>(&theData.front()), theData.size());
      }
   }
}

void ossimNitfDataExtensionSegmentV2_1::writeStream(std::ostream &out)
{
   if (out)
   {
      out.write(theIdentSecurityChunk.theFilePartType, 2);
      out.write(theIdentSecurityChunk.theUniqueDataExtTypeId, 25);
      out.write(theIdentSecurityChunk.theVersionDataFieldDef, 2);
      out.write(theIdentSecurityChunk.theDataExtSecurityClass, 1);
      out.write(theIdentSecurityChunk.theDataExtClassSystem, 2);
      out.write(theIdentSecurityChunk.theDataExtCodewords, 11);
      out.write(theIdentSecurityChunk.theDataExtControlAndHand, 2);
      out.write(theIdentSecurityChunk.theDataExtReleasingInst, 20);
      out.write(theIdentSecurityChunk.theDataExtDeclassType, 2);
      out.write(theIdentSecurityChunk.theDataExtDeclassDate, 8);
      out.write(theIdentSecurityChunk.theDataExtDeclassExempt, 4);
      out.write(theIdentSecurityChunk.theDataExtSecurityDowngrade, 1);
      out.write(theIdentSecurityChunk.theDataExtSecurityDowngradeDate, 8);
      out.write(theIdentSecurityChunk.theDataExtSecurityClassText, 43);
      out.write(theIdentSecurityChunk.theDataExtClassAuthType, 1);
      out.write(theIdentSecurityChunk.theDataExtClassAuthority, 40);
      out.write(theIdentSecurityChunk.theDataExtClassReason, 1);
      out.write(theIdentSecurityChunk.theDataExtSecuritySourceDate, 8);
      out.write(theIdentSecurityChunk.theDataExtSecurityConNum, 15);

      bool overflow = ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId).trim().upcase() == "TRE_OVERFLOW";
      if (overflow)
      {
         out.write(theOverflowedHeaderType, 6);
         out.write(theDataItemOverflowed, 3);
      }

      out.write(theLengthOfUserDefinedSubheaderFields, 4);
      for (unsigned int i = 0; i < theUserDefinedSubheaderFields.size(); i++)
      {
         out.write(reinterpret_cast<char*>(&theUserDefinedSubheaderFields.front()), theUserDefinedSubheaderFields.size());
      }

      if (overflow && theTagList.empty() == false)
      {
         for (std::vector<ossimNitfTagInformation>::iterator iter = theTagList.begin();
            iter != theTagList.end(); ++iter)
         {
            iter->writeStream(out);
         }
      }
      else
      {
         out.write(reinterpret_cast<char*>(&theData.front()), theData.size());
      }
   }
}

void ossimNitfDataExtensionSegmentV2_1::clearFields()
{
   memset(theIdentSecurityChunk.theFilePartType, ' ', 2);
   memset(theIdentSecurityChunk.theUniqueDataExtTypeId, ' ', 25);
   memset(theIdentSecurityChunk.theVersionDataFieldDef, '0', 2);
   memset(theIdentSecurityChunk.theDataExtSecurityClass, ' ', 1);
   memset(theIdentSecurityChunk.theDataExtClassSystem, ' ', 2);
   memset(theIdentSecurityChunk.theDataExtCodewords, ' ', 11);
   memset(theIdentSecurityChunk.theDataExtControlAndHand, ' ', 2);
   memset(theIdentSecurityChunk.theDataExtReleasingInst, ' ', 20);
   memset(theIdentSecurityChunk.theDataExtDeclassType, ' ', 2);
   memset(theIdentSecurityChunk.theDataExtDeclassDate, ' ', 8);
   memset(theIdentSecurityChunk.theDataExtDeclassExempt, ' ', 4);
   memset(theIdentSecurityChunk.theDataExtSecurityDowngrade, ' ', 1);
   memset(theIdentSecurityChunk.theDataExtSecurityDowngradeDate, ' ', 8);
   memset(theIdentSecurityChunk.theDataExtSecurityClassText, ' ', 43);
   memset(theIdentSecurityChunk.theDataExtClassAuthType, ' ', 1);
   memset(theIdentSecurityChunk.theDataExtClassAuthority, ' ', 40);
   memset(theIdentSecurityChunk.theDataExtClassReason, ' ', 1);
   memset(theIdentSecurityChunk.theDataExtSecuritySourceDate, ' ', 8);
   memset(theIdentSecurityChunk.theDataExtSecurityConNum, ' ', 15);

   memset(theOverflowedHeaderType, ' ', 6);
   memset(theDataItemOverflowed, '0', 3);
   memset(theLengthOfUserDefinedSubheaderFields, '0', 4);
   
   theIdentSecurityChunk.theFilePartType[2] = '\0';
   theIdentSecurityChunk.theUniqueDataExtTypeId[25] = '\0';
   theIdentSecurityChunk.theVersionDataFieldDef[2] = '\0';
   theIdentSecurityChunk.theDataExtSecurityClass[1] = '\0';
   theIdentSecurityChunk.theDataExtClassSystem[2] = '\0';
   theIdentSecurityChunk.theDataExtCodewords[11] = '\0';
   theIdentSecurityChunk.theDataExtControlAndHand[2] = '\0';
   theIdentSecurityChunk.theDataExtReleasingInst[20] = '\0';
   theIdentSecurityChunk.theDataExtDeclassType[2] = '\0';
   theIdentSecurityChunk.theDataExtDeclassDate[8] = '\0';
   theIdentSecurityChunk.theDataExtDeclassExempt[4] = '\0';
   theIdentSecurityChunk.theDataExtSecurityDowngrade[1] = '\0';
   theIdentSecurityChunk.theDataExtSecurityDowngradeDate[8] = '\0';
   theIdentSecurityChunk.theDataExtSecurityClassText[43] = '\0';
   theIdentSecurityChunk.theDataExtClassAuthType[1] = '\0';
   theIdentSecurityChunk.theDataExtClassAuthority[40] = '\0';
   theIdentSecurityChunk.theDataExtClassReason[1] = '\0';
   theIdentSecurityChunk.theDataExtSecuritySourceDate[8] = '\0';
   theIdentSecurityChunk.theDataExtSecurityConNum[15] = '\0';

   theOverflowedHeaderType[6] = '\0';
   theDataItemOverflowed[3]   = '\0';
   theLengthOfUserDefinedSubheaderFields[4] = '\0';
   theUserDefinedSubheaderFields.clear();
   theData.clear();
   theTagList.clear();
}

void ossimNitfDataExtensionSegmentV2_1::setTagList(const std::vector<ossimNitfTagInformation> &tagList)
{
   theTagList = tagList;
   theData.clear();
}

void ossimNitfDataExtensionSegmentV2_1::setSecurityMarkings(const ossimNitfFileHeaderV2_1 &fileHeader)
{
   strcpy(theIdentSecurityChunk.theDataExtSecurityClass, fileHeader.getSecurityClassification().c_str());
   strcpy(theIdentSecurityChunk.theDataExtClassSystem, fileHeader.getSecurityClassificationSys().c_str());
   strcpy(theIdentSecurityChunk.theDataExtCodewords, fileHeader.getCodeWords().c_str());
   strcpy(theIdentSecurityChunk.theDataExtControlAndHand, fileHeader.getControlAndHandling().c_str());
   strcpy(theIdentSecurityChunk.theDataExtReleasingInst, fileHeader.getReleasingInstructions().c_str());
   strcpy(theIdentSecurityChunk.theDataExtDeclassType, fileHeader.getDeclassificationType().c_str());
   strcpy(theIdentSecurityChunk.theDataExtDeclassDate, fileHeader.getDeclassificationDate().c_str());
   strcpy(theIdentSecurityChunk.theDataExtDeclassExempt, fileHeader.getDeclassificationExemption().c_str());
   strcpy(theIdentSecurityChunk.theDataExtSecurityDowngrade, fileHeader.getDowngrade().c_str());
   strcpy(theIdentSecurityChunk.theDataExtSecurityDowngradeDate, fileHeader.getDowngradingDate().c_str());
   strcpy(theIdentSecurityChunk.theDataExtSecurityClassText, fileHeader.getClassificationText().c_str());
   strcpy(theIdentSecurityChunk.theDataExtClassAuthType, fileHeader.getClassificationAuthorityType().c_str());
   strcpy(theIdentSecurityChunk.theDataExtClassAuthority, fileHeader.getClassificationAuthority().c_str());
   strcpy(theIdentSecurityChunk.theDataExtClassReason, fileHeader.getClassificationReason().c_str());
   strcpy(theIdentSecurityChunk.theDataExtSecuritySourceDate, fileHeader.getSecuritySourceDate().c_str());
   strcpy(theIdentSecurityChunk.theDataExtSecurityConNum, fileHeader.getSecurityControlNumber().c_str());
}
  
std::ostream& ossimNitfDataExtensionSegmentV2_1::print(std::ostream& out)const
{
   out << "theIdentSecurityChunk:                   "
       << theIdentSecurityChunk
       << "\ntheOverflowedHeaderType:               "
       << theOverflowedHeaderType
       << "\ntheDataItemOverflowed:                 "
       << theDataItemOverflowed
       << "\ntheLengthOfUserDefinedSubheaderFields: "
       << theLengthOfUserDefinedSubheaderFields
       << "\ntag count:                             "
       << theTagList.size() << std::endl;
   return out;
}

ossim_uint32 ossimNitfDataExtensionSegmentV2_1::getHeaderLength() const
{
   if (ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId).trim().upcase() == "TRE_OVERFLOW")
   {
      return 209;
   }

   return 200 + theUserDefinedSubheaderFields.size();
}

ossim_uint64 ossimNitfDataExtensionSegmentV2_1::getDataLength() const
{
   ossim_uint64 totalDataLength = 0;
   if (theTagList.empty() == true)
   {
      totalDataLength = theData.size();
   }
   else
   {
      for (std::vector<ossimNitfTagInformation>::const_iterator iter = theTagList.begin();
         iter != theTagList.end(); iter++)
      {
         totalDataLength += iter->getTotalTagLength();
      }
   }

   return totalDataLength;
}

void ossimNitfDataExtensionSegmentV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (property.get() == NULL)
   {
      return;
   }

   const ossimString& name = property->getName();
   if (name == DE_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theFilePartType, property->valueToString(), 2);
   }
   else if (name == DESID_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theUniqueDataExtTypeId, property->valueToString(), 25);
   }
   else if (name == DESVER_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theVersionDataFieldDef, property->valueToString(), 2, ios::right, '0');
   }
   else if (name == DECLAS_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecurityClass, property->valueToString(), 1);
   }
   else if (name == DESCLSY_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtClassSystem, property->valueToString(), 2);
   }
   else if (name == DESCODE_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtCodewords, property->valueToString(), 11);
   }
   else if (name == DESCTLH_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtControlAndHand, property->valueToString(), 2);
   }
   else if (name == DESREL_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtReleasingInst, property->valueToString(), 20);
   }
   else if (name == DESDCTP_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtDeclassType, property->valueToString(), 2);
   }
   else if (name == DESDCDT_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtDeclassDate, property->valueToString(), 8);
   }
   else if (name == DESDCXM_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtDeclassExempt, property->valueToString(), 4);
   }
   else if (name == DESDG_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecurityDowngrade, property->valueToString(), 1);
   }
   else if (name == DESDGDT_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecurityDowngradeDate, property->valueToString(), 8);
   }
   else if (name == DESCLTX_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecurityClassText, property->valueToString(), 43);
   }
   else if (name == DESCATP_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtClassAuthType, property->valueToString(), 1);
   }
   else if (name == DESCAUT_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtClassAuthority, property->valueToString(), 40);
   }
   else if (name == DESCRSN_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtClassReason, property->valueToString(), 1);
   }
   else if (name == DESSRDT_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecuritySourceDate, property->valueToString(), 8);
   }
   else if (name == DESCTLN_KW)
   {
      ossimNitfCommon::setField(theIdentSecurityChunk.theDataExtSecurityConNum, property->valueToString(), 15);
   }
   else if (name == DESOFLW_KW)
   {
      ossimNitfCommon::setField(theOverflowedHeaderType, property->valueToString(), 6);
   }
   else if (name == DESITEM_KW)
   {
      ossimNitfCommon::setField(theDataItemOverflowed, property->valueToString(), 3, ios::right, '0');
   }
   else if (name == DESSHL_KW)
   {
      ossimNitfCommon::setField(theLengthOfUserDefinedSubheaderFields, property->valueToString(), 4, ios::right, '0');
   }
   else if (name == DESSHF_KW)
   {
      ossimBinaryDataProperty* pBinaryData = PTR_CAST(ossimBinaryDataProperty, property.get());
      if (pBinaryData != NULL)
      {
         theUserDefinedSubheaderFields = pBinaryData->getBinaryData();
      }
   }
   else if (name == DESDATA_KW)
   {
      ossimBinaryDataProperty* pBinaryData = PTR_CAST(ossimBinaryDataProperty, property.get());
      if (pBinaryData != NULL)
      {
         theData = pBinaryData->getBinaryData();
      }
   }
}

ossimRefPtr<ossimProperty> ossimNitfDataExtensionSegmentV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;
   if (name == DE_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theFilePartType).trim());
   }
   else if (name == DESID_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId).trim());
   }
   else if (name == DESVER_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theVersionDataFieldDef).trim());
   }
   else if (name == DECLAS_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityClass).trim());
   }
   else if (name == DESCLSY_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtClassSystem).trim());
   }
   else if (name == DESCODE_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtCodewords).trim());
   }
   else if (name == DESCTLH_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtControlAndHand).trim());
   }
   else if (name == DESREL_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtReleasingInst).trim());
   }
   else if (name == DESDCTP_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtDeclassType).trim());
   }
   else if (name == DESDCDT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtDeclassDate).trim());
   }
   else if (name == DESDCXM_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtDeclassExempt).trim());
   }
   else if (name == DESDG_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityDowngrade).trim());
   }
   else if (name == DESDGDT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityDowngradeDate).trim());
   }
   else if (name == DESCLTX_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityClassText).trim());
   }
   else if (name == DESCATP_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtClassAuthType).trim());
   }
   else if (name == DESCAUT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtClassAuthority).trim());
   }
   else if (name == DESCRSN_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtClassReason).trim());
   }
   else if (name == DESSRDT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecuritySourceDate).trim());
   }
   else if (name == DESCTLN_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityConNum).trim());
   }
   else if (name == DESOFLW_KW)
   {
      property = new ossimStringProperty(name, ossimString(theOverflowedHeaderType).trim());
   }
   else if (name == DESITEM_KW)
   {
      property = new ossimStringProperty(name, ossimString(theDataItemOverflowed).trim());
   }
   else if (name == DESSHL_KW)
   {
      property = new ossimStringProperty(name, ossimString(theLengthOfUserDefinedSubheaderFields).trim());
   }
   else if (name == DESSHF_KW)
   {
      property = new ossimBinaryDataProperty(name, theUserDefinedSubheaderFields);
   }
   else if (name == DESDATA_KW)
   {
      // Only valid if setTagList has not been called on this object.
      property = new ossimBinaryDataProperty(name, theData);
   }

   return property;
}

void ossimNitfDataExtensionSegmentV2_1::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfDataExtensionSegment::getPropertyNames(propertyNames);
   propertyNames.push_back(DESID_KW);
   propertyNames.push_back(DESCLSY_KW);
   propertyNames.push_back(DESDCTP_KW);
   propertyNames.push_back(DESDCDT_KW);
   propertyNames.push_back(DESDCXM_KW);
   propertyNames.push_back(DESDG_KW);
   propertyNames.push_back(DESDGDT_KW);
   propertyNames.push_back(DESCLTX_KW);
   propertyNames.push_back(DESCATP_KW);
   propertyNames.push_back(DESCRSN_KW);
   propertyNames.push_back(DESSRDT_KW);
}
