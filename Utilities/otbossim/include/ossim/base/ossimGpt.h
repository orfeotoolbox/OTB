//*******************************************************************
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts
//
// Description:
//
// Contains class declaration for gpt.
//
// CONSTRUCTOR TAKES LAT/LON IN DEGREES!!!
//
// Height is relative to the ellipsoid in meters.
//
//*******************************************************************
//  $Id: ossimGpt.h 19900 2011-08-04 14:19:57Z dburken $

#ifndef ossimGpt_HEADER
#define ossimGpt_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimString.h>
#include <iosfwd>

class  ossimDatum;
class  ossimEcefPoint;

class OSSIM_DLL ossimGpt
{
public:
   /**
    * Constructor.  The values are assumed to be in DEGREES.
    */
   ossimGpt(const double alat=0,  // degrees
            const double alon=0,  // degrees
            const double ahgt=0,
            const ossimDatum* aDatum=ossimDatumFactory::instance()->wgs84())
      : lat(alat),
        lon(alon),
        hgt(ahgt),  // relative to the ellipsoid
        theDatum(aDatum) {}//limitLonTo180();}

   /**
    * Copy Constructor:
    */
   ossimGpt(const ossimGpt& src);
   
   /**
    * Constructor.  Conversion from geocentric to ground.
    */
   ossimGpt(const ossimEcefPoint &aPt,
            const ossimDatum* aDatum=ossimDatumFactory::instance()->wgs84());

   /**
    * latr().  Returns the latitude in radian measure.  
    */
   double latr()const{return lat*RAD_PER_DEG;}

   /**
    * Returns the latitude in radian measure.  
    */
   void latr(double radianValue){lat = radianValue*DEG_PER_RAD;}

   /**
    * Returns the longitude in radian measure.  
    */
   double lonr()const{return lon*RAD_PER_DEG;}

   /**
    * Assumes the value being passed in is in radians.
    */
   void lonr(double radianValue)
      {lon = radianValue*DEG_PER_RAD; }//limitLonTo180();}

   /**
    * Will convert the radian measure to degrees.
    */
   double latd()const{return lat;}

   /**
    * Assumes the passed in value is in degrees.
    */
   void latd(double degreeValue){lat = degreeValue;}

   /**
    * Will convert the radian measure to degrees.
    */
   double lond()const{return lon;}

   /**
    *  Assumes the passed in value is in degrees.
    */
   void lond(double degreeValue){lon = degreeValue; }//limitLonTo180();}

   /**
    * @return Returns the height in meters above the ellipsoid.
    */
   double height()const{return hgt;}

   /**
    * @return Returns the height in meters above mean sea level (msl).
    * 
    * @note This is the height above the ellipsoid minus any geoid offset.
    */
   double heightMSL() const;

   /**
    * Sets the "hgt" data member to height.
    *
    * @param height Height above the ellipsoid in meters.
    */
   void height(double height){hgt = height;}

   /**
    * Sets the "hgt" data member to heightMsl adding any geiod offset.
    *
    * @param heightMSL Height in meters above msl.
    */
   void heightMSL(double heightMSL);

   void makeNan(){lat=ossim::nan(); lon=ossim::nan(); hgt=ossim::nan();}
   bool isNan()const
   {
      return (ossim::isnan(lat)&&ossim::isnan(lon)&&ossim::isnan(hgt));
   }
   bool hasNans()const
   {
      return (ossim::isnan(lat)||ossim::isnan(lon)||ossim::isnan(hgt));
   }
   bool isLatNan()const{return ossim::isnan(lat);}
   bool isLonNan()const{return ossim::isnan(lon);}
   bool isHgtNan()const{return ossim::isnan(hgt);}

