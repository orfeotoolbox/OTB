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
// $Id: ossimNitfSymbolHeaderV2_0.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/support_data/ossimNitfSymbolHeaderV2_0.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDrect.h>
#include <cstring> // for memset
#include <istream>

RTTI_DEF1(ossimNitfSymbolHeaderV2_0, "ossimNitfSymbolHeaderV2_0", ossimNitfSymbolHeader)

ossimNitfSymbolHeaderV2_0::ossimNitfSymbolHeaderV2_0()
{
   clearFields();
}

void ossimNitfSymbolHeaderV2_0::parseStream(std::istream &in)
{
   if(in)
   {
      clearFields();
      
      in.read(theFilePartType, 2); 
      in.read(theSymbolId, 10);    
      in.read(theSymbolName, 20); 
      in.read(theSymbolSecurityClass, 1);
      in.read(theSymbolCodewords, 40);
      in.read(theSymbolControlAndHandling, 40);
      in.read(theSymbolReleasingInstructions, 40);
      in.read(theSymbolClassificationAuthority, 20);
      in.read(theSymbolSecurityControlNum, 20);
      in.read(theSymbolSecurityDowngrade, 6);
      if(ossimString(theSymbolSecurityDowngrade) == "999998")
      {
         in.read(theSymbolDowngradingEvent, 40);
      }
      in.read(theSymbolEncryption, 1);
      in.read(theSymbolType, 1);
      in.read(theNumberLinesPerSymbol, 4);
      in.read(theSymbolNumberPixelsPerLine, 4);
      in.read(theSymbolLineWidth, 4);
      in.read(theSymbolNumberBitsPerPixel, 1);
      in.read(theSymbolDisplayLevel, 3);
      in.read(theSymbolAttachmentLevel, 3);
      in.read(theSymbolLocation, 10);
      in.read(theSecondSymbolLocation, 10);
      in.read(theSymbolColor, 1);
      in.read(theSymbolNumber, 6);
      in.read(theSymbolRotation, 3);
      in.read(theSymbolNumberOfLutEntries, 3);

      // do lut here.  we will temporarily ignore
      in.ignore(ossimString(theSymbolNumberOfLutEntries).toLong());
      
      in.read(theSymbolExtendedSubheaderDataLength, 5);
      in.read(theSymbolSubheaderOverflow, 3);
      
   }
}

std::ostream& ossimNitfSymbolHeaderV2_0::print(std::ostream &out)const
{
   
   out << "theFilePartType:                   " << theFilePartType << std::endl
       << "theSymbolId:                       " << theSymbolId << std::endl
       << "theSymbolName:                     " << theSymbolName << std::endl
       << "theSymbolSecurityClass:            " << theSymbolSecurityClass << std::endl
       << "theSymbolCodewords:                " << theSymbolCodewords << std::endl
       << "theSymbolControlAndHandling:       " << theSymbolControlAndHandling << std::endl
       << "theSymbolReleasingInstructions:    " << theSymbolReleasingInstructions << std::endl
       << "theSymbolClassificationAuthority:  " << theSymbolClassificationAuthority << std::endl
       << "theSymbolSecurityControlNum:       " << theSymbolSecurityControlNum << std::endl
       << "theSymbolSecurityDowngrade:        " << theSymbolSecurityDowngrade << std::endl
       << "theSymbolDowngradingEvent:         " << theSymbolDowngradingEvent << std::endl
       << "theSymbolEncryption:               " << theSymbolEncryption << std::endl
       << "theSymbolType:                     " << theSymbolType << std::endl
       << "theNumberLinesPerSymbol:           " << theNumberLinesPerSymbol << std::endl
       << "theSymbolNumberPixelsPerLine:      " << theSymbolNumberPixelsPerLine << std::endl
       << "theSymbolLineWidth:                " << theSymbolLineWidth << std::endl
       << "theSymbolNumberBitsPerPixel:       " << theSymbolNumberBitsPerPixel << std::endl
       << "theSymbolDisplayLevel:             " << theSymbolDisplayLevel << std::endl
       << "theSymbolAttachmentLevel:          " << theSymbolAttachmentLevel << std::endl
       << "theSymbolLocation:                 " << theSymbolLocation << std::endl
       << "theSecondSymbolLocation:           " << theSecondSymbolLocation << std::endl
       << "theSymbolColor:                    " << theSymbolColor << std::endl
       << "theSymbolNumber:                   " << theSymbolNumber << std::endl
       << "theSymbolRotation:                 " << theSymbolRotation << std::endl
       << "theSymbolNumberOfLutEntries:       " << theSymbolNumberOfLutEntries << std::endl
       << "theSymbolExtendedSubheaderDataLength:  " << theSymbolExtendedSubheaderDataLength << std::endl
       << "theSymbolSubheaderOverflow:        " << theSymbolSubheaderOverflow << std::endl;
   return out;
}

