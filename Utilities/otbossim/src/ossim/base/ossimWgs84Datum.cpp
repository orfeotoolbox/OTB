//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Wgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a Wgs84Datum
//*******************************************************************
//  $Id: ossimWgs84Datum.cpp 22321 2013-07-22 11:40:45Z gpotts $

#include <ossim/base/ossimWgs84Datum.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimEllipsoidFactory.h>
#include <ossim/base/ossimEllipsoid.h>


RTTI_DEF1(ossimWgs84Datum, "ossimWgs84Datum", ossimThreeParamDatum);

ossimWgs84Datum::ossimWgs84Datum()
   :ossimThreeParamDatum("WGE",
                         "World Geodetic System 1984",
                         ossimEllipsoidFactory::instance()->wgs84(),
                         0.0, 0.0, 0.0,
                         -180.0,180.0,-90.0,90.0,
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
