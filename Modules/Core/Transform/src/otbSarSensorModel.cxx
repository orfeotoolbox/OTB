/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

namespace otb
{
double DotProduct(const itk::Point<double, 3> & pt1, const itk::Point<double, 3> & pt2)
{
  return std::inner_product(pt1.Begin(), pt1.End(), pt2.Begin(), 0.);
}

SarSensorModel::SarSensorModel( const std::string & productType,
                                const SARParam & sarParam,
                                const Projection::GCPParam & gcps)
                            : m_ProductType(productType),
                              m_GCP(gcps),
                              m_SarParam(sarParam),
                              m_AzimuthTimeOffset(MetaData::Duration::Seconds(0)),
                              m_RangeTimeOffset(0.),
                              m_EcefToWorldTransform(otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::New()),
                              m_WorldToEcefTransform(otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::New())

{
  if (m_GCP.GCPs.empty())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"no GCP found in the input metadata, at least one is required in SARSensorModel");
  }

  OptimizeTimeOffsetsFromGcps();

  const std::vector<std::string> grdProductTypes = {"GRD", "MGD", "GEC", "EEC"};
  m_IsGrd = std::find(grdProductTypes.begin(), grdProductTypes.end(), m_ProductType) != grdProductTypes.end();

}

SarSensorModel::SarSensorModel(const ImageMetadata & imd)
          : SarSensorModel(imd.Has(MDStr::ProductType) ? imd[MDStr::ProductType] : "UNKNOWN",
                            boost::any_cast<SARParam>(imd[MDGeom::SAR]),
                            imd.GetGCPParam())
{
}

void SarSensorModel::WorldToLineSample(const Point3DType& inGeoPoint, Point2DType& outLineSample) const
{
  TimeType azimuthTime;
  double rangeTime;
  Point3DType sensorPos; 
  Vector3DType sensorVel;

  const bool success = WorldToAzimuthRangeTime(inGeoPoint, azimuthTime, rangeTime, sensorPos, sensorVel);

  if(!success)
  {
    //TODO return Nan or throw ?
    return;
  }

  // Convert azimuth time to line
  AzimuthTimeToLine(azimuthTime, outLineSample[1]);

  if (m_IsGrd)
  {
    // GRD case
    double groundRange(0);
    SlantRangeToGroundRange(rangeTime*C/2,azimuthTime,groundRange);

    // Eq 32 p. 31
    outLineSample[0] = groundRange/m_SarParam.rangeResolution;
  }
  else
  {
    // SLC case
    // Eq 23 and 24 p. 28
    outLineSample[0] = (rangeTime - m_SarParam.nearRangeTime) 
                        * m_SarParam.rangeSamplingRate;
  }
}

