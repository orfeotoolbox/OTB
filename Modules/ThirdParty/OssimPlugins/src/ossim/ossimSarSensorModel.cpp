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
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "ossim/ossimSarSensorModel.h"
#include "ossim/ossimKeyWordListUtilities.h"
#include "ossim/ossimTraceHelpers.h"
#include "ossim/ossimRangeUtilities.h"
#include "ossim/ossimSarSensorModelPathsAndKeys.h"
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <boost/static_assert.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#if defined(USE_BOOST_TIME)
using boost::posix_time::microseconds;
using boost::posix_time::seconds;
#else
using ossimplugins::time::microseconds;
using ossimplugins::time::seconds;
#endif

namespace {// Anonymous namespace
   const bool         k_verbose = false; // global verbose constant; TODO: use an option

   // Sometimes, we don't need to compare the actual distance, its square value is
   // more than enough.
   inline double squareDistance(ossimDpt const& lhs, ossimDpt const& rhs) {
      const double dx = lhs.x - rhs.x;
      const double dy = lhs.y - rhs.y;
      return dx*dx + dy*dy;
   }

   inline double squareDistance(ossimGpt const& lhs, ossimGpt const& rhs) {
      const ossimEcefPoint l(lhs);
      const ossimEcefPoint r(rhs);
      return (l-r).norm2();
   }

   template <typename Container>
      inline void unzip(Container const& in, Container& out_even, Container& out_odd)
      {
         typedef typename Container::const_iterator const_iterator;
         typedef typename Container::size_type      size_type;

         const size_type size                         = in.size();
         const bool      has_a_odd_number_of_elements = size % 2 == 1;

         out_even.reserve(size/2+1);
         out_odd.reserve(size/2);

         const_iterator end = in.end();
         if (has_a_odd_number_of_elements)
         {
            std::advance(end, -1);
         }
         for (const_iterator it=in.begin(); it != end ; )
         {
            out_even.push_back(*it++);
            out_odd.push_back(*it++);
         }
         if (has_a_odd_number_of_elements)
         {
            assert(end != in.end());
            out_even.push_back(*end);
         }
         assert(out_even.size() >= out_odd.size());
         assert(out_odd.capacity() == size/2); // The correct number of element have been reserved
         assert(out_even.capacity() == size/2+1); // The correct number of element have been reserved
         assert(out_odd.size() + out_even.size() == size);
      }

   ossimTrace traceExec  ("ossimSarSensorModel:exec");
   ossimTrace traceDebug ("ossimSarSensorModel:debug");

   typedef char const* const* strings_iterator;
   static char const* const PRODUCTTYPE_STRINGS[] = { "SLC", "GRD", "MGD", "GEC", "EEC" };
}// Anonymous namespace

namespace ossimplugins
{
   RTTI_DEF1(ossimSarSensorModel, "ossimSarSensorModel", ossimSensorModel);

   const double ossimSarSensorModel::C = 299792458;

   const unsigned int ossimSarSensorModel::thePluginVersion = 3;
  const unsigned int ossimSarSensorModel::thePluginVersionMin = 2;

   ossimSarSensorModel::ProductType::ProductType(string_view const& s)
   {
      using ossimplugins::begin;
      using ossimplugins::end;
      strings_iterator const ProductType_it = std::find(begin(::PRODUCTTYPE_STRINGS),end(::PRODUCTTYPE_STRINGS), s);
      if (ProductType_it == end(::PRODUCTTYPE_STRINGS))  {
         throw std::runtime_error("Invalid Sar Sensor Product type: `"+s+"'");
      }
      m_value = Type(std::distance(begin(::PRODUCTTYPE_STRINGS), ProductType_it));
      assert(m_value < MAX__);
   }

   string_view ossimSarSensorModel::ProductType::ToString() const
   {
      BOOST_STATIC_ASSERT((MAX__ == array_size(::PRODUCTTYPE_STRINGS)));
      assert(m_value != UNDEFINED__); // Yes, I know UNDEFINED__ > MAX__
      assert(m_value < MAX__);
      return PRODUCTTYPE_STRINGS[m_value];
   }

   ossimSarSensorModel::ossimSarSensorModel()
      : theRadarFrequency(0.),
      theAzimuthTimeInterval(seconds(0)),
      theNearRangeTime(0.),
      theRangeSamplingRate(0.),
      theRangeResolution(0.),
      theBistaticCorrectionNeeded(false),
      theAzimuthTimeOffset(seconds(0)),
      theRangeTimeOffset(0.),
      theRightLookingFlag(true),
      redaptMedataAfterDeburst(false)
      {}

   ossimSarSensorModel::GCPRecordType const&
      ossimSarSensorModel::findClosestGCP(ossimDpt const& imPt) const
      {
         assert(!theGCPRecords.empty()&&"theGCPRecords is empty.");

         // Find the closest GCP
         double distance2 = squareDistance(imPt, theGCPRecords.front().imPt);

         std::vector<GCPRecordType>::const_iterator refGcp = theGCPRecords.begin();
         std::vector<GCPRecordType>::const_iterator gcpIt  = theGCPRecords.begin();
         std::vector<GCPRecordType>::const_iterator gcpEnd = theGCPRecords.end();
         for(++gcpIt ; gcpIt!=gcpEnd ; ++gcpIt)
         {
            const double currentDistance2 = squareDistance(imPt, gcpIt->imPt);

            if(currentDistance2 < distance2)
            {
               distance2 = currentDistance2;
               refGcp = gcpIt;
            }
         }

         assert(refGcp != theGCPRecords.end() && "A GCP record shall have been found!");
         return *refGcp;
      }

   void ossimSarSensorModel::lineSampleHeightToWorld(const ossimDpt& imPt, const double & heightAboveEllipsoid, ossimGpt& worldPt) const
   {
      // std::clog << "ossimSarSensorModel::lineSampleHeightToWorld()\n";
      assert(!theGCPRecords.empty()&&"theGCPRecords is empty.");

      GCPRecordType const& refGcp = findClosestGCP(imPt);

      // Set the height reference
      ossim_float64 hgtSet;
      if ( ossim::isnan(heightAboveEllipsoid) )
      {
         hgtSet = refGcp.worldPt.height();
      }
      else
      {
         hgtSet = heightAboveEllipsoid;
      }
      const ossimHgtRef hgtRef(AT_HGT, hgtSet);

      ossimEcefPoint ellPt;

      // Simple iterative inversion of inverse model starting at closest gcp
      projToSurface(refGcp,imPt,hgtRef,ellPt);

      worldPt = ossimGpt(ellPt);
   }

   void ossimSarSensorModel::lineSampleToWorld(ossimDpt const& imPt, ossimGpt& worldPt) const
   {
      // std::clog << "ossimSarSensorModel::lineSampleToWorld()\n";
      assert(!theGCPRecords.empty()&&"theGCPRecords is empty.");

      GCPRecordType const& refGcp = findClosestGCP(imPt);
      ossimGpt      const& refPt = refGcp.worldPt;

      const ossimHgtRef hgtRef(AT_DEM);

      ossimEcefPoint ellPt;

      // Simple iterative inversion of inverse model starting at closest gcp
      projToSurface(refGcp,imPt,hgtRef,ellPt);

      worldPt = ossimGpt(ellPt);
   }

