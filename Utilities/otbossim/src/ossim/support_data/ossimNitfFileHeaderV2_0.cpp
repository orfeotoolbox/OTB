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
// $Id: ossimNitfFileHeaderV2_0.cpp 19058 2011-03-11 20:03:24Z dburken $


#include <sstream>
#include <cstring> // for memset
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_0.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/support_data/ossimNitfSymbolHeaderV2_0.h>
#include <ossim/support_data/ossimNitfLabelHeaderV2_0.h>
#include <ossim/support_data/ossimNitfTextHeaderV2_0.h>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_0.h>

#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStringProperty.h>


const ossimString ossimNitfFileHeaderV2_0::FSDWNG_KW = "fsdwng";
const ossimString ossimNitfFileHeaderV2_0::FSDEVT_KW = "fsdevt";

static const ossimTrace traceDebug("ossimNitfFileHeaderV2_0:debug");

RTTI_DEF1(ossimNitfFileHeaderV2_0, "ossimNitfFileHeaderV2_0", ossimNitfFileHeaderV2_X)

std::ostream& operator <<(std::ostream& out,
                          const ossimNitfImageInfoRecordV2_0 &data)
{
   return out << "theImageSubheaderLength:       "
              << data.theImageSubheaderLength << std::endl
              << "theImageLength:                "
              << data.theImageLength;
}

ossim_uint32 ossimNitfImageInfoRecordV2_0::getHeaderLength()const
{
   return ossimString(theImageSubheaderLength).toInt32();
}

ossim_uint64  ossimNitfImageInfoRecordV2_0::getImageLength()const
{
   return ossimString(theImageLength).toInt32();
}

ossim_uint64 ossimNitfImageInfoRecordV2_0::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
}

void ossimNitfImageInfoRecordV2_0::setSubheaderLength(ossim_uint32 length)
{
   ostringstream out;
   
   out << std::setw(6)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << length;
   
   memcpy(theImageSubheaderLength, out.str().c_str(), 6);
   theImageSubheaderLength[6] = '\0';
}

void ossimNitfImageInfoRecordV2_0::setImageLength(ossim_uint64 length)
{
   ostringstream out;
   
   out << std::setw(10)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << length;
   
   memcpy(theImageLength, out.str().c_str(), 10);
   theImageLength[10] = '\0';
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfSymbolInfoRecordV2_0 &data)
{
   return out << "theSymbolSubheaderLength:       "
              << data.theSymbolSubheaderLength << std::endl
              << "theSymbolLength:                "
              << data.theSymbolLength;
}
ossim_int32 ossimNitfSymbolInfoRecordV2_0::getHeaderLength()const
{
   return ossimString(theSymbolSubheaderLength).toInt32();
}

ossim_int32 ossimNitfSymbolInfoRecordV2_0::getImageLength()const
{
   return ossimString(theSymbolLength).toInt32();
}

ossim_int32 ossimNitfSymbolInfoRecordV2_0::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfLabelInfoRecordV2_0 &data)
{
   return out << "theLabelSubheaderLength:       "
              << data.theLabelSubheaderLength << std::endl
              << "theLabelLength:                "
              << data.theLabelLength;
}

ossim_int32 ossimNitfLabelInfoRecordV2_0::getHeaderLength()const
{
   return ossimString(theLabelSubheaderLength).toInt32();
}

ossim_int32 ossimNitfLabelInfoRecordV2_0::getImageLength()const
{
   return ossimString(theLabelLength).toInt32();
}

ossim_int32 ossimNitfLabelInfoRecordV2_0::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfTextInfoRecordV2_0 &data)
{
   return out << "theTextSubheaderLength:       "
              << data.theTextSubheaderLength << std::endl
              << "theTextLength:                "
              << data.theTextLength;
}

ossim_int32 ossimNitfTextInfoRecordV2_0::getHeaderLength()const
{
   return ossimString(theTextSubheaderLength).toInt32();
}

ossim_int32 ossimNitfTextInfoRecordV2_0::getImageLength()const
{
   return ossimString(theTextLength).toInt32();
}

ossim_int32 ossimNitfTextInfoRecordV2_0::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfDataExtSegInfoRecordV2_0 &data)
{
   return out << "theDataExtSegSubheaderLength:       "
              << data.theDataExtSegSubheaderLength << std::endl
              << "theDataExtSegLength:                "
              << data.theDataExtSegLength;
}

