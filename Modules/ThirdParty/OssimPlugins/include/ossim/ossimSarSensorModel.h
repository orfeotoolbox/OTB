/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimSarSensorModel_HEADER
#define ossimSarSensorModel_HEADER

#include <boost/config.hpp>
#include <string>
#include <stdint.h>
#include <cassert>

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Woverloaded-virtual"
#   pragma GCC diagnostic ignored "-Wshadow"
#   include <ossim/projection/ossimSensorModel.h>
#   include <ossim/elevation/ossimHgtRef.h>
#   include "ossim/ossimTimeUtilities.h"

# pragma GCC diagnostic pop

#else
#   include <ossim/projection/ossimSensorModel.h>
#   include <ossim/elevation/ossimHgtRef.h>
#   include "ossim/ossimTimeUtilities.h"
#endif

#if defined(USE_BOOST_TIME)
#  include <boost/date_time/posix_time/posix_time.hpp>
#include <ostream>
#endif

namespace ossimplugins
{

class OSSIM_PLUGINS_DLL ossimSarSensorModel : public ossimSensorModel
{
public:

   struct ProductType
   : private equality_comparable<ProductType>
   , private less_than_comparable<ProductType>
   {
      enum Type { SLC, GRD, MGD, GEC, EEC, MAX__, UNDEFINED__, FIRST__=0 };

      explicit ProductType(unsigned char value)
         : m_value(Type(value))
         {
            if (value >= MAX__)
               throw std::runtime_error("Invalid Sar Sensor Product Type id");
         }
      ProductType(Type value)
         : m_value(Type(value))
         { assert(m_value < MAX__); }
      ProductType()
         : m_value(UNDEFINED__)
         {}
      ProductType(string_view const& s);
      ProductType& operator++() {
         assert(m_value < MAX__);
         m_value = ProductType::Type(m_value+1);
         return *this;
      }
      ProductType operator++(int) {
         ProductType tmp = *this;
         ++*this;
         return tmp;
      }

      string_view ToString() const;
      Type ToInternal() const { return m_value; }
      friend bool operator< (ProductType lhs, ProductType rhs)
      { return lhs.m_value <  rhs.m_value; }
      friend bool operator==(ProductType lhs, ProductType rhs)
      { return lhs.m_value == rhs.m_value; }
      friend std::ostream & operator<<(std::ostream & os, const ossimSarSensorModel::ProductType & v)
      { return os << v.ToString(); }


      static ProductType Max  ()
      { return ProductType(build_max()); }
      static ProductType begin()
      { return FIRST__; }
      static ProductType end  ()
      { return ProductType(build_max()); }

   private:
      struct build_max {};
      ProductType(build_max) : m_value(MAX__) {}

      Type m_value;
   };

#if defined(USE_BOOST_TIME)
   typedef boost::posix_time::ptime            TimeType;
   typedef boost::posix_time::precise_duration DurationType;
#else
   typedef time::ModifiedJulianDate TimeType;
   typedef time::Duration           DurationType;
#endif

   struct OrbitRecordType
   {
      TimeType        azimuthTime;
      ossimEcefPoint  position;
      ossimEcefVector velocity;
      friend std::ostream & operator<<(std::ostream & os, const OrbitRecordType & v)
      {
         return os << "{ azimuthTime: " << v.azimuthTime
            <<        ", position: "    << v.position
            <<        ", velocity: "    << v.velocity
            <<        "}";
      }
   };

   struct GCPRecordType
   {
      TimeType azimuthTime;
      double   slantRangeTime;
      ossimDpt imPt;
      ossimGpt worldPt;
      friend std::ostream & operator<<(std::ostream & os, const GCPRecordType & v)
      {
         return os << "{ azimuthTime: " << v.azimuthTime
            <<        ", imPt: "       << v.imPt
            <<        ", worldPt: "    << v.worldPt
            <<        "}";
      }
   };

   struct BurstRecordType
   {
      TimeType      azimuthStartTime;
      unsigned long startLine;
      TimeType      azimuthStopTime;
      unsigned long endLine;
      unsigned long startSample;
      unsigned long endSample;
      friend std::ostream & operator<<(std::ostream & os, const BurstRecordType & v)
      {
         return os << "{ azimuthStartTime: " << v.azimuthStartTime
            <<        ", azimuthStopTime: "  << v.azimuthStopTime
            <<        ", startLine: "        << v.startLine
            <<        ", stopLine: "         << v.endLine
            <<        "}";
      }
   };

   struct IsWithin {
      IsWithin(BurstRecordType const& br)
         : m_start(br.azimuthStartTime)
         , m_stop(br.azimuthStartTime)
         {}
      bool operator()(TimeType v) const
      { return m_start <= v && v < m_stop; }
   private:
      TimeType m_start;
      TimeType m_stop;
   };

   struct DoesContain {
      DoesContain(TimeType value)
         : m_value(value)
         {}
      bool operator()(BurstRecordType const& br) const
      { return br.azimuthStartTime <= m_value && m_value < br.azimuthStopTime; }
   private:
      TimeType m_value;
   };

   struct CoordinateConversionRecordType
   {
      TimeType            azimuthTime;
      double              rg0;
      std::vector<double> coefs;
      friend std::ostream & operator<<(std::ostream & os, const CoordinateConversionRecordType & v)
      {
         return os << "{ azimuthTime: " << v.azimuthTime
            <<        ", rg0: "         << v.rg0
            <<        ", coeffs: "      << v.coefs.size()
            <<        "}";
      }
   };

   /** Constructor */
   ossimSarSensorModel();

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
   /** Copy constructor */
   ossimSarSensorModel(ossimSarSensorModel const& m) = default;
   /** Move constructor */
   ossimSarSensorModel(ossimSarSensorModel && m) = default;

   /** Destructor */
   virtual ~ossimSarSensorModel() = default;
#endif

   virtual void lineSampleHeightToWorld(const ossimDpt& imPt, const double & heightEllipsoid, ossimGpt& worldPt) const override;

   virtual void lineSampleToWorld(const ossimDpt& imPt, ossimGpt& worldPt) const override;


   /** This method implement inverse sar geolocation using method found
    *  in ESA document "Guide to ASAR geocoding" (ref
    *  RSL-ASAR-GC-AD). Equation numbers can be found in source code
    *  comments.
    *
    * \param[in] worldPt World point to geocode
    * \param[out] imPt Corresponding estimated image point
    */
   virtual void worldToLineSample(const ossimGpt& worldPt, ossimDpt & imPt) const override;

   /**
    * Sub-routine of lineSampleToWorld that computes azimuthTime and
    * slant range time from worldPoint
    *
    * \param[in] worldPoint World point to geocode
    * \param[out] azimuthTime Estimated zero-doppler azimuth time
    * \param[out] rangeTime Estimated range time
    * \param[out] interpSensorPos interpolated ECEF sensor position
    * \param[out] interpSensorVel interpolated ECEF sensor velocity
    * \return True if success, false otherwise. In this case,
    * azimuthTime and rangeTime will not be modified.
    */
   /*virtual*/ bool worldToAzimuthRangeTime(const ossimGpt& worldPt, TimeType & azimuthTime, double & rangeTime, ossimEcefPoint & interpSensorPos, ossimEcefVector & interpSensorVel) const;

   /**
    * This method implement inverse sar geolocation similar to
    * worldToLineSample, except that it also returns (y,z)
    * coordinates, defined as follows:
    * Let n = |sensorPos|,
    * ps2 = scalar_product(sensorPos,worldPoint)
    * d = distance(sensorPos,worldPoint)
    *
    * z = n - ps2/n
    * y = sqrt(d*d - z*z)
    * 
    * sign of y is furher adapted to be inverted if sensor is left or
    * right looking 
    *
    * \param[in] worldPoint World point to geocode
    * \param[out] imPt Corresponding estimated image point
    * \param[out] y 
    * \param[out] z 
    * \return True if success, false otherwise. In this case,
    */
   void worldToLineSampleYZ(const ossimGpt& worldPt, ossimDpt& imPt, double & y, double & z) const;
   
   void lineSampleToAzimuthRangeTime(const ossimDpt & imPt, TimeType & azimuthTime, double & rangeTime) const;

   // TODO: document me
   bool autovalidateInverseModelFromGCPs(const double & xtol = 1, const double & ytol = 1, const double azTimeTol = 500, const double &rangeTimeTo=0.0000000001) const;

   // TODO: document me
   bool autovalidateForwardModelFromGCPs(double resTol = 25);

   //Pure virtual in base class
   bool useForward() const override;

   void optimizeTimeOffsetsFromGcps();

   /**
    * This method will perform a deburst operation, and return the
    * a vector of lines range of lines to keep in the image file.
    * Note that the deburst operation has no effect if theBurstRecords
    * contains a single burst. Otherwise it will merge burst together
    * into a single burst, and update GCPs accordingly.
    * \return true if the deburst operation succeeded. No changes is
    * made to the object if the operation fails.
    * \param lines A container for the lines ranges to keep in the
    * deburst image.
    */
   bool deburst(std::vector<std::pair<unsigned long,unsigned long> >& lines);

   /**
    * This is a helper function to convert image line to deburst image
    * line.
    * \param lines The vector of lines range to keep
    * \param imageLine The input image line
    * \param deburstLine The output deburst line
    * \return True if imageLine is within a preserved range, false otherwise
    */
   static bool imageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long imageLine, unsigned long & deburstLine);

   /** 
    * This is a helper function to convert deburst line to input image
    * line
    * \param lines The vector of lines range to keep
    * \param imageLine The input deburst line
    * \param deburstLine The output original image line
    */
   static void deburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long deburstLine, unsigned long & imageLine);

   /**
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const override;

   //TODO: Add virtual method readAnnotationFile?

   virtual bool saveState(ossimKeywordlist      & kwl, const char* prefix=NULL) const override;
   virtual bool loadState(ossimKeywordlist const& kwl, const char* prefix=NULL) override;

   bool isGRD() const {
      switch (theProductType.ToInternal()) {
         case ProductType::GRD:
         case ProductType::MGD:
         case ProductType::GEC:
         case ProductType::EEC:
            return true;
         default:
            return false;
      }
   }

   virtual std::ostream& print(std::ostream& out) const override;
protected:
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
   TYPE_DATA
#pragma clang diagnostic pop
#else
   TYPE_DATA
#endif

   /**
    * Compute range and doppler frequency from an input point, sensor
    * position and velocity.
    *
    * \param[in] inputPt The target point
    * \param[in] sensorPos The sensor position
    * \param[in] sensorvel The sensor velocity
    * \param[out] range Estimated range
    * \param[out] doppler Estimated doppler frequency
    */
   virtual void computeRangeDoppler(const ossimEcefPoint & inputPt, const ossimEcefPoint & sensorPos, const ossimEcefVector sensorVel, double & range, double & doppler) const;

   /**
    * Interpolate sensor position and velocity at given azimuth time
    * using lagragian interpolation of orbital records.
    *
    * \param[in] azimuthTime The time at which to interpolate
    * \param[out] sensorPos Interpolated sensor position
    * \param[out] sensorvel Interpolated sensor velocity
    * \param[in] deg Degree of lagragian interpolation
    */
   /*virtual*/ void interpolateSensorPosVel(const TimeType & azimuthTime, ossimEcefPoint& sensorPos, ossimEcefVector& sensorVel, unsigned int deg = 8) const;

   /**
    * Convert slant range to ground range by interpolating slant range
    * to ground range coefficients.
    *
    * \param[in] slantRangeTime The slantRange to convert (meters)
    * \param[in] azimuthTime The corresponding azimuth time
    * \param[out] groundRange The estimated ground range (meters)
    */
   /*virtual*/ void slantRangeToGroundRange(const double & slantRange, const TimeType & azimuthTime, double & groundRange) const;

   // TODO: Document me
   /*virtual*/ void groundRangeToSlantRange(const double & groundRange, const TimeType & azimuthTime, double & slantRange) const;

   // TODO: Document me
   /*virtual*/ void applyCoordinateConversion(const double & in, const TimeType& azimuthTime, const std::vector<CoordinateConversionRecordType> & records, double & out) const;
   /**
    * Estimate the zero-doppler azimuth time and corresponding sensor
    * position and velocity from the inputPt.
    *
    * \param[in] inputPt The point to estimated zero-doppler time on
    * \param[out] interpAzimuthTime Interpolated azimuth time
    * \param[out] interpSensorPos Interpolated sensor position
    * \param[out] interpSensorVel Interpolated sensor velocity
    * \return True if success, false otherwise. In this case, output
    * parameters are left untouched.
    */
   /*virtual*/ bool zeroDopplerLookup(const ossimEcefPoint & inputPt, TimeType & interpAzimuthTime, ossimEcefPoint & interpSensorPos, ossimEcefVector & interpSensorVel) const;

   /**
    * Compute the bistatic correction to apply.
    *
    * \param[in] inputPt The point to compute bistatic correction on
    * \param[in] sensorPos The corresponding sensor position
    * \param[out] bistaticCorrection The estimated bistatic correction
    */
   /*virtual*/ void computeBistaticCorrection(const ossimEcefPoint & inputPt, const ossimEcefPoint & sensorPos, DurationType & bistaticCorrection) const;

   /**
    * Convert azimuth time to fractional line.
    *
    * \param[in] azimuthTime The azimuth time to convert
    * \param[out] The estimated fractional line
    */
   /*virtual*/ // TODO: check why virtual
   void azimuthTimeToLine(const TimeType & azimuthTime, double & line) const;

   // TODO: document me
   /*virtual*/ // TODO: check why virtual
   void lineToAzimuthTime(const double & line, TimeType & azimuthTime) const;

   // TODO: document me
   /*virtual*/ // TODO: check why virtual
   bool projToSurface(const GCPRecordType & initGcp, const ossimDpt & target, const ossimHgtRef & hgtRef, ossimEcefPoint & ellpt) const;

   /**
    * Finds closest GCP.
    *
    * \param[in] imPt  «imPt-explanations»
    * \return the closest GCP record to \c imPt.
    * \throw None
    * \pre `theGCPRecords` shall not be empty.
    */
   GCPRecordType const& findClosestGCP(ossimDpt const& imPt) const;


   std::vector<OrbitRecordType>                theOrbitRecords;
   std::vector<GCPRecordType>                  theGCPRecords;
   std::vector<BurstRecordType>                theBurstRecords;
   std::vector<CoordinateConversionRecordType> theSlantRangeToGroundRangeRecords;
   std::vector<CoordinateConversionRecordType> theGroundRangeToSlantRangeRecords;

   double                                      theRadarFrequency; // in Hz
   DurationType                                theAzimuthTimeInterval;
   double                                      theNearRangeTime; // in seconds
   double                                      theRangeSamplingRate; // in Hz
   double                                      theRangeResolution; // in meters
   bool                                        theBistaticCorrectionNeeded; // Do we need to compute
   // bistatic correction ?
   ProductType                                 theProductType; // GRD/SLC
   DurationType                                theAzimuthTimeOffset; // Offset computed
   double                                      theRangeTimeOffset; // Offset in seconds, computed
   bool                                        theRightLookingFlag;
   
   static const double C;

   static const unsigned int thePluginVersion; // version of the SarSensorModel plugin
   static const unsigned int thePluginVersionMin; // minimal version required of the SarSensorModel plugin

private:
   /** Disabled assignment operator.  */
   ossimSarSensorModel& operator=(ossimSarSensorModel const& rhs);
   // Disable a warning, and disable inherited buggy assignment (buggy because
   // value semantics are incompatible with public inheritance thanks to
   // slicing)
   using ossimSensorModel::operator=;
   using ossimOptimizableProjection::operator=;
};

}

#endif
