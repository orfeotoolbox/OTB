//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFileHeaderV2_1.cpp 22418 2013-09-26 15:01:12Z gpotts $

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring> // for memset

#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfTextHeaderV2_0.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimColorProperty.h>
#include <ossim/base/ossimDateProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_X.h>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_1.h> // ??? drb


RTTI_DEF1(ossimNitfFileHeaderV2_1,
          "ossimNitfFileHeaderV2_1",
          ossimNitfFileHeaderV2_X)

const ossimString ossimNitfFileHeaderV2_1::FSCLSY_KW  = "FSCLSY";
const ossimString ossimNitfFileHeaderV2_1::FSDCTP_KW  = "FSDCTP";
const ossimString ossimNitfFileHeaderV2_1::FSDCDT_KW  = "FSDCDT";
const ossimString ossimNitfFileHeaderV2_1::FSDCXM_KW  = "FSDCXM";
const ossimString ossimNitfFileHeaderV2_1::FSDG_KW    = "FSDG";
const ossimString ossimNitfFileHeaderV2_1::FSDGDT_KW  = "FSDGDT";
const ossimString ossimNitfFileHeaderV2_1::FSCLTX_KW  = "FSCLTX";
const ossimString ossimNitfFileHeaderV2_1::FSCATP_KW  = "FSCATP";
const ossimString ossimNitfFileHeaderV2_1::FSCRSN_KW  = "FSCRSN";
const ossimString ossimNitfFileHeaderV2_1::FSSRDT_KW  = "FSSRDT";
const ossimString ossimNitfFileHeaderV2_1::FBKGC_KW   = "FBKGC";

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

ossim_uint64 ossimNitfImageInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theImageSubheaderLength).toUInt64();
}

ossim_uint64 ossimNitfImageInfoRecordV2_1::getImageLength()const
{
   return ossimString(theImageLength).toUInt64();
}

ossim_uint64 ossimNitfImageInfoRecordV2_1::getTotalLength()const
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
ossim_uint64 ossimNitfGraphicInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theGraphicSubheaderLength).toUInt64();
}

ossim_uint64 ossimNitfGraphicInfoRecordV2_1::getGraphicLength()const
{
   return ossimString(theGraphicLength).toUInt64();
}

ossim_uint64 ossimNitfGraphicInfoRecordV2_1::getTotalLength()const
{
   return getGraphicLength() + getHeaderLength();
}

void ossimNitfTextFileInfoRecordV2_1::setSubheaderLength(ossim_uint64 length)
{
   ostringstream out;
   
   out << std::setw(4)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << length;
   
   memcpy(theTextFileSubheaderLength, out.str().c_str(), 4);
   theTextFileSubheaderLength[4] = '\0';
}

void ossimNitfTextFileInfoRecordV2_1::setTextLength(ossim_uint64 length)
{
   ostringstream out;
   
   out << std::setw(5)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << length;
   
   memcpy(theTextFileLength, out.str().c_str(), 5);
   theTextFileLength[5] = '\0';
}

ossim_uint64 ossimNitfTextFileInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theTextFileSubheaderLength).toUInt64();
}

ossim_uint64 ossimNitfTextFileInfoRecordV2_1::getTextLength()const
{
   return ossimString(theTextFileLength).toUInt64();
}

ossim_uint64 ossimNitfTextFileInfoRecordV2_1::getTotalLength()const
{
   return (getHeaderLength() + getTextLength());
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

ossim_uint64 ossimNitfDataExtSegInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theDataExtSegSubheaderLength).toUInt64();
}

ossim_uint64 ossimNitfDataExtSegInfoRecordV2_1::getDataExtSegLength()const
{
   return ossimString(theDataExtSegLength).toUInt64();
}

