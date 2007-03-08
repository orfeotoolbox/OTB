//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero
// 
// Description:  This class gives access to the File Header Label
//               (HDR) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedHdr.cpp 10261 2007-01-14 18:52:50Z dburken $

#include <iostream>
#include <fstream>

#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/base/ossimNotifyContext.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedHdr::ossimDtedHdr(const ossimFilename& dted_file, ossim_int32 offset)
   :
      theRecSen(),
      theField2(),
      theFilename(),
      theField4(),
      theField5(),
      theField6(),
      theVersion(),
      theCreationDate(),
      theField9(),
      theField10(),
      theField11(),
      theField12(),
      theField13(),
      theField14(),
      theStartOffset(offset),
      theStopOffset(0)
{
   // Check to see that dted file exists.
   if(!dted_file.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedHdr::ossimDtedHdr: The DTED file does not exist: " << dted_file << std::endl;
      return;
   }

   // Check to see that the dted file is readable.
   if(!dted_file.isReadable())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedHdr::ossimDtedHdr: The DTED file is not readable: " << dted_file << std::endl;
      return;
   }

   std::ifstream in(dted_file.c_str());
   // Open the dted file for reading.
   if(!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedHdr::ossimDtedHdr: Error opening the DTED file: " << dted_file << std::endl;
      return;
   }

   // Continue parsing all the record fields.
   parse(in);

   in.close();
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedHdr::ossimDtedHdr(std::istream& in, ossim_int32 offset)
   :
      theRecSen(),
      theField2(),
      theFilename(),
      theField4(),
      theField5(),
      theField6(),
      theVersion(),
      theCreationDate(),
      theField9(),
      theField10(),
      theField11(),
      theField12(),
      theField13(),
      theField14(),
      theStartOffset(offset),
      theStopOffset(0)
{
   parse(in);
}

//**************************************************************************
// ossimDtedHdr::parse()
//**************************************************************************
void ossimDtedHdr::parse(std::istream& in)
{
   // Seek to the start of the record.
   in.seekg(theStartOffset, std::ios::beg);
   
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "HDR", 3) == 0))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return;
   }
   
   // Parse Field 2
   in.read(theField2, FIELD2_SIZE);
   theField2[FIELD2_SIZE] = '\0';
   
   // Parse theFilename
   in.read(theFilename, FIELD3_SIZE);
   theFilename[FIELD3_SIZE] = '\0';
   
   // Parse Field 4
   in.read(theField4, FIELD4_SIZE);
   theField4[FIELD4_SIZE] = '\0';
   
   // Parse Field 5
   in.read(theField5, FIELD5_SIZE);
   theField5[FIELD5_SIZE] = '\0';
   
   // Parse Field 6
   in.read(theField6, FIELD6_SIZE);
   theField6[FIELD6_SIZE] = '\0';
   
   // Parse Field 7
   in.read(theVersion, FIELD7_SIZE);
   theVersion[FIELD7_SIZE] = '\0';
   
   // Parse theCreationDate
   in.read(theCreationDate, FIELD8_SIZE);
   theCreationDate[FIELD8_SIZE] = '\0';
   
   // Parse Field 9
   in.read(theField9, FIELD9_SIZE);
   theField9[FIELD9_SIZE] = '\0';   
   
   // Parse Field 10
   in.read(theField10, FIELD10_SIZE);
   theField10[FIELD10_SIZE] = '\0';   
   
   // Parse Field 11
   in.read(theField11, FIELD11_SIZE);
   theField11[FIELD11_SIZE] = '\0';   
   
   // Parse Field 12
   in.read(theField12, FIELD12_SIZE);
   theField12[FIELD12_SIZE] = '\0';   
   
   // Parse Field 13
   in.read(theField13, FIELD13_SIZE);
   theField13[FIELD13_SIZE] = '\0';   
   
   // Parse Field 14
   in.read(theField14, FIELD14_SIZE);
   theField14[FIELD14_SIZE] = '\0';

   // Set the stop offset.
   theStopOffset = theStartOffset + HDR_LENGTH;
}

//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedHdr& hdr)
{
   os << "\nDTED Header (VOL):"
      << "\n-------------------------------"
      << "\ntheRecSen:             " << hdr.theRecSen
      << "\nField 2:               " << hdr.theField2
      << "\ntheFilename:           " << hdr.theFilename
      << "\nField 4:               " << hdr.theField4
      << "\nField 5:               " << hdr.theField5
      << "\nField 6:               " << hdr.theField6
      << "\nVersion:               " << hdr.theVersion
      << "\ntheCreationDate:       " << hdr.theCreationDate
      << "\nField 9:               " << hdr.theField9
      << "\nField 10:              " << hdr.theField10
      << "\nField 11:              " << hdr.theField11
      << "\nField 12:              " << hdr.theField12
      << "\nField 13:              " << hdr.theField13
      << "\nField 14:              " << hdr.theField14
      << std::endl;
   
   return os;
}

ossimString ossimDtedHdr::recoginitionSentinel() const
{
   return theRecSen;
}

ossimString ossimDtedHdr::fileName() const
{
   return theFilename;
}
ossimString ossimDtedHdr::version() const
{ return theVersion;
}

ossimString ossimDtedHdr::creationDate() const
{
   return theCreationDate;
}

ossim_int32 ossimDtedHdr::startOffset() const
{
   return theStartOffset;
}

ossim_int32 ossimDtedHdr::stopOffset() const
{
   return theStopOffset;
}

ossimDtedHdr::ossimDtedHdr(const ossimDtedHdr& source)
{}

const ossimDtedHdr& ossimDtedHdr::operator=(const ossimDtedHdr& rhs)
{
   return rhs;
}
