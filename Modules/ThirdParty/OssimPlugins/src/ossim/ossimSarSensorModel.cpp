//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "ossimSarSensorModel.h"
#include "ossimKeyWordListUtilities.h"
#include "ossimTraceHelpers.h"
#include "ossimRangeUtilities.h"
#include "ossimSarSensorModelPathsAndKeys.h"
#include <ossim/base/ossimLsrSpace.h>
#include <boost/static_assert.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#if defined(USE_BOOST_TIME)
using boost::posix_time::microseconds;
#else
using ossimplugins::time::microseconds;
#endif

namespace {// Anonymous namespace
   const bool k_verbose = false; // global verbose constant; TODO: use an option

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
   const double ossimSarSensorModel::C = 299792458;

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
      theAzimuthTimeInterval(0.),
      theNearRangeTime(0.),
      theRangeSamplingRate(0.),
      theRangeResolution(0.),
      theBistaticCorrectionNeeded(false),
      theAzimuthTimeOffset(0),
      theRangeTimeOffset(0)
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
      assert(theRangeResolution>0&&"theRangeResolution is null.");

      // First compute azimuth and range time
      TimeType azimuthTime;
      double rangeTime;

      const bool success = worldToAzimuthRangeTime(worldPt, azimuthTime, rangeTime);

      if(!success)
      {
         imPt.makeNan();
         return;
      }
      // std::cout << "AzimuthTime: " << azimuthTime << "\n";
      // std::cout << "RangeTime: " << rangeTime << "\n";
      // std::cout << "GRD: " << isGRD() << "\n";

      // Convert azimuth time to line
      azimuthTimeToLine(azimuthTime,imPt.y);

