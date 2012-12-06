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
// $Id: ossimNitfImageHeaderV2_0.cpp 21518 2012-08-22 21:15:56Z dburken $


#include <iomanip>

#include <ossim/support_data/ossimNitfImageHeaderV2_0.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDrect.h>
#include <cstring> //for memset
#include <ossim/base/ossimEndian.h>
#include <ossim/support_data/ossimNitfVqCompressionHeader.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStringProperty.h>
#include <stdexcept>
#include <sstream>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimNitfCommon.h>
static const ossimTrace traceDebug(ossimString("ossimNitfImageHeaderV2_0:debug"));

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

   //---
   // Note: "C4" added to skip over the image data mask subheader.
   // See MIL-STD-2500A paragraph 5.5.1.5
   //
   // Seems like CIB data does not have.  Could not find a hard and fast
   // rule in the specs.  Need a better was to detect if this needs to be
   // read. (drb - 20100317)
   //---
   if( (compressionType == "NM") ||
       (compressionType == "M0") ||
       (compressionType == "M3") ||
       (compressionType == "M4") ||
       ( (compressionType == "C4") && ( !getImageId().contains("CIB")) ) )
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
      if((thePadPixelMaskRecordLength > 0)||
         (( (getCompressionCode().upcase() == "M3"))&&
          (thePadPixelMaskRecordLength == 0)))
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
   }

   theCompressionHeader = 0;
   if((getCompressionCode() == "C4")||
      (getCompressionCode() == "M4"))
   {
      ossimRefPtr<ossimNitfVqCompressionHeader> compressionHeader =
         new ossimNitfVqCompressionHeader;
      compressionHeader->parseStream(in);
      // do a check to see if the compression header is good
      //
      
      if( compressionHeader->getCompressionAlgorithmId()!= 1 )
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
   out.write(theType, 2);
   out.write(theImageId, 10);
   out.write(theDateTime, 14);
   out.write(theTargetId, 17);
   out.write(theTitle, 80);
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
   if(theCoordinateSystem[0] != 'N')
   {
      out.write(theGeographicLocation, 60);
   }
   out.write(theNumberOfComments, 1);
   out.write(theCompression, 2);
   if(ossimString(theCompression) != "NC")
   {
      out.write(theCompressionRateCode, 4);
   }
   out.write(theNumberOfBands, 1);
   ossim_uint32 bandIdx = 0;
   for(bandIdx=0;bandIdx<theImageBands.size();++bandIdx)
   {
      theImageBands[bandIdx]->writeStream(out);
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
   ossim_uint32 totalLength = getTotalTagLength();
   
   if(totalLength == 0)
   {
      //memcpy(theExtendedSubheaderDataLen, "00000", 5);
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
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfFileHeaderV2_0::writeStream: Only support writing of total tag length < 99999" << std::endl;
      }
   }
}

