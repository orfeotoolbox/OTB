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

#ifndef otbNISARCalibrationLookupData_h
#define otbNISARCalibrationLookupData_h

#include "otbSarCalibrationLookupData.h"

#include <vector>

namespace otb
{

/**
 * \class NISARCalibrationLookupData
 * \brief LUT-backed calibration helper for NISAR products serialized through OTB GDAL metadata.
 *
 * The lookup values are stored on a regular image grid (row/column) and on a
 * calibration grid expressed in zero-Doppler time and slant range. GetValue()
 * converts pixel indices to the image physical radar coordinates and performs a
 * bilinear interpolation in the calibration grid.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT NISARCalibrationLookupData : public SarCalibrationLookupData
{
public:
  /** Standard typedefs */
  using Self = NISARCalibrationLookupData;
  using Superclass = SarCalibrationLookupData;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(NISARCalibrationLookupData, SarCalibrationLookupData);

  using IndexValueType = itk::IndexValueType;

  NISARCalibrationLookupData();
  ~NISARCalibrationLookupData() override = default;

  void Initialize(short type,
                  double imageFirstAzimuthTime,
                  double imageAzimuthTimeSpacing,
                  double imageFirstSlantRange,
                  double imageSlantRangeSpacing,
                  std::vector<double> lutAzimuthTimes,
                  std::vector<double> lutSlantRanges,
                  std::vector<float> lutValues);

  double GetValue(IndexValueType x, IndexValueType y) const override;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const override;
  /** Keywordlist import */
  void FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix) override;

private:
  NISARCalibrationLookupData(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::size_t GetValueIndex(std::size_t row, std::size_t col) const;

  double m_ImageFirstAzimuthTime;
  double m_ImageAzimuthTimeSpacing;
  double m_ImageFirstSlantRange;
  double m_ImageSlantRangeSpacing;
  std::vector<double> m_LUTAzimuthTimes;
  std::vector<double> m_LUTSlantRanges;
  std::vector<float> m_LUTValues;
};

} // namespace otb

#endif
