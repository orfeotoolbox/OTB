//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeaderV2_1.cpp 12403 2008-02-04 17:59:13Z gpotts $

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring> // for memset

#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimColorProperty.h>
#include <ossim/base/ossimDateProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>

#ifndef NULL
#  include <cstddef>
#endif

RTTI_DEF1(ossimNitfFileHeaderV2_1,
          "ossimNitfFileHeaderV2_1",
          ossimNitfFileHeaderV2_X)

const ossimString ossimNitfFileHeaderV2_1::FSCLASY_KW = "fsclasy";
const ossimString ossimNitfFileHeaderV2_1::FSDCTP_KW  = "fsdctp";
const ossimString ossimNitfFileHeaderV2_1::FSDCDT_KW  = "fsdcdt";
const ossimString ossimNitfFileHeaderV2_1::FSDCXM_KW  = "fsdcxm";
const ossimString ossimNitfFileHeaderV2_1::FSDG_KW    = "fsdg";
const ossimString ossimNitfFileHeaderV2_1::FSDGDT_KW  = "fsdgdt";
const ossimString ossimNitfFileHeaderV2_1::FSCLTX_KW  = "fscltx";
const ossimString ossimNitfFileHeaderV2_1::FSCATP_KW  = "fscatp";
const ossimString ossimNitfFileHeaderV2_1::FSCRSN_KW  = "fscrsn";
const ossimString ossimNitfFileHeaderV2_1::FSSRDT_KW  = "fssrdt";
const ossimString ossimNitfFileHeaderV2_1::FBKGC_KW   = "fbkgc";

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

ossim_int32 ossimNitfImageInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theImageSubheaderLength).toInt32();
}

ossim_int32 ossimNitfImageInfoRecordV2_1::getImageLength()const
{
   return ossimString(theImageLength).toInt32();
}

