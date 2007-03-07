//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeaderV2_1.cpp,v 1.22 2005/11/17 18:12:23 dburken Exp $

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring> // for memset

#include <support_data/nitf/ossimNitfFileHeaderV2_1.h>
#include <base/data_types/ossimString.h>
#include <base/property/ossimColorProperty.h>
#include <base/property/ossimDateProperty.h>
#include <base/property/ossimStringProperty.h>
#include <base/property/ossimNumericProperty.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <support_data/nitf/ossimNitfImageHeaderV2_1.h>

#ifndef NULL
#  include <cstddef>
#endif

RTTI_DEF1(ossimNitfFileHeaderV2_1,
          "ossimNitfFileHeaderV2_1",
          ossimNitfFileHeader)

static const char* CLEVEL_KW  = "clevel";
static const char* STYPE_KW   = "stype";
static const char* OSTAID_KW  = "ostaid";
static const char* FDT_KW     = "fdt";  
static const char* FTITLE_KW  = "ftitle";
static const char* FSCLAS_KW  = "fsclas";
static const char* FSCLASY_KW = "fsclasy";
static const char* FSCODE_KW  = "fscode";
static const char* FSCTLH_KW  = "fsctlh";
static const char* FSREL_KW   = "fsrel";
static const char* FSDCTP_KW  = "fsdctp";
static const char* FSDCDT_KW  = "fsdcdt";
static const char* FSDCXM_KW  = "fsdcxm";
static const char* FSDG_KW    = "fsdg";
static const char* FSDGDT_KW  = "fsdgdt";
static const char* FSCLTX_KW  = "fscltx";
static const char* FSCATP_KW  = "fscatp";
static const char* FSCAUT_KW  = "fscaut";
static const char* FSCRSN_KW  = "fscrsn";
static const char* FSSRDT_KW  = "fssrdt";
static const char* FSCTLN_KW  = "fsctln";
static const char* FSCOP_KW   = "fscop";
static const char* FSCPYS_KW  = "fscpys";
static const char* ENCRYP_KW  = "encryp";
static const char* FBKGC_KW   = "fbkgc" ;
static const char* ONAME_KW   = "oname";
static const char* OPHONE_KW  = "ophone";

static const
ossimTrace traceDebug(ossimString("ossimNitfFileHeaderV2_1:debug"));
   
std::ostream& operator <<(std::ostream& out,
                     const ossimNitfImageInfoRecordV2_1 &data)
{
   return out << "theImageSubheaderLength:       "
              << data.theImageSubheaderLength
              << "\ntheImageLength:                "
              << data.theImageLength
              << std::endl;
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfGraphicInfoRecordV2_1 &data)
{
   return out << "theGraphicSubheaderLength:     "
              << data.theGraphicSubheaderLength
              << "\ntheGraphicLength:              "
              << data.theGraphicLength
              << std::endl;
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfTextFileInfoRecordV2_1 &data)
{
   return out << "theTextFileSubheaderLength:    "
              << data.theTextFileSubheaderLength
              << "\ntheTextFileLength:             "
              << data.theTextFileLength
              << std::endl;
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfDataExtSegInfoRecordV2_1 &data)
{
   return out << "theDataExtSegSubheaderLength:  "
              << data.theDataExtSegSubheaderLength
              << "\ntheDataExtSegLength:           "
              << data.theDataExtSegLength
              << std::endl;
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfResExtSegInfoRecordV2_1 &data)
{
   return out << "theResExtSegSubheaderLength:   "
              << data.theResExtSegSubheaderLength
              << "\ntheResExtSegLength:            "
              << data.theResExtSegLength
              << std::endl;
}


void ossimNitfImageInfoRecordV2_1::setSubheaderLength(ossim_uint32 length)
{
   ostringstream out;

   out << std::setw(6)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << length;

   memcpy(theImageSubheaderLength, out.str().c_str(), 6);
   theImageSubheaderLength[6] = '\0';
}

void ossimNitfImageInfoRecordV2_1::setImageLength(ossim_uint32 length)
{
   ostringstream out;

   out << std::setw(10)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << length;
   
   memcpy(theImageLength, out.str().c_str(), 10);
   theImageLength[10] = '\0';
}



ossimNitfFileHeaderV2_1::ossimNitfFileHeaderV2_1()
   :ossimNitfFileHeader()
{
   clearFields();
}

ossimNitfFileHeaderV2_1::~ossimNitfFileHeaderV2_1()
{
}

void ossimNitfFileHeaderV2_1::parseStream(std::istream& in)
{
   clearFields();

   // identification and origination group
   in.read(theFileTypeVersion, 9);
   theHeaderSize += 9;
   in.read(theComplexityLevel, 2);
   theHeaderSize += 2;
   in.read(theSystemType, 4);
   theHeaderSize += 4;
   in.read(theOriginatingStationId, 10);
   theHeaderSize += 10;
   in.read(theDateTime, 14);
   theHeaderSize += 14;
   in.read(theFileTitle, 80);
   theHeaderSize += 80;

   // read security group
   in.read(theSecurityClassification, 1);
   theHeaderSize ++;
   in.read(theSecurityClassificationSys, 2);
   theHeaderSize += 2;
   in.read(theCodewords, 11);
   theHeaderSize += 11;
   in.read(theControlAndHandling, 2);
   theHeaderSize += 2;
   in.read(theReleasingInstructions, 20);
   theHeaderSize += 20;
   in.read(theDeclassificationType, 2);
   theHeaderSize += 2;
   in.read(theDeclassificationDate, 8);
   theHeaderSize += 8;
   in.read(theDeclassificationExemption, 4);
   theHeaderSize += 4;
      
   in.read(theDowngrade, 1);
   theHeaderSize ++;
   in.read(theDowngradingDate, 8);
   theHeaderSize += 8;
   in.read(theClassificationText, 43);
   theHeaderSize += 43;
   in.read(theClassificationAuthorityType, 1);
   theHeaderSize ++;
   in.read(theClassificationAuthority, 40);
   theHeaderSize += 40;
   in.read(theClassificationReason, 1);
   theHeaderSize ++;
   in.read(theSecuritySourceDate, 8);
   theHeaderSize += 8;
   in.read(theSecurityControlNumber, 15);
   theHeaderSize += 15;
   in.read(theCopyNumber, 5);
   theHeaderSize += 5;
   in.read(theNumberOfCopies, 5);
   theHeaderSize += 5;
   in.read(theEncryption, 1);
   theHeaderSize ++;
   in.read((char*)theFileBackgroundColor, 3);
   theHeaderSize += 3;
   in.read(theOriginatorsName, 24);
   theHeaderSize += 24;
   in.read(theOriginatorsPhone, 18);
   theHeaderSize += 18;
   in.read(theFileLength, 12);
   theHeaderSize += 12;
   in.read(theHeaderLength, 6);
   theHeaderSize += 6;
   
   // image description group
   in.read(theNumberOfImageInfoRecords, 3); 
   theHeaderSize += 3;
   readImageInfoRecords(in); 

   // symbol description group
   in.read(theNumberOfGraphicInfoRecords, 3); 
   theHeaderSize += 3;
   readGraphicInfoRecords(in);

   in.read(theReservedForFutureUse1, 3);
   theHeaderSize += 3;
      
   // text file information group
   in.read(theNumberOfTextFileInfoRecords, 3); 
   theHeaderSize += 3;
   readTextFileInfoRecords(in);

   // Data extension group
   in.read(theNumberOfDataExtSegInfoRecords, 3); 
   theHeaderSize += 3;
   readDataExtSegInfoRecords(in);

   // Reserve Extension Segment group
   in.read(theNumberOfResExtSegInfoRecords, 3); 
   theHeaderSize += 3;
   readResExtSegInfoRecords(in);

   in.read(theUserDefinedHeaderDataLength, 5);
   theHeaderSize += 5;

   // only get the header overflow if there even exists
   // user defined data.
   std::streampos userDefinedHeaderLength = ossimString(theUserDefinedHeaderDataLength).toLong();
   ossimNitfTagInformation         headerTag;
   std::streampos start   = in.tellg();
   std::streampos current = in.tellg();
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
      in.seekg(start + userDefinedHeaderLength);
      theHeaderSize += (userDefinedHeaderLength);
   }
   in.read(theExtendedHeaderDataLength, 5);
   theHeaderSize += 5;
   std::streampos extendedHeaderDataLength = ossimString(theExtendedHeaderDataLength).toLong();

   start   = in.tellg();
   current = in.tellg();
   // for now let's just ignore it
   if(extendedHeaderDataLength > 0)
   {
      in.read(theExtendedHeaderDataOverflow, 3);
      while((current - start) < extendedHeaderDataLength)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         
         in.ignore(headerTag.getTagLength());
         headerTag.clearFields();
         current = in.tellg();
      }
      theHeaderSize += extendedHeaderDataLength;
      in.seekg(start + extendedHeaderDataLength);
   }

   // this need to be re-thought
   initializeAllOffsets();
}

