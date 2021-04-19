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
using boost::posix_time::seconds;

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


SarSensorModel::SarSensorModel(const ImageMetadata & imd) 
          : m_Imd(imd),
            m_AzimuthTimeOffset(boost::posix_time::seconds(0)),
            m_RangeTimeOffset(0.)
{
  if (!imd.Bands[0].Has(MDGeom::SAR))
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Input metadata does not contain SAR parameters");
  }

  m_SarParam = boost::any_cast<SARParam>(imd.Bands[0][MDGeom::SAR]);
  
  OptimizeTimeOffsetsFromGcps();

  const std::vector<std::string> grdProductTypes = {"GRD", "MGD", "GEC", "EEC"};
  m_IsGrd = m_Imd.Has(MDStr::ProductType) &&
    std::find(grdProductTypes.begin(), grdProductTypes.end(), m_Imd[MDStr::ProductType]) != grdProductTypes.end();

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
    //TODO GRD case
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
  if (!m_Imd.Has(MDGeom::GCP))
  {
    return;
  }

  DurationType cumulAzimuthTime(seconds(0));
  double cumulRangeTime(0);
  unsigned int count=0;

  
  // reset offsets before optimisation
  m_AzimuthTimeOffset = seconds(0);
  auto gcpParam = m_Imd.GetGCPParam();
  // First, fix the azimuth time
  for(auto gcpIt = gcpParam.GCPs.begin(); gcpIt!=gcpParam.GCPs.end(); ++gcpIt)
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
  m_AzimuthTimeOffset = count > 0 ? cumulAzimuthTime / count : DurationType(seconds(0));
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

} //namespace otb