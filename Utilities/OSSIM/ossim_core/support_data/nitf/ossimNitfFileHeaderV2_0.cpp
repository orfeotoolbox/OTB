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
// $Id: ossimNitfFileHeaderV2_0.cpp,v 1.9 2005/08/24 12:02:47 gpotts Exp $
#include <support_data/nitf/ossimNitfFileHeaderV2_0.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_0.h>
#include <support_data/nitf/ossimNitfSymbolHeaderV2_0.h>
#include <support_data/nitf/ossimNitfLabelHeaderV2_0.h>
#include <support_data/nitf/ossimNitfTextHeaderV2_0.h>
#include <support_data/nitf/ossimNitfTagFactoryRegistry.h>
#include <support_data/nitf/ossimNitfRegisteredTag.h>
#include <support_data/nitf/ossimNitfDataExtensionSegmentV2_0.h>

#include <cstring> // for memset
#include <base/data_types/ossimString.h>
#include <base/common/ossimTrace.h>


static const ossimTrace traceDebug("ossimNitfFileHeaderV2_0:debug");

RTTI_DEF1(ossimNitfFileHeaderV2_0, "ossimNitfFileHeaderV2_0", ossimNitfFileHeader)

ostream& operator <<(ostream& out,
                     const ossimNitfImageInfoRecordV2_0 &data)
{
   return out << "theImageSubheaderLength:       " << data.theImageSubheaderLength << endl
              << "theImageLength:                " << data.theImageLength;
}

ostream& operator <<(ostream& out,
                     const ossimNitfSymbolInfoRecordV2_0 &data)
{
   return out << "theSymbolSubheaderLength:       " << data.theSymbolSubheaderLength << endl
              << "theSymbolLength:                " << data.theSymbolLength;
}

ostream& operator <<(ostream& out,
                     const ossimNitfLabelInfoRecordV2_0 &data)
{
   return out << "theLabelSubheaderLength:       " << data.theLabelSubheaderLength << endl
              << "theLabelLength:                " << data.theLabelLength;
}

ostream& operator <<(ostream& out,
                     const ossimNitfTextInfoRecordV2_0 &data)
{
   return out << "theTextSubheaderLength:       " << data.theTextSubheaderLength << endl
              << "theTextLength:                " << data.theTextLength;
}

ostream& operator <<(ostream& out,
                     const ossimNitfDataExtSegInfoRecordV2_0 &data)
{
   return out << "theDataExtSegSubheaderLength:       " << data.theDataExtSegSubheaderLength << endl
              << "theDataExtSegLength:                " << data.theDataExtSegLength;
}

ostream& operator <<(ostream& out,
                     const ossimNitfResExtSegInfoRecordV2_0 &data)
{
   return out << "theResExtSegSubheaderLength:       " << data.theResExtSegSubheaderLength << endl
              << "theResExtSegLength:                " << data.theResExtSegLength;
}





ossimNitfFileHeaderV2_0::ossimNitfFileHeaderV2_0()
   :ossimNitfFileHeader()
{
   clearFields();
}

ossimNitfFileHeaderV2_0::~ossimNitfFileHeaderV2_0()
{
}


