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
// $Id: ossimNitfImageHeaderV2_0.cpp 10160 2007-01-02 15:38:54Z gpotts $


#include <iomanip>

#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDrect.h>
#include <cstring> //for memset
#include <ossim/base/ossimEndian.h>
#include <ossim/support_data/ossimNitfVqCompressionHeader.h>
#include <ossim/base/ossimTrace.h>

// static const ossimTrace traceDebug("ossimNitfFileHeaderV2_0:debug");

RTTI_DEF1(ossimNitfImageHeaderV2_0,
          "ossimNitfImageHeaderV2_0",
          ossimNitfImageHeaderV2_X);

const ossimString ossimNitfImageHeaderV2_0::ISCODE_KW = "iscode";
const ossimString ossimNitfImageHeaderV2_0::ISCTLH_KW = "isctlh";
const ossimString ossimNitfImageHeaderV2_0::ISREL_KW = "isrel";
const ossimString ossimNitfImageHeaderV2_0::ISCAUT_KW = "iscaut";
const ossimString ossimNitfImageHeaderV2_0::CTLN_KW = "ctln";
const ossimString ossimNitfImageHeaderV2_0::ISDWNG_KW = "isdwng";
const ossimString ossimNitfImageHeaderV2_0::ISDEVT_KW = "isdevt";

ossimNitfImageHeaderV2_0::ossimNitfImageHeaderV2_0()
{
   clearFields();
}

ossimNitfImageHeaderV2_0::~ossimNitfImageHeaderV2_0()
{
}