ossim_uint64 ossimNitfDataExtSegInfoRecordV2_1::getTotalLength()const
{
   return getDataExtSegLength() + getHeaderLength();
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

ossim_uint64 ossimNitfResExtSegInfoRecordV2_1::getHeaderLength()const
{
   return ossimString(theResExtSegSubheaderLength).toUInt64();
}

ossim_uint64 ossimNitfResExtSegInfoRecordV2_1::getResExtSegLength()const
{
   return ossimString(theResExtSegLength).toUInt64();
}

ossim_uint64 ossimNitfResExtSegInfoRecordV2_1::getTotalLength()const
{
   return getResExtSegLength() + getHeaderLength();
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

void ossimNitfImageInfoRecordV2_1::setImageLength(ossim_uint64 length)
{
   ostringstream out;

   out << std::setw(10)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << length;
   
   memcpy(theImageLength, out.str().c_str(), 10);
   theImageLength[10] = '\0';
}

// 

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
      current = in.tellg();
         
      while((current - start) < userDefinedHeaderLength)
      {
         headerTag.parseStream(in);
         headerTag.setTagType("UDHD");
         theTagList.push_back(headerTag);
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
      //in.seekg(start + userDefinedHeaderLength);
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
      current = in.tellg();
      while((current - start) < extendedHeaderDataLength)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
      theHeaderSize += extendedHeaderDataLength;
      //in.seekg(start + extendedHeaderDataLength);
   }

   // this need to be re-thought
   initializeAllOffsets();
   readOverflowTags(in);
}

void ossimNitfFileHeaderV2_1::readOverflowTags(istream& in)
{
   ossim_int32 overflow = ossimString(theUserDefinedHeaderOverflow).toInt32();
   if (overflow != 0)
   {
      ossimNitfDataExtensionSegment *des = getNewDataExtensionSegment(overflow-1, in);
      if (des != NULL)
      {
         const vector<ossimNitfTagInformation> &desTags = des->getTagList();
         for (vector<ossimNitfTagInformation>::const_iterator iter = desTags.begin(); iter != desTags.end(); ++iter)
         {
            iter->setTagType("UDHD");
            theTagList.push_back(*iter);
         }
      }
      delete des;
   }

   overflow = ossimString(theExtendedHeaderDataOverflow).toInt32();
   if (overflow != 0)
   {
      ossimNitfDataExtensionSegment *des = getNewDataExtensionSegment(overflow-1, in);
      if (des != NULL)
      {
         const vector<ossimNitfTagInformation> &desTags = des->getTagList();
         for (vector<ossimNitfTagInformation>::const_iterator iter = desTags.begin(); iter != desTags.end(); ++iter)
         {
            iter->setTagType("XHD");
            theTagList.push_back(*iter);
         }
      }
      delete des;
   }
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
/*
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

      // for now we hard code the 000 for we do not currently support writing to the DES if the total tag length is
      // larger than supported
      //
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
   */
      ossim_uint32 totalLength = ossimString(theUserDefinedHeaderDataLength).toUInt32();
   if (totalLength > 0)
   {
      totalLength += 3;
   }

   // Scope tempOut
   {
      std::ostringstream tempOut;
      tempOut << std::setw(5)
              << std::setfill('0')
              << std::setiosflags(ios::right)
              << totalLength;

      out.write(tempOut.str().c_str(), 5);
   }

   if (totalLength > 0)
   {
      if(totalLength <= 99999)
      {
         out.write(theUserDefinedHeaderOverflow, 3);

         for (unsigned int i = 0; i < theTagList.size(); ++i)
         {
            if (theTagList[i].getTagType() == "UDHD")
            {
               theTagList[i].writeStream(out);
            }
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_1::writeStream: Only support writing of total tag length <= 99999" << std::endl;
      }
   }

   totalLength = ossimString(theExtendedHeaderDataLength).toUInt32();
   if (totalLength > 0)
   {
      totalLength += 3;
   }

   // Scope tempOut
   {
      std::ostringstream tempOut;
      tempOut << std::setw(5)
              << std::setfill('0')
              << std::setiosflags(ios::right)
              << totalLength;

      out.write(tempOut.str().c_str(), 5);
   }

   if (totalLength > 0)
   {
      if(totalLength <= 99999)
      {
         out.write(theExtendedHeaderDataOverflow, 3);

         for(unsigned int i = 0; i < theTagList.size(); ++i)
         {
            if (theTagList[i].getTagType() == "XHD")
            {
               theTagList[i].writeStream(out);
            }
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_1::writeStream: Only support writing of total tag length <= 99999" << std::endl;
      }
   }

}

std::ostream& ossimNitfFileHeaderV2_1::print(std::ostream& out,
                                             const std::string& prefix) const
{
   out << setiosflags(ios::left)
       << prefix << std::setw(24) << "FHDR:"
       << theFileTypeVersion << "\n"
       << prefix << std::setw(24) << "CLEVEL:"
       << theComplexityLevel << "\n"
       << prefix << std::setw(24) << "STYPE:"
       << theSystemType << "\n"
       << prefix << std::setw(24) << "OSTAID:"
       << theOriginatingStationId << "\n"
       << prefix << std::setw(24) << "FDT:"
       << theDateTime << "\n"      
       << prefix << std::setw(24) << "FTITLE:"
       << theFileTitle << "\n"    
       << prefix << std::setw(24) << "FSCLAS:"
       << theSecurityClassification<< "\n"
       << prefix << std::setw(24) << "FSCLSY:"
       << theSecurityClassificationSys<< "\n"
       << prefix << std::setw(24) << "FSCODE:"
       << theCodewords << "\n"
       << prefix << std::setw(24) << "FSCTLH:"
       << theControlAndHandling << "\n"
       << prefix << std::setw(24) << "FSREL:"
       << theReleasingInstructions << "\n"
       << prefix << std::setw(24) << "FSDCTP:"
       << theDeclassificationType << "\n"
       << prefix << std::setw(24) << "FSDCDT:"
       << theDeclassificationDate << "\n"
       << prefix << std::setw(24) << "FSDCXM:"
       << theDeclassificationExemption << "\n"
       << prefix << std::setw(24) << "FSDG:"
       << theDowngrade << "\n"
       << prefix << std::setw(24) << "FSDGDT:"
       << theDowngradingDate << "\n"
       << prefix << std::setw(24) << "FSCLTX:"
       << theClassificationText << "\n"
       << prefix << std::setw(24) << "FSCATP:"
       << theClassificationAuthorityType << "\n"
       << prefix << std::setw(24) << "FSCAUT:"
       << theClassificationAuthority << "\n"
       << prefix << std::setw(24) << "FSCRSN:"
       << theClassificationReason << "\n"
       << prefix << std::setw(24) << "FSSRDT:"
       << theSecuritySourceDate << "\n"
       << prefix << std::setw(24) << "FSCTLN:"
       << theSecurityControlNumber << "\n"
       << prefix << std::setw(24) << "FSCOP:"
       << theCopyNumber << "\n"
       << prefix << std::setw(24) << "FSCOPYS:"
       << theNumberOfCopies << "\n"
       << prefix << std::setw(24) << "ENCRYP:"
       << theEncryption << "\n"
       << prefix << std::setw(24) << "FBKGC[0]:"
       << (int)theFileBackgroundColor[0] << "\n"
       << prefix << std::setw(24) << "FBKGC[1]:"
       << (int)theFileBackgroundColor[1] << "\n"
       << prefix << std::setw(24) << "FBKGC[2]:"
       << (int)theFileBackgroundColor[2] << "\n"
       << prefix << std::setw(24) << "ONAME:"
       << theOriginatorsName<< "\n"
       << prefix << std::setw(24) << "OPHONE:"
       << theOriginatorsPhone << "\n"
       << prefix << std::setw(24) << "FL:"
       << theFileLength  << "\n"
       << prefix << std::setw(24) << "HL:"
       << theHeaderLength << "\n"
       << prefix << std::setw(24) << "NUMI:"
       << theNumberOfImageInfoRecords << "\n";

   ossim_uint32 index;

   for (index = 0; index < theNitfImageInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LISH";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageSubheaderLength << "\n";
      tmpStr = "LI";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageLength << "\n";
   }

   out << prefix << std::setw(24) << "NUMS:" << theNumberOfGraphicInfoRecords
       << "\n";
 
   for (index = 0; index < theNitfGraphicInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LSSH";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfGraphicInfoRecords[index].theGraphicSubheaderLength
          << "\n";

      tmpStr = "LS";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr 
          << theNitfGraphicInfoRecords[index].theGraphicLength
          << "\n";
   }

   out << prefix << std::setw(24) << "NUMX:" << theReservedForFutureUse1 << "\n"
       << prefix << std::setw(24) << "NUMT:" 
       << theNumberOfTextFileInfoRecords << "\n";
   
   for (index = 0; index < theNitfTextFileInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LTSH";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfTextFileInfoRecords[index].theTextFileSubheaderLength
          << "\n";

      tmpStr = "LT";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfTextFileInfoRecords[index].theTextFileLength << "\n";
   }

   out << prefix << std::setw(24) << "NUMDES:" << theNumberOfDataExtSegInfoRecords
       << "\n";

   for (index = 0; index < theNitfDataExtSegInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LDSH";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfDataExtSegInfoRecords[index].theDataExtSegSubheaderLength
          << "\n";

      tmpStr = "LD";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfDataExtSegInfoRecords[index].theDataExtSegLength
          << "\n";
   }

   out << prefix << std::setw(24) << "NUMRES:"
       << theNumberOfResExtSegInfoRecords << "\n";
   
   for (index = 0; index < theNitfResExtSegInfoRecords.size(); ++index)
   {
      ostringstream os;
      os << setw(3) << setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LRESSH";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfResExtSegInfoRecords[index].theResExtSegSubheaderLength
          << "\n";

      tmpStr = "LRE";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfResExtSegInfoRecords[index].theResExtSegLength
          << "\n";
   }
   
   out << prefix << std::setw(24) << "UDHDL:"
       << theUserDefinedHeaderDataLength
       << "\n"
       << prefix << std::setw(24) << "UDHOFL:"
       << theUserDefinedHeaderOverflow
       << "\n"
       << prefix << std::setw(24) << "XHDL:"
       << theExtendedHeaderDataLength
       << "\n"
       << prefix << std::setw(24) << "XHDLOFL:"
       << theExtendedHeaderDataOverflow
       << "\n";

   return ossimNitfFileHeader::print(out, prefix);
}

ossimNitfImageHeader* ossimNitfFileHeaderV2_1::allocateImageHeader()const
{
   return new ossimNitfImageHeaderV2_1;
}

ossimNitfTextHeader *ossimNitfFileHeaderV2_1::allocateTextHeader()const
{
   return new ossimNitfTextHeaderV2_0;
}

ossimNitfDataExtensionSegment* ossimNitfFileHeaderV2_1::allocateDataExtSegment()const
{
   return new ossimNitfDataExtensionSegmentV2_1;
}

bool ossimNitfFileHeaderV2_1::isEncrypted()const
{
   return (theEncryption[0]=='1');
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfImages()const
{
   return (ossim_int32)theNitfImageInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfTextSegments()const
{
   return static_cast<ossim_int32>(theNitfTextFileInfoRecords.size());
}

ossim_int32 ossimNitfFileHeaderV2_1::getHeaderSize()const
{
   return theHeaderSize;
}

ossim_int64 ossimNitfFileHeaderV2_1::getFileSize()const
{
   ossimString temp = theFileLength;

   if(temp == "999999999999")
   {
      return -1;
   }
   else
   {
      return temp.toInt64();
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
   memset(theCopyNumber, '0', 5);
   memset(theNumberOfCopies, '0', 5);
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
   return theNitfGraphicInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfDataExtSegments()const
{
   return theNitfDataExtSegInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_1::getNumberOfReservedExtSegments()const
{
   return theNitfResExtSegInfoRecords.size();
}

const char* ossimNitfFileHeaderV2_1::getDateTime()const
{
   return theDateTime;
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

void ossimNitfFileHeaderV2_1::addTextInfoRecord(const ossimNitfTextFileInfoRecordV2_1& recordInfo)
{
   theNitfTextFileInfoRecords.push_back(recordInfo);

   setNumberOfTextInfoRecords(theNitfTextFileInfoRecords.size());
}

void ossimNitfFileHeaderV2_1::addDataExtSegInfoRecord(const ossimNitfDataExtSegInfoRecordV2_1& recordInfo)
{
   theNitfDataExtSegInfoRecords.push_back(recordInfo);

   setNumberOfDataExtSegInfoRecords(theNitfDataExtSegInfoRecords.size());
}


void ossimNitfFileHeaderV2_1::replaceImageInfoRecord(int i, const ossimNitfImageInfoRecordV2_1& recordInfo)
{
   if ( i < static_cast<int>(theNitfImageInfoRecords.size()) )
   {
      theNitfImageInfoRecords[i] = recordInfo;
   }
}

ossimNitfSymbolHeader *ossimNitfFileHeaderV2_1::allocateSymbolHeader()const
{
   return 0;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_1::allocateLabelHeader()const
{
   return 0;
}

void ossimNitfFileHeaderV2_1::initializeAllOffsets()
{
   // this will be a running tally 
   ossim_uint64 tally = theHeaderSize;
   ossim_uint64 idx = 0;

   // clear out all offset inforamtion and begin populating them
   theImageOffsetList.clear();
   theGraphicOffsetList.clear();
   theTextFileOffsetList.clear();
   theDataExtSegOffsetList.clear();

   for(idx = 0; idx < theNitfImageInfoRecords.size(); ++idx)
   {
      theImageOffsetList.push_back(ossimNitfImageOffsetInformation(tally,
                                                                   tally + theNitfImageInfoRecords[idx].getHeaderLength()));
      tally += theNitfImageInfoRecords[idx].getTotalLength();
   }
   for(idx = 0; idx < theNitfGraphicInfoRecords.size(); ++idx)
   {
      theGraphicOffsetList.push_back(ossimNitfGraphicOffsetInformation(tally,
                                                                     tally + theNitfGraphicInfoRecords[idx].getHeaderLength()));
      tally += theNitfGraphicInfoRecords[idx].getTotalLength();
   }

   for(idx = 0; idx < theNitfTextFileInfoRecords.size(); ++idx)
   {
      theTextFileOffsetList.push_back(ossimNitfTextOffsetInformation(tally,
                                                                     tally + theNitfTextFileInfoRecords[idx].getHeaderLength()));
      tally += theNitfTextFileInfoRecords[idx].getTotalLength();
   }

   for(idx = 0; idx < theNitfDataExtSegInfoRecords.size(); ++idx)
   {
      theDataExtSegOffsetList.push_back(ossimNitfDataExtSegOffsetInformation(tally,
                                                                             tally + theNitfDataExtSegInfoRecords[idx].getHeaderLength()));
      tally += theNitfDataExtSegInfoRecords[idx].getTotalLength();
   }
}

ossimNitfImageHeader*
ossimNitfFileHeaderV2_1::getNewImageHeader(ossim_uint32 imageNumber,
                                           std::istream& in)const
{
   ossimNitfImageHeader *result = 0;
   
   if( (getNumberOfImages() > 0) && (imageNumber < theImageOffsetList.size()) )
   {
      result = allocateImageHeader();
      in.seekg(theImageOffsetList[imageNumber].theImageHeaderOffset, ios::beg);
      result->parseStream(in);
   }
   else
   {
#if 0
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimNitfFileHeaderV2_1::getNewImageHeader: "
                                          << "\nNo images in file or image number (" << imageNumber
                                          << ") is out of range!\n";
#endif
   }
   
   return result;
}

ossimNitfSymbolHeader*
ossimNitfFileHeaderV2_1::getNewSymbolHeader(ossim_uint32 /* symbolNumber */,
                                            std::istream& /* in */)const
{
   // Currently not implemented...
   
   ossimNitfSymbolHeader *result = 0;


   
   return result;
}

ossimNitfLabelHeader*
ossimNitfFileHeaderV2_1::getNewLabelHeader(ossim_uint32 /* labelNumber */,
                                           std::istream& /* in */)const
{
   // Currently not implemented...
   ossimNitfLabelHeader *result = 0;
   
   return result;
}

ossimNitfTextHeader*
ossimNitfFileHeaderV2_1::getNewTextHeader(ossim_uint32 /* textNumber */,
                                          std::istream& /* in */)const
{
   // Currently not implemented...
   ossimNitfTextHeader *result = 0;
   
   return result;
}

ossimNitfDataExtensionSegment*
ossimNitfFileHeaderV2_1::getNewDataExtensionSegment(
   ossim_int32 dataExtNumber, std::istream&  in )const
{
   ossimNitfDataExtensionSegment *result = 0;

   if((getNumberOfDataExtSegments() > 0) &&
      (dataExtNumber < (ossim_int32)theNitfDataExtSegInfoRecords.size()) &&
      (dataExtNumber >= 0))
   {
      result = allocateDataExtSegment();
      ossimIFStream64::seekg64(in, theDataExtSegOffsetList[dataExtNumber].theDataExtSegHeaderOffset, ios::beg);
      result->parseStream(in, theNitfDataExtSegInfoRecords[dataExtNumber].getDataExtSegLength());
   }
   
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



void ossimNitfFileHeaderV2_1::setNumberOfGraphicInfoRecords(ossim_uint64 num)
	{
		if (num < 1000)
   {
      ostringstream out;
      
      out << std::setw(3)
          << std::setfill('0')
          << std::setiosflags(ios::right)
          << num;
      
      memcpy(theNumberOfGraphicInfoRecords, out.str().c_str(), 3);
   }
   else
   {
      std::string s = "ossimNitfFileHeaderV2_1::setNumberOfGraphicInfoRecords:";
      s += " ERROR\nExceeded max number of 999!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
		
	}

void ossimNitfFileHeaderV2_1::setNumberOfDataExtSegInfoRecords(ossim_uint64 num)
{
   if (num < 1000)
   {
      ostringstream out;
      
      out << std::setw(3)
          << std::setfill('0')
          << std::setiosflags(ios::right)
          << num;
      
      memcpy(theNumberOfDataExtSegInfoRecords, out.str().c_str(), 3);
   }
   else
   {
      std::string s = "ossimNitfFileHeaderV2_1::setNumberOfDataExtSegInfoRecords:";
      s += " ERROR\nExceeded max number of 999!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
}


void ossimNitfFileHeaderV2_1::setNumberOfTextInfoRecords(ossim_uint64 num)
{
   if (num < 1000)
   {
      ostringstream out;
      
      out << std::setw(3)
          << std::setfill('0')
          << std::setiosflags(ios::right)
          << num;
      
      memcpy(theNumberOfTextFileInfoRecords, out.str().c_str(), 3);
   }
   else
   {
      std::string s = "ossimNitfFileHeaderV2_1::setNumberOfTextRecords:";
      s += " ERROR\nExceeded max number of 999!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
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

   memcpy(theSecurityClassificationSys, out.str().c_str(), 2);
}

void ossimNitfFileHeaderV2_1::setCodeWords(const ossimString& codeWords)
{
   std::ostringstream out;
   
   out << std::setw(11)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(codeWords).trim();

   memcpy(theCodewords, out.str().c_str(), 11);
}

void ossimNitfFileHeaderV2_1::setControlAndHandling(const ossimString& controlAndHandling)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(controlAndHandling).trim();

   memcpy(theControlAndHandling, out.str().c_str(), 2);
}

void ossimNitfFileHeaderV2_1::setReleasingInstructions(const ossimString& releasingInstructions)
{
   std::ostringstream out;
   
   out << std::setw(20)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << ossimString(releasingInstructions).trim();

   memcpy(theReleasingInstructions, out.str().c_str(), 20);
}

void ossimNitfFileHeaderV2_1::setDeclassificationType(const ossimString& declassType)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << declassType.trim();

   memcpy(theDeclassificationType, out.str().c_str(), 2);
}

void ossimNitfFileHeaderV2_1::setDeclassificationDate(const ossimLocalTm& d)
{
   memcpy(theDeclassificationDate, formatDate(getVersion(), d).c_str(), 8);
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
   memcpy(theDowngradingDate, formatDate(getVersion(), d).c_str(), 8);
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
   memcpy(theSecuritySourceDate, formatDate(getVersion(), d).c_str(), 8);
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

bool ossimNitfFileHeaderV2_1::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   // Note: Currently not looking up all fieds only ones that make sense.
   
   const char* lookup;

   lookup = kwl.find( prefix, FSCLSY_KW);
   if ( lookup )
   {
      setSecurityClassificationSys( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSDCTP_KW);
   if ( lookup )
   {
      setDeclassificationType( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSDCDT_KW);
   if ( lookup )
   {
      setDeclassificationDate( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSDCXM_KW);
   if ( lookup )
   {
      setDeclassificationExemption( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSDG_KW);
   if ( lookup )
   {
      setDowngrade( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSDGDT_KW);
   if ( lookup )
   {
      setDowngradingDate( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSCLTX_KW );
   if ( lookup )
   {
      setClassificationText( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSCATP_KW );
   if ( lookup )
   {
      setClassificationAuthorityType( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSCRSN_KW );
   if ( lookup )
   {
      setClassificationReason( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSSRDT_KW );
   if ( lookup )
   {
      setClassificationReason( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FSSRDT_KW);
   if ( lookup )
   {
      setSecuritySourceDate( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, FBKGC_KW );
   if ( lookup )
   {
      ossimString value = lookup;
      std::vector<ossimString> splitString;
      value = value.trim();
      value.split(splitString, " ");
      if(splitString.size() == 3)
      {
         setFileBackgroundColor((ossim_uint8)splitString[0].toUInt32(), 
                                (ossim_uint8)splitString[1].toUInt32(), 
                                (ossim_uint8)splitString[2].toUInt32());
      }
   }  
   
   return ossimNitfFileHeaderV2_X::loadState(kwl, prefix);
}

void ossimNitfFileHeaderV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();

   // Make case insensitive:
   name.upcase();
   
   if(name == FSCLSY_KW)
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
      ossimColorProperty* colorProp = PTR_CAST(ossimColorProperty, property.get());
      if (colorProp)
      {
         ossim_uint8 r = colorProp->getRed();
         ossim_uint8 g = colorProp->getGreen();
         ossim_uint8 b = colorProp->getBlue();

         setFileBackgroundColor(r, g, b);
      }
      else
      {
        ossimString value = property->valueToString();
        std::vector<ossimString> splitString;
        value = value.trim();
        value.split(splitString, " ");
        if(splitString.size() == 3)
        {
           setFileBackgroundColor((ossim_uint8)splitString[0].toUInt32(), 
                                  (ossim_uint8)splitString[1].toUInt32(), 
                                  (ossim_uint8)splitString[2].toUInt32());
        }
      }

   }
   else
   {
      ossimNitfFileHeaderV2_X::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfFileHeaderV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;
   ossimStringProperty* stringProperty = 0;
   ossimColorProperty* colorProperty = 0;

   if(name == CLEVEL_KW)
   {
      property = new ossimStringProperty(name, getComplexityLevel().trim());
   }
   else if(name == FSCLSY_KW)
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
      return ossimNitfFileHeaderV2_X::getProperty(name);
   }
   return property;
}


void ossimNitfFileHeaderV2_1::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfFileHeaderV2_X::getPropertyNames(propertyNames);

   propertyNames.push_back(FSCLSY_KW);
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

void ossimNitfFileHeaderV2_1::addTag(ossimNitfTagInformation tag, bool unique)
{
   if (unique)
   {
      removeTag(tag.getTagName());
   }
   theTagList.push_back(tag);
}

void ossimNitfFileHeaderV2_1::removeTag(const ossimString& tagName)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theTagList.size(); ++idx)
   {
      if(theTagList[idx].getTagName() == tagName)
      {
         theTagList.erase(theTagList.begin() + idx);
         return;
      }
   }
}

bool ossimNitfFileHeaderV2_1::takeOverflowTags(std::vector<ossimNitfTagInformation> &overflowTags,
   ossim_uint32 potentialDesIndex, bool userDefinedTags)
{
   overflowTags.clear();
   std::vector<ossimNitfTagInformation>::iterator iter;
   std::vector<ossimNitfTagInformation> specifiedTags;
   const ossimString tagType(userDefinedTags ? "UDHD" : "XHD");
   for (iter = theTagList.begin(); iter != theTagList.end(); ++iter)
   {
      if (iter->getTagType() == tagType)
      {
         specifiedTags.push_back(*iter);
      }
   }

   std::sort(specifiedTags.begin(), specifiedTags.end());

   ossim_uint32 totalSize = 0;
   const ossim_uint32 maxSize = 9996;
   for (iter = specifiedTags.begin(); iter != specifiedTags.end() &&
      totalSize + iter->getTotalTagLength() <= maxSize; ++iter)
   {
      totalSize += iter->getTotalTagLength();
   }

   for (; iter != specifiedTags.end(); ++iter)
   {
      overflowTags.push_back(*iter);
      removeTag(iter->getTagName());
   }

   // If there are no overflow tags, then no DES is required
   if (overflowTags.empty() == true)
   {
      potentialDesIndex = 0;
   }

   std::ostringstream overflowDes;
   overflowDes << std::setw(3)
           << std::setfill('0')
           << std::setiosflags(ios::right)
           << potentialDesIndex;

   std::ostringstream tagLength;
   tagLength << std::setw(5)
           << std::setfill('0')
           << std::setiosflags(ios::right)
           << totalSize;

   // Even if no overflow tags exist, update the fields
   if (userDefinedTags)
   {
      strcpy(theUserDefinedHeaderOverflow, overflowDes.str().c_str());
      strcpy(theUserDefinedHeaderDataLength, tagLength.str().c_str());
   }
   else
   {
      strcpy(theExtendedHeaderDataOverflow, overflowDes.str().c_str());
      strcpy(theExtendedHeaderDataLength, tagLength.str().c_str());
   }

   return (overflowTags.empty() == false);
}

