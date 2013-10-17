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
// $Id: ossimNitfImageHeaderV2_1.cpp 22418 2013-09-26 15:01:12Z gpotts $
#include <sstream>
#include <iomanip>
#include <cstring> // for memset

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/support_data/ossimNitfImageHeaderV2_1.h>
#include <ossim/support_data/ossimNitfVqCompressionHeader.h>
#include <ossim/support_data/ossimNitfFileHeaderV2_1.h>
#include <ossim/support_data/ossimNitfDataExtensionSegmentV2_1.h>

RTTI_DEF1(ossimNitfImageHeaderV2_1,
          "ossimNitfImageHeaderV2_1",
          ossimNitfImageHeaderV2_X)

const ossimString ossimNitfImageHeaderV2_1::ISCLSY_KW = "isclsy";
const ossimString ossimNitfImageHeaderV2_1::ISCODE_KW = "iscode";
const ossimString ossimNitfImageHeaderV2_1::ISCTLH_KW = "isctlh";
const ossimString ossimNitfImageHeaderV2_1::ISREL_KW = "isrel";
const ossimString ossimNitfImageHeaderV2_1::ISDCTP_KW = "isdctp";
const ossimString ossimNitfImageHeaderV2_1::ISDCDT_KW = "isdcdt";
const ossimString ossimNitfImageHeaderV2_1::ISDCXM_KW = "isdcxm";
const ossimString ossimNitfImageHeaderV2_1::ISDG_KW = "isdg";
const ossimString ossimNitfImageHeaderV2_1::ISDGDT_KW = "isdgdt";
const ossimString ossimNitfImageHeaderV2_1::ISCLTX_KW = "iscltx";
const ossimString ossimNitfImageHeaderV2_1::ISCATP_KW = "iscatp";
const ossimString ossimNitfImageHeaderV2_1::ISCAUT_KW = "iscaut";
const ossimString ossimNitfImageHeaderV2_1::ISCRSN_KW = "iscrsn";
const ossimString ossimNitfImageHeaderV2_1::ISSRDT_KW = "issrdt";
const ossimString ossimNitfImageHeaderV2_1::ISCTLN_KW = "isctln";
const ossimString ossimNitfImageHeaderV2_1::XBANDS_KW = "xbands";

static const
ossimTrace traceDebug(ossimString("ossimNitfImageHeaderV2_1:debug"));

ossimNitfImageHeaderV2_1::ossimNitfImageHeaderV2_1()
{
   clearFields();
}
ossimNitfImageHeaderV2_1::~ossimNitfImageHeaderV2_1()
{
}

void ossimNitfImageHeaderV2_1::parseStream(std::istream &in)
{
   parseStream(in, NULL);
}

