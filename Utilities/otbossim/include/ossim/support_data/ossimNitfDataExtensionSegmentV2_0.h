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
// $Id: ossimNitfDataExtensionSegmentV2_0.h 22418 2013-09-26 15:01:12Z gpotts $
#ifndef ossimNitfDataExtensionSegmentV2_0_HEADER
#define ossimNitfDataExtensionSegmentV2_0_HEADER
#include <ossim/support_data/ossimNitfDataExtensionSegment.h>
#include <ossim/support_data/ossimNitfTagInformation.h>

struct ossimNitfDataExtIdentSecurityChunkV2_0
{
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfDataExtIdentSecurityChunkV2_0& data);
   char theFilePartType[3]; // 2 byte alpha
   char theUniqueDataExtTypeId[26]; // 25 byte alpha numeric
   char theVersionDataFieldDef[3]; // 2 byte numeric
   char theDataExtSecurityClass[2]; // 1 byte alpha
   char theDataExtCodewords[41];    // 40 byte alpha numeric
   char theDataExtControlAndHand[41]; // 40 byte alpha numeric
   char theDataExtReleasingInst[41]; // 40 byte alpha numeric
   char theDataExtClassAuthority[21]; // 20 byte alph num
   char theDataExtSecurityConNum[21]; // 20 byte alpha num
   char theDataExtSecurityDowngrade[7]; // 6 byte alpha num
   char theDataExtDowngradingEvent[41]; // 40 byte alpha num
};

class OSSIMDLLEXPORT ossimNitfDataExtensionSegmentV2_0 : public ossimNitfDataExtensionSegment
{
public:
   ossimNitfDataExtensionSegmentV2_0();
   virtual ~ossimNitfDataExtensionSegmentV2_0();
   virtual void parseStream(std::istream &in, ossim_uint64 dataLength);
   virtual std::ostream& print(std::ostream& out)const;
   virtual const std::vector<ossimNitfTagInformation>&  getTagList()const
      {
         return theTagList;
      }

   ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   static const ossimString DESTAG_KW;
   static const ossimString DESDWNG_KW;
   static const ossimString DESDEVT_KW;

private:
   void clearFields();
   
   ossimNitfDataExtIdentSecurityChunkV2_0 theIdentSecurityChunk;
   
   /*!
    * This is a 6 byte field
    */
   char theOverflowedHeaderType[7];

   /*!
    * This is a 3 byte field
    */
   char theDataItemOverflowed[4];

   /*!
    * This is a 4 byte field.
    */
   char theLengthOfUserDefinedSubheaderFields[5];

   /*!
    * This is an n-byte field, where n is theLengthOfUserDefinedSubheaderFields.
    */
   std::vector<unsigned char> theUserDefinedSubheaderFields;

   /*!
    * This is an n-byte field, where n is found in the file header.
    */
   std::vector<unsigned char> theData;

   std::vector<ossimNitfTagInformation> theTagList;

TYPE_DATA
};

#endif
