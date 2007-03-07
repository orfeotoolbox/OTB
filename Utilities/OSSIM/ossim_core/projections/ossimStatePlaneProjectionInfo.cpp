//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//*******************************************************************
//  $Id: ossimStatePlaneProjectionInfo.cpp,v 1.9 2004/06/09 15:36:16 dburken Exp $
#include "ossimStatePlaneProjectionInfo.h"
#include "map_projections/ossimTransMercatorProjection.h"
#include "map_projections/ossimLambertConformalConicProjection.h"
#include "base/common/ossimKeywordNames.h"
#include "base/factory/ossimDatumFactory.h"
#include "base/common/ossimConstants.h"

ossimStatePlaneProjectionInfo::ossimStatePlaneProjectionInfo(char*  name,
                                                             int    pcsCode,
                                                             char*  projName,
                                                             char*  param1,
                                                             char*  param2,
                                                             char*  param3,
                                                             char*  param4,
                                                             double falseEast,
                                                             double falseNorth,
                                                             char*  units)
   :
      thePcsCode        (pcsCode),
      theName           (name),
      theProjectionName (projName),
      theDatum          (0),
      theOriginLat      (param1),
      theOriginLon      (param2),
      theOrigin         (),
      theParameter3     (0.0),
      theParameter4     (0.0),
      theFalseEasting   (falseEast),
      theFalseNorthing  (falseNorth),
      theScaleFactor    (0.0),
      theUnits          (units)
      
{
   // Set the parameters 
   theParameter3 = ossimDms(param3).getDegrees();
   theParameter4 = ossimDms(param4).getDegrees();

   if(theName.contains("NAD27"))
   {
      theDatum = ossimDatumFactory::instance()->create("NAS-C"); // NAD 1927
   }
   else
   {
      theDatum = ossimDatumFactory::instance()->create("NAR-C");  // NAD 1983
   }

   theOrigin = ossimGpt(theOriginLat.getDegrees(),
                        theOriginLon.getDegrees(),
                        0,
                        theDatum);   
}

ossimStatePlaneProjectionInfo::ossimStatePlaneProjectionInfo(char*  name,
                                                             int    pcsCode,
                                                             char*  projName,
                                                             char*  param1,
                                                             char*  param2,
                                                             double param3,
                                                             double param4,
                                                             double falseEast,
                                                             double falseNorth,
                                                             char*  units)
   :
      thePcsCode        (pcsCode),
      theName           (name),
      theProjectionName (projName),
      theDatum          (0),
      theOriginLat      (param1),
      theOriginLon      (param2),
      theOrigin         (),
      theParameter3     (param3),
      theParameter4     (param4),
      theFalseEasting   (falseEast),
      theFalseNorthing  (falseNorth),
      theScaleFactor    (0.0),
      theUnits          (units)
{
   if(theName.contains("NAD27"))
   {
      theDatum = ossimDatumFactory::instance()->create("NAS-C"); // NAD 1927
   }
   else
   {
      theDatum = ossimDatumFactory::instance()->create("NAR-C");  // NAD 1983
   }

   theOrigin = ossimGpt(theOriginLat.getDegrees(),
                        theOriginLon.getDegrees(),
                        0,
                        theDatum);   
   if(fabs(theParameter3) > FLT_EPSILON)
   {
      theScaleFactor = 1.0 - (1.0 / theParameter3);
   }
   else
   {
      theScaleFactor = 1.0;
   }
   
}

int ossimStatePlaneProjectionInfo::code() const
{
   return thePcsCode;
}

const ossimString& ossimStatePlaneProjectionInfo::name() const
{
   return theName;
}

const ossimString& ossimStatePlaneProjectionInfo::projName() const
{
   return theProjectionName;
}

double ossimStatePlaneProjectionInfo::originLat() const
{
   return theOriginLat.getDegrees();
}

double ossimStatePlaneProjectionInfo::originLon() const
{
   return theOriginLon.getDegrees();
}

const ossimDatum* ossimStatePlaneProjectionInfo::datum() const
{
   return theDatum;
}

ossimGpt ossimStatePlaneProjectionInfo::origin() const
{
   return theOrigin;
}

double ossimStatePlaneProjectionInfo::parallel1() const
{
   return theParameter3;
}

double ossimStatePlaneProjectionInfo::parallel2() const
{
   return theParameter4;
}

double ossimStatePlaneProjectionInfo::falseEasting() const
{
   return theFalseEasting;
}

double ossimStatePlaneProjectionInfo::falseNorthing() const
{
   return theFalseNorthing;
}

double ossimStatePlaneProjectionInfo::scaleFactor() const
{
   return theScaleFactor;
}

const ossimString&  ossimStatePlaneProjectionInfo::units() const
{
   return theUnits;
}

bool ossimStatePlaneProjectionInfo::isSameCode(int pcsCode) const
{
   if(pcsCode == thePcsCode)
   {
      return true;
   }

   return false;
}

double ossimStatePlaneProjectionInfo::falseEastingInMeters() const
{
   if (units()=="m")
   {
      return theFalseEasting;
   }
   return theFalseEasting * US_METERS_PER_FT;
}

double ossimStatePlaneProjectionInfo::falseNorthingInMeters() const
{
   if (units()=="m")
   {
      return theFalseNorthing;
   }

   return theFalseNorthing * US_METERS_PER_FT;
}

void ossimStatePlaneProjectionInfo::populateProjectionKeywords(ossimKeywordlist& kwl,
                                                               const char* prefix)const
{
  kwl.add(prefix,
	  ossimKeywordNames::PCS_CODE_KW,
	  thePcsCode,
	  true);

   kwl.add(prefix,
           ossimKeywordNames::FALSE_EASTING_KW,
           falseEastingInMeters(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::FALSE_NORTHING_KW,
           falseNorthingInMeters(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           origin().latd(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           origin().lond(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           projName(),
           true);
   if(theDatum)
   {
      
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theDatum->code(),
              true);
   }

   if (projName()== STATIC_TYPE_NAME(ossimLambertConformalConicProjection))
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              STATIC_TYPE_NAME(ossimLambertConformalConicProjection),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_1_KW,
              parallel1(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_2_KW,
              parallel2(),
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::SCALE_FACTOR_KW,
              theScaleFactor,
              true);
   }
}

ostream& operator<<(ostream& os, const ossimStatePlaneProjectionInfo& thePlane)
{
   if (thePlane.projName()== STATIC_TYPE_NAME(ossimTransMercatorProjection))
   {
      return  os << "ossimStatePlaneProjection: "
                 << "\nName:    " << thePlane.name()
                 << "\nPcsCode: " << thePlane.code()
                 << "\norigin:  " << thePlane.origin()
                 << "\nscale factor:  " << setprecision(12)
                 << thePlane.scaleFactor()
                 << "\nfalseEasting:   " << thePlane.falseEasting()
                 << "\nfalseNorthing:  " << thePlane.falseNorthing()
                 << endl;
   }
   else  // Lambert
   {
      return  os << "ossimStatePlaneProjection: "
                 << "\nName:    " << thePlane.name()
                 << "\nPcsCode: " << thePlane.code()
                 << "\norigin:  " << thePlane.origin()
                 << "\nphi1:    "
                 << setprecision(12) << thePlane.parallel1()
                 << "\nphi2:    "
                 << setprecision(12) << thePlane.parallel2()
                 << "\nfalseEasting:   " << thePlane.falseEasting()
                 << "\nfalseNorthing:  " << thePlane.falseNorthing()
                 << endl;
   }
}
