//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// This is the Universal Polar Stereographic (UPS) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUpspt.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/projection/ossimUpspt.h>
#include <ossim/projection/ossimUtmpt.h>
#include <ossim/projection/ossimUps.h>

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>

ossimUpspt::ossimUpspt(const ossimGpt &aPt)
{
   convertFromGeodetic(aPt);
}

ossimUpspt::ossimUpspt(const ossimEcefPoint &aPt)
{
   convertFromGeodetic(ossimGpt(aPt));
}


void ossimUpspt::convertFromGeodetic(const ossimGpt &aPt)
{
   const ossimDatum *aDatum = aPt.datum();

   if(aDatum)
   {
      Set_UPS_Parameters(aDatum->ellipsoid()->a(),
                         aDatum->ellipsoid()->b());
      
      Convert_Geodetic_To_UPS(aPt.latr(),
                              aPt.lonr(),
                              &theHemisphere,
                              &theEasting,
                              &theNorthing);
      theDatum = const_cast<ossimDatum*>(aDatum);
   }
   else
   {
      //ERROR: should never happen
   }
}
