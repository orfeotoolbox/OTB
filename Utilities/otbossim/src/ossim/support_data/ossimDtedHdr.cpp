//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Ken Melero
// 
// Description:  This class gives access to the File Header Label
//               (HDR) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedHdr.cpp 17501 2010-06-02 11:14:55Z dburken $

#include <iostream>
#include <fstream>
#include <string>

#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimProperty.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedHdr::ossimDtedHdr(const ossimFilename& dted_file, ossim_int32 offset)
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
      in.seekg(offset);
      // Continue parsing all the record fields.
      parse(in);
      
      in.close();
   }
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedHdr::ossimDtedHdr(std::istream& in)
   :
      theStartOffset(0),
      theStopOffset(0)
{
   parse(in);
}

//**************************************************************************
// ossimDtedHdr::parse()
//**************************************************************************
void ossimDtedHdr::parse(std::istream& in)
{
   clearErrorStatus();
   theStartOffset = in.tellg();
   theStopOffset  = theStartOffset;
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "HDR", 3) == 0))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      in.seekg(theStartOffset);
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

ossimRefPtr<ossimProperty> ossimDtedHdr::getProperty(
   const ossimString& /* name */) const
{
   ossimRefPtr<ossimProperty> result = 0;
   return result;
}

void ossimDtedHdr::getPropertyNames(
   std::vector<ossimString>& propertyNames) const
{
   propertyNames.push_back(ossimString("dted_hdr_record"));
}


//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedHdr& hdr)
{
   std::string prefix;
   return hdr.print(os, prefix);
}

std::ostream& ossimDtedHdr::print(std::ostream& out,
                                  const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += "hdr.";
   
   out << pfx << "recognition_sentinel:  " << theRecSen << "\n"
       << pfx << "field2:                " << theField2 << "\n"
       << pfx << "filename:              " << theFilename << "\n"
       << pfx << "field4:                " << theField4 << "\n"
       << pfx << "field5:                " << theField5 << "\n"
       << pfx << "field6:                " << theField6 << "\n"
       << pfx << "version:               " << theVersion << "\n"
       << pfx << "creation_date:         " << theCreationDate << "\n"
       << pfx << "field9:                " << theField9 << "\n"
       << pfx << "field10:               " << theField10 << "\n"
       << pfx << "field11:               " << theField11 << "\n"
       << pfx << "field12:               " << theField12 << "\n"
       << pfx << "field13:               " << theField13 << "\n"
       << pfx << "field14:               " << theField14 << "\n"
       << std::endl;
   
   return out;
}

ossimString ossimDtedHdr::recognitionSentinel() const
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

ossimDtedHdr::ossimDtedHdr(const ossimDtedHdr& /* source */)
{}

const ossimDtedHdr& ossimDtedHdr::operator=(const ossimDtedHdr& rhs)
{
   return rhs;
}