void ossimNitfImageHeaderV2_1::parseStream(std::istream &in, const ossimNitfFileHeaderV2_1 *file)
{
   if (!in)
   {
      return;
   }
   clearFields();
   theTagList.clear();
   in.read(theType, 2);
   in.read(theImageId, 10);
   in.read(theDateTime,14);
   in.read(theTargetId, 17);
   in.read(theTitle, 80);
   in.read(theSecurityClassification, 1);
   in.read(theSecurityClassificationSys, 2);
   in.read(theCodewords, 11);
   in.read(theControlAndHandling, 2);
   in.read(theReleasingInstructions, 20);
   in.read(theDeclassificationType, 2);
   in.read(theDeclassificationDate, 8);
   in.read(theDeclassificationExempt, 4);
   in.read(theDowngrade, 1);
   in.read(theDowngradeDate, 8);
   in.read(theClassificationText, 43);
   in.read(theClassificationAuthType, 1);
   in.read(theClassificationAuthority, 40);
   in.read(theClassificationReason, 1);
   in.read(theSecuritySourceDate, 8);
   in.read(theSecurityControlNumber, 15);
   in.read(theEncryption, 1);
   in.read(theImageSource, 42);
   in.read(theSignificantRows, 8);
   in.read(theSignificantCols, 8);
   in.read(thePixelValueType, 3);
   in.read(theRepresentation, 8);
   in.read(theCategory, 8);
   in.read(theActualBitsPerPixelPerBand, 2);
   in.read(theJustification, 1);
   in.read(theCoordinateSystem, 1);
   // if it's not blank then read coordinates
   if(theCoordinateSystem[0] != ' ')
   {
      in.read(theGeographicLocation, 60);
   }
   in.read(theNumberOfComments, 1);
   ossim_int32 numberOfComments = ossimString(theNumberOfComments).toInt32();
   
   // for now just ignore the comments
   if(numberOfComments > 0)
   {
      in.ignore(numberOfComments*80);
   }
   in.read(theCompression, 2);
   
   // only need the Rate code if its not
   // NC (No compression)
   ossimString temp = theCompression;
   if((temp != "NC") &&
      (temp != "NM"))
   {
      in.read(theCompressionRateCode, 4);
   }
   in.read(theNumberOfBands, 1);
   // check to see if multi spectral bands
   // exceed 9.  A value of 0 indicates this
   ossim_int32 numberOfBands = ossimString(theNumberOfBands).toInt32();
   if (numberOfBands == 0)
   {
      in.read(theNumberOfMultispectralBands, 5);
      numberOfBands = ossimString(theNumberOfMultispectralBands).toInt32();
   }
   theImageBands.clear();
   theImageBands.resize(numberOfBands);
   for (ossim_int32 band = 0; band < numberOfBands; ++band)
   {
      theImageBands[band] = new ossimNitfImageBandV2_1;
      theImageBands[band]->parseStream(in);
   }
   in.read(theImageSyncCode, 1);
   in.read(theImageMode, 1);
   in.read(theNumberOfBlocksPerRow, 4);
   in.read(theNumberOfBlocksPerCol, 4);
   in.read(theNumberOfPixelsPerBlockHoriz, 4);
   in.read(theNumberOfPixelsPerBlockVert, 4);
   in.read(theNumberOfBitsPerPixelPerBand, 2);
   in.read(theDisplayLevel, 3);
   in.read(theAttachmentLevel, 3);
   in.read(theImageLocation, 10);
   in.read(theImageMagnification, 4);
   in.read(theUserDefinedImageDataLength, 5);
   
   ossimNitfTagInformation         headerTag;
   std::streampos start   = in.tellg();
   std::streampos current = in.tellg();
   
   std::streampos userDefinedDataLen
      = ossimString(theUserDefinedImageDataLength).toInt32();
   if(userDefinedDataLen > 0)
   {
      in.read(theUserDefinedOverflow, 3);
      current = in.tellg();
      
      while((current - start) < userDefinedDataLen)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         headerTag.setTagType("UDID");
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
//      in.seekg(start + userDefinedDataLen);
            ossim_int32 overflow = ossimString(theUserDefinedOverflow).toLong();
      if (overflow != 0 && file != NULL)
      {
         ossimNitfDataExtensionSegment *des = file->getNewDataExtensionSegment(overflow-1, in);
         if (des != NULL)
         {
            const vector<ossimNitfTagInformation> &desTags = des->getTagList();
            for (vector<ossimNitfTagInformation>::const_iterator iter = desTags.begin(); iter != desTags.end(); ++iter)
            {
               iter->setTagType("UDID");
               theTagList.push_back(*iter);
            }
         }
         delete des;
      }
      ossimIFStream64::seekg64(in, start + userDefinedDataLen, ios::beg);
   }
   in.read(theExtendedSubheaderDataLen, 5);
   std::streampos extSubHeadLen = ossimString(theExtendedSubheaderDataLen).toInt32();
   start   = in.tellg();
   current = in.tellg();
   if(extSubHeadLen > 0)
   {
      in.read(theExtendedSubheaderOverflow, 3);
      current = in.tellg();
      while((current - start) < extSubHeadLen)
      {
         headerTag.parseStream(in);
         headerTag.setTagType("IXSHD");
         theTagList.push_back(headerTag);
         
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
//      in.seekg(start + extSubHeadLen);
      ossim_int32 overflow = ossimString(theExtendedSubheaderOverflow).toLong();
      if (overflow != 0 && file != NULL)
      {
         ossimNitfDataExtensionSegment *des = file->getNewDataExtensionSegment(overflow-1, in);
         if (des != NULL)
         {
            const vector<ossimNitfTagInformation> &desTags = des->getTagList();
            for (vector<ossimNitfTagInformation>::const_iterator iter = desTags.begin(); iter != desTags.end(); ++iter)
            {
               iter->setTagType("IXSHD");
               theTagList.push_back(*iter);
            }
            delete des;
         }
      }
      ossimIFStream64::seekg64(in, start + extSubHeadLen, ios::beg);
   }
   ossimString compressionType = theCompression;
   compressionType = compressionType.trim().upcase();
   ossimEndian endian;

   //---
   // Note: "C4" added to skip over the image data mask subheader.
   // See MIL-STD-2500C paragraph 5.4.3.2
   //---
   if((compressionType == "NM")||
      (compressionType == "M1")||
      (compressionType == "M3")||
      (compressionType == "M4")||
      (compressionType == "M5")||
      (compressionType == "C4"))
   {
      ossim_uint64 locationBefore = in.tellg();
      in.read((char*)(&theBlockedImageDataOffset), 4);
      in.read((char*)(&theBlockMaskRecordLength),2);
      in.read((char*)(&thePadPixelMaskRecordLength), 2);
      in.read((char*)(&theTransparentOutputPixelCodeLength), 2);

      
      if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
      {
         endian.swap(theBlockedImageDataOffset);
         endian.swap(theBlockMaskRecordLength);
         endian.swap(thePadPixelMaskRecordLength);
         endian.swap(theTransparentOutputPixelCodeLength);
      }
      if(theTransparentOutputPixelCodeLength <9)
      {
         if(theTransparentOutputPixelCodeLength != 0)
         {
            ossim_uint8 padOutputPixelCode;
            in.read((char*)(&padOutputPixelCode), 1);
            thePadOutputPixelCode = padOutputPixelCode;
         }
      }
      else
      {
         in.read((char*)(&thePadOutputPixelCode), 2);
         if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
         {
            endian.swap(thePadOutputPixelCode);
         }
         // I need to add code here to check for justification when its 2 bytes
         // but the code length is less than 16 bits.
         //
      }
      if(theBlockMaskRecordLength>0)
      {
         ossim_uint32 totalNumber = 0;
         if(theImageMode[0] == 'S')
         {
             totalNumber = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol()*getNumberOfBands();
         }
         else
         {
            totalNumber = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol();
         }
         ossim_uint32 *blockRead = new ossim_uint32[totalNumber];
         ossim_uint32 idx = 0;
         theBlockMaskRecords.resize(totalNumber);
         in.read((char*)(blockRead), totalNumber*4);
         for(idx = 0; idx < totalNumber; ++idx)
         {
            if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
            {
               endian.swap(blockRead[idx]);
            }
            theBlockMaskRecords[idx] = blockRead[idx];
         }
         delete [] blockRead;
      }
      if( (thePadPixelMaskRecordLength > 0) ||
          ( (getCompressionCode().upcase() == "M3") && (thePadPixelMaskRecordLength == 0) ) )
      {
         ossim_uint32 totalNumber = 0;
         if(theImageMode[0] == 'S')
         {
            totalNumber = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol()*getNumberOfBands();
         }
         else
         {
            totalNumber = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol();
         }
         ossim_uint32 *blockRead = new ossim_uint32[totalNumber];
         ossim_uint32 idx = 0;
         thePadPixelMaskRecords.resize(totalNumber);
         in.read((char*)(blockRead), totalNumber*4);
         for(idx = 0; idx < totalNumber; ++idx)
         {
            if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
            {
               endian.swap(blockRead[idx]);
            }
            thePadPixelMaskRecords[idx] = blockRead[idx];
         }
         delete [] blockRead;
      }
      theCompressionHeader = 0;
      if((getCompressionCode() == "C4")||
         (getCompressionCode() == "M4"))
      {
         ossimRefPtr<ossimNitfVqCompressionHeader> compressionHeader = new ossimNitfVqCompressionHeader;
         compressionHeader->parseStream(in);
         // do a check to see if the compression header is good
         //
         if(compressionHeader->getCompressionAlgorithmId()!= 1)
         {
            compressionHeader = 0;
         }
         theCompressionHeader = compressionHeader.get();
      }
      
      ossim_uint64 delta = (ossim_uint64)in.tellg() - locationBefore;
      if(delta < theBlockedImageDataOffset)
      {
         in.ignore(theBlockedImageDataOffset-delta);
      }
   }
   //***
   // The stream should now be at the start of the data location so capture
   // it.
   //***
   theDataLocation = in.tellg();
}

void ossimNitfImageHeaderV2_1::writeStream(std::ostream &out)
{
   out.write(theType, 2);
   out.write(theImageId, 10);
   out.write(theDateTime,14);
   out.write(theTargetId, 17);
   out.write(theTitle, 80);
   out.write(theSecurityClassification, 1);
   out.write(theSecurityClassificationSys, 2);
   out.write(theCodewords, 11);
   out.write(theControlAndHandling, 2);
   out.write(theReleasingInstructions, 20);
   out.write(theDeclassificationType, 2);
   out.write(theDeclassificationDate, 8);
   out.write(theDeclassificationExempt, 4);
   out.write(theDowngrade, 1);
   out.write(theDowngradeDate, 8);
   out.write(theClassificationText, 43);
   out.write(theClassificationAuthType, 1);
   out.write(theClassificationAuthority, 40);
   out.write(theClassificationReason, 1);
   out.write(theSecuritySourceDate, 8);
   out.write(theSecurityControlNumber, 15);
   out.write(theEncryption, 1);
   out.write(theImageSource, 42);
   out.write(theSignificantRows, 8);
   out.write(theSignificantCols, 8);
   out.write(thePixelValueType, 3);
   out.write(theRepresentation, 8);
   out.write(theCategory, 8);
   out.write(theActualBitsPerPixelPerBand, 2);
   out.write(theJustification, 1);
   out.write(theCoordinateSystem, 1);
   
   if(theCoordinateSystem[0] != ' ')
   {
      out.write(theGeographicLocation, 60);
   }
   // for now force the number of comments to be 0
   out.write(theNumberOfComments, 1);
   
   out.write(theCompression, 2);
   ossimString compressionTest = theCompression;
   if(compressionTest != "NC" &&
      compressionTest != "NM")
     {
      out.write(theCompressionRateCode, 4);
     }

   out.write(theNumberOfBands, 1);

   if(ossimString(theNumberOfBands).toInt32() == 0)
   {
      out.write(theNumberOfMultispectralBands, 5);
   }

   if(theImageBands.size())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < theImageBands.size(); ++idx)
      {
         if (theImageBands[idx].valid() == false)
         {
            theImageBands[idx] = new ossimNitfImageBandV2_1;
         }
         theImageBands[idx]->writeStream(out);
      }
   }

   out.write(theImageSyncCode, 1);
   out.write(theImageMode, 1);
   out.write(theNumberOfBlocksPerRow, 4);
   out.write(theNumberOfBlocksPerCol, 4);
   out.write(theNumberOfPixelsPerBlockHoriz, 4);
   out.write(theNumberOfPixelsPerBlockVert, 4);
   out.write(theNumberOfBitsPerPixelPerBand, 2);
   out.write(theDisplayLevel, 3);
   out.write(theAttachmentLevel, 3);
   out.write(theImageLocation, 10);
   out.write(theImageMagnification, 4);
   out.write(theUserDefinedImageDataLength, 5);

   if(ossimString(theUserDefinedImageDataLength).toInt32() > 0)
   {
      out.write(theUserDefinedOverflow, 3);
   }

   // need to ouput tagged data
   // here
   //
   ossim_uint32 totalLength = getTotalTagLength();

   if (totalLength == 0)
   {
      out.write(theExtendedSubheaderDataLen, 5);
   }
   else
   {
      totalLength += 3; // per Table A-3 of MIL-STD-2500B
      
      if(totalLength <= 99999)
      {
         std::ostringstream tempOut;
         
         tempOut << std::setw(5)
                 << std::setfill('0')
                 << std::setiosflags(ios::right)
                 << totalLength;
         
         memcpy(theExtendedSubheaderDataLen, tempOut.str().c_str(), 5);
         
         out.write(theExtendedSubheaderDataLen, 5);
         memset(theExtendedSubheaderOverflow, '0', 3);
         
         if(totalLength > 0)
         {
            out.write(theExtendedSubheaderOverflow, 3);
            
            ossim_uint32 i = 0;
            
            for(i = 0; i < theTagList.size(); ++i)
            {
               theTagList[i].writeStream(out);
            }
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING ossimNitfFileHeaderV2_1::writeStream: Only support writing of total tag length < 99999" << std::endl;
      }
   }
}