void SarSensorModel::WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const
{
  TimeType azimuthTime;
  double rangeTime;
  Point3DType sensorPos; 
  Vector3DType sensorVel;

  const bool success = WorldToAzimuthRangeTime(inGeoPoint, azimuthTime, rangeTime, sensorPos, sensorVel);

  if(!success)
  {
    //TODO return Nan or throw ?
    return;
  }

  // Convert azimuth time to line
  AzimuthTimeToLine(azimuthTime, cr[1]);

  if (m_IsGrd)
  {
    // GRD case
    double groundRange(0);
    SlantRangeToGroundRange(rangeTime*C/2,azimuthTime,groundRange);

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
  double NormeS = std::sqrt(sensorPos[0]*sensorPos[0] + sensorPos[1]*sensorPos[1] + sensorPos[2]*sensorPos[2]);
  
  double PS2 = inputPt[0]*sensorPos[0] + inputPt[1]*sensorPos[1] + inputPt[2]*sensorPos[2];

  assert(NormeS>1e-6);
  yz[1] = NormeS - PS2/NormeS;

  double distance = std::sqrt((sensorPos[0]-inputPt[0])*(sensorPos[0]-inputPt[0]) + 
                             (sensorPos[1]-inputPt[1])*(sensorPos[1]-inputPt[1]) + 
                             (sensorPos[2]-inputPt[2])*(sensorPos[2]-inputPt[2]));  



  yz[0] = std::sqrt(distance*distance - yz[1] * yz[1]);

  // Check view side and change sign of Y accordingly
  if ( (( sensorVel[0] * (sensorPos[1]* inputPt[2] - sensorPos[2]* inputPt[1]) +
              sensorVel[1] * (sensorPos[2]* inputPt[0] - sensorPos[0]* inputPt[2]) +
              sensorVel[2] * (sensorPos[0]* inputPt[1] - sensorPos[1]* inputPt[0])) > 0) ^ m_SarParam.rightLookingFlag )
  {
    yz[0] = -yz[0];
  }
}

bool SarSensorModel::WorldToAzimuthRangeTime(const Point3DType& inGeoPoint, 
                                        TimeType & azimuthTime, 
                                        double & rangeTime, 
                                        Point3DType& sensorPos, 
                                        Vector3DType& sensorVel) const
{
  auto ecefPoint = WorldToEcef(inGeoPoint);

  const bool success = ZeroDopplerLookup(ecefPoint,azimuthTime,sensorPos,sensorVel);

  if(!success)
  {
    return false;
  }

  //TODO Bistatic correction when needed
  const double rangeDistance = sensorPos.EuclideanDistanceTo(ecefPoint);

  rangeTime = m_RangeTimeOffset + 2*rangeDistance/C;
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


void SarSensorModel::LineSampleHeightToWorld(const Point2DType& imPt,
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

void SarSensorModel::LineSampleToWorld(const Point2DType& imPt,
                                             Point3DType& worldPt) const
{
  assert(m_GCP.GCPs.size());

  const auto& gcp = findClosestGCP(imPt, m_GCP);

  Point3DType ecefPoint;

  auto heightFunction = [](const double lon, const double lat)
                        {return DEMHandler::GetInstance().GetHeightAboveEllipsoid(lon, lat);};

  // Simple iterative inversion of inverse model starting at closest gcp
  worldPt = EcefToWorld(projToSurface(gcp, imPt, heightFunction));
}

bool SarSensorModel::ZeroDopplerLookup(const Point3DType& inEcefPoint, 
                                        TimeType & azimuthTime, 
                                        Point3DType& sensorPos, 
                                        Vector3DType& sensorVel) const
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
    doppler1 = DotProduct(inEcefPoint - record1->position,record1->velocity);
    doppler2 = DotProduct(inEcefPoint - record2->position,record2->velocity);
    const DurationType delta_td = record2->time - record1->time;

    azimuthTime = record1->time - doppler1 / (doppler2 - doppler1) * delta_td;
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
    azimuthTime = record1->time + td + m_AzimuthTimeOffset;
  }

  // Interpolate sensor position and velocity
  interpolateSensorPosVel(azimuthTime, sensorPos, sensorVel);

  return true;
}


void SarSensorModel::interpolateSensorPosVel(const TimeType & azimuthTime, 
                                Point3DType& sensorPos, 
                                Vector3DType& sensorVel, 
                                unsigned int deg) const
{
  assert(m_SarParam.orbits.size() &&"The orbit records vector is empty");

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
  if(m_SarParam.orbits.size() < deg)
  {
     nEnd = m_SarParam.orbits.size()-1;
  }
  else
  {
     // Search for the deg number of records around the azimuth time
     unsigned int t_min_idx = 0;
    auto t_min = Abs(azimuthTime - m_SarParam.orbits.front().time);
     unsigned int count = 0;
     for(auto it = m_SarParam.orbits.cbegin();it!= m_SarParam.orbits.cend();++it,++count)
     {
        const auto current_time = Abs(azimuthTime-it->time);
        if(t_min > current_time)
        {
           t_min_idx = count;
           t_min = current_time;
        }
     }
     // TODO: see if these expressions can be simplified
     nBegin = std::max((int)t_min_idx-(int)deg/2+1,(int)0);
     nEnd = std::min(nBegin+deg-1,(unsigned int)m_SarParam.orbits.size());
     nBegin = nEnd<m_SarParam.orbits.size()-1 ? nBegin : nEnd-deg+1;
  }

  // Compute lagrangian interpolation using records from nBegin to nEnd
  for(unsigned int i = nBegin; i < nEnd; ++i)
  {
     double w = 1.;
     unsigned int j = nBegin;
     for( ; j != i ; ++j)
     {
        const DurationType td1 = azimuthTime                    - m_SarParam.orbits[j].time;
        const DurationType td2 = m_SarParam.orbits[i].time - m_SarParam.orbits[j].time;
        const double f = td1 / td2;
        w *= f;
     }
     ++j;
     for( ; j < nEnd; ++j)
     {
        const DurationType td1 = azimuthTime                    - m_SarParam.orbits[j].time;
        const DurationType td2 = m_SarParam.orbits[i].time - m_SarParam.orbits[j].time;
        const double f = td1 / td2;
        w *= f;
     }
     sensorPos[0]+=w*m_SarParam.orbits[i].position[0];
     sensorPos[1]+=w*m_SarParam.orbits[i].position[1];
     sensorPos[2]+=w*m_SarParam.orbits[i].position[2];

     sensorVel[0]+=w*m_SarParam.orbits[i].velocity[0];
     sensorVel[1]+=w*m_SarParam.orbits[i].velocity[1];
     sensorVel[2]+=w*m_SarParam.orbits[i].velocity[2];
  }

}


void SarSensorModel::OptimizeTimeOffsetsFromGcps()
{
  assert(m_GCP.GCPs.size());

  DurationType cumulAzimuthTime(DurationType::Seconds(0));
  unsigned int count=0;

  // reset offsets before optimisation
  m_AzimuthTimeOffset = DurationType::Seconds(0);

  // First, fix the azimuth time
  for(auto gcpIt = m_GCP.GCPs.begin(); gcpIt!= m_GCP.GCPs.end(); ++gcpIt)
  {
    auto gcpTimeIt = m_SarParam.gcpTimes.find(gcpIt->m_Id);
    if (gcpTimeIt != std::end(m_SarParam.gcpTimes))
    {
      TimeType estimatedAzimuthTime;
      double   estimatedRangeTime;

      Point3DType sensorPos;
      Point3DType sensorVel;

      Point3DType inWorldPoint;
      inWorldPoint[0] = gcpIt->m_GCPX;
      inWorldPoint[1] = gcpIt->m_GCPY;
      inWorldPoint[2] = gcpIt->m_GCPZ;

      // Estimate times
      const bool s1 = this->WorldToAzimuthRangeTime(inWorldPoint,estimatedAzimuthTime,estimatedRangeTime, sensorPos, sensorVel);

      if(s1)
      {
        cumulAzimuthTime -= (estimatedAzimuthTime - gcpTimeIt->second.azimuthTime);
        ++count;
      }
    }
  }
  m_AzimuthTimeOffset = count > 0 ? cumulAzimuthTime / count : DurationType(MetaData::Duration::Seconds(0));
}

void SarSensorModel::AzimuthTimeToLine(const TimeType & azimuthTime, double & line) const
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
  line = (timeSinceStart/m_SarParam.azimuthTimeInterval) + currentBurst->startLine;
}

