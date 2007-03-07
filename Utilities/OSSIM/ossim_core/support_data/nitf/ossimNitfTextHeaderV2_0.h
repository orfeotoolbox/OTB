//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTextHeaderV2_0.h,v 1.2 2004/05/23 18:42:33 dburken Exp $

#ifndef ossimNitfTextHeaderV2_0_HEADER
#define ossimNitfTextHeaderV2_0_HEADER
#include "ossimNitfTextHeader.h"

class OSSIMDLLEXPORT ossimNitfTextHeaderV2_0 : public ossimNitfTextHeader
{
public:
   ossimNitfTextHeaderV2_0();
   virtual ~ossimNitfTextHeaderV2_0(){}
   
   virtual void parseStream(istream &in);
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
