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
// $Id: ossimNitfDataExtensionSegmentV2_1.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfDataExtensionSegmentV2_1_HEADER
#define ossimNitfDataExtensionSegmentV2_1_HEADER
#include <ossim/support_data/ossimNitfDataExtensionSegment.h>
#include <ossim/support_data/ossimNitfTagInformation.h>

class ossimNitfFileHeaderV2_1;

struct ossimNitfDataExtIdentSecurityChunkV2_1
{
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNitfDataExtIdentSecurityChunkV2_1& data);
   char theFilePartType[3]; // 2 byte alpha
   char theUniqueDataExtTypeId[26]; // 25 byte alpha numeric
   char theVersionDataFieldDef[3]; // 2 byte numeric
   char theDataExtSecurityClass[2]; // 1 byte alpha
   char theDataExtClassSystem[3];
   char theDataExtCodewords[12];    // 11 byte alpha numeric
   char theDataExtControlAndHand[3]; // 2 byte alpha numeric
   char theDataExtReleasingInst[21]; // 20 byte alpha numeric
   char theDataExtDeclassType[3];
   char theDataExtDeclassDate[9];
   char theDataExtDeclassExempt[5];
   char theDataExtSecurityDowngrade[2]; // 1 byte alpha
   char theDataExtSecurityDowngradeDate[9];
   char theDataExtSecurityClassText[44];
   char theDataExtClassAuthType[2];
   char theDataExtClassAuthority[41]; // 40 byte alph num
   char theDataExtClassReason[2];
   char theDataExtSecuritySourceDate[9];
   char theDataExtSecurityConNum[16]; // 20 byte alpha num
};

class OSSIMDLLEXPORT ossimNitfDataExtensionSegmentV2_1 : public ossimNitfDataExtensionSegment
{
public:
   ossimNitfDataExtensionSegmentV2_1();
   virtual ~ossimNitfDataExtensionSegmentV2_1();
   virtual void parseStream(std::istream &in, ossim_uint64 dataLength);
   virtual void writeStream(std::ostream &out);
   virtual std::ostream& print(std::ostream& out)const;
   virtual const std::vector<ossimNitfTagInformation>&  getTagList()const
      {
         return theTagList;
      }

   virtual void setTagList(const std::vector<ossimNitfTagInformation> &tagList);

   virtual void setSecurityMarkings(const ossimNitfFileHeaderV2_1 &fileHeader);

   virtual ossim_uint32 getHeaderLength() const;
   virtual ossim_uint64 getDataLength() const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   static const ossimString DESID_KW;
   static const ossimString DESCLSY_KW;
   static const ossimString DESDCTP_KW;
   static const ossimString DESDCDT_KW;
   static const ossimString DESDCXM_KW;
   static const ossimString DESDG_KW;
   static const ossimString DESDGDT_KW;
   static const ossimString DESCLTX_KW;
   static const ossimString DESCATP_KW;
   static const ossimString DESCRSN_KW;
   static const ossimString DESSRDT_KW;

private:
   void clearFields();
   
   ossimNitfDataExtIdentSecurityChunkV2_1 theIdentSecurityChunk;
   
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
