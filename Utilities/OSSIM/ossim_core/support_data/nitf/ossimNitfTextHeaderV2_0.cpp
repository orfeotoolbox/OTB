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
// $Id: ossimNitfTextHeaderV2_0.cpp,v 1.2 2004/05/23 18:42:33 dburken Exp $
#include "ossimNitfTextHeaderV2_0.h"
#include "base/data_types/ossimString.h"
#include <string.h> // for memset

RTTI_DEF1(ossimNitfTextHeaderV2_0, "ossimNitfTextHeaderV2_0", ossimNitfTextHeader)
   
ossimNitfTextHeaderV2_0::ossimNitfTextHeaderV2_0()
{
   clearFields();
}

void ossimNitfTextHeaderV2_0::parseStream(istream &in)
{
   if(in)
   {
      clearFields();
      
      in.read(theFilePartType, 2);
      in.read(theTextId, 10);
      in.read(theDataAndTime, 14);
      in.read(theTextTitle, 80);
      in.read(theTextSecurityClassification, 1);
      in.read(theTextCodewords, 40);
      in.read(theTextControlAndHandling, 40);
      in.read(theTextReleasingInstructions, 40);
      in.read(theTextClassificationAuthority, 20);
      in.read(theTextSecurityControlNumber, 20);
      in.read(theTextSecurityDowngrade, 6);
      if(ossimString(theTextSecurityDowngrade) == "999998")
      {
         in.read(theTextSecurityDowngradeEvent, 40);
      }
      in.read(theTextEncyption, 1);
      in.read(theTextFormat, 3);
      in.read(theExtSubheaderDataLength, 5);
      long dataLength = ossimString(theExtSubheaderDataLength).toLong();
      if(dataLength > 0)
      {
         in.read(theExtSubheaderOverflow, 3);

         // ignore the data for now
         in.ignore(dataLength - 3);
      }
   }
}

std::ostream& ossimNitfTextHeaderV2_0::print(std::ostream &out)const
{
   out << "theFilePartType:                 " << theFilePartType << endl
       << "theTextId:                       " << theTextId << endl
       << "theDataAndTime:                  " << theDataAndTime << endl
       << "theTextTitle:                    " << theTextTitle << endl
       << "theTextSecurityClassification:   " << theTextSecurityClassification << endl
       << "theTextCodewords:                " << theTextCodewords << endl
       << "theTextControlAndHandling:       " << theTextControlAndHandling << endl
       << "theTextReleasingInstructions:    " << theTextReleasingInstructions << endl
       << "theTextClassificationAuthority:  " << theTextClassificationAuthority << endl
       << "theTextSecurityControlNumber:    " << theTextSecurityControlNumber << endl
       << "theTextSecurityDowngrade:        " << theTextSecurityDowngrade << endl
       << "theTextSecurityDowngradeEvent:   " << theTextSecurityDowngradeEvent << endl
       << "theTextEncyption:                " << theTextEncyption << endl
       << "theTextFormat:                   " << theTextFormat << endl
       << "theExtSubheaderDataLength:       " << theExtSubheaderDataLength << endl
       << "theExtSubheaderOverflow:         " << theExtSubheaderOverflow;
   return out;
}

void ossimNitfTextHeaderV2_0::clearFields()
{
   
   memset(theFilePartType, ' ', 2);
   memset(theTextId, ' ', 10);
   memset(theDataAndTime, ' ', 14);
   memset(theTextTitle, ' ', 80);
   memset(theTextSecurityClassification, ' ', 1);
   memset(theTextCodewords, ' ', 40);
   memset(theTextControlAndHandling, ' ', 40);
   memset(theTextReleasingInstructions, ' ', 40);
   memset(theTextClassificationAuthority, ' ', 20);
   memset(theTextSecurityControlNumber, ' ', 20);
   memset(theTextSecurityDowngrade, ' ', 6);
   memset(theTextSecurityDowngradeEvent, ' ', 40);
   memset(theTextEncyption, ' ', 1);
   memset(theTextFormat, ' ', 3);
   memset(theExtSubheaderDataLength, ' ', 5);
   memset(theExtSubheaderOverflow, ' ', 3);

   theFilePartType[2] = '\0';
   theTextId[10] = '\0';
   theDataAndTime[14] = '\0';
   theTextTitle[80] = '\0';
   theTextSecurityClassification[1] = '\0';
   theTextCodewords[40] = '\0';
   theTextControlAndHandling[40] = '\0';
   theTextReleasingInstructions[40] = '\0';
   theTextClassificationAuthority[20] = '\0';
   theTextSecurityControlNumber[20] = '\0';
   theTextSecurityDowngrade[6] = '\0';
   theTextSecurityDowngradeEvent[40] = '\0';
   theTextEncyption[1] = '\0';
   theTextFormat[3] = '\0';
   theExtSubheaderDataLength[5] = '\0';
   theExtSubheaderOverflow[3] = '\0';
}