   void ossimSarSensorModel::worldToLineSample(const ossimGpt& worldPt, ossimDpt & imPt) const
   {
      // std::clog << "ossimSarSensorModel::worldToLineSample()\n";
      assert(theRangeResolution>0&&"theRangeResolution is null.");

      // First compute azimuth and range time
      TimeType azimuthTime;
      double rangeTime;

      ossimEcefPoint sensorPos;
      ossimEcefVector sensorVel;

      const bool success = worldToAzimuthRangeTime(worldPt, azimuthTime, rangeTime, sensorPos, sensorVel);

      if(!success)
      {
         imPt.makeNan();
         return;
      }
      // std::clog << "AzimuthTime: " << azimuthTime << "\n";
      // std::clog << "RangeTime: " << rangeTime << "\n";
      // std::clog << "GRD: " << isGRD() << "\n";

      // Convert azimuth time to line
      azimuthTimeToLine(azimuthTime,imPt.y);

      if(isGRD())
      {
         // GRD case
         double groundRange(0);
         slantRangeToGroundRange(rangeTime*C/2,azimuthTime,groundRange);
         // std::clog << "GroundRange: " << groundRange << "\n";
         // std::clog << "TheRangeResolution: " << theRangeResolution << "\n";

         // Eq 32 p. 31
         // TODO: possible micro-optimization: precompute 1/theRangeResolution, and
         // use *
         imPt.x = groundRange/theRangeResolution;
      }
      else
      {
         // std::clog << "TheNearRangeTime: " << theNearRangeTime << "\n";
         // std::clog << "TheRangeSamplingRate: " << theRangeSamplingRate << "\n";
         // SLC case
         // Eq 23 and 24 p. 28
         imPt.x = (rangeTime - theNearRangeTime)*theRangeSamplingRate;
      }
   }

void ossimSarSensorModel::worldToLineSampleYZ(const ossimGpt& worldPt, ossimDpt & imPt, double & y, double & z) const
   {
      // std::clog << "ossimSarSensorModel::worldToLineSample()\n";
      assert(theRangeResolution>0&&"theRangeResolution is null.");

      // First compute azimuth and range time
      TimeType azimuthTime;
      double rangeTime;
      ossimEcefPoint sensorPos;
      ossimEcefVector sensorVel;

      const bool success = worldToAzimuthRangeTime(worldPt, azimuthTime, rangeTime,sensorPos,sensorVel);

      if(!success)
      {
         imPt.makeNan();
         return;
      }
      // std::clog << "AzimuthTime: " << azimuthTime << "\n";
      // std::clog << "RangeTime: " << rangeTime << "\n";
      // std::clog << "GRD: " << isGRD() << "\n";

      // Convert azimuth time to line
      azimuthTimeToLine(azimuthTime,imPt.y);

      if(isGRD())
      {
         // GRD case
         double groundRange(0);
         slantRangeToGroundRange(rangeTime*C/2,azimuthTime,groundRange);
         // std::clog << "GroundRange: " << groundRange << "\n";
         // std::clog << "TheRangeResolution: " << theRangeResolution << "\n";

         // Eq 32 p. 31
         // TODO: possible micro-optimization: precompute 1/theRangeResolution, and
         // use *
         imPt.x = groundRange/theRangeResolution;
      }
      else
      {
         // std::clog << "TheNearRangeTime: " << theNearRangeTime << "\n";
         // std::clog << "TheRangeSamplingRate: " << theRangeSamplingRate << "\n";
         // SLC case
         // Eq 23 and 24 p. 28
         imPt.x = (rangeTime - theNearRangeTime)*theRangeSamplingRate;
      }

      // Now computes Y and Z
      ossimEcefPoint inputPt(worldPt);
      double NormeS = sqrt(sensorPos[0]*sensorPos[0] + sensorPos[1]*sensorPos[1] + sensorPos[2]*sensorPos[2]);  /* distance du radar */                                                                           
      double PS2 = inputPt[0]*sensorPos[0] + inputPt[1]*sensorPos[1] + inputPt[2]*sensorPos[2];

      // TODO check for small NormesS to avoid division by zero ?
      // Should never happen ...
      assert(NormeS>1e-6);
      z = NormeS - PS2/NormeS;

      double distance = sqrt((sensorPos[0]-inputPt[0])*(sensorPos[0]-inputPt[0]) + 
                             (sensorPos[1]-inputPt[1])*(sensorPos[1]-inputPt[1]) + 
                             (sensorPos[2]-inputPt[2])*(sensorPos[2]-inputPt[2]));  

      y = sqrt(distance*distance - z*z);

      // Check view side and change sign of Y accordingly
      if ( (( sensorVel[0] * (sensorPos[1]* inputPt[2] - sensorPos[2]* inputPt[1]) +
              sensorVel[1] * (sensorPos[2]* inputPt[0] - sensorPos[0]* inputPt[2]) +
              sensorVel[2] * (sensorPos[0]* inputPt[1] - sensorPos[1]* inputPt[0])) > 0) ^ theRightLookingFlag )
        {
        y = -y;
        }
   }



bool ossimSarSensorModel::worldToAzimuthRangeTime(const ossimGpt& worldPt, TimeType & azimuthTime, double & rangeTime, ossimEcefPoint & interpSensorPos, ossimEcefVector & interpSensorVel) const
   {
      // std::clog << "ossimSarSensorModel::worldToAzimuthRangeTime()\n";
      // First convert lat/lon to ECEF
      ossimEcefPoint inputPt(worldPt);

      // Compute zero doppler time
      TimeType interpTime;

      const bool success = zeroDopplerLookup(inputPt,azimuthTime,interpSensorPos,interpSensorVel);

      if(!success)
      {
         // TODO: check whether we could throw instead
         return false;
      }

      if(theBistaticCorrectionNeeded)
      {
         // Compute bistatic correction if needed
         DurationType bistaticCorrection;
         computeBistaticCorrection(inputPt,interpSensorPos,bistaticCorrection);

         // Update interpolated azimuth time
         azimuthTime += bistaticCorrection;

         // Update sensor position and velocity
         interpolateSensorPosVel(interpTime,interpSensorPos,interpSensorVel);
      }

      // rangeTime is the round-tripping time to target
      const double rangeDistance = (interpSensorPos-inputPt).magnitude();
      rangeTime = theRangeTimeOffset + 2*rangeDistance/C;

      return true;
   }

   void ossimSarSensorModel::lineSampleToAzimuthRangeTime(const ossimDpt & imPt, TimeType & azimuthTime, double & rangeTime) const
   {
      // std::clog << "ossimSarSensorModel::lineSampleToAzimuthRangeTime()\n";
      // First compute azimuth time here
      lineToAzimuthTime(imPt.y,azimuthTime);

      // Then compute range time
      if(isGRD())
      {
         // Handle grd case here
         double slantRange;
         groundRangeToSlantRange(imPt.x*theRangeResolution,azimuthTime, slantRange);
         rangeTime = theRangeTimeOffset + 2*slantRange/C;
      }
      else
      {
         rangeTime = theRangeTimeOffset + theNearRangeTime + imPt.x*(1/theRangeSamplingRate);
      }
   }

   void ossimSarSensorModel::computeRangeDoppler(const ossimEcefPoint & inputPt, const ossimEcefPoint & sensorPos, const ossimEcefVector sensorVel, double & range, double & doppler) const
   {
      // std::clog << "ossimSarSensorModel::computeRangeDoppler()\n";
      assert(theRadarFrequency>0&&"theRadarFrequency is null");

      // eq. 19, p. 25
      const ossimEcefVector s2gVec = inputPt - sensorPos;

      range = s2gVec.magnitude();

      const double coef = -2*C/(theRadarFrequency*range);

      doppler = coef * sensorVel.dot(s2gVec);
   }

   void ossimSarSensorModel::interpolateSensorPosVel(const TimeType & azimuthTime, ossimEcefPoint& sensorPos, ossimEcefVector& sensorVel, unsigned int deg) const
   {
      assert(!theOrbitRecords.empty()&&"The orbit records vector is empty");

      // Lagrangian interpolation of sensor position and velocity

      unsigned int nBegin(0), nEnd(0);

      sensorPos[0] = 0;
      sensorPos[1] = 0;
      sensorPos[2] = 0;

      sensorVel[0] = 0;
      sensorVel[1] = 0;
      sensorVel[2] = 0;

      // First, we search for the correct set of record to use during
      // interpolation

      // If there are less records than degrees, use them all
      if(theOrbitRecords.size()<deg)
      {
         nEnd = theOrbitRecords.size()-1;
      }
      else
      {
         // Search for the deg number of records around the azimuth time
         unsigned int t_min_idx = 0;
         DurationType t_min = abs(azimuthTime - theOrbitRecords.front().azimuthTime);

         unsigned int count = 0;

         for(std::vector<OrbitRecordType>::const_iterator it = theOrbitRecords.begin();it!=theOrbitRecords.end();++it,++count)
         {
            const DurationType current_time = abs(azimuthTime-it->azimuthTime);

            if(t_min > current_time)
            {
               t_min_idx = count;
               t_min = current_time;
            }
         }
         // TODO: see if these expressions can be simplified
         nBegin = std::max((int)t_min_idx-(int)deg/2+1,(int)0);
         nEnd = std::min(nBegin+deg-1,(unsigned int)theOrbitRecords.size());
         nBegin = nEnd<theOrbitRecords.size()-1 ? nBegin : nEnd-deg+1;
      }

      // Compute lagrangian interpolation using records from nBegin to nEnd
      for(unsigned int i = nBegin; i < nEnd; ++i)
      {
         double w = 1.;

         unsigned int j = nBegin;
         for( ; j != i ; ++j)
         {
            const DurationType td1 = azimuthTime                    - theOrbitRecords[j].azimuthTime;
            const DurationType td2 = theOrbitRecords[i].azimuthTime - theOrbitRecords[j].azimuthTime;
            const double f = td1 / td2;
            w *= f;
         }
         ++j;
         for( ; j < nEnd; ++j)
         {
            const DurationType td1 = azimuthTime                    - theOrbitRecords[j].azimuthTime;
            const DurationType td2 = theOrbitRecords[i].azimuthTime - theOrbitRecords[j].azimuthTime;
            const double f = td1 / td2;
            w *= f;
         }

         sensorPos[0]+=w*theOrbitRecords[i].position[0];
         sensorPos[1]+=w*theOrbitRecords[i].position[1];
         sensorPos[2]+=w*theOrbitRecords[i].position[2];

         sensorVel[0]+=w*theOrbitRecords[i].velocity[0];
         sensorVel[1]+=w*theOrbitRecords[i].velocity[1];
         sensorVel[2]+=w*theOrbitRecords[i].velocity[2];
      }
   }

   void ossimSarSensorModel::slantRangeToGroundRange(const double & slantRange, const TimeType & azimuthTime, double & groundRange) const
   {
      applyCoordinateConversion(slantRange,azimuthTime,theSlantRangeToGroundRangeRecords,groundRange);
   }

   void ossimSarSensorModel::groundRangeToSlantRange(const double & groundRange, const TimeType & azimuthTime, double & slantRange) const
   {
      applyCoordinateConversion(groundRange,azimuthTime,theGroundRangeToSlantRangeRecords,slantRange);
   }

