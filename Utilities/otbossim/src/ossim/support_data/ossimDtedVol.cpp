//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Volume Header Label
//               (VOL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedVol.cpp 10261 2007-01-14 18:52:50Z dburken $

#include <iostream>
#include <fstream>

#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/base/ossimNotifyContext.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedVol::ossimDtedVol(const ossimFilename& dted_file,
                           ossim_int32 offset)
   :
      theRecSen(),
      theField2(),
      theReelNumber(),
      theField4(),
      theField5(),
      theAccountNumber(),
      theField7(),
      theField8(),
      theStartOffset(offset),
      theStopOffset(0)
{
   // Check to see that dted file exists.
   if(!dted_file.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedVol::ossimDtedVol"
         << "\nThe DTED file does not exist: " << dted_file << std::endl;
      return;
   }

   // Check to see that the dted file is readable.
   if(!dted_file.isReadable())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedVol::ossimDtedVol"
         << "\nThe DTED file is not readable: " << dted_file << std::endl;
      return;
   }
   
   // Open the dted file for reading.
   std::ifstream in(dted_file.c_str());
   if(!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedVol::ossimDtedVol"
         << "\nUnable to open the DTED file: " << dted_file << std::endl;
      return;
   }

   parse(in);

   in.close();
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedVol::ossimDtedVol(std::istream& in,
                           ossim_int32 offset)
   :
      theRecSen(),
      theReelNumber(),
      theAccountNumber(),
      theStartOffset(offset),
      theStopOffset(0)
{
   parse(in);
}

void ossimDtedVol::parse(std::istream& in)
{
   // Seek to the start of the record.
   in.seekg(theStartOffset, std::ios::beg);
   
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "VOL", 3) == 0))
   {
      // Not a volume header label.
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return;
   }   

   // Parse Field 2
   in.read(theField2, FIELD2_SIZE);
   theField2[FIELD2_SIZE] = '\0';
   
   // Parse theReelNumber
   in.read(theReelNumber, FIELD3_SIZE);
   theReelNumber[FIELD3_SIZE] = '\0';
   
   // Parse Field 4
   in.read(theField4, FIELD4_SIZE);
   theField4[FIELD4_SIZE] = '\0';
   
   // Parse Field 5
   in.read(theField5, FIELD5_SIZE);
   theField5[FIELD5_SIZE] = '\0';
   
   // Parse theAccountNumber
   in.read(theAccountNumber, FIELD6_SIZE);
   theAccountNumber[FIELD6_SIZE] = '\0';
   
   // Parse Field 7
   in.read(theField7, FIELD7_SIZE);
   theField7[FIELD7_SIZE] = '\0';

   // Parse Field 8
   in.read(theField8, FIELD8_SIZE);
   theField8[FIELD8_SIZE] = '\0';

   // Set the stop offset.
   theStopOffset = theStartOffset + VOL_LENGTH;
}

ossimString ossimDtedVol::getRecoginitionSentinel() const
{
   return ossimString(theRecSen);
}

ossimString ossimDtedVol::getReelNumber() const
{
   return ossimString(theReelNumber);
}

ossimString ossimDtedVol::getAccountNumber() const
{
   return ossimString(theAccountNumber);
}

ossim_int32 ossimDtedVol::startOffset() const
{
   return theStartOffset;
}

ossim_int32 ossimDtedVol::stopOffset() const
{
   return theStopOffset;
}

//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedVol& vol)
{
   os << "\nDTED Header (VOL):"
      << "\n-------------------------------"
      << "\nRecoginition Sentinel: " << vol.theRecSen
      << "\nField 2:               " << vol.theField2
      << "\ntheReelNumber:         " << vol.theReelNumber
      << "\nField 4:               " << vol.theField4
      << "\nField 5:               " << vol.theField5
      << "\ntheAccountNumber:      " << vol.theAccountNumber
      << "\nField 7:               " << vol.theField7
      << "\nField 8:               " << vol.theField8
      << std::endl;
   
   return os;
}

ossimDtedVol::ossimDtedVol(const ossimDtedVol& source)
{
}

const ossimDtedVol& ossimDtedVol::operator=(const ossimDtedVol& rhs)
{
   return rhs;
}