std::ostream& ossimNitfImageHeaderV2_0::print(std::ostream& out,
                                              const std::string& prefix) const
{
   out << setiosflags(std::ios::left)
       << prefix << std::setw(24) << "IM:"
       << theType << "\n"
       << prefix << std::setw(24) << "IID:"
       << theImageId  << "\n"
       << prefix << std::setw(24) << "IDATIM:"
       << theDateTime << "\n" 
       << prefix << std::setw(24) << "TGTID:"
       << theTargetId  << "\n"
       << prefix << std::setw(24) << "ITITLE:"
       << theTitle  << "\n"   
       << prefix << std::setw(24) << "ISCLAS:"
       << theSecurityClassification << "\n"
       << prefix << std::setw(24) << "ISCODE:"
       << theCodewords  << "\n"
       << prefix << std::setw(24) << "ISCTLH:"
       << theControlAndHandling << "\n" 
       << prefix << std::setw(24) << "ISREL:"
       << theReleasingInstructions << "\n" 
       << prefix << std::setw(24) << "ISCAUT:"
       << theClassificationAuthority << "\n" 
       << prefix << std::setw(24) << "ISCTLN:"
       << theSecurityControlNumber << "\n" 
       << prefix << std::setw(24) << "ISDWNG:"
       << theSecurityDowngrade  << "\n"    
       << prefix << std::setw(24) << "ISDEBT:"
       << theDowngradingEvent  << "\n"
       << prefix << std::setw(24) << "ENCRYP:"
       << theEncryption  << "\n"
       << prefix << std::setw(24) << "ISORCE:"
       << theImageSource  << "\n"
       << prefix << std::setw(24) << "NROWS:"
       << theSignificantRows  << "\n"
       << prefix << std::setw(24) << "NCOLS:"
       << theSignificantCols  << "\n"
       << prefix << std::setw(24) << "PVTYPE:"
       << thePixelValueType  << "\n"
       << prefix << std::setw(24) << "IREP:"
       << theRepresentation  << "\n"
       << prefix << std::setw(24) << "ICAT:"
       << theCategory  << "\n"      
       << prefix << std::setw(24) << "ABPP:"
       << theActualBitsPerPixelPerBand << "\n"
       << prefix << std::setw(24) << "PJUST:"
       << theJustification  << "\n"
       << prefix << std::setw(24) << "ICORDS:"
       << theCoordinateSystem  << "\n"
       << prefix << std::setw(24) << "IGEOLO:"
       << theGeographicLocation  << "\n"
       << prefix << std::setw(24) << "NICOM:"
       << theNumberOfComments << "\n" 
       << prefix << std::setw(24) << "IC:"
       << theCompression  << "\n"     
       << prefix << std::setw(24) << "COMRAT:"
       << theCompressionRateCode  << "\n"
       << prefix << std::setw(24) << "NBANDS:"
       << theNumberOfBands  << "\n";

   ossim_uint32 idx = 0;
   for(idx = 0; idx < theImageBands.size(); ++idx)
   {
      if(theImageBands[idx].valid())
      {
         theImageBands[idx]->print(out, prefix, idx); 
      }
   }
   
   out << prefix << std::setw(24) << "ISYNC:"
       << theImageSyncCode << "\n" 
       << prefix << std::setw(24) << "IMODE:"
       << theImageMode  << "\n"
       << prefix << std::setw(24) << "NBPR:"
       << theNumberOfBlocksPerRow << "\n" 
       << prefix << std::setw(24) << "NBPC:"
       << theNumberOfBlocksPerCol << "\n" 
       << prefix << std::setw(24) << "NPPBH:"
       << theNumberOfPixelsPerBlockHoriz << "\n" 
       << prefix << std::setw(24) << "NPPBV:"
       << theNumberOfPixelsPerBlockVert  << "\n"
       << prefix << std::setw(24) << "NBPP:"
       << theNumberOfBitsPerPixelPerBand  << "\n"
       << prefix << std::setw(24) << "IDLVL:"
       << theDisplayLevel << "\n"
       << prefix << std::setw(24) << "IALVL:"
       << theAttachmentLevel << "\n" 
       << prefix << std::setw(24) << "ILOC:"
       << theImageLocation << "\n" 
       << prefix << std::setw(24) << "IMAG:"
       << theImageMagnification  << "\n"
       << prefix << std::setw(24) << "UDIDL:"
       << theUserDefinedImageDataLength  << "\n"
       << prefix << std::setw(24) << "UDOFL:"
       << theUserDefinedOverflow  << "\n"
       << prefix << std::setw(24) << "IXSHDL:"
       << theExtendedSubheaderDataLen  << "\n"
       << prefix << std::setw(24) << "IXSOFL:"
       << theExtendedSubheaderOverflow  << "\n";

   if ( traceDebug() )
   {
       out << prefix << std::setw(24) << "theDataLocation:"
           << theDataLocation << "\n"
           << prefix << std::setw(24) << "theBlockedImageDataOffset:"
           << theBlockedImageDataOffset << "\n"
           << prefix << std::setw(24) << "theBlockMaskRecordLength:"
           << theBlockMaskRecordLength << "\n"
           << prefix << std::setw(24) << "thePadPixelMaskRecordLength:"
           << thePadPixelMaskRecordLength << "\n"
           << prefix << std::setw(24) << "theTransparentOutputPixelCodeLength:"
           << theTransparentOutputPixelCodeLength << "\n"
           << prefix << std::setw(24) << "thePadOutputPixelCode:"
           << thePadOutputPixelCode << "\n";
   }

   if(theCompressionHeader.valid())
   {
      theCompressionHeader->print(out, prefix);
   }

   out << std::endl;

   return printTags(out, prefix);
}

