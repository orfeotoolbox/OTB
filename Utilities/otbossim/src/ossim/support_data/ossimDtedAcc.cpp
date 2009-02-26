//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Accuracy Description
//               (ACC) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedAcc.cpp 13571 2008-09-12 14:59:59Z gpotts $

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <ossim/support_data/ossimDtedAcc.h>
#include <ossim/base/ossimNotifyContext.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedAcc::ossimDtedAcc(const ossimFilename& dted_file,
                           ossim_int32 offset)
{
	clearFields();
	theStartOffset = offset;
   // Check to see that dted file exists. 
   if(!dted_file.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimDtedAcc::ossimDtedAcc: The DTED file does not exist: " << dted_file << std::endl;
      return;
   }

   // Check to see that the dted file is readable.
   if(!dted_file.isReadable())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimDtedAcc::ossimDtedAcc: The DTED file is not readable: " << dted_file << std::endl;
      return;
   }
   
   // Open the dted file for reading.
   std::ifstream in(dted_file.c_str());
   if(!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimDtedAcc::ossimDtedAcc: Can't open the DTED file: " << dted_file << std::endl;
      return;
   }
   
   parse(in);
}

void ossimDtedAcc::clearFields()
{
   memset(theRecSen, '\0', FIELD1_SIZE+1);
   memset(theAbsoluteCE, '\0',FIELD2_SIZE+1);
   memset(theAbsoluteLE, '\0',FIELD3_SIZE+1);
   memset(theRelativeCE, '\0',FIELD4_SIZE+1);
   memset(theRelativeLE, '\0',FIELD5_SIZE+1);
   memset(theField6, '\0',FIELD6_SIZE+1);
   memset(theField7, '\0',FIELD7_SIZE+1);
   memset(theField8, '\0',FIELD8_SIZE+1);
   memset(theField9, '\0',FIELD9_SIZE+1);
   memset(theField10, '\0',FIELD10_SIZE+1);
   memset(theField11, '\0',FIELD11_SIZE+1);
   memset(theField12, '\0',FIELD12_SIZE+1);
   memset(theField13, '\0',FIELD13_SIZE+1);
   memset(theField14, '\0',FIELD14_SIZE+1);
   memset(theField15, '\0',FIELD15_SIZE+1);
   memset(theField16, '\0',FIELD16_SIZE+1);
   memset(theField17, '\0',FIELD17_SIZE+1);
   
   theStartOffset = 0;
   theStopOffset = 0;
}
//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedAcc::ossimDtedAcc(std::istream& in,
                           ossim_int32 offset)
{
	clearFields();
	theStartOffset = offset;
   parse(in);
}

//**************************************************************************
// ossimDtedAcc::parse()
//**************************************************************************
void ossimDtedAcc::parse(std::istream& in)
{
   // Seek to the start of the record.
   in.seekg(theStartOffset, std::ios::beg);
   
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "ACC", 3) == 0))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return;
   }
   
   // Parse theAbsoluteCE
   in.read(theAbsoluteCE, FIELD2_SIZE);
   theAbsoluteCE[FIELD2_SIZE] = '\0';
   
   // Parse theAbsoluteLE
   in.read(theAbsoluteLE, FIELD3_SIZE);
   theAbsoluteLE[FIELD3_SIZE] = '\0';
   
   // Parse theRelativeCE
   in.read(theRelativeCE, FIELD4_SIZE);
   theRelativeCE[FIELD4_SIZE] = '\0';
   
   // Parse theRelativeLE
   in.read(theRelativeLE, FIELD5_SIZE);
   theRelativeLE[FIELD5_SIZE] = '\0';
   
   // Parse Field 6
   in.read(theField6, FIELD6_SIZE);
   theField6[FIELD6_SIZE] = '\0';
   
   // Parse Field 7
   in.read(theField7, FIELD7_SIZE);
   theField7[FIELD7_SIZE] = '\0';
   
   // Parse Field 8
   in.read(theField8, FIELD8_SIZE);
   theField8[FIELD8_SIZE] = '\0';
   
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
   
   // Parse Field 15
   in.read(theField15, FIELD15_SIZE);
   theField15[FIELD15_SIZE] = '\0';
   
   // Parse Field 16
   in.read(theField16, FIELD16_SIZE);
   theField16[FIELD16_SIZE] = '\0';
   
   // Parse Field 17
   in.read(theField17, FIELD17_SIZE);
   theField17[FIELD17_SIZE] = '\0';

   // Set the stop offset.
   theStopOffset = theStartOffset + ACC_LENGTH;
}

ossim_int32 ossimDtedAcc::absCE() const
{
   return (theAbsoluteCE ? atoi(theAbsoluteCE) : 0);
}

ossim_int32 ossimDtedAcc::absLE() const
{
   return (theAbsoluteLE ? atoi(theAbsoluteLE) : 0);
}

ossim_int32 ossimDtedAcc::relCE() const
{
   return (theRelativeCE ? atoi(theRelativeCE) : 0);
}

ossim_int32 ossimDtedAcc::relLE() const
{
   return (theRelativeLE ? atoi(theRelativeLE) : 0);
}
   
ossim_int32 ossimDtedAcc::startOffset() const
{
   return theStartOffset;
}

ossim_int32 ossimDtedAcc::stopOffset()  const
{
   return theStopOffset;
}



//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedAcc& acc)
{
   os << "\nDTED Header (ACC):"
      << "\n-------------------------------"
      << "\nRecoginition Sentinel: " << acc.theRecSen
      << "\nAbsolute CE:           " << acc.theAbsoluteCE
      << "\nAbsolute LE:           " << acc.theAbsoluteLE
      << "\nRelative CE:           " << acc.theRelativeCE
      << "\nRelative LE:           " << acc.theRelativeLE
      << "\nStart Offset:          " << acc.theStartOffset
      << "\nStop Offset:           " << acc.theStopOffset
      << std::endl;

   return os;
}

ossimDtedAcc::ossimDtedAcc(const ossimDtedAcc& source)
{}

const ossimDtedAcc& ossimDtedAcc::operator=(const ossimDtedAcc& rhs)
{
   return rhs;
}
