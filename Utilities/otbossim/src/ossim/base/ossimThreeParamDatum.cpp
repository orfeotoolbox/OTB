//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//*******************************************************************
//  $Id: ossimThreeParamDatum.cpp 19795 2011-06-30 15:04:48Z gpotts $

#include <cmath>

#include <ossim/base/ossimThreeParamDatum.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimWgs84Datum.h>

RTTI_DEF1(ossimThreeParamDatum, "ossimThreeParamDatum", ossimDatum);
ossimGpt ossimThreeParamDatum::shiftToWgs84(const ossimGpt &aPt)const
{
   
   if(ossim::almostEqual(param1(),  0.0)&&
      ossim::almostEqual(param2(), 0.0)&&
      ossim::almostEqual(param3(), 0.0))
   {
      return ossimGpt(aPt.latd(),
                      aPt.lond(),
                      aPt.latd(),
                      ossimGpt().datum());
   }
      
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
   if(ossim::almostEqual(param1(), 0.0)&&
      ossim::almostEqual(param2(), 0.0)&&
      ossim::almostEqual(param3(), 0.0))
   {
      return ossimGpt(aPt.latd(),
                      aPt.lond(),
                      aPt.latd(),
                      this);
   }
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

bool ossimThreeParamDatum::isEqualTo(const ossimObject& obj, ossimCompareType compareType)const
{
   const ossimThreeParamDatum* datum = dynamic_cast<const ossimThreeParamDatum*> (&obj);
   
   bool result = datum&&ossimDatum::isEqualTo(obj, compareType);
   
   if(result)
   {
      result = (ossim::almostEqual(theParam1, datum->theParam1)&&
                ossim::almostEqual(theParam2, datum->theParam2)&&
                ossim::almostEqual(theParam3, datum->theParam3));
   }
   
   return result;
}
