//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL
//
// See top level LICENSE.txt
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeaderV2_1.cpp,v 1.44 2005/11/19 00:34:14 dburken Exp $
#include <sstream>
#include <iomanip>
#include <support_data/nitf/ossimNitfImageHeaderV2_1.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimDms.h>
#include <base/property/ossimStringProperty.h>
#include <base/context/ossimNotifyContext.h>
#include <base/misc/ossimEndian.h>
#include <cstring> //for memset
#include <support_data/nitf/ossimNitfVqCompressionHeader.h>

#ifndef NULL
#include <cstddef>
#endif

RTTI_DEF1(ossimNitfImageHeaderV2_1,
          "ossimNitfImageHeaderV2_1",
          ossimNitfImageHeader)

static const char* IID1_KW = "iid1";
static const char* IDATIM_KW = "idatim";
static const char* TGTID_KW = "tgtid";
static const char* IID2_KW = "iid2";
static const char* ISCLAS_KW = "isclas";
static const char* ISCLSY_KW = "isclsy";
static const char* ISCODE_KW = "iscode";
static const char* ISCTLH_KW = "isctlh";
static const char* ISREL_KW = "isrel";
static const char* ISDCTP_KW = "isdctp";
static const char* ISDCDT_KW = "isdcdt";
static const char* ISDCXM_KW = "isdcxm";
static const char* ISDG_KW = "isdg";
static const char* ISDGDT_KW = "isdgdt";
static const char* ISCLTX_KW = "iscltx";
static const char* ISCATP_KW = "iscatp";
static const char* ISCAUT_KW = "iscaut";
static const char* ISCRSN_KW = "iscrsn";
static const char* ISSRDT_KW = "issrdt";
static const char* ISCTLN_KW = "isctln";
static const char* ENCRYP_KW = "encryp";
static const char* ISORCE_KW = "isorce";
// static const char* NROWS_KW = "nrows";
// static const char* NCOLS_KW = "ncols";
static const char* PVTYPE_KW = "pvtype";
static const char* IREP_KW = "irep";
static const char* ICAT_KW = "icat";
static const char* ABPP_KW = "abpp";
static const char* PJUST_KW = "pjust";
static const char* ICORDS_KW = "icords";
static const char* IGEOLO_KW = "igeolo";
static const char* NICOM_KW = "nicom";
static const char* IC_KW = "ic";
static const char* COMRAT_KW = "comrat";
static const char* NBANDS_KW = "nbands";
static const char* XBANDS_KW = "xbands";
static const char* ISYNC_KW = "isync";
static const char* IMODE_KW = "imode";
static const char* NBPR_KW = "nbpr";
static const char* NBPC_KW = "nbpc";
static const char* NPPBH_KW = "nppbh";
static const char* NPPBV_KW = "nppbv";
static const char* NBPP_KW = "nbpp";
static const char* IDLVL_KW = "idlvl";
static const char* IALVL_KW = "ialvl";
static const char* ILOC_KW = "iloc";
static const char* IMAG_KW = "imag";
// static const char* UDIDL_KW = "udidl";

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
   long numberOfComments = ossimString(theNumberOfComments).toLong();
   
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
   long numberOfBands = ossimString(theNumberOfBands).toLong();
   if (numberOfBands == 0)
   {
      in.read(theNumberOfMultispectralBands, 5);
      numberOfBands = ossimString(theNumberOfMultispectralBands).toLong();
   }
   theImageBands.clear();
   theImageBands.resize(numberOfBands);
   for (long band = 0; band < numberOfBands; ++band)
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
      = ossimString(theUserDefinedImageDataLength).toLong();
   if(userDefinedDataLen > 0)
   {
      in.read(theUserDefinedOverflow, 3);
      while((current - start) < userDefinedDataLen)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         in.ignore(headerTag.getTagLength());
         headerTag.clearFields();
         current = in.tellg();
      }
      in.seekg(start + userDefinedDataLen);
   }
   in.read(theExtendedSubheaderDataLen, 5);
   std::streampos extSubHeadLen = ossimString(theExtendedSubheaderDataLen).toLong();
   start   = in.tellg();
   current = in.tellg();
   if(extSubHeadLen > 0)
   {
      in.read(theExtendedSubheaderOverflow, 3);
      while((current - start) < extSubHeadLen)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         
         in.ignore(headerTag.getTagLength());
         headerTag.clearFields();
         current = in.tellg();
      }
      in.seekg(start + extSubHeadLen);
   }
   ossimString compressionType = theCompression;
   compressionType = compressionType.trim().upcase();
   ossimEndian endian;
   if((compressionType == "NM")||
      (compressionType == "M1")||
      (compressionType == "M3")||
      (compressionType == "M4")||
      (compressionType == "M5"))
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
         if((theImageMode[0] == 'S'))
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
      }
      if((thePadPixelMaskRecordLength > 0)||
         (( (getCompressionCode().upcase() == "M3"))&&
          (thePadPixelMaskRecordLength == 0)))
      {
         ossim_uint32 totalNumber = 0;
         if((theImageMode[0] == 'S'))
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
//   out.write(theNumberOfComments, 1);
   out.write("0", 1);
   
   out.write(theCompression, 2);
   ossimString compressionTest = theCompression;
   if(compressionTest != "NC" &&
      compressionTest != "NM")
     {
      out.write(theCompressionRateCode, 4);
     }

   out.write(theNumberOfBands, 1);

   if(ossimString(theNumberOfBands).toLong() == 0)
   {
      out.write(theNumberOfMultispectralBands, 5);
   }

   if(theImageBands.size())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < theImageBands.size(); ++idx)
      {
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

   if(ossimString(theUserDefinedImageDataLength).toLong() > 0)
   {
      out.write(theUserDefinedOverflow, 3);
   }

   // need to ouput tagged data
   // here
   //
   ossim_uint32 totalLength = getTotalTagLength();

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
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_1::writeStream: Only support writing of total tag length < 99999" << std::endl;
   }
}