   void ossimSarSensorModel::applyCoordinateConversion(const double & in, const TimeType& azimuthTime, const std::vector<CoordinateConversionRecordType> & records, double & out) const
   {
      assert(!records.empty()&&"The records vector is empty.");
      // std::clog << "conv coord(" << in << ", az="<<azimuthTime<<")\n";

      // First, we need to find the correct pair of records for interpolation
      std::vector<CoordinateConversionRecordType>::const_iterator it = records.begin();

      CoordinateConversionRecordType srgrRecord;

      std::vector<CoordinateConversionRecordType>::const_iterator  previousRecord = it;
      ++it;

      std::vector<CoordinateConversionRecordType>::const_iterator nextRecord = it;

      // Look for the correct record
      // std::clog << "Looking for " << azimuthTime << " within records:\n";
      while(it!=records.end())
      {
         // std::clog << "- record: " << it->azimuthTime << "...";
         // nextRecord = it;

         if(azimuthTime >= previousRecord->azimuthTime
               && azimuthTime < nextRecord->azimuthTime)
         {
            // std::clog << " found!\n";
            break;
         }
         else
         {
            previousRecord = nextRecord;
            ++it;
            nextRecord = it;
            // std::clog << " NOT found => next!\n";
         }
      }
      assert(nextRecord == it);
      if(it == records.end())
      {
         if(azimuthTime < records.front().azimuthTime)
         {
            srgrRecord = records.front();
            // std::clog << "Not found, but before first => srgrRecord: " << srgrRecord.azimuthTime << "\n";
         }
         else if(azimuthTime >= records.back().azimuthTime)
         {
            srgrRecord = records.back();
            // std::clog << "Not found, but after last => srgrRecord: " << srgrRecord.azimuthTime << "\n";
         }
      }
      else
      {
         assert(nextRecord != records.end());
         assert(!previousRecord->coefs.empty()&&"previousRecord coefficients vector is empty.");
         assert(!nextRecord->coefs.empty()&&"nextRecord coefficients vector is empty.");

         // If azimuth time is between 2 records, interpolate
         const double interp
            = DurationType(azimuthTime             - previousRecord->azimuthTime)
            / (nextRecord->azimuthTime - previousRecord->azimuthTime)
            ;
         // std::clog << "interp: " << interp << " ="
         // << " (" << azimuthTime             << " - " << previousRecord->azimuthTime << " (="<< (azimuthTime             - previousRecord->azimuthTime)<< ") )"
         // << "/(" << nextRecord->azimuthTime << " - " << previousRecord->azimuthTime << " (="<< (nextRecord->azimuthTime - previousRecord->azimuthTime)<< ") )"
         // << "\n";

         srgrRecord.rg0 = (1-interp) * previousRecord->rg0 + interp*nextRecord->rg0;

         srgrRecord.coefs.clear();
         std::vector<double>::const_iterator pIt = previousRecord->coefs.begin();
         std::vector<double>::const_iterator nIt = nextRecord->coefs.begin();

         for(;pIt != previousRecord->coefs.end() && nIt != nextRecord->coefs.end();++pIt,++nIt)
         {
            srgrRecord.coefs.push_back(interp*(*nIt)+(1-interp)*(*pIt));
         }

         assert(!srgrRecord.coefs.empty()&&"Slant range to ground range interpolated coefficients vector is empty.");
      }

      // Now that we have the interpolated coefs, compute ground range
      // from slant range
      const double sr_minus_sr0 =  in-srgrRecord.rg0;

      assert(!srgrRecord.coefs.empty()&&"Slant range to ground range coefficients vector is empty.");

      out = 0;

      for(std::vector<double>::const_reverse_iterator cIt = srgrRecord.coefs.rbegin();cIt!=srgrRecord.coefs.rend();++cIt)
      {
         out = *cIt + sr_minus_sr0*out;
      }
   }


   bool ossimSarSensorModel::zeroDopplerLookup(const ossimEcefPoint & inputPt, TimeType & interpAzimuthTime, ossimEcefPoint & interpSensorPos, ossimEcefVector & interpSensorVel) const
   {
      assert((theOrbitRecords.size()>=2) && "Orbit records vector contains less than 2 elements");

      std::vector<OrbitRecordType>::const_iterator it = theOrbitRecords.begin();

      double doppler2(0.);

      // Compute range and doppler of first record
      // NOTE: here we only use the scalar product with vel and discard
      // the constant coef as it has no impact on doppler sign

      double doppler1 = (inputPt-it->position).dot(it->velocity);


      bool dopplerSign1 = doppler1 < 0;

      ++it; // -> it != begin

      // Look for the consecutive records where doppler freq changes sign
      // Note: implementing a bisection algorithm here might be faster
      for ( ; it!=theOrbitRecords.end() ; ++it)
      {
         // compute range and doppler of current record
         doppler2 = (inputPt-it->position).dot(it->velocity);

         const bool dopplerSign2 = doppler2 <0;

         // If a change of sign is detected
         if(dopplerSign1 != dopplerSign2)
         {
            break;
         }
         else
         {
            doppler1 = doppler2;
         }
      }

      // In this case, we need to extrapolate
      if(it == theOrbitRecords.end())
      {
         std::vector<OrbitRecordType>::const_iterator record1 = theOrbitRecords.begin();
         std::vector<OrbitRecordType>::const_iterator record2 = record1 + theOrbitRecords.size()-1;
         doppler1 = (inputPt-record1->position).dot(record1->velocity);
         doppler2 = (inputPt-record2->position).dot(record2->velocity);
         const DurationType delta_td = record2->azimuthTime - record1->azimuthTime;
         interpAzimuthTime = record1->azimuthTime - doppler1 / (doppler2 - doppler1) * delta_td;
      }
      else
      {
         assert(it != theOrbitRecords.begin());
         assert(it != theOrbitRecords.end());
         std::vector<OrbitRecordType>::const_iterator record2 = it;
         std::vector<OrbitRecordType>::const_iterator record1 = --it;
         // now interpolate time and sensor position
         const double abs_doppler1 = std::abs(doppler1);
         const double interpDenom = abs_doppler1+std::abs(doppler2);

         assert(interpDenom>0&&"Both doppler frequency are null in interpolation weight computation");

         const double interp = abs_doppler1/interpDenom;
         // std::clog << "interp: " << interp << "\n";

         const DurationType delta_td = record2->azimuthTime - record1->azimuthTime;
         // std::clog << "delta_td: " << delta_td << " = " << record2->azimuthTime <<" - " <<record1->azimuthTime<< "\n";

         // Compute interpolated time offset wrt record1
         // (No need for that many computations (day-frac -> ms -> day frac))
         const DurationType td     = delta_td * interp;
         // std::clog << "td: " << td  << "(" << td.total_microseconds() << "us)\n";
         // Compute interpolated azimuth time
         interpAzimuthTime = record1->azimuthTime + td + theAzimuthTimeOffset;
      }

      // std::clog << "interpAzimuthTime: " << interpAzimuthTime << "\n";

      // Interpolate sensor position and velocity
      interpolateSensorPosVel(interpAzimuthTime,interpSensorPos, interpSensorVel);

      return true;
   }

   void ossimSarSensorModel::computeBistaticCorrection(const ossimEcefPoint & inputPt, const ossimEcefPoint & sensorPos, DurationType & bistaticCorrection) const
   {
      // Bistatic correction (eq 25, p 28)
      double halftrange = 1000000. * (sensorPos-inputPt).magnitude()/C;
      bistaticCorrection= microseconds(static_cast<unsigned long>(floor(halftrange+0.5)));
   }


   void ossimSarSensorModel::azimuthTimeToLine(const TimeType & azimuthTime, double & line) const
   {
      assert(!theBurstRecords.empty()&&"Burst records are empty (at least one burst should be available)");

      std::vector<BurstRecordType>::const_iterator currentBurst = theBurstRecords.begin();

      // Look for the correct burst. In most cases the number of burst
      // records will be 1 (except for TOPSAR Sentinel1 products)
      std::vector<BurstRecordType>::const_iterator it = theBurstRecords.begin();
      std::vector<BurstRecordType>::const_iterator itend = theBurstRecords.end();
      for(; it!= itend ; ++it)
      {
         if(azimuthTime >= it->azimuthStartTime
               && azimuthTime < it->azimuthStopTime)
         {
            currentBurst = it;
            break;
         }
      }

      // If no burst is found, we will use the first (resp. last burst to
      // extrapolate line
      if(it == itend)
      {
         if(! theBurstRecords.empty())
         {
            if(azimuthTime < theBurstRecords.front().azimuthStartTime)
            {
               currentBurst = theBurstRecords.begin();
            }
            else if (azimuthTime > theBurstRecords.back().azimuthStopTime)
            {
               currentBurst = theBurstRecords.end()-1;
            }
         }
         else
         {
            // Fall back to the only record
            currentBurst = theBurstRecords.begin();
         }
      }

      const DurationType timeSinceStart = azimuthTime - currentBurst->azimuthStartTime;

      // Eq 22 p 27
      line = (timeSinceStart/theAzimuthTimeInterval) + currentBurst->startLine;
      // std::clog << "line = " << line << " <- " << timeSinceStart << "/" << theAzimuthTimeInterval << "+" << currentBurst->startLine << "\n";
   }

