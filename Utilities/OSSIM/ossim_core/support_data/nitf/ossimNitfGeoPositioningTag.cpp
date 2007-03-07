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
// $Id: ossimNitfGeoPositioningTag.cpp,v 1.3 2004/09/28 19:16:40 gpotts Exp $
#include <support_data/nitf/ossimNitfGeoPositioningTag.h>
#include <cstring>

RTTI_DEF1(ossimNitfGeoPositioningTag, "ossimNitfGeoPositioningTag", ossimNitfRegisteredTag);

ossimNitfGeoPositioningTag::ossimNitfGeoPositioningTag()
      :ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfGeoPositioningTag::~ossimNitfGeoPositioningTag()
{
}

ossimString ossimNitfGeoPositioningTag::getRegisterTagName()const
{
   return "GEOPSB";
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

ossim_uint32 ossimNitfGeoPositioningTag::getSizeInBytes()const
{
   return 443;
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

   theType[3]                    = '\0';
   theCoordinateUnits[3]         = '\0';
   theGeodeticDatumName[80]      = '\0';
   theGeodeticDatumCode[4]       = '\0';
   theEllipsoidName[80]          = '\0';
   theEllipsoidCode[3]           = '\0';
   theVerticalDatumReference[80] = '\0';
   theVerticalReferenceCode[4]   = '\0';
   theSoundingDatumName[80]      = '\0';
   theZFalseOrigin[15]           = '\0';
   theGridCode[3]                = '\0';
   theGridDescription[80]        = '\0';
   theGridZoneNumber[4]          = '\0';
   
}