std::ostream& ossimNitfImageHeaderV2_1::print(std::ostream& out)const
{
   out << "theType:                       " << theType
       << "\ntheImageId:                    " << theImageId
       << "\ntheDateTime:                   " << theDateTime
       << "\ntheTargetId:                   " << theTargetId
       << "\ntheTitle:                      " << theTitle
       << "\ntheSecurityClassification:     " << theSecurityClassification
       << "\ntheSecurityClassificationSys:  " << theSecurityClassificationSys
       << "\ntheCodewords:                  " << theCodewords
       << "\ntheControlAndHandling:         " << theControlAndHandling
       << "\ntheReleasingInstructions:      " << theReleasingInstructions
       << "\ntheDeclassificationType:       " << theDeclassificationType
       << "\ntheDeclassificationDate:       " << theDeclassificationDate
       << "\ntheDeclassificationExempt:     " << theDeclassificationExempt
       << "\ntheDowngrade:                  " << theDowngrade
       << "\ntheDowngradeDate:              " << theDowngradeDate
       << "\ntheClassificationText:         " << theClassificationText
       << "\ntheClassificationAuthType:     " << theClassificationAuthType
       << "\ntheClassificationAuthority:    " << theClassificationAuthority
       << "\ntheClassificationReason:       " << theClassificationReason
       << "\ntheSecuritySourceDate:         " << theSecuritySourceDate
       << "\ntheSecurityControlNumber:      " << theSecurityControlNumber
       << "\ntheEncryption:                 " << theEncryption
       << "\ntheImageSource:                " << theImageSource
       << "\ntheSignificantRows:            " << theSignificantRows
       << "\ntheSignificantCols:            " << theSignificantCols
       << "\nthePixelValueType:             " << thePixelValueType
       << "\ntheRepresentation:             " << theRepresentation
       << "\ntheCategory:                   " << theCategory
       << "\ntheActualBitsPerPixelPerBand:        " << theActualBitsPerPixelPerBand
       << "\ntheJustification:              " << theJustification
       << "\ntheCoordinateSystem:           " << theCoordinateSystem
       << "\ntheGeographicLocation:         " << theGeographicLocation
       << "\ntheNumberOfComments:           " << theNumberOfComments
       << "\ntheCompression:                " << theCompression
       << "\ntheCompressionRateCode:        " << theCompressionRateCode
       << "\ntheNumberOfBands:              " << theNumberOfBands
       << "\ntheNumberOfMultispectralBands: " << theNumberOfMultispectralBands
       << endl;
   
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theImageBands.size(); ++idx)
   {
      if(theImageBands[idx].valid())
      {
         out << *theImageBands[idx] << std::endl;
      }
   }
   if(theImageBands.size() < 2)
   {
      out << endl;
   }

   out << "theImageSyncCode:               " << theImageSyncCode
       << "\ntheImageMode:                   " << theImageMode
       << "\ntheNumberOfBlocksPerRow:        " << theNumberOfBlocksPerRow
       << "\ntheNumberOfBlocksPerCol:        " << theNumberOfBlocksPerCol
       << "\ntheNumberOfPixelsPerBlockHoriz: "
       << theNumberOfPixelsPerBlockHoriz
       << "\ntheNumberOfPixelsPerBlockVert:  "
       << theNumberOfPixelsPerBlockVert
       << "\ntheNumberOfBitsPerPixelPerBand: "
       << theNumberOfBitsPerPixelPerBand
       << "\ntheDisplayLevel:                " << theDisplayLevel
       << "\ntheAttachmentLevel:             " << theAttachmentLevel
       << "\ntheImageLocation:               " << theImageLocation
       << "\ntheImageMagnification:          " << theImageMagnification
       << "\ntheUserDefinedImageDataLength:  " << theUserDefinedImageDataLength
       << "\ntheUserDefinedOverflow:         " << theUserDefinedOverflow
       << "\ntheExtendedSubheaderDataLen:    " << theExtendedSubheaderDataLen
       << "\ntheExtendedSubheaderOverflow:   " << theExtendedSubheaderOverflow
       << "\ntheDataLocation:                " << theDataLocation
       << endl;

   out << "theBlockedImageDataOffset:           "
       << theBlockedImageDataOffset << std::endl
       << "theBlockMaskRecordLength:            "
       << theBlockMaskRecordLength << std::endl
       << "thePadPixelMaskRecordLength:         "
       << thePadPixelMaskRecordLength << std::endl
       << "theTransparentOutputPixelCodeLength: "
       << theTransparentOutputPixelCodeLength << std::endl
       << "thePadOutputPixelCode:               "
       << thePadOutputPixelCode << std::endl;
      
   printTags(out);
   
   return out;
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