ossim_int32 ossimNitfDataExtSegInfoRecordV2_0::getHeaderLength()const
{
   return ossimString(theDataExtSegSubheaderLength).toInt32();
}

ossim_int32 ossimNitfDataExtSegInfoRecordV2_0::getImageLength()const
{
   return ossimString(theDataExtSegLength).toInt32();
}

ossim_int32 ossimNitfDataExtSegInfoRecordV2_0::getTotalLength()const
{
   return (getHeaderLength() + getImageLength());
}

std::ostream& operator <<(std::ostream& out,
                     const ossimNitfResExtSegInfoRecordV2_0 &data)
{
   return out << "theResExtSegSubheaderLength:       "
              << data.theResExtSegSubheaderLength << std::endl
              << "theResExtSegLength:                "
              << data.theResExtSegLength;
}

ossimNitfFileHeaderV2_0::ossimNitfFileHeaderV2_0()
   :ossimNitfFileHeaderV2_X()
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
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfFileHeaderV2_0::parseStream:   entered ......."
         << std::endl;
   }
   
   clearFields();
   
   // identification and origination group
   in.read(theFileTypeVersion, 9);
   theHeaderSize+=9;
   in.read(theComplexityLevel, 2);
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
   in.read(theCopyNumber, 5);
   theHeaderSize+=5;
   in.read(theNumberOfCopies, 5);
   theHeaderSize+=5;
   in.read(theEncryption, 1);
   theHeaderSize++;
   in.read(theOriginatorsName, 27);
   theHeaderSize+=27;
   in.read(theOriginatorsPhone, 18);
   theHeaderSize+=18;
   
   in.read(theFileLength, 12);
   theHeaderSize+=12;
   in.read(theHeaderLength, 6);
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
   ossim_int32 userDefinedHeaderLength = ossimString(theUserDefinedHeaderDataLength).toInt32();
   
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
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();

         //---
         // We will check the stream here as there have been instances of
         // rpf's with bad stream offsets.
         //---
         if (!in)
         {
            std::string e =
               "ossimNitfFileHeaderV2_0::parseStream stream error!";
            throw ossimException(e);
         }

         current = in.tellg();
      }
   }
   in.read(theExtendedHeaderDataLength, 5);
   theHeaderSize+=5;
   ossim_int32 extendedHeaderDataLength = ossimString(theExtendedHeaderDataLength).toInt32();
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
      ossimNitfFileHeader::print( ossimNotify(ossimNotifyLevel_DEBUG) );
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfFileHeaderV2_0::parseStream:   Leaving......."
         << std::endl;
   }
//      initializeDisplayLevels(in);

}

