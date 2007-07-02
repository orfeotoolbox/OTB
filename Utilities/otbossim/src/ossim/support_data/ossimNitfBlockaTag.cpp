//----------------------------------------------------------------------------
// Copyright (c) 2004, David Burken, all rights reserved.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: BLOCKA tag class declaration.
//
//----------------------------------------------------------------------------
// $Id: ossimNitfBlockaTag.cpp 9455 2006-08-25 14:48:06Z dburken $

#include <cstring> /* for memcpy */
#include <sstream>
#include <iomanip>

#include <ossim/support_data/ossimNitfBlockaTag.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimDpt.h>

static const ossimTrace traceDebug(ossimString("ossimNitfBlockaTag:debug"));

RTTI_DEF1(ossimNitfBlockaTag, "ossimNitfBlockaTag", ossimNitfRegisteredTag);

#ifndef MIN
#  define MIN(a,b) ((a)<(b)?(a):(b))
#endif

ossimNitfBlockaTag::ossimNitfBlockaTag()
{
   clearFields();
}

ossimString ossimNitfBlockaTag::getRegisterTagName() const
{
   return ossimString("BLOCKA");
}

void ossimNitfBlockaTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theBlockInstance, BLOCK_INSTANCE_SIZE);
   in.read(theNGray,         N_GRAY_SIZE);
   in.read(theLLines,        L_LINES_SIZE);
   in.read(theLayoverAngle,  LAYOVER_ANGLE_SIZE);
   in.read(theShadowAngle,   SHADOW_ANGLE_SIZE);
   in.read(theField6,        FIELD_6_SIZE);
   in.read(theFrlcLoc,       FRLC_LOC_SIZE);
   in.read(theLrlcLoc,       LRLC_LOC_SIZE);
   in.read(theLrfcLoc,       LRFC_LOC_SIZE);
   in.read(theFrfcLoc,       FRFC_LOC_SIZE);
   in.read(theField11,       FIELD_11_SIZE);
   
}

void ossimNitfBlockaTag::writeStream(std::ostream& out)
{
   out.write(theBlockInstance, BLOCK_INSTANCE_SIZE);
   out.write(theNGray,         N_GRAY_SIZE);
   out.write(theLLines,        L_LINES_SIZE);
   out.write(theLayoverAngle,  LAYOVER_ANGLE_SIZE);
   out.write(theShadowAngle,   SHADOW_ANGLE_SIZE);
   out.write(theField6,        FIELD_6_SIZE);
   out.write(theFrlcLoc,       FRLC_LOC_SIZE);
   out.write(theLrlcLoc,       LRLC_LOC_SIZE);
   out.write(theLrfcLoc,       LRFC_LOC_SIZE);
   out.write(theFrfcLoc,       FRFC_LOC_SIZE);
   out.write(theField11,       FIELD_11_SIZE);
}

ossim_uint32 ossimNitfBlockaTag::getSizeInBytes()const
{
   return 123;
}

void ossimNitfBlockaTag::clearFields()
{
   memcpy(theBlockInstance, "01",    BLOCK_INSTANCE_SIZE);
   memset(theNGray,         '0',     N_GRAY_SIZE);
   memset(theLLines,        '0',     L_LINES_SIZE);
   memset(theLayoverAngle,  ' ',     LAYOVER_ANGLE_SIZE);
   memset(theShadowAngle,   ' ',     SHADOW_ANGLE_SIZE);
   memset(theField6,        ' ',     FIELD_6_SIZE);
   memset(theFrlcLoc,       ' ',     FRLC_LOC_SIZE);
   memset(theLrlcLoc,       ' ',     LRLC_LOC_SIZE);
   memset(theLrfcLoc,       ' ',     LRFC_LOC_SIZE);
   memset(theFrfcLoc,       ' ',     FRFC_LOC_SIZE);
   memcpy(theField11,       "010.0", FIELD_11_SIZE);
   
   theBlockInstance[BLOCK_INSTANCE_SIZE] = '\0';
   theNGray[N_GRAY_SIZE]                 = '\0';
   theLLines[L_LINES_SIZE]               = '\0';
   theLayoverAngle[LAYOVER_ANGLE_SIZE]   = '\0';
   theShadowAngle[SHADOW_ANGLE_SIZE]     = '\0';
   theField6[FIELD_6_SIZE]               = '\0';
   theFrlcLoc[FRLC_LOC_SIZE]             = '\0';
   theLrlcLoc[LRLC_LOC_SIZE]             = '\0';
   theLrfcLoc[LRFC_LOC_SIZE]             = '\0';
   theFrfcLoc[FRFC_LOC_SIZE]             = '\0';
   theField11[FIELD_11_SIZE]              = '\0';
}

