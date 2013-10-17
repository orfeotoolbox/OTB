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
// $Id: ossimNitfTagInformation.h 22418 2013-09-26 15:01:12Z gpotts $
#ifndef ossimNitfTagInformation_HEADER
#define ossimNitfTagInformation_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class ossimString;

class OSSIMDLLEXPORT ossimNitfTagInformation : public ossimObject
{
public:
   ossimNitfTagInformation(ossimRefPtr<ossimNitfRegisteredTag> tagData = 0);
   virtual ~ossimNitfTagInformation();
   
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   void setTagName(const ossimString& tagName);
   void setTagLength(ossim_uint32 tagLength);

   /**
    * Length of the 5 byte tag length the 6 byte tag name and
    * the data length.
    * So we have Data length + 11 bytes.
    */
   ossim_uint32 getTotalTagLength()const;

   /**
    * Should return the value of theTagLength which is the length of
    * the data in bytes.
    */
   ossim_uint32 getTagLength()const;
   ossim_uint64 getTagOffset()const;
   ossim_uint64 getTagDataOffset()const;
   
   ossimString   getTagName()const;
   virtual std::ostream& print(std::ostream& out)const;
   void clearFields();
   
   ossimRefPtr<ossimNitfRegisteredTag> getTagData();
   const ossimRefPtr<ossimNitfRegisteredTag> getTagData()const;
   void setTagData(ossimRefPtr<ossimNitfRegisteredTag> tagData);
   ossimString getTagType() const;
   void setTagType(const ossimString& tagType) const;

   bool operator<(const ossimNitfTagInformation& rhs) const
   {
      return getTotalTagLength() < rhs.getTotalTagLength();
   }
   
private:

   /**
    * This is a 6 byte field
    */
   char          theTagName[7];

   /**
    * This is a 5 byte field
    */
   char           theTagLength[6];

   /**
    * This is a 6 byte field which should be UDHD, UDID, XHD, IXSHD, SXSHD, or TXSHD.
    * This is declared as mutable since changing the tag type doesn't change the contents of the tag.
    */
   mutable char theTagType[7];

   /**
    * This will hold the offset to the start of the above information
    * This is just a work variable.
    */
   ossim_uint64 theTagOffset;

   /**
    * This will hold the start to the data.  This is just the
    * position in the file just past the tag length field.
    * This is just a work variable
    */
   ossim_uint64 theTagDataOffset;

   /**
    * Used to hold the tag data.
    */
   ossimRefPtr<ossimNitfRegisteredTag> theTagData;
};

#endif