ossimString ossimNitfImageHeaderV2_1::getCompressionCode()const
{
   return ossimString(theCompression).trim();
}

long ossimNitfImageHeaderV2_1::getDisplayLevel()const
{
   return ossimString(theDisplayLevel).toLong();
}

long ossimNitfImageHeaderV2_1::getAttachmentLevel()const
{
   return ossimString(theAttachmentLevel).toLong();
}

long ossimNitfImageHeaderV2_1::getNumberOfBands()const
{
   if(theNumberOfBands[0] == '0')
   {
      return ossimString(theNumberOfMultispectralBands).toLong();
   }
   else
   {
      return ossimString(theNumberOfBands).toLong();
   }
}

long ossimNitfImageHeaderV2_1::getNumberOfRows()const
{
   return ossimString(theSignificantRows).toLong();
}

long ossimNitfImageHeaderV2_1::getNumberOfCols()const
{
   return ossimString(theSignificantCols).toLong();
}

ossimString ossimNitfImageHeaderV2_1::getImageId()const
{
   return theImageId;
}

long ossimNitfImageHeaderV2_1::getBitsPerPixelPerBand()const
{
   return ossimString(theNumberOfBitsPerPixelPerBand).toLong();
}

long ossimNitfImageHeaderV2_1::getActualBitsPerPixelPerBand()const
{
   return ossimString(theActualBitsPerPixelPerBand).toLong();
}

ossimString ossimNitfImageHeaderV2_1::getIMode() const
{
   return ossimString(theImageMode).trim();
}