bool ossimNitfImageHeaderV2_0::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   ossimNitfImageHeaderV2_X::saveState(kwl, prefix);
   std::ostringstream out;
   
   kwl.add(prefix, "ITITLE", theTitle);

   ossim_uint32 idx = 0;
   if(theCompressionHeader.valid())
   {
      theCompressionHeader->saveState(kwl, prefix + "compression_header.");
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

ossimString  ossimNitfImageHeaderV2_0::getImageDateAndTime() const
{
   return ossimString(theDateTime);
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
   
   memcpy(theType, "IM", 2);
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
   memset(theEncryption, '0', 1);
   memset(theImageSource, ' ', 42);
   memset(theSignificantRows, ' ', 8);
   memset(theSignificantCols, ' ', 8);
   memset(thePixelValueType, ' ', 3);
   memset(theRepresentation, ' ', 8);
   memset(theCategory, ' ', 8);
   memset(theActualBitsPerPixelPerBand, '0', 2);
   memset(theJustification, ' ', 1);
   memset(theCoordinateSystem, 'N', 1);
   memset(theGeographicLocation, ' ', 60);
   memset(theNumberOfComments, '0', 1);
   memcpy(theCompression, "NC", 2);
   memset(theCompressionRateCode, ' ', 4);
   memset(theNumberOfBands, ' ', 1);
   memset(theImageSyncCode, '0', 1);
   memset(theImageMode, ' ', 1);
   memset(theNumberOfBlocksPerRow, ' ', 4);
   memset(theNumberOfBlocksPerCol, ' ', 4);
   memset(theNumberOfPixelsPerBlockHoriz, ' ', 4);
   memset(theNumberOfPixelsPerBlockVert, ' ', 4);
   memset(theNumberOfBitsPerPixelPerBand, '0', 2);
   memset(theDisplayLevel, ' ', 3);
   memset(theAttachmentLevel, ' ', 3);
   memcpy(theImageLocation, "0000000000", 10);
   memcpy(theImageMagnification, "1.00", 4);
   memset(theUserDefinedImageDataLength, '0', 5);
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

void ossimNitfImageHeaderV2_0::setNumberOfBands(ossim_uint32 nbands)
{
   std::ostringstream out;
   if(nbands > 9)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimNitfImageHeaderV2_0::setNumberOfBands: NBANDS is too large.  For 2.0 spec we can only have up to 9 bands\n";
      }
      std::string s = "ossimNitfImageHeaderV2_0::setNumberOfBands:";
      s += " ERROR\nExceeded max number of bands of 9!";
      throw std::out_of_range(s);
   }
   else
   {
      out << nbands;
      theNumberOfBands[0] = out.str().c_str()[0];
   }
   
   theImageBands.resize(getNumberOfBands());
}

void ossimNitfImageHeaderV2_0::setBandInfo(ossim_uint32 idx,
                                           const ossimNitfImageBandV2_0& info)
{
   if(idx < theImageBands.size())
   {
      if(!theImageBands[idx].valid())
      {
         theImageBands[idx] = new ossimNitfImageBandV2_0;
      }
      (*theImageBands[idx]) = info;
   }
}
void ossimNitfImageHeaderV2_0::setNumberOfRows(ossim_uint32 rows)
{
   std::ostringstream out;
   if(rows > 99999999) rows = 99999999;
   
   out << rows;
   ossimNitfCommon::setField(theSignificantRows, out.str(), 8, ios::right, '0');
}

void ossimNitfImageHeaderV2_0::setNumberOfCols(ossim_uint32 cols)
{
   std::ostringstream out;
   if(cols > 99999999) cols = 99999999;
   
   out << cols;
   ossimNitfCommon::setField(theSignificantCols, out.str(), 8, ios::right, '0');
}

void ossimNitfImageHeaderV2_0::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimNitfImageHeaderV2_X::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimNitfImageHeaderV2_0::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> property = 0;
   if(name == ISCODE_KW)
   {
      property = new ossimStringProperty(name, theCodewords);
   }
   else if(name == ISCTLH_KW)
   {
      property = new ossimStringProperty(name, theControlAndHandling);
   }
   else if(name == ISREL_KW)
   {
      property = new ossimStringProperty(name, theReleasingInstructions);
   }
   else if(name == ISCAUT_KW)
   {
      property = new ossimStringProperty(name, theClassificationAuthority);
   }
   else if(name == CTLN_KW)
   {
      property = new ossimStringProperty(name, theSecurityControlNumber);
   }
   else if(name == ISDWNG_KW)
   {
      property = new ossimStringProperty(name, theSecurityDowngrade);
   }
   else if(name == ISDEVT_KW)
   {
      property = new ossimStringProperty(name, theDowngradingEvent);
   }
   else 
   {
      property = ossimNitfImageHeaderV2_X::getProperty(name);
   }
   return property;
}

void ossimNitfImageHeaderV2_0::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfImageHeaderV2_X::getPropertyNames(propertyNames);
   propertyNames.push_back(ISCODE_KW);
   propertyNames.push_back(ISCTLH_KW);
   propertyNames.push_back(ISREL_KW);
   propertyNames.push_back(ISCAUT_KW);
   propertyNames.push_back(CTLN_KW);
   propertyNames.push_back(ISDWNG_KW);
   propertyNames.push_back(ISDEVT_KW);
}
