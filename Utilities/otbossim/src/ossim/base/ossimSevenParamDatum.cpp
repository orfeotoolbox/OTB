//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// SevenParamDatum. This is a base class for all
// seven param datums.
//*******************************************************************
//  $Id: ossimSevenParamDatum.cpp 19795 2011-06-30 15:04:48Z gpotts $

#include <ossim/base/ossimSevenParamDatum.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimWgs84Datum.h>

RTTI_DEF1(ossimSevenParamDatum, "ossimSevenParamDatum", ossimDatum);
ossimGpt ossimSevenParamDatum::shift(const ossimGpt &aPt)const
{
   const ossimDatum* aDatum = aPt.datum();

   if( (ellipsoid()->getA()== aPt.datum()->ellipsoid()->getA())&&
       (ellipsoid()->getB()== aPt.datum()->ellipsoid()->getB()))
   {
      return ossimGpt(aPt.latd(), aPt.lond(), aPt.height(), this);
   }
   
   if(aDatum)
   {
      return shiftFromWgs84( aDatum->shiftToWgs84(aPt) );
   }

   return aPt;
}

ossimGpt ossimSevenParamDatum::shiftToWgs84(const ossimGpt &aPt)const
{
   ossimEcefPoint p1 = aPt;
   ossimEcefPoint p2;

//    if(aPt.isHgtNan())
//    {
//       p1 = ossimEcefPoint(ossimGpt(aPt.latd(),
//                                    aPt.lond(),
//                                    0,
//                                    aPt.datum()));
//    }
   
//    if(withinMolodenskyRange(aPt.latd()))
//    {
//       ossimWgs84Datum wgs84;
//       double latin, lonin, hgtin;
//       double latout, lonout, hgtout;
      
//       double a = wgs84.ellipsoid()->getA();
//       double f = wgs84.ellipsoid()->getFlattening();
//       double da = a - ellipsoid()->getA();
//       double df = f - ellipsoid()->getFlattening();
      
//       latin = aPt.latr();
//       lonin = aPt.lonr();
//       hgtin = aPt.height();

//       if(aPt.isHgtNan())
//       {
//          hgtin = 0.0;
//       }
//       molodenskyShift(a, da, f, df, param1(), param2(), param3(),
//                       latin, lonin, hgtin,
//                       latout, lonout, hgtout);
      
//       ossimGpt g;

//       g.latr(latout);
//       g.lonr(lonout);
//       g.height(hgtout);
//       if(aPt.isHgtNan())
//       {
//          g.height(OSSIM_DBL_NAN);
//       }
//       return g;
//    }
//    else
   {
      //this is straight from Geotrans
      p2.x() = p1.x() + theParam1 + theParam6*p1.y() -
               theParam5*p1.z() + theParam7*p1.x();
      p2.y() = p1.y() + theParam2 - theParam6*p1.x()+
               theParam4*p1.z() + theParam7*p1.y();
      p2.z() = p1.z() + theParam3 + theParam5*p1.x() -
               theParam4*p1.y() + theParam7*p1.z();
   }
   
   return ossimGpt(p2); // defaults to WGS84
}

ossimGpt ossimSevenParamDatum::shiftFromWgs84(const ossimGpt &aPt)const
{      
   ossimEcefPoint p1;
   ossimEcefPoint p2;

   if(aPt.isHgtNan())
   {
      p1 = ossimEcefPoint(ossimGpt(aPt.latd(),
                                   aPt.lond(),
                                   0,
                                   aPt.datum()));
   }
   else
   {
      p1 = ossimEcefPoint(aPt);
   }
//    if(withinMolodenskyRange(aPt.latd()))
//    {
//       ossimWgs84Datum wgs84;
//       double latin, lonin, hgtin;
//       double latout, lonout, hgtout;
      
//       double a = wgs84.ellipsoid()->getA();
//       double f = wgs84.ellipsoid()->getFlattening();
//       double da = ellipsoid()->getA() - a;
//       double df = ellipsoid()->getFlattening() - f;

//       latin = aPt.latr();
//       lonin = aPt.lonr();
//       hgtin = aPt.height();

//       if(aPt.isHgtNan())
//       {
//          hgtin = 0.0;
//       }
//       molodenskyShift(a, da, f, df, -param1(), -param2(), -param3(),
//                       latin, lonin, hgtin,
//                       latout, lonout, hgtout);
//       ossimGpt g;

//       g.latr(latout);
//       g.lonr(lonout);
//       g.height(hgtout);
//       g.datum(this);
//       return g;
//    }
//    else
//    {
      p2.x() = p1.x() - theParam1 - theParam6 * p1.y()
               + theParam5 * p1.z() - theParam7 * p1.x();
      p2.y() = p1.y() - theParam2 + theParam6 * p1.x()
               - theParam4 *p1.z() - theParam7 * p1.y();

      p2.z() = p1.z() - theParam3 - theParam5 * p1.x()
               + theParam4 * p1.y() - theParam7 * p1.z();
//   }
   
   return ossimGpt(p2, this);
}

bool ossimSevenParamDatum::isEqualTo(const ossimObject& obj, ossimCompareType compareType)const
{
   const ossimSevenParamDatum* datum = dynamic_cast<const ossimSevenParamDatum*> (&obj);
   
   bool result = datum&&ossimDatum::isEqualTo(obj, compareType);
   
   if(result)
   {
      result = (ossim::almostEqual(theParam1, datum->theParam1)&&
                ossim::almostEqual(theParam2, datum->theParam2)&&
                ossim::almostEqual(theParam3, datum->theParam3)&&
                ossim::almostEqual(theParam4, datum->theParam4)&&
                ossim::almostEqual(theParam5, datum->theParam5)&&
                ossim::almostEqual(theParam6, datum->theParam6)&&
                ossim::almostEqual(theParam7, datum->theParam7)
                );
   }
   
   return result;
}