ossimString ossimNitfImageHeaderV2_1::getSecurityClassification()const
{
   return theSecurityClassification;
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
   memset(theDateTime, ' ',14);
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
   memset(theJustification, ' ',1);
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
   memcpy(theImageMagnification, "01.0", 4);
   
   memset(theUserDefinedImageDataLength,'0', 5);
   memset(theUserDefinedOverflow, '0', 3);
   memset(theExtendedSubheaderDataLen, '0', 5);
   memset(theExtendedSubheaderOverflow, ' ', 3);
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

long ossimNitfImageHeaderV2_1::getNumberOfBlocksPerRow()const
{
   return ossimString(theNumberOfBlocksPerRow).toLong();
}

long ossimNitfImageHeaderV2_1::getNumberOfBlocksPerCol()const
{
   return ossimString(theNumberOfBlocksPerCol).toLong();
}

long ossimNitfImageHeaderV2_1::getNumberOfPixelsPerBlockHoriz()const
{
   return ossimString(theNumberOfPixelsPerBlockHoriz).toLong();
}

long ossimNitfImageHeaderV2_1::getNumberOfPixelsPerBlockVert()const
{
   return ossimString(theNumberOfPixelsPerBlockVert).toLong();
}

ossimDrect ossimNitfImageHeaderV2_1::getImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
    double rows = ossimString(theSignificantRows).toDouble();
    double cols = ossimString(theSignificantCols).toDouble();

    ossimDpt lr(ul.x + cols-1,
                ul.y + rows-1);
   
    return ossimDrect(ul, lr);
}

ossimDrect ossimNitfImageHeaderV2_1::getBlockImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
   double rows = getNumberOfPixelsPerBlockVert()*getNumberOfBlocksPerCol();
   double cols = getNumberOfPixelsPerBlockHoriz()*getNumberOfBlocksPerRow();;
   
   ossimDpt lr(ul.x + cols-1,
               ul.y + rows-1);
   
    return ossimDrect(ul, lr);
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
   
   out << std::setw(8)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << rows;

   memcpy(theSignificantRows, out.str().c_str(), 8);
}

void ossimNitfImageHeaderV2_1::setNumberOfCols(ossim_uint32 cols)
{
   std::ostringstream out;
   if(cols > 99999999) cols = 99999999;
   
   out << std::setw(8)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << cols;

   memcpy(theSignificantCols, out.str().c_str(), 8);
}

void ossimNitfImageHeaderV2_1::setPixelType(const ossimString& pixelType)
{
   std::ostringstream out;
   
   out << std::setw(3)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << pixelType.c_str();

   memcpy(thePixelValueType , out.str().c_str(), 3);
}

void ossimNitfImageHeaderV2_1::setBitsPerPixel(ossim_uint32 bitsPerPixel)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << bitsPerPixel;

   memcpy(theNumberOfBitsPerPixelPerBand, out.str().c_str(), 2);
}

void ossimNitfImageHeaderV2_1::setActualBitsPerPixel(ossim_uint32 bitsPerPixel)
{
   std::ostringstream out;
   
   out << std::setw(2)
       << std::setfill('0')
       << std::setiosflags(ios::right) 
       << bitsPerPixel;

   memcpy(theActualBitsPerPixelPerBand, out.str().c_str(), 2);
   
}

void ossimNitfImageHeaderV2_1::setImageMode(char mode)
{
   theImageMode[0] = mode;
}

void ossimNitfImageHeaderV2_1::setCategory(const ossimString& category)
{
   std::ostringstream out;
   
   out << std::setw(8)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << category.c_str();

   memcpy(theCategory, out.str().c_str(), 8); 
}

void ossimNitfImageHeaderV2_1::setRepresentation(const ossimString& rep)
{
   std::ostringstream out;
   
   out << std::setw(8)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << rep.c_str();

   memcpy(theRepresentation, out.str().c_str(), 8); 
}

void ossimNitfImageHeaderV2_1::setBlocksPerRow(ossim_uint32 blocks)
{
   std::ostringstream out;
   
   out << std::setw(4)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << blocks;

   memcpy(theNumberOfBlocksPerRow, out.str().c_str(), 4);
}

