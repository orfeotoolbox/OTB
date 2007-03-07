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
// $Id: ossimNitfDataExtensionSegmentV2_0.h,v 1.2 2004/05/23 18:42:33 dburken Exp $
#ifndef ossimNitfDataExtensionSegmentV2_0_HEADER
#define ossimNitfDataExtensionSegmentV2_0_HEADER
#include "ossimNitfDataExtensionSegment.h"
#include "ossimNitfTagInformation.h"

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
   char theDataExtSecuityDowngrade[7]; // 6 byte alpha num
   char theDataExtDowngradingEvent[41]; // 40 byte alpha num
};

class OSSIMDLLEXPORT ossimNitfDataExtensionSegmentV2_0 : public ossimNitfDataExtensionSegment
{
public:
   ossimNitfDataExtensionSegmentV2_0();
   virtual ~ossimNitfDataExtensionSegmentV2_0();
   virtual void parseStream(istream &in);
   virtual std::ostream& print(std::ostream& out)const;
   virtual const ossimNitfTagInformation&  getTagInformation()const
      {
         return theTag;
      }

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
   
   ossimNitfTagInformation                theTag;

TYPE_DATA
};

#endif
