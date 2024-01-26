/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbSarSensorModel.h"
#include "otbDEMHandler.h"

#include <numeric>
#include <algorithm>

namespace
{ // Anonymous namespace

double DotProduct(itk::Point<double, 3> const& pt1, itk::Point<double, 3> const& pt2)
{
  // Manual dot product is a bit faster w/ gcc...
  return pt1[0]*pt2[0] + pt1[1]*pt2[1] + pt1[2]*pt2[2];
  // return std::inner_product(pt1.Begin(), pt1.End(), pt2.Begin(), 0.);
}

otb::MetaData::TimePoint time(otb::MetaData::TimePoint t) { return t; }
otb::MetaData::TimePoint time(otb::Orbit const& o) { return o.time; }
otb::MetaData::TimePoint time(otb::CoordinateConversionRecord const& o) { return o.azimuthTime; }

auto const cmp_times = [](auto const& a, auto const& b)
{ return time(a) < time(b);};

// Speed of light
static constexpr double C = 299'792'458;
static constexpr double two_on_C = 2.0 / C;

double SlantRangeToGroundRange(double in, otb::CoordinateConversionRecord const& srgrRecord)
{
  auto const& coeffs = srgrRecord.coeffs;
  assert(!coeffs.empty()&&"Slant range to ground range coefficients vector is empty.");

  const double sr_minus_sr0 =  in-srgrRecord.rg0;
  double res = 0.;

  for(auto cIt = coeffs.crbegin(); cIt!=coeffs.crend(); ++cIt)
  {
    res = *cIt + sr_minus_sr0*res;
  }
  return res;
}

double SlantRangeToGroundRange(
    double in, otb::MetaData::TimePoint azimuthTime,
    otb::CoordinateConversionRecord const& previousRecord,
    otb::CoordinateConversionRecord const& nextRecord)
{
  otb::CoordinateConversionRecord srgrRecord;

  auto const& previous_coeffs = previousRecord.coeffs;
  auto const& next_coeffs     = nextRecord.coeffs;

  assert(!previous_coeffs.empty()&&"previousRecord coefficients vector is empty.");
  assert(!next_coeffs.empty()    &&"nextRecord coefficients vector is empty.");

  // If azimuth time is between 2 records, interpolate
  const double interp
    = (azimuthTime - previousRecord.azimuthTime)
    / (nextRecord.azimuthTime - previousRecord.azimuthTime)
    ;

  auto const rg0 = (1.-interp) * previousRecord.rg0 + interp*nextRecord.rg0;


  // Let's avoid allocation (every time the function is called) to
  // store the interpolated elements. Use instead a single loop that
  // does the Horner polynomial evaluation as it computed each
  // coefficients.
  //
  // Let's keep the algorithm how it used to work: align on the first
  // elements even if there aren't the same number of elements in both
  // lists.
  auto const prev_size   = previous_coeffs.size();
  auto const next_size   = next_coeffs    .size();
  auto const min_size    = std::min(prev_size, next_size);
  assert(min_size>0&&"Slant range to ground range interpolated coefficients vector is empty.");
  auto const prev_offset = prev_size - min_size;
  auto const next_offset = next_size - min_size;

  auto pIt = previous_coeffs.crbegin() + prev_offset;
  auto nIt = next_coeffs    .crbegin() + next_offset;

  auto const sr_minus_sr0 =  in - rg0;

  double res = 0.;

  for(;pIt != previous_coeffs.crend() && nIt != next_coeffs.crend();++pIt,++nIt)
  {
    // interpolated the coeffs
    auto const coeff = interp*(*nIt) + (1.-interp)*(*pIt);
    // compute ground range from slant range (by horner polynomial
    // evaluation on the coeeficcients)
    res = coeff + sr_minus_sr0*res;
  }
  return res;
}

} // Anonymous namespace

namespace otb
{

SarSensorModel::SarSensorModel(
    std::string  productType,
    SARParam  sarParam,
    Projection::GCPParam  gcps,
    unsigned int polynomial_degree)
  : m_ProductType(std::move(productType))
  , m_GCP(std::move(gcps))
  , m_SarParam(std::move(sarParam))
  , m_AzimuthTimeOffset(MetaData::Duration::Seconds(0))
  , m_RangeTimeOffset(0.)
  , m_OrbitInterpolator(m_SarParam.orbits, polynomial_degree)
  , m_polynomial_degree(polynomial_degree)
  , m_EcefToWorldTransform(otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::New())
  , m_WorldToEcefTransform(otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::New())
{
  if (m_GCP.GCPs.empty())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"no GCP found in the input metadata, at least one is required in SARSensorModel");
  }

  OptimizeTimeOffsetsFromGcps();

  const std::vector<std::string> grdProductTypes = {"GRD", "MGD", "GEC", "EEC"};
  m_IsGrd = std::find(grdProductTypes.begin(), grdProductTypes.end(), m_ProductType) != grdProductTypes.end();

}

SarSensorModel::SarSensorModel(const ImageMetadata & imd, unsigned int polynomial_degree)
  : SarSensorModel(
      imd.Has(MDStr::ProductType) ? imd[MDStr::ProductType] : "UNKNOWN",
      boost::any_cast<SARParam>(imd[MDGeom::SAR]),
      imd.GetGCPParam(),
      polynomial_degree)
{
}

void SarSensorModel::WorldToLineSample(const Point3DType& inGeoPoint, Point2DType& outLineSample) const
{
  TimeType azimuthTime;
  double rangeTime;
  Point3DType sensorPos;
  Vector3DType sensorVel;

  WorldToAzimuthRangeTime(inGeoPoint, azimuthTime, rangeTime, sensorPos, sensorVel);

  // Convert azimuth time to line
  outLineSample[1] = AzimuthTimeToLine(azimuthTime);

  if (m_IsGrd)
  { // GRD case
    double const groundRange = SlantRangeToGroundRange(C/2.*rangeTime, azimuthTime);

    // Eq 32 p. 31
    outLineSample[0] = groundRange/m_SarParam.rangeResolution;
  }
  else
  { // SLC case
    // Eq 23 and 24 p. 28
    outLineSample[0] = (rangeTime - m_SarParam.nearRangeTime)
                        * m_SarParam.rangeSamplingRate;
  }
}

