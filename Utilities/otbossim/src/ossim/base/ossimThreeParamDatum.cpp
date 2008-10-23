//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//*******************************************************************
//  $Id: ossimThreeParamDatum.cpp 13136 2008-07-06 14:50:09Z dburken $

#include <cmath>

#include <ossim/base/ossimThreeParamDatum.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimWgs84Datum.h>

ossimGpt ossimThreeParamDatum::shiftToWgs84(const ossimGpt &aPt)const
{
   ossimEcefPoint p1 = aPt;
   ossimEcefPoint p2;
 
   if(withinMolodenskyRange(aPt.latd()))
   {
      ossimWgs84Datum wgs84;
      double latin, lonin, hgtin;
      double latout, lonout, hgtout;
      
      double da = wgs84.ellipsoid()->getA() - ellipsoid()->getA();
      double df = wgs84.ellipsoid()->getFlattening() - ellipsoid()->getFlattening();

      latin = aPt.latr();
      lonin = aPt.lonr();
      hgtin = aPt.height();

      if(aPt.isHgtNan())
      {
         hgtin = 0.0;
      }
      molodenskyShift(ellipsoid()->getA(), da, ellipsoid()->getFlattening(), df, param1(), param2(), param3(),
                      latin, lonin, hgtin,
                      latout, lonout, hgtout);
      ossimGpt g;

      g.latr(latout);
      g.lonr(lonout);
      g.height(hgtout);
      g.datum(this);
      return g;
   }
   else
     {
       p2 = ossimEcefPoint(p1.x() + theParam1,
			   p1.y() + theParam2,
			   p1.z() + theParam3);
     }
   
   return ossimGpt(p2); // defaults to WGS84
}

ossimGpt ossimThreeParamDatum::shiftFromWgs84(const ossimGpt &aPt)const
{
   ossimEcefPoint p1=aPt;
   ossimEcefPoint p2=aPt;
   
   if(withinMolodenskyRange(aPt.latd()))
   {
      ossimWgs84Datum wgs84;
      double latin, lonin, hgtin;
      double latout, lonout, hgtout;
      
      double da = ellipsoid()->getA() -  wgs84.ellipsoid()->getA();
      double df = ellipsoid()->getFlattening() - wgs84.ellipsoid()->getFlattening();

      latin = aPt.latr();
      lonin = aPt.lonr();
      hgtin = aPt.height();

      if(aPt.isHgtNan())
      {
         hgtin = 0.0;
      }
      molodenskyShift(wgs84.ellipsoid()->getA(), da, wgs84.ellipsoid()->getFlattening(), df, -param1(), -param2(), -param3(),
                      latin, lonin, hgtin,
                      latout, lonout, hgtout);
      ossimGpt g;

      g.latr(latout);
      g.lonr(lonout);
      g.height(hgtout);
      g.datum(this);
      return g;
   }
   else
   {
      p2 = ossimEcefPoint(p1.x() - theParam1,
                          p1.y() - theParam2,
                          p1.z() - theParam3);
   }
   
   
   return ossimGpt(p2, this);
}


ossimGpt ossimThreeParamDatum::shift(const ossimGpt &aPt)const
{
   const ossimDatum *aDatum = aPt.datum();

   if( code() == aDatum->code())
   {
      return ossimGpt(aPt.latd(), aPt.lond(), aPt.height(), this);
   }
   
   if(aDatum)
   {
      return shiftFromWgs84(aDatum->shiftToWgs84(aPt));
   }

   return aPt;
}
