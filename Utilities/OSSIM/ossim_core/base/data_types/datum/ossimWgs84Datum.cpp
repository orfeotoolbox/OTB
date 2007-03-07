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
// Wgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a Wgs84Datum
//*******************************************************************
//  $Id: ossimWgs84Datum.cpp,v 1.5 2001/10/26 19:41:17 gpotts Exp $

#include "ossimWgs84Datum.h"
#include "base/common/ossimConstants.h"
#include "base/factory/ossimEllipsoidFactory.h"
#include "base/data_types/ellipse/ossimEllipsoid.h"



ossimWgs84Datum::ossimWgs84Datum()
   :ossimThreeParamDatum("WGE",
                         "World Geodetic System 1984",
                         ossimEllipsoidFactory::instance()->wgs84(),
                         0.0, 0.0, 0.0,
                         -M_PI/2.0, M_PI/2.0, -M_PI, M_PI,
                         0.0, 0.0, 0.0)
{
   if(!ellipsoid())
   {
      //ERROR
   }
}


ossimGpt ossimWgs84Datum::shift(const ossimGpt &aPt)const
{
   const ossimDatum *aDatum = aPt.datum();
   
    if( aPt.datum()->code() == code())
    {
       return ossimGpt(aPt.latd(), aPt.lond(), aPt.height(), this);
    }
   
    if(aDatum)
    {
       return aDatum->shiftToWgs84(aPt);
    }

   return aPt;      
}

ossimGpt ossimWgs84Datum::shiftToWgs84(const ossimGpt &aPt)const
{
   return ossimGpt(aPt.latd(),
                   aPt.lond(),
                   aPt.height(),
                   ossimDatumFactory::instance()->wgs84());
}

ossimGpt ossimWgs84Datum::shiftFromWgs84(const ossimGpt &aPt)const
{
   return aPt;
}