SarSensorModel::LineSampleYZ
SarSensorModel::Doppler0ToLineSampleYZ(
    Point3DType const& ecefGround, ZeroDopplerInfo const& zdi,
    double rangeTime) const
{
  LineSampleYZ res {};

  // Convert azimuth time to line
  res.col_row[1] = AzimuthTimeToLine(zdi.azimuthTime);

  if (m_IsGrd)
  { // GRD case
    double const groundRange = SlantRangeToGroundRange(C/2.*rangeTime, zdi.azimuthTime);

    // Eq 32 p. 31
    res.col_row[0] = groundRange/m_SarParam.rangeResolution;
  }
  else
  { // SLC case
    // Eq 23 and 24 p. 28
    res.col_row[0] = (rangeTime - m_SarParam.nearRangeTime)
                        * m_SarParam.rangeSamplingRate;
  }

  // Radar distance
  auto const& sensorPos = zdi.sensorPos;
  auto const& sensorVel = zdi.sensorVel;

  double const NormeS = std::sqrt(DotProduct(sensorPos, sensorPos));

  double const PS2 = DotProduct(ecefGround, sensorPos);

  assert(NormeS>1e-6);
  res.yz[1] = NormeS - PS2/NormeS;

  auto const grd_sat_vector = sensorPos - ecefGround;
  double const distance2 = DotProduct(grd_sat_vector, grd_sat_vector);
  res.yz[0] = std::sqrt(distance2 - res.yz[1] * res.yz[1]);

  // Check view side and change sign of Y accordingly
  if ( (( sensorVel[0] * (sensorPos[1]* ecefGround[2] - sensorPos[2]* ecefGround[1]) +
          sensorVel[1] * (sensorPos[2]* ecefGround[0] - sensorPos[0]* ecefGround[2]) +
          sensorVel[2] * (sensorPos[0]* ecefGround[1] - sensorPos[1]* ecefGround[0])) > 0) ^ m_SarParam.rightLookingFlag )
  {
    res.yz[0] = -res.yz[0];
  }

  return res;
}

void SarSensorModel::WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const
{
  TimeType azimuthTime;
  double rangeTime;
  Point3DType sensorPos;
  Vector3DType sensorVel;

  // WorldToAzimuthRangeTime always returns true..
  WorldToAzimuthRangeTime(inGeoPoint, azimuthTime, rangeTime, sensorPos, sensorVel);

  // Convert azimuth time to line
  cr[1] = AzimuthTimeToLine(azimuthTime);

  if (m_IsGrd)
  {
    // GRD case
    double const groundRange = SlantRangeToGroundRange(C/2.*rangeTime, azimuthTime);

    // Eq 32 p. 31
    cr[0] = groundRange/m_SarParam.rangeResolution;
  }
  else
  {
    // SLC case
    // Eq 23 and 24 p. 28
    cr[0] = (rangeTime - m_SarParam.nearRangeTime)
                        * m_SarParam.rangeSamplingRate;
  }

  auto inputPt = WorldToEcef(inGeoPoint);

  // Radar distance
  double const NormeS = std::sqrt(DotProduct(sensorPos, sensorPos));

  double const PS2 = DotProduct(inputPt, sensorPos);

  assert(NormeS>1e-6);
  yz[1] = NormeS - PS2/NormeS;

  auto const grd_sat_vector = sensorPos - inputPt;
  double const distance2 = DotProduct(grd_sat_vector, grd_sat_vector);
  yz[0] = std::sqrt(distance2 - yz[1] * yz[1]);

  // Check view side and change sign of Y accordingly
  if ( (( sensorVel[0] * (sensorPos[1]* inputPt[2] - sensorPos[2]* inputPt[1]) +
              sensorVel[1] * (sensorPos[2]* inputPt[0] - sensorPos[0]* inputPt[2]) +
              sensorVel[2] * (sensorPos[0]* inputPt[1] - sensorPos[1]* inputPt[0])) > 0) ^ m_SarParam.rightLookingFlag )
  {
    yz[0] = -yz[0];
  }
}

double SarSensorModel::CalculateRangeTime(
    Point3DType const& ecefGround, Point3DType const& sensorPos) const
{
  //TODO Bistatic correction when needed
  double const rangeDistance = sensorPos.EuclideanDistanceTo(ecefGround);
  return m_RangeTimeOffset + two_on_C * rangeDistance; // This impacts precision
}

bool SarSensorModel::WorldToAzimuthRangeTime(
    const Point3DType& inGeoPoint,
    TimeType & azimuthTime,
    double & rangeTime,
    Point3DType& sensorPos,
    Vector3DType& sensorVel) const
{
  auto const ecefPoint = WorldToEcef(inGeoPoint);

  auto const zdi = ZeroDopplerLookup(ecefPoint);
  rangeTime   = CalculateRangeTime(ecefPoint, zdi.sensorPos);

  azimuthTime = zdi.azimuthTime;
  sensorPos   = zdi.sensorPos;
  sensorVel   = zdi.sensorVel;

  return true;
}

bool SarSensorModel::WorldToSatPositionAndVelocity(const Point3DType& inGeoPoint,
                                                   Point3DType& satellitePosition,
                                                   Point3DType& satelliteVelocity) const
{
  TimeType azimuthTime;
  double   rangeTime;

  return WorldToAzimuthRangeTime(inGeoPoint,
                                 azimuthTime,
                                 rangeTime,
                                 satellitePosition,
                                 satelliteVelocity);
}


void SarSensorModel::LineSampleHeightToWorld(
    const Point2DType& imPt,
    double  heightAboveEllipsoid,
    Point3DType& worldPt) const
{
  assert(m_GCP.GCPs.size());

  const auto& gcp = findClosestGCP(imPt, m_GCP);

  auto heightFunction = [heightAboveEllipsoid](const double, const double)
                        {return heightAboveEllipsoid;};

  // Simple iterative inversion of inverse model starting at closest gcp
  worldPt = EcefToWorld(projToSurface(gcp, imPt, heightFunction));
}

void SarSensorModel::LineSampleToWorld(
    const Point2DType& imPt, Point3DType& worldPt) const
{
  assert(m_GCP.GCPs.size());

  const auto& gcp = findClosestGCP(imPt, m_GCP);

  Point3DType ecefPoint;

  auto heightFunction = [](const double lon, const double lat)
                        {return DEMHandler::GetInstance().GetHeightAboveEllipsoid(lon, lat);};

  // Simple iterative inversion of inverse model starting at closest gcp
  worldPt = EcefToWorld(projToSurface(gcp, imPt, heightFunction));
}

std::tuple<SarSensorModel::TimeType, SarSensorModel::OrbitIterator, SarSensorModel::OrbitIterator>
SarSensorModel::ZeroDopplerTimeLookupInternal(Point3DType const& inEcefPoint) const
{
  if (m_SarParam.orbits.size() < 2)
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Orbit records vector contains less than 2 elements");
  }

  auto it = m_SarParam.orbits.cbegin();

  // Compute range and doppler of first record
  // NOTE: here we only use the scalar product with vel and discard
  // the constant coef as it has no impact on doppler sign

  double doppler1 = DotProduct(inEcefPoint - it->position, it->velocity);

  bool dopplerSign1 = doppler1 < 0;

  ++it;
  double doppler2 = 0.;

  // Look for the consecutive records where doppler freq changes sign
  // Note: implementing a bisection algorithm here might be faster
  for ( ; it != m_SarParam.orbits.cend() ; ++it)
  {
     // compute range and doppler of current record
     doppler2 = DotProduct(inEcefPoint - it->position, it->velocity);

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
  if(it == m_SarParam.orbits.cend())
  {
    //TODO test this case
    auto record1 = m_SarParam.orbits.cbegin();
    auto record2 = record1 + m_SarParam.orbits.size()-1;
    doppler1 = DotProduct(inEcefPoint - record1->position, record1->velocity);
    doppler2 = DotProduct(inEcefPoint - record2->position, record2->velocity);
    const DurationType delta_td = record2->time - record1->time;

    return {
      record1->time - doppler1 / (doppler2 - doppler1) * delta_td,
      record1, record2
    };
  }
  else
  {
    assert(it != m_SarParam.orbits.cbegin());
    assert(it != m_SarParam.orbits.cend());
    auto record2 = it;
    auto record1 = --it;
    // now interpolate time and sensor position
    const double abs_doppler1 = std::abs(doppler1);
    const double interpDenom = abs_doppler1+std::abs(doppler2);
    assert(interpDenom>0&&"Both doppler frequency are null in interpolation weight computation");
    const double interp = abs_doppler1/interpDenom;
    const DurationType delta_td = record2->time - record1->time;
    // Compute interpolated time offset wrt record1
    // (No need for that many computations (day-frac -> ms -> day frac))
    const DurationType td     = delta_td * interp;

    // Compute interpolated azimuth time
    return { record1->time + td + m_AzimuthTimeOffset, record1, record2 };
  }
}

SarSensorModel::TimeType
SarSensorModel::ZeroDopplerTimeLookup(Point3DType const& inEcefPoint) const
{
  return std::get<0>(ZeroDopplerTimeLookupInternal(inEcefPoint));
}

SarSensorModel::ZeroDopplerInfo
SarSensorModel::ZeroDopplerLookup(Point3DType const& inEcefPoint) const
{
  TimeType      azimuthTime;
  OrbitIterator itRecord1, itRecord2;
  std::tie(azimuthTime, itRecord1, itRecord2)
    = ZeroDopplerTimeLookupInternal(inEcefPoint);

  // Interpolate sensor position and velocity
  Point3DType  sensorPos;
  Vector3DType sensorVel;
  if (Abs(time(*itRecord1) - azimuthTime) > Abs(time(*itRecord2) - azimuthTime))
  { // While this may degrade results, this makes sure to keep identical
    // results to previous code
    // Ideally, we should center around the interval [record1, record2]
    ++itRecord1;
  }
  std::tie(sensorPos, sensorVel) = interpolateSensorPosVel(azimuthTime, itRecord1);

  ZeroDopplerInfo res{azimuthTime, sensorPos, sensorVel};
  return res;
}

SarSensorModel::OrbitIterator
SarSensorModel::searchLagrangianNeighbourhood(TimeType azimuthTime) const
{
  // If there are less records than degrees, use them all
  if(m_SarParam.orbits.size() < m_polynomial_degree)
  {
    // nEnd = m_SarParam.orbits.size()-1;
    return m_SarParam.orbits.end();
  }
  else
  { // TODO: use std::upper_bound
    // Search for the deg number of records around the azimuth time
    unsigned int t_min_idx = 0;
    auto t_min = Abs(azimuthTime - m_SarParam.orbits.front().time);
    unsigned int count = 0;

    // Unfortunatelly, we cannot loop from m_FirstLineInOrbit to
    // m_LastLineInOrbit as there is no guarantee the azimuthTime happens
    // outside this range => we must search the whole orbit information.
    // for(auto it = m_FirstLineInOrbit; it!= m_LastLineInOrbit; ++it,++count)
     for(auto it = m_SarParam.orbits.cbegin();it!= m_SarParam.orbits.cend();++it,++count)
     {
        const auto current_time = Abs(azimuthTime-it->time);
        if(t_min > current_time)
        {
        // TODO: break when the distance is growing.
           t_min_idx = count;
           t_min = current_time;
        }
     }

    return m_SarParam.orbits.begin() + t_min_idx;
  }
}

std::pair<SarSensorModel::Point3DType, SarSensorModel::Vector3DType>
SarSensorModel::interpolateSensorPosVel(TimeType azimuthTime, OrbitIterator itRecord1) const
{
  assert(m_SarParam.orbits.size() &&"The orbit records vector is empty");
  auto deg = m_polynomial_degree;

  auto t_min_idx = std::distance(m_SarParam.orbits.begin(), itRecord1);

  // TODO: see if these expressions can be simplified
  std::size_t nBegin = std::max(t_min_idx-(int)deg/2+1, std::ptrdiff_t{});
  std::size_t nEnd   = std::min(nBegin+deg-1, m_SarParam.orbits.size());
  nBegin = nEnd<m_SarParam.orbits.size()-1 ? nBegin : nEnd-deg+1;

  // If there are less records than degrees, use them all
  if(m_SarParam.orbits.size() < deg)
  {
    nBegin = 0;
    nEnd   = m_SarParam.orbits.size()-1;
  }

  // Compute lagrangian interpolation using records from nBegin to nEnd
  assert(nEnd - nBegin < 30); // Lagrangian interpolator fails miserably at 20
                              // elements... Let's expected less than 30 as
                              // reasonable
  assert(nBegin != nEnd);

  return m_OrbitInterpolator.interpolatePosVel(azimuthTime, nBegin, nEnd);
}

std::pair<SarSensorModel::Point3DType, SarSensorModel::Vector3DType>
SarSensorModel::interpolateSensorPosVel(TimeType azimuthTime) const
{
  auto const closest_osv = searchLagrangianNeighbourhood(azimuthTime);
  return interpolateSensorPosVel(azimuthTime, closest_osv);
}

void SarSensorModel::OptimizeTimeOffsetsFromGcps()
{
  assert(m_GCP.GCPs.size());

  DurationType cumulAzimuthTime(DurationType::Seconds(0));
  unsigned int count=0;

  // reset offsets before optimisation
  m_AzimuthTimeOffset = DurationType::Seconds(0);

  // First, fix the azimuth time
  for(auto const& gcp : m_GCP.GCPs)
  {
    auto gcpTimeIt = m_SarParam.gcpTimes.find(gcp.m_Id);
    if (gcpTimeIt != std::end(m_SarParam.gcpTimes))
    {
      Point3DType inWorldPoint;
      inWorldPoint[0] = gcp.m_GCPX;
      inWorldPoint[1] = gcp.m_GCPY;
      inWorldPoint[2] = gcp.m_GCPZ;

      // Estimate times
      auto const ecefPoint = WorldToEcef(inWorldPoint);
      TimeType const estimatedAzimuthTime = ZeroDopplerTimeLookup(ecefPoint);

      cumulAzimuthTime -= (estimatedAzimuthTime - gcpTimeIt->second.azimuthTime);
      ++count;
    }
  }
  m_AzimuthTimeOffset = count > 0 ? cumulAzimuthTime / count : DurationType(MetaData::Duration::Seconds(0));
}

double SarSensorModel::AzimuthTimeToLine(const TimeType & azimuthTime) const
{
  assert(!m_SarParam.burstRecords.empty()&&"Burst records are empty (at least one burst should be available)");

  auto currentBurst = m_SarParam.burstRecords.cbegin();

  // Look for the correct burst. In most cases the number of burst
  // records will be 1 (except for TOPSAR Sentinel1 products)
  auto it = m_SarParam.burstRecords.cbegin();
  auto itend = m_SarParam.burstRecords.cend();

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
    if(! m_SarParam.burstRecords.empty())
    {
      if(azimuthTime < m_SarParam.burstRecords.front().azimuthStartTime)
      {
        currentBurst = m_SarParam.burstRecords.cbegin();
      }
      else if (azimuthTime > m_SarParam.burstRecords.back().azimuthStopTime)
      {
        currentBurst = m_SarParam.burstRecords.cend()-1;
      }
    }
    else
    {
      // Fall back to the only record
      currentBurst = m_SarParam.burstRecords.cbegin();
    }
  }

  const DurationType timeSinceStart = azimuthTime - currentBurst->azimuthStartTime;

  // Eq 22 p 27
  return (timeSinceStart/m_SarParam.azimuthTimeInterval) + currentBurst->startLine;
}

double SarSensorModel::SlantRangeToGroundRange(double slantRange, const TimeType & azimuthTime) const
{
  return ApplyCoordinateConversion(slantRange, azimuthTime, m_SarParam.slantRangeToGroundRangeRecords);
}

double SarSensorModel::ApplyCoordinateConversion(
    double in,
    const TimeType& azimuthTime,
    const std::vector<CoordinateConversionRecord> & records) const
{
  assert(!records.empty()&&"The records vector is empty.");

  // First, we need to find the correct pair of records for interpolation
  // Look for the correct record
  auto const wh = std::upper_bound(
      std::begin(records), std::end(records),
      azimuthTime, cmp_times);

  if (wh == records.cend())
  {
    assert(azimuthTime > records.back().azimuthTime);
    return ::SlantRangeToGroundRange(in, records.back());
  }
  else if (wh == records.cbegin())
  {
    assert(azimuthTime < records.front().azimuthTime);
    return ::SlantRangeToGroundRange(in, records.front());
  }
  else
  {
    auto const previousRecord = wh-1;
    auto const nextRecord     = wh;
    assert(previousRecord->azimuthTime <= azimuthTime);
    assert(nextRecord    ->azimuthTime >  azimuthTime);
    return ::SlantRangeToGroundRange(in, azimuthTime, *previousRecord, *nextRecord);
  }
}

const GCP & SarSensorModel::findClosestGCP(const Point2DType& imPt, const Projection::GCPParam & gcpParam) const
{
  assert(gcpParam.GCPs.size() > 0);
  const GCP* closest ;

  // Squared distance between a Point and a gcp
  auto squaredDistance = [](const Point2DType & imPt, const GCP & gcp)
    {
      double dx = imPt[0] - gcp.m_GCPCol;
      double dy = imPt[1] - gcp.m_GCPRow;

      return dx*dx + dy*dy;
    };

  double minDistance = std::numeric_limits<double>::max();

  for (const auto & elem : gcpParam.GCPs)
  {
    auto currentDistance = squaredDistance(imPt, elem);
    if (currentDistance < minDistance)
    {
      minDistance = currentDistance;
      closest = &elem;
    }
  }

  return *closest;
}