void ossimNitfFileHeaderV2_0::parseStream(std::istream &in)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "ossimNitfFileHeaderV2_0::parseStream:   entered ......." << std::endl;
   }
   
   clearFields();
   
   // identification and origination group
   in.read(theFileTypeVersion, 9);
   theHeaderSize+=9;
   in.read(theComplianceLevel, 2);
   theHeaderSize+=2;
   in.read(theSystemType, 4);
   theHeaderSize+=4;
   in.read(theOriginatingStationId, 10);
   theHeaderSize+=10;
   in.read(theDateTime, 14);
   theHeaderSize+=14;
   in.read(theFileTitle, 80);
   theHeaderSize+=80;
   
   // read security group
   in.read(theSecurityClassification, 1);
   theHeaderSize++;
   in.read(theCodewords, 40);
   theHeaderSize+=40;
   in.read(theControlAndHandling, 40);
   theHeaderSize+=40;
   in.read(theReleasingInstructions, 40);
   theHeaderSize+=40;
   in.read(theClassificationAuthority, 20);
   theHeaderSize+=20;
   in.read(theSecurityControlNumber, 20);
   theHeaderSize+=20;
   in.read(theSecurityDowngrade, 6);
   theHeaderSize+=6;
   if(ossimString(theSecurityDowngrade) == "999998")
   {
      in.read(theDowngradingEvent, 40);
      theHeaderSize+=40;
   }      
   in.read(theMessageCopyNumber, 5);
   theHeaderSize+=5;
   in.read(theNumberOfCopies, 5);
   theHeaderSize+=5;
   in.read(theEncryption, 1);
   theHeaderSize++;
   in.read(theOriginatorsName, 27);
   theHeaderSize+=27;
   in.read(theOriginatorsPhone, 18);
   theHeaderSize+=18;
   
   in.read(theNitfFileLength, 12);
   theHeaderSize+=12;
   in.read(theNitfHeaderLength, 6);
   theHeaderSize+=6;
   
   // image description group
   in.read(theNumberOfImageInfoRecords, 3);
   theHeaderSize+=3;
   readImageInfoRecords(in);

   // symbol description group
   in.read(theNumberOfSymbolInfoRecords, 3);
   theHeaderSize+=3;
   readSymbolInfoRecords(in);
   
   // label description group
   in.read(theNumberOfLabelInfoRecords, 3);
   theHeaderSize+=3;
   readLabelInfoRecords(in);
   
   // text file information group
   in.read(theNumberOfTextFileInfoRecords, 3);
   theHeaderSize+=3;
   readTextFileInfoRecords(in);
   
   // Data extension group
   in.read(theNumberOfDataExtSegInfoRecords, 3);
   theHeaderSize+=3;
   readDataExtSegInfoRecords(in);
   
   // Reserve Extension Segment group
   in.read(theNumberOfResExtSegInfoRecords, 3);
   theHeaderSize+=3;
   readResExtSegInfoRecords(in);
   
   in.read(theUserDefinedHeaderDataLength, 5);
   theHeaderSize+=5;
   
   theTagList.clear();
   // only get the header overflow if there even exists
   // user defined data.
   long userDefinedHeaderLength = ossimString(theUserDefinedHeaderDataLength).toLong();
   ossimNitfTagInformation         headerTag;
   
   std::streampos start   = in.tellg();
   std::streampos current = in.tellg();
   theHeaderSize+=userDefinedHeaderLength;
   if(userDefinedHeaderLength > 0)
   {
      in.read(theUserDefinedHeaderOverflow, 3);
      
      while((current - start) < userDefinedHeaderLength)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         in.ignore(headerTag.getTagLength());
         headerTag.clearFields();
         current = in.tellg();
      }
   }
   in.read(theExtendedHeaderDataLength, 5);
   theHeaderSize+=5;
   long extendedHeaderDataLength = ossimString(theExtendedHeaderDataLength).toLong();
   theHeaderSize+=extendedHeaderDataLength;
   
   start   = in.tellg();
   current = in.tellg();
   // for now let's just ignore it
   if(extendedHeaderDataLength > 0)
   {
      in.read(theExtendedHeaderOverflow, 3);
      
      while((current - start) < extendedHeaderDataLength)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         in.ignore(headerTag.getTagLength());
         headerTag.clearFields();
         current = in.tellg();
      }
   }
   
   // this need to be re-thought
   initializeAllOffsets();

   if(traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG)) << std::endl;
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< "ossimNitfFileHeaderV2_0::parseStream:   Leaving......." << std::endl;
   }
//      initializeDisplayLevels(in);

}

void ossimNitfFileHeaderV2_0::writeStream(std::ostream &out)
{
   
}

