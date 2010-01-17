//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// This is the Universal Transverse Mercator (UTM) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUtmpt.cpp 10058 2006-12-08 20:04:23Z mlucas $

#include <ossim/projection/ossimUtmpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/projection/ossimUtm.h>

ossimUtmpt::ossimUtmpt(const ossimGpt &aPt)
{
   convertFromGround(aPt);
}

ossimUtmpt::ossimUtmpt(const ossimEcefPoint &aPt)
{
   convertFromGround(ossimGpt(aPt));
}

ossimUtmpt::ossimUtmpt(long         zone,
                       char         hemisphere,  // N or S
                       double       easting,
                       double       northing,
                       const ossimDatum* datum)
   :
      theZone       (zone),
      theHemisphere (hemisphere),
      theEasting    (easting),
      theNorthing   (northing),
      theDatum      (datum)
{}

void ossimUtmpt::convertFromGround(const ossimGpt &aPt)
{
   const ossimDatum *aDatum = aPt.datum();

   if(aDatum)
   {
      //call Geotrans init code
      Set_UTM_Parameters(aDatum->ellipsoid()->a(), aDatum->ellipsoid()->flattening(), 0);
      Convert_Geodetic_To_UTM(aPt.latr(),
                              aPt.lonr(),
                              &theZone,
                              &theHemisphere,
                              &theEasting,
                              &theNorthing);
      theDatum = aDatum;
   }
   else
   {
      //ERROR: Should never happen
   }
}

void ossimUtmpt::convertToGround(ossimGpt &aPt)
{
   Set_UTM_Parameters( theDatum->ellipsoid()->a(),
                       theDatum->ellipsoid()->flattening(),
                       0 );
   
   double latitude  = 0.0;
   double longitude = 0.0;

   Convert_UTM_To_Geodetic( zone(),
                            hemisphere(),
                            easting(),
                            northing(),
                            &latitude,
                            &longitude );

   aPt.latr(latitude);
   aPt.lonr(longitude);
   aPt.datum(theDatum);
}