void ossimNitfImageHeaderV2_1::setBlocksPerCol(ossim_uint32 blocks)
{
   std::ostringstream out;
   
   out << std::setw(4)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << blocks;
   
   memcpy(theNumberOfBlocksPerCol, out.str().c_str(), 4);
}

void ossimNitfImageHeaderV2_1::setNumberOfPixelsPerBlockRow(ossim_uint32 pixels)
{
   std::ostringstream out;
   
   out << std::setw(4)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << pixels;
   
   memcpy(theNumberOfPixelsPerBlockHoriz, out.str().c_str(), 4);
}

void ossimNitfImageHeaderV2_1::setNumberOfPixelsPerBlockCol(ossim_uint32 pixels)
{
   std::ostringstream out;
   
   out << std::setw(4)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << pixels;
   
   memcpy(theNumberOfPixelsPerBlockVert, out.str().c_str(), 4);
}

ossim_uint64 ossimNitfImageHeaderV2_1::getDataLocation() const
{
   return theDataLocation;
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

void ossimNitfImageHeaderV2_1::setImageId(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(10)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theImageId, out.str().c_str(), 10);
}

void ossimNitfImageHeaderV2_1::setAquisitionDateTime(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(14)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDateTime, out.str().c_str(), 14);
}

void ossimNitfImageHeaderV2_1::setTargetId(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(17)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theTargetId, out.str().c_str(), 17);
   
}

void ossimNitfImageHeaderV2_1::setTitle(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(80)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theTitle, out.str().c_str(), 80);
}

void ossimNitfImageHeaderV2_1::setSecurityClassification(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
      
       << value.trim().c_str();
   
   memcpy(theSecurityClassification, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setSecurityClassificationSystem(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theSecurityClassificationSys, out.str().c_str(), 2);
}

void ossimNitfImageHeaderV2_1::setCodewords(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(11)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theCodewords, out.str().c_str(), 11);
}

void ossimNitfImageHeaderV2_1::setControlAndHandling(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theControlAndHandling, out.str().c_str(), 2);
}

void ossimNitfImageHeaderV2_1::setReleasingInstructions(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(20)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theReleasingInstructions, out.str().c_str(), 20);
}

void ossimNitfImageHeaderV2_1::setDeclassificationType(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDeclassificationType, out.str().c_str(), 2);
}

void ossimNitfImageHeaderV2_1::setDeclassificationDate(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(8)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDeclassificationDate, out.str().c_str(), 8);
}

void ossimNitfImageHeaderV2_1::setDeclassificationExempt(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(4)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDeclassificationExempt, out.str().c_str(), 4);
}

void ossimNitfImageHeaderV2_1::setDowngrade(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDowngrade, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setDowngradeDate(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(8)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theDowngradeDate, out.str().c_str(), 8);
}

void ossimNitfImageHeaderV2_1::setClassificationText(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(43)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theClassificationText, out.str().c_str(), 43);
}

void ossimNitfImageHeaderV2_1::setClassificationAuthorityType(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theClassificationAuthType, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setClassificationAuthority(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(40)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theClassificationAuthority, out.str().c_str(), 40);
}

void ossimNitfImageHeaderV2_1::setClassificationReason(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theClassificationReason, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setSecuritySourceDate(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(8)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theSecuritySourceDate, out.str().c_str(), 8);
}


void ossimNitfImageHeaderV2_1::setSecurityControlNumber(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(15)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theSecurityControlNumber, out.str().c_str(), 15);
}

void ossimNitfImageHeaderV2_1::setEncryption(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theEncryption, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setImageSource(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(42)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theImageSource, out.str().c_str(), 42);
}

void ossimNitfImageHeaderV2_1::setJustification(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theJustification, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setCoordinateSystem(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theCoordinateSystem, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setGeographicLocation(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(60)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theGeographicLocation, out.str().c_str(), 60);
}

void ossimNitfImageHeaderV2_1::setNumberOfComments(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(1)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theNumberOfComments, out.str().c_str(), 1);
}

