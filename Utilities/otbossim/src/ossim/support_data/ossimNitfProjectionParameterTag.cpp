//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfProjectionParameterTag.cpp 22013 2012-12-19 17:37:20Z dburken $
#include <ossim/support_data/ossimNitfProjectionParameterTag.h>
#include <sstream>
#include <iomanip>

ossimNitfProjectionParameterTag::ossimNitfProjectionParameterTag()
   :ossimNitfRegisteredTag(std::string("PRJPSB"), 0 )
{
   clearFields();
}

ossimNitfProjectionParameterTag::~ossimNitfProjectionParameterTag()
{
}

void ossimNitfProjectionParameterTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theProjectionName, 80);
   in.read(theProjectionCode, 2);
   in.read(theNumberOfParameters, 1);

   ossim_uint32 numberOfParameters = ossimString(theNumberOfParameters).toUInt32();

   for(ossim_uint32 i = 0; i < numberOfParameters; ++i)
   {
      char param[15];

      in.read(param, 15);
      theProjectionParameters.push_back(ossimString(param,
                                                    param + 15));
   }
   in.read(theFalseXOrigin, 15);
   in.read(theFalseYOrigin, 15);

   // Set the base tag length.
   setTagLength( getSizeInBytes() );
}

void ossimNitfProjectionParameterTag::writeStream(std::ostream& out)
{
   out.write(theProjectionName, 80);
   out.write(theProjectionCode, 2);

   {
      std::ostringstream tempOut;

      tempOut << std::setw(1)
              << theProjectionParameters.size();
      theNumberOfParameters[0] = *(tempOut.str().c_str());
   }
   out.write(theNumberOfParameters, 1);
   ossim_uint32 numberOfParameters = ossimString(theNumberOfParameters).toUInt32();

   for(ossim_uint32 i = 0; i < numberOfParameters; ++i)
   {
      out.write(theProjectionParameters[i].c_str(), 15);
   }
   out.write(theFalseXOrigin, 15);
   out.write(theFalseYOrigin, 15);
}

ossim_uint32 ossimNitfProjectionParameterTag::getSizeInBytes()const
{
   return (113 + (ossim_uint32)theProjectionParameters.size()*15);
}

std::ostream& ossimNitfProjectionParameterTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";

   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:" << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getTagLength() << "\n"
       << pfx << std::setw(24) << "PRN:"   << theProjectionName << "\n"
       << pfx << std::setw(24) << "PCO:"   << theProjectionCode << "\n"
       << pfx << std::setw(24) << "NUM_PRJ:" << theNumberOfParameters << "\n";

   for (ossim_uint32 i = 0; i < theProjectionParameters.size(); ++i)
   {
      ossimString s = "PRJ";
      s += ossimString::toString(i);
      s += ":";
      out << pfx << std::setw(24) << s
          << theProjectionParameters[i] << "\n";
   }

   out << pfx << std::setw(24) << "XOR:"   <<theFalseXOrigin << "\n"
       << pfx << std::setw(24) << "YOR:"   <<theFalseYOrigin << std::endl;
   
   return out;   
}
    
void ossimNitfProjectionParameterTag::clearFields()
{
   theProjectionParameters.clear();
   
   memset(theProjectionName, ' ', 80);
   memset(theProjectionCode, ' ', 2);
   memset(theNumberOfParameters, '0', 1);
   memset(theFalseXOrigin, '0', 15);
   memset(theFalseYOrigin, '0', 15);

   theProjectionName[80] = '\0';
   theProjectionCode[2] = '\0';
   theNumberOfParameters[1] = '\0';
   theFalseXOrigin[15] = '\0';
   theFalseYOrigin[15] = '\0';

   // Set the base tag length.
   setTagLength( 0 );
}

void ossimNitfProjectionParameterTag::clearParameters()
{
   theProjectionParameters.clear();
   theNumberOfParameters[0] = '0';
}

void ossimNitfProjectionParameterTag::addParameter(const ossimString& param)
{
   std::ostringstream out;

   out << std::setw(15)
       << std::setfill('0')
       << param.c_str();

   theProjectionParameters.push_back(out.str());
}

void ossimNitfProjectionParameterTag::addParameter(const ossim_float64& param)
{
   addParameter(ossimString::toString(param));
}

void ossimNitfProjectionParameterTag::setName(const ossimString& name)
{
   std::ostringstream out;

   out << std::setw(80)
       << std::setfill(' ')
       << name.c_str();

   memcpy(theProjectionName, out.str().c_str(), 80);
}

void ossimNitfProjectionParameterTag::setCode(const ossimString& code)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << code.c_str();

   memcpy(theProjectionCode, out.str().c_str(), 2);
}

ossimString ossimNitfProjectionParameterTag::getName()const
{
   return ossimString(theProjectionName).trim();
}

ossimString ossimNitfProjectionParameterTag::getCode()const
{
   return ossimString(theProjectionCode).trim();
}

void ossimNitfProjectionParameterTag::setFalseX(double falseX)
{
   std::ostringstream out;
   out << std::setw(15)
       << std::setprecision(15)
       << std::setfill('0')
       << falseX;
   
   memcpy(theFalseXOrigin, out.str().c_str(), 15);
}

void ossimNitfProjectionParameterTag::setFalseY(double falseY)
{
   std::ostringstream out;
   out << std::setw(15)
       << std::setprecision(15)
       << std::setfill('0')
       << falseY;
   
   memcpy(theFalseYOrigin, out.str().c_str(), 15);
}

double ossimNitfProjectionParameterTag::getFalseX()const
{
   return ossimString(theFalseXOrigin).toDouble();
}

double ossimNitfProjectionParameterTag::getFalseY()const
{
   return ossimString(theFalseYOrigin).toDouble();
}