std::ostream& ossimNitfImageHeaderV2_1::print(std::ostream& out,
                                              const std::string& prefix) const
{
   out << setiosflags(ios::left)
       << prefix << setw(24)
       << "IM:"     << theType << "\n"
       << prefix << setw(24)
       << "IID1:"   << theImageId << "\n"
       << prefix << setw(24)
       << "IDATIM:" << theDateTime << "\n"
       << prefix << setw(24)
       << "TGTID:"  << theTargetId << "\n"
       << prefix << setw(24)
       << "IID2:"   << theTitle << "\n"
       << prefix << setw(24)
       << "ISCLAS:" << theSecurityClassification << "\n"
       << prefix << setw(24)
       << "ISCLSY:" << theSecurityClassificationSys << "\n"
       << prefix << setw(24)
       << "ISCODE:" << theCodewords << "\n"
       << prefix << setw(24)
       << "ISCTLH:" << theControlAndHandling << "\n"
       << prefix << setw(24)
       << "ISREL:"  << theReleasingInstructions << "\n"
       << prefix << setw(24)
       << "ISDCTP:" << theDeclassificationType << "\n"
       << prefix << setw(24)
       << "ISDCDT:" << theDeclassificationDate << "\n"
       << prefix << setw(24)
       << "ISDCXM:" << theDeclassificationExempt << "\n"
       << prefix << setw(24)
       << "ISDG:"   << theDowngrade << "\n"
       << prefix << setw(24)
       << "ISDGDT:" << theDowngradeDate << "\n"
       << prefix << setw(24)
       << "ISCLTX:" << theClassificationText << "\n"
       << prefix << setw(24)
       << "ISCATP:" << theClassificationAuthType << "\n"
       << prefix << setw(24)
       << "ISCAUT:" << theClassificationAuthority << "\n"
       << prefix << setw(24)
       << "ISCRSN:" << theClassificationReason << "\n"
       << prefix << setw(24)
       << "ISSRDT:" << theSecuritySourceDate << "\n"
       << prefix << setw(24)
       << "ISCTLN:" << theSecurityControlNumber << "\n"
       << prefix << setw(24)
       << "ENCRYP:" << theEncryption << "\n"
       << prefix << setw(24)
       << "ISORCE:" << theImageSource << "\n"
       << prefix << setw(24)
       << "NROWS:"  << theSignificantRows << "\n"
       << prefix << setw(24)
       << "NCOLS:"  << theSignificantCols << "\n"
       << prefix << setw(24)
       << "PVTYPE:" << thePixelValueType << "\n"
       << prefix << setw(24)
       << "IREP:"   << theRepresentation << "\n"
       << prefix << setw(24)
       << "ICAT:"   << theCategory << "\n"
       << prefix << setw(24)
       << "ABPP:"   << theActualBitsPerPixelPerBand << "\n"
       << prefix << setw(24)
       << "PJUST:"  << theJustification << "\n"
       << prefix << setw(24)
       << "ICORDS:" << theCoordinateSystem << "\n"
       << prefix << setw(24)
       << "IGEOLO:" << theGeographicLocation << "\n"
       << prefix << setw(24)
       << "NICOM:"  << theNumberOfComments << "\n"
       << prefix << setw(24)
       << "IC:"     << theCompression << "\n"
       << prefix << setw(24)
       << "COMRAT:" << theCompressionRateCode << "\n"
       << prefix << setw(24)
       << "NBANDS:" << theNumberOfBands << "\n"
       << prefix << setw(24)
       << "XBANDS:" << theNumberOfMultispectralBands << "\n";
   
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theImageBands.size(); ++idx)
   {
      if(theImageBands[idx].valid())
      {
         theImageBands[idx]->print(out, prefix, idx); 
      }
   }

   out << prefix << setw(24)
       << "ISYNC:"     << theImageSyncCode << "\n"
       << prefix << setw(24)
       << "IMODE:"     << theImageMode << "\n"
       << prefix << setw(24)
       << "NBPR:"      << theNumberOfBlocksPerRow << "\n"
       << prefix << setw(24)
       << "NBPC:"      << theNumberOfBlocksPerCol << "\n"
       << prefix << setw(24)
       << "NPPBH:"     << theNumberOfPixelsPerBlockHoriz << "\n"
       << prefix << setw(24)
       << "NPPBV:"     << theNumberOfPixelsPerBlockVert << "\n"
       << prefix << setw(24)
       << "NBPP:"      << theNumberOfBitsPerPixelPerBand << "\n"
       << prefix << setw(24)
       << "IDLVL:"     << theDisplayLevel << "\n"
       << prefix << setw(24)
       << "IALVL:"     << theAttachmentLevel << "\n"
       << prefix << setw(24)
       << "ILOC:"      << theImageLocation << "\n"
       << prefix << setw(24)
       << "IMAG:"      << theImageMagnification << "\n"
       << prefix << setw(24)
       << "UDIDL:"     << theUserDefinedImageDataLength << "\n"
       << prefix << setw(24)
       << "UDOFL:"     << theUserDefinedOverflow << "\n"
       << prefix << setw(24)
       << "IXSHDL:"    << theExtendedSubheaderDataLen << "\n"
       << prefix << setw(24)
       << "IXSOFL:"    << theExtendedSubheaderOverflow << "\n"
       << prefix << setw(24)
       << "IMDATOFF:"  << theBlockedImageDataOffset << "\n"
       << prefix << setw(24)
       << "BMRLNTH:"   << theBlockMaskRecordLength << "\n"
       << prefix << setw(24)
       << "TMRLNTH:"   << thePadPixelMaskRecordLength << "\n"
       << prefix << setw(24)
       << "TPXCDLNTH:" << theTransparentOutputPixelCodeLength << "\n"
       << prefix << setw(24)
       << "TPXCD:"     << thePadOutputPixelCode << "\n";

   if ( traceDebug() )
   {
      out << prefix << setw(24)
          << "theDataLocation:" << theDataLocation << "\n";
   }

   out << std::endl;

   return printTags(out, prefix);
}


