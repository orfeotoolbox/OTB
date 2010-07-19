//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the User Header Label
//               (UHL) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedUhl.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimProperty.h>


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
      theStartOffset(0),
      theStopOffset(0)
{
   if(!dted_file.empty())
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
      in.seekg(offset);
      parse(in);
      
      in.close();
   }
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedUhl::ossimDtedUhl(std::istream& in)
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
      theStartOffset(0),
      theStopOffset(0)
{
   parse(in);
}

//**************************************************************************
// ossimDtedUhl::parse()
//**************************************************************************
void ossimDtedUhl::parse(std::istream& in)
{
   clearErrorStatus();
   theStartOffset = in.tellg();
   theStopOffset  = theStartOffset;
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';

   if(!(strncmp(theRecSen, "UHL", 3) == 0))
   {
      // Not a user header label.
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;  
      in.seekg(theStartOffset);
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
   in.ignore(FIELD13_SIZE);
   // Set the stop offset.
   theStopOffset = theStartOffset + UHL_LENGTH;
}

ossimRefPtr<ossimProperty> ossimDtedUhl::getProperty(
   const ossimString& /* name */) const
{
   ossimRefPtr<ossimProperty> result = 0;
   return result;
}

void ossimDtedUhl::getPropertyNames(
   std::vector<ossimString>& propertyNames) const
{
   propertyNames.push_back(ossimString("dted_uhl_record"));
}

ossimString ossimDtedUhl::recognitionSentinel() const
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
std::ostream& operator<<( std::ostream& out, const ossimDtedUhl& uhl)
{
   std::string prefix;
   return uhl.print(out, prefix);
}

std::ostream& ossimDtedUhl::print(std::ostream& out,
                                  const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += "uhl.";
   
   out << setiosflags(ios::left)
       << pfx << setw(28) << "recognition_sentinel:" << theRecSen << "\n"
       << pfx << setw(28) << "lon_origin:" << theLonOrigin << "\n"
       << pfx << setw(28) << "lat_origin:" << theLatOrigin << "\n"
       << pfx << setw(28) << "lon_interval:" << theLonInterval << "\n"
       << pfx << setw(28) << "lat_interval:" << theLatInterval << "\n"
       << pfx << setw(28) << "absolute_le:" << theAbsoluteLE << "\n"
       << pfx << setw(28) << "security_code:" << theSecurityCode << "\n"
       << pfx << setw(28) << "number_of_lat_points:" << theNumLatPoints << "\n"
       << pfx << setw(28) << "number_of_lon_lines:" << theNumLonLines << "\n"
       << pfx << setw(28) << "multiple_accuracy:"
       << theMultipleAccuracy << "\n"
       << pfx << setw(28) << "start_offset:" << theStartOffset << "\n"
       << pfx << setw(28) << "stop_offset:" << theStopOffset
       << std::endl;
   
   return out;
}

ossimDtedUhl::ossimDtedUhl(const ossimDtedUhl& /* source */)
{
}

const ossimDtedUhl& ossimDtedUhl::operator=(const ossimDtedUhl& rhs)
{
   return rhs;
}