SarSensorModel::Point3DType SarSensorModel::projToSurface(
    const GCP & gcp, const Point2DType& imPt, std::function<double(double, double)> heightFunction) const
{
  // Stop condition: img residual < 1e-2 pixels, height residual² <
  // 0.01² m, nb iter < 50. the loop runs at least once.
  const int maxIter = 50; //50
  const double imgResidual = 1e-2;
  const double heightResidual = 1e-2;

  using MatrixType = itk::Matrix<double, 3, 3>;
  using VectorType = itk::Vector<double, 3>;

  Point3DType groundGcp;
  groundGcp[0] = gcp.m_GCPX;
  groundGcp[1] = gcp.m_GCPY;
  groundGcp[2] = gcp.m_GCPZ;

  // Initialize current estimation
  auto currentEstimation = WorldToEcef(groundGcp);

  // Corresponding image position
  Point2DType currentImPoint;
  currentImPoint[0] = gcp.m_GCPCol;
  currentImPoint[1] = gcp.m_GCPRow;

  auto currentImSquareResidual = imPt.SquaredEuclideanDistanceTo(currentImPoint);

  double currentHeightResidual = groundGcp[2] - heightFunction(groundGcp[0], groundGcp[1]);

  MatrixType B, BtB;
  VectorType BtF;
  VectorType F;
  VectorType dR;

  // Delta use for partial derivatives estimation (in meters)
  const double d = 10.;

  for (int i = 0; i < maxIter; i++)
  {
    // compute residuals
    F[0] = imPt[0] - currentImPoint[0];
    F[1] = imPt[1] - currentImPoint[1];
    F[2] = currentHeightResidual;

    // Compute partial derivatives
    VectorType p_fx, p_fy, p_fh, dx(0.) ,dy(0.), dz(0.);
    dx[0] = d;
    dy[1] = d;
    dz[2] = d;

    Point2DType tmpImPt;

    auto currentEstimationWorld = EcefToWorld(currentEstimation);

    auto tmpGpt = EcefToWorld(currentEstimation + dx);
    WorldToLineSample(tmpGpt, tmpImPt);

    p_fx[0] = (currentImPoint[0] - tmpImPt[0])/d;
    p_fy[0] = (currentImPoint[1] - tmpImPt[1])/d;
    p_fh[0] = (currentEstimationWorld[2] - tmpGpt[2])/d;

    tmpGpt = EcefToWorld(currentEstimation + dy);
    WorldToLineSample(tmpGpt,tmpImPt);


    p_fx[1] = (currentImPoint[0] - tmpImPt[0])/d;
    p_fy[1] = (currentImPoint[1] - tmpImPt[1])/d;
    p_fh[1] = (currentEstimationWorld[2] - tmpGpt[2])/d;

    tmpGpt = EcefToWorld(currentEstimation + dz);
    WorldToLineSample(tmpGpt,tmpImPt);
    p_fx[2] = (currentImPoint[0] - tmpImPt[0])/d;
    p_fy[2] = (currentImPoint[1] - tmpImPt[1])/d;
    p_fh[2] = (currentEstimationWorld[2] - tmpGpt[2])/d;

    B(0,0) = p_fx[0];
    B(0,1) = p_fx[1];
    B(0,2) = p_fx[2];
    B(1,0) = p_fy[0];
    B(1,1) = p_fy[1];
    B(1,2) = p_fy[2];
    B(2,0) = p_fh[0];
    B(2,1) = p_fh[1];
    B(2,2) = p_fh[2];

    // Invert system
    try
    {
      dR = MatrixType(B.GetInverse()) * F;
    }
    catch (itk::ExceptionObject const& e)
    {
      otbLogMacro(Warning, << "SarSensorModel::projToSurface(): singular matrix can not be inverted. Returning best estimation so far("
                           << currentEstimation
                           << ") for output point ("
                           << imPt << ")" << std::endl);
      return currentEstimation;
    }

    currentEstimation -= dR;

    currentEstimationWorld = EcefToWorld(currentEstimation);

    currentImSquareResidual = imPt.SquaredEuclideanDistanceTo(currentImPoint);

    const double atHgt = heightFunction(currentEstimationWorld[0], currentEstimationWorld[1]);
    currentHeightResidual = atHgt - currentEstimationWorld[2];

    WorldToLineSample(currentEstimationWorld, currentImPoint);

    if (currentImSquareResidual <= imgResidual && currentHeightResidual <= heightResidual)
    {
      break;
    }
  }

  return currentEstimation;
}

bool SarSensorModel::Deburst(std::vector<std::pair<unsigned long, unsigned long>>& lines,
                             std::pair<unsigned long, unsigned long>& samples,
                             bool onlyValidSample)
{
  auto & burstRecords = m_SarParam.burstRecords;
  if (burstRecords.empty())
  {
    return false;
  }

  // First, clear lines record
  lines.clear();

  // Check the single burst record case
  if(burstRecords.size() == 1)
  {
    lines.push_back(std::make_pair(burstRecords.front().startLine, burstRecords.front().endLine));
    return false;
  }

  // Process each burst
  auto it = burstRecords.cbegin();
  // Correct since we have at least 2 bursts records
  auto next = it + 1;

  unsigned long currentStart  = it->startLine;
  TimeType deburstAzimuthStartTime = it->azimuthStartTime;
  double deburstAzimuthAnxTime = it->azimuthAnxTime;
  unsigned long deburstEndLine = 0;
  samples = std::make_pair(it->startSample, it->endSample);

  for(; next != burstRecords.cend() ;++it,++next)
  {
    DurationType timeOverlapEnd = (it->azimuthStopTime - next->azimuthStartTime);

    unsigned long overlapLength = timeOverlapEnd/m_SarParam.azimuthTimeInterval;
    unsigned long halfLineOverlapEnd = overlapLength/2;
    TimeType endTimeInNextBurst = it->azimuthStopTime-(halfLineOverlapEnd-1)*m_SarParam.azimuthTimeInterval;

    unsigned long halfLineOverlapBegin = std::floor(0.5+(endTimeInNextBurst-next->azimuthStartTime)/m_SarParam.azimuthTimeInterval);

    unsigned long currentStop = it->endLine-halfLineOverlapEnd;

    deburstEndLine+= currentStop - currentStart + 1; // +1 because currentStart/Stop are both valids

    lines.push_back(std::make_pair(currentStart,currentStop));

    currentStart = next->startLine+halfLineOverlapBegin;

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

  TimeType deburstAzimuthStopTime = it->azimuthStopTime;
  deburstEndLine+=it->endLine-currentStart;

  lines.push_back(std::make_pair(currentStart,it->endLine));

  if (it->startSample > samples.first)
  {
    samples.first = it->startSample;
  }
  if (it->endSample < samples.second)
  {
    samples.second = it->endSample;
  }

  // Clear the previous burst records
  burstRecords.clear();

  // Create the single burst
  BurstRecord deburstBurst;
  deburstBurst.startLine = 0;
  deburstBurst.azimuthStartTime = deburstAzimuthStartTime;
  deburstBurst.endLine = deburstEndLine;
  deburstBurst.azimuthStopTime = deburstAzimuthStopTime;
  deburstBurst.azimuthAnxTime = deburstAzimuthAnxTime;

  if (onlyValidSample)
  {
    deburstBurst.startSample = 0;
    deburstBurst.endSample = samples.second - samples.first;
  }
  else
  {
    deburstBurst.startSample = samples.first;
    deburstBurst.endSample = samples.second;
  }

  burstRecords.push_back(deburstBurst);

  // Now move GCPs

  std::vector<GCP> deburstGCPs;

  for (auto gcp : m_GCP.GCPs)
  {
    unsigned long newLine=0;

    unsigned long gcpLine = std::floor(gcp.m_GCPRow + 0.5);
    unsigned long gcpSample = std::floor(gcp.m_GCPCol + 0.5);

    // Be careful about fractional part of GCPs
    double fractionalLines = gcp.m_GCPRow - gcpLine;
    double fractionalSamples = gcp.m_GCPCol - gcpSample;

    bool linesOk = ImageLineToDeburstLine(lines, gcpLine, newLine);

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
      gcp.m_GCPRow = newLine + fractionalLines;
      gcp.m_GCPCol = newSample + fractionalSamples;
      deburstGCPs.push_back(gcp);
    }
  }

  m_GCP.GCPs.swap(deburstGCPs);

  // redaptMedataAfterDeburst = true;
  m_FirstLineTime = deburstBurst.azimuthStartTime;
  m_LastLineTime = deburstBurst.azimuthStopTime;

  if (onlyValidSample)
    m_SarParam.nearRangeTime += samples.first / m_SarParam.rangeSamplingRate;


  return true;
}


bool SarSensorModel::BurstExtraction(const unsigned int burst_index,
                                     std::pair<unsigned long,unsigned long> & lines,
                                     std::pair<unsigned long,unsigned long> & samples,
                                     bool allPixels)
{
  auto & burstRecords = m_SarParam.burstRecords;
  if (burstRecords.empty())
  {
    return false;
  }

  // Check the single burst record case
  if(burstRecords.size() == 1)
  {
    lines = {burstRecords.front().startLine, burstRecords.front().endLine};
    return false;
  }

  if (allPixels)
  {
    samples = {0, m_SarParam.numberOfSamplesPerBurst - 1};
    lines = {burst_index*m_SarParam.numberOfLinesPerBurst,
                           (burst_index+1)*m_SarParam.numberOfLinesPerBurst - 1};

    //redaptMedataAfterDeburst = true;
    m_FirstLineTime = burstRecords[burst_index].azimuthStartTime - (burstRecords[burst_index].startLine - lines.first)
                       * m_SarParam.azimuthTimeInterval;
    m_LastLineTime = m_FirstLineTime + (lines.second - lines.first) * m_SarParam.azimuthTimeInterval;

    // Clear the previous burst records
    burstRecords.clear();

    // Create the single burst
    BurstRecord burst;
    burst.startLine = 0;
    burst.azimuthStartTime = m_FirstLineTime;
    burst.endLine = lines.second - lines.first;
    burst.azimuthStopTime = m_LastLineTime;
    burst.startSample = 0;
    burst.endSample = samples.second - samples.first;
    burst.azimuthAnxTime = 0;

    burstRecords.push_back(burst);
  }
  else
  {
    // Retrieve into TheBurstRecord, the required index
    auto burstInd_Record = burstRecords[burst_index];
    lines = {burstInd_Record.startLine, burstInd_Record.endLine};
    samples = {burstInd_Record.startSample, burstInd_Record.endSample};
    auto burstAzimuthStartTime = burstInd_Record.azimuthStartTime;
    auto burstAzimuthStopTime = burstInd_Record.azimuthStopTime;

    // Clear the previous burst records
    burstRecords.clear();

    // Create the single burst
    BurstRecord oneBurst;
    oneBurst.startLine = 0;
    oneBurst.azimuthStartTime = burstAzimuthStartTime;
    oneBurst.endLine = lines.second - lines.first;
    oneBurst.azimuthStopTime = burstAzimuthStopTime;
    oneBurst.startSample = 0;
    oneBurst.endSample = samples.second - samples.first;
    oneBurst.azimuthAnxTime = burstInd_Record.azimuthAnxTime;

    burstRecords.push_back(oneBurst);

    // Adapt general metadata : theNearRangeTime, first_time_line, last_time_line
    //redaptMedataAfterDeburst = true;
    m_FirstLineTime = oneBurst.azimuthStartTime;
    m_LastLineTime = oneBurst.azimuthStopTime;
    m_SarParam.nearRangeTime += samples.first / m_SarParam.rangeSamplingRate;
  }

  std::vector<GCP> oneBurstGCPs;

  // Now move GCPs
  for (auto const& token : m_GCP.GCPs)
  {
    auto currentGCP = token;

    unsigned long gcpLine = std::floor(currentGCP.m_GCPRow + 0.5);
    unsigned long gcpSample = std::floor(currentGCP.m_GCPCol + 0.5);

    // Be careful about fractional part of GCPs
    double fractionalLines = currentGCP.m_GCPRow - gcpLine;
    double fractionalSamples = currentGCP.m_GCPCol - gcpSample;

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
      currentGCP.m_GCPRow = newLine + fractionalLines;
      currentGCP.m_GCPCol = newSample + fractionalSamples;
      oneBurstGCPs.push_back(currentGCP);
    }
  }

  // emit a warning if there are no GCP inside the burst, as it makes the output model unusable
  // See Gitlab issue #2230
  if (oneBurstGCPs.empty())
  {
    otbLogMacro(Warning, << "There are no GCP inside the extracted burst")
  }

  m_GCP.GCPs.swap(oneBurstGCPs);

  return true;

}