bool ossimNitfImageHeaderV2_1::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   ossimNitfImageHeaderV2_X::saveState(kwl, prefix);
   
   kwl.add(prefix, "ISCLSY", theSecurityClassificationSys);
   kwl.add(prefix, "ISCODE", theCodewords);
   kwl.add(prefix, "ISCTLH", theControlAndHandling);
   kwl.add(prefix, "ISREL", theReleasingInstructions);
   kwl.add(prefix, "ISDCTP", theDeclassificationType);
   kwl.add(prefix, "ISDCDT", theDeclassificationDate);
   kwl.add(prefix, "ISDCXM", theDeclassificationExempt);
   kwl.add(prefix, "ISDG", theDowngrade);
   kwl.add(prefix, "ISDGDT", theDowngradeDate);
   kwl.add(prefix, "ISCLTX", theClassificationText);
   kwl.add(prefix, "ISCATP", theClassificationAuthType);
   kwl.add(prefix, "ISCAUT", theClassificationAuthority);
   kwl.add(prefix, "ISCRSN", theClassificationReason);
   kwl.add(prefix, "ISSRDT", theSecuritySourceDate);
   kwl.add(prefix, "ISCTLN", theSecurityControlNumber);
   kwl.add(prefix, "XBANDS", theNumberOfMultispectralBands);
   
   ossim_uint32 idx = 0;
   
   std::ostringstream out;
   if(theCompressionHeader.valid())
   {
      theCompressionHeader->saveState(kwl, prefix);
   }

   for(idx = 0; idx < theImageBands.size(); ++idx)
   {
      if(theImageBands[idx].valid())
      {
         theImageBands[idx]->print(out, "", idx); 
      }
   }
   
   out << std::endl;
   
   ossimKeywordlist kwlTemp;
   
   std::istringstream in(out.str());
   if(kwlTemp.parseStream(in))
   {
      kwl.add(prefix, kwlTemp);
   }
   
   return true;
}