std::ostream& ossimNitfBlockaTag::print(std::ostream& out) const
{
   // Grab the corners parsed into points.
   ossimDpt ulPt;
   ossimDpt urPt;
   ossimDpt lrPt;
   ossimDpt llPt;
   getFrfcLoc(ulPt);
   getFrlcLoc(urPt);
   getLrlcLoc(lrPt);
   getLrfcLoc(llPt);
   
   out << setiosflags(ios::left) << "ossimNitfBlockaTag::print"
       << setw(24) << "\nBLOCK_INSTANCE:" << theBlockInstance
       << setw(24) << "\nN_GRAY:"         << theNGray
       << setw(24) << "\nL_LINES:"        << theLLines
       << setw(24) << "\nLAYOVER_ANGLE:"  << theLayoverAngle
       << setw(24) << "\nSHADOW_ANGLE:"   << theShadowAngle
       << setw(24) << "\nFIELD_6:"        << theField6
       << setw(24) << "\nFRLC_LOC:"       << theFrlcLoc
       << setw(24) << "\nLRLC_LOC:"       << theLrlcLoc
       << setw(24) << "\nLRFC_LOC:"       << theLrfcLoc
       << setw(24) << "\nFRFC_LOC:"       << theFrfcLoc
       << setw(24) << "\nFIELD_11:"       << theField11
       << setw(24) << "\nupper left:"     << ulPt
       << setw(24) << "\nupper right:"    << urPt
       << setw(24) << "\nlower right:"    << lrPt
       << setw(24) << "\nlower left:"     << llPt
       << std::endl;

   return out;
}

void ossimNitfBlockaTag::setBlockInstance(ossim_uint32 block)
{
   if ( (block > 0) && (block < 100) )
   {
      ossimString os =
         ossimNitfCommon::convertToIntString(block,
                                             BLOCK_INSTANCE_SIZE);
      memcpy(theBlockInstance, os.c_str(), BLOCK_INSTANCE_SIZE);
   }
}

ossimString ossimNitfBlockaTag::getBlockInstance() const
{
   return ossimString(theBlockInstance);
}

void ossimNitfBlockaTag::setNGray(ossim_uint32 grayCount)
{
   if (grayCount < 100000)
   {
      ossimString os =
         ossimNitfCommon::convertToUIntString(grayCount,
                                              N_GRAY_SIZE);
      memcpy(theNGray, os.c_str(), N_GRAY_SIZE);
   }
}

ossimString ossimNitfBlockaTag::getNGray() const
{
   return ossimString(theNGray);
}

void ossimNitfBlockaTag::setLLines(ossim_uint32 lines)
{
   if (lines < 100000)
   {
      ossimString os =
         ossimNitfCommon::convertToUIntString(lines, L_LINES_SIZE);
      memcpy(theLLines, os.c_str(), L_LINES_SIZE);
   }
}

ossimString ossimNitfBlockaTag::getLLines() const
{
   return ossimString(theLLines);
}

void ossimNitfBlockaTag::setLayoverAngle(ossim_uint32 angle)
{
   if (angle < 360)
   {
      ossimString os =
         ossimNitfCommon::convertToUIntString(angle, LAYOVER_ANGLE_SIZE);
      memcpy(theLayoverAngle, os.c_str(), LAYOVER_ANGLE_SIZE);
   }
}

ossimString ossimNitfBlockaTag::getLayoverAngle() const
{
   return ossimString(theLayoverAngle);
}

