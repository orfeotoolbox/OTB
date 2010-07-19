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
// $Id: ossimDtedVol.cpp 17501 2010-06-02 11:14:55Z dburken $

#include <iostream>
#include <fstream>
#include <string>

#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimProperty.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedVol::ossimDtedVol(const ossimFilename& dted_file,
                           ossim_int32 offset)
   :
      theStartOffset(0),
      theStopOffset(0)
{
   if(!dted_file.empty())
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
      in.seekg(offset);
      parse(in);
      
      in.close();
   }
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedVol::ossimDtedVol(std::istream& in)
   :
      theStartOffset(0),
      theStopOffset(0)
{
   parse(in);
}

void ossimDtedVol::parse(std::istream& in)
{
   clearErrorStatus();
   theStartOffset = in.tellg();
   theStopOffset  = theStartOffset;
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';
   if(!(strncmp(theRecSen, "VOL", 3) == 0))
   {
      // Not a volume header label.
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      in.seekg(theStartOffset);
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

ossimRefPtr<ossimProperty> ossimDtedVol::getProperty(
   const ossimString& /* name */) const
{
   ossimRefPtr<ossimProperty> result = 0;
   return result;
}

void ossimDtedVol::getPropertyNames(
   std::vector<ossimString>& propertyNames) const
{
   propertyNames.push_back(ossimString("dted_vol_record"));
}

ossimString ossimDtedVol::getRecognitionSentinel() const
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

std::ostream& ossimDtedVol::print(std::ostream& out,
                                  const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += "vol.";
   
   out << pfx << "recognition_sentinel: " << theRecSen << "\n"
       << pfx << "field2:                " << theField2 << "\n"
       << pfx << "reel_number:           " << theReelNumber << "\n"
       << pfx << "field4:                " << theField4 << "\n"
       << pfx << "field5:                " << theField5 << "\n"
       << pfx << "account_number:        " << theAccountNumber << "\n"
       << pfx << "field7:                " << theField7 << "\n"
       << pfx << "field8:                " << theField8 << "\n"
       << std::endl;
   return out;
}

//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& out, const ossimDtedVol& vol)
{
   std::string prefix;
   return vol.print(out, prefix);
}

ossimDtedVol::ossimDtedVol(const ossimDtedVol& /* source */)
{
}

const ossimDtedVol& ossimDtedVol::operator=(const ossimDtedVol& rhs)
{
   return rhs;
}