bool ossimNitfImageHeaderV2_1::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Note: Currently not looking up all fieds only ones that make sense.
   
   const char* lookup;

   lookup = kwl.find( prefix, ISCLSY_KW );
   if ( lookup )
   {
      setSecurityClassificationSystem( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCODE_KW );
   if ( lookup )
   {
      setCodewords( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCTLH_KW );
   if ( lookup )
   {
      setControlAndHandling( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISREL_KW);
   if ( lookup )
   {
      setReleasingInstructions( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISDCTP_KW );
   if ( lookup )
   {
      setDeclassificationType( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISDCDT_KW );
   if ( lookup )
   {
      setDeclassificationDate( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISDCXM_KW );
   if ( lookup )
   {
      setDeclassificationExempt( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISDG_KW );
   if ( lookup )
   {
      setDowngrade( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISDGDT_KW );
   if ( lookup )
   {
      setDowngradeDate( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCLTX_KW );
   if ( lookup )
   {
      setClassificationText( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCATP_KW );
   if ( lookup )
   {
      setClassificationAuthorityType( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCAUT_KW );
   if ( lookup )
   {
      setClassificationAuthority( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCRSN_KW );
   if ( lookup )
   {
      setClassificationReason( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISSRDT_KW );
   if ( lookup )
   {
      setSecuritySourceDate( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCTLN_KW );
   if ( lookup )
   {
      setSecurityControlNumber( ossimString(lookup) );
   }
   
   return ossimNitfImageHeaderV2_X::loadState(kwl, prefix);
}

bool ossimNitfImageHeaderV2_1::isCompressed()const
{
   ossimString temp = theCompression;
   temp = temp.trim();
   return ((temp != "NC") && (temp != "NM"));
}

bool ossimNitfImageHeaderV2_1::isEncrypted()const
{
   return (theEncryption[0] == '1');
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfBands()const
{
   if(theNumberOfBands[0] == '0')
   {
      return ossimString(theNumberOfMultispectralBands).toInt32();
   }
   else
   {
      return ossimString(theNumberOfBands).toInt32();
   }
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfRows()const
{
   return ossimString(theSignificantRows).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfCols()const
{
   return ossimString(theSignificantCols).toInt32();
}

ossimString ossimNitfImageHeaderV2_1::getImageId()const
{
   return theImageId;
}

ossim_int32 ossimNitfImageHeaderV2_1::getBitsPerPixelPerBand()const
{
   return ossimString(theNumberOfBitsPerPixelPerBand).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_1::getActualBitsPerPixelPerBand()const
{
   return ossimString(theActualBitsPerPixelPerBand).toInt32();
}

ossimString ossimNitfImageHeaderV2_1::getIMode() const
{
   return ossimString(theImageMode).trim();
}

ossimString ossimNitfImageHeaderV2_1::getSecurityClassification()const
{
   return theSecurityClassification;
}

ossimString  ossimNitfImageHeaderV2_1::getImageDateAndTime() const
{
   return ossimString(theDateTime);
}

ossimString ossimNitfImageHeaderV2_1::getAcquisitionDateMonthDayYear(ossim_uint8 separationChar)const
{
   ossimString result;

   result += ossimString(theDateTime + 4,
                         theDateTime + 6);
   result += ossimString(separationChar);
   result += ossimString(theDateTime + 6,
                         theDateTime + 8);
   result += ossimString(separationChar);
   result += ossimString(theDateTime,
                         theDateTime + 4);
   
   return result;
}

ossimString ossimNitfImageHeaderV2_1::getCategory()const
{
   return theCategory;
}

ossimString ossimNitfImageHeaderV2_1::getImageSource()const
{
   return theImageSource;
}

ossimString ossimNitfImageHeaderV2_1::getRepresentation()const
{
   return theRepresentation;
}

ossimString ossimNitfImageHeaderV2_1::getCoordinateSystem()const
{
   return theCoordinateSystem;
}
ossimString ossimNitfImageHeaderV2_1::getGeographicLocation()const
{
  return theGeographicLocation;
}

bool ossimNitfImageHeaderV2_1::hasBlockMaskRecords()const
{
   return (theBlockMaskRecords.size() > 0);
}

bool ossimNitfImageHeaderV2_1::hasPadPixelMaskRecords()const
{
   return (thePadPixelMaskRecords.size()>0);
}

bool ossimNitfImageHeaderV2_1::hasTransparentCode()const
{
   return (theTransparentOutputPixelCodeLength > 0);
}

ossim_uint32 ossimNitfImageHeaderV2_1::getTransparentCode()const
{
   return thePadOutputPixelCode;
}

ossim_uint32 ossimNitfImageHeaderV2_1::getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                                ossim_uint32 bandNumber)const
{
   ossim_uint32 maxBlock = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol();
   ossim_uint32 result = 0xffffffff;
   
   if((hasBlockMaskRecords())&&
      (blockNumber < maxBlock))
   {
      if(theImageMode[0] == 'S')
      {
         if(bandNumber < (ossim_uint32)getNumberOfBands())
         {
            result = theBlockMaskRecords[bandNumber*maxBlock + blockNumber];
         }
      }
      else
      {
         result = theBlockMaskRecords[blockNumber];
      }
   }

   return result;
}

ossim_uint32 ossimNitfImageHeaderV2_1::getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                                   ossim_uint32 bandNumber)const
{
   ossim_uint32 maxBlock = getNumberOfBlocksPerRow()*getNumberOfBlocksPerCol();
   ossim_uint32 result = 0xffffffff;
   
   if((hasPadPixelMaskRecords())&&
      (blockNumber < maxBlock))
   {
      if(theImageMode[0] == 'S')
      {
         if(bandNumber < (ossim_uint32)getNumberOfBands())
         {
            result = thePadPixelMaskRecords[bandNumber*maxBlock + blockNumber];
         }
      }
      else
      {
         result = thePadPixelMaskRecords[blockNumber];
      }
   }

   return result;
}


void ossimNitfImageHeaderV2_1::clearFields()
{
   memcpy(theType, "IM", 2);
   memset(theImageId, ' ',10);
   memset(theDateTime, '-',14);
   memset(theTargetId, ' ',17);
   memset(theTitle, ' ',80);
   memset(theSecurityClassification, ' ',1);
   memset(theSecurityClassificationSys, ' ',2);
   memset(theCodewords, ' ',11);
   memset(theControlAndHandling, ' ',2);
   memset(theReleasingInstructions, ' ',20);
   memset(theDeclassificationType, ' ',2);
   memset(theDeclassificationDate, ' ',8);
   memset(theDeclassificationExempt, ' ',4);
   memset(theDowngrade, ' ',1);
   memset(theDowngradeDate, ' ',8);
   memset(theClassificationText, ' ',43);
   memset(theClassificationAuthType, ' ',1);
   memset(theClassificationAuthority, ' ',40);
   memset(theClassificationReason, ' ',1);
   memset(theSecuritySourceDate, ' ',8);
   memset(theSecurityControlNumber, ' ',15);
   memset(theEncryption, '0',1);
   memset(theImageSource, ' ',42);
   memset(theSignificantRows, '0',8);
   memset(theSignificantCols, '0',8);
   memset(thePixelValueType, ' ',3);
   memset(theRepresentation, ' ',8);
   memset(theCategory, ' ',8);
   memset(theActualBitsPerPixelPerBand, ' ',2);
   memset(theJustification, 'R',1);
   memset(theCoordinateSystem, ' ',1);
   memset(theGeographicLocation, ' ',60);
   memset(theNumberOfComments, '0', 1);
   memcpy(theCompression, "NC",2);
   memset(theCompressionRateCode, ' ',4);
   memset(theNumberOfBands, '0',1);
   memset(theNumberOfMultispectralBands, ' ',5);
   memset(theImageSyncCode, '0', 1);
   memset(theImageMode, 'B', 1);
   memset(theNumberOfBlocksPerRow, '0', 4);
   memset(theNumberOfBlocksPerCol, '0', 4);
   memset(theNumberOfPixelsPerBlockHoriz, '0', 4);
   memset(theNumberOfPixelsPerBlockVert, '0', 4);
   memset(theNumberOfBitsPerPixelPerBand, '0', 2);
   memcpy(theDisplayLevel, "001", 3);
   memset(theAttachmentLevel, '0', 3);
   memset(theImageLocation, '0', 10);
   memcpy(theImageMagnification, "1.0 ", 4);
   
   memset(theUserDefinedImageDataLength,'0', 5);
   memset(theUserDefinedOverflow, '0', 3);
   memset(theExtendedSubheaderDataLen, '0', 5);
   memset(theExtendedSubheaderOverflow, '0', 3);
   theBlockMaskRecords.clear();
   thePadPixelMaskRecords.clear();

   theBlockedImageDataOffset = 0;
   theBlockMaskRecordLength = 0;
   thePadPixelMaskRecordLength = 0;
   theTransparentOutputPixelCodeLength = 0;
   thePadOutputPixelCode = 0;
   
   theCompressionHeader = 0;
   
   theType[2] = '\0';
   theImageId[10] = '\0';
   theDateTime[14] = '\0';
   theTargetId[17] = '\0';
   theTitle[80] = '\0';
   theSecurityClassification[1] = '\0';
   theSecurityClassificationSys[2] = '\0';
   theCodewords[11] = '\0';
   theControlAndHandling[2] = '\0';
   theReleasingInstructions[20] = '\0';
   theDeclassificationType[2] = '\0';
   theDeclassificationDate[8] = '\0';
   theDeclassificationExempt[4] = '\0';
   theDowngrade[1] = '\0';
   theDowngradeDate[8] = '\0';
   theClassificationText[43] = '\0';
   theClassificationAuthType[1] = '\0';
   theClassificationAuthority[40] = '\0';
   theClassificationReason[1] = '\0';
   theSecuritySourceDate[8] = '\0';
   theSecurityControlNumber[15] = '\0';
   theEncryption[1] = '\0';
   theImageSource[42] = '\0';
   theSignificantRows[8] = '\0';
   theSignificantCols[8] = '\0';
   thePixelValueType[3] = '\0';
   theRepresentation[8] = '\0';
   theCategory[8] = '\0';
   theActualBitsPerPixelPerBand[2] = '\0';
   theJustification[1] = '\0';
   theCoordinateSystem[1] = '\0';
   theGeographicLocation[60] = '\0';
   theNumberOfComments[1] = '\0';
   theCompression[2] = '\0';
   theCompressionRateCode[4] = '\0';
   theNumberOfBands[1] = '\0';
   theNumberOfMultispectralBands[5] = '\0';
   theImageSyncCode[1] = '\0';
   theImageMode[1] = '\0';
   theNumberOfBlocksPerRow[4] = '\0';
   theNumberOfBlocksPerCol[4] = '\0';
   theNumberOfPixelsPerBlockHoriz[4] = '\0';
   theNumberOfPixelsPerBlockVert[4] = '\0';
   theNumberOfBitsPerPixelPerBand[2] = '\0';
   theDisplayLevel[3] = '\0';
   theAttachmentLevel[3] = '\0';
   theImageLocation[10] = '\0';
   theImageMagnification[4] = '\0';
   theUserDefinedImageDataLength[5] = '\0';
   theUserDefinedOverflow[3] = '\0';
   theExtendedSubheaderDataLen[5] = '\0';
   theExtendedSubheaderOverflow[3] = '\0';
   
   theDataLocation = 0;
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfBlocksPerRow()const
{
   return ossimString(theNumberOfBlocksPerRow).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfBlocksPerCol()const
{
   return ossimString(theNumberOfBlocksPerCol).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfPixelsPerBlockHoriz()const
{
 //  return ossimString(theNumberOfPixelsPerBlockHoriz).toInt32();
   ossim_int32 rval = ossimString(theNumberOfPixelsPerBlockHoriz).toInt32();
   if ((rval == 0) && (getNumberOfBlocksPerCol() == 1))
   {
      rval = getNumberOfCols();
   }
   return rval;
   
}

ossim_int32 ossimNitfImageHeaderV2_1::getNumberOfPixelsPerBlockVert()const
{
//   return ossimString(theNumberOfPixelsPerBlockVert).toInt32();
   ossim_int32 rval = ossimString(theNumberOfPixelsPerBlockVert).toInt32();
   if ((rval == 0) && (getNumberOfBlocksPerRow() == 1))
   {
      rval = getNumberOfRows();
   }
   return rval;
}

ossimIrect ossimNitfImageHeaderV2_1::getImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
    double rows = ossimString(theSignificantRows).toDouble();
    double cols = ossimString(theSignificantCols).toDouble();

    ossimDpt lr(ul.x + cols-1,
                ul.y + rows-1);
   
    return ossimIrect(ul, lr);
}

ossimIrect ossimNitfImageHeaderV2_1::getBlockImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
   double rows = getNumberOfPixelsPerBlockVert()*getNumberOfBlocksPerCol();
   double cols = getNumberOfPixelsPerBlockHoriz()*getNumberOfBlocksPerRow();;
   
   ossimDpt lr(ul.x + cols-1,
               ul.y + rows-1);
   
    return ossimIrect(ul, lr);
}


ossimString ossimNitfImageHeaderV2_1::getPixelValueType()const
{
   return ossimString(thePixelValueType).trim();
}


void ossimNitfImageHeaderV2_1::setNumberOfBands(ossim_uint32 nbands)
{
   std::ostringstream out;

   if(nbands > 9)
   {
      out << std::setw(5)
          << std::setfill('0')
          << std::setiosflags(ios::right)
          << nbands;

      theNumberOfBands[0] = '0';
      memcpy(theNumberOfMultispectralBands, out.str().c_str(), 5);
   }
   else
   {
      out << nbands;
      theNumberOfBands[0] = out.str().c_str()[0];
   }

   theImageBands.resize(getNumberOfBands());
}

void ossimNitfImageHeaderV2_1::setBandInfo(ossim_uint32 idx,
                                           const ossimNitfImageBandV2_1& info)
{
   if(idx < theImageBands.size())
   {
      if(!theImageBands[idx].valid())
      {
         theImageBands[idx] = new ossimNitfImageBandV2_1;
      }
      (*theImageBands[idx]) = info;
   }
}

void ossimNitfImageHeaderV2_1::setNumberOfRows(ossim_uint32 rows)
{
   std::ostringstream out;
   if(rows > 99999999) rows = 99999999;
   
   out << rows;
   ossimNitfCommon::setField(theSignificantRows, out.str(), 8, ios::right, '0');
}

void ossimNitfImageHeaderV2_1::setNumberOfCols(ossim_uint32 cols)
{
   std::ostringstream out;
   if(cols > 99999999) cols = 99999999;

   out << cols;
   ossimNitfCommon::setField(theSignificantCols, out.str(), 8, ios::right, '0');
}

void ossimNitfImageHeaderV2_1::setGeographicLocationDms(const ossimDpt& ul,
                                                        const ossimDpt& ur,
                                                        const ossimDpt& lr,
                                                        const ossimDpt& ll)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << ossimDms(ul.y, true).toString("ddmmss.ssssC").c_str()
         << ossimDms(ul.x, false).toString("dddmmss.ssssC").c_str()
         << ossimDms(ur.y, true).toString("ddmmss.ssssC").c_str()
         << ossimDms(ur.x, false).toString("dddmmss.ssssC").c_str()
         << ossimDms(lr.y, true).toString("ddmmss.ssssC").c_str()
         << ossimDms(lr.x, false).toString("dddmmss.ssssC").c_str()
         << ossimDms(ll.y, true).toString("ddmmss.ssssC").c_str()
         << ossimDms(ll.x, false).toString("dddmmss.ssssC").c_str()
         << std::endl;

      checkForGeographicTiePointTruncation(ul);
      checkForGeographicTiePointTruncation(ur);
      checkForGeographicTiePointTruncation(lr);
      checkForGeographicTiePointTruncation(ll);
   }
      
   theCoordinateSystem[0] = 'G';
   std::ostringstream out;

   out << ossimDms(ul.y, true).toString("ddmmssC").c_str();
   out << ossimDms(ul.x, false).toString("dddmmssC").c_str();
   out << ossimDms(ur.y, true).toString("ddmmssC").c_str();
   out << ossimDms(ur.x, false).toString("dddmmssC").c_str();
   out << ossimDms(lr.y, true).toString("ddmmssC").c_str();
   out << ossimDms(lr.x, false).toString("dddmmssC").c_str();
   out << ossimDms(ll.y, true).toString("ddmmssC").c_str();
   out << ossimDms(ll.x, false).toString("dddmmssC").c_str();

   memcpy(theGeographicLocation, out.str().c_str(), 60);
}

void ossimNitfImageHeaderV2_1::setGeographicLocationDecimalDegrees(
   const ossimDpt& ul,
   const ossimDpt& ur,
   const ossimDpt& lr,
   const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'D';
   ostringstream out;

   out << (ul.lat >= 0.0?"+":"")
       << std::setw(6)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ul.lat
       << (ul.lon >= 0.0?"+":"")
       << std::setw(7)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ul.lon;
   out << (ur.lat >= 0.0?"+":"")
       << std::setw(6)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ur.lat
       << (ur.lon >= 0.0?"+":"")
       << std::setw(7)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ur.lon;
   out << (lr.lat >= 0.0?"+":"")
       << std::setw(6)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << lr.lat
       << (lr.lon >= 0.0?"+":"")
       << std::setw(7)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << lr.lon;
   out << (ll.lat >= 0.0?"+":"")
       << std::setw(6)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ll.lat
       << (ll.lon >= 0.0?"+":"")
       << std::setw(7)
       << std::setfill('0')
       << std::setprecision(3)
       << std::setiosflags(std::ios::fixed)
       << ll.lon;

   memcpy(theGeographicLocation, out.str().c_str(), 60);
}

void ossimNitfImageHeaderV2_1::setUtmNorth(ossim_uint32 zone,
                                           const ossimDpt& ul,
                                           const ossimDpt& ur,
                                           const ossimDpt& lr,
                                           const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'N';
   
   memcpy(theGeographicLocation,
          encodeUtm(zone, ul, ur, lr, ll).c_str(), 60);
}

void ossimNitfImageHeaderV2_1::setUtmSouth(ossim_uint32 zone,
                                           const ossimDpt& ul,
                                           const ossimDpt& ur,
                                           const ossimDpt& lr,
                                           const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'S';
   
   memcpy(theGeographicLocation,
          encodeUtm(zone, ul, ur, lr, ll).c_str(), 60);
}


void ossimNitfImageHeaderV2_1::setSecurityClassificationSystem(const ossimString& value)
{
   ossimNitfCommon::setField(theSecurityClassificationSys, value, 2);
}

void ossimNitfImageHeaderV2_1::setCodewords(const ossimString& value)
{
   ossimNitfCommon::setField(theCodewords, value, 11);
}

void ossimNitfImageHeaderV2_1::setControlAndHandling(const ossimString& value)
{
   ossimNitfCommon::setField(theControlAndHandling, value, 2);
}

void ossimNitfImageHeaderV2_1::setReleasingInstructions(const ossimString& value)
{
   ossimNitfCommon::setField(theReleasingInstructions, value, 20);
}

void ossimNitfImageHeaderV2_1::setDeclassificationType(const ossimString& value)
{
   ossimNitfCommon::setField(theDeclassificationType, value, 2);
}

void ossimNitfImageHeaderV2_1::setDeclassificationDate(const ossimString& value)
{
   ossimNitfCommon::setField(theDeclassificationDate, value, 8);
}

void ossimNitfImageHeaderV2_1::setDeclassificationExempt(const ossimString& value)
{
   ossimNitfCommon::setField(theDeclassificationExempt, value, 4);
}

void ossimNitfImageHeaderV2_1::setDowngrade(const ossimString& value)
{
   ossimNitfCommon::setField(theDowngrade, value, 1);
}

void ossimNitfImageHeaderV2_1::setDowngradeDate(const ossimString& value)
{
   ossimNitfCommon::setField(theDowngradeDate, value, 8);
}

void ossimNitfImageHeaderV2_1::setClassificationText(const ossimString& value)
{
   ossimNitfCommon::setField(theClassificationText, value, 43);
}

void ossimNitfImageHeaderV2_1::setClassificationAuthorityType(const ossimString& value)
{
   ossimNitfCommon::setField(theClassificationAuthType, value, 1);
}

void ossimNitfImageHeaderV2_1::setClassificationAuthority(const ossimString& value)
{
   ossimNitfCommon::setField(theClassificationAuthority, value, 40);
}

void ossimNitfImageHeaderV2_1::setClassificationReason(const ossimString& value)
{
   ossimNitfCommon::setField(theClassificationReason, value, 1);
}

void ossimNitfImageHeaderV2_1::setSecuritySourceDate(const ossimString& value)
{
   ossimNitfCommon::setField(theSecuritySourceDate, value, 8);
}

void ossimNitfImageHeaderV2_1::setSecurityControlNumber(const ossimString& value)
{
   ossimNitfCommon::setField(theSecurityControlNumber, value, 15);
}

void ossimNitfImageHeaderV2_1::setImageMagnification(const ossimString& value)
{
   ossimNitfCommon::setField(theImageMagnification, value, 4);
}

void ossimNitfImageHeaderV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();

   // Make case insensitive:
   name.upcase();
   
   std::ostringstream out;
   
   if(!property) return;
   
   if(name.contains(ISCLSY_KW))
   {
      setSecurityClassificationSystem(property->valueToString());
   }
   else if(name.contains(ISCODE_KW))
   {
      setCodewords(property->valueToString());
   }
   else if(name.contains(ISCTLH_KW))
   {
      setControlAndHandling(property->valueToString());
   }
   else if(name.contains(ISREL_KW))
   {
      setReleasingInstructions(property->valueToString());
   }
   else if(name.contains(ISDCTP_KW))
   {
      setDeclassificationType(property->valueToString());
   }
   else if(name.contains(ISDCDT_KW))
   {
      setDeclassificationDate(property->valueToString());
   }
   else if(name.contains(ISDCXM_KW))
   {
      setDeclassificationExempt(property->valueToString());
   }
   else if(name.contains(ISDG_KW))
   {
      setDowngrade(property->valueToString());
   }
   else if(name.contains(ISDGDT_KW))
   {
      setDowngradeDate(property->valueToString());
   }
   else if(name.contains(ISCLTX_KW))
   {
      setClassificationText(property->valueToString());
   }
   else if(name.contains(ISCATP_KW))
   {
      setClassificationAuthorityType(property->valueToString());
   }
   else if(name.contains(ISCAUT_KW))
   {
      setClassificationAuthority(property->valueToString());
   }
   else if(name.contains(ISCRSN_KW))
   {
      setClassificationReason(property->valueToString());
   }
   else if(name == ISCRSN_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theClassificationReason).trim());
   }
   else if(name.contains(ISSRDT_KW))
   {
      setSecuritySourceDate(property->valueToString());
   }
   else if(name.contains(ISCTLN_KW))
   {
      setSecurityControlNumber(property->valueToString());
   }
   else
   {
      ossimNitfImageHeaderV2_X::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfImageHeaderV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;

   if(name == ISCLSY_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theSecurityClassificationSys).trim());
   }
   else if(name == ISCODE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCodewords).trim());
   }
   else if(name == ISCTLH_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theControlAndHandling).trim());
   }
   else if(name == ISREL_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theReleasingInstructions).trim());
   }
   else if(name == ISDCTP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDeclassificationType).trim());
   }
   else if(name == ISDCDT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDeclassificationDate).trim());
   }
   else if(name == ISDCXM_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDeclassificationExempt).trim());
   }
   else if(name == ISDG_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDowngrade).trim());
   }
   else if(name == ISDGDT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDowngradeDate).trim());
   }
   else if(name == ISCLTX_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theClassificationText).trim());
   }
   else if(name == ISCATP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theClassificationAuthType).trim());
   }
   else if(name == ISCRSN_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theClassificationAuthority).trim());
   }
   else if(name == ISSRDT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theSecuritySourceDate).trim());
   }
   else if(name == ISCTLN_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theSecurityControlNumber).trim());
   }
   else
   {
      return ossimNitfImageHeaderV2_X::getProperty(name);
   }
   
   return property;
}