   std::ostream& print(std::ostream& os, ossim_uint32 precision=15) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimGpt& pt);
   
   /**
    * @param precision Output floating point precision.
    * 
    * @return ossimString representing point.
    *
    * Output format:
    * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
    *   -----latitude----  -----longitude----  ------height----  datum
    */
   ossimString toString(ossim_uint32 precision=15) const;

   /**
    * Initializes this point from string.  This method opens an istream to
    * s and then calls operator>>.
    *
    * Expected format:
    * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
    *  -----latitude----  -----longitude----  ------height----  datum
    *
    * @param s String to initialize from.
    *
    * @see operator>>
    */
   void toPoint(const std::string& s);
   
   /**
    * Method to input the formatted string of the "operator<<".
    * 
    * This method starts by doing a "makeNan" on aPt.  So if anything goes
    * wrong with the stream or parsing aPt could be all or partially nan.
    *
    * @param is Input stream istream to formatted text.
    * @param aPt osimGpt to be initialized from stream.
    * @return istream pass in.
    *
    * Expected format:
    * ( 30.00000000000000, -90.00000000000000, 0.00000000000000, WGE )
    *  -----latitude----  -----longitude----  ------height----  datum
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimGpt& pt);

   /**
    * datum().  returns the datum associated with this ground.
    *
    */
   const ossimDatum* datum()const{return theDatum;}

   /**
    * Note: this will not do a shift.  This just allows you to set the datum.
    *       If you want an automatic shift to occur then you must call the
    *       changeDatum method
    */
   void datum(const ossimDatum* aDatum){theDatum = aDatum?aDatum:theDatum;}

   /**
    * This will actually perform a shift.
    */
   void changeDatum(const ossimDatum *datum);

   const ossimGpt& operator = (const ossimGpt &aPt);
   bool operator ==(const ossimGpt& gpt)const;

   bool operator != (const ossimGpt& gpt) const { return !(*this == gpt); }
   
   /**
    * METHOD: limitLonTo180()
    * Converts the lon data member to a value between -180 and +180:
    */
   void  limitLonTo180()
   { if (lon <= -180.0) lon += 360.0; else if (lon > 180.0) lon -= 360.0; }

   /**
    * @brief Wrap method to maintain longitude between -180 and +180 and latitude between
    * -90 and +90.  Inlined below.
    */
   void wrap();

   void clampLon(double low, double high)
      {
         if(lon < low) lon = low;
         if(lon > high) lon = high;
      }

   void clampLat(double low, double high)
      {
         if(lat < low) lat = low;
         if(lat > high) lat = high;
      }

   void clampHgt(double low, double high)
      {
         if(hgt < low) hgt = low;
         if(hgt > high) hgt = high;
      }
      
   /**
    * METHOD: distanceTo(ossimGpt)
    * Computes straight-line distance in meters between this and arg gpt:
    */
   double distanceTo(const ossimGpt& arg_gpt) const;

   /**
   * METHOD: azimuthTo(ossimGpt)
   * Computes the great-circle starting azimuth (i.e., at this gpt) to the argument gpt in degrees.
   * In other words, what direction we would need to start walking in to travel the shortest 
   * distance to arg_gpt (assumes spherical earth)
   */
   double azimuthTo(const ossimGpt& arg_gpt) const;

   ossimDpt metersPerDegree() const;
   
   ossimString toDmsString()const;

   bool isEqualTo(const ossimGpt& rhs, ossimCompareType compareType=OSSIM_COMPARE_FULL)const;
   ossim_float64 lat; //> latitude in degrees measure
   ossim_float64 lon; //> longitude in degrees measure

   /**
    * Height in meters above the ellipsiod.
    *
    * @note This is NOT the same as "height msl".  "Height msl" is above
    * the geoid or better know as mean sea level.
    */ 
   ossim_float64 hgt;

private:
   
   /**
    * Know reference location plus an implied ellipsoid.
    */
   const ossimDatum* theDatum;
 
};

inline const ossimGpt& ossimGpt::operator=(const ossimGpt& aPt)
{
   if ( this != &aPt )
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
   }
   return *this;
}

inline void ossimGpt::wrap()
{
   if ( lon > 180.0 )
   {
      do
      {
         lon = lon - 360.0;
      } while ( lon > 180.0 );
   }
   else if ( lon < -180.0  )
   {
      do
      {
         lon = lon + 360.0;
      } while ( lon < -180.0 );
   }
   if ( lat > 90.0 )
   {
      if ( lat > 360.0 ) // Remove total wraps.
      {
         do
         {
            lat = lat - 360.0;
         } while ( lat > 360.0);
      }
      if ( lat > 270.0 ) // Between 270 and 360.
      {
         lat = lat - 360.0;
      }
      else if ( lat > 90 ) // Between 90 and 270.
      {
         lat = 180.0 - lat;
      }
   }
   else if ( lat < -90.0  )
   {
      if ( lat < -360.0 ) // Remove total wraps.
      {
         do
         {
            lat = lat + 360.0;
         } while ( lat < -360.0);
      }
      if ( lat < -270.0 ) 
      {
         lat = 360.0 + lat; // Between -270 and -360;
      }
      else if ( lat < -90.0 )
      {
         lat = -180.0 - lat;
      }
   }
}

#endif /* #ifndef ossimGpt_HEADER */
