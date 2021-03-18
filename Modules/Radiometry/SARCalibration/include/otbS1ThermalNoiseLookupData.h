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

#ifndef otbS1ThermalNoiseLookupData_h
#define otbS1ThermalNoiseLookupData_h

#include "otbSentinel1ImageMetadataInterface.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "ossim/ossimTimeUtilities.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimTimeUtilities.h"
#endif

namespace otb
{

template <class T = double>
class S1ThermalNoiseLookupData : public itk::LightObject
{

public:
  /** Standard typedefs */
  using Self = S1ThermalNoiseLookupData;
  using Superclass = itk::LightObject;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(S1ThermalNoiseLookupData, itk::LightObject);


  void SetImageKeywordlist(const ImageKeywordlist & kwl)
  {
    m_FirstLineTime = ossimplugins::time::toModifiedJulianDate(kwl.GetMetadataByKey("calibration.startTime")).as_day_frac();
    m_LastLineTime  = ossimplugins::time::toModifiedJulianDate(kwl.GetMetadataByKey("calibration.stopTime")).as_day_frac();
    m_NumOfLines = std::stoi(kwl.GetMetadataByKey("number_lines"));
    m_LineTimeInterval = (m_LastLineTime - m_FirstLineTime) / (m_NumOfLines - 1);

    if (!kwl.HasKey("noise.count"))
    {
      //TODO : noise.count is not read for older products.
    }

    m_RangeCount = std::stoi(kwl.GetMetadataByKey("noise.rangeCount"));
    m_RangeNoiseVectorList.clear();
    
    double lastMJD = 0;

    for (int i = 0; i < m_RangeCount; i++)
    {
      const std::string prefix = "noise.noiseVector[" + std::to_string(i) + "].";


      Sentinel1CalibrationStruct rangeNoiseVector;
      rangeNoiseVector.timeMJD = ossimplugins::time::toModifiedJulianDate(kwl.GetMetadataByKey(prefix + "azimuthTime")).as_day_frac();
      rangeNoiseVector.deltaMJD = rangeNoiseVector.timeMJD - lastMJD;
      rangeNoiseVector.line = std::stoi(kwl.GetMetadataByKey(prefix + "line"));

      Utils::ConvertStringToVector(kwl.GetMetadataByKey( prefix + "pixel"), 
                                   rangeNoiseVector.pixels, prefix + "pixel");
      Utils::ConvertStringToVector(kwl.GetMetadataByKey( prefix + "noiseLut"), 
                                   rangeNoiseVector.vect, prefix + "noiseLut");

      int prevPixel = 0;
      for (const auto & pixel: rangeNoiseVector.pixels)
      {
        rangeNoiseVector.deltaPixels.push_back(pixel - prevPixel);
        prevPixel = pixel;
      }

      m_RangeNoiseVectorList.push_back(rangeNoiseVector);

      lastMJD = rangeNoiseVector.timeMJD;
    }

    if (kwl.HasKey("noise.azimuthCount"))
    {
      m_AzimuthCount = std::stoi(kwl.GetMetadataByKey("noise.azimuthCount"));
    }
  }


  using IndexValueType = itk::IndexValueType ;

  T GetValue(const IndexValueType x, const IndexValueType y)
  {
    return GetRangeNoise(x,y) * GetAzimuthNoise(x,y);
  }

  T GetRangeNoise(const IndexValueType x, const IndexValueType y)
  {
    const auto vecIdx = GetVectorIndex(y);
    assert(vecIdx >= 0 && vecIdx < m_RangeCount - 1);

    const auto& vec0 = m_RangeNoiseVectorList[vecIdx];
    const auto& vec1 = m_RangeNoiseVectorList[vecIdx + 1];

    const auto azTime = m_FirstLineTime + y * m_LineTimeInterval;
    const auto muY = (azTime - vec0.timeMJD) / vec1.deltaMJD;
    const auto pixelIdx = GetPixelIndex(x, vec0);
    const double muX = (x - vec0.pixels[pixelIdx]) / vec0.deltaPixels[pixelIdx + 1];
    const double lutVal =
        (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) + muY * ((1 - muX) * vec1.vect[pixelIdx] + muX * vec1.vect[pixelIdx + 1]);
    return lutVal;
  }


  T GetAzimuthNoise(const IndexValueType x, const IndexValueType y)
  {
    if (m_AzimuthCount)
    {
      //TODO compute azimuth correction
      return 1.;
    }
    // Older products do not contain azimuth noise Luts.
    else
    {
      return 1.;
    }
  }

protected:
  S1ThermalNoiseLookupData() : m_FirstLineTime(0.), m_LastLineTime(0.) {m_FirstLineTime = 1.;};
  ~S1ThermalNoiseLookupData() = default;

private:

  struct Sentinel1AzimuthNoiseStruct
  {
    int firstAzimuthLine = 0;
    int lastAzimuthLine = 0;
    int firstRangeSample = 0;
    int lastRangeSample = 0;

    std::vector<int> pixels;
    //std::vector<double> deltaPixels;
    std::vector<float>  vect;
  };

  int GetVectorIndex(int y) const
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

  int GetPixelIndex(int x, const Sentinel1CalibrationStruct& vec) const
  {
    const int                        size = vec.pixels.size();
    std::vector<int>::const_iterator wh   = std::upper_bound(vec.pixels.begin(), vec.pixels.end(), x);
    return wh == vec.pixels.end() ? size - 2 : std::distance(vec.pixels.begin(), wh) - 1;
  }

  //ImageKeywordlist m_kwl;
  double m_FirstLineTime = 0.;
  double m_LastLineTime = 0.;
  double m_LineTimeInterval = 0.;

  int m_NumOfLines = 0;
  int m_RangeCount = 0;
  int m_AzimuthCount = 0;

  std::vector<Sentinel1CalibrationStruct> m_RangeNoiseVectorList;
  std::vector<Sentinel1AzimuthNoiseStruct> m_AzimuthNoiseVectorList;
};

}

#endif