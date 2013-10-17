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
// $Id: ossimNitfDataExtensionSegmentV2_0.cpp 22429 2013-10-02 12:53:06Z gpotts $

#include <iostream>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_0.h>

#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimBinaryDataProperty.h>
RTTI_DEF1(ossimNitfDataExtensionSegmentV2_0, "ossimNitfDataExtensionSegmentV2_0",ossimNitfDataExtensionSegment)

const ossimString ossimNitfDataExtensionSegmentV2_0::DESTAG_KW = "destag";
const ossimString ossimNitfDataExtensionSegmentV2_0::DESDWNG_KW = "desdwng";
const ossimString ossimNitfDataExtensionSegmentV2_0::DESDEVT_KW = "desdevt";

std::ostream& operator <<(std::ostream& out,
                          const ossimNitfDataExtIdentSecurityChunkV2_0& data)
{
   out << "theFilePartType:                " << data.theFilePartType
       << "\ntheUniqueDataExtTypeId:         " << data.theUniqueDataExtTypeId
       << "\ntheVersionDataFieldDef:         " << data.theVersionDataFieldDef
       << "\ntheDataExtSecurityClass:        " << data.theDataExtSecurityClass
       << "\ntheDataExtCodewords:            " << data.theDataExtCodewords
       << "\ntheDataExtControlAndHand:       " << data.theDataExtControlAndHand
       << "\ntheDataExtReleasingInst:        " << data.theDataExtReleasingInst
       << "\ntheDataExtClassAuthority:       " << data.theDataExtClassAuthority
       << "\ntheDataExtSecurityConNum:       " << data.theDataExtSecurityConNum
       << "\ntheDataExtSecurityDowngrade:     " << data.theDataExtSecurityDowngrade
       << "\ntheDataExtDowngradingEvent:     " << data.theDataExtDowngradingEvent
       << std::endl;
   
   return out;
}
   
ossimNitfDataExtensionSegmentV2_0::ossimNitfDataExtensionSegmentV2_0()
{
   clearFields();
}

ossimNitfDataExtensionSegmentV2_0::~ossimNitfDataExtensionSegmentV2_0()
{
}

void ossimNitfDataExtensionSegmentV2_0::parseStream(std::istream &in, ossim_uint64 dataLength)
{
   if(in)
   {
      clearFields();

      in.read(theIdentSecurityChunk.theFilePartType, 2); // 2 byte alpha
      in.read(theIdentSecurityChunk.theUniqueDataExtTypeId, 25); // 25 byte alpha numeric
      in.read(theIdentSecurityChunk.theVersionDataFieldDef, 2); // 2 byte numeric
      in.read(theIdentSecurityChunk.theDataExtSecurityClass, 1); // 1 byte alpha
      in.read(theIdentSecurityChunk.theDataExtCodewords, 40);    // 40 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtControlAndHand, 40); // 40 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtReleasingInst, 40); // 40 byte alpha numeric
      in.read(theIdentSecurityChunk.theDataExtClassAuthority, 20); // 20 byte alph num
      in.read(theIdentSecurityChunk.theDataExtSecurityConNum, 20); // 20 byte alpha num
      in.read(theIdentSecurityChunk.theDataExtSecurityDowngrade, 6); // 6 byte alpha num      
      if(ossimString(theIdentSecurityChunk.theDataExtSecurityDowngrade) == "999998")
      {
         in.read(theIdentSecurityChunk.theDataExtDowngradingEvent, 40); // 40 byte alpha num
      }

      ossimString desTag = ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId,
                                       theIdentSecurityChunk.theUniqueDataExtTypeId+25);
      desTag = desTag.trim().upcase();
      const bool overflow = (desTag == "REGISTERED EXTENSIONS" || 
                             desTag == "CONTROLLED EXTENSIONS");

      if (overflow&&in.good())
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
            istream::pos_type endData = dataBegin + static_cast<istream::pos_type>(dataLength);
            while (in.good()&&(in.tellg() < endData))
            {
               ossimNitfTagInformation tag;
               tag.parseStream(in);
               theTagList.push_back(tag);
            }
            if(!in.good()) in.clear();
            in.seekg(dataBegin);
         }
         theData.resize(dataLength);
         in.read(reinterpret_cast<char*>(&theData.front()), theData.size());
      }
   }
}