   void ossimSarSensorModel::lineToAzimuthTime(const double & line, TimeType & azimuthTime) const
   {
      assert(!theBurstRecords.empty()&&"Burst records are empty (at least one burst should be available)");

      std::vector<BurstRecordType>::const_iterator currentBurst = theBurstRecords.begin();

      if(theBurstRecords.size() != 1)
      {
         // Look for the correct burst. In most cases the number of burst
         // records will be 1 (except for TOPSAR Sentinel1 products)
         std::vector<BurstRecordType>::const_iterator it = theBurstRecords.begin();
         std::vector<BurstRecordType>::const_iterator itend = theBurstRecords.end();
         for( ; it!= itend; ++it)
         {
            if(line >= it->startLine && line < it->endLine)
            {
               currentBurst = it;
               break;
            }
         }

         if(it == itend)
         {
            if(line < theBurstRecords.front().startLine)
            {
               currentBurst = theBurstRecords.begin();
            }
            else if (line >= theBurstRecords.back().endLine)
            {
               currentBurst = theBurstRecords.end()-1;
            }
         }

      }

      const DurationType timeSinceStart = (line - currentBurst->startLine)*theAzimuthTimeInterval;
      // std::clog << "timeSinceStart: " << timeSinceStart.total_microseconds() << "us\n";

      // Eq 22 p 27
      azimuthTime = currentBurst->azimuthStartTime + timeSinceStart + theAzimuthTimeOffset;
      // std::clog << "offset: "         << theAzimuthTimeOffset << "\n";
      // std::clog << "->azimuthTime: "  << azimuthTime << "\n";
   }



   bool ossimSarSensorModel::projToSurface(const GCPRecordType & initGcp, const ossimDpt & target, const ossimHgtRef & hgtRef, ossimEcefPoint & ellPt) const
   {
     
      // Initialize current estimation
      ossimEcefPoint currentEstimation(initGcp.worldPt);

      // Compute corresponding image position
      // std::clog << "initGCP: " << initGcp.imPt << "\n";
      ossimDpt currentImPoint(initGcp.imPt);

      ossim_float64 currentImSquareResidual = squareDistance(target,currentImPoint);
      double currentHeightResidual = initGcp.worldPt.height() - hgtRef.getRefHeight(initGcp.worldPt);

      bool init = true;

      unsigned int iter = 0;

      // TODO: Every time the function is called, an allocation (+a free) is done.
      // This is not efficient. => Find a static matrix of 3x3 elements
      // Moreover, NEWMAT implies a lot of objet creations, hence allocations
      NEWMAT::SymmetricMatrix BtB(3);
      NEWMAT::ColumnVector BtF(3);
      NEWMAT::ColumnVector F(3);
      NEWMAT::ColumnVector dR(3);

      // Stop condition: img residual < 1e-2 pixels, height residual² <
      // 0.01² m, nb iter < 50. init ensure that loop runs at least once.
      while((init || (currentImSquareResidual > (0.01*0.01) || std::abs(currentHeightResidual) > 0.01))  && iter < 50)
      {
         if(init)
            init =false;

         // std::clog<<"Iter: "<<iter<<", Res: im="<<currentImSquareResidual<<", hgt="<<currentHeightResidual<<'\n';

         // compute residuals
         F(1) = target.x - currentImPoint.x;
         F(2) = target.y - currentImPoint.y;
         F(3) = currentHeightResidual;

         // std::clog<<"F("<<iter<<")="<<F<<'\n';

         // Delta use for partial derivatives estimation (in meters)
         const double d = 10.;

         // Compute partial derivatives
         ossimEcefVector p_fx, p_fy, p_fh,dx(d,0,0),dy(0,d,0),dz(0,0,d);
         ossimDpt tmpImPt;

         ossim_float64 rdx(0.0),rdy(0.0),rdz(0.0), fdx(0.0),fdy(0.0),fdz(0.0);

         ossimGpt currentEstimationWorld(currentEstimation);
         ossimGpt tmpGpt = ossimGpt(currentEstimation+dx);
         worldToLineSample(tmpGpt,tmpImPt);
         // std::clog << "currentEstimationWorld: " << currentEstimationWorld << "\n";
         // std::clog << "currentEstimation: " << currentEstimation << "\n";
         // std::clog << "tmpGpt: " << tmpGpt << "\n";
         // std::clog << "tmpImPt: " << tmpImPt << "\n";
         p_fx[0] = (currentImPoint.x-tmpImPt.x)/d;
         p_fy[0] = (currentImPoint.y-tmpImPt.y)/d;
         p_fh[0] = (currentEstimationWorld.height()-tmpGpt.height())/d;

         tmpGpt = ossimGpt(currentEstimation+dy);
         worldToLineSample(tmpGpt,tmpImPt);
         p_fx[1] = (currentImPoint.x-tmpImPt.x)/d;
         p_fy[1] = (currentImPoint.y-tmpImPt.y)/d;
         p_fh[1] = (currentEstimationWorld.height()-tmpGpt.height())/d;

         tmpGpt = ossimGpt(currentEstimation+dz);
         worldToLineSample(tmpGpt,tmpImPt);
         p_fx[2] = (currentImPoint.x-tmpImPt.x)/d;
         p_fy[2] = (currentImPoint.y-tmpImPt.y)/d;
         p_fh[2] = (currentEstimationWorld.height()-tmpGpt.height())/d;

         // Form B-matrix
         NEWMAT::Matrix B = ossimMatrix3x3::create(p_fx[0], p_fx[1], p_fx[2],
               p_fy[0], p_fy[1], p_fy[2],
               p_fh[0], p_fh[1], p_fh[2]);

         // std::clog<<"B: "<<B<<'\n';

         // Invert system
         try {
            dR = B.i() * F;
         } catch (NEWMAT::SingularException const& e) {
         ellPt = currentEstimation;
         
         ossimNotify(ossimNotifyLevel_WARN) <<"ossim::SarSensorModel::projToSurface(): singular matrix can not be inverted. Returning best estimation so far ("<<ellPt<<") for output point ("<<target<<")\n";
         std::clog << "initGCP: " << initGcp.imPt <<", "<<initGcp.worldPt<< "\n";
         
         return true;
         }

         // Update estimate
         for (ossim_int32 k=0; k<3; k++)
         {
            currentEstimation[k] -= dR(k+1);
         }

         // std::clog<<"dR: "<<dR<<'\n';

         currentEstimationWorld=ossimGpt(currentEstimation);

         // Update residuals
         const ossim_float64 atHgt = hgtRef.getRefHeight(currentEstimationWorld);
         currentHeightResidual = atHgt - currentEstimationWorld.height();

         worldToLineSample(currentEstimationWorld,currentImPoint);

         // std::clog<<currentImPoint<<'\n';

         currentImSquareResidual = squareDistance(currentImPoint,target);

         ++iter;
      }

      // std::clog<<"Iter: "<<iter<<", Res: im="<<currentImSquareResidual<<", hgt="<<currentHeightResidual<<'\n';

      ellPt = currentEstimation;
      return true;
   }



   //*************************************************************************************************
   // Infamous DUP
   //*************************************************************************************************
   ossimObject* ossimSarSensorModel::dup() const
   {
      return new ossimSarSensorModel(*this);
   }
   bool ossimSarSensorModel::useForward() const
   {
      return false;
   }