std::ostream& ossimNitfFileHeaderV2_0::print(std::ostream& out)const
{
   out << "theFileTypeVersion:         " << theFileTypeVersion << endl
       << "theComplianceLevel:         " << theComplianceLevel << endl
       << "theSystemType:              " << theSystemType      << endl
       << "theOriginatingStationId:    " << theOriginatingStationId << endl
       << "theDateTime:                " << theDateTime        << endl
       << "theFileTitle:               " << theFileTitle       << endl
       << "theSecurityClassification:  " << theSecurityClassification << endl
       << "theCodewords:               " << theCodewords << endl
       << "theControlAndHandling:      " << theControlAndHandling << endl
       << "theReleasingInstructions:   " << theReleasingInstructions << endl
       << "theClassificationAuthority: " << theClassificationAuthority << endl
       << "theSecurityControlNumber:   " << theSecurityControlNumber << endl
       << "theSecurityDowngrade:       " << theSecurityDowngrade << endl
       << "theDowngradingEvent:        " << theDowngradingEvent << endl
       << "theMessageCopyNumber:       " << theMessageCopyNumber << endl
       << "theNumberOfCopies:          " << theNumberOfCopies << endl
       << "theEncryption:              " << theEncryption << endl
       << "theOriginatorsName:         " << theOriginatorsName<< endl
       << "theOriginatorsPhone:        " << theOriginatorsPhone << endl
       << "theNitfFileLength:          " << theNitfFileLength   << endl
       << "theNitfHeaderLength:        " << theNitfHeaderLength << endl
       << "theNumberOfImageInfoRecords: " << theNumberOfImageInfoRecords << endl;

   copy(theNitfImageInfoRecords.begin(),
        theNitfImageInfoRecords.end(),
        ostream_iterator<ossimNitfImageInfoRecordV2_0>(out, "\n"));

   out << "theNumberOfSymbolInfoRecords: " << theNumberOfSymbolInfoRecords << endl;
   copy(theNitfSymbolInfoRecords.begin(),
        theNitfSymbolInfoRecords.end(),
        ostream_iterator<ossimNitfSymbolInfoRecordV2_0>(out, "\n"));

   out << "theNumberOfLabelInfoRecords: " << theNumberOfLabelInfoRecords << endl;
   copy(theNitfLabelInfoRecords.begin(),
        theNitfLabelInfoRecords.end(),
        ostream_iterator<ossimNitfLabelInfoRecordV2_0>(out, "\n"));

   out << "theNumberOfTextFileInfoRecords: " << theNumberOfTextFileInfoRecords << endl;
   copy(theNitfTextInfoRecords.begin(),
        theNitfTextInfoRecords.end(),
        ostream_iterator<ossimNitfTextInfoRecordV2_0>(out, "\n"));

   out << "theNumberOfDataExtSegInfoRecords: " << theNumberOfDataExtSegInfoRecords << endl;
   copy(theNitfDataExtSegInfoRecords.begin(),
        theNitfDataExtSegInfoRecords.end(),
        ostream_iterator<ossimNitfDataExtSegInfoRecordV2_0>(out, "\n"));

   out << "theNumberOfResExtSegInfoRecords: " << theNumberOfResExtSegInfoRecords << endl;
   copy(theNitfResExtSegInfoRecords.begin(),
        theNitfResExtSegInfoRecords.end(),
        ostream_iterator<ossimNitfResExtSegInfoRecordV2_0>(out, "\n"));

   out << "theUserDefinedHeaderDataLength:  " << theUserDefinedHeaderDataLength << endl
       << "theUserDefinedHeaderOverflow:    " << theUserDefinedHeaderOverflow << endl
       << "theExtendedHeaderDataLength:     " << theExtendedHeaderDataLength << endl;

   out << "TAGS:\n";
   std::copy(theTagList.begin(),
             theTagList.end(),
             std::ostream_iterator<ossimNitfTagInformation>(out,"\n"));
//   out << theHeaderTag;
   return out;
}

ossimDrect ossimNitfFileHeaderV2_0::getImageRect()const
{
   return theImageRect;
}