void ossimNitfImageHeaderV2_1::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfImageHeaderV2_X::getPropertyNames(propertyNames);

   propertyNames.push_back(ISCLSY_KW);
   propertyNames.push_back(ISCODE_KW);
   propertyNames.push_back(ISCTLH_KW);
   propertyNames.push_back(ISREL_KW);
   propertyNames.push_back(ISDCTP_KW);
   propertyNames.push_back(ISDCDT_KW);
   propertyNames.push_back(ISDCXM_KW);
   propertyNames.push_back(ISDG_KW);
   propertyNames.push_back(ISDGDT_KW);
   propertyNames.push_back(ISCLTX_KW);
   propertyNames.push_back(ISCATP_KW);
   propertyNames.push_back(ISCAUT_KW);
   propertyNames.push_back(ISCRSN_KW);
   propertyNames.push_back(ISSRDT_KW);
   propertyNames.push_back(ISCTLN_KW);
   propertyNames.push_back(XBANDS_KW);
}

const ossimRefPtr<ossimNitfCompressionHeader> ossimNitfImageHeaderV2_1::getCompressionHeader()const
{
   return theCompressionHeader;
}

const ossimRefPtr<ossimNitfImageBand> ossimNitfImageHeaderV2_1::getBandInformation(ossim_uint32 idx)const
{
   if(idx < theImageBands.size())
   {
      return (ossimNitfImageBand*)theImageBands[idx].get();
   }
   
   return 0;
}