bool SarSensorModel::DeburstAndConcatenate(std::vector<std::pair<unsigned long,unsigned long> >& linesBursts,
                                           std::vector<std::pair<unsigned long,unsigned long> >& samplesBursts,
                                           unsigned int & linesOffset,
                                           unsigned int first_burstInd,
                                           bool inputWithInvalidPixels)
{
  auto & burstRecords = m_SarParam.burstRecords;
  if (burstRecords.empty())
  {
    return false;
  }

  // declare lines and samples
  std::vector<std::pair<unsigned long,unsigned long> > lines;
  lines.reserve(burstRecords.size());
  std::pair<unsigned long,unsigned long> samples;

  // First, clear lines record
  linesBursts.clear();
  samplesBursts.clear();

  // Check the single burst record case
  if(burstRecords.size() == 1)
  {
    linesBursts.push_back({burstRecords.front().startLine, burstRecords.front().endLine});
    samplesBursts.push_back({burstRecords.front().startLine, burstRecords.front().endLine});
    return false;
  }

  ///////// deburst operation ////////
  // Process each burst
  auto it = burstRecords.cbegin();
  // Correct since we have at least 2 bursts records
  auto next = it+1;

  unsigned long currentStart  = it->startLine;
  TimeType deburstAzimuthStartTime = it->azimuthStartTime;
  double deburstAzimuthAnxTime = it->azimuthAnxTime;

  unsigned long deburstEndLine = 0;
  samples = {it->startSample, it->endSample};

  // Store halfLineOverlapBegin/End
  std::vector<unsigned long> halfLineOverlapBegin(burstRecords.size());
  std::vector<unsigned long> halfLineOverlapEnd(burstRecords.size());

  halfLineOverlapBegin[0] = 0;

  unsigned int counterBegin = 1;
  unsigned int counterEnd = 0;

  for(; next!= burstRecords.cend() ;++it,++next)
  {
    DurationType timeOverlapEnd = (it->azimuthStopTime - next->azimuthStartTime);
    unsigned long overlapLength = timeOverlapEnd / m_SarParam.azimuthTimeInterval;

    halfLineOverlapEnd[counterEnd] = overlapLength/2;
    TimeType endTimeInNextBurst = it->azimuthStopTime-(halfLineOverlapEnd[counterEnd]-1)*m_SarParam.azimuthTimeInterval;

    halfLineOverlapBegin[counterBegin] = std::floor(0.5+(endTimeInNextBurst-next->azimuthStartTime)/m_SarParam.azimuthTimeInterval);

    unsigned long currentStop = it->endLine-halfLineOverlapEnd[counterEnd];

    deburstEndLine+= currentStop - currentStart + 1; // +1 because currentStart/Stop are both valids

    lines.push_back({currentStart,currentStop});

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

  halfLineOverlapEnd[burstRecords.size() - 1] = 0;

  TimeType deburstAzimuthStopTime = it->azimuthStopTime;
  deburstEndLine+=it->endLine - currentStart;

  lines.push_back({currentStart,it->endLine});

  if (it->startSample > samples.first)
  {
    samples.first = it->startSample;
  }
  if (it->endSample < samples.second)
  {
    samples.second = it->endSample;
  }

  // Now, update other metadata accordingly
  std::vector<GCP> deburstGCPs;

  for (auto gcp : m_GCP.GCPs)
  {
    unsigned long newLine=0;

    unsigned long gcpLine = std::floor(gcp.m_GCPRow + 0.5);
    unsigned long gcpSample = std::floor(gcp.m_GCPCol + 0.5);

    // Be careful about fractional part of GCPs
    double fractionalLines = gcp.m_GCPRow - gcpLine;
    double fractionalSamples = gcp.m_GCPCol - gcpSample;

    bool linesOk = ImageLineToDeburstLine(lines,gcpLine,newLine);

    // Gcp into valid samples
    unsigned long newSample = gcpSample;

    if (linesOk && gcpSample >= samples.first && gcpSample <= samples.second)
    {
      newSample -= samples.first; // Offset with first valid sample
      gcp.m_GCPRow = newLine + fractionalLines;
      gcp.m_GCPCol = newSample + fractionalSamples;
      deburstGCPs.push_back(gcp);
     }

  }

  m_GCP.GCPs.swap(deburstGCPs);

  ///// linesBursts and samplesBursts (into Burst geometry) /////

  unsigned int counter = 0;

  for (const auto & burst: burstRecords)
  {
    unsigned long currentStart_L = halfLineOverlapBegin[counter];

    if (inputWithInvalidPixels)
    {
      currentStart_L += burst.startLine - counter * m_SarParam.numberOfLinesPerBurst;
    }

    unsigned long currentStop_L = burst.endLine - burst.startLine - halfLineOverlapEnd[counter];

    if (inputWithInvalidPixels)
    {
      currentStop_L = burst.endLine - counter * m_SarParam.numberOfLinesPerBurst - halfLineOverlapEnd[counter];
    }

    linesBursts.push_back({currentStart_L, currentStop_L});

    unsigned long currentStart_S = 0;
    unsigned long currentStop_S = samples.second-samples.first;

    if (inputWithInvalidPixels)
    {
      currentStart_S = samples.first;
    }
    else if (burst.startSample < samples.first)
    {
      currentStart_S = samples.first - burst.startSample;
    }

    currentStop_S += currentStart_S;

    samplesBursts.push_back({currentStart_S, currentStop_S});

    ++counter;
  }

  // Define linesOffset
  linesOffset = (burstRecords[first_burstInd].azimuthStartTime - burstRecords[0].azimuthStartTime)/m_SarParam.azimuthTimeInterval;

  // Clear the previous burst records
  burstRecords.clear();

  // Create the single burst
  BurstRecord deburstBurst;
  deburstBurst.startLine = 0;
  deburstBurst.azimuthStartTime = deburstAzimuthStartTime;
  deburstBurst.endLine = deburstEndLine;
  deburstBurst.azimuthStopTime = deburstAzimuthStopTime;
  deburstBurst.startSample = 0;
  deburstBurst.endSample = samples.second - samples.first;
  deburstBurst.azimuthAnxTime = deburstAzimuthAnxTime;

  burstRecords.push_back(deburstBurst);

  // Adapt general metadata : theNearRangeTime, first_time_line, last_time_line
  //redaptMedataAfterDeburst = true;
  m_FirstLineTime = deburstBurst.azimuthStartTime;
  m_LastLineTime = deburstBurst.azimuthStopTime;

  m_SarParam.nearRangeTime += samples.first*(1/m_SarParam.rangeSamplingRate);

  return true;
}

bool SarSensorModel::Overlap(std::pair<unsigned long, unsigned long>& linesUp, std::pair<unsigned long, unsigned long>& linesLow,
               std::pair<unsigned long, unsigned long>& samplesUp, std::pair<unsigned long, unsigned long>& samplesLow, unsigned int burstIndUp,
               bool inputWithInvalidPixels)
{
  auto & burstRecords = m_SarParam.burstRecords;

  // Check the no burst record case, the single burst record case or inferior to burstIndUp + 1
  if(burstRecords.size() <= 1 || burstRecords.size() <= ( burstIndUp + 1))
  {
    return false;
  }

  auto & burstUp = burstRecords[burstIndUp];
  auto & burstLow = burstRecords[burstIndUp+1];

  // Overlap for samples (valid samples)
  std::pair<unsigned long,unsigned long> samples = {burstUp.startSample, burstUp.endSample};

  if (burstLow.startSample > samples.first)
  {
    samples.first = burstLow.startSample;
  }
  if (burstLow.endSample < samples.second)
  {
    samples.second = burstLow.endSample;
  }

  unsigned long currentStartUp_S = 0;
  unsigned long currentStartLow_S = 0;
  unsigned long currentStopUp_S = samples.second-samples.first;
  unsigned long currentStopLow_S = samples.second-samples.first;

  if (inputWithInvalidPixels)
  {
    currentStartUp_S = samples.first;
    currentStartLow_S = samples.first;
  }
  else
  {
    if (burstUp.startSample < samples.first)
    {
      currentStartUp_S = samples.first - burstUp.startSample;
    }
    if (burstLow.startSample < samples.first)
    {
      currentStartLow_S = samples.first - burstLow.startSample;
    }
  }

  currentStopUp_S += currentStartUp_S;
  currentStopLow_S += currentStartLow_S;

  samplesUp = std::make_pair(currentStartUp_S, currentStopUp_S);
  samplesLow = std::make_pair(currentStartLow_S, currentStopLow_S);

  // Overlap for lines (valid + overlap IW)
  auto timeOverlapEnd = (burstUp.azimuthStopTime - burstLow.azimuthStartTime);
  unsigned long overlapLength = timeOverlapEnd / m_SarParam.azimuthTimeInterval;

  unsigned long lastValidBurstUp = burstUp.endLine - burstUp.startLine;
  unsigned long firstValidBurstLow = 0;

  if (inputWithInvalidPixels)
  {
    lastValidBurstUp = burstUp.endLine - burstIndUp * m_SarParam.numberOfLinesPerBurst;
    firstValidBurstLow = burstLow.startLine - (burstIndUp+1) * m_SarParam.numberOfLinesPerBurst;
  }

  linesUp = std::make_pair(lastValidBurstUp - overlapLength, lastValidBurstUp);
  linesLow = std::make_pair(firstValidBurstLow, firstValidBurstLow + overlapLength);

  return true;
}

void SarSensorModel::UpdateImageMetadata(ImageMetadata & imd)
{
  imd.Add(MDGeom::SAR, m_SarParam);
  imd.Add(MDGeom::GCP, m_GCP);
}

bool SarSensorModel::ImageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines,
                                            unsigned long imageLine,
                                            unsigned long & deburstLine)
{
  auto vit = lines.cbegin();
  auto nit = vit + 1;

  auto lineOffset = vit->first;
  deburstLine = imageLine;

  for(; nit != lines.end(); ++vit, ++nit)
  {
    if(imageLine >= vit->first && imageLine <= vit->second)
    {
      deburstLine-=lineOffset;
      return true;
    }
    lineOffset+=nit->first - vit->second-1;
  }
  return false;
}