void ossimNitfImageHeaderV2_1::setCompression(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theCompression, out.str().c_str(), 2);
}

void ossimNitfImageHeaderV2_1::setCompressionRateCode(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(4)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theCompressionRateCode, out.str().c_str(), 4);
}
void ossimNitfImageHeaderV2_1::setDisplayLevel(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(3)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << value.trim().c_str();
   
   memcpy(theDisplayLevel, out.str().c_str(), 3);
}

void ossimNitfImageHeaderV2_1::setAttachmentLevel(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(3)
       << std::setfill('0')
       << std::setiosflags(ios::right)
       << value.trim().c_str();
   
   memcpy(theAttachmentLevel, out.str().c_str(), 3);
}

void ossimNitfImageHeaderV2_1::setImageMagnification(const ossimString& value)
{
   std::ostringstream out;

   out << std::setw(4)
       << std::setfill(' ')
       << std::setiosflags(ios::left)
       << value.trim().c_str();
   
   memcpy(theImageMagnification, out.str().c_str(), 4);
}

void ossimNitfImageHeaderV2_1::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   std::ostringstream out;
   
   if(!property) return;
   
   if(name.contains(IID1_KW))
   {
      setImageId(property->valueToString());
   }
   else if(name.contains(IDATIM_KW))
   {
      setAquisitionDateTime(property->valueToString());
   }
   else if(name.contains(TGTID_KW))
   {
      setTargetId(property->valueToString());
   }
   else if(name.contains(IID2_KW))
   {
      setTitle(property->valueToString());
   }
   else if(name.contains(ISCLAS_KW))
   {
      setSecurityClassification(property->valueToString());
   }
   else if(name.contains(ISCLSY_KW))
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
   else if(name.contains(ISSRDT_KW))
   {
      setSecuritySourceDate(property->valueToString());
   }
   else if(name.contains(ISCTLN_KW))
   {
      setSecurityControlNumber(property->valueToString());
   }
   else if(name.contains(ENCRYP_KW))
   {
      setEncryption(property->valueToString());
   }
   else if(name.contains(ISORCE_KW))
   {
      setImageSource(property->valueToString());
   }
   else if(name.contains(PVTYPE_KW))
   {
      setPixelType(property->valueToString());
   }
   else if(name.contains(IREP_KW))
   {
      setRepresentation(property->valueToString());
   }
   else if(name.contains(ICAT_KW))
   {
      setCategory(property->valueToString());
   }
   else if(name.contains(ABPP_KW))
   {
      setActualBitsPerPixel(property->valueToString().toUInt32());
   }
   else if(name.contains(PJUST_KW))
   {
      setJustification(property->valueToString());
   }
   else if(name.contains(ICORDS_KW))
   {
      setCoordinateSystem(property->valueToString());
   }
   else if(name.contains(IGEOLO_KW))
   {
      setGeographicLocation(property->valueToString());
   }
   else if(name.contains(NICOM_KW))
   {
      setNumberOfComments(property->valueToString());
   }
   else if(name.contains(IC_KW))
   {
      setCompression(property->valueToString());
   }
   else if(name.contains(COMRAT_KW))
   {
      setCompressionRateCode(property->valueToString());
   }
   else if(name.contains(ISYNC_KW))
   {
   }
   else if(name.contains(IMODE_KW))
   {
      setImageMode(*(property->valueToString().trim().begin()));
   }
   else if(name.contains(NBPR_KW))
   {
      setBlocksPerRow(property->valueToString().toUInt32());
   }
   else if(name.contains(NBPC_KW))
   {
      setBlocksPerCol(property->valueToString().toUInt32());
   }
   else if(name.contains(NPPBH_KW))
   {
      setNumberOfPixelsPerBlockRow(property->valueToString().toUInt32());
   }
   else if(name.contains(NPPBV_KW))
   {
      setNumberOfPixelsPerBlockCol(property->valueToString().toUInt32());
   }
   else if(name.contains(NBPP_KW))
   {
      setBitsPerPixel(property->valueToString().toUInt32());
   }
   else if(name.contains(IDLVL_KW))
   {
      setDisplayLevel(property->valueToString());
   }
   else if(name.contains(IALVL_KW))
   {
      setAttachmentLevel(property->valueToString());
   }
   else if(name.contains(ILOC_KW))
   {
   }
   else if(name.contains(IMAG_KW))
   {
      (property->valueToString());
   }
   else
   {
      ossimNitfImageHeader::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfImageHeaderV2_1::getProperty(const ossimString& name)const
{
   ossimProperty* property = NULL;

   if(name == IID1_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageId).trim());
   }
   else if(name == IDATIM_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDateTime).trim());
   }
   else if(name == TGTID_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theTargetId).trim());
   }
   else if(name == IID2_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theTitle).trim());
   }
   else if(name == ISCLAS_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theSecurityClassification).trim());
   }
   else if(name == ISCLSY_KW)
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
   else if(name == ENCRYP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theEncryption).trim());
   }
   else if(name == ISORCE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageSource).trim());
   }
   else if(name == PVTYPE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(thePixelValueType).trim());
   }
   else if(name == IREP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theRepresentation).trim());
   }
   else if(name == ICAT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCategory).trim());
   }
   else if(name == ABPP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theActualBitsPerPixelPerBand).trim());
   }
   else if(name == PJUST_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theJustification).trim());
   }
   else if(name == ICORDS_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCoordinateSystem).trim());
   }
   else if(name == IGEOLO_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theGeographicLocation).trim());
   }
   else if(name == NICOM_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfComments).trim());
   }
   else if(name == IC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCompression).trim());
   }
   else if(name == COMRAT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCompressionRateCode).trim());
   }
   else if(name == ISYNC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageSyncCode).trim());
   }
   else if(name == IMODE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageMode).trim());
   }
   else if(name == NBPR_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBlocksPerRow).trim());
   }
   else if(name == NBPC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBlocksPerCol).trim());
   }
   else if(name == NPPBH_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfPixelsPerBlockHoriz).trim());
   }
   else if(name == NPPBV_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfPixelsPerBlockVert).trim());
   }
   else if(name == NBPP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBitsPerPixelPerBand).trim());
   }
   else if(name == IDLVL_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDisplayLevel).trim());
   }
   else if(name == IALVL_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theAttachmentLevel).trim());
   }
   else if(name == ILOC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageLocation).trim());
   }
   else if(name == IMAG_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageMagnification).trim());
   }
   else
   {
      return ossimNitfImageHeader::getProperty(name);
   }
   
   return property;
}