      if(isGRD())
      {
         // GRD case
         double groundRange(0);
         slantRangeToGroundRange(rangeTime*C/2,azimuthTime,groundRange);
         // std::cout << "GroundRange: " << groundRange << "\n";
         // std::cout << "TheRangeResolution: " << theRangeResolution << "\n";

         // Eq 32 p. 31
         // TODO: possible micro-optimization: precompute 1/theRangeResolution, and
         // use *
         imPt.x = groundRange/theRangeResolution;
      }
      else
      {
         // std::cout << "TheNearRangeTime: " << theNearRangeTime << "\n";
         // std::cout << "TheRangeSamplingRate: " << theRangeSamplingRate << "\n";
         // SLC case
         // Eq 23 and 24 p. 28
         imPt.x = (rangeTime - theNearRangeTime)*theRangeSamplingRate;
      }
   }

   bool ossimSarSensorModel::worldToAzimuthRangeTime(const ossimGpt& worldPt, TimeType & azimuthTime, double & rangeTime) const
   {
      // First convert lat/lon to ECEF
      ossimEcefPoint inputPt(worldPt);

      // Compute zero doppler time
      TimeType interpTime;
      ossimEcefPoint interpSensorPos;
      ossimEcefVector interpSensorVel;

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
         DurationType t_min = azimuthTime - theOrbitRecords.front().azimuthTime;

         if(t_min.is_negative())
            t_min = t_min.invert_sign();

         unsigned int count = 0;

         for(std::vector<OrbitRecordType>::const_iterator it = theOrbitRecords.begin();it!=theOrbitRecords.end();++it,++count)
         {
            DurationType current_time = azimuthTime-it->azimuthTime;

            if(current_time.is_negative())
               current_time = current_time.invert_sign();

            if(t_min > current_time)
            {
               t_min_idx = count;
               t_min = current_time;
            }
         }
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
            const double td1 = (azimuthTime - theOrbitRecords[j].azimuthTime).total_microseconds();
            const double td2 = (theOrbitRecords[i].azimuthTime - theOrbitRecords[j].azimuthTime).total_microseconds();
            w*=td1/td2;
         }
         ++j;
         for( ; j < nEnd; ++j)
         {
            const double td1 = (azimuthTime - theOrbitRecords[j].azimuthTime).total_microseconds();
            const double td2 = (theOrbitRecords[i].azimuthTime - theOrbitRecords[j].azimuthTime).total_microseconds();
            w*=td1/td2;
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
      // std::cout << "SR -> GR\n";
      applyCoordinateConversion(slantRange,azimuthTime,theSlantRangeToGroundRangeRecords,groundRange);
   }

   void ossimSarSensorModel::groundRangeToSlantRange(const double & groundRange, const TimeType & azimuthTime, double & slantRange) const
   {
      // std::cout << "GR -> SR\n";
      applyCoordinateConversion(groundRange,azimuthTime,theGroundRangeToSlantRangeRecords,slantRange);
   }

   void ossimSarSensorModel::applyCoordinateConversion(const double & in, const TimeType& azimuthTime, const std::vector<CoordinateConversionRecordType> & records, double & out) const
   {
      assert(!records.empty()&&"The records vector is empty.");
      // std::cout << "conv coord(" << in << ", az="<<azimuthTime<<")\n";

      // First, we need to find the correct pair of records for interpolation
      std::vector<CoordinateConversionRecordType>::const_iterator it = records.begin();

      CoordinateConversionRecordType srgrRecord;

      std::vector<CoordinateConversionRecordType>::const_iterator  previousRecord = it;
      ++it;

      std::vector<CoordinateConversionRecordType>::const_iterator nextRecord = it;

      // Look for the correct record
      // std::cout << "Looking for " << azimuthTime << " within records:\n";
      while(it!=records.end())
      {
         // std::cout << "- record: " << it->azimuthTime << "...";
         // nextRecord = it;

         if(azimuthTime >= previousRecord->azimuthTime
               && azimuthTime < nextRecord->azimuthTime)
         {
            // std::cout << " found!\n";
            break;
         }
         else
         {
            previousRecord = nextRecord;
            ++it;
            nextRecord = it;
            // std::cout << " NOT found => next!\n";
         }
      }
      assert(nextRecord == it);
      if(it == records.end())
      {
         if(azimuthTime < records.front().azimuthTime)
         {
            srgrRecord = records.front();
            // std::cout << "Not found, but before first => srgrRecord: " << srgrRecord.azimuthTime << "\n";
         }
         else if(azimuthTime >= records.back().azimuthTime)
         {
            srgrRecord = records.back();
            // std::cout << "Not found, but after last => srgrRecord: " << srgrRecord.azimuthTime << "\n";
         }
      }
      else
      {
         assert(nextRecord != records.end());
         assert(!previousRecord->coefs.empty()&&"previousRecord coefficients vector is empty.");
         assert(!nextRecord->coefs.empty()&&"nextRecord coefficients vector is empty.");

         // If azimuth time is between 2 records, interpolate
#if defined(USE_BOOST_TIME)
         const double interp
            =                    (azimuthTime             - previousRecord->azimuthTime).total_microseconds()
            /static_cast<double>((nextRecord->azimuthTime - previousRecord->azimuthTime).total_microseconds());
#else
         const double interp
            = (azimuthTime             - previousRecord->azimuthTime)
            / (nextRecord->azimuthTime - previousRecord->azimuthTime)
            ;
#endif
         // std::cout << "interp: " << interp << " ="
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
      assert(!theOrbitRecords.empty()&&"Orbit records vector is empty()");

      std::vector<OrbitRecordType>::const_iterator it = theOrbitRecords.begin();

      double doppler2(0.);

      // Compute range and doppler of first record
      // NOTE: here we only use the scalar product with vel and discard
      // the constant coef as it has no impact on doppler sign

      double doppler1 = (inputPt-it->position).dot(it->velocity);

      std::vector<OrbitRecordType>::const_iterator record1 = it;

      bool dopplerSign1 = doppler1 < 0;

      ++it;

      std::vector<OrbitRecordType>::const_iterator record2;

      // Look for the consecutive records where doppler freq changes sign
      // Note: implementing a bisection algorithm here might be faster
      for ( ; it!=theOrbitRecords.end() ; ++it)
      {
         record2 = it;

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
            record1 = record2;
         }
      }

      // If not found, pass error to caller (not a programming error, but
      // eronous input parameters
      if(it == theOrbitRecords.end())
      {
         return false;
      }

      // now interpolate time and sensor position
      const double abs_doppler1 = std::abs(doppler1);
      const double interpDenom = abs_doppler1+std::abs(doppler2);

      assert(interpDenom>0&&"Both doppler frequency are null in interpolation weight computation");

      const double interp = abs_doppler1/interpDenom;
      // std::cout << "OK - interp: " << interp << "\n";

      // Note that microsecond precision is used here
      const DurationType delta_td = record2->azimuthTime - record1->azimuthTime;
      const double deltat = static_cast<double>(delta_td.total_microseconds());
      // std::cout << "OK - delta_td: " << delta_td << "\n";
      // std::cout << "OK - deltat: " << deltat << "ms\n";

      // Compute interpolated time offset wrt record1
      // const DurationType td     = microseconds(static_cast<unsigned long>(floor(interp * deltat+0.5)));
      const DurationType td     = delta_td * interp;
      const DurationType offset = microseconds(static_cast<unsigned long>(floor(theAzimuthTimeOffset+0.5)));
      // std::cout << "td: " << td << "\n";
      // std::cout << "offset: " << offset << "\n";

      // Compute interpolated azimuth time
      interpAzimuthTime = record1->azimuthTime + td + offset;
      // std::cout << "interpAzimuthTime: " << interpAzimuthTime << "\n";

      // Interpolate sensor position and velocity
      interpolateSensorPosVel(interpAzimuthTime,interpSensorPos, interpSensorVel);

      return true;
   }

   void ossimSarSensorModel::computeBistaticCorrection(const ossimEcefPoint & inputPt, const ossimEcefPoint & sensorPos, DurationType & bistaticCorrection) const
   {
      // Bistatic correction (eq 25, p 28)
      double halftrange = 1000000 * (sensorPos-inputPt).magnitude()/C;
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
      const double timeSinceStartInMicroSeconds = static_cast<double>(timeSinceStart.total_microseconds());

      // Eq 22 p 27
      line = (timeSinceStartInMicroSeconds/theAzimuthTimeInterval) + currentBurst->startLine;
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

      const double timeSinceStartInMicroSeconds = (line - currentBurst->startLine)*theAzimuthTimeInterval;
      // std::cout << "timeSinceStartInMicroSeconds: " << timeSinceStartInMicroSeconds << "\n";

      const DurationType timeSinceStart = microseconds(timeSinceStartInMicroSeconds);
      const DurationType offset         = microseconds(theAzimuthTimeOffset);
      // Eq 22 p 27
      azimuthTime = currentBurst->azimuthStartTime + timeSinceStart + offset;
      // std::cout << "timeSinceStart: " << timeSinceStart << "\n";
      // std::cout << "offset: "         << offset << " (" << theAzimuthTimeOffset << ")\n";
      // std::cout << "->azimuthTime: "  << azimuthTime << "\n";
   }



   bool ossimSarSensorModel::projToSurface(const GCPRecordType & initGcp, const ossimDpt & target, const ossimHgtRef & hgtRef, ossimEcefPoint & ellPt) const
   {
      // Initialize current estimation
      ossimEcefPoint currentEstimation(initGcp.worldPt);

      // Compute corresponding image position
      // std::cout << "initGCP: " << initGcp.imPt << "\n";
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

         // std::cout<<"Iter: "<<iter<<", Res: im="<<currentImSquareResidual<<", hgt="<<currentHeightResidual<<'\n';

         // compute residuals
         F(1) = target.x - currentImPoint.x;
         F(2) = target.y - currentImPoint.y;
         F(3) = currentHeightResidual;

         // std::cout<<"F("<<iter<<")="<<F<<'\n';

         // Delta use for partial derivatives estimation (in meters)
         const double d = 10.;

         // Compute partial derivatives
         ossimEcefVector p_fx, p_fy, p_fh,dx(d,0,0),dy(0,d,0),dz(0,0,d);
         ossimDpt tmpImPt;

         ossim_float64 rdx,rdy,rdz, fdx,fdy,fdz;

         ossimGpt currentEstimationWorld(currentEstimation);
         ossimGpt tmpGpt = ossimGpt(currentEstimation+dx);
         worldToLineSample(tmpGpt,tmpImPt);
         // std::cout << "currentEstimationWorld: " << currentEstimationWorld << "\n";
         // std::cout << "currentEstimation: " << currentEstimation << "\n";
         // std::cout << "tmpGpt: " << tmpGpt << "\n";
         // std::cout << "tmpImPt: " << tmpImPt << "\n";
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

         // std::cout<<"B: "<<B<<'\n';

         // Invert system
         try {
            dR = B.i() * F;
         } catch (RBD_COMMON::BaseException const& e) {
            // NEWMATH exception
            throw std::runtime_error(e.what());
         }

         // Update estimate
         for (ossim_int32 k=0; k<3; k++)
         {
            currentEstimation[k] -= dR(k+1);
         }

         // std::cout<<"dR: "<<dR<<'\n';

         currentEstimationWorld=ossimGpt(currentEstimation);

         // Update residuals
         const ossim_float64 atHgt = hgtRef.getRefHeight(currentEstimationWorld);
         currentHeightResidual = atHgt - currentEstimationWorld.height();

         worldToLineSample(currentEstimationWorld,currentImPoint);

         // std::cout<<currentImPoint<<'\n';

         currentImSquareResidual = squareDistance(currentImPoint,target);

         ++iter;
      }

      // std::cout<<"Iter: "<<iter<<", Res: im="<<currentImSquareResidual<<", hgt="<<currentHeightResidual<<'\n';

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
      if(theGCPRecords.empty())
      {
         return false;
      }

      bool success = true;

      unsigned int gcpId = 1;

      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt,++gcpId)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         // Estimate times
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime);
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
            std::cout<<"GCP #"<<gcpId<<'\n';
            std::cout<<"Azimuth time: ref="<<gcpIt->azimuthTime<<", predicted: "<<estimatedAzimuthTime<<", res="<<to_simple_string(estimatedAzimuthTime-gcpIt->azimuthTime)<<'\n';
            std::cout<<"Slant range time: ref="<<gcpIt->slantRangeTime<<", predicted: "<<estimatedRangeTime<<", res="<<std::abs(estimatedRangeTime - gcpIt->slantRangeTime)<<'\n';
            std::cout<<"Image point: ref="<<gcpIt->imPt<<", predicted="<<estimatedImPt<<", res="<<estimatedImPt-gcpIt->imPt<<'\n';
            std::cout<<'\n';
         }
      }

      if(success)
      {
         std::cout<<"All GCPs within "<<ytol <<" azimuth pixel, "<<xtol<<" range pixel, "<<azTimeTol<<" s of azimuth time, "<<rangeTimeTol<<" of range time\n";
      }

      return success;
   }

   bool ossimSarSensorModel::autovalidateForwardModelFromGCPs(double resTol)
   {
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
               std::cout<<"GCP #"<<gcpId<<'\n';
               std::cout<<"Azimuth time: ref="<<gcpIt->azimuthTime<<", predicted: "<<estimatedAzimuthTime<<", res="<<to_simple_string(estimatedAzimuthTime-gcpIt->azimuthTime)<<'\n';
               std::cout<<"Slant range time: ref="<<gcpIt->slantRangeTime<<", predicted: "<<estimatedRangeTime<<", res="<<std::abs(estimatedRangeTime - gcpIt->slantRangeTime)<<'\n';
               std::cout<<"Im point: "<<gcpIt->imPt<<'\n';
               std::cout<<"World point: ref="<<refPt<<", predicted="<<estimatedWorldPt<<", res="<<sqrt(res)<<" m\n";
               std::cout<<'\n';
            }
         }
      }

      theGCPRecords.swap(gcpRecordSave);

      return success;
   }

   void ossimSarSensorModel::optimizeTimeOffsetsFromGcps()
   {
      double cumulRangeTime(0), cumulAzimuthTime(0);
      unsigned int count=0;

      // First, fix the azimuth time
      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         // Estimate times
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime);

         if(s1)
         {
            cumulAzimuthTime+=-(estimatedAzimuthTime-gcpIt->azimuthTime).total_microseconds();
            ++count;
         }
      }

      theAzimuthTimeOffset= cumulAzimuthTime/=count;

      // Then, fix the range time
      count=0;

      for(std::vector<GCPRecordType>::const_iterator gcpIt = theGCPRecords.begin(); gcpIt!=theGCPRecords.end();++gcpIt)
      {
         ossimDpt estimatedImPt;
         TimeType estimatedAzimuthTime;
         double   estimatedRangeTime;

         // Estimate times
         const bool s1 = this->worldToAzimuthRangeTime(gcpIt->worldPt,estimatedAzimuthTime,estimatedRangeTime);

         if(s1)
         {
            cumulRangeTime+=-estimatedRangeTime+gcpIt->slantRangeTime;
            ++count;
         }
      }

      theRangeTimeOffset = cumulRangeTime/=count;
   }

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::OrbitRecordType> & orbitRecords)
   {
      char orbit_prefix_[256];
      // TODO: read number of bursts ->
      std::size_t nbOrbits;
      get(kwl, "orbitList.nb_orbits", nbOrbits);
      for (std::size_t i=0; i!=nbOrbits ; ++i) {
         const int pos = std::snprintf(orbit_prefix_, sizeof(orbit_prefix_), "orbitList.orbit[%d].", int(i));
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

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::BurstRecordType> & burstRecords)
   {
      char burstPrefix_[1024];
      std::size_t nbBursts ;
      get(kwl, BURST_NUMBER_KEY, nbBursts);
      for (std::size_t burstId=0; burstId!=nbBursts ; ++burstId) {
         const int pos = std::snprintf(burstPrefix_, sizeof(burstPrefix_), "%s[%d].", BURST_PREFIX.c_str(), burstId);
         assert(pos > 0 && pos < sizeof(burstPrefix_));
         const std::string burstPrefix(burstPrefix_, pos);

         ossimSarSensorModel::BurstRecordType burstRecord;
         get(kwl, burstPrefix + keyStartLine,        burstRecord.startLine);
         get(kwl, burstPrefix + keyEndLine,          burstRecord.endLine);
         get(kwl, burstPrefix + keyAzimuthStartTime, burstRecord.azimuthStartTime);
         get(kwl, burstPrefix + keyAzimuthStopTime,  burstRecord.azimuthStopTime);
         burstRecords.push_back(burstRecord);
      }
   }

   void get(
         ossimKeywordlist                             const& kwl,
         std::vector<ossimSarSensorModel::GCPRecordType> & gcpRecords)
   {
      char prefix_[1024];
      std::size_t nbGCPs ;
      get(kwl, GCP_NUMBER_KEY, nbGCPs);
      for (std::size_t gcpId=0; gcpId!=nbGCPs ; ++gcpId) {
         const int pos = std::snprintf(prefix_, sizeof(prefix_), "%s[%d].", GCP_PREFIX.c_str(), gcpId);
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

   void get(
         ossimKeywordlist                                            const& kwl,
         std::string                                                 const& sr_gr_prefix,
         std::string                                                 const& rg0,
         std::vector<ossimSarSensorModel::CoordinateConversionRecordType> & outputRecords)
   {
      char prefix_[1024];
      std::size_t nbCoords ;
      get(kwl, sr_gr_prefix +"."+ NUMBER_KEY, nbCoords);

      for (std::size_t idx=0 ; idx!=nbCoords ; ++idx)
      {
         const int pos = std::snprintf(prefix_, sizeof(prefix_), "%s[%d].", sr_gr_prefix.c_str(), idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix_));
         std::string prefix(prefix_, pos);

         ossimSarSensorModel::CoordinateConversionRecordType coordRecord;
         get(kwl, prefix + keyAzimuthTime,  coordRecord.azimuthTime);
         get(kwl, prefix + rg0,             coordRecord.rg0);

         std::size_t nbCoeffs;
         get(kwl, prefix + NUMBER_KEY,      nbCoeffs);
         for (std::size_t coeff_idx=0; coeff_idx!=nbCoeffs ; ++coeff_idx) {
            const int pos2 = std::snprintf(prefix_+pos, sizeof(prefix_)-pos, "coeff[%d]", coeff_idx);
            assert(pos2 > 0 && pos+pos2 < sizeof(prefix_));
            prefix.assign(prefix_, pos+pos2);
            double coeff;
            get(kwl, prefix, coeff);
            coordRecord.coefs.push_back(coeff);
         }
         outputRecords.push_back(coordRecord);
      }
   }

   bool ossimSarSensorModel::loadState(const ossimKeywordlist& kwl, const char* prefix)
   {
      static const char MODULE[] = "ossimplugins::ossimSarSensorModel::loadState";
      SCOPED_LOG(traceDebug, MODULE);

      ossimSensorModel::loadState(kwl, prefix);

      // And finally, extract data into fields
      std::string product_type_string;
      get(kwl, HEADER_PREFIX + "product_type", product_type_string);
      theProductType = ProductType(product_type_string);

      get(kwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", theNearRangeTime      );
      get(kwl, SUPPORT_DATA_PREFIX, "range_sampling_rate"       , theRangeSamplingRate  );
      get(kwl, SUPPORT_DATA_PREFIX, "range_spacing"             , theRangeResolution    );
      get(kwl, SUPPORT_DATA_PREFIX, "radar_frequency"           , theRadarFrequency     );
      get(kwl, SUPPORT_DATA_PREFIX, "line_time_interval"        , theAzimuthTimeInterval);
      theAzimuthTimeInterval *= 1000000;

      get(kwl, theOrbitRecords);
      // TODO: don't fetch burst records if already read thanks to xml loading
      // that required them
      get(kwl, theBurstRecords);
      if (isGRD())
      {
         get(kwl, SR_PREFIX, keySr0, theSlantRangeToGroundRangeRecords);
         get(kwl, GR_PREFIX, keyGr0, theGroundRangeToSlantRangeRecords);
      }
      get(kwl, theGCPRecords);

      optimizeTimeOffsetsFromGcps();

      return true;
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
      << "\n            theAzimuthTimeInterval: " << theAzimuthTimeInterval << "us"
      << "\n                  theNearRangeTime: " << theNearRangeTime << "s"
      << "\n              theRangeSamplingRate: " << theRangeSamplingRate << "Hz"
      << "\n                theRangeResolution: " << theRangeResolution << "m"
      << "\n       theBistaticCorrectionNeeded: " << theBistaticCorrectionNeeded
      << "\n                    theProductType: " << theProductType
      << "\n              theAzimuthTimeOffset: " << theAzimuthTimeOffset << "us offset"
      << "\n                theRangeTimeOffset: " << theRangeTimeOffset << "s offset"
      << "\n                     theGCPRecords: " << "\n" << theGCPRecords
      << "\n                   theOrbitRecords: " << "\n" << theOrbitRecords
      << "\n                   theBurstRecords: " << "\n" << theBurstRecords
      << "\n theSlantRangeToGroundRangeRecords: " << "\n" << theSlantRangeToGroundRangeRecords
      << "\n theGroundRangeToSlantRangeRecords: " << "\n" << theGroundRangeToSlantRangeRecords
      << "\n"
      ;
}

