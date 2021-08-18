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

#include "otbSentinel1CalibrationLookupData.h"

namespace otb
{

Sentinel1CalibrationLookupData::Sentinel1CalibrationLookupData() : firstLineTime(0.), lastLineTime(0.), numOfLines(0), count(0), lineTimeInterval(0.)
{
}

void Sentinel1CalibrationLookupData::InitParameters(short type, double ft, double lt, int lines, int c, std::vector<Sentinel1CalibrationStruct> const& vlist)
{
  firstLineTime         = ft;
  lastLineTime          = lt;
  numOfLines            = lines;
  count                 = c;
  calibrationVectorList = vlist;
  this->SetType(type);
  lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0);
}

double Sentinel1CalibrationLookupData::GetValue(const IndexValueType x, const IndexValueType y) const
{
  const int calVecIdx = GetVectorIndex(y);
  assert(calVecIdx >= 0 && calVecIdx < count - 1);
  const Sentinel1CalibrationStruct& vec0     = calibrationVectorList[calVecIdx];
  const Sentinel1CalibrationStruct& vec1     = calibrationVectorList[calVecIdx + 1];
  const double                      azTime   = firstLineTime + y * lineTimeInterval;
  const double                      muY      = (azTime - vec0.timeMJD) / vec1.deltaMJD;
  const int                         pixelIdx = GetPixelIndex(x, calibrationVectorList[calVecIdx]);
  const double                      muX      = (x - vec0.pixels[pixelIdx]) / vec0.deltaPixels[pixelIdx + 1];
  const double                      lutVal =
      (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) + muY * ((1 - muX) * vec1.vect[pixelIdx] + muX * vec1.vect[pixelIdx + 1]);
  return lutVal;
}

int Sentinel1CalibrationLookupData::GetVectorIndex(int y) const
{
  for (int i = 1; i < count; i++)
  {
    if (y < calibrationVectorList[i].line)
    {
      return i - 1;
    }
  }
  return -1;
}

int Sentinel1CalibrationLookupData::GetPixelIndex(int x, const Sentinel1CalibrationStruct& calVec) const
{
  const int                        size = calVec.pixels.size();
  std::vector<int>::const_iterator wh   = std::upper_bound(calVec.pixels.begin(), calVec.pixels.end(), x);
  return wh == calVec.pixels.end() ? size - 2 : std::distance(calVec.pixels.begin(), wh) - 1;
}

} // namespace otb