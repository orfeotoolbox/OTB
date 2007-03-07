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
//*******************************************************************
//  $Id: ossimThreeParamDatum.cpp,v 1.13 2005/09/19 11:40:10 gpotts Exp $

#include <math.h>

#include "ossimThreeParamDatum.h"
#include "base/common/ossimConstants.h"
#include "base/factory/ossimDatumFactory.h"
#include "base/data_types/ellipse/ossimEllipsoid.h"
#include "base/data_types/ossimGpt.h"
#include "base/data_types/ossimEcefPoint.h"
#include "base/data_types/datum/ossimWgs84Datum.h"

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
