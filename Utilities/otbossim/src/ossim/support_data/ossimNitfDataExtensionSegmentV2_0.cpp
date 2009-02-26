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
// $Id: ossimNitfDataExtensionSegmentV2_0.cpp 13218 2008-07-23 19:22:37Z dburken $

#include <iostream>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_0.h>


RTTI_DEF1(ossimNitfDataExtensionSegmentV2_0, "ossimNitfDataExtensionSegmentV2_0",ossimNitfDataExtensionSegment)

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
       << "\ntheDataExtSecuityDowngrade:     " << data.theDataExtSecuityDowngrade
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

void ossimNitfDataExtensionSegmentV2_0::parseStream(std::istream &in)
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
      in.read(theIdentSecurityChunk.theDataExtSecuityDowngrade, 6); // 6 byte alpha num      
      if(ossimString(theIdentSecurityChunk.theDataExtSecuityDowngrade) == "999998")
      {
         in.read(theIdentSecurityChunk.theDataExtDowngradingEvent, 40); // 40 byte alpha num
      }
      in.read(theOverflowedHeaderType, 6);
      in.read(theDataItemOverflowed, 3);
      in.read(theLengthOfUserDefinedSubheaderFields, 4);
      // look to see if there is any data and if so read the tag
      // to see what it is
      long length = ossimString(theLengthOfUserDefinedSubheaderFields).toLong();
      
      if(length > 0)
      {
         in.ignore(length);
      }
      // currently I think we are looking for registered extensions keyword in
      // the theUniqueDataExtTypeId field
      if(ossimString(theIdentSecurityChunk.theUniqueDataExtTypeId).trim().upcase() == "REGISTERED EXTENSIONS")
      {
         theTag.clearFields();
         theTag.parseStream(in);
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
   memset(theIdentSecurityChunk.theDataExtSecuityDowngrade, ' ', 6); // 6 byte alpha num
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
   theIdentSecurityChunk.theDataExtSecuityDowngrade[6] = '\0'; // 6 byte alpha num
   theIdentSecurityChunk.theDataExtDowngradingEvent[40] = '\0'; // 40 byte alpha num
   theOverflowedHeaderType[6] = '\0';
   theDataItemOverflowed[3]   = '\0';
   theLengthOfUserDefinedSubheaderFields[4] = '\0';
   
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
       << theLengthOfUserDefinedSubheaderFields << std::endl
       << theTag;
   return out;
}