void ossimNitfImageHeaderV2_0::parseStream(std::istream &in)
{
   clearFields();
   theImageBands.clear();
      
   in.read(theType, 2);
   if(ossimString(theType) != "IM")
   {
      // error
   }
   in.read(theImageId, 10);
   in.read(theDateTime, 14);
   in.read(theTargetId, 17);
   in.read(theTitle, 80);
   in.read(theSecurityClassification, 1);
   in.read(theCodewords, 40);
   in.read(theControlAndHandling, 40);
   in.read(theReleasingInstructions, 40);
   in.read(theClassificationAuthority, 20);
   in.read(theSecurityControlNumber, 20);
   in.read(theSecurityDowngrade, 6);
   if(ossimString(theSecurityDowngrade) == "999998")
   {
      in.read(theDowngradingEvent, 40);
   }
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
   if(theCoordinateSystem[0] != 'N')
   {
      in.read(theGeographicLocation, 60);
   }
   in.read(theNumberOfComments, 1);
   ossim_uint32 numberOfComments = ossimString(theNumberOfComments).toInt32();
   if(numberOfComments > 0)
   {
      // for now let's ignore the comments about the image
      in.ignore(numberOfComments*80);
   }
   in.read(theCompression, 2);
   // check to see if there is compression
   ossimString temp = theCompression;
   if((temp !=  "NC") &&
      (temp != "NM"))
   {
      // get the rate if compression exists.
      in.read(theCompressionRateCode, 4);
   }
   in.read(theNumberOfBands, 1);
   ossim_uint32 numberOfBands = ossimString(theNumberOfBands).toUInt32();
   theImageBands.resize(numberOfBands);
   ossim_uint32 idx = 0;
   while(idx < numberOfBands)
   {
      theImageBands[idx] = new ossimNitfImageBandV2_0;
      theImageBands[idx]->parseStream(in);
      ++idx;
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
   std::streampos userDefinedDataLen = (std::streampos)ossimString(theUserDefinedImageDataLength).toUInt32();
   theTagList.clear();
   ossimNitfTagInformation         headerTag;
   std::streampos start   = in.tellg();
   std::streampos current = in.tellg();
   if(userDefinedDataLen > 0)
   {
      in.read(theUserDefinedOverflow, 3);
      
      while((current - start) < userDefinedDataLen)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
   }

   in.read(theExtendedSubheaderDataLen, 5);
   std::streampos extSubHeadLen = (std::streampos)ossimString(theExtendedSubheaderDataLen).toUInt32();
   start   = in.tellg();
   current = in.tellg();
   if(extSubHeadLen > 0)
   {
      in.read(theExtendedSubheaderOverflow, 3);

      // ossim_uint32 test = 0;
      while((current - start) < extSubHeadLen)
      {
         headerTag.parseStream(in);
         theTagList.push_back(headerTag);
         // test = headerTag.getTagLength()+in.tellg();
         // in.ignore(headerTag.getTagLength());
         // headerTag.clearFields();
         current = in.tellg();
      }
   }
   
   ossimString compressionType = theCompression;
   compressionType = compressionType.trim().upcase();
   ossimEndian endian;
   ossim_uint64 locationBefore = in.tellg();
    if((compressionType == "NM")||
       (compressionType == "M0")||
       (compressionType == "M3")||
       (compressionType == "M4"))
   {
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
    
    //
    // The stream should now be at the start of the data location so capture
    // it.
    //
    theDataLocation = in.tellg();
}

void ossimNitfImageHeaderV2_0::writeStream(std::ostream &out)
{
}

std::ostream& ossimNitfImageHeaderV2_0::print(std::ostream& out)const
{
   out << setiosflags(std::ios::left) << "ossimNitfImageHeaderV2_0::print"
       << std::setw(24) << "\nIM:" << theType 
       << std::setw(24) << "\nIID:" << theImageId 
       << std::setw(24) << "\nIDATIM:" << theDateTime 
       << std::setw(24) << "\nTGTID:" << theTargetId 
       << std::setw(24) << "\nITITLE:" << theTitle    
       << std::setw(24) << "\nISCLAS:" << theSecurityClassification 
       << std::setw(24) << "\nISCODE:" << theCodewords 
       << std::setw(24) << "\nISCTLH:" << theControlAndHandling 
       << std::setw(24) << "\nISREL:" << theReleasingInstructions 
       << std::setw(24) << "\nISCAUT:" << theClassificationAuthority 
       << std::setw(24) << "\nISCTLN:" << theSecurityControlNumber 
       << std::setw(24) << "\nISDWNG:" << theSecurityDowngrade     
       << std::setw(24) << "\nISDEBT:" << theDowngradingEvent 
       << std::setw(24) << "\nENCRYP:" << theEncryption 
       << std::setw(24) << "\nISORCE:" << theImageSource 
       << std::setw(24) << "\nNROWS:" << theSignificantRows 
       << std::setw(24) << "\nNCOLS:" << theSignificantCols 
       << std::setw(24) << "\nPVTYPE:" << thePixelValueType 
       << std::setw(24) << "\nIREP:" << theRepresentation 
       << std::setw(24) << "\nICAT:" << theCategory       
       << std::setw(24) << "\nABPP:" << theActualBitsPerPixelPerBand 
       << std::setw(24) << "\nPJUST:" << theJustification 
       << std::setw(24) << "\nICORDS:" << theCoordinateSystem 
       << std::setw(24) << "\nIGEOLO:" << theGeographicLocation 
       << std::setw(24) << "\nNICOM:" << theNumberOfComments 
       << std::setw(24) << "\nIC:" << theCompression      
       << std::setw(24) << "\nCOMRAT:" << theCompressionRateCode 
       << std::setw(24) << "\nNBANDS:" << theNumberOfBands ;

   ossim_uint32 idx = 0;
   for(idx = 0; idx < theImageBands.size(); ++idx)
   {
      if(theImageBands[idx].valid())
      {
         theImageBands[idx]->print(out, idx); 
      }
   }
   
   out << std::setw(24) << "\nISYNC:" << theImageSyncCode 
       << std::setw(24) << "\nIMODE:" << theImageMode 
       << std::setw(24) << "\nNBPR:" << theNumberOfBlocksPerRow 
       << std::setw(24) << "\nNBPC:" << theNumberOfBlocksPerCol 
       << std::setw(24) << "\nNPPBH:" << theNumberOfPixelsPerBlockHoriz 
       << std::setw(24) << "\nNPPBV:" << theNumberOfPixelsPerBlockVert 
       << std::setw(24) << "\nNBPP:" << theNumberOfBitsPerPixelPerBand 
       << std::setw(24) << "\nIDLVL:" << theDisplayLevel 
       << std::setw(24) << "\nIALVL:" << theAttachmentLevel 
       << std::setw(24) << "\nILOC:" << theImageLocation 
       << std::setw(24) << "\nIMAG:" << theImageMagnification 
       << std::setw(24) << "\nUDIDL:" << theUserDefinedImageDataLength 
       << std::setw(24) << "\nUDOFL:" << theUserDefinedOverflow 
       << std::setw(24) << "\nIXSHDL:" << theExtendedSubheaderDataLen 
       << std::setw(24) << "\nIXSOFL:" << theExtendedSubheaderOverflow 
       << std::setw(24) << "\ntheDataLocation:" << theDataLocation
       << std::setw(24) << "\ntheBlockedImageDataOffset:"
       << theBlockedImageDataOffset
       << std::setw(24) << "\ntheBlockMaskRecordLength:"
       << theBlockMaskRecordLength
       << std::setw(24) << "\nthePadPixelMaskRecordLength:"
       << thePadPixelMaskRecordLength
       << std::setw(24) << "\ntheTransparentOutputPixelCodeLength:"
       << theTransparentOutputPixelCodeLength
       << std::setw(24) << "\nthePadOutputPixelCode:" << thePadOutputPixelCode
       << std::endl;

   if(theCompressionHeader.valid())
   {
      theCompressionHeader->print(out);
      out << std::endl;
   }

   printTags(out);
   
   
//    out << setw(24) << "\nTAGS:\n";
//    std::copy(theTagList.begin(),
//              theTagList.end(),
//              std::ostream_iterator<ossimNitfTagInformation>(out,"\n"));
   return out;
}

ossimDrect ossimNitfImageHeaderV2_0::getImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation, (char*)(&theImageLocation[5])).toDouble());
   
   double rows = ossimString(theSignificantRows).toDouble();
   double cols = ossimString(theSignificantCols).toDouble();

   ossimDpt lr(ul.x + cols-1,
               ul.y + rows-1);
   
   return ossimDrect(ul, lr);
}