   bool ossimSarSensorModel::autovalidateInverseModelFromGCPs(const double & xtol, const double & ytol, const double azTimeTol, const double & rangeTimeTol) const
   {
      std::clog << "ossimSarSensorModel::autovalidateInverseModelFromGCPs()\n";
      if(theGCPRecords.empty())
      {
         return false;
      }

      bool success = true;

      unsigned int gcpId = 1;

      std::clog << theGCPRecords.size() << " GCPS\n";
      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt,++gcpId)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         // Estimate times
         ossimEcefPoint sensorPos;
         ossimEcefVector sensorVel;
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime,sensorPos, sensorVel);
         this->worldToLineSample(gcpIt->worldPt,estimatedImPt);

         const bool thisSuccess
            =  s1
            && (std::abs(estimatedImPt.x - gcpIt->imPt.x) <= xtol)
            && (std::abs(estimatedImPt.y - gcpIt->imPt.y) <= ytol)
            && (std::abs((estimatedAzimuthTime-gcpIt->azimuthTime).total_microseconds()) <= azTimeTol)
            && (std::abs(estimatedRangeTime - gcpIt->slantRangeTime) <= rangeTimeTol)
            ;

         const bool verbose = k_verbose;

         success = success && thisSuccess;

         if(verbose)
         {
            std::clog<<"GCP #"<<gcpId<< (thisSuccess ? "succeeded" : "failed") << '\n';
            std::clog<<"Azimuth time: ref="<<gcpIt->azimuthTime<<", predicted: "<<estimatedAzimuthTime<<", res="<<to_simple_string(estimatedAzimuthTime-gcpIt->azimuthTime)<<'\n';
            std::clog<<"Slant range time: ref="<<gcpIt->slantRangeTime<<", predicted: "<<estimatedRangeTime<<", res="<<std::abs(estimatedRangeTime - gcpIt->slantRangeTime)<<'\n';
            std::clog<<"Image point: ref="<<gcpIt->imPt<<", predicted="<<estimatedImPt<<", res="<<estimatedImPt-gcpIt->imPt<<'\n';
            std::clog<<'\n';
         }
      }

      if(success)
      {
         std::cout<<"All GCPs within "<<ytol <<" azimuth pixel, "<<xtol<<" range pixel, "<<azTimeTol<<" microseconds of azimuth time, "<<rangeTimeTol<<" of range time\n";
      }

      return success;
   }

   bool ossimSarSensorModel::autovalidateForwardModelFromGCPs(double resTol)
   {
      // std::clog << "ossimSarSensorModel::autovalidateForwardModelFromGCPs()\n";
      resTol *= resTol; // as internally we won't be using sqrt on norms

      // First, split half of the gcps to serve as tests, and remove them
      // temporarily from theGCPRecord.
      std::vector<GCPRecordType> gcpRecordSave, testGcps;

      gcpRecordSave.swap(theGCPRecords); // steal the data; `gcpRecordSave = move(theGCPRecords);` in C++11

      unsigned int count = 0;

      unzip(gcpRecordSave, theGCPRecords, testGcps);
      assert(theGCPRecords.size() >= testGcps.size());

      bool success = true;
      const bool verbose = k_verbose;

      unsigned int gcpId = 1;

      // std::clog << testGcps.size() << " GCPS\n";
      for(std::vector<GCPRecordType>::const_iterator gcpIt = testGcps.begin(); gcpIt!=testGcps.end();++gcpIt,++gcpId)
      {
         ossimGpt estimatedWorldPt;
         ossimGpt const& refPt = gcpIt->worldPt;

         double estimatedRangeTime;
         TimeType estimatedAzimuthTime;

         lineSampleToAzimuthRangeTime(gcpIt->imPt,estimatedAzimuthTime,estimatedRangeTime);

         lineSampleHeightToWorld(gcpIt->imPt,refPt.height(),estimatedWorldPt);

         const double res = squareDistance(refPt, estimatedWorldPt);

         if(res>resTol || estimatedWorldPt.hasNans())
         {
            success = false;

            if(verbose)
            {
               std::clog<<"GCP #"<<gcpId<<'\n';
               std::clog<<"Azimuth time: ref="<<gcpIt->azimuthTime<<", predicted: "<<estimatedAzimuthTime<<", res="<<to_simple_string(estimatedAzimuthTime-gcpIt->azimuthTime)<<'\n';
               std::clog<<"Slant range time: ref="<<gcpIt->slantRangeTime<<", predicted: "<<estimatedRangeTime<<", res="<<std::abs(estimatedRangeTime - gcpIt->slantRangeTime)<<'\n';
               std::clog<<"Im point: "<<gcpIt->imPt<<'\n';
               std::clog<<"World point: ref="<<refPt<<", predicted="<<estimatedWorldPt<<", res="<<sqrt(res)<<" m\n";
               std::clog<<'\n';
            }
         }
      }

      theGCPRecords.swap(gcpRecordSave);

      return success;
   }

   void ossimSarSensorModel::optimizeTimeOffsetsFromGcps()
   {
      // std::clog << "ossimSarSensorModel::optimizeTimeOffsetsFromGcps()\n";
      DurationType cumulAzimuthTime(seconds(0));
      double cumulRangeTime(0);
      unsigned int count=0;
      // reset offsets before optimisation
      theAzimuthTimeOffset = seconds(0);
      theRangeTimeOffset = 0.0;

      // First, fix the azimuth time
      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         ossimEcefPoint sensorPos;
         ossimEcefVector sensorVel;
         // Estimate times
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime, sensorPos, sensorVel);

         if(s1)
         {
            cumulAzimuthTime -= (estimatedAzimuthTime-gcpIt->azimuthTime);
            ++count;
         }
      }

      theAzimuthTimeOffset = count > 0 ? cumulAzimuthTime / count : DurationType(0);

      // Then, fix the range time
      count=0;

      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         ossimEcefPoint sensorPos;
         ossimEcefVector sensorVel;

         // Estimate times
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime, sensorPos, sensorVel);

         if(s1)
         {
            cumulRangeTime+=-estimatedRangeTime+gcpIt->slantRangeTime;
            ++count;
         }
      }

      theRangeTimeOffset = count > 0 ? cumulRangeTime/count : 0;
   }

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::OrbitRecordType> & orbitRecords)
   {
      char orbit_prefix_[256];
      std::size_t nbOrbits(0);
      try {
         get(kwl, "orbitList.nb_orbits", nbOrbits);
      } catch (kw_runtime_error const& e) {
         nbOrbits = 0;
         ossimRegExp regExp;
         regExp.compile("orbitList\\.orbit\\[.*\\]\\.time");
         ossimKeywordlist::KeywordMap::const_iterator i =
           kwl.getMap().begin();
         for(; i != kwl.getMap().end(); ++i)
         {
            if(regExp.find( (*i).first.c_str()))
            {
               ++nbOrbits;
            }
         }
         // Method getNumberOfKeysThatMatch not available in ossim 1.8.16
         //nbOrbits = kwl.getNumberOfKeysThatMatch("orbitList\\.orbit\\[.*\\]\\.time");
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: " << e.what()
            << "\n\tNumber of orbits manually counted to " << nbOrbits
            << ".\n\tPlease update your geom file.\n";
      }

      for (std::size_t i=0; i!=nbOrbits ; ++i) {
         const int pos = s_printf(orbit_prefix_, "orbitList.orbit[%d].", int(i));
         assert(pos > 0 && pos < 256);
         const std::string orbit_prefix(orbit_prefix_, pos);

         ossimSarSensorModel::OrbitRecordType orbitRecord;
         get(kwl, orbit_prefix + keyTime, orbitRecord.azimuthTime);
         get(kwl, orbit_prefix + keyPosX, orbitRecord.position[0]);
         get(kwl, orbit_prefix + keyPosY, orbitRecord.position[1]);
         get(kwl, orbit_prefix + keyPosZ, orbitRecord.position[2]);
         get(kwl, orbit_prefix + keyVelX, orbitRecord.velocity[0]);
         get(kwl, orbit_prefix + keyVelY, orbitRecord.velocity[1]);
         get(kwl, orbit_prefix + keyVelZ, orbitRecord.velocity[2]);
         orbitRecords.push_back(orbitRecord);
      }
   }

   void add(
       ossimKeywordlist & kwl,
       const std::vector<ossimSarSensorModel::OrbitRecordType> & orbitRecords)
   {
     char orbit_prefix_[256];

     add(kwl,ORBIT_NUMBER_KEY,(ossim_uint32)orbitRecords.size());

     for(std::size_t i = 0; i!=orbitRecords.size();++i)
       {
       const int pos = s_printf(orbit_prefix_, "%s[%d].", ORBIT_PREFIX.c_str(), int(i));
       assert(pos > 0 && pos < 256);
       const std::string orbit_prefix(orbit_prefix_, pos);

       add(kwl, orbit_prefix + keyTime, orbitRecords[i].azimuthTime);
       add(kwl, orbit_prefix + keyPosX, orbitRecords[i].position[0]);
       add(kwl, orbit_prefix + keyPosY, orbitRecords[i].position[1]);
       add(kwl, orbit_prefix + keyPosZ, orbitRecords[i].position[2]);
       add(kwl, orbit_prefix + keyVelX, orbitRecords[i].velocity[0]);
       add(kwl, orbit_prefix + keyVelY, orbitRecords[i].velocity[1]);
       add(kwl, orbit_prefix + keyVelZ, orbitRecords[i].velocity[2]);
       }
   }

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::BurstRecordType> & burstRecords)
   {
      char burstPrefix_[1024];
      std::size_t nbBursts(0);
      get(kwl, BURST_NUMBER_KEY, nbBursts);
      for (std::size_t burstId=0; burstId!=nbBursts ; ++burstId) {
         const int pos = s_printf(burstPrefix_, "%s[%d].", BURST_PREFIX.c_str(), burstId);
         assert(pos > 0 && pos < sizeof(burstPrefix_));
         const std::string burstPrefix(burstPrefix_, pos);

         ossimSarSensorModel::BurstRecordType burstRecord;
         get(kwl, burstPrefix + keyStartLine,        burstRecord.startLine);
         get(kwl, burstPrefix + keyEndLine,          burstRecord.endLine);
	 
	 try {
            unsigned int version;
            get(kwl, HEADER_PREFIX, "version", version);
	    // startSample and endSample since version 3
            if (version >= 3) 
	      {
                get(kwl, burstPrefix + keyStartSample,        burstRecord.startSample);
		get(kwl, burstPrefix + keyEndSample,          burstRecord.endSample);
	      }
         } 
	 catch (...) {
            throw std::runtime_error("Geom file generated with previous version of ossim plugins");
         }
        
	 get(kwl, burstPrefix + keyAzimuthStartTime, burstRecord.azimuthStartTime);
         get(kwl, burstPrefix + keyAzimuthStopTime,  burstRecord.azimuthStopTime);
         burstRecords.push_back(burstRecord);
      }
   }

   void add(
     ossimKeywordlist                                        & kwl,
     const std::vector<ossimSarSensorModel::BurstRecordType> & burstRecords)
   {
     char burstPrefix_[1024];
     add(kwl, BURST_NUMBER_KEY, (ossim_uint32)burstRecords.size());
     for (std::size_t burstId=0; burstId!=burstRecords.size() ; ++burstId) {
     const int pos = s_printf(burstPrefix_, "%s[%d].", BURST_PREFIX.c_str(), burstId);
     assert(pos > 0 && pos < sizeof(burstPrefix_));
     const std::string burstPrefix(burstPrefix_, pos);

     add(kwl, burstPrefix + keyStartLine, (ossim_uint32) burstRecords[burstId].startLine);
     add(kwl, burstPrefix + keyEndLine, (ossim_uint32) burstRecords[burstId].endLine);
     add(kwl, burstPrefix + keyStartSample, (ossim_uint32) burstRecords[burstId].startSample);
     add(kwl, burstPrefix + keyEndSample, (ossim_uint32) burstRecords[burstId].endSample);
     add(kwl, burstPrefix + keyAzimuthStartTime, burstRecords[burstId].azimuthStartTime);
     add(kwl, burstPrefix + keyAzimuthStopTime,  burstRecords[burstId].azimuthStopTime);
     }
   }

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::GCPRecordType> & gcpRecords)
   {
      char prefix_[1024];
      std::size_t nbGCPs(0);
      get(kwl, GCP_NUMBER_KEY, nbGCPs);
      for (std::size_t gcpId=0; gcpId!=nbGCPs ; ++gcpId) {
         const int pos = s_printf(prefix_, "%s[%d].", GCP_PREFIX.c_str(), gcpId);
         assert(pos > 0 && pos < sizeof(prefix_));
         const std::string prefix(prefix_, pos);

         ossimSarSensorModel::GCPRecordType gcpRecord;
         get(kwl, prefix, keyAzimuthTime,    gcpRecord.azimuthTime);
         get(kwl, prefix, keySlantRangeTime, gcpRecord.slantRangeTime);
         get(kwl, prefix, keyImPtX,          gcpRecord.imPt.x);
         get(kwl, prefix, keyImPtY,          gcpRecord.imPt.y);
         get(kwl, prefix, keyWorldPtLat,     gcpRecord.worldPt.lat);
         get(kwl, prefix, keyWorldPtLon,     gcpRecord.worldPt.lon);
         get(kwl, prefix, keyWorldPtHgt,     gcpRecord.worldPt.hgt);
         gcpRecords.push_back(gcpRecord);
      }
   }

   void add(
     ossimKeywordlist                                      & kwl,
     const std::vector<ossimSarSensorModel::GCPRecordType> & gcpRecords)
   {
     char prefix_[1024];
     add(kwl, GCP_NUMBER_KEY, (ossim_uint32)gcpRecords.size());
     for (std::size_t gcpId=0; gcpId!=gcpRecords.size() ; ++gcpId) {
     const int pos = s_printf(prefix_, "%s[%d].", GCP_PREFIX.c_str(), gcpId);
     assert(pos > 0 && pos < sizeof(prefix_));
     const std::string prefix(prefix_, pos);

     add(kwl, prefix, keyAzimuthTime,    gcpRecords[gcpId].azimuthTime);
     add(kwl, prefix, keySlantRangeTime, gcpRecords[gcpId].slantRangeTime);
     add(kwl, prefix, keyImPtX,          gcpRecords[gcpId].imPt.x);
     add(kwl, prefix, keyImPtY,          gcpRecords[gcpId].imPt.y);
     add(kwl, prefix, keyWorldPtLat,     gcpRecords[gcpId].worldPt.lat);
     add(kwl, prefix, keyWorldPtLon,     gcpRecords[gcpId].worldPt.lon);
     add(kwl, prefix, keyWorldPtHgt,     gcpRecords[gcpId].worldPt.hgt);
     }
   }


   void get(
         ossimKeywordlist                                            const& kwl,
         std::string                                                 const& sr_gr_prefix,
         std::string                                                 const& rg0,
         std::vector<ossimSarSensorModel::CoordinateConversionRecordType> & outputRecords)
   {
      char prefix_[1024];
      std::size_t nbCoords(0);
      get(kwl, sr_gr_prefix +"."+ NUMBER_KEY, nbCoords);

      for (std::size_t idx=0 ; idx!=nbCoords ; ++idx)
      {
         const int pos = s_printf(prefix_, "%s[%d].", sr_gr_prefix.c_str(), idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix_));
         std::string prefix(prefix_, pos);

         ossimSarSensorModel::CoordinateConversionRecordType coordRecord;
         get(kwl, prefix + keyAzimuthTime,  coordRecord.azimuthTime);
         get(kwl, prefix + rg0,             coordRecord.rg0);

         std::size_t nbCoeffs(0);
         get(kwl, prefix + NUMBER_KEY,      nbCoeffs);
         for (std::size_t coeff_idx=0; coeff_idx!=nbCoeffs ; ++coeff_idx) {
            const int pos2 = s_printf(prefix_+pos, sizeof(prefix_)-pos, "coeff[%d]", coeff_idx);
            assert(pos2 > 0 && pos+pos2 < sizeof(prefix_));
            prefix.assign(prefix_, pos+pos2);
            double coeff;
            get(kwl, prefix, coeff);
            coordRecord.coefs.push_back(coeff);
         }
         outputRecords.push_back(coordRecord);
      }
   }

   void add(
     ossimKeywordlist                                                       & kwl,
     std::string                                                       const& sr_gr_prefix,
     std::string                                                       const& rg0,
     const std::vector<ossimSarSensorModel::CoordinateConversionRecordType> & conversionRecords)
   {
     char prefix_[1024];
     add(kwl, sr_gr_prefix +"."+ NUMBER_KEY, (ossim_uint32)conversionRecords.size());

     for (std::size_t idx=0 ; idx!=conversionRecords.size() ; ++idx)
       {
       const int pos = s_printf(prefix_, "%s[%d].", sr_gr_prefix.c_str(), idx);
       assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix_));
       std::string prefix(prefix_, pos);


       add(kwl, prefix + keyAzimuthTime,  conversionRecords[idx].azimuthTime);
       add(kwl, prefix + rg0,             conversionRecords[idx].rg0);

       std::size_t nbCoeffs = conversionRecords[idx].coefs.size();
       add(kwl, prefix + NUMBER_KEY,      (ossim_uint32)nbCoeffs);
       for (std::size_t coeff_idx=0; coeff_idx!=nbCoeffs ; ++coeff_idx)
         {
         const int pos2 = s_printf(prefix_+pos, sizeof(prefix_)-pos, "coeff[%d]", coeff_idx);
         assert(pos2 > 0 && pos+pos2 < sizeof(prefix_));
         prefix.assign(prefix_, pos+pos2);
         add(kwl, prefix, conversionRecords[idx].coefs[coeff_idx]);
         }
       }
   }
   bool ossimSarSensorModel::saveState(ossimKeywordlist& kwl, const char* prefix) const
   {
     static const char MODULE[] = "ossimplugins::ossimSarSensorModel::saveState";
     SCOPED_LOG(traceDebug, MODULE);

     // Prevent override of subclasses TYPE_KW
     
     if(!kwl.hasKey(ossimKeywordNames::TYPE_KW))
       {
       kwl.add(prefix,
               ossimKeywordNames::TYPE_KW,
               "ossimSarSensorModel",
               true);
       }

     std::string product_type = theProductType.ToString().data();
     add(kwl, SUPPORT_DATA_PREFIX + "product_type", product_type);
     add(kwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", theNearRangeTime      );
     add(kwl, SUPPORT_DATA_PREFIX, "range_sampling_rate"       , theRangeSamplingRate  );
     add(kwl, SUPPORT_DATA_PREFIX, "range_spacing"             , theRangeResolution    );
     add(kwl, SUPPORT_DATA_PREFIX, "radar_frequency"           , theRadarFrequency     );
     add(kwl, SUPPORT_DATA_PREFIX, "line_time_interval"        , theAzimuthTimeInterval.total_seconds());

     kwl.removeKeysThatMatch(ORBIT_PREFIX+"*");
     add(kwl, theOrbitRecords);

     kwl.removeKeysThatMatch(BURST_PREFIX+"*");
     add(kwl, theBurstRecords);

     if (isGRD())
       {
       kwl.removeKeysThatMatch(SR_PREFIX+"*");
       add(kwl, SR_PREFIX, keySr0, theSlantRangeToGroundRangeRecords);
       kwl.removeKeysThatMatch(GR_PREFIX+"*");
       add(kwl, GR_PREFIX, keyGr0, theGroundRangeToSlantRangeRecords);
       }
     kwl.removeKeysThatMatch(GCP_PREFIX+"*");
     add(kwl, theGCPRecords);

     add(kwl, HEADER_PREFIX, "version", thePluginVersion);

     if (redaptMedataAfterDeburst)
       {
	 add(kwl, SUPPORT_DATA_PREFIX, "first_line_time", theFirstLineTime);
	 add(kwl, SUPPORT_DATA_PREFIX, "last_line_time", theLastLineTime);
	 add(kwl, HEADER_PREFIX, "first_line_time", theFirstLineTime);
	 add(kwl, HEADER_PREFIX, "last_line_time", theLastLineTime);
       }

     return ossimSensorModel::saveState(kwl, prefix);
   }

   bool ossimSarSensorModel::loadState(ossimKeywordlist const& kwl, const char* prefix)
   {
      // std::clog << "ossimSarSensorModel::loadState()\n";
      static const char MODULE[] = "ossimplugins::ossimSarSensorModel::loadState";
      SCOPED_LOG(traceDebug, MODULE);

      try
      {
         const bool success = ossimSensorModel::loadState(kwl, prefix);
         if (!success) {
            return false;
         }

         // And finally, extract data into fields
         std::string product_type_string;
         get(kwl, SUPPORT_DATA_PREFIX + "product_type", product_type_string);
         theProductType = ProductType(product_type_string);

         get(kwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", theNearRangeTime      );
         get(kwl, SUPPORT_DATA_PREFIX, "range_sampling_rate"       , theRangeSamplingRate  );
         get(kwl, SUPPORT_DATA_PREFIX, "range_spacing"             , theRangeResolution    );
         get(kwl, SUPPORT_DATA_PREFIX, "radar_frequency"           , theRadarFrequency     );
         double azimuthTimeInterval = 0.; // in seconds
         get(kwl, SUPPORT_DATA_PREFIX, "line_time_interval"        , azimuthTimeInterval);
#if defined(USE_BOOST_TIME)
         theAzimuthTimeInterval = boost::posix_time::precise_duration(azimuthTimeInterval * 1000000.);
#else
         theAzimuthTimeInterval = seconds(azimuthTimeInterval);
#endif

         get(kwl, theOrbitRecords);
         // TODO: don't fetch burst records if already read thanks to xml loading
         // that required them
         theBurstRecords.clear();
         get(kwl, theBurstRecords);
         if (isGRD())
         {
            get(kwl, SR_PREFIX, keySr0, theSlantRangeToGroundRangeRecords);
            get(kwl, GR_PREFIX, keyGr0, theGroundRangeToSlantRangeRecords);
         }
         get(kwl, theGCPRecords);

         try {
            unsigned int version;
            get(kwl, HEADER_PREFIX, "version", version);
            if (version < thePluginVersionMin) {
               throw std::runtime_error("Geom file generated with previous version of ossim plugins");
            }
         } catch (...) {
            throw std::runtime_error("Geom file generated with previous version of ossim plugins");
         }

         optimizeTimeOffsetsFromGcps();
         return true;
      } catch (std::runtime_error const& e) {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: " << e.what()
            << "\n\tIt won't be possible to orthorectify the associated images!"
            << "\n\tPlease upgrade your geom file.\n";
      }
      return false;
   }

bool ossimSarSensorModel::deburst(std::vector<std::pair<unsigned long, unsigned long> >& lines, 
				  std::pair<unsigned long,unsigned long> & samples, bool onlyValidSample)
{
  if(theBurstRecords.empty())
    return false;
  
  // First, clear lines record
  lines.clear();

  // Check the single burst record case
  if(theBurstRecords.size() == 1)
    {
    lines.push_back(std::make_pair(theBurstRecords.front().startLine,theBurstRecords.front().endLine));
    return false;
    }

  // TODO: Ensure bursts are sorted
  
  // Process each burst
  std::vector<BurstRecordType>::const_iterator it = theBurstRecords.begin();
  // Correct since we have at least 2 bursts records
  std::vector<BurstRecordType>::const_iterator next = it+1;
  std::vector<BurstRecordType>::const_iterator itend = theBurstRecords.end();

  unsigned long currentStart  = it->startLine;  
  TimeType deburstAzimuthStartTime = it->azimuthStartTime;

  unsigned long deburstEndLine = 0;

  samples = std::make_pair(it->startSample, it->endSample);
  
  for(; next!= itend ;++it,++next)
    {
    DurationType timeOverlapEnd = (it->azimuthStopTime - next->azimuthStartTime);

    unsigned long overlapLength = timeOverlapEnd/theAzimuthTimeInterval;

    unsigned long halfLineOverlapEnd = overlapLength/2;
    TimeType endTimeInNextBurst = it->azimuthStopTime-(halfLineOverlapEnd-1)*theAzimuthTimeInterval;
    
    unsigned long halfLineOverlapBegin = std::floor(0.5+(endTimeInNextBurst-next->azimuthStartTime)/theAzimuthTimeInterval);
      
    unsigned long currentStop = it->endLine-halfLineOverlapEnd;

    deburstEndLine+= currentStop - currentStart + 1; // +1 because currentStart/Stop are both valids

    
    lines.push_back(std::make_pair(currentStart,currentStop));

    currentStart = next->startLine+halfLineOverlapBegin;

     if (onlyValidSample)
      {
	// Find the first and last valid sampleburst
	if (it->startSample > samples.first)
	  {
	    samples.first = it->startSample;
	  }
	if (it->endSample < samples.second)
	  {
	    samples.second = it->endSample;
	  }
      }
    }

  TimeType deburstAzimuthStopTime = it->azimuthStopTime;
  deburstEndLine+=it->endLine-currentStart;

  lines.push_back(std::make_pair(currentStart,it->endLine));

  if (onlyValidSample)
    {
      if (it->startSample > samples.first)
	{
	  samples.first = it->startSample;
	}
      if (it->endSample < samples.second)
	{
	  samples.second = it->endSample;
	}
    }

  
  // Now, update other metadata accordingly

  // Clear the previous burst records
  theBurstRecords.clear();

  // Create the single burst
  BurstRecordType deburstBurst;
  deburstBurst.startLine = 0;
  deburstBurst.azimuthStartTime = deburstAzimuthStartTime;
  deburstBurst.endLine = deburstEndLine;
  deburstBurst.azimuthStopTime = deburstAzimuthStopTime;

  if (onlyValidSample)
    {
      deburstBurst.startSample = 0;
      deburstBurst.endSample = samples.second - samples.first;
    }

  
  theBurstRecords.push_back(deburstBurst);

  std::vector<GCPRecordType> deburstGCPs;
  
  // Now move GCPs
  for(std::vector<GCPRecordType>::iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
      {
      GCPRecordType currentGCP = *gcpIt;
      unsigned long newLine=0;

      unsigned long gcpLine = std::floor(currentGCP.imPt.y+0.5);
      unsigned long gcpSample = std::floor(currentGCP.imPt.x+0.5);


      // Be careful about fractional part of GCPs
      double fractionalLines = currentGCP.imPt.y - gcpLine;
      double fractionalSamples = currentGCP.imPt.x - gcpSample;

      bool linesOk = imageLineToDeburstLine(lines,gcpLine,newLine);

      // Gcp into valid samples
      bool samplesOk = true;
      unsigned long newSample = gcpSample;
      
      if (onlyValidSample)
	{
	  samplesOk = false;
	  if (gcpSample >= samples.first && gcpSample <= samples.second)
	    {
	      samplesOk = true;
	      newSample -= samples.first; // Offset with first valid sample
	    } 
	}

      if(linesOk && samplesOk)
        {
	  currentGCP.imPt.y = newLine + fractionalLines;
	  currentGCP.imPt.x = newSample + fractionalSamples;
	  
	  deburstGCPs.push_back(currentGCP);
        }        
      }

  theGCPRecords.swap(deburstGCPs);


  // Adapt general metadata : theNearRangeTime, first_time_line, last_time_line
  redaptMedataAfterDeburst = true;
  theFirstLineTime = deburstBurst.azimuthStartTime;
  theLastLineTime = deburstBurst.azimuthStopTime;
  
  if (onlyValidSample)
    theNearRangeTime += samples.first*(1/theRangeSamplingRate); 


  return true;
}


bool 
ossimSarSensorModel::burstExtraction(const unsigned int burst_index, 
				     std::pair<unsigned long,unsigned long> & lines, 
				     std::pair<unsigned long,unsigned long> & samples)
{
   if(theBurstRecords.empty())
    return false;

   // Check the single burst record case
   if(theBurstRecords.size() == 1)
     {
       lines = std::make_pair(theBurstRecords.front().startLine,theBurstRecords.front().endLine);
       return false;
     }

   // Retrieve into TheBurstRecord, the required index
   BurstRecordType burstInd_Record = theBurstRecords[burst_index];
   lines = std::make_pair(burstInd_Record.startLine, burstInd_Record.endLine);
   samples = std::make_pair(burstInd_Record.startSample, burstInd_Record.endSample);
   TimeType burstAzimuthStartTime = burstInd_Record.azimuthStartTime;
   TimeType burstAzimuthStopTime = burstInd_Record.azimuthStopTime;

   // Clear the previous burst records
   theBurstRecords.clear();

   // Create the single burst
   BurstRecordType oneBurst;
   oneBurst.startLine = 0;
   oneBurst.azimuthStartTime = burstAzimuthStartTime;
   oneBurst.endLine = lines.second - lines.first;
   oneBurst.azimuthStopTime = burstAzimuthStopTime;
   oneBurst.startSample = 0;
   oneBurst.endSample = samples.second - samples.first;
   
   theBurstRecords.push_back(oneBurst);


    std::vector<GCPRecordType> oneBurstGCPs;
    
    // Now move GCPs
    for (auto const& token : theGCPRecords)   
      {
	GCPRecordType currentGCP = token;

	unsigned long gcpLine = std::floor(currentGCP.imPt.y+0.5);
	unsigned long gcpSample = std::floor(currentGCP.imPt.x+0.5);

	// Be careful about fractional part of GCPs
	double fractionalLines = currentGCP.imPt.y - gcpLine;
	double fractionalSamples = currentGCP.imPt.x - gcpSample;


	// Gcp into valid samples and valid lines
	unsigned long newSample = gcpSample;
	bool samplesOk = false;
	
	if (gcpSample >= samples.first && gcpSample <= samples.second)
	  {
	    samplesOk = true;
	    newSample -= samples.first; // Offset with first valid sample
	  } 

	unsigned long newLine = gcpLine;
	bool linesOk = false;
	
	if (gcpLine >= lines.first && gcpLine <= lines.second)
	  {
	    linesOk = true;
	    newLine -= lines.first; // Offset with first valid line
	  }
      
	if(linesOk && samplesOk)
	  {
	    currentGCP.imPt.y = newLine + fractionalLines;
	    currentGCP.imPt.x = newSample + fractionalSamples;
	    oneBurstGCPs.push_back(currentGCP);
	  }        
      }

  theGCPRecords.swap(oneBurstGCPs);

  // Adapt general metadata : theNearRangeTime, first_time_line, last_time_line
  redaptMedataAfterDeburst = true;
  theFirstLineTime = oneBurst.azimuthStartTime;
  theLastLineTime = oneBurst.azimuthStopTime;
  theNearRangeTime += samples.first*(1/theRangeSamplingRate); 

  return true;
}

bool 
ossimSarSensorModel::deburstAndConcatenate(std::vector<std::pair<unsigned long,unsigned long> >& linesBursts, 
					   std::vector<std::pair<unsigned long,unsigned long> >& samplesBursts)
{
   if(theBurstRecords.empty())
    return false;
  
   // declare lines and samples 
   std::vector<std::pair<unsigned long,unsigned long> > lines;
   lines.reserve(theBurstRecords.size());
   std::pair<unsigned long,unsigned long> samples;

   // First, clear lines record
   linesBursts.clear();
   samplesBursts.clear();

   // Check the single burst record case
   if(theBurstRecords.size() == 1)
     {
       linesBursts.push_back(std::make_pair(theBurstRecords.front().startLine,theBurstRecords.front().endLine));
       samplesBursts.push_back(std::make_pair(theBurstRecords.front().startLine,theBurstRecords.front().endLine));
       return false;
     }


   ///////// deburst operation ////////
   // Process each burst
   std::vector<BurstRecordType>::const_iterator it = theBurstRecords.begin();
   // Correct since we have at least 2 bursts records
   std::vector<BurstRecordType>::const_iterator next = it+1;
   std::vector<BurstRecordType>::const_iterator itend = theBurstRecords.end();

   unsigned long currentStart  = it->startLine;  
   TimeType deburstAzimuthStartTime = it->azimuthStartTime;

   unsigned long deburstEndLine = 0;
  
   samples = std::make_pair(it->startSample, it->endSample);

   // Store halfLineOverlapBegin/End
   unsigned long halfLineOverlapBegin[theBurstRecords.size()];
   unsigned long halfLineOverlapEnd[theBurstRecords.size()];
   
   halfLineOverlapBegin[0] = 0;

   unsigned int counterBegin = 1;
   unsigned int counterEnd = 0;

   for(; next!= itend ;++it,++next)
     {
       DurationType timeOverlapEnd = (it->azimuthStopTime - next->azimuthStartTime);

       unsigned long overlapLength = timeOverlapEnd/theAzimuthTimeInterval;

       halfLineOverlapEnd[counterEnd] = overlapLength/2;
       TimeType endTimeInNextBurst = it->azimuthStopTime-(halfLineOverlapEnd[counterEnd]-1)*theAzimuthTimeInterval;
    
       halfLineOverlapBegin[counterBegin] = std::floor(0.5+(endTimeInNextBurst-next->azimuthStartTime)/theAzimuthTimeInterval);
      
       unsigned long currentStop = it->endLine-halfLineOverlapEnd[counterEnd];

       deburstEndLine+= currentStop - currentStart + 1; // +1 because currentStart/Stop are both valids
    
       lines.push_back(std::make_pair(currentStart,currentStop));

       currentStart = next->startLine+halfLineOverlapBegin[counterBegin];

       // Find the first and last valid sampleburst
       if (it->startSample > samples.first)
	 {
	   samples.first = it->startSample;
	 }
       if (it->endSample < samples.second)
	 {
	   samples.second = it->endSample;
	 }


       ++counterBegin;
       ++counterEnd;
     }

   halfLineOverlapEnd[theBurstRecords.size() - 1] = 0;
   
   TimeType deburstAzimuthStopTime = it->azimuthStopTime;
   deburstEndLine+=it->endLine - currentStart;

   lines.push_back(std::make_pair(currentStart,it->endLine));

   if (it->startSample > samples.first)
     {
       samples.first = it->startSample;
     }
   if (it->endSample < samples.second)
     {
       samples.second = it->endSample;
     }
  
   // Now, update other metadata accordingly
   std::vector<GCPRecordType> deburstGCPs;
  
   // Now move GCPs
   for(std::vector<GCPRecordType>::iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
     {
       GCPRecordType currentGCP = *gcpIt;
       unsigned long newLine=0;

       unsigned long gcpLine = std::floor(currentGCP.imPt.y+0.5);
       unsigned long gcpSample = std::floor(currentGCP.imPt.x+0.5);

       // Be careful about fractional part of GCPs
       double fractionalLines = currentGCP.imPt.y - gcpLine;
       double fractionalSamples = currentGCP.imPt.x - gcpSample;

       bool linesOk = imageLineToDeburstLine(lines,gcpLine,newLine);

       // Gcp into valid samples
       unsigned long newSample = gcpSample;
      
       if (linesOk && gcpSample >= samples.first && gcpSample <= samples.second)
	 {
	   newSample -= samples.first; // Offset with first valid sample
	   currentGCP.imPt.y = newLine + fractionalLines;
	   currentGCP.imPt.x = newSample + fractionalSamples;
	   deburstGCPs.push_back(currentGCP);
	 }
     }

   theGCPRecords.swap(deburstGCPs);

   ///// linesBursts and samplesBursts (into Burst geometry) /////
   std::vector<BurstRecordType>::const_iterator itBursts = theBurstRecords.begin();
   std::vector<std::pair<unsigned long,unsigned long> >::const_iterator itlines = lines.begin();
   
   unsigned int counter = 0;

   for(; itBursts!= theBurstRecords.end() ;++itBursts)
     {       
       unsigned long currentStart_L = halfLineOverlapBegin[counter];
       unsigned long currentStop_L = itBursts->endLine - itBursts->startLine - halfLineOverlapEnd[counter];
       linesBursts.push_back(std::make_pair(currentStart_L, currentStop_L));

       unsigned long currentStart_S = 0;
       unsigned long currentStop_S = samples.second-samples.first;

       if (itBursts->startSample < samples.first)
	 {
	   currentStart_S = samples.first - itBursts->startSample;
	 }
       currentStop_S += currentStart_S;

       samplesBursts.push_back(std::make_pair(currentStart_S, currentStop_S));

       ++counter;
     }

   // Clear the previous burst records
   theBurstRecords.clear();

   // Create the single burst
   BurstRecordType deburstBurst;
   deburstBurst.startLine = 0;
   deburstBurst.azimuthStartTime = deburstAzimuthStartTime;
   deburstBurst.endLine = deburstEndLine;
   deburstBurst.azimuthStopTime = deburstAzimuthStopTime;
   deburstBurst.startSample = 0;
   deburstBurst.endSample = samples.second - samples.first;

   theBurstRecords.push_back(deburstBurst);

   // Adapt general metadata : theNearRangeTime, first_time_line, last_time_line
   redaptMedataAfterDeburst = true;
   theFirstLineTime = deburstBurst.azimuthStartTime;
   theLastLineTime = deburstBurst.azimuthStopTime;
  
   theNearRangeTime += samples.first*(1/theRangeSamplingRate); 


   return true;
}


bool ossimSarSensorModel::imageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long imageLine, unsigned long & deburstLine)
{
  std::vector<std::pair<unsigned long,unsigned long> >::const_iterator vit = lines.begin();
  std::vector<std::pair<unsigned long,unsigned long> >::const_iterator nit = vit+1;
  
  unsigned long lineOffset = vit->first;
  
  deburstLine = imageLine;
  
  while(nit != lines.end())
    {
    if(imageLine>=vit->first && imageLine<=vit->second)
      {
      deburstLine-=lineOffset;
      return true;
      }
    lineOffset+=nit->first - vit->second-1;
    ++vit;
    ++nit;
    }
  return false;
}

void ossimSarSensorModel::deburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long deburstLine, unsigned long & imageLine)
{
  std::vector<std::pair<unsigned long,unsigned long> >::const_iterator vit = lines.begin();
  std::vector<std::pair<unsigned long,unsigned long> >::const_iterator nit = vit+1;
  
  unsigned long lineOffset = vit->first;

  imageLine = deburstLine;
  
  while(nit != lines.end())
    {
    if(imageLine+lineOffset>=vit->first && imageLine+lineOffset<=vit->second)
      break;

    lineOffset+=nit->first - vit->second-1;
    ++vit;
    ++nit;
    }
  imageLine+=lineOffset;
}


}




namespace ossimplugins {
   template <typename T> inline
   std::ostream & operator<<(std::ostream & os, const std::vector<T> & v)
   {
      for (typename std::vector<T>::const_iterator b = v.begin(), e = v.end()
            ; b != e
            ; ++b
          )
      {
         os << "- " << *b << "\n";
      }

      return os;
   }
} // ossimplugins namespace


/*virtual*/ std::ostream& ossimplugins::ossimSarSensorModel::print(std::ostream& out) const
{
   return ossimSensorModel::print(out)
      << "\n ossimSarSensorModel data-members:\n"
      << "\n                 theRadarFrequency: " << theRadarFrequency << "Hz"
      << "\n            theAzimuthTimeInterval: " << theAzimuthTimeInterval.total_microseconds() << "us"
      << "\n                  theNearRangeTime: " << theNearRangeTime << "s"
      << "\n              theRangeSamplingRate: " << theRangeSamplingRate << "Hz"
      << "\n                theRangeResolution: " << theRangeResolution << "m"
      << "\n       theBistaticCorrectionNeeded: " << theBistaticCorrectionNeeded
      << "\n                    theProductType: " << theProductType
      << "\n              theAzimuthTimeOffset: " << theAzimuthTimeOffset.total_microseconds() << "us offset"
      << "\n                theRangeTimeOffset: " << theRangeTimeOffset << "s offset"
      << "\n                     theGCPRecords: " << "\n" << theGCPRecords
      << "\n                   theOrbitRecords: " << "\n" << theOrbitRecords
      << "\n                   theBurstRecords: " << "\n" << theBurstRecords
      << "\n theSlantRangeToGroundRangeRecords: " << "\n" << theSlantRangeToGroundRangeRecords
      << "\n theGroundRangeToSlantRangeRecords: " << "\n" << theGroundRangeToSlantRangeRecords
      << "\n"
      ;
}