void ossimNitfFileHeaderV2_0::writeStream(std::ostream &out)
{
   // identification and origination group
   out.write(theFileTypeVersion, 9);
   out.write(theComplexityLevel, 2);
   out.write(theSystemType, 4);
   out.write(theOriginatingStationId, 10);
   out.write(theDateTime, 14);
   out.write(theFileTitle, 80);
   
   // read security group
   out.write(theSecurityClassification, 1);
   out.write(theCodewords, 40);
   out.write(theControlAndHandling, 40);
   out.write(theReleasingInstructions, 40);
   out.write(theClassificationAuthority, 20);
   out.write(theSecurityControlNumber, 20);
   out.write(theSecurityDowngrade, 6);
   if(ossimString(theSecurityDowngrade) == "999998")
   {
      out.write(theDowngradingEvent, 40);
   }      
   out.write(theCopyNumber, 5);
   out.write(theNumberOfCopies, 5);
   out.write(theEncryption, 1);
   out.write(theOriginatorsName, 27);
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
      << theNitfSymbolInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      
      for(idx = 0; idx < theNitfSymbolInfoRecords.size(); ++idx)
      {
         out.write(theNitfSymbolInfoRecords[idx].theSymbolSubheaderLength, 4);
         out.write(theNitfSymbolInfoRecords[idx].theSymbolLength, 6);
      }
   }
   {
      ostringstream outString;
      
      outString << std::setw(3)
      << std::setfill('0')
      << std::setiosflags(ios::right)
      << theNitfLabelInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      
      for(idx = 0; idx < theNitfLabelInfoRecords.size(); ++idx)
      {
         out.write(theNitfLabelInfoRecords[idx].theLabelSubheaderLength, 4);
         out.write(theNitfLabelInfoRecords[idx].theLabelLength, 3);
      }
   }
   {
      ostringstream outString;
      
      outString << std::setw(3)
      << std::setfill('0')
      << std::setiosflags(ios::right)
      << theNitfTextInfoRecords.size();
      
      out.write(outString.str().c_str(), 3);
      
      for(idx = 0; idx < theNitfTextInfoRecords.size(); ++idx)
      {
         out.write(theNitfTextInfoRecords[idx].theTextSubheaderLength, 4);
         out.write(theNitfTextInfoRecords[idx].theTextLength, 5);
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
      
      // for now we hard code te 000 for we do not currently support writing to the DES if the total tag length is
      // larger than supported
      //
      memset(theExtendedHeaderOverflow, '0', 3);
      if(totalLength > 0)
      {
         ossim_uint32 i = 0;
         out.write(theExtendedHeaderOverflow, 3);
         
         for(i = 0; i < theTagList.size(); ++i)
         {
            theTagList[i].writeStream(out);
         }
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_0::writeStream: Only support writing of total tag length < 99999" << std::endl;
   }
}

std::ostream& ossimNitfFileHeaderV2_0::print(std::ostream& out,
                                             const std::string& prefix) const
{
   out << setiosflags(std::ios::left)
       << prefix << std::setw(24) << "FHDR:"
       << theFileTypeVersion << "\n"
       << prefix << std::setw(24) << "CLEVEL:"
       << theComplexityLevel << "\n"
       << prefix << std::setw(24) << "STYPE:"
       << theSystemType  << "\n"    
       << prefix << std::setw(24) << "OSTAID:"
       << theOriginatingStationId << "\n"
       << prefix << std::setw(24) << "FDT:"
       << theDateTime  << "\n"      
       << prefix << std::setw(24) << "FTITLE:"
       << theFileTitle  << "\n"     
       << prefix << std::setw(24) << "FSCLAS:"
       << theSecurityClassification << "\n"
       << prefix << std::setw(24) << "FSCODE:"
       << theCodewords << "\n"
       << prefix << std::setw(24) << "FSCTLH:"
       << theControlAndHandling << "\n"
       << prefix << std::setw(24) << "FSREL:"
       << theReleasingInstructions << "\n"
       << prefix << std::setw(24) << "FSCAUT:"
       << theClassificationAuthority << "\n"
       << prefix << std::setw(24) << "FSCTLN:"
       << theSecurityControlNumber << "\n"
       << prefix << std::setw(24) << "FSDWNG:"
       << theSecurityDowngrade << "\n"
       << prefix << std::setw(24) << "FSDEVT:"
       << theDowngradingEvent << "\n"
       << prefix << std::setw(24) << "FSCOP:"
       << theCopyNumber << "\n"
       << prefix << std::setw(24) << "FSCPYS:"
       << theNumberOfCopies << "\n"
       << prefix << std::setw(24) << "ENCRYP:"
       << theEncryption << "\n"
       << prefix << std::setw(24) << "ONAME:"
       << theOriginatorsName << "\n"
       << prefix << std::setw(24) << "OPHONE:"
       << theOriginatorsPhone << "\n"
       << prefix << std::setw(24) << "FL:"
       << theFileLength << "\n"
       << prefix << std::setw(24) << "HL:"
       << theHeaderLength << "\n"
       << prefix << std::setw(24) << "NUMI:"
       << theNumberOfImageInfoRecords << "\n";

   ossim_uint32 index;
   
   for (index = 0; index < theNitfImageInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";
      
      ossimString tmpStr = "LISH";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageSubheaderLength << "\n";
      tmpStr = "LI";
      tmpStr += os.str();
      
      out << prefix << std::setw(24) << tmpStr
          << theNitfImageInfoRecords[index].theImageLength << "\n";
   }

   out << prefix << std::setw(24) << "NUMS:" << theNumberOfSymbolInfoRecords
       << "\n";

   for (index = 0; index < theNitfSymbolInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LSSH";
      tmpStr += os.str();

      out << tmpStr
          << theNitfSymbolInfoRecords[index].theSymbolSubheaderLength << "\n";

      tmpStr = "LS";
      tmpStr += os.str();

      out << tmpStr 
          << theNitfSymbolInfoRecords[index].theSymbolLength << "\n";
   }

   
   out << prefix << std::setw(24) << "NUML:" << theNumberOfLabelInfoRecords
       << "\n";

   for (index = 0; index < theNitfLabelInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LLSH";
      tmpStr += os.str();

      out << tmpStr
          << theNitfLabelInfoRecords[index].theLabelSubheaderLength  << "\n";

      tmpStr = "LL";
      tmpStr += os.str();

      out << tmpStr 
          << theNitfLabelInfoRecords[index].theLabelLength << "\n";
   }

   out << prefix << std::setw(24) << "NUMT:" << theNumberOfTextFileInfoRecords
       << "\n";

   for (index = 0; index < theNitfTextInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LTSH";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfTextInfoRecords[index].theTextSubheaderLength << "\n";

      tmpStr = "LT";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr 
          << theNitfTextInfoRecords[index].theTextLength<< "\n";
   }

   out << prefix << std::setw(24) << "NUMDES:"
       << theNumberOfDataExtSegInfoRecords << "\n";

   for (index = 0; index < theNitfDataExtSegInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LDSH";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr
          << theNitfDataExtSegInfoRecords[index].theDataExtSegSubheaderLength
          << "\n";

      tmpStr = "LD";
      tmpStr += os.str();

      out << prefix << std::setw(24) << tmpStr 
          << theNitfDataExtSegInfoRecords[index].theDataExtSegLength << "\n";
   }

   out << prefix << std::setw(24) << "NUMRES:"
       << theNumberOfResExtSegInfoRecords << "\n";

   for (index = 0; index < theNitfResExtSegInfoRecords.size(); ++index)
   {
      std::ostringstream os;
      os << std::setw(3) << std::setfill('0') << (index+1) << ":";

      ossimString tmpStr = "LRSH";
      tmpStr += os.str();

      out << tmpStr
          << theNitfResExtSegInfoRecords[index].theResExtSegSubheaderLength
          << "\n";

      tmpStr = "LR";
      tmpStr += os.str();

      out << tmpStr 
          << theNitfResExtSegInfoRecords[index].theResExtSegLength
          << "\n";
   }

   out << prefix << std::setw(24) << "UDHDL:"
       << theUserDefinedHeaderDataLength << "\n"
       << prefix << std::setw(24) << "UDHOFL:"
       << theUserDefinedHeaderOverflow << "\n"
       << prefix << std::setw(24) << "XHDL:"
       << theExtendedHeaderDataLength << "\n";
   
   return ossimNitfFileHeader::print(out, prefix);
}


bool ossimNitfFileHeaderV2_0::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   bool result = ossimNitfFileHeaderV2_X::saveState(kwl, prefix);
   
   if(result)
   {
      kwl.add(prefix, "FSCODE",theCodewords);
      kwl.add(prefix, "FSCTLH",theControlAndHandling);
      kwl.add(prefix, "FSREL",theReleasingInstructions);
      kwl.add(prefix, "FSCAUT",theClassificationAuthority);
      kwl.add(prefix, "FSCTLN",theSecurityControlNumber);
      kwl.add(prefix, "FSDWNG",theSecurityDowngrade);
      kwl.add(prefix, "FSDEVT",theDowngradingEvent);
      kwl.add(prefix, "ONAME",theOriginatorsName);
      kwl.add(prefix, "OPHONE",theOriginatorsPhone);
      kwl.add(prefix, "FL",theFileLength);
      kwl.add(prefix, "HL",theHeaderLength);
      kwl.add(prefix, "NUMI",theNumberOfImageInfoRecords);
      kwl.add(prefix, "UDHDL",theUserDefinedHeaderDataLength);
      kwl.add(prefix, "UDHOFL",theUserDefinedHeaderDataLength);
      kwl.add(prefix, "XHDL",theExtendedHeaderDataLength);

      std::ostringstream out;
      ossim_uint32 index;
      for (index = 0; index < theNitfImageInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LISH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfImageInfoRecords[index].theImageSubheaderLength << "\n";
         tmpStr = "LI";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfImageInfoRecords[index].theImageLength << "\n";
      }
      
      out <<"NUMS:" << theNumberOfSymbolInfoRecords
      << "\n";
      
      for (index = 0; index < theNitfSymbolInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LSSH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfSymbolInfoRecords[index].theSymbolSubheaderLength << "\n";
         
         tmpStr = "LS";
         tmpStr += os.str();
         
         out << tmpStr 
         << theNitfSymbolInfoRecords[index].theSymbolLength << "\n";
      }
      
      
      out << "NUML:" << theNumberOfLabelInfoRecords
      << "\n";
      
      for (index = 0; index < theNitfLabelInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LLSH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfLabelInfoRecords[index].theLabelSubheaderLength  << "\n";
         
         tmpStr = "LL";
         tmpStr += os.str();
         
         out << tmpStr 
         << theNitfLabelInfoRecords[index].theLabelLength << "\n";
      }
      
      out << "NUMT:" << theNumberOfTextFileInfoRecords
      << "\n";
      
      for (index = 0; index < theNitfTextInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LTSH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfTextInfoRecords[index].theTextSubheaderLength << "\n";
         
         tmpStr = "LT";
         tmpStr += os.str();
         
         out << tmpStr 
         << theNitfTextInfoRecords[index].theTextLength<< "\n";
      }
      
      out << "NUMDES:"
      << theNumberOfDataExtSegInfoRecords << "\n";
      
      for (index = 0; index < theNitfDataExtSegInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LDSH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfDataExtSegInfoRecords[index].theDataExtSegSubheaderLength
         << "\n";
         
         tmpStr = "LD";
         tmpStr += os.str();
         
         out << tmpStr 
         << theNitfDataExtSegInfoRecords[index].theDataExtSegLength << "\n";
      }
      
      out << "NUMRES:"
      << theNumberOfResExtSegInfoRecords << "\n";
      
      for (index = 0; index < theNitfResExtSegInfoRecords.size(); ++index)
      {
         std::ostringstream os;
         os << std::setw(3) << std::setfill('0') << (index+1) << ":";
         
         ossimString tmpStr = "LRSH";
         tmpStr += os.str();
         
         out << tmpStr
         << theNitfResExtSegInfoRecords[index].theResExtSegSubheaderLength
         << "\n";
         
         tmpStr = "LR";
         tmpStr += os.str();
         
         out << tmpStr 
         << theNitfResExtSegInfoRecords[index].theResExtSegLength
         << "\n";
      }
      
      {
         std::istringstream in(out.str());
         ossimKeywordlist tempKwl;
         if(tempKwl.parseStream(in))
         {
            result = true;
            kwl.add(prefix, tempKwl);
         }
      }
   }
   
   return result;
}