ossimDrect ossimNitfImageHeaderV2_0::getBlockImageRect()const
{
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation, (char*)(&theImageLocation[5])).toDouble());
   
   double rows = getNumberOfPixelsPerBlockVert()*getNumberOfBlocksPerCol();
   double cols = getNumberOfPixelsPerBlockHoriz()*getNumberOfBlocksPerRow();;

   ossimDpt lr(ul.x + cols-1,
               ul.y + rows-1);
   
   return ossimDrect(ul, lr);
}

bool ossimNitfImageHeaderV2_0::isCompressed()const
{
   ossimString temp = theCompression;
   temp = temp.trim();
   return ((temp != "NC") &&
           (temp != "NM"));
}

bool ossimNitfImageHeaderV2_0::isEncrypted()const
{
   return (theEncryption[0] == '1');
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfBands()const
{
   return ossimString(theNumberOfBands).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfRows()const
{
   return ossimString(theSignificantRows).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfCols()const
{
   return ossimString(theSignificantCols).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfBlocksPerRow()const
{
   return ossimString(theNumberOfBlocksPerRow).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfBlocksPerCol()const
{
   return ossimString(theNumberOfBlocksPerCol).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfPixelsPerBlockHoriz()const
{
   return ossimString(theNumberOfPixelsPerBlockHoriz).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getNumberOfPixelsPerBlockVert()const
{
   return ossimString(theNumberOfPixelsPerBlockVert).toInt32();
}

ossimString ossimNitfImageHeaderV2_0::getImageId()const
{
   return theImageId;
}

ossim_int32 ossimNitfImageHeaderV2_0::getBitsPerPixelPerBand()const
{
   return ossimString(theNumberOfBitsPerPixelPerBand).toInt32();
}

ossim_int32 ossimNitfImageHeaderV2_0::getActualBitsPerPixelPerBand()const
{
   return ossimString(theActualBitsPerPixelPerBand).toInt32();
}

ossimString ossimNitfImageHeaderV2_0::getIMode() const
{
   return ossimString(theImageMode).trim();
}

ossimString ossimNitfImageHeaderV2_0::getSecurityClassification()const
{
   return ossimString(theSecurityClassification);
}

ossimString ossimNitfImageHeaderV2_0::getAcquisitionDateMonthDayYear(ossim_uint8 separationChar)const
{
   ossimString result;
   ossimString month(theDateTime+9,
                     theDateTime+12);
   month = month.downcase();
   if(month == "jan")
   {
      result += "01";
   }
   else if(month == "feb")
   {
      result += "02";
   }
   else if(month == "mar")
   {
      result += "03";
   }
   else if(month == "apr")
   {
      result += "04";
   }
   else if(month == "may")
   {
      result += "05";
   }
   else if(month == "jun")
   {
      result += "06";
   }
   else if(month == "jul")
   {
      result += "07";
   }
   else if(month == "aug")
   {
      result += "08";
   }
   else if(month == "sep")
   {
      result += "09";
   }
   else if(month == "oct")
   {
      result += "10";
   }
   else if(month == "nov")
   {
      result += "11";
   }
   else if(month == "dec")
   {
      result += "12";
   }
   result += ossimString(separationChar);
   result += ossimString(theDateTime,
                         theDateTime + 2);
   result += ossimString(separationChar);
   ossimString year(theDateTime+12,
                    theDateTime+14);
   if(year.toUInt32() < 70)
   {
      result += "20";
   }
   else
   {
      result += "19";
   }
   result += ossimString(theDateTime+12,
                         theDateTime+14);

   return result;
}

ossimString ossimNitfImageHeaderV2_0::getCategory()const
{
   return theCategory;
}

ossimString ossimNitfImageHeaderV2_0::getImageSource()const
{
   return theImageSource;
}

ossimString ossimNitfImageHeaderV2_0::getRepresentation()const
{
   return theRepresentation;
}

ossimString ossimNitfImageHeaderV2_0::getCoordinateSystem()const
{
	return theCoordinateSystem;
}

ossimString ossimNitfImageHeaderV2_0::getGeographicLocation()const
{
	return theGeographicLocation;
}

ossimString ossimNitfImageHeaderV2_0::getPixelValueType()const
{
   return ossimString(thePixelValueType).trim();
}

bool ossimNitfImageHeaderV2_0::hasBlockMaskRecords()const
{
   return (theBlockMaskRecords.size() > 0);
}

bool ossimNitfImageHeaderV2_0::hasPadPixelMaskRecords()const
{
   return (thePadPixelMaskRecords.size() > 0);
}

bool ossimNitfImageHeaderV2_0::hasTransparentCode()const
{
   return (theTransparentOutputPixelCodeLength > 0);
}

ossim_uint32 ossimNitfImageHeaderV2_0::getTransparentCode()const
{
   return thePadOutputPixelCode;
}

ossim_uint32 ossimNitfImageHeaderV2_0::getBlockMaskRecordOffset(ossim_uint32 blockNumber,
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
ossim_uint32 ossimNitfImageHeaderV2_0::getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
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

void ossimNitfImageHeaderV2_0::clearFields()
{
   theTagList.clear();
   
   memset(theType, ' ', 2);
   memset(theImageId, ' ', 10);
   memset(theDateTime, ' ', 14);
   memset(theTargetId, ' ', 17);
   memset(theTitle, ' ', 80);
   memset(theSecurityClassification, ' ', 1);
   memset(theCodewords, ' ', 40);
   memset(theControlAndHandling, ' ', 40);
   memset(theReleasingInstructions, ' ', 40);
   memset(theClassificationAuthority, ' ', 20);
   memset(theSecurityControlNumber, ' ', 20);
   memset(theSecurityDowngrade, ' ', 6);
   memset(theDowngradingEvent, ' ', 40);
   memset(theEncryption, ' ', 1);
   memset(theImageSource, ' ', 42);
   memset(theSignificantRows, ' ', 8);
   memset(theSignificantCols, ' ', 8);
   memset(thePixelValueType, ' ', 3);
   memset(theRepresentation, ' ', 8);
   memset(theCategory, ' ', 8);
   memset(theActualBitsPerPixelPerBand, ' ', 2);
   memset(theJustification, ' ', 1);
   memset(theCoordinateSystem, ' ', 1);
   memset(theGeographicLocation, ' ', 60);
   memset(theNumberOfComments, ' ', 1);
   memset(theCompression, ' ', 2);
   memset(theCompressionRateCode, ' ', 4);
   memset(theNumberOfBands, ' ', 1);
   memset(theImageSyncCode, ' ', 1);
   memset(theImageMode, ' ', 1);
   memset(theNumberOfBlocksPerRow, ' ', 4);
   memset(theNumberOfBlocksPerCol, ' ', 4);
   memset(theNumberOfPixelsPerBlockHoriz, ' ', 4);
   memset(theNumberOfPixelsPerBlockVert, ' ', 4);
   memset(theNumberOfBitsPerPixelPerBand, ' ', 2);
   memset(theDisplayLevel, ' ', 3);
   memset(theAttachmentLevel, ' ', 3);
   memset(theImageLocation, ' ', 10);
   memset(theImageMagnification, ' ', 4);
   memset(theUserDefinedImageDataLength, ' ', 5);
   memset(theUserDefinedOverflow, ' ', 3);
   memset(theExtendedSubheaderDataLen, ' ', 5);
   memset(theExtendedSubheaderOverflow, ' ', 3);
   
   theBlockMaskRecords.clear();
   thePadPixelMaskRecords.clear();

   theBlockedImageDataOffset = 0;
   theBlockMaskRecordLength = 0;
   thePadPixelMaskRecordLength = 0;
   theTransparentOutputPixelCodeLength = 0;
   thePadOutputPixelCode = 0;
   theCompressionHeader  = 0;
   theType[2] = '\0';
   theImageId[10] = '\0';
   theDateTime[14] = '\0';
   theTargetId[17] = '\0';
   theTitle[80] = '\0';
   theSecurityClassification[1] = '\0';
   theCodewords[40] = '\0';
   theControlAndHandling[40] = '\0';
   theReleasingInstructions[40] = '\0';
   theClassificationAuthority[20] = '\0';
   theSecurityControlNumber[20] = '\0';
   theSecurityDowngrade[6] = '\0';
   theDowngradingEvent[40] = '\0';
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

const ossimRefPtr<ossimNitfCompressionHeader> ossimNitfImageHeaderV2_0::getCompressionHeader()const
{
   return theCompressionHeader;
}

const ossimRefPtr<ossimNitfImageBand> ossimNitfImageHeaderV2_0::getBandInformation(ossim_uint32 idx)const
{
   if(idx < theImageBands.size())
   {
      return (ossimNitfImageBand*)theImageBands[idx].get();
   }
   
   return NULL;
}
