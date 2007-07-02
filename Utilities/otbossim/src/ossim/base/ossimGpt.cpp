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
//  $Id: ossimGpt.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>
#include <sstream>

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimGeocent.h>
#include <ossim/base/ossimString.h>

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


void ossimGpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
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
   
   const int SZ = 64; // Handle real big number...
   ossimString os;
   char buf[SZ];
   char c = 0;

   //---
   // LATITUDE SECTION:
   //---
   
   // Grab data up to the first comma.
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;
   
   // Get rid of the '(' if there is any.
   std::string::size_type pos = os.find('(');
   if (pos != std::string::npos)
   {
      os.erase(pos, 1);
   }   

   if (os.contains("nan") == false)
   {
      pt.latd(os.toFloat64());
   }
   else
   {
      pt.latd(OSSIM_DBL_NAN);
   }

   // Eat the comma that we stopped at.
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }
   
   //---
   // LONGITUDE SECTION:
   //---
   
   // Grab the data up to the next ','
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;

   if (os.contains("nan") == false)
   {
      pt.lond(os.toFloat64());
   }
   else
   {
      pt.lond(OSSIM_DBL_NAN);
   }

   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // HEIGHT SECTION:
   //---
   
   // Grab the data up to the ','
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;
   
   if (os.contains("nan") == false)
   {
      pt.height(os.toFloat64());
   }
   else
   {
      pt.height(OSSIM_DBL_NAN);
   }

   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // DATUM SECTION:
   //---
   
   // Grab the data up to the ')'
   is.get(buf, SZ, ')');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;
   os.trim(); // Just in case datum factory doesn't handle spaces.
   
   const ossimDatum* datum = ossimDatumFactory::instance()->create(os);
   if (datum)
   {
      pt.datum(datum);
   }
   else
   {
      pt.datum(ossimDatumFactory::instance()->wgs84());
   }

   // Gobble the trailing ")".
   c = 0;
   while (c != ')')
   {
      is.get(c);
      if (!is) break;
   }

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