ossimDrect ossimNitfFileHeaderV2_0::getImageRect()const
{
   return theImageRect;
}

void ossimNitfFileHeaderV2_0::addImageInfoRecord(const ossimNitfImageInfoRecordV2_0& recordInfo)
{
   theNitfImageInfoRecords.push_back(recordInfo);
   
   setNumberOfImageInfoRecords(theNitfImageInfoRecords.size());
}

void ossimNitfFileHeaderV2_0::replaceImageInfoRecord(ossim_uint32 i, const ossimNitfImageInfoRecordV2_0& recordInfo)
{
   theNitfImageInfoRecords[i]=recordInfo;
}

ossimNitfImageHeader*
ossimNitfFileHeaderV2_0::getNewImageHeader(ossim_uint32 imageNumber,
                                           std::istream& in)const
{
   ossimNitfImageHeader *result = 0;
   
   if( (getNumberOfImages() > 0) && (imageNumber < theImageOffsetList.size()) )
   {
      result = allocateImageHeader();
      in.seekg(theImageOffsetList[imageNumber].theImageHeaderOffset, std::ios::beg);
      result->parseStream(in);
   }
   else
   {
#if 0
      ossimNotify(ossimNotifyLevel_FATAL) << "ossimNitfFileHeaderV2_0::getNewImageHeader ERROR:"
                                          << "\nNo images in file or image number (" << imageNumber
                                          << ") is out of range!\n";
#endif
   }
   
   return result;
}