void ossimNitfDataExtensionSegmentV2_0::clearFields()
{

   memset(theIdentSecurityChunk.theFilePartType, ' ', 2); // 2 byte alpha
   memset(theIdentSecurityChunk.theUniqueDataExtTypeId, ' ', 25); // 25 byte alpha numeric
   memset(theIdentSecurityChunk.theVersionDataFieldDef, ' ', 2); // 2 byte numeric
   memset(theIdentSecurityChunk.theDataExtSecurityClass, ' ', 1); // 1 byte alpha
   memset(theIdentSecurityChunk.theDataExtCodewords, ' ', 40);    // 40 byte alpha numeric
   memset(theIdentSecurityChunk.theDataExtControlAndHand, ' ', 40); // 40 byte alpha numeric
   memset(theIdentSecurityChunk.theDataExtReleasingInst, ' ', 40); // 40 byte alpha numeric
   memset(theIdentSecurityChunk.theDataExtClassAuthority, ' ', 20); // 20 byte alph num
   memset(theIdentSecurityChunk.theDataExtSecurityConNum, ' ', 20); // 20 byte alpha num
   memset(theIdentSecurityChunk.theDataExtSecurityDowngrade, ' ', 6); // 6 byte alpha num
   memset(theIdentSecurityChunk.theDataExtDowngradingEvent, ' ', 40); // 40 byte alpha num
   memset(theOverflowedHeaderType, ' ', 6);
   memset(theDataItemOverflowed, ' ', 3);
   memset(theLengthOfUserDefinedSubheaderFields, ' ', 4);

   theIdentSecurityChunk.theFilePartType[2] = '\0'; // 2 byte alpha
   theIdentSecurityChunk.theUniqueDataExtTypeId[25] = '\0'; // 25 byte alpha numeric
   theIdentSecurityChunk.theVersionDataFieldDef[2] = '\0'; // 2 byte numeric
   theIdentSecurityChunk.theDataExtSecurityClass[1] = '\0'; // 1 byte alpha
   theIdentSecurityChunk.theDataExtCodewords[40] = '\0';    // 40 byte alpha numeric
   theIdentSecurityChunk.theDataExtControlAndHand[40] = '\0'; // 40 byte alpha numeric
   theIdentSecurityChunk.theDataExtReleasingInst[40] = '\0'; // 40 byte alpha numeric
   theIdentSecurityChunk.theDataExtClassAuthority[20] = '\0'; // 20 byte alph num
   theIdentSecurityChunk.theDataExtSecurityConNum[20] = '\0'; // 20 byte alpha num
   theIdentSecurityChunk.theDataExtSecurityDowngrade[6] = '\0'; // 6 byte alpha num
   theIdentSecurityChunk.theDataExtDowngradingEvent[40] = '\0'; // 40 byte alpha num
   theOverflowedHeaderType[6] = '\0';
   theDataItemOverflowed[3]   = '\0';
   theLengthOfUserDefinedSubheaderFields[4] = '\0';

   theUserDefinedSubheaderFields.clear();
   theData.clear();
   theTagList.clear();
}

  
std::ostream& ossimNitfDataExtensionSegmentV2_0::print(std::ostream& out)const
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

ossimRefPtr<ossimProperty> ossimNitfDataExtensionSegmentV2_0::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;
   if (name == DE_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theFilePartType).trim());
   }
   else if (name == DESTAG_KW)
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
   else if (name == DESCAUT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtClassAuthority).trim());
   }
   else if (name == DESCTLN_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityConNum).trim());
   }
   else if (name == DESDWNG_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtSecurityDowngrade).trim());
   }
   else if (name == DESDEVT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theIdentSecurityChunk.theDataExtDowngradingEvent).trim());
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

void ossimNitfDataExtensionSegmentV2_0::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfDataExtensionSegment::getPropertyNames(propertyNames);
   propertyNames.push_back(DESTAG_KW);
   propertyNames.push_back(DESDWNG_KW);
   propertyNames.push_back(DESDEVT_KW);
}
