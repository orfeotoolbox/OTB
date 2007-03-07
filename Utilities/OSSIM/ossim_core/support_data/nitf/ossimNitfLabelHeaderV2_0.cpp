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
// $Id: ossimNitfLabelHeaderV2_0.cpp,v 1.3 2004/05/23 18:42:33 dburken Exp $
#include <support_data/nitf/ossimNitfLabelHeaderV2_0.h>
#include <base/data_types/ossimString.h>
#include <string.h> // for memset

RTTI_DEF1(ossimNitfLabelHeaderV2_0, "ossimNitfLabelHeaderV2_0", ossimNitfLabelHeader)

ossimNitfLabelHeaderV2_0::ossimNitfLabelHeaderV2_0()
{
   clearFields();
}

void ossimNitfLabelHeaderV2_0::parseStream(istream &in)
{
   if(in)
   {
      clearFields();
      
      in.read(theFilePartType, 2);
      in.read(theLabelId, 10);
      in.read(theLabelSecurityClassification, 1);
      in.read(theLabelCodewords, 40);
      in.read(theLabelControlAndHandling, 40);
      in.read(theLabelReleasingInstructions, 40);
      in.read(theLabelClassificationAuthority, 20);
      in.read(theLabelSecurityControlNumber, 20);
      in.read(theLabelSecurityDowngrade, 6);
      if(ossimString(theLabelSecurityDowngrade) == "999998")
      {
         in.read(theLabelDowngradingEvent, 40);
      }
      in.read(theLabelEncryption, 1);
      in.read(theLabelFontStyle, 1);
      in.read(theLabelCellWidth, 2);
      in.read(theLabelCellHeight, 2);
      in.read(theLabelDisplayLevel, 3);
      in.read(theLabelAttachmentLevel, 3);
      in.read(theLabelLocation, 10);
      in.read(theLabelTextColor, 3);
      in.read(theLabelBackgroundColor, 3);
      in.read(theExtendedHeaderDataLength, 5);
      
      long dataLength = ossimString(theExtendedHeaderDataLength).toLong();
      if(dataLength > 0)
      {
         in.read(theExtendedSubheaderOverflow, 3);

         // for now just ignore the extended header data
         in.ignore(dataLength-3);
      }
   }
}

std::ostream& ossimNitfLabelHeaderV2_0::print(std::ostream &out)const
{
   out << "theFilePartType:                  " << theFilePartType << std::endl
       << "theLabelId:                       " << theLabelId << std::endl
       << "theLabelSecurityClassification:   " << theLabelSecurityClassification << std::endl
       << "theLabelCodewords:                " << theLabelCodewords << std::endl
       << "theLabelControlAndHandling:       " << theLabelControlAndHandling << std::endl
       << "theLabelReleasingInstructions:    " << theLabelReleasingInstructions << std::endl
       << "theLabelClassificationAuthority:  " << theLabelClassificationAuthority << std::endl
       << "theLabelSecurityControlNumber:    " << theLabelSecurityControlNumber << std::endl
       << "theLabelSecurityDowngrade:        " << theLabelSecurityDowngrade << std::endl
       << "theLabelEncryption:               " << theLabelEncryption << std::endl
       << "theLabelFontStyle:                " << theLabelFontStyle << std::endl
       << "theLabelCellWidth:                " << theLabelCellWidth << std::endl
       << "theLabelCellHeight:               " << theLabelCellHeight << std::endl
       << "theLabelDisplayLevel:             " << theLabelDisplayLevel << std::endl
       << "theLabelAttachmentLevel:          " << theLabelAttachmentLevel << std::endl
       << "theLabelLocation:                 " << theLabelLocation << std::endl
       << "theLabelTextColor:                " << theLabelTextColor << std::endl
       << "theLabelBackgroundColor:          " << theLabelBackgroundColor << std::endl
       << "theExtendedHeaderDataLength:      " << theExtendedHeaderDataLength << std::endl
       << "theExtendedSubheaderOverflow:     " << theExtendedSubheaderOverflow;
   return out;
}

long ossimNitfLabelHeaderV2_0::getDisplayLevel()const
{
   return ossimString(theLabelDisplayLevel).toLong();
}

ossimDrect ossimNitfLabelHeaderV2_0::getImageRect()const
{
   double ulx = ossimString((const char*)(&theLabelLocation[5]),
                            (const char*)(&theLabelLocation[10])).toDouble();
   double uly = ossimString((const char*)theLabelLocation,
                            (const char*)(&theLabelLocation[5])).toDouble();

   return ossimDrect(ulx,
                     uly,
                     ulx + ossimString(theLabelCellWidth).toDouble(),
                     uly + ossimString(theLabelCellHeight).toDouble());
}

void ossimNitfLabelHeaderV2_0::clearFields()
{
   memset(theFilePartType, ' ', 2);
   memset(theLabelId, ' ', 10);
   memset(theLabelSecurityClassification, ' ', 1);
   memset(theLabelCodewords, ' ', 40);
   memset(theLabelControlAndHandling, ' ', 40);
   memset(theLabelReleasingInstructions, ' ', 40);
   memset(theLabelClassificationAuthority, ' ', 20);
   memset(theLabelSecurityControlNumber, ' ', 20);
   memset(theLabelSecurityDowngrade, ' ', 6);
   memset(theLabelDowngradingEvent, ' ', 40);
   memset(theLabelEncryption, ' ', 1);
   memset(theLabelFontStyle, ' ', 1);
   memset(theLabelCellWidth, ' ', 2);
   memset(theLabelCellHeight, ' ', 2);
   memset(theLabelDisplayLevel, ' ', 3);
   memset(theLabelAttachmentLevel, ' ', 3);
   memset(theLabelLocation, ' ', 10);
   memset(theLabelTextColor, ' ', 3);
   memset(theLabelBackgroundColor, ' ', 3);
   memset(theExtendedHeaderDataLength, ' ', 5);
   memset(theExtendedSubheaderOverflow, ' ', 3);

   theFilePartType[2] = '\0';
   theLabelId[10] = '\0';
   theLabelSecurityClassification[1] = '\0';
   theLabelCodewords[40] = '\0';
   theLabelControlAndHandling[40] = '\0';
   theLabelReleasingInstructions[40] = '\0';
   theLabelClassificationAuthority[20] = '\0';
   theLabelSecurityControlNumber[20] = '\0';
   theLabelSecurityDowngrade[6] = '\0';
   theLabelDowngradingEvent[40] = '\0';
   theLabelEncryption[1] = '\0';
   theLabelFontStyle[1] = '\0';
   theLabelCellWidth[2] = '\0';
   theLabelCellHeight[2] = '\0';
   theLabelDisplayLevel[3] = '\0';
   theLabelAttachmentLevel[3] = '\0';
   theLabelLocation[10] = '\0';
   theLabelTextColor[3] = '\0';
   theLabelBackgroundColor[3] = '\0';
   theExtendedHeaderDataLength[5] = '\0';
   theExtendedSubheaderOverflow[3] = '\0';
}