void ossimNitfImageHeaderV2_1::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(IID1_KW);
   propertyNames.push_back(IDATIM_KW);
   propertyNames.push_back(TGTID_KW);
   propertyNames.push_back(IID2_KW);
   propertyNames.push_back(ISCLAS_KW);
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
   propertyNames.push_back(ENCRYP_KW);
   propertyNames.push_back(ISORCE_KW);
   propertyNames.push_back(PVTYPE_KW);
   propertyNames.push_back(IREP_KW);
   propertyNames.push_back(ICAT_KW);
   propertyNames.push_back(ABPP_KW);
   propertyNames.push_back(PJUST_KW);
   propertyNames.push_back(ICORDS_KW);
   propertyNames.push_back(IGEOLO_KW);
   propertyNames.push_back(NICOM_KW);
   propertyNames.push_back(IC_KW);
   propertyNames.push_back(COMRAT_KW);
   propertyNames.push_back(NBANDS_KW);
   propertyNames.push_back(XBANDS_KW);
   propertyNames.push_back(ISYNC_KW);
   propertyNames.push_back(IMODE_KW);
   propertyNames.push_back(NBPR_KW);
   propertyNames.push_back(NBPC_KW);
   propertyNames.push_back(NPPBH_KW);
   propertyNames.push_back(NPPBV_KW);
   propertyNames.push_back(NBPP_KW);
   propertyNames.push_back(IDLVL_KW);
   propertyNames.push_back(IALVL_KW);
   propertyNames.push_back(ILOC_KW);
   propertyNames.push_back(IMAG_KW);
   
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
   
   return NULL;
}