ossimNitfImageHeader*
ossimNitfFileHeaderV2_0::getNewImageHeader(long imageNumber,
                                           std::istream& in)const
{
   ossimNitfImageHeader *result = 0;
   
   if((getNumberOfImages() > 0) &&
      (imageNumber < (long)theImageOffsetList.size()) &&
      (imageNumber >= 0))
   {
      result = allocateImageHeader();
      in.seekg(theImageOffsetList[imageNumber].theImageHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "ossimNitfFileHeaderV2_0::getNewImageHeader ERROR:"
                                          << "\nNo images in file or image number (" << imageNumber
                                          << ") is out of range!\n";
   }
   
   return result;
}

ossimNitfSymbolHeader *ossimNitfFileHeaderV2_0::getNewSymbolHeader(long symbolNumber,
                                                                   std::istream& in)const
{
   ossimNitfSymbolHeader *result = 0;

   if((getNumberOfSymbols() > 0) &&
      (symbolNumber < (long)theSymbolOffsetList.size()) &&
      (symbolNumber >= 0))
   {
      result = allocateSymbolHeader();
      in.seekg(theSymbolOffsetList[symbolNumber].theSymbolHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_0::getNewLabelHeader(long labelNumber,
                                                                 std::istream& in)const
{
   ossimNitfLabelHeader *result = 0;

   if((getNumberOfLabels() > 0) &&
      (labelNumber < (long)theLabelOffsetList.size()) &&
      (labelNumber >= 0))
   {
      result = allocateLabelHeader();
      in.seekg(theLabelOffsetList[labelNumber].theLabelHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfTextHeader *ossimNitfFileHeaderV2_0::getNewTextHeader(long textNumber,
                                                               std::istream& in)const
{
   ossimNitfTextHeader *result = 0;

   if((getNumberOfTextSegments() > 0) &&
      (textNumber < (long)theTextOffsetList.size()) &&
      (textNumber >= 0))
   {
      result = allocateTextHeader();
      in.seekg(theTextOffsetList[textNumber].theTextHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfDataExtensionSegment* ossimNitfFileHeaderV2_0::getNewDataExtensionSegment(long dataExtNumber,
                                                                                   std::istream& in)const
{
   ossimNitfDataExtensionSegment *result = 0;

   if((getNumberOfDataExtSegments() > 0) &&
      (dataExtNumber < (long)theNitfDataExtSegInfoRecords.size()) &&
      (dataExtNumber >= 0))
   {
      result = allocateDataExtSegment();
      in.seekg(theDataExtSegOffsetList[dataExtNumber].theDataExtSegHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

void ossimNitfFileHeaderV2_0::initializeDisplayLevels(std::istream& in)
{
   // we will need to temporarily save the get pointer since
   // initializeDisplayLevels changes it.
   long saveTheGetPointer = in.tellg();
   
   vector<ossimNitfImageOffsetInformation>::iterator imageOffsetList = theImageOffsetList.begin();

   // allocate temporary space to store image headers
   ossimNitfImageHeader* imageHeader = allocateImageHeader();

   // clear the list
   theDisplayInformationList.clear();
   
   theImageRect = ossimDrect(0,0,0,0);
   if(!imageHeader)
   {
      return;
   }
   
   long index = 0;
   while(imageOffsetList != theImageOffsetList.end())
   {
      // position the get pointer in the input
      // stream to the start of the image header
      in.seekg((*imageOffsetList).theImageHeaderOffset, ios::beg);
      // get the data
      imageHeader->parseStream(in);
      // create a union of rects.  The result should be the image rect.
      ossimDrect tempRect = imageHeader->getImageRect();
      if((tempRect.width() > 1) &&
         (tempRect.height() > 1))
      {
         theImageRect = theImageRect.combine(tempRect);
      }
      
      insertIntoDisplayInfoList(ossimNitfDisplayInfo(ossimString("IM"),
                                                     imageHeader->getDisplayLevel(),
                                                     index));
      
      ++imageOffsetList;
      ++index;                                       
   }
   delete imageHeader;
   imageHeader = 0;


   
   // finally we reset the saved get state back
   // to its original position
   in.seekg(saveTheGetPointer, ios::beg);
}

void ossimNitfFileHeaderV2_0::insertIntoDisplayInfoList(const ossimNitfDisplayInfo &displayInformation)
{
   vector<ossimNitfDisplayInfo>::iterator displayList = theDisplayInformationList.begin();

   while(displayList != theDisplayInformationList.end())
   {
      if(displayInformation.theDisplayLevel < (*displayList).theDisplayLevel)
      {
         theDisplayInformationList.insert(displayList, displayInformation);
         return;
      }
      ++displayList;
   }

   // If we get here it means it's larger than all others
   // and we push onto the end
   theDisplayInformationList.push_back(displayInformation);
}

void ossimNitfFileHeaderV2_0::initializeAllOffsets()
{
   // this will be a running tally 
   long tally = theHeaderSize;
   long index = 0;

   // clear out all offset inforamtion and begin populating them
   theImageOffsetList.clear();
   theSymbolOffsetList.clear();
   theLabelOffsetList.clear();
   theLabelOffsetList.clear();
   

   for(index = 0; index < (long)theNitfImageInfoRecords.size(); index++)
   {
      theImageOffsetList.push_back(ossimNitfImageOffsetInformation(tally,
                                                                   tally + theNitfImageInfoRecords[index].getTotalLength()));
      tally += theNitfImageInfoRecords[index].getTotalLength();
   }
   for(index = 0; index < (long)theNitfSymbolInfoRecords.size(); index++)
   {
      theSymbolOffsetList.push_back(ossimNitfSymbolOffsetInformation(tally,
                                                                     tally + theNitfSymbolInfoRecords[index].getTotalLength()));
      tally += theNitfSymbolInfoRecords[index].getTotalLength();
   }

   for(index = 0; index < (long)theNitfLabelInfoRecords.size(); index++)
   {
      theLabelOffsetList.push_back(ossimNitfLabelOffsetInformation(tally,
                                                                   tally + theNitfLabelInfoRecords[index].getTotalLength()));
      tally += theNitfLabelInfoRecords[index].getTotalLength();
   }

   for(index = 0; index < (long)theNitfTextInfoRecords.size(); index++)
   {
      theTextOffsetList.push_back(ossimNitfTextOffsetInformation(tally,
                                                                 tally + theNitfTextInfoRecords[index].getTotalLength()));
      tally += theNitfTextInfoRecords[index].getTotalLength();
   }

   for(index = 0; index < (long)theNitfDataExtSegInfoRecords.size(); index++)
   {
      theDataExtSegOffsetList.push_back(ossimNitfDataExtSegOffsetInformation(tally,
                                                                             tally + theNitfDataExtSegInfoRecords[index].getTotalLength()));
      tally += theNitfDataExtSegInfoRecords[index].getTotalLength();
   }
}

ossimNitfImageHeader *ossimNitfFileHeaderV2_0::allocateImageHeader()const
{
   return new ossimNitfImageHeaderV2_0;
}

ossimNitfSymbolHeader *ossimNitfFileHeaderV2_0::allocateSymbolHeader()const
{
   return new ossimNitfSymbolHeaderV2_0;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_0::allocateLabelHeader()const
{
   return new ossimNitfLabelHeaderV2_0;
}

ossimNitfTextHeader *ossimNitfFileHeaderV2_0::allocateTextHeader()const
{
   return new ossimNitfTextHeaderV2_0;
}

ossimNitfDataExtensionSegment* ossimNitfFileHeaderV2_0::allocateDataExtSegment()const
{
   return new ossimNitfDataExtensionSegmentV2_0;
}

bool ossimNitfFileHeaderV2_0::isEncrypted()const
{
   return (theEncryption[0] == '1');
}

long ossimNitfFileHeaderV2_0::getNumberOfImages()const
{
   return theNitfImageInfoRecords.size();
}

long ossimNitfFileHeaderV2_0::getNumberOfLabels()const
{
   return (theNitfLabelInfoRecords.size());
}

long ossimNitfFileHeaderV2_0::getNumberOfSymbols()const
{
   return (theNitfSymbolInfoRecords.size());
}

long ossimNitfFileHeaderV2_0::getNumberOfGraphics()const
{
   return 0;
}

long ossimNitfFileHeaderV2_0::getNumberOfTextSegments()const
{
   return theNitfTextInfoRecords.size();
}

long ossimNitfFileHeaderV2_0::getNumberOfDataExtSegments()const
{
   return theNitfDataExtSegInfoRecords.size();
}

long ossimNitfFileHeaderV2_0::getHeaderSize()const
{
   return theHeaderSize;
}

long ossimNitfFileHeaderV2_0::getFileSize()const
{
   ossimString temp = theNitfFileLength;
   if(temp == "999999999999")
   {
      return -1;
   }
   else
   {
      return temp.toLong();
   }
}

const char* ossimNitfFileHeaderV2_0::getVersion()const
{
   return &theFileTypeVersion[4];
}

const char* ossimNitfFileHeaderV2_0::getDateTime()const
{
   return theDateTime;
}

ossimString ossimNitfFileHeaderV2_0::getSecurityClassification()const
{
   return theSecurityClassification;
}

void ossimNitfFileHeaderV2_0::clearFields()
{
   theDisplayInformationList.clear();
   theImageOffsetList.clear();
   theSymbolOffsetList.clear();
   theLabelOffsetList.clear();
   theTextOffsetList.clear();
   theDataExtSegOffsetList.clear();
   theNitfSymbolInfoRecords.clear();
   theNitfLabelInfoRecords.clear();
   theNitfTextInfoRecords.clear();
   theNitfDataExtSegInfoRecords.clear();
   theNitfResExtSegInfoRecords.clear();
   
   theFilename = "";
   memset(theFileTypeVersion, ' ', 9);
   memset(theComplianceLevel, ' ', 2);
   memset(theSystemType, ' ', 4);
   memset(theOriginatingStationId, ' ', 10);
   memset(theDateTime, ' ', 14);
   memset(theFileTitle, ' ', 80);
   memset(theSecurityClassification, ' ', 1);
   memset(theCodewords, ' ', 40);
   memset(theControlAndHandling, ' ', 40);
   memset(theReleasingInstructions, ' ', 40);
   memset(theClassificationAuthority, ' ', 20);
   memset(theSecurityControlNumber, ' ', 20);
   memset(theSecurityDowngrade, ' ', 6);
   memset(theDowngradingEvent, ' ', 40);
   memset(theMessageCopyNumber, ' ', 5);
   memset(theNumberOfCopies, ' ', 5);
   memset(theEncryption, ' ', 1);
   memset(theOriginatorsName, ' ', 27);
   memset(theOriginatorsPhone, ' ', 18);
   memset(theNitfFileLength, ' ', 12);
   memset(theNitfHeaderLength, ' ', 6);
   memset(theNumberOfImageInfoRecords, ' ', 3);
   memset(theNumberOfSymbolInfoRecords, ' ', 3);
   memset(theNumberOfLabelInfoRecords, ' ', 3);
   memset(theNumberOfTextFileInfoRecords, ' ', 3);
   memset(theNumberOfDataExtSegInfoRecords, ' ', 3);
   memset(theNumberOfResExtSegInfoRecords, ' ', 3);
   memset(theUserDefinedHeaderDataLength, ' ', 5);
   memset(theUserDefinedHeaderOverflow, ' ', 3);
   memset(theExtendedHeaderDataLength, ' ', 5);
   memset(theExtendedHeaderOverflow, ' ', 3);
   
   theFileTypeVersion[9] = '\0';
   theComplianceLevel[2] = '\0';
   theSystemType[4]      = '\0';
   theOriginatingStationId[10] = '\0';
   theDateTime[14]       = '\0';
   theFileTitle[80]      = '\0';
   theSecurityClassification[1] = '\0';
   theCodewords[40] = '\0';
   theControlAndHandling[40] = '\0';
   theReleasingInstructions[40] = '\0';
   theClassificationAuthority[20] = '\0';
   theSecurityControlNumber[20] = '\0';
   theSecurityDowngrade[6] = '\0';
   theDowngradingEvent[40] = '\0';
   theMessageCopyNumber[5] = '\0';
   theNumberOfCopies[5] = '\0';
   theEncryption[1] = '\0';
   theOriginatorsName[27] = '\0';
   theOriginatorsPhone[18] = '\0';
   theNitfFileLength[12]  = '\0';
   theNitfHeaderLength[6] = '\0';
   theNumberOfImageInfoRecords[3] = '\0';
   theNumberOfSymbolInfoRecords[3] = '\0';
   theNumberOfLabelInfoRecords[3] = '\0';
   theNumberOfTextFileInfoRecords[3] = '\0';
   theNumberOfDataExtSegInfoRecords[3] = '\0';
   theNumberOfResExtSegInfoRecords[3] = '\0';
   theUserDefinedHeaderDataLength[5] = '\0';
   theUserDefinedHeaderOverflow[3] = '\0';
   theExtendedHeaderDataLength[5] = '\0';
   theExtendedHeaderOverflow[3] = '\0';
   theHeaderSize = 0;
}

void ossimNitfFileHeaderV2_0::readImageInfoRecords(std::istream &in)
{
   long numberOfImages = ossimString(theNumberOfImageInfoRecords).toLong();
   long index;

   theNitfImageInfoRecords.clear();
   for(index=0; index < numberOfImages; index++)
   {
      ossimNitfImageInfoRecordV2_0 temp;
      
      in.read(temp.theImageSubheaderLength, 6);
      in.read(temp.theImageLength, 10);
      theHeaderSize+=16;
      temp.theImageSubheaderLength[6] = '\0';
      temp.theImageLength[10] = '\0';

      theNitfImageInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_0::readSymbolInfoRecords(std::istream &in)
{
   long numberOfSymbols = ossimString(theNumberOfSymbolInfoRecords).toLong();
   long index;

   theNitfSymbolInfoRecords.clear();
   
   for(index=0; index < numberOfSymbols; index++)
   {
      ossimNitfSymbolInfoRecordV2_0 temp;

      
      in.read(temp.theSymbolSubheaderLength, 4);
      in.read(temp.theSymbolLength, 6);
      theHeaderSize+=10;
      
      temp.theSymbolSubheaderLength[4] = '\0';
      temp.theSymbolLength[6] = '\0';
      
      theNitfSymbolInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_0::readLabelInfoRecords(std::istream &in)
{
   long numberOfLabels = ossimString(theNumberOfLabelInfoRecords).toLong();
   long index;

   theNitfLabelInfoRecords.clear();
   
   for(index=0; index < numberOfLabels; index++)
   {
      ossimNitfLabelInfoRecordV2_0 temp;
      
      in.read(temp.theLabelSubheaderLength, 4);
      in.read(temp.theLabelLength, 3);
      theHeaderSize+=7;
      temp.theLabelSubheaderLength[4] = '\0';
      temp.theLabelLength[3]          = '\0';

      theNitfLabelInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_0::readTextFileInfoRecords(std::istream &in)
{
   long numberOfTextFiles = ossimString(theNumberOfTextFileInfoRecords).toLong();
   long index;

   theNitfTextInfoRecords.clear();
   for(index=0; index < numberOfTextFiles; index++)
   {
      ossimNitfTextInfoRecordV2_0 temp;
      
      in.read(temp.theTextSubheaderLength, 4);
      in.read(temp.theTextLength, 5);
      theHeaderSize+=9;
      
      temp.theTextSubheaderLength[4] = '\0';
      temp.theTextLength[5] = '\0';
      
      theNitfTextInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_0::readDataExtSegInfoRecords(std::istream &in)
{
   long numberOfDataExtSegs = ossimString(theNumberOfDataExtSegInfoRecords).toLong();
   long index;

   theNitfDataExtSegInfoRecords.clear();
   for(index=0; index < numberOfDataExtSegs; index++)
   {
      ossimNitfDataExtSegInfoRecordV2_0 temp;
      
      in.read(temp.theDataExtSegSubheaderLength, 4);
      in.read(temp.theDataExtSegLength, 9);
      theHeaderSize+=13;
      
      temp.theDataExtSegSubheaderLength[4] = '\0';
      temp.theDataExtSegLength[9]          = '\0';

      theNitfDataExtSegInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_0::readResExtSegInfoRecords(std::istream &in)
{
   long numberOfResExtSegs = ossimString(theNumberOfResExtSegInfoRecords).toLong();
   long index;

   theNitfResExtSegInfoRecords.clear();
   for(index=0; index < numberOfResExtSegs; index++)
   {
      ossimNitfResExtSegInfoRecordV2_0 temp;

      in.read(temp.theResExtSegSubheaderLength, 4);
      in.read(temp.theResExtSegLength, 7);
      theHeaderSize+=11;
      
      temp.theResExtSegSubheaderLength[4] = '\0';
      temp.theResExtSegLength[7]          = '\0';
      
      theNitfResExtSegInfoRecords.push_back(temp);      
   }
}