ossim_int32 ossimNitfImageInfoRecordV2_1::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
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
   :ossimNitfFileHeaderV2_X()
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
   std::streampos userDefinedHeaderLength = ossimString(theUserDefinedHeaderDataLength).toInt32();
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
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
      in.seekg(start + userDefinedHeaderLength);
      theHeaderSize += (userDefinedHeaderLength);
   }
   in.read(theExtendedHeaderDataLength, 5);
   theHeaderSize += 5;
   std::streampos extendedHeaderDataLength = ossimString(theExtendedHeaderDataLength).toInt32();

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
         
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
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
   if(ossimString(theUserDefinedHeaderDataLength).toInt32() > 0)
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
   out << setiosflags(ios::left)
       << "\nossimNitfFileHeaderV2_1::print"
       << setw(24) << "\nFHDR:"     << theFileTypeVersion
       << setw(24) << "\nCLEVEL:"   << theComplexityLevel
       << setw(24) << "\nSTYPE:"    << theSystemType     
       << setw(24) << "\nOSTAID:"   << theOriginatingStationId
       << setw(24) << "\nFDT:"      << theDateTime       
       << setw(24) << "\nFTITLE:"   << theFileTitle      
       << setw(24) << "\nFSCLAS:"   << theSecurityClassification
       << setw(24) << "\nFSCLASY:"  << theSecurityClassificationSys
       << setw(24) << "\nFSCODE:"   << theCodewords
       << setw(24) << "\nFSCTLH:"   << theControlAndHandling
       << setw(24) << "\nFSREL:"    << theReleasingInstructions
       << setw(24) << "\nFSDCTP:"   << theDeclassificationType
       << setw(24) << "\nFSDCDT:"   << theDeclassificationDate
       << setw(24) << "\nFSDCXM:"   << theDeclassificationExemption
       << setw(24) << "\nFSDG:"     << theDowngrade
       << setw(24) << "\nFSDGDT:"   << theDowngradingDate
       << setw(24) << "\nFSCLTX:"   << theClassificationText
       << setw(24) << "\nFSCATP:"   << theClassificationAuthorityType
       << setw(24) << "\nFSCAUT:"   << theClassificationAuthority
       << setw(24) << "\nFSCRSN:"   << theClassificationReason
       << setw(24) << "\nFSSRDT:"   << theSecuritySourceDate
       << setw(24) << "\nFSCTLN:"   << theSecurityControlNumber
       << setw(24) << "\nFSCOP:"    << theCopyNumber
       << setw(24) << "\nFSCOPYS:"  << theNumberOfCopies
       << setw(24) << "\nENCRYP:"   << theEncryption
       << setw(24) << "\nFBKGC[0]:" << (int)theFileBackgroundColor[0]
       << setw(24) << "\nFBKGC[1]:" << (int)theFileBackgroundColor[1]
       << setw(24) << "\nFBKGC[2]:" << (int)theFileBackgroundColor[2]
       << setw(24) << "\nONAME:"    << theOriginatorsName
       << setw(24) << "\nOPHONE:"   << theOriginatorsPhone
       << setw(24) << "\nFL:"       << theFileLength  
       << setw(24) << "\nHL:"       << theHeaderLength
       << setw(24) << "\nNUMI:"     << theNumberOfImageInfoRecords;

   ossim_uint32 index;

   for (index = 0; index < theNitfImageInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "\nLISH";
      tmpStr += os.str();
      
      out << setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageSubheaderLength;
      tmpStr = "\nLI";
      tmpStr += os.str();

      out << setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageLength;
   }

   out << setw(24) << "\nNUMS:" << theNumberOfGraphicInfoRecords;

   for (index = 0; index < theNitfGraphicInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "\nLSSH";
      tmpStr += os.str();

      out << tmpStr
          << theNitfGraphicInfoRecords[index].theGraphicSubheaderLength;

      tmpStr = "\nLS";
      tmpStr += os.str();

      out << tmpStr 
          << theNitfGraphicInfoRecords[index].theGraphicLength;
   }

   out << setw(24) << "\nNUMX:" << theReservedForFutureUse1
       << setw(24) << "\nNUMT:" 
       << theNumberOfTextFileInfoRecords;
   
   for (index = 0; index < theNitfTextFileInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "\nLTSH";
      tmpStr += os.str();
      
      out << tmpStr
          << theNitfTextFileInfoRecords[index].theTextFileSubheaderLength;

      tmpStr = "\nLT";
      tmpStr += os.str();

      out << setw(24) << tmpStr
          << theNitfTextFileInfoRecords[index].theTextFileLength;
   }

   out << setw(24) << "\nNUMDES:" << theNumberOfDataExtSegInfoRecords;

   for (index = 0; index < theNitfDataExtSegInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "\nLDSH";
      tmpStr += os.str();
      
      out << setw(24) << tmpStr
          << theNitfDataExtSegInfoRecords[index].theDataExtSegSubheaderLength;

      tmpStr = "\nLD";
      tmpStr += os.str();

      out << setw(24) << tmpStr
          << theNitfDataExtSegInfoRecords[index].theDataExtSegLength;
   }

   out << setw(24) << "\nNUMRES:"
       << theNumberOfResExtSegInfoRecords;
   
   for (index = 0; index < theNitfResExtSegInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "\nLRESSH";
      tmpStr += os.str();
      
      out << tmpStr
          << theNitfResExtSegInfoRecords[index].theResExtSegSubheaderLength;

      tmpStr = "\nLRE";
      tmpStr += os.str();

      out << setw(24) << tmpStr
          << theNitfResExtSegInfoRecords[index].theResExtSegLength;
   }
   
   out << setw(24) << "\nUDHDL:"   << theUserDefinedHeaderDataLength
       << setw(24) << "\nUDHOFL:"  << theUserDefinedHeaderOverflow
       << setw(24) << "\nXHDL:"    << theExtendedHeaderDataLength
       << setw(24) << "\nXHDLOFL:" << theExtendedHeaderDataOverflow
       << std::endl;

   out << "TAGS:\n";
   std::copy(theTagList.begin(),
             theTagList.end(),
             std::ostream_iterator<ossimNitfTagInformation>(out,"\n"));
   
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

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfImages()const
{
   return theNitfImageInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfTextSegments()const
{
   return static_cast<ossim_int32>(theNitfTextFileInfoRecords.size());
}

ossim_int32 ossimNitfFileHeaderV2_1::getHeaderSize()const
{
   return theHeaderSize;
}

ossim_int32 ossimNitfFileHeaderV2_1::getFileSize()const
{
   ossimString temp = theFileLength;

   if(temp = "999999999999")
   {
      return -1;
   }
   else
   {
      return temp.toInt32();
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
   memset(theExtendedHeaderDataOverflow, '0', 3);
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
   theReservedForFutureUse1[3] = '\0';
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
   ossim_int32 numberOfImages = ossimString(theNumberOfImageInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfGraphics = ossimString(theNumberOfGraphicInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfTextFiles = ossimString(theNumberOfTextFileInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfDataExtSegs = ossimString(theNumberOfDataExtSegInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfResExtSegs = ossimString(theNumberOfResExtSegInfoRecords).toInt32();
   ossim_int32 index;

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

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfLabels()const
{
   return 0;
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfSymbols()const
{
   return 0;
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfGraphics()const
{
   return 0;
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfDataExtSegments()const
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
   return NULL;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_1::allocateLabelHeader()const
{
   return NULL;
}

void ossimNitfFileHeaderV2_1::initializeAllOffsets()
{
   // this will be a running tally 
   ossim_uint64 tally = theHeaderSize;
   ossim_uint64 idx = 0;

   // clear out all offset inforamtion and begin populating them
   theImageOffsetList.clear();

   for(idx = 0; idx < theNitfImageInfoRecords.size(); ++idx)
   {
      theImageOffsetList.push_back(ossimNitfImageOffsetInformation(tally,
                                                                   tally + (ossim_uint64)theNitfImageInfoRecords[idx].getHeaderLength()));
      tally += theNitfImageInfoRecords[idx].getTotalLength();
   }
}

ossimNitfImageHeader*
ossimNitfFileHeaderV2_1::getNewImageHeader(ossim_int32 imageNumber,
                                           std::istream& in)const
{
   ossimNitfImageHeader *result = NULL;
   
   if((getNumberOfImages() > 0) &&
      (imageNumber < (ossim_int32)theImageOffsetList.size()) &&
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
ossimNitfFileHeaderV2_1::getNewSymbolHeader(ossim_int32 symbolNumber,
                                            std::istream& in)const
{
   // Currently not implemented...
   
   ossimNitfSymbolHeader *result = NULL;
   
   return result;
}

ossimNitfLabelHeader*
ossimNitfFileHeaderV2_1::getNewLabelHeader(ossim_int32 labelNumber,
                                           std::istream& in)const
{
   // Currently not implemented...
   ossimNitfLabelHeader *result = NULL;
   
   return result;
}

ossimNitfTextHeader*
ossimNitfFileHeaderV2_1::getNewTextHeader(ossim_int32 textNumber,
                                          std::istream& in)const
{
   // Currently not implemented...
   ossimNitfTextHeader *result = NULL;
   
   return result;
}

ossimNitfDataExtensionSegment*
ossimNitfFileHeaderV2_1::getNewDataExtensionSegment(ossim_int32 dataExtNumber,
                                                    std::istream& in)const
{
   // Currently not implemented...
   ossimNitfDataExtensionSegment *result = NULL;
   
   return result;
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

void ossimNitfFileHeaderV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();
   if(name == FSCLASY_KW)
   {
     setSecurityClassificationSys(property->valueToString()); 
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
   else if(name == FSCRSN_KW)
   {
      setClassificationReason(property->valueToString());
   }
   else if(name == FSSRDT_KW)
   {
      setSecuritySourceDate(property->valueToString());
   }
   else if(name == FBKGC_KW)
   {
      ossimString value = property->valueToString();
      istringstream in(value);
      ossim_uint8 r, g, b;

      in >> r >> g >> b;
      
      setFileBackgroundColor(r, g, b);
   }
   else
   {
      ossimNitfFileHeaderV2_X::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfFileHeaderV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = NULL;
   ossimStringProperty* stringProperty = NULL;
   ossimColorProperty* colorProperty = NULL;

   if(name == CLEVEL_KW)
   {
      property = new ossimStringProperty(name, getComplexityLevel().trim());
   }
   else if(name == FSCLASY_KW)
   {
      property = new ossimStringProperty(name,
                                               getSecurityClassificationSys().trim());
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
   else if(name == FBKGC_KW)
   {
      ossim_uint8 r, g, b;
      getBackgroundColor(r, g, b);
      
      colorProperty = new ossimColorProperty(name,
                                             ossimRgbVector(r, g, b));

      property = colorProperty;
   }
   else if(name == ONAME_KW)
   {
      property = new ossimStringProperty(name, ossimString(theOriginatorsName).trim());
   }
   else if(name == OPHONE_KW)
   {
      property = new ossimStringProperty(name, ossimString(theOriginatorsPhone).trim());
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

   propertyNames.push_back(FSCLASY_KW);
   propertyNames.push_back(FSDCTP_KW);
   propertyNames.push_back(FSDCDT_KW);
   propertyNames.push_back(FSDCXM_KW);
   propertyNames.push_back(FSDG_KW);
   propertyNames.push_back(FSDGDT_KW);
   propertyNames.push_back(FSCLTX_KW);
   propertyNames.push_back(FSCATP_KW);
   propertyNames.push_back(FSCRSN_KW);
   propertyNames.push_back(FSSRDT_KW);
   propertyNames.push_back(FBKGC_KW);
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

