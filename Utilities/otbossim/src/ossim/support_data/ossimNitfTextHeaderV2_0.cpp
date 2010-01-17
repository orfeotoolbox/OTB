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
// $Id: ossimNitfTextHeaderV2_0.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/support_data/ossimNitfTextHeaderV2_0.h>
#include <ossim/base/ossimString.h>
#include <cstring> // for memset
#include <istream>

RTTI_DEF1(ossimNitfTextHeaderV2_0, "ossimNitfTextHeaderV2_0", ossimNitfTextHeader)
   
ossimNitfTextHeaderV2_0::ossimNitfTextHeaderV2_0()
{
   clearFields();
}

void ossimNitfTextHeaderV2_0::parseStream(std::istream &in)
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
   out << "theFilePartType:                 " << theFilePartType
       << "\ntheTextId:                       " << theTextId
       << "\ntheDataAndTime:                  " << theDataAndTime
       << "\ntheTextTitle:                    " << theTextTitle
       << "\ntheTextSecurityClassification:   " << theTextSecurityClassification
       << "\ntheTextCodewords:                " << theTextCodewords
       << "\ntheTextControlAndHandling:       " << theTextControlAndHandling
       << "\ntheTextReleasingInstructions:    " << theTextReleasingInstructions
       << "\ntheTextClassificationAuthority:  " << theTextClassificationAuthority
       << "\ntheTextSecurityControlNumber:    " << theTextSecurityControlNumber
       << "\ntheTextSecurityDowngrade:        " << theTextSecurityDowngrade
       << "\ntheTextSecurityDowngradeEvent:   " << theTextSecurityDowngradeEvent
       << "\ntheTextEncyption:                " << theTextEncyption
       << "\ntheTextFormat:                   " << theTextFormat
       << "\ntheExtSubheaderDataLength:       " << theExtSubheaderDataLength
       << "\ntheExtSubheaderOverflow:         " << theExtSubheaderOverflow
       << std::endl;
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
