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

#include "otbSentinel1ThermalNoiseLookupData.h"

namespace otb
{

void Sentinel1ThermalNoiseLookupData::InitParameters(double firstLineTime,
                    double lastLineTime,
                    int numOfLines,
                    std::vector<Sentinel1CalibrationStruct> const& rangeNoiseVectorList,
                    std::vector<Sentinel1AzimuthNoiseStruct> const& azimuthNoiseVectorList)
{
  m_FirstLineTime = firstLineTime,
  m_LastLineTime = lastLineTime;
  m_NumOfLines = numOfLines;
  m_LineTimeInterval = (lastLineTime - firstLineTime) / (numOfLines - 1);;
  m_RangeNoiseVectorList = rangeNoiseVectorList;
  m_AzimuthNoiseVectorList = azimuthNoiseVectorList;

  m_RangeCount = rangeNoiseVectorList.size();
  m_AzimuthCount = azimuthNoiseVectorList.size();
}



double Sentinel1ThermalNoiseLookupData::GetValue(const IndexValueType x, const IndexValueType y) const
{
  return GetRangeNoise(x,y) * GetAzimuthNoise(x,y);
}

double Sentinel1ThermalNoiseLookupData::GetRangeNoise(const IndexValueType x, const IndexValueType y) const
{
  if (m_RangeCount)
  {
    const auto vecIdx = GetRangeVectorIndex(y);
    assert(vecIdx >= 0 && vecIdx < m_RangeCount - 1);

    const auto& vec0 = m_RangeNoiseVectorList[vecIdx];
    const auto& vec1 = m_RangeNoiseVectorList[vecIdx + 1];

    const auto azTime = m_FirstLineTime + y * m_LineTimeInterval;
    const auto muY = (azTime - vec0.timeMJD) / vec1.deltaMJD;
    const auto pixelIdx = GetPixelIndex(x, vec0.pixels);
    const double muX = (x - vec0.pixels[pixelIdx]) / vec0.deltaPixels[pixelIdx + 1];
    const double lutVal =
        (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) + muY * ((1 - muX) * vec1.vect[pixelIdx] + muX * vec1.vect[pixelIdx + 1]);
    return lutVal;
  }
  else
  {
    return 1.;
  }
}

double Sentinel1ThermalNoiseLookupData::GetAzimuthNoise(const IndexValueType x, const IndexValueType y) const
{
  if (m_AzimuthCount)
  {
    const auto vecIdx = GetAzimuthVectorIndex(x, y);
    const auto& vec = m_AzimuthNoiseVectorList[vecIdx];

    const auto pixelIdx = GetPixelIndex(y, vec.lines);

    const double lutVal = vec.vect[pixelIdx] + (vec.vect[pixelIdx + 1] - vec.vect[pixelIdx]) *
      (static_cast<double>(y - vec.lines[pixelIdx]) / static_cast<double>(vec.lines[pixelIdx+1] - vec.lines[pixelIdx]));
    return lutVal;
  }
  else
  {
    return 1.;
  }
}

int Sentinel1ThermalNoiseLookupData::GetRangeVectorIndex(int y) const
{
  for (int i = 1; i < m_RangeCount; i++)
  {
    if (y <= m_RangeNoiseVectorList[i].line)
    {
      return i - 1;
    }
  }

  return -1;
}

int Sentinel1ThermalNoiseLookupData::GetAzimuthVectorIndex(int x, int y) const
{
  for (int i = 0; i < m_AzimuthCount; i++)
  {
    const auto & azimuthVector = m_AzimuthNoiseVectorList[i];

    if (x >= azimuthVector.firstRangeSample && x <= azimuthVector.lastRangeSample
        && y >= azimuthVector.firstAzimuthLine && y <= azimuthVector.lastAzimuthLine)
    {
      return i;
    }
  }

  return -1;
}

int Sentinel1ThermalNoiseLookupData::GetPixelIndex(int x, const std::vector<int> & vec) const
{
  const int size = vec.size();
  std::vector<int>::const_iterator wh   = std::upper_bound(vec.begin(), vec.end(), x);
  return wh == vec.end() ? size - 2 : std::distance(vec.begin(), wh) - 1;
}

} //namespace otb