ossimNitfSymbolHeader *ossimNitfFileHeaderV2_0::getNewSymbolHeader(
   ossim_uint32 symbolNumber, std::istream& in)const
{
   ossimNitfSymbolHeader *result = 0;

   if( (getNumberOfSymbols() > 0) &&
       (symbolNumber < theSymbolOffsetList.size()) )
   {
      result = allocateSymbolHeader();
      in.seekg(theSymbolOffsetList[symbolNumber].theSymbolHeaderOffset, std::ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfLabelHeader *ossimNitfFileHeaderV2_0::getNewLabelHeader(
   ossim_uint32 labelNumber, std::istream& in)const
{
   ossimNitfLabelHeader *result = 0;

   if( (getNumberOfLabels() > 0) &&
       (labelNumber < theLabelOffsetList.size()) )
   {
      result = allocateLabelHeader();
      in.seekg(theLabelOffsetList[labelNumber].theLabelHeaderOffset, std::ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfTextHeader *ossimNitfFileHeaderV2_0::getNewTextHeader(
   ossim_uint32 textNumber, std::istream& in)const
{
   ossimNitfTextHeader *result = 0;

   if( (getNumberOfTextSegments() > 0) &&
       (textNumber < theTextOffsetList.size()) )
   {
      result = allocateTextHeader();
      in.seekg(theTextOffsetList[textNumber].theTextHeaderOffset, std::ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

ossimNitfDataExtensionSegment*
ossimNitfFileHeaderV2_0::getNewDataExtensionSegment(
    ossim_uint32 dataExtNumber, std::istream& in)const
{
   ossimNitfDataExtensionSegment *result = 0;

   if( (getNumberOfDataExtSegments() > 0) &&
       (dataExtNumber < theNitfDataExtSegInfoRecords.size()) )
   {
      result = allocateDataExtSegment();
      in.seekg(theDataExtSegOffsetList[dataExtNumber].theDataExtSegHeaderOffset, std::ios::beg);
      result->parseStream(in);
   }
   
   return result;
}

void ossimNitfFileHeaderV2_0::initializeDisplayLevels(std::istream& in)
{
   // we will need to temporarily save the get pointer since
   // initializeDisplayLevels changes it.
   std::streampos saveTheGetPointer = in.tellg();
   
   std::vector<ossimNitfImageOffsetInformation>::iterator imageOffsetList = theImageOffsetList.begin();

   // allocate temporary space to store image headers
   ossimNitfImageHeader* imageHeader = allocateImageHeader();

   // clear the list
   theDisplayInformationList.clear();
   
   theImageRect = ossimDrect(0,0,0,0);
   if(!imageHeader)
   {
      return;
   }
   
   ossim_uint32 idx = 0;
   while(imageOffsetList != theImageOffsetList.end())
   {
      // position the get pointer in the input
      // stream to the start of the image header
      in.seekg((*imageOffsetList).theImageHeaderOffset, std::ios::beg);
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
                                                     idx));
      
      ++imageOffsetList;
      ++idx;                                       
   }
   delete imageHeader;
   imageHeader = 0;


   
   // finally we reset the saved get state back
   // to its original position
   in.seekg(saveTheGetPointer, std::ios::beg);
}

void ossimNitfFileHeaderV2_0::insertIntoDisplayInfoList(const ossimNitfDisplayInfo &displayInformation)
{
   std::vector<ossimNitfDisplayInfo>::iterator displayList = theDisplayInformationList.begin();

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
   ossim_uint64 tally = theHeaderSize;
   ossim_uint32 idx = 0;

   // clear out all offset inforamtion and begin populating them
   theImageOffsetList.clear();
   theSymbolOffsetList.clear();
   theLabelOffsetList.clear();
   theLabelOffsetList.clear();
   

   for(idx = 0; idx < theNitfImageInfoRecords.size(); ++idx)
   {
      theImageOffsetList.push_back(ossimNitfImageOffsetInformation(tally,
                                                                   tally + theNitfImageInfoRecords[idx].getHeaderLength()));
      tally += theNitfImageInfoRecords[idx].getTotalLength();
   }
   for(idx = 0; idx < theNitfSymbolInfoRecords.size(); ++idx)
   {
      theSymbolOffsetList.push_back(ossimNitfSymbolOffsetInformation(tally,
                                                                     tally + theNitfSymbolInfoRecords[idx].getHeaderLength()));
      tally += theNitfSymbolInfoRecords[idx].getTotalLength();
   }

   for(idx = 0; idx < theNitfLabelInfoRecords.size(); ++idx)
   {
      theLabelOffsetList.push_back(ossimNitfLabelOffsetInformation(tally,
                                                                   tally + theNitfLabelInfoRecords[idx].getHeaderLength()));
      tally += theNitfLabelInfoRecords[idx].getTotalLength();
   }

   for(idx = 0; idx < theNitfTextInfoRecords.size(); ++idx)
   {
      theTextOffsetList.push_back(ossimNitfTextOffsetInformation(tally,
                                                                 tally + theNitfTextInfoRecords[idx].getHeaderLength()));
      tally += theNitfTextInfoRecords[idx].getTotalLength();
   }

   for(idx = 0; idx < theNitfDataExtSegInfoRecords.size(); ++idx)
   {
      theDataExtSegOffsetList.push_back(ossimNitfDataExtSegOffsetInformation(tally,
                                                                             tally + theNitfDataExtSegInfoRecords[idx].getHeaderLength()));
      tally += theNitfDataExtSegInfoRecords[idx].getTotalLength();
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

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfImages()const
{
   return (ossim_int32)theNitfImageInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfLabels()const
{
   return ((ossim_int32)theNitfLabelInfoRecords.size());
}

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfSymbols()const
{
   return ((ossim_int32)theNitfSymbolInfoRecords.size());
}

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfGraphics()const
{
   return 0;
}

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfTextSegments()const
{
   return (ossim_int32)theNitfTextInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_0::getNumberOfDataExtSegments()const
{
   return (ossim_int32)theNitfDataExtSegInfoRecords.size();
}

ossim_int32 ossimNitfFileHeaderV2_0::getHeaderSize()const
{
   return theHeaderSize;
}

ossim_int64 ossimNitfFileHeaderV2_0::getFileSize()const
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
   memcpy(theFileTypeVersion, "NITF02.00", 9);
   memset(theComplexityLevel, ' ', 2);
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
   memset(theCopyNumber, ' ', 5);
   memset(theNumberOfCopies, ' ', 5);
   memset(theEncryption, ' ', 1);
   memset(theOriginatorsName, ' ', 27);
   memset(theOriginatorsPhone, ' ', 18);
   memset(theFileLength, ' ', 12);
   memset(theHeaderLength, ' ', 6);
   memset(theNumberOfImageInfoRecords, ' ', 3);
   memset(theNumberOfSymbolInfoRecords, ' ', 3);
   memset(theNumberOfLabelInfoRecords, ' ', 3);
   memset(theNumberOfTextFileInfoRecords, ' ', 3);
   memset(theNumberOfDataExtSegInfoRecords, ' ', 3);
   memset(theNumberOfResExtSegInfoRecords, ' ', 3);
   memset(theUserDefinedHeaderDataLength, '0', 5);
   memset(theUserDefinedHeaderOverflow, ' ', 3);
   memset(theExtendedHeaderDataLength, '0', 5);
   memset(theExtendedHeaderOverflow, ' ', 3);
   
   theFileTypeVersion[9] = '\0';
   theComplexityLevel[2] = '\0';
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
   theCopyNumber[5] = '\0';
   theNumberOfCopies[5] = '\0';
   theEncryption[1] = '\0';
   theOriginatorsName[27] = '\0';
   theOriginatorsPhone[18] = '\0';
   theFileLength[12]  = '\0';
   theHeaderLength[6] = '\0';
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

void ossimNitfFileHeaderV2_0::setNumberOfImageInfoRecords(ossim_uint64 num)
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
      std::string s = "ossimNitfFileHeaderV2_0::setNumberOfImageInfoRecords:";
      s += " ERROR\nExceeded max image info number of 999!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
}

void ossimNitfFileHeaderV2_0::readImageInfoRecords(std::istream &in)
{
   ossim_int32 numberOfImages = ossimString(theNumberOfImageInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfSymbols = ossimString(theNumberOfSymbolInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfLabels = ossimString(theNumberOfLabelInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfTextFiles = ossimString(theNumberOfTextFileInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfDataExtSegs = ossimString(theNumberOfDataExtSegInfoRecords).toInt32();
   ossim_int32 index;

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
   ossim_int32 numberOfResExtSegs = ossimString(theNumberOfResExtSegInfoRecords).toInt32();
   ossim_int32 index;

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

void ossimNitfFileHeaderV2_0::setComplianceLevel(const ossimString& complianceLevel)
{
   ossimNitfCommon::setField(theComplexityLevel, complianceLevel, 2);
}

void ossimNitfFileHeaderV2_0::setCodeWords(const ossimString& codeWords)
{
   ossimNitfCommon::setField(theCodewords, codeWords, 40);
}

void ossimNitfFileHeaderV2_0::setControlAndHandling(const ossimString& controlAndHandling)
{
   ossimNitfCommon::setField(theControlAndHandling, controlAndHandling, 40);
}

void ossimNitfFileHeaderV2_0::setReleasingInstructions(const ossimString& releasingInstructions)
{
   ossimNitfCommon::setField(theReleasingInstructions, releasingInstructions, 40);
}

void ossimNitfFileHeaderV2_0::setClassificationAuthority(const ossimString& classAuth)
{
   ossimNitfCommon::setField(theClassificationAuthority, classAuth, 20);
}

void ossimNitfFileHeaderV2_0::setSecurityControlNumber(const ossimString& controlNo)
{
   ossimNitfCommon::setField(theSecurityControlNumber, controlNo, 20);
}

void ossimNitfFileHeaderV2_0::setOriginatorsName(const ossimString& originatorName)
{
   ossimNitfCommon::setField(theOriginatorsName, originatorName, 27);
}

void ossimNitfFileHeaderV2_0::setOriginatorsPhone(const ossimString& originatorPhone)
{
   ossimNitfCommon::setField(theOriginatorsPhone, originatorPhone, 18);
}

void ossimNitfFileHeaderV2_0::setSecurityDowngrade(const ossimString& securityDowngrade)
{
   ossimNitfCommon::setField(theSecurityDowngrade, securityDowngrade, 6);
}

void ossimNitfFileHeaderV2_0::setDowngradingEvent(const ossimString& downgradeEvent)
{
   ossimNitfCommon::setField(theDowngradingEvent, downgradeEvent, 40);
}

void ossimNitfFileHeaderV2_0::setFileLength(ossim_uint64 fileLength)
{
   ostringstream out;
   
   out << std::setw(12)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << fileLength;
   
   memcpy(theFileLength, out.str().c_str(), 12);
}

void ossimNitfFileHeaderV2_0::setHeaderLength(ossim_uint64 headerLength)
{
   ostringstream out;
   
   out << std::setw(6)
   << std::setfill('0')
   << std::setiosflags(ios::right)
   << headerLength;
   
   memcpy(theHeaderLength, out.str().c_str(), 6);
}

ossimString ossimNitfFileHeaderV2_0::getComplianceLevel()const
{
   return theComplexityLevel;
}

ossimString ossimNitfFileHeaderV2_0::getSecurityDowngrade()const
{
   return theSecurityDowngrade;
}

ossimString ossimNitfFileHeaderV2_0::getDowngradingEvent()const
{
   return theDowngradingEvent;
}

ossimString ossimNitfFileHeaderV2_0::getCodeWords()const
{
   return theCodewords;
}

ossimString ossimNitfFileHeaderV2_0::getControlAndHandling()const
{
   return theControlAndHandling;
}

ossimString ossimNitfFileHeaderV2_0::getReleasingInstructions()const
{
   return theReleasingInstructions;
}

ossimString ossimNitfFileHeaderV2_0::getClassificationAuthority()const
{
   return theClassificationAuthority;
}

ossimString ossimNitfFileHeaderV2_0::getSecurityControlNumber()const
{
   return theSecurityControlNumber;
}

ossimString ossimNitfFileHeaderV2_0::getOriginatorsName()const
{
   return theOriginatorsName;
}

ossimString ossimNitfFileHeaderV2_0::getOriginatorsPhone()const
{
   return theOriginatorsPhone;
}

void ossimNitfFileHeaderV2_0::setProperty(ossimRefPtr<ossimProperty> property)
{
   const ossimString& name = property->getName();
   if(name == CLEVEL_KW)
   {
      setComplianceLevel(property->valueToString());
   }
   else if(name == FSDWNG_KW)
   {
      setSecurityDowngrade(property->valueToString());
   }
   else if(name == FSDEVT_KW)
   {
      setDowngradingEvent(property->valueToString());
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
      ossimNitfFileHeaderV2_X::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfFileHeaderV2_0::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;

	
   if(name == CLEVEL_KW)
   {
      property = new ossimStringProperty(name, ossimString(theComplexityLevel).trim());
   }
   else if(name == FSDWNG_KW)
   {
      property = new ossimStringProperty(name, ossimString(theSecurityDowngrade).trim());
   }
   else if(name == FSDEVT_KW)
   {
      property = new ossimStringProperty(name, ossimString(theDowngradingEvent).trim());
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

void ossimNitfFileHeaderV2_0::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfFileHeaderV2_X::getPropertyNames(propertyNames);
}