void ossimNitfBlockaTag::setShadowAngle(ossim_uint32 angle)
{
   if (angle < 360)
   {
      ossimString os =
         ossimNitfCommon::convertToUIntString(angle, SHADOW_ANGLE_SIZE);
      memcpy(theShadowAngle, os.c_str(), SHADOW_ANGLE_SIZE);
   }
}

ossimString ossimNitfBlockaTag::getShadowAngle() const
{
   return ossimString(theShadowAngle);
}

void ossimNitfBlockaTag::setFrlcLoc(const ossimDpt& pt)
{
   ossimString os = ossimNitfCommon::convertToDdLatLonLocString(pt, 6);
   memcpy(theFrlcLoc, os.c_str(), FRLC_LOC_SIZE);
}

ossimString ossimNitfBlockaTag::getFrlcLoc() const
{
   return ossimString(theFrlcLoc);
}

void ossimNitfBlockaTag::getFrlcLoc(ossimDpt& pt) const
{
   ossimString os = theFrlcLoc;
   converLocStringToPt(os, pt);
}

void ossimNitfBlockaTag::setLrlcLoc(const ossimDpt& pt)
{
   ossimString os = ossimNitfCommon::convertToDdLatLonLocString(pt, 6);
   memcpy(theLrlcLoc, os.c_str(), LRLC_LOC_SIZE);
}
   
ossimString ossimNitfBlockaTag::getLrlcLoc() const
{
   return ossimString(theLrlcLoc);
}

void ossimNitfBlockaTag::getLrlcLoc(ossimDpt& pt) const
{
   ossimString os = theLrlcLoc;
   converLocStringToPt(os, pt);
}

void ossimNitfBlockaTag::setLrfcLoc(const ossimDpt& pt)
{
   ossimString os = ossimNitfCommon::convertToDdLatLonLocString(pt, 6);
   memcpy(theLrfcLoc, os.c_str(), LRFC_LOC_SIZE);
}

ossimString ossimNitfBlockaTag::getLrfcLoc() const
{
   return ossimString(theLrfcLoc);
}

void ossimNitfBlockaTag::getLrfcLoc(ossimDpt& pt) const
{
   ossimString os = theLrfcLoc;
   converLocStringToPt(os, pt); 
}

void ossimNitfBlockaTag::setFrfcLoc(const ossimDpt& pt)
{
   ossimString os = ossimNitfCommon::convertToDdLatLonLocString(pt, 6);
   memcpy(theFrfcLoc, os.c_str(), FRFC_LOC_SIZE);
}

ossimString ossimNitfBlockaTag::getFrfcLoc() const
{
   return ossimString(theFrfcLoc);
}

void ossimNitfBlockaTag::getFrfcLoc(ossimDpt& pt) const
{
   ossimString os = theFrfcLoc;
   converLocStringToPt(os, pt);
}

void ossimNitfBlockaTag::converLocStringToPt(const ossimString& locationString,
                                             ossimDpt& pt) const
{
   if (locationString.size() != 21) return;

   std::string s = locationString;
   
   std::string lat(s, 0, 10);
   std::string lon(s, 10, 11);

   if ( (lat[0] == 'N') || (lat[0] == 'S') )
   {
      // Need to enter spaces for ossimDms to parse correctly
      std::ostringstream latStream;
      latStream << lat.substr(0, 3) << " " << lat.substr(3, 2)
                << " " << lat.substr(5);
      lat = latStream.str();
      
      // Use the dms class to parse.
      ossimDms dms(0.0, true);
      dms.setDegrees(lat.c_str());
      pt.y = dms.getDegrees();
   }
   else
   {
      ossimString os = lat;
      pt.y = os.toDouble();
   }
   if ( (lon[0] == 'E') || (lon[0] == 'W') )
   {
      // Need to enter spaces for ossimDms to parse correctly
      std::ostringstream lonStream;
      lonStream << lon.substr(0, 4) << " " << lon.substr(4, 2)
                << " " << lon.substr(6);
      lon = lonStream.str();
      
      // Use the dms class to parse.
      ossimDms dms(0.0, false);
      dms.setDegrees(lon.c_str());
      pt.x = dms.getDegrees();
   }
   else
   {
      ossimString os = lon;
      pt.x = os.toDouble();
   }
}
