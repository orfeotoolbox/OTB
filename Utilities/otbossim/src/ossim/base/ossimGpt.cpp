//*******************************************************************
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
//  $Id: ossimGpt.cpp 20615 2012-02-27 12:43:12Z gpotts $

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimGeocent.h>
#include <iostream>
#include <sstream>

std::ostream& ossimGpt::print(std::ostream& os, ossim_uint32 precision) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = os.flags();
   
   // Set the new precision capturing old.
   std::streamsize oldPrecision = os.precision(precision);
   
   os << setiosflags(ios::fixed)
      << "( "; 

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
      // millimeter precision for height
      os << setprecision(3) << height() << ", ";
   }
   
   os << (theDatum?theDatum->code().c_str():"") << " )";

   // Reset flags and precision.
   os.setf(f);
   os.precision(oldPrecision);

   return os;
}

std::ostream& operator<<(std::ostream& os, const ossimGpt& pt)
{
   return pt.print(os);
}

ossimString ossimGpt::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   
   os << setprecision(precision);
   os << "(";
   
   if(isLatNan())
   {
      os << "nan" << ",";
   }
   else
   {
      os << latd() << ",";
   }
   if(isLonNan())
   {
      os << "nan" << ",";
   }
   else
   {
      os << lond() << ",";
   }
   if(isHgtNan())
   {
      os << "nan" << ",";
   }
   else
   {
      // millimeter precision for height
      os << height() << ",";
   }
   
   os << (theDatum?theDatum->code().c_str():"") << ")";
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
      pt.latd(ossim::nan());
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
      pt.lond(ossim::nan());
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
      pt.height(ossim::nan());
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
   
   const ossimDatum* datum = ossimDatumFactoryRegistry::instance()->create(os);
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
//  METHOD: ossimGpt::changeDatum
//*****************************************************************************
void ossimGpt::changeDatum(const ossimDatum *datum)
{
   if (*datum == *theDatum) 
      return;

   // only shift if all values lat and lon is good
   if(!isLatNan() && !isLonNan()) 
   {
      if(datum)
      {
         double h = hgt;
         *this = datum->shift(*this);
         if(ossim::isnan(h))
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

bool ossimGpt::isEqualTo(const ossimGpt& rhs, ossimCompareType compareType)const
{
   bool result = false;
   if(!ossim::isnan(lat)&&!ossim::isnan(rhs.lat))
   {
      result = ossim::almostEqual(lat, rhs.lat);
   }
   else 
   {
      result = ossim::isnan(lat)&&ossim::isnan(rhs.lat);
   }

   if(result)
   {
      if(!ossim::isnan(lon)&&!ossim::isnan(rhs.lon))
      {
         result = ossim::almostEqual(lon, rhs.lon);
      }
      else 
      {
         result = ossim::isnan(lon)&&ossim::isnan(rhs.lon);
      }
   }
   if(result)
   {
      if(!ossim::isnan(hgt)&&!ossim::isnan(rhs.hgt))
      {
         result = ossim::almostEqual(hgt, rhs.hgt);
      }
      else 
      {
         result = ossim::isnan(hgt)&&ossim::isnan(rhs.hgt);
      }
   }
   
   if(result)
   {
      if(theDatum&&rhs.theDatum)
      {
         if(compareType == OSSIM_COMPARE_FULL)
         {
            result = theDatum->isEqualTo(*rhs.theDatum);
         }
         else 
         {
            result = theDatum == rhs.theDatum;
         }

      }
      else if(reinterpret_cast<ossim_uint64>(theDatum)|reinterpret_cast<ossim_uint64>(rhs.theDatum))
      {
         result = false;
      }
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
// METHOD: azimuthTo(ossimGpt)
// Computes the great-circle starting azimuth (i.e., at this gpt) to the argument gpt in degrees.
// In other words, what direction we would need to start walking in to travel the shortest 
// distance to arg_gpt (assumes spherical earth). 
// Taken from American Practical Navigator, a.k.a. Bowditch (NIMA 2002)
//*****************************************************************************
double ossimGpt::azimuthTo(const ossimGpt& gpt) const
{
   using namespace ossim; // for trig functions in degrees

   //### NOT WORKING ###
   //double dlo = fabs(lon - gpt.lon);
   //if (lat * gpt.lat < 0)
   //   dlo *= -1.0;

   //double c = atand(sind(dlo)/((cosd(lat)*tand(gpt.lat)) - (sind(lat)*cosd(dlo))));
   //return c;

   // Use alternate local method (not great circle):
   double mean_lat = 0.5*(lat + gpt.lat);
   double dlon = ossim::cosd(mean_lat)*(gpt.lon - lon);
   double dlat = gpt.lat - lat;
   double theta = ossim::atan2d(dlon,dlat);
   if (theta < 0)
      theta += 360.0;
   return theta;
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
//#define USE_ELLIPTICAL_RADII
#ifdef USE_ELLIPTICAL_RADII
   ossimDpt radii;
   theDatum->ellipsoid()->geodeticRadii(lat, radii);
   return ossimDpt (RAD_PER_DEG * radii.x * ossim::cosd(lat), 
                    RAD_PER_DEG * radii.y);
#else 
   ossimDpt result;

   double radius = theDatum->ellipsoid()->geodeticRadius(lat);
   result.y =  RAD_PER_DEG * radius;
   result.x =  result.y * ossim::cosd(lat);

   return result;
#endif
}

bool ossimGpt::operator==(const ossimGpt& gpt) const
{
   return ( ossim::almostEqual(lat, gpt.lat) &&
            ossim::almostEqual(lon, gpt.lon) &&
            ossim::almostEqual(hgt, gpt.hgt) &&
            (*theDatum == *(gpt.theDatum)));
}

