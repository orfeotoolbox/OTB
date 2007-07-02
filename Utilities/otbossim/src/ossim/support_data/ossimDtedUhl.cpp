//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the User Header Label
//               (UHL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedUhl.cpp 10261 2007-01-14 18:52:50Z dburken $

#include <iostream>
#include <fstream>

#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/base/ossimNotifyContext.h>


//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedUhl::ossimDtedUhl(const ossimFilename& dted_file, ossim_int32 offset)
   :
      theRecSen(),
      theField2(),
      theLonOrigin(),
      theLatOrigin(),
      theLonInterval(),
      theLatInterval(),
      theAbsoluteLE(),
      theSecurityCode(),
      theNumLonLines(),
      theNumLatPoints(),
      theMultipleAccuracy(),
      theStartOffset(offset),
      theStopOffset(0)
{
   // Check to see that dted file exists.
   if(!dted_file.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedUhl::ossimDtedUhl: The DTED file does not exist: " << dted_file << std::endl;
      return;
   }

   // Check to see that the dted file is readable.
   if(!dted_file.isReadable())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedUhl::ossimDtedUhl: The DTED file is not readable --> " << dted_file << std::endl;
      return;
   }

   std::ifstream in(dted_file.c_str());
   if(!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedUhl::ossimDtedUhl: Error opening the DTED file: " << dted_file << std::endl;
      
      return;
   }

   parse(in);

   in.close();
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedUhl::ossimDtedUhl(std::istream& in, ossim_int32 offset)
   :
      theRecSen(),
      theLonOrigin(),
      theLatOrigin(),
      theLonInterval(),
      theLatInterval(),
      theAbsoluteLE(),
      theSecurityCode(),
      theNumLonLines(),
      theNumLatPoints(),
      theMultipleAccuracy(),
      theStartOffset(offset),
      theStopOffset(0)
{
   parse(in);
}

//**************************************************************************
// ossimDtedUhl::parse()
//**************************************************************************
void ossimDtedUhl::parse(std::istream& in)
{
   // Seek to the start of the record.
   in.seekg(theStartOffset, std::ios::beg);
   
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "UHL", 3) == 0))
   {
      // Not a user header label.
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;  
      return;
   }  
   
   // Parse Field 2
   in.read(theField2, FIELD2_SIZE);
   theField2[FIELD2_SIZE] = '\0';
   
   // Parse theLonOrigin
   in.read(theLonOrigin, FIELD3_SIZE);
   theLonOrigin[FIELD3_SIZE] = '\0';
   
   // Parse theLatOrigin
   in.read(theLatOrigin, FIELD4_SIZE);
   theLatOrigin[FIELD4_SIZE] = '\0';
   
   // Parse theLonInterval
   in.read(theLonInterval, FIELD5_SIZE);
   theLonInterval[FIELD5_SIZE] = '\0';
   
   // Parse theLatInterval
   in.read(theLatInterval, FIELD6_SIZE);
   theLatInterval[FIELD6_SIZE] = '\0';
   
   // Parse theAbsoluteLE
   in.read(theAbsoluteLE, FIELD7_SIZE);
   theAbsoluteLE[FIELD7_SIZE] = '\0';
   
   // Parse theSecurityCode
   in.read(theSecurityCode, FIELD8_SIZE);
   theSecurityCode[FIELD8_SIZE] = '\0';
   
   // Parse Field 9
   in.read(theField9, FIELD9_SIZE);
   theField9[FIELD9_SIZE] = '\0';
   
   // Parse theNumLonLines
   in.read(theNumLonLines, FIELD10_SIZE);
   theNumLonLines[FIELD10_SIZE] = '\0';
   
   // Parse theNumLatPoints
   in.read(theNumLatPoints, FIELD11_SIZE);
   theNumLatPoints[FIELD11_SIZE] = '\0';
   
   // Parse theMultipleAccuracy
   in.read(theMultipleAccuracy, FIELD12_SIZE);
   theMultipleAccuracy[FIELD12_SIZE] = '\0';

   // Field 13 not parsed as it's unused.

   // Set the stop offset.
   theStopOffset = theStartOffset + UHL_LENGTH;
}

