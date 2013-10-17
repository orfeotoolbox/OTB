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
// $Id: ossimNitfGeoPositioningTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <ossim/support_data/ossimNitfGeoPositioningTag.h>
#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

RTTI_DEF1(ossimNitfGeoPositioningTag, "ossimNitfGeoPositioningTag", ossimNitfRegisteredTag);

ossimNitfGeoPositioningTag::ossimNitfGeoPositioningTag()
   : ossimNitfRegisteredTag(std::string("GEOPSB"),  443)
{
   clearFields();
}

ossimNitfGeoPositioningTag::~ossimNitfGeoPositioningTag()
{
}

void ossimNitfGeoPositioningTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theType, 3);
   in.read(theCoordinateUnits, 3);
   in.read(theGeodeticDatumName, 80);
   in.read(theGeodeticDatumCode, 4);
   in.read(theEllipsoidName, 80);
   in.read(theEllipsoidCode, 3);
   in.read(theVerticalDatumReference, 80);
   in.read(theVerticalReferenceCode, 4);
   in.read(theSoundingDatumName, 80);
   in.read(theSoundingDatumCode, 4);
   in.read(theZFalseOrigin, 15);
   in.read(theGridCode, 3);
   in.read(theGridDescription, 80);
   in.read(theGridZoneNumber, 4);
}

void ossimNitfGeoPositioningTag::writeStream(std::ostream& out)
{
   out.write(theType, 3);
   out.write(theCoordinateUnits, 3);
   out.write(theGeodeticDatumName, 80);
   out.write(theGeodeticDatumCode, 4);
   out.write(theEllipsoidName, 80);
   out.write(theEllipsoidCode, 3);
   out.write(theVerticalDatumReference, 80);
   out.write(theVerticalReferenceCode, 4);
   out.write(theSoundingDatumName, 80);
   out.write(theSoundingDatumCode, 4);
   out.write(theZFalseOrigin, 15);
   out.write(theGridCode, 3);
   out.write(theGridDescription, 80);
   out.write(theGridZoneNumber, 4);
}

void ossimNitfGeoPositioningTag::clearFields()
{
   memset(theType, ' ', 3);
   memset(theCoordinateUnits, ' ', 3);
   memset(theGeodeticDatumName, ' ', 80);
   memset(theGeodeticDatumCode, ' ', 4);
   memset(theEllipsoidName, ' ', 80);
   memset(theEllipsoidCode, ' ', 3);
   memset(theVerticalDatumReference, ' ', 80);
   memset(theVerticalReferenceCode, ' ', 4);
   memset(theSoundingDatumName, ' ', 80);
   memset(theSoundingDatumCode, ' ', 4);
   memset(theZFalseOrigin, '0', 15);
   memset(theGridCode, ' ', 3);
   memset(theGridDescription, ' ', 80);
   memset(theGridZoneNumber, '0',  4);
   
   memcpy(theType, "MAP", 3);
   memcpy(theCoordinateUnits, "M  ", 3);
   memcpy(theGeodeticDatumName, "World Geodetic System 1984", 26);
   memcpy(theGeodeticDatumCode, "WGE", 3);
   memcpy(theEllipsoidName, "World Geodetic System 1984", 26);
   memcpy(theEllipsoidCode, "WE", 2);
   memcpy(theVerticalDatumReference, "Geodetic", 8);
   memcpy(theVerticalReferenceCode, "GEOD", 4);
   memcpy(theSoundingDatumName, "Mean Sea", 8);
   memcpy(theSoundingDatumCode, "MSL", 3);

   theType[3]                    = '\0';
   theCoordinateUnits[3]         = '\0';
   theGeodeticDatumName[80]      = '\0';
   theGeodeticDatumCode[4]       = '\0';
   theEllipsoidName[80]          = '\0';
   theEllipsoidCode[3]           = '\0';
   theVerticalDatumReference[80] = '\0';
   theVerticalReferenceCode[4]   = '\0';
   theSoundingDatumName[80]      = '\0';
   theSoundingDatumCode[4]       = '\0';
   theZFalseOrigin[15]           = '\0';
   theGridCode[3]                = '\0';
   theGridDescription[80]        = '\0';
   theGridZoneNumber[4]          = '\0';
   
}

std::ostream& ossimNitfGeoPositioningTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getTagLength() << "\n"
       << pfx << std::setw(24) << "TYP:" << theType << "\n"
       << pfx << std::setw(24) << "UNI:" << theCoordinateUnits << "\n"
       << pfx << std::setw(24) << "DAG:" << theGeodeticDatumName << "\n"
       << pfx << std::setw(24) << "DCD:" << theGeodeticDatumCode << "\n"
       << pfx << std::setw(24) << "ELL:" << theEllipsoidName << "\n"
       << pfx << std::setw(24) << "ELC:" << theEllipsoidCode << "\n"
       << pfx << std::setw(24) << "DVR:" << theVerticalDatumReference << "\n"
       << pfx << std::setw(24) << "VDCDVR:" << theVerticalReferenceCode << "\n"
       << pfx << std::setw(24) << "SDA:" << theSoundingDatumName << "\n"
       << pfx << std::setw(24) << "VDCSDA:" << theSoundingDatumCode << "\n"
       << pfx << std::setw(24) << "ZOR:" << theZFalseOrigin << "\n"
       << pfx << std::setw(24) << "GRD:" << theGridCode << "\n"
       << pfx << std::setw(24) << "GRN:" << theGridDescription << "\n"
       << pfx << std::setw(24) << "ZNA:" << theGridZoneNumber << "\n";
   
   return out;
}
