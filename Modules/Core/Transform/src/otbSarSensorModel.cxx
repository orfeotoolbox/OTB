/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbGenericMapProjection.h"

#include <numeric>

namespace otb
{
itk::Point<double, 3> EcefToWorld(const itk::Point<double, 3> & ecefPoint)
{
  auto transform = otb::GenericMapProjection<otb::TransformDirection::INVERSE, double,3, 3>::New();
  transform->SetWkt("EPSG:4978");

  return transform->TransformPoint(ecefPoint);
}


itk::Point<double, 3> WorldToEcef(const itk::Point<double, 3> & worldPoint)
{
  auto transform = otb::GenericMapProjection<otb::TransformDirection::FORWARD, double,3, 3>::New();
  transform->SetWkt("EPSG:4978");

  return transform->TransformPoint(worldPoint);
}


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
                              m_AzimuthTimeOffset(MetaData::seconds(0)),
                              m_RangeTimeOffset(0.)
{
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
    //imPt.x = groundRange/theRangeResolution;
  }
  else
  {
    // SLC case
    // Eq 23 and 24 p. 28
    outLineSample[0] = (rangeTime - m_SarParam.nearRangeTime) 
                        * m_SarParam.rangeSamplingRate;
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

void SarSensorModel::LineSampleHeightToWorld(const Point2DType& imPt,
                                             double  heightAboveEllipsoid,
                                             Point3DType& worldPt) const
{
  assert(m_GCP.GCP.size());

  const auto& gcp = findClosestGCP(imPt, m_GCP);

  Point3DType ecefPoint;

  // Simple iterative inversion of inverse model starting at closest gcp
  worldPt = EcefToWorld(projToSurface(gcp, imPt, heightAboveEllipsoid));
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
     DurationType t_min = abs(azimuthTime - m_SarParam.orbits.front().time);
     unsigned int count = 0;
     for(auto it = m_SarParam.orbits.cbegin();it!= m_SarParam.orbits.cend();++it,++count)
     {
        const DurationType current_time = abs(azimuthTime-it->time);
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
  assert(m_GCP.GCP.size());

  DurationType cumulAzimuthTime(MetaData::seconds(0));
  unsigned int count=0;

  // reset offsets before optimisation
  m_AzimuthTimeOffset = MetaData::seconds(0);

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
  m_AzimuthTimeOffset = count > 0 ? cumulAzimuthTime / count : DurationType(MetaData::seconds(0));
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
  const GCP* closest;

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

SarSensorModel::Point3DType SarSensorModel::projToSurface(const GCP & gcp, const Point2DType& imPt, double heightAboveEllipsoid) const
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
  double currentHeightResidual = 0.; //TODO

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
/* TODO manage elevation
         const ossim_float64 atHgt = hgtRef.getRefHeight(currentEstimationWorld);
         currentHeightResidual = atHgt - currentEstimationWorld.height();
*/

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

  burstRecords.push_back(deburstBurst);

  // Now move GCPs

  std::vector<GCP> deburstGCPs;
  
  for (auto gcp : m_GCP.GCPs)
  {
    unsigned long newLine=0;

    unsigned long gcpLine = std::floor(gcp.m_GCPY + 0.5);
    unsigned long gcpSample = std::floor(gcp.m_GCPX + 0.5);

    // Be careful about fractional part of GCPs
    double fractionalLines = gcp.m_GCPY - gcpLine;
    double fractionalSamples = gcp.m_GCPX - gcpSample;

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
      gcp.m_GCPY = newLine + fractionalLines;
      gcp.m_GCPX = newSample + fractionalSamples;
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

    unsigned long gcpLine = std::floor(currentGCP.m_GCPY + 0.5);
    unsigned long gcpSample = std::floor(currentGCP.m_GCPX + 0.5);

    // Be careful about fractional part of GCPs
    double fractionalLines = currentGCP.m_GCPY - gcpLine;
    double fractionalSamples = currentGCP.m_GCPX - gcpSample;

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
      currentGCP.m_GCPY = newLine + fractionalLines;
      currentGCP.m_GCPX = newSample + fractionalSamples;
      oneBurstGCPs.push_back(currentGCP);
    }
  }

  m_GCP.GCPs.swap(oneBurstGCPs);

  return true;

}

bool SarSensorModel::ImageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines,
                                            unsigned long imageLine,
                                            unsigned long & deburstLine) const
{
  auto vit = lines.cbegin();
  auto nit = vit + 1;
  
  auto lineOffset = vit->first;
  deburstLine = imageLine;
  
  for(; nit != lines.end(); ++vit, ++nit)
  while(nit != lines.end())
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

} //namespace otb