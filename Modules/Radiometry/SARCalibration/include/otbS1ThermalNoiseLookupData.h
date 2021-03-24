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


  void SetImageKeywordlist(const ImageKeywordlist & kwl);

  using IndexValueType = itk::IndexValueType ;

  T GetValue(const IndexValueType x, const IndexValueType y);

  T GetRangeNoise(const IndexValueType x, const IndexValueType y);

  T GetAzimuthNoise(const IndexValueType x, const IndexValueType y);

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

    std::vector<int> lines;
    std::vector<float>  vect;
  };

  int GetRangeVectorIndex(int y) const;

  int GetAzimuthVectorIndex(int x, int y) const;

  int GetPixelIndex(int x, const std::vector<int> & vec) const;

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

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbS1ThermalNoiseLookupData.hxx"
#endif

#endif