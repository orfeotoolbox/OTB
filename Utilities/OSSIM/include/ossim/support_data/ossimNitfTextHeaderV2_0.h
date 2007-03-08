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
// $Id: ossimNitfTextHeaderV2_0.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimNitfTextHeaderV2_0_HEADER
#define ossimNitfTextHeaderV2_0_HEADER
#include <ossim/support_data/ossimNitfTextHeader.h>

class OSSIMDLLEXPORT ossimNitfTextHeaderV2_0 : public ossimNitfTextHeader
{
public:
   ossimNitfTextHeaderV2_0();
   virtual ~ossimNitfTextHeaderV2_0(){}
   
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream &out)const;

TYPE_DATA
private:
   void clearFields();

   /*!
    * This is a required 2 byte fields and has
    * a value of TE.
    */
   char theFilePartType[3];

   /*!
    * This is a required 10 byte field.
    */
   char theTextId[11];

   /*!
    * This is a required 14 byte field.  It has the
    * format of:
    *
    *  DDHHMMSSZMONYY
    *
    * DD   2 character day
    * HH   2 characters for the Hour
    * MM   2 characters for the minute
    * SS   2 characters for the seconds
    * Z    required
    * MON  3 characters of the month
    * YY   2 characters for the year
    */
   char theDataAndTime[15];

   /*!
    * is an optional 80 byte field
    */
   char theTextTitle[81];

   /*!
    * This is a required 1 byte field. Can have
    * of either:
    *
    * T   Top secret
    * S   Secret
    * C   Confidential
    * R   Restricted
    * U   Unclassified
    */
   char theTextSecurityClassification[2];

   /*!
    * This is an optional 40 byte field.
    */
   char theTextCodewords[41];

   /*!
    * This is an optional 40 yte field.
    */
   char theTextControlAndHandling[41];

   /*!
    * This is an optional 40 yte field.
    */
   char theTextReleasingInstructions[41];

   /*!
    * optional 20 byte field
    */
   char theTextClassificationAuthority[21];

   /*!
    * optional 20 byte field
    */
   char theTextSecurityControlNumber[21];

   /*!
    * optional 6 byte field
    */
   char theTextSecurityDowngrade[7];

   /*
    * This is a conditional 40 byte field.  if 
    * theSecurityDowngrade = 999998 then this field
    * exists.
    */
   char theTextSecurityDowngradeEvent[41];

   /*!
    * This is the encription type and is a
    * required 1 byte field.  It can have values
    *
    * 0  Not encrypted
    * 1  Encrypted
    */
   char theTextEncyption[2];

   /*!
    * This is a required 3 byte field and will hold
    * either:
    *
    * MTF  indicates USMTF ( refer to JCS PUB 6-04
    *      for examples
    * STA  indicates NITF ASCII
    * OTH  indicates other or user defined
    */
   char theTextFormat[4];

   /*!
    * is a required 5 byute field.
    */
   char theExtSubheaderDataLength[6];

   /*!
    * is a conditional 3 byte field.  It exists if
    * theExtSubheaderDataLength is not 0
    */
   char theExtSubheaderOverflow[4];
};

#endif