ossimString ossimDtedUhl::recoginitionSentinel() const
{
   return theRecSen;
}

double ossimDtedUhl::lonOrigin() const
{
   return degreesFromString(theLonOrigin);
}

double ossimDtedUhl::latOrigin() const
{
   return degreesFromString(theLatOrigin);
}

double ossimDtedUhl::lonInterval()      const
{
   return spacingFromString(theLonInterval);
}

double ossimDtedUhl::latInterval()      const
{
   return spacingFromString(theLatInterval);
}
  
double ossimDtedUhl::degreesFromString(const char* str) const
{
   // Parse the string:  DDDMMMSSH
   if (!str)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedUhl::degreesFromString: "
         << "Null pointer passed to method!" << std::endl;
      return 0.0;
   }
   
   if (strlen(str) < 8)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedUhl::decimalDegreesFromString:"
         << "String not big enough!" << std::endl;
      return 0.0;
   }

   double d = ((str[0]-'0')*100 + (str[1]-'0')*10 + (str[2]-'0') +
               (str[3]-'0')/6.0 + (str[4]-'0')/60.0 +
               (str[5]-'0')/360.0 + (str[6]-'0')/3600.0);
   
   if ( (str[7] == 'S') || (str[7] == 's') ||
        (str[7] == 'W') || (str[7] == 'w') )
   {
      d *= -1.0;
   }
   
   return d;
}
 
double ossimDtedUhl::spacingFromString(const char* str) const
{
   // Parse the string: SSSS (tenths of a second)
   if (!str)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimDtedUhl::decimalDegreesFromString: "
         << "Null pointer passed to method!" << std::endl;
      return 0.0;
   }

   return atof(str) / 36000.0;  // return 10ths of second as decimal degrees.
}

double ossimDtedUhl::absoluteLE() const
{
   return (theAbsoluteLE ? atoi(theAbsoluteLE) : 0);
}

ossimString ossimDtedUhl::securityCode() const
{
   return theSecurityCode;
}

ossim_int32 ossimDtedUhl::numLonLines() const
{
   return (theNumLonLines ? atoi(theNumLonLines) : 0);
}

ossim_int32 ossimDtedUhl::numLatPoints() const
{
   return (theNumLatPoints ? atoi(theNumLatPoints) : 0);
}

ossim_int32 ossimDtedUhl::mulitpleAccuracy() const
{
   return (theMultipleAccuracy ? atoi(theMultipleAccuracy) : 0);
}

ossim_int32 ossimDtedUhl::startOffset() const
{
   return theStartOffset;
}

ossim_int32 ossimDtedUhl::stopOffset() const
{
   return theStopOffset;
}

//**************************************************************************
// operator <<
//**************************************************************************
std::ostream& operator<<( std::ostream& os, const ossimDtedUhl& uhl)
{
   os << "\nDTED Header (UHL):"
      << "\n-------------------------------"
      << "\nRecoginition Sentinel: " << uhl.theRecSen
      << "\nLon Origin:            " << uhl.theLonOrigin
      << "\nLat Origin:            " << uhl.theLatOrigin
      << "\nLon Interval:          " << uhl.theLonInterval
      << "\nLat Interval:          " << uhl.theLatInterval
      << "\nAbsolute LE:           " << uhl.theAbsoluteLE
      << "\nSecurity Code:         " << uhl.theSecurityCode
      << "\nNumber of Lon Lines:   " << uhl.theNumLonLines
      << "\nNumber of Lat Lines:   " << uhl.theNumLatPoints
      << "\nMultiple Accuracy:     " << uhl.theMultipleAccuracy
      << "\nStart Offset:          " << uhl.theStartOffset
      << "\nStop Offset:           " << uhl.theStopOffset
      << std::endl;
   
   return os;
}

ossimDtedUhl::ossimDtedUhl(const ossimDtedUhl& source)
{
}

const ossimDtedUhl& ossimDtedUhl::operator=(const ossimDtedUhl& rhs)
{
   return rhs;
}