ossim_int32 ossimNitfSymbolHeaderV2_0::getDisplayLevel() const
{
   return ossimString(theSymbolDisplayLevel).toInt32();
}

ossimDrect ossimNitfSymbolHeaderV2_0::getImageRect()const
{
   // for now we will return an empty rect.  This will be more
   // complicated to compute a symbol bound.  The origin depends
   // on the symbol and is different for different symbols.
   // We will implement this one last.
   return ossimDrect(0,0,0,0);
}


void ossimNitfSymbolHeaderV2_0::clearFields()
{   
   memset(theFilePartType, ' ', 2);
   memset(theSymbolId, ' ', 10);
   memset(theSymbolName, ' ', 20);
   memset(theSymbolSecurityClass, ' ', 1);
   memset(theSymbolCodewords, ' ', 40);
   memset(theSymbolControlAndHandling, ' ', 40);
   memset(theSymbolReleasingInstructions, ' ', 40);
   memset(theSymbolClassificationAuthority, ' ', 20);
   memset(theSymbolSecurityControlNum, ' ', 20);
   memset(theSymbolSecurityDowngrade, ' ', 6);
   memset(theSymbolDowngradingEvent, ' ', 40);
   memset(theSymbolEncryption, ' ', 1);
   memset(theSymbolType, ' ', 1);
   memset(theNumberLinesPerSymbol, ' ', 4);
   memset(theSymbolNumberPixelsPerLine, ' ', 4);
   memset(theSymbolLineWidth, ' ', 4);
   memset(theSymbolNumberBitsPerPixel, ' ', 1);
   memset(theSymbolDisplayLevel, ' ', 3);
   memset(theSymbolAttachmentLevel, ' ', 3);
   memset(theSymbolLocation, ' ', 10);
   memset(theSecondSymbolLocation, ' ', 10);
   memset(theSymbolColor, ' ', 1);
   memset(theSymbolNumber, ' ', 6);
   memset(theSymbolRotation, ' ', 3);
   memset(theSymbolNumberOfLutEntries, ' ', 3);
   memset(theSymbolExtendedSubheaderDataLength, ' ', 5);
   memset(theSymbolSubheaderOverflow, ' ', 3);


   theFilePartType[2]                  = '\0';
   theSymbolId[10]                     = '\0';
   theSymbolName[20]                   = '\0';
   theSymbolSecurityClass[1]           = '\0';
   theSymbolCodewords[40]              = '\0';
   theSymbolControlAndHandling[40]     = '\0';
   theSymbolReleasingInstructions[40]  = '\0';
   theSymbolClassificationAuthority[20] = '\0';
   theSymbolSecurityControlNum[20]     = '\0';
   theSymbolSecurityDowngrade[6]       = '\0';
   theSymbolDowngradingEvent[40]       = '\0';
   theSymbolEncryption[1]              = '\0';
   theSymbolType[1]                    = '\0';
   theNumberLinesPerSymbol[4]          = '\0';
   theSymbolNumberPixelsPerLine[4]     = '\0';
   theSymbolLineWidth[4]               = '\0';
   theSymbolNumberBitsPerPixel[1]      = '\0';
   theSymbolDisplayLevel[3]            = '\0';
   theSymbolAttachmentLevel[3]         = '\0';
   theSymbolLocation[10]               = '\0';
   theSecondSymbolLocation[10]         = '\0';
   theSymbolColor[1]                   = '\0';
   theSymbolNumber[6]                  = '\0';
   theSymbolRotation[3]                = '\0';
   theSymbolNumberOfLutEntries[3]      = '\0';
   theSymbolExtendedSubheaderDataLength[5] = '\0';
   theSymbolSubheaderOverflow[3]       ='\0';
}