void SarSensorModel::DeburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines,
                                            unsigned long deburstLine,
                                            unsigned long & imageLine)
{
  auto vit = lines.cbegin();
  auto nit = vit+1;

  unsigned long lineOffset = vit->first;

  imageLine = deburstLine;

  while(nit != lines.end())
  {
    if(imageLine+lineOffset>=vit->first && imageLine+lineOffset<=vit->second)
      break;

    lineOffset += nit->first - vit->second-1;
    ++vit;
    ++nit;
  }
  imageLine += lineOffset;
}

SarSensorModel::TimeType SarSensorModel::LineToAzimuthTime(double line) const
{
  assert(!m_SarParam.burstRecords.empty()&&"Burst records are empty (at least one burst should be available)");

  auto currentBurst = m_SarParam.burstRecords.cbegin();

  if(m_SarParam.burstRecords.size() != 1)
  {
    // Look for the correct burst. In most cases the number of burst
    // records will be 1 (except for TOPSAR Sentinel1 products)
    auto it = m_SarParam.burstRecords.cbegin();
    auto itend = m_SarParam.burstRecords.cend();
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
      if(line < m_SarParam.burstRecords.front().startLine)
      {
        currentBurst = m_SarParam.burstRecords.cbegin();
      }
      else if (line >= m_SarParam.burstRecords.back().endLine)
      {
        currentBurst = m_SarParam.burstRecords.end()-1;
      }
    }
  }

  const auto timeSinceStart = (line - currentBurst->startLine) * m_SarParam.azimuthTimeInterval;

  // Eq 22 p 27
  return currentBurst->azimuthStartTime + timeSinceStart + m_AzimuthTimeOffset;
}


bool SarSensorModel::LineToSatPositionAndVelocity(double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const
{
  TimeType const azimuthTime = LineToAzimuthTime(line);
  auto const pos_vel = interpolateSensorPosVel(azimuthTime);
  satellitePosition = pos_vel.first;
  satelliteVelocity = pos_vel.second;

  return !(vnl_math_isnan(satellitePosition[0]) && vnl_math_isnan(satellitePosition[1]) && vnl_math_isnan(satellitePosition[1])
           && vnl_math_isnan(satelliteVelocity[0]) && vnl_math_isnan(satelliteVelocity[1]) && vnl_math_isnan(satelliteVelocity[1]));
}

itk::Point<double, 3> SarSensorModel::EcefToWorld(const itk::Point<double, 3> & ecefPoint) const
{
  return m_EcefToWorldTransform->TransformPoint(ecefPoint);
}

itk::Point<double, 3> SarSensorModel::WorldToEcef(const itk::Point<double, 3> & worldPoint) const
{
  return m_WorldToEcefTransform->TransformPoint(worldPoint);
}

} //namespace otb
