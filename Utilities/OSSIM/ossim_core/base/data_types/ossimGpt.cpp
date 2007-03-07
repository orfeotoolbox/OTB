//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// LATITUDE AND LONGITUDE VALUES ARE IN DEGREES.
//
//*******************************************************************
//  $Id: ossimGpt.cpp,v 1.33 2005/12/16 14:25:11 dburken Exp $

#include <sstream>

#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimEcefVector.h>
#include <base/data_types/ossimDms.h>
#include <base/data_types/datum/ossimDatum.h>
#include <base/data_types/geoid/ossimGeoidManager.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/ossimGeocent.h>
#include <base/data_types/ossimString.h>

std::ostream& ossimGpt::print(std::ostream& os, ossim_uint32 precision) const
{
   os << setiosflags(ios::fixed) << setprecision(precision);
   os << "( ";

   if(isLatNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      os << latd() << ", ";
   }
   if(isLonNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      os << lond() << ", ";
   }
   if(isHgtNan())
   {
      os << "nan" << ", ";
   }
   else
   {
      os << height() << ", ";
   }
   
   os << (theDatum?theDatum->code().c_str():"") << " )";

   return os;
}

std::ostream& operator<<(std::ostream& os, const ossimGpt& pt)
{
   return pt.print(os);
}

ossimString ossimGpt::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   print(os, precision);
   return ossimString(os.str());
}

std::istream& operator>>(std::istream& is, ossimGpt &pt)
{
   //---
   // Expected input format:
   // ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
   //   -----latitude----  -----longitude----  ------height----  datum
   //---

   // Start with a nan point.
   pt.makeNan();

   // Check the stream.
   if (!is) return is;

   const int SZ = 22;
   ossimString tempString;
   char tempChars[SZ];
   char c;

   // Gobble the "(".
   is >> tempString;
   if (!is) return is;
   
   // Get the latitude.
   is.get(tempChars, SZ, ',');
   if (!is) return is;
   tempChars[SZ-1] = '\0';
   tempString = tempChars;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.latd(OSSIM_DBL_NAN);
   }
   else
   {
      pt.latd(tempString.toDouble());
   }

   // Eat the ",".
   is.get(c);
   
   // Get the longitude.
   is.get(tempChars, SZ, ',');
   if (!is) return is;
   tempChars[SZ-1] = '\0';
   tempString = tempChars;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.lond(OSSIM_DBL_NAN);
   }
   else
   {
      pt.lond(tempString.toDouble());
   }

   // Eat the ",".
   is.get(c);
   
   // Get the height.
   is.get(tempChars, SZ, ',');
   if (!is) return is;
   tempChars[SZ-1] = '\0';
   tempString = tempChars;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.height(OSSIM_DBL_NAN);
   }
   else
   {
      pt.height(tempString.toDouble());
   }

   // Eat the ",".
   is.get(c);

   // Get the datum.
   is >> tempString;
   if (!is) return is;
   const ossimDatum* datum = ossimDatumFactory::instance()->create(tempString);
   if (datum)
   {
      pt.datum(datum);
   }
   else
   {
      pt.datum(ossimDatumFactory::instance()->wgs84());
   }

   // Gobble the trailing ")".
   is >> tempString;

   // Finished
   return is;
}


//*****************************************************************************
//  COPY CONSTRUCTOR: ossimGpt(ossimGpt) 
//*****************************************************************************
ossimGpt::ossimGpt(const ossimGpt& src)
   : lat(src.lat),
     lon(src.lon),
     hgt(src.hgt)
{
   theDatum = src.datum();
   if(!theDatum)
   {
      theDatum = ossimDatumFactory::instance()->wgs84();
   }
//   limitLonTo180();
}

//*****************************************************************************
//  CONSTRUCTOR:  ossimGpt(ossimEcefPoint, datum)
//*****************************************************************************
ossimGpt::ossimGpt(const ossimEcefPoint& ecef_point,
                   const ossimDatum*     datum)
   :
      theDatum (datum)
{
   if(!theDatum)
   {
      theDatum = ossimDatumFactory::instance()->wgs84();
   }
   if(ecef_point.isNan())
   {
      makeNan();
   }
   else
   {
      theDatum->ellipsoid()->XYZToLatLonHeight(ecef_point.x(),
                                               ecef_point.y(),
                                               ecef_point.z(),
                                               lat,
                                               lon,
                                               hgt);
      
   }
}

//*****************************************************************************
//  METHOD: ossimGpt::operator =
//*****************************************************************************
const ossimGpt& ossimGpt::operator = (const ossimGpt &aPt)
{
   lat = aPt.lat;
   lon = aPt.lon;
   hgt = aPt.hgt;
   
   if(aPt.datum())
   {
      theDatum = aPt.datum();
   }
   if(!theDatum)
   {
      theDatum = ossimDatumFactory::instance()->wgs84();
   }
   
   return *this;
}

//*****************************************************************************
//  METHOD: ossimGpt::changeDatum
//*****************************************************************************
void ossimGpt::changeDatum(const ossimDatum *datum)
{
   if(datum == theDatum) return;
   if(!isLatNan() && !isLonNan()) // only shift if all values lat and lon is good
   {
      if(datum)
      {
	double h = hgt;
        *this = datum->shift(*this);
        if(h == OSSIM_DBL_NAN)
        {
           hgt = h;
        }
        theDatum = datum;
      }
   }
}


//*****************************************************************************
//  METHOD: ossimGpt::toDmsString()
//*****************************************************************************
ossimString ossimGpt::toDmsString()const
{
   ossimString result;

   result += "lat: ";
   if(isLatNan())
   {
      result += "nan";
   }
   else
   {
      result += ossimDms(latd()).toString("dd@mm'ss.ssss\"C");
   }
   result += " lon: ";
   if(isLonNan())
   {
      result += "nan";
   }
   else
   {
      result += ossimDms(lond(),false).toString("dd@mm'ss.ssss\"C");
   }
   
   return result;
}

//*****************************************************************************
//  METHOD: ossimGpt::distanceTo(ossimGpt)
//
//  Computes straight-line distance to arg point, in meters.
//
//*****************************************************************************
double ossimGpt::distanceTo(const ossimGpt& arg_pt) const
{
   ossimEcefPoint p1 (*this);
   ossimEcefPoint p2 (arg_pt);

   return (p1 - p2).magnitude();
}

//*****************************************************************************
//  METHOD: ossimGpt::heightMSL() const 
//
//  Returns the height data member adjusted for geoid.
//
//*****************************************************************************
double ossimGpt::heightMSL() const
{
   double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(*this);
   return (hgt - offset);
}

//*****************************************************************************
//  METHOD: ossimGpt::heightMSL(double)
//
//  Sets the height data member adjusted for geoid.
//
//*****************************************************************************
void ossimGpt::heightMSL(double heightMSL)
{
   double offset = ossimGeoidManager::instance()->offsetFromEllipsoid(*this);
   hgt = heightMSL + offset;
}

//*****************************************************************************
//  METHOD: ossimGpt::metersPerDegree(double)
//
//  Computes geodetic arc lengths at this ground point.
//
//*****************************************************************************
ossimDpt ossimGpt::metersPerDegree() const
{
   ossimDpt result;

   double radius = theDatum->ellipsoid()->geodeticRadius(lat);
   result.y =  RAD_PER_DEG * radius;
   result.x =  result.y * cosd(lat);

   return result;
}