void ossimNitfFileHeaderV2_1::writeStream(std::ostream &out)
{
   out.write(theFileTypeVersion, 9);
   out.write(theComplexityLevel, 2);
   out.write(theSystemType, 4);
   out.write(theOriginatingStationId, 10);
   out.write(theDateTime, 14);
   out.write(theFileTitle, 80);
   out.write(theSecurityClassification, 1);
   out.write(theSecurityClassificationSys, 2);
   out.write(theCodewords, 11);
   out.write(theControlAndHandling, 2);
   out.write(theReleasingInstructions, 20);
   out.write(theDeclassificationType, 2);
   out.write(theDeclassificationDate, 8);
   out.write(theDeclassificationExemption, 4);
   out.write(theDowngrade, 1);
   out.write(theDowngradingDate, 8);
   out.write(theClassificationText, 43);
   out.write(theClassificationAuthorityType, 1);
   out.write(theClassificationAuthority, 40);
   out.write(theClassificationReason, 1);
   out.write(theSecuritySourceDate, 8);
   out.write(theSecurityControlNumber, 15);
   out.write(theCopyNumber, 5);
   out.write(theNumberOfCopies, 5);
   out.write(theEncryption, 1);
   out.write((char*)theFileBackgroundColor, 3);
   out.write(theOriginatorsName, 24);
   out.write(theOriginatorsPhone, 18);
   out.write(theFileLength, 12);
   out.write(theHeaderLength, 6);
   ossim_uint32 idx = 0;
   {
      ostringstream outString;
      
      
      outString << std::setw(3)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << theNitfImageInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      
      for(idx = 0; idx < theNitfImageInfoRecords.size(); ++idx)
      {
         out.write(theNitfImageInfoRecords[idx].theImageSubheaderLength, 6);
         out.write(theNitfImageInfoRecords[idx].theImageLength, 10);
      }
   }
   {
      ostringstream outString;
      
      
      outString << std::setw(3)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << theNitfGraphicInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      for(idx = 0; idx < theNitfGraphicInfoRecords.size(); ++idx)
      {
         out.write(theNitfGraphicInfoRecords[idx].theGraphicSubheaderLength, 4);
         out.write(theNitfGraphicInfoRecords[idx].theGraphicLength, 6);
      }
   }
   out.write(theReservedForFutureUse1, 3);
   {
      ostringstream outString;
      
      
      outString << std::setw(3)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << theNitfTextFileInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      for(idx = 0; idx < theNitfTextFileInfoRecords.size(); ++idx)
      {
         out.write(theNitfTextFileInfoRecords[idx].theTextFileSubheaderLength, 4);
         out.write(theNitfTextFileInfoRecords[idx].theTextFileLength, 5);
      }
   }
   {
      ostringstream outString;
      
      
      outString << std::setw(3)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << theNitfDataExtSegInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      for(idx = 0; idx < theNitfDataExtSegInfoRecords.size(); ++idx)
      {
         out.write(theNitfDataExtSegInfoRecords[idx].theDataExtSegSubheaderLength, 4);
         out.write(theNitfDataExtSegInfoRecords[idx].theDataExtSegLength, 9);
      }
   }
   {
      ostringstream outString;
      
      
      outString << std::setw(3)
                << std::setfill('0')
                << std::setiosflags(ios::right)
                << theNitfResExtSegInfoRecords.size();

      out.write(outString.str().c_str(), 3);
      for(idx = 0; idx < theNitfResExtSegInfoRecords.size(); ++idx)
      {
         out.write(theNitfResExtSegInfoRecords[idx].theResExtSegSubheaderLength, 4);
         out.write(theNitfResExtSegInfoRecords[idx].theResExtSegLength, 7);
      }
   }
   out.write(theUserDefinedHeaderDataLength, 5);
   if(ossimString(theUserDefinedHeaderDataLength).toLong() > 0)
   {
      out.write(theUserDefinedHeaderOverflow, 3);
   }

   ossim_uint32 totalLength = getTotalTagLength();
   if(totalLength <= 99999)
   {
      std::ostringstream tempOut;

      tempOut << std::setw(5)
              << std::setfill('0')
	      << std::setiosflags(ios::right)
              << totalLength;
      
      memcpy(theExtendedHeaderDataLength, tempOut.str().c_str(), 5);
      
      out.write(theExtendedHeaderDataLength, 5);

      memset(theExtendedHeaderDataOverflow, '0', 3);

      if(totalLength > 0)
      {
         out.write(theExtendedHeaderDataOverflow, 3);
         ossim_uint32 i = 0;
         
         for(i = 0; i < theTagList.size(); ++i)
         {
            theTagList[i].writeStream(out);
         }
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_1::writeStream: Only support writing of total tag length < 99999" << std::endl;
   }
}

std::ostream& ossimNitfFileHeaderV2_1::print(std::ostream& out)const
{
   out << "theFileTypeVersion:           " << theFileTypeVersion
       << "\ntheComplexityLevel:           " << theComplexityLevel
       << "\ntheSystemType:                " << theSystemType     
       << "\ntheOriginatingStationId:      " << theOriginatingStationId
       << "\ntheDateTime:                  " << theDateTime       
       << "\ntheFileTitle:                 " << theFileTitle      
       << "\ntheSecurityClassification:    " << theSecurityClassification
       << "\ntheSecurityClassificationSys: " << theSecurityClassificationSys
       << "\ntheCodewords:                 " << theCodewords
       << "\ntheControlAndHandling:        " << theControlAndHandling
       << "\ntheReleasingInstructions:     " << theReleasingInstructions
       << "\ntheDeclassificationType:      " << theDeclassificationType
       << "\ntheDeclassificationDate:      " << theDeclassificationDate
       << "\ntheDeclassificationExemption: " << theDeclassificationExemption
       << "\ntheDowngrade:                 " << theDowngrade
       << "\ntheDowngradingDate:           " << theDowngradingDate
       << "\ntheClassificationText:        " << theClassificationText
       << "\ntheClassificationAuthorityType: " << theClassificationAuthorityType
       << "\ntheClassificationAuthority:   " << theClassificationAuthority
       << "\ntheClassificationReason:      " << theClassificationReason
       << "\ntheSecuritySourceDate:        " << theSecuritySourceDate
       << "\ntheSecurityControlNumber:     " << theSecurityControlNumber
       << "\ntheCopyNumber:                " << theCopyNumber
       << "\ntheNumberOfCopies:            " << theNumberOfCopies
       << "\ntheEncryption:                " << theEncryption
       << "\ntheFileBackgroundColorRed:    " << (int)theFileBackgroundColor[0]
       << "\ntheFileBackgroundColorGreen:  " << (int)theFileBackgroundColor[1]
       << "\ntheFileBackgroundColorBlue:   " << (int)theFileBackgroundColor[2]
       << "\ntheOriginatorsName:           " << theOriginatorsName
       << "\ntheOriginatorsPhone:          " << theOriginatorsPhone
       << "\ntheFileLength:                " << theFileLength  
       << "\ntheHeaderLength:              " << theHeaderLength
       << "\ntheNumberOfImageInfoRecords:  " << theNumberOfImageInfoRecords
       << std::endl;
   copy(theNitfImageInfoRecords.begin(),
        theNitfImageInfoRecords.end(),
        std::ostream_iterator<ossimNitfImageInfoRecordV2_1>(out, "\n"));

   out << "theNumberOfGraphicInfoRecords: "
       << theNumberOfGraphicInfoRecords << std::endl;
   
   copy(theNitfGraphicInfoRecords.begin(),
        theNitfGraphicInfoRecords.end(),
        std::ostream_iterator<ossimNitfGraphicInfoRecordV2_1>(out, "\n"));

   out << "theNumberOfTextFileInfoRecords: "
       << theNumberOfTextFileInfoRecords << std::endl;
   
   copy(theNitfTextFileInfoRecords.begin(),
        theNitfTextFileInfoRecords.end(),
        std::ostream_iterator<ossimNitfTextFileInfoRecordV2_1>(out, "\n"));

   out << "theNumberOfDataExtSegInfoRecords: "
       << theNumberOfDataExtSegInfoRecords << std::endl;

   copy(theNitfDataExtSegInfoRecords.begin(),
        theNitfDataExtSegInfoRecords.end(),
        std::ostream_iterator<ossimNitfDataExtSegInfoRecordV2_1>(out, "\n"));

   out << "theNumberOfResExtSegInfoRecords: "
       << theNumberOfResExtSegInfoRecords << std::endl;
   
   copy(theNitfResExtSegInfoRecords.begin(),
        theNitfResExtSegInfoRecords.end(),
        std::ostream_iterator<ossimNitfResExtSegInfoRecordV2_1>(out, "\n"));
   
   out << "theUserDefinedHeaderDataLength:  " << theUserDefinedHeaderDataLength
       << "\ntheUserDefinedHeaderOverflow:    " << theUserDefinedHeaderOverflow
       << "\ntheExtendedHeaderDataLength:     " << theExtendedHeaderDataLength
       << std::endl;
   return out;
}

ossimNitfImageHeader* ossimNitfFileHeaderV2_1::allocateImageHeader()const
{
   return new ossimNitfImageHeaderV2_1;
}

ossimNitfTextHeader *ossimNitfFileHeaderV2_1::allocateTextHeader()const
{
   return NULL;
}

ossimNitfDataExtensionSegment*
ossimNitfFileHeaderV2_1::allocateDataExtSegment()const
{
   return NULL;
}

bool ossimNitfFileHeaderV2_1::isEncrypted()const
{
   return (theEncryption[0]=='1');
}

long ossimNitfFileHeaderV2_1::getNumberOfImages()const
{
   return theNitfImageInfoRecords.size();
}

long ossimNitfFileHeaderV2_1::getNumberOfTextSegments()const
{
   return theNitfTextFileInfoRecords.size();
}

long ossimNitfFileHeaderV2_1::getHeaderSize()const
{
   return theHeaderSize;
}

long ossimNitfFileHeaderV2_1::getFileSize()const
{
   ossimString temp = theFileLength;

   if(temp = "999999999999")
   {
      return -1;
   }
   else
   {
      return temp.toLong();
   }
}

const char* ossimNitfFileHeaderV2_1::getVersion()const
{
   return &theFileTypeVersion[4];
}


void ossimNitfFileHeaderV2_1::clearFields()
{
   memcpy(theFileTypeVersion, "NITF02.10", 9);
   memcpy(theComplexityLevel, "01", 2);
   memcpy(theSystemType, "BF01", 4);
   memset(theOriginatingStationId, ' ', 10);
   memset(theDateTime, ' ', 14);
   memset(theFileTitle, ' ', 80);
   memset(theSecurityClassification, ' ', 1);
   memset(theSecurityClassificationSys, ' ', 2);
   memset(theCodewords, ' ', 11);
   memset(theControlAndHandling, ' ', 2);
   memset(theReleasingInstructions, ' ', 20);
   memset(theDeclassificationType, ' ', 2);
   memset(theDeclassificationDate, ' ', 8);
   memset(theDeclassificationExemption, ' ', 4);
   memset(theDowngrade, ' ', 1);
   memset(theDowngradingDate, ' ', 8);
   memset(theClassificationText, ' ', 43);
   memset(theClassificationAuthorityType, ' ', 1);
   memset(theClassificationAuthority, ' ', 40);
   memset(theClassificationReason, ' ', 1);
   memset(theSecuritySourceDate, ' ', 8);
   memset(theSecurityControlNumber, ' ', 15);
   memset(theCopyNumber, ' ', 5);
   memset(theNumberOfCopies, ' ', 5);
   memset(theEncryption, ' ', 1);
   memset(theFileBackgroundColor, 0, 3);
   memset(theOriginatorsName, ' ', 24);
   memset(theOriginatorsPhone, ' ', 18);
   memset(theFileLength, ' ', 12);
   memset(theHeaderLength, ' ', 6);
   memset(theNumberOfImageInfoRecords, '0', 3);
   memset(theNumberOfGraphicInfoRecords, '0', 3);
   memset(theReservedForFutureUse1, '0', 3);
   memset(theNumberOfTextFileInfoRecords, '0', 3);
   memset(theNumberOfDataExtSegInfoRecords, '0', 3);
   memset(theNumberOfResExtSegInfoRecords, '0', 3);
   memset(theUserDefinedHeaderDataLength, '0', 5);
   memset(theUserDefinedHeaderOverflow, '0',3);
   memset(theExtendedHeaderDataLength, '0',5);
   theFileTypeVersion[9] = '\0';
   theComplexityLevel[2] = '\0';
   theSystemType[4] = '\0';
   theOriginatingStationId[10] = '\0';
   theDateTime[14] = '\0';
   theFileTitle[80] = '\0';
   theSecurityClassification[1] = '\0';
   theSecurityClassificationSys[2] = '\0';
   theCodewords[11] = '\0';
   theControlAndHandling[2] = '\0';
   theReleasingInstructions[20] = '\0';
   theDeclassificationType[2] = '\0';
   theDeclassificationDate[8] = '\0';
   theDeclassificationExemption[4] = '\0';
   theDowngrade[1] = '\0';
   theDowngradingDate[8] = '\0';
   theClassificationText[43] = '\0';
   theClassificationAuthorityType[1] = '\0';
   theClassificationAuthority[40] = '\0';
   theClassificationReason[1] = '\0';
   theSecuritySourceDate[8] = '\0';
   theSecurityControlNumber[15] = '\0';
   theCopyNumber[5] = '\0';
   theNumberOfCopies[5] = '\0';
   theEncryption[1] = '\0';
   theOriginatorsName[24] = '\0';
   theOriginatorsPhone[18] = '\0';
   theFileLength[12] = '\0';
   theHeaderLength[6] = '\0';
   theNumberOfImageInfoRecords[3] = '\0';
   theNumberOfGraphicInfoRecords[3] = '\0';
   theNumberOfTextFileInfoRecords[3] = '\0';
   theNumberOfDataExtSegInfoRecords[3] = '\0';
   theNumberOfResExtSegInfoRecords[3] = '\0';
   theUserDefinedHeaderDataLength[5] = '\0';
   theUserDefinedHeaderOverflow[3] = '\0';
   theExtendedHeaderDataLength[5] = '\0';
   theExtendedHeaderDataOverflow[3] = '\0';

   theHeaderSize = 0;
}

void ossimNitfFileHeaderV2_1::readImageInfoRecords(std::istream &in)
{
   long numberOfImages = ossimString(theNumberOfImageInfoRecords).toLong();
   long index;

   theNitfImageInfoRecords.clear();
   for(index=0; index < numberOfImages; index++)
   {
      ossimNitfImageInfoRecordV2_1 temp;

      
      in.read(temp.theImageSubheaderLength, 6);
      in.read(temp.theImageLength, 10);
      theHeaderSize += 16;

      temp.theImageSubheaderLength[6] = '\0';
      temp.theImageLength[10] = '\0';
      
      theNitfImageInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_1::readGraphicInfoRecords(std::istream &in)
{
   long numberOfGraphics = ossimString(theNumberOfGraphicInfoRecords).toLong();
   long index;

   theNitfGraphicInfoRecords.clear();
   
   for(index=0; index < numberOfGraphics; index++)
   {
      ossimNitfGraphicInfoRecordV2_1 temp;

      
      in.read(temp.theGraphicSubheaderLength, 4);
      in.read(temp.theGraphicLength, 6);
      theHeaderSize += 10;

      temp.theGraphicSubheaderLength[4] = '\0';
      temp.theGraphicLength[6] = '\0';
      
      theNitfGraphicInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_1::readTextFileInfoRecords(std::istream &in)
{
   long numberOfTextFiles = ossimString(theNumberOfTextFileInfoRecords).toLong();
   long index;

   theNitfTextFileInfoRecords.clear();
   for(index=0; index < numberOfTextFiles; index++)
   {
      ossimNitfTextFileInfoRecordV2_1 temp;
      
      in.read(temp.theTextFileSubheaderLength, 4);
      in.read(temp.theTextFileLength, 5);
      theHeaderSize += 9;
      
      temp.theTextFileSubheaderLength[4] = '\0';
      temp.theTextFileLength[5] = '\0';
      
      theNitfTextFileInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_1::readDataExtSegInfoRecords(std::istream &in)
{
   long numberOfDataExtSegs = ossimString(theNumberOfDataExtSegInfoRecords).toLong();
   long index;

   theNitfDataExtSegInfoRecords.clear();
   for(index=0; index < numberOfDataExtSegs; index++)
   {
      ossimNitfDataExtSegInfoRecordV2_1 temp;
      
      in.read(temp.theDataExtSegSubheaderLength, 4);
      in.read(temp.theDataExtSegLength, 9);
      theHeaderSize += 13;

      temp.theDataExtSegSubheaderLength[4] = '\0';
      temp.theDataExtSegLength[9]          = '\0';

      theNitfDataExtSegInfoRecords.push_back(temp);
   }
}

void ossimNitfFileHeaderV2_1::readResExtSegInfoRecords(std::istream &in)
{
   long numberOfResExtSegs = ossimString(theNumberOfResExtSegInfoRecords).toLong();
   long index;

   theNitfResExtSegInfoRecords.clear();
   for(index=0; index < numberOfResExtSegs; index++)
   {
      ossimNitfResExtSegInfoRecordV2_1 temp;

      in.read(temp.theResExtSegSubheaderLength, 4);
      in.read(temp.theResExtSegLength, 7);
      theHeaderSize += 11;

      temp.theResExtSegSubheaderLength[4] = '\0';
      temp.theResExtSegLength[7]          = '\0';
      
      theNitfResExtSegInfoRecords.push_back(temp);      
   }
}

long ossimNitfFileHeaderV2_1::getNumberOfLabels()const
{
   return 0;
}

long ossimNitfFileHeaderV2_1::getNumberOfSymbols()const
{
   return 0;
}

long ossimNitfFileHeaderV2_1::getNumberOfGraphics()const
{
   return 0;
}

long ossimNitfFileHeaderV2_1::getNumberOfDataExtSegments()const
{
   return 0;
}

const char* ossimNitfFileHeaderV2_1::getDateTime()const
{
   return NULL;
}

ossimDrect ossimNitfFileHeaderV2_1::getImageRect()const
{
   return theImageRect;
}

void ossimNitfFileHeaderV2_1::addImageInfoRecord(const ossimNitfImageInfoRecordV2_1& recordInfo)
{
   theNitfImageInfoRecords.push_back(recordInfo);

   setNumberOfImageInfoRecords(theNitfImageInfoRecords.size());
}

void ossimNitfFileHeaderV2_1::replaceImageInfoRecord(int i, const ossimNitfImageInfoRecordV2_1& recordInfo)
{
   theNitfImageInfoRecords[i]=recordInfo;
}

ossimNitfSymbolHeader *ossimNitfFileHeaderV2_1::allocateSymbolHeader()const
{
   return NULL;                      ;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_1::allocateLabelHeader()const
{
   return NULL;                      ;
}

void ossimNitfFileHeaderV2_1::initializeAllOffsets()
{
   // this will be a running tally 
   long tally = theHeaderSize;
   long index = 0;

   // clear out all offset inforamtion and begin populating them
   theImageOffsetList.clear();

   for(index = 0; index < (long)theNitfImageInfoRecords.size(); ++index)
   {
      theImageOffsetList.push_back(ossimNitfImageOffsetInformation(tally,
                                                                   tally + theNitfImageInfoRecords[index].getTotalLength()));
      tally += theNitfImageInfoRecords[index].getTotalLength();
   }
}

ossimNitfImageHeader*
ossimNitfFileHeaderV2_1::getNewImageHeader(long imageNumber,
                                           std::istream& in)const
{
   ossimNitfImageHeader *result = NULL;
   
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
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimNitfFileHeaderV2_1::getNewImageHeader: "
                                          << "\nNo images in file or image number (" << imageNumber
                                          << ") is out of range!\n";
   }
   
   return result;
}

ossimNitfSymbolHeader*
ossimNitfFileHeaderV2_1::getNewSymbolHeader(long symbolNumber,
                                            std::istream& in)const
{
   // Currently not implemented...
   
   ossimNitfSymbolHeader *result = NULL;
   
   return result;
}

ossimNitfLabelHeader*
ossimNitfFileHeaderV2_1::getNewLabelHeader(long labelNumber,
                                           std::istream& in)const
{
   // Currently not implemented...
   ossimNitfLabelHeader *result = NULL;
   
   return result;
}

ossimNitfTextHeader*
ossimNitfFileHeaderV2_1::getNewTextHeader(long textNumber,
                                          std::istream& in)const
{
   // Currently not implemented...
   ossimNitfTextHeader *result = NULL;
   
   return result;
}

ossimNitfDataExtensionSegment*
ossimNitfFileHeaderV2_1::getNewDataExtensionSegment(long dataExtNumber,
                                                    std::istream& in)const
{
   // Currently not implemented...
   ossimNitfDataExtensionSegment *result = NULL;
   
   return result;
}

ossimString ossimNitfFileHeaderV2_1::getComplexityLevel()const
{
   return theComplexityLevel;
}

ossimString ossimNitfFileHeaderV2_1::getSystemType()const
{
   return theSystemType;
}

ossimString ossimNitfFileHeaderV2_1::getOriginatingStationId()const
{
   return theOriginatingStationId;
}

ossimString ossimNitfFileHeaderV2_1::getDate()const
{
   return theDateTime;
}

ossimString ossimNitfFileHeaderV2_1::getTitle()const
{
   return theFileTitle;
}

ossimString ossimNitfFileHeaderV2_1::getSecurityClassification()const
{
   return theSecurityClassification;
}

ossimString ossimNitfFileHeaderV2_1::getSecurityClassificationSys()const
{
   return theSecurityClassificationSys;
}

ossimString ossimNitfFileHeaderV2_1::getCodeWords()const
{
   return theCodewords;
}

ossimString ossimNitfFileHeaderV2_1::getControlAndHandling()const
{
   return theControlAndHandling;
}

ossimString ossimNitfFileHeaderV2_1::getReleasingInstructions()const
{
   return theReleasingInstructions;
}

ossimString ossimNitfFileHeaderV2_1::getDeclassificationType()const
{
   return theDeclassificationType;
}

ossimString ossimNitfFileHeaderV2_1::getDeclassificationDate()const
{
   return theDeclassificationDate;
}

ossimString ossimNitfFileHeaderV2_1::getDeclassificationExemption()const
{
   return theDeclassificationExemption;
}

ossimString ossimNitfFileHeaderV2_1::getDowngrade()const
{
   return theDowngrade;
}

ossimString ossimNitfFileHeaderV2_1::getDowngradingDate()const
{
   return theDowngradingDate;
}

ossimString ossimNitfFileHeaderV2_1::getClassificationText()const
{
   return theClassificationText;
}

ossimString ossimNitfFileHeaderV2_1::getClassificationAuthorityType()const
{
   return theClassificationAuthorityType;
}

ossimString ossimNitfFileHeaderV2_1::getClassificationAuthority()const
{
   return theClassificationAuthority;
}

ossimString ossimNitfFileHeaderV2_1::getClassificationReason()const
{
   return theClassificationReason;
}

ossimString ossimNitfFileHeaderV2_1::getSecuritySourceDate()const
{
   return theSecuritySourceDate;
}

ossimString ossimNitfFileHeaderV2_1::getSecurityControlNumber()const
{
   return theSecurityControlNumber;
}

ossimString ossimNitfFileHeaderV2_1::getCopyNumber()const
{
   return theCopyNumber;
}

ossimString ossimNitfFileHeaderV2_1::getNumberOfCopies()const
{
   return theNumberOfCopies;
}

ossimString ossimNitfFileHeaderV2_1::getEncryption()const
{
   return theEncryption;
}

void ossimNitfFileHeaderV2_1::getBackgroundColor(ossim_uint8& r,
                                                 ossim_uint8& g,
                                                 ossim_uint8& b)const
{
   r = theFileBackgroundColor[0];
   g = theFileBackgroundColor[1];
   b = theFileBackgroundColor[2];
}

ossimString ossimNitfFileHeaderV2_1::getOriginatorsName()const
{
   return theOriginatorsName;
}

ossimString ossimNitfFileHeaderV2_1::getOriginatorsPhone()const
{
   return theOriginatorsPhone;
}

void ossimNitfFileHeaderV2_1::setFileLength(ossim_uint64 fileLength)
{
   ostringstream out;

   out << std::setw(12)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << fileLength;

   memcpy(theFileLength, out.str().c_str(), 12);
}

void ossimNitfFileHeaderV2_1::setNumberOfImageInfoRecords(ossim_uint64 num)
   throw(std::out_of_range)
{
   if (num < 1000)
   {
      ostringstream out;
      
      out << std::setw(3)
          << std::setfill('0')
          << std::setiosflags(ios::right)
          << num;
      
      memcpy(theNumberOfImageInfoRecords, out.str().c_str(), 3);
   }
   else
   {
      std::string s = "ossimNitfFileHeaderV2_1::setNumberOfImageInfoRecords:";
      s += " ERROR\nExceeded max image info number of 999!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
}

void ossimNitfFileHeaderV2_1::setHeaderLength(ossim_uint64 headerLength)
{
   ostringstream out;

   out << std::setw(6)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << headerLength;

   memcpy(theHeaderLength, out.str().c_str(), 6);
}

void ossimNitfFileHeaderV2_1::setComplexityLevel(const ossimString& level)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << level.c_str();

   memcpy(theComplexityLevel, out.str().c_str(), 2);
}

void ossimNitfFileHeaderV2_1::setSystemType(const ossimString& systemType)
{
   std::ostringstream out;

   out << std::setw(4)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << systemType.c_str();

   memcpy(theSystemType, out.str().c_str(), 4);
}

void ossimNitfFileHeaderV2_1::setOriginatingStationId(const ossimString& originationId)
{
   std::ostringstream out;
   
   out << std::setw(10)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(originationId).trim();

   memcpy(theOriginatingStationId, out.str().c_str(), 10);
}

void ossimNitfFileHeaderV2_1::setDate(const ossimLocalTm& d)
{
   memcpy(theDateTime, formatDate(d).c_str(), 14);
}

void ossimNitfFileHeaderV2_1::setDate(const ossimString& d)
{
   if(d.size()==14)
   {
      memcpy(theDateTime, d.c_str(), 14);
   }
}

void ossimNitfFileHeaderV2_1::setTitle(const ossimString& title)
{
   std::ostringstream out;
   
   out << std::setw(80)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(title).trim();

   memcpy(theFileTitle, out.str().c_str(), 80);
}

void ossimNitfFileHeaderV2_1::setFileSecurityClassification(const ossimString& securityClassification)
{
   std::ostringstream out;
   
   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(securityClassification).trim();

   memcpy(theSecurityClassification, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setSecurityClassificationSys(const ossimString& value)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(value).trim();

   memcpy(theSecurityClassificationSys, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setCodeWords(const ossimString& codeWords)
{
   std::ostringstream out;
   
   out << std::setw(11)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(codeWords).trim();

   memcpy(theCodewords, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setControlAndHandling(const ossimString& controlAndHandling)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(controlAndHandling).trim();

   memcpy(theControlAndHandling, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setReleasingInstructions(const ossimString& releasingInstructions)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(releasingInstructions).trim();

   memcpy(theReleasingInstructions, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setDeclassificationType(const ossimString& declassType)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << declassType.trim();

   memcpy(theDeclassificationType, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setDeclassificationDate(const ossimLocalTm& d)
{
   memcpy(theDeclassificationDate, formatDate(d).c_str(), 8);
}

void ossimNitfFileHeaderV2_1::setDeclassificationDate(const ossimString& d)
{
   if(d.size() >=8)
   {
      memcpy(theDeclassificationDate, d.c_str(), 8);
   }
}

void ossimNitfFileHeaderV2_1::setDeclassificationExemption(const ossimString& exemption)
{
   std::ostringstream out;
   
   out << std::setw(4)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << exemption.trim();

   memcpy(theDeclassificationExemption, out.str().c_str(), 4);
}

void ossimNitfFileHeaderV2_1::setDowngrade(const ossimString& downgrade)
{
   std::ostringstream out;
   
   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << downgrade.trim();

   memcpy(theDowngrade, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setDowngradingDate(const ossimLocalTm& d)
{
   memcpy(theDowngradingDate, formatDate(d).c_str(), 8);
}

void ossimNitfFileHeaderV2_1::setDowngradingDate(const ossimString& d)
{
   if(d.size() >=8)
   {
      memcpy(theDowngradingDate, d.c_str(), 8);
   }
}

void ossimNitfFileHeaderV2_1::setClassificationText(const ossimString& classificationText)
{
   std::ostringstream out;
   
   out << std::setw(43)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << classificationText.trim();

   memcpy(theClassificationText, out.str().c_str(), 43);
}

void ossimNitfFileHeaderV2_1::setClassificationAuthorityType(const ossimString& authorityType)
{
   std::ostringstream out;
   
   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << authorityType.trim();

   memcpy(theClassificationAuthorityType, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setClassificationAuthority(const ossimString& authority)
{
   std::ostringstream out;
   
   out << std::setw(40)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << authority.trim();

   memcpy(theClassificationAuthority, out.str().c_str(), 40);
}

void ossimNitfFileHeaderV2_1::setClassificationReason(const ossimString& reason)
{
   std::ostringstream out;
   
   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << reason.trim();

   memcpy(theClassificationReason, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setSecuritySourceDate(const ossimLocalTm& d)
{
   memcpy(theSecuritySourceDate, formatDate(d).c_str(), 8);
}

void ossimNitfFileHeaderV2_1::setSecuritySourceDate(const ossimString& d)
{
   if(d.size() >=8)
   {
      memcpy(theSecuritySourceDate, d.c_str(), 8);
   }
}

void ossimNitfFileHeaderV2_1::setSecurityControlNumber(const ossimString& number)
{
   std::ostringstream out;
   
   out << std::setw(15)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << number.trim();

   memcpy(theSecurityControlNumber, out.str().c_str(), 15);
}

void ossimNitfFileHeaderV2_1::setCopyNumber(const ossimString& copyNumber)
{
   if(copyNumber.trim() == "")
   {
      memset(theCopyNumber, ' ', 5);
   }
   else
   {
      std::ostringstream out;
      
      out << std::setw(5)
          << std::setfill('0')
	  << std::setiosflags(ios::right)
          << copyNumber.trim();
      
      memcpy(theCopyNumber, out.str().c_str(), 5);
   }
}

void ossimNitfFileHeaderV2_1::setNumberOfCopies(const ossimString& numberOfCopies)
{
   if(numberOfCopies.trim() == "")
   {
      memset(theNumberOfCopies, ' ', 5);
   }
   else
   {
      std::ostringstream out;
      
      out << std::setw(5)
          << std::setfill('0')
	  << std::setiosflags(ios::right)
          << numberOfCopies.trim();
      
      memcpy(theNumberOfCopies, out.str().c_str(), 5);
   }
}

void ossimNitfFileHeaderV2_1::setEncryption(const ossimString& encryption)
{
   std::ostringstream out;
   
   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << encryption.trim();
   
   memcpy(theEncryption, out.str().c_str(), 1);
}

void ossimNitfFileHeaderV2_1::setFileBackgroundColor(ossim_uint8 r,
                                                     ossim_uint8 g,
                                                     ossim_uint8 b)
{
   theFileBackgroundColor[0] = r;
   theFileBackgroundColor[1] = g;
   theFileBackgroundColor[2] = b;
}

void ossimNitfFileHeaderV2_1::setOriginatorsName(const ossimString& name)
{
   std::ostringstream out;
   
   out << std::setw(24)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << name.trim();
   
   memcpy(theOriginatorsName, out.str().c_str(), 24);
}

void ossimNitfFileHeaderV2_1::setOriginatorsPhone(const ossimString& phone)
{
   std::ostringstream out;
   
   out << std::setw(18)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << phone.trim();
   
   memcpy(theOriginatorsPhone, out.str().c_str(), 18);
}

ossimString ossimNitfFileHeaderV2_1::formatDate(const ossimLocalTm& d)
{
   ostringstream out;

   out << std::setw(4)
       << std::setfill('0')
       << d.getYear()
       << std::setw(2)
       << std::setfill('0')
       << d.getMonth()
       << std::setw(2)
       << std::setfill('0')
       << d.getDay()
       << std::setw(2)
       << std::setfill('0')
       << d.getHour()
       << std::setw(2)
       << std::setfill('0')
       << d.getMin()
       << std::setw(2)
       << std::setfill('0')
       << d.getSec();
   
   return out.str();
}

void ossimNitfFileHeaderV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();
   if(name == "clevel")
   {
      setComplexityLevel(property->valueToString());
   }
   else if(name == STYPE_KW)
   {
      setSystemType(property->valueToString());
   }
   else if(name == OSTAID_KW)
   {
      setOriginatingStationId(property->valueToString());
   }
   else if(name == FDT_KW)
   {
      setDate(property->valueToString());
   }
   else if(name == FTITLE_KW)
   {
      setTitle(property->valueToString());
   }
   else if(name == FSCLAS_KW)
   {
      setFileSecurityClassification(property->valueToString());
   }
   else if(name == FSCLASY_KW)
   {
     setSecurityClassificationSys(property->valueToString()); 
   }
   else if(name == FSCODE_KW)
   {
     setCodeWords(property->valueToString()); 
   }
   else if(name == FSCTLH_KW)
   {
      setControlAndHandling(property->valueToString());
   }
   else if(name == FSREL_KW)
   {
      setReleasingInstructions(property->valueToString());
   }
   else if(name == FSDCTP_KW)
   {
      setDeclassificationType(property->valueToString());
   }
   else if(name == FSDCDT_KW)
   {
      setDeclassificationDate(property->valueToString());
   }
   else if(name == FSDCXM_KW)
   {
      setDeclassificationExemption(property->valueToString());
   }
   else if(name == FSDG_KW)
   {
      setDowngrade(property->valueToString());
   }
   else if(name == FSDGDT_KW)
   {
      setDowngradingDate(property->valueToString());
   }
   else if(name == FSCLTX_KW)
   {
      setClassificationText(property->valueToString());
   }
   else if(name == FSCATP_KW)
   {
      setClassificationAuthorityType(property->valueToString());
   }
   else if(name == FSCAUT_KW)
   {
      setClassificationAuthority(property->valueToString()); 
   }
   else if(name == FSCRSN_KW)
   {
      setClassificationReason(property->valueToString());
   }
   else if(name == FSSRDT_KW)
   {
      setSecuritySourceDate(property->valueToString());
   }
   else if(name == FSCTLN_KW)
   {
      setSecurityControlNumber(property->valueToString());
   }
   else if(name == FSCOP_KW)
   {
      setCopyNumber(property->valueToString());
   }
   else if(name == FSCPYS_KW)
   {
      setNumberOfCopies(property->valueToString());
   }
   else if(name == ENCRYP_KW)
   {
      setEncryption(property->valueToString());
   }
   else if(name == FBKGC_KW)
   {
      ossimString value = property->valueToString();
      istringstream in(value);
      ossim_uint8 r, g, b;

      in >> r >> g >> b;
      
      setFileBackgroundColor(r, g, b);
   }
   else if(name == ONAME_KW)
   {
      setOriginatorsName(property->valueToString());
   }
   else if(name == OPHONE_KW)
   {
      setOriginatorsPhone(property->valueToString());
   }
   else
   {
      ossimNitfFileHeader::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfFileHeaderV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = NULL;
   ossimStringProperty* stringProperty = NULL;
   ossimColorProperty* colorProperty = NULL;
   
   if(name == CLEVEL_KW)
   {
      ossimNumericProperty* numericProperty = new ossimNumericProperty(name,
                                                                       getComplexityLevel(),
                                                                       1, 99);
      numericProperty->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
      property = numericProperty;
      
   }
   else if(name == STYPE_KW)
   {
      property = new ossimStringProperty(name,
                                         getSystemType().trim());
   }
   else if(name == OSTAID_KW)
   {
      property = new ossimStringProperty(name,
                                         getOriginatingStationId().trim());
   }
   else if(name == FDT_KW)
   {
      property = new ossimDateProperty(name,
                                        getDate());
   }
   else if(name == FTITLE_KW)
   {
      property = new ossimStringProperty(name,
                                         getTitle().trim());
   }
   else if(name == FSCLAS_KW)
   {
      stringProperty = new ossimStringProperty(name,
                                               getSecurityClassification().trim(),
                                               false);
      
      stringProperty->addConstraint("");
      stringProperty->addConstraint("T");
      stringProperty->addConstraint("S");
      stringProperty->addConstraint("C");
      stringProperty->addConstraint("R");
      stringProperty->addConstraint("U");
      
      property = stringProperty;
   }
   else if(name == FSCLASY_KW)
   {
      stringProperty = new ossimStringProperty(name,
                                               getSecurityClassificationSys().trim());
   }
   else if(name == FSCODE_KW)
   {
      property = new ossimStringProperty(name,
                                         getCodeWords().trim());
   }
   else if(name == FSCTLH_KW)
   {
      property = new ossimStringProperty(name,
                                         getControlAndHandling().trim());
   }
   else if(name == FSREL_KW)
   {
      property = new ossimStringProperty(name,
                                         getReleasingInstructions().trim());
   }
   else if(name == FSDCTP_KW)
   {
      property = new ossimStringProperty(name,
                                         getDeclassificationType().trim());
   }
   else if(name == FSDCDT_KW)
   {
      property = new ossimStringProperty(name,
                                         getDeclassificationDate().trim());
   }
   else if(name == FSDCXM_KW)
   {
      property = new ossimStringProperty(name,
                                         getDeclassificationExemption().trim());
   }
   else if(name == FSDG_KW)
   {
      stringProperty = new ossimStringProperty(name,
                                               getDowngrade().trim(),
                                               false);
      
      stringProperty->addConstraint("");
      stringProperty->addConstraint("S");
      stringProperty->addConstraint("C");
      stringProperty->addConstraint("R");

      property = stringProperty;
   }
   else if(name == FSDGDT_KW)
   {
      property = new ossimStringProperty(name,
                                         getDowngradingDate().trim());
   }
   else if(name == FSCLTX_KW)
   {
      property = new ossimStringProperty(name,
                                         getClassificationText().trim());
   }
   else if(name == FSCATP_KW)
   {
      stringProperty = new ossimStringProperty(name,
                                               getClassificationAuthorityType().trim(),
                                               false);
      
      stringProperty->addConstraint("");
      stringProperty->addConstraint("O");
      stringProperty->addConstraint("D");
      stringProperty->addConstraint("M");
      
      property = stringProperty;
   }
   else if(name == FSCAUT_KW)
   {
      property = new ossimStringProperty(name,
                                         getClassificationAuthority().trim());
   }
   else if(name == FSCRSN_KW)
   {
      stringProperty = new ossimStringProperty(name,
                                               getClassificationReason().trim(),
                                               false);
      
      stringProperty->addConstraint("");
      stringProperty->addConstraint("A");
      stringProperty->addConstraint("B");
      stringProperty->addConstraint("C");
      stringProperty->addConstraint("D");
      stringProperty->addConstraint("E");
      stringProperty->addConstraint("F");
      stringProperty->addConstraint("G");
      
      property = stringProperty;
   }
   else if(name == FSSRDT_KW)
   {
      property = new ossimStringProperty(name,
                                         getSecuritySourceDate().trim());
   }
   else if(name == FSCTLN_KW)
   {
      property = new ossimStringProperty(name,
                                         getSecurityControlNumber().trim());

   }
   else if(name == FSCOP_KW)
   {
      property = new ossimStringProperty(name,
                                         getCopyNumber().trim());
   }
   else if(name == FSCPYS_KW)
   {
      property = new ossimStringProperty(name,
                                         getNumberOfCopies().trim());
   }
   else if(name == ENCRYP_KW)
   {
      property = new ossimStringProperty(name,
                                         getEncryption().trim(),
                                         false);
   }
   else if(name == FBKGC_KW)
   {
      ossim_uint8 r, g, b;
      getBackgroundColor(r, g, b);
      
      colorProperty = new ossimColorProperty(name,
                                             ossimRgbVector(r, g, b));

      property = colorProperty;
   }
   else
   {
      return ossimNitfFileHeader::getProperty(name);
   }
   return property;
}

void ossimNitfFileHeaderV2_1::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfFileHeader::getPropertyNames(propertyNames);

   propertyNames.push_back(CLEVEL_KW);
   propertyNames.push_back(STYPE_KW);
   propertyNames.push_back(OSTAID_KW);
   propertyNames.push_back(FDT_KW);
   propertyNames.push_back(FTITLE_KW);
   propertyNames.push_back(FSCLAS_KW);
   propertyNames.push_back(FSCLASY_KW);
   propertyNames.push_back(FSCODE_KW);
   propertyNames.push_back(FSCTLH_KW);
   propertyNames.push_back(FSREL_KW);
   propertyNames.push_back(FSDCTP_KW);
   propertyNames.push_back(FSDCDT_KW);
   propertyNames.push_back(FSDCXM_KW);
   propertyNames.push_back(FSDG_KW);
   propertyNames.push_back(FSDGDT_KW);
   propertyNames.push_back(FSCLTX_KW);
   propertyNames.push_back(FSCATP_KW);
   propertyNames.push_back(FSCAUT_KW);
   propertyNames.push_back(FSCRSN_KW);
   propertyNames.push_back(FSSRDT_KW);
   propertyNames.push_back(FSCTLN_KW);
   propertyNames.push_back(FSCOP_KW);
   propertyNames.push_back(FSCPYS_KW);
   propertyNames.push_back(ENCRYP_KW);
   propertyNames.push_back(FBKGC_KW);
   propertyNames.push_back(ONAME_KW);
   propertyNames.push_back(OPHONE_KW);
}

bool ossimNitfFileHeaderV2_1::setDefaults(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
#if 0	
   const char* complexityLevel             = kwl.find(prefix, CLEVEL_KW);
   const char* systemType                  = kwl.find(prefix, STYPE_KW);
   const char* originatingStationId        = kwl.find(prefix, OSTAID_KW);
   const char* dateTime                    = kwl.find(prefix, FDT_KW);
   const char* title                       = kwl.find(prefix, FTITLE_KW);
   const char* securityClassification      = kwl.find(prefix, FSCLAS_KW);
   const char* securityClassificationSys   = kwl.find(prefix, FSCLASY_KW);
   const char* codeWords                   = kwl.find(prefix, FSCODE_KW);
   const char* controlAndHandling          = kwl.find(prefix, FSCTLH_KW);
   const char* releasingInstructions       = kwl.find(prefix, FSREL_KW);
   const char* declassificationType        = kwl.find(prefix, FSDCTP_KW);
   const char* declassificationDate        = kwl.find(prefix, FSDCDT_KW);
   const char* declassificationExemption   = kwl.find(prefix, FSDCXM_KW);
   const char* downgrade                   = kwl.find(prefix, FSDG_KW);
   const char* downgradingDate             = kwl.find(prefix, FSDGDT_KW);
   const char* classificationText          = kwl.find(prefix, FSCLTX_KW);
   const char* classificationAuthorityType = kwl.find(prefix, FSCATP_KW);
   const char* classificationAuthority     = kwl.find(prefix, FSCAUT_KW);
   const char* classificationReason        = kwl.find(prefix, FSCRSN_KW);
   const char* securitySourceDate          = kwl.find(prefix, FSSRDT_KW);
   const char* securityControlNumber       = kwl.find(prefix, FSCTLN_KW);
   const char* copyNumber                  = kwl.find(prefix, FSCOP_KW);
   const char* numberOfCopies              = kwl.find(prefix, FSCPYS_KW);
   const char* encryption                  = kwl.find(prefix, ENCRYP_KW);
   const char* fileBackgroundColor         = kwl.find(prefix, FBKGC_KW);
   const char* originatorsName             = kwl.find(prefix, ONAME_KW);
   const char* originatorsPhone            = kwl.find(prefix, OPHONE_KW);
   if(complexityLevel)
   {
      setComplexityLevel(complexityLevel);
   }
#endif
   return true;
}

