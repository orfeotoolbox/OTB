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
//
// Description:
//
// SevenParamDatum. This is a base class for all
// seven param datums.
//*******************************************************************
//  $Id: ossimSevenParamDatum.cpp,v 1.9 2003/09/29 13:57:19 gpotts Exp $

#include "ossimSevenParamDatum.h"
#include "base/factory/ossimDatumFactory.h"
#include "base/data_types/ellipse/ossimEllipsoid.h"
#include "base/data_types/ossimEcefPoint.h"
#include "ossimWgs84Datum.h"

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