ossimString ossimNitfImageHeaderV2_1::encodeUtm(
   ossim_uint32 zone,
   const ossimDpt& ul,
   const ossimDpt& ur,
   const ossimDpt& lr,
   const ossimDpt& ll)const throw(std::out_of_range)
{
   ostringstream out;

   if(zone > 60)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nUTM zone greate than 60!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   ossim_float64 east  = ul.x;
   ossim_float64 north = ul.y;
   
   if((ossim_uint32)(east+.5) > 999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nUpper left easting too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   if((ossim_uint32)(north+.5) > 9999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nUpper left northing too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }

   out << setw(2)
       << setfill('0')
       << zone
       << setw(6)
       << setfill('0')
       <<(ossim_uint32)(east+.5)
       << setw(7)
          << setfill('0')
       <<(ossim_uint32)(north+.5);

   
   east  = ur.x;
   north = ur.y;
   
   if((ossim_uint32)(east+.5) > 999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nUpper right easting too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   if((ossim_uint32)(north+.5) > 9999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nUpper right northing too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   out << setw(2)
       << setfill('0')
       << zone
       << setw(6)
       << setfill('0')
       <<(ossim_uint32)(east+.5)
       << setw(7)
       << setfill('0')
       <<(ossim_uint32)(north+.5);
   east  = lr.x;
   north = lr.y;

   if((ossim_uint32)(east+.5) > 999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nLower right easting too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   if((ossim_uint32)(north+.5) > 9999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nLower right northing too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }   

   out << setw(2)
       << setfill('0')
       << zone
       << setw(6)
       << setfill('0')
       <<(ossim_uint32)(east+.5)
       << setw(7)
       << setfill('0')
       <<(ossim_uint32)(north+.5);
   
   east  = ll.x;
   north = ll.y;

   if((ossim_uint32)(east+.5) > 999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nLower left easting too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   if((ossim_uint32)(north+.5) > 9999999)
   {
      std::string s = "ossimNitfImageHeaderV2_1::encodeUtm: ERROR\nLower left northing too large for NITF field!";
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << s << std::endl;
      }
      throw std::out_of_range(s);
   }
   
   out << setw(2)
       << setfill('0')
       << zone
       << setw(6)
       << setfill('0')
       <<(ossim_uint32)(east+.5)
       << setw(7)
       << setfill('0')
       <<(ossim_uint32)(north+.5);
   
   return out.str().c_str();
}

bool ossimNitfImageHeaderV2_1::takeOverflowTags(std::vector<ossimNitfTagInformation> &overflowTags,
   ossim_uint32 potentialDesIndex, bool userDefinedTags)
{
   overflowTags.clear();
   std::vector<ossimNitfTagInformation>::iterator iter;
   std::vector<ossimNitfTagInformation> specifiedTags;
   const ossimString tagType(userDefinedTags ? "UDID" : "IXSHD");
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

   if (userDefinedTags)
   {
      strcpy(theUserDefinedOverflow, overflowDes.str().c_str());
      strcpy(theUserDefinedImageDataLength, tagLength.str().c_str());
   }
   else
   {
      strcpy(theExtendedSubheaderOverflow, overflowDes.str().c_str());
      strcpy(theExtendedSubheaderDataLen, tagLength.str().c_str());
   }

   return (overflowTags.empty() == false);
}

