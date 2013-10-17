//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfLocalGeographicTag.cpp 22013 2012-12-19 17:37:20Z dburken $
#include <ossim/support_data/ossimNitfLocalGeographicTag.h>
#include <iomanip>
#include <sstream>

RTTI_DEF1(ossimNitfLocalGeographicTag, "ossimNitfLocalGeographicTag", ossimNitfRegisteredTag);

ossimNitfLocalGeographicTag::ossimNitfLocalGeographicTag()
   : ossimNitfRegisteredTag(std::string("GEOLOB"), 48)
{
   clearFields();
}

ossimNitfLocalGeographicTag::~ossimNitfLocalGeographicTag()
{
}

void ossimNitfLocalGeographicTag::parseStream(std::istream& in)
{
   in.read(theLonDensity, 9);
   in.read(theLatDensity, 9);
   in.read(theLonOrigin, 15);
   in.read(theLatOrigin, 15);
}

void ossimNitfLocalGeographicTag::writeStream(std::ostream& out)
{
   out.write(theLonDensity, 9);
   out.write(theLatDensity, 9);
   out.write(theLonOrigin, 15);
   out.write(theLatOrigin, 15);
}

void ossimNitfLocalGeographicTag::clearFields()
{
   memset(theLonDensity, ' ', 9);
   memset(theLatDensity, ' ', 9);
   memset(theLonOrigin, '0', 15);
   memset(theLatOrigin, '0', 15);

   
   
   theLonDensity[9] = '\0';
   theLatDensity[9] = '\0';
   theLonOrigin[15] = '\0';
   theLatOrigin[14] = '\0';
}

void ossimNitfLocalGeographicTag::setDegreesPerPixelLat(double deltaLat)
{
   ossim_uint32 pixels = 0;
   if(deltaLat > 0.0)
   {
      pixels = (ossim_uint32)((1.0/deltaLat)*360.0 + .5);
   }
   std::ostringstream out;

   out << std::setw(9)
       << std::setfill('0')
       << pixels;

   memcpy(theLatDensity, out.str().c_str(), 9);
}

void ossimNitfLocalGeographicTag::setDegreesPerPixelLon(double deltaLon)
{
   ossim_uint32 pixels = 0;
   if(deltaLon > 0.0)
   {
      pixels = (ossim_uint32)((1.0/deltaLon)*360.0 + .5);
   }
   std::ostringstream out;

   out << std::setw(9)
       << std::setfill('0')
       << pixels;

   memcpy(theLonDensity, out.str().c_str(), 9);
}

void ossimNitfLocalGeographicTag::setLatOrigin(double origin)
{
   std::ostringstream out;

   out << std::setw(15)
       << std::setfill('0')
       << origin;
   memcpy(theLatOrigin, out.str().c_str(), 15);
}

void ossimNitfLocalGeographicTag::setLonOrigin(double origin)
{
   std::ostringstream out;

   out << std::setw(15)
       << std::setfill('0')
       << origin;
   memcpy(theLonOrigin, out.str().c_str(), 15);
}