void SarSensorModel::SlantRangeToGroundRange(double slantRange, const TimeType & azimuthTime, double & groundRange) const
{
  ApplyCoordinateConversion(slantRange, azimuthTime, m_SarParam.slantRangeToGroundRangeRecords , groundRange);
}

void SarSensorModel::ApplyCoordinateConversion(double in,
                                 const TimeType& azimuthTime,
                                 const std::vector<CoordinateConversionRecord> & records,
                                 double & out) const
{
  assert(!records.empty()&&"The records vector is empty.");

  // First, we need to find the correct pair of records for interpolation
  auto it = records.cbegin();
  auto previousRecord = it;
  ++it;

  auto nextRecord = it;

  // Look for the correct record
  while(it!=records.end())
  {
    if(azimuthTime >= previousRecord->azimuthTime
        && azimuthTime < nextRecord->azimuthTime)
    {
      break;
    }

    previousRecord = nextRecord;
    ++it;
    nextRecord = it;
  }

  CoordinateConversionRecord srgrRecord;

  if(it == records.cend())
  {
    if(azimuthTime < records.front().azimuthTime)
    {
      srgrRecord = records.front();
    }
    else if(azimuthTime >= records.back().azimuthTime)
    {
      srgrRecord = records.back();
    }

  }

  else
  {
    assert(nextRecord != records.end());
    assert(!previousRecord->coeffs.empty()&&"previousRecord coefficients vector is empty.");
    assert(!nextRecord->coeffs.empty()&&"nextRecord coefficients vector is empty.");

    // If azimuth time is between 2 records, interpolate
    const double interp
            = DurationType(azimuthTime             - previousRecord->azimuthTime)
            / (nextRecord->azimuthTime - previousRecord->azimuthTime)
            ;

    srgrRecord.rg0 = (1-interp) * previousRecord->rg0 + interp*nextRecord->rg0;

    srgrRecord.coeffs.clear();
    std::vector<double>::const_iterator pIt = previousRecord->coeffs.cbegin();
    std::vector<double>::const_iterator nIt = nextRecord->coeffs.cbegin();

    for(;pIt != previousRecord->coeffs.end() && nIt != nextRecord->coeffs.cend();++pIt,++nIt)
    {
      srgrRecord.coeffs.push_back(interp*(*nIt)+(1-interp)*(*pIt));
    }

    assert(!srgrRecord.coeffs.empty()&&"Slant range to ground range interpolated coefficients vector is empty.");
  }

  // Now that we have the interpolated coeffs, compute ground range
  // from slant range
  const double sr_minus_sr0 =  in-srgrRecord.rg0;

  assert(!srgrRecord.coeffs.empty()&&"Slant range to ground range coefficients vector is empty.");

  out = 0.;

  for(auto cIt = srgrRecord.coeffs.crbegin();cIt!=srgrRecord.coeffs.crend();++cIt)
  {
    out = *cIt + sr_minus_sr0*out;
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

SarSensorModel::Point3DType SarSensorModel::projToSurface(const GCP & gcp, const Point2DType& imPt, std::function<double(double, double)> heightFunction) const
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

void SarSensorModel::LineToAzimuthTime(double line, TimeType & azimuthTime) const
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
  azimuthTime = currentBurst->azimuthStartTime + timeSinceStart + m_AzimuthTimeOffset;
}


bool SarSensorModel::LineToSatPositionAndVelocity(double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const
{
  TimeType azimuthTime;

  LineToAzimuthTime(line, azimuthTime);
  interpolateSensorPosVel(azimuthTime, satellitePosition, satelliteVelocity);

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
