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

#ifndef otbSentinel1CalibrationLookupData_h
#define otbSentinel1CalibrationLookupData_h

#include "otbSarImageMetadataInterface.h"


namespace otb
{

/**
 * \struct Sentinel1CalibrationStruct
 * \brief Structure holding a calibration vector from Sentinel1 annotation files
 *
 * \ingroup OTBMetadata
 */
struct Sentinel1CalibrationStruct
{
public:
  double              timeMJD;
  double              deltaMJD; // time difference to previous MJD in the list
  int                 line;
  std::vector<int>    pixels;
  std::vector<double> deltaPixels;
  std::vector<float>  vect;
  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const;
  /** Keywordlist import */
  static Sentinel1CalibrationStruct FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix);
};

/**
 * \class Sentinel1CalibrationLookupData
 * \brief Computes the Sentinel 1 calibration contribution for the given pixel using a LUT. Depending on how
 * it is initialized, the LUT can be a ALPHA, BETA, SIGMA or DN calibration LUT
 *
 * \ingroup OTBMetadata
 */
class Sentinel1CalibrationLookupData : public SarCalibrationLookupData
{
public:
  /** Standard typedefs */
  typedef Sentinel1CalibrationLookupData Self;
  typedef SarCalibrationLookupData       Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(Sentinel1CalibrationLookupData, SarCalibrationLookupData);

  typedef itk::IndexValueType IndexValueType;

  Sentinel1CalibrationLookupData();

  ~Sentinel1CalibrationLookupData() override = default;

  void InitParameters(short type, double ft, double lt, int lines, int c, std::vector<Sentinel1CalibrationStruct> const& vlist);

  double GetValue(const IndexValueType x, const IndexValueType y) const override;

  int GetVectorIndex(int y) const;

  int GetPixelIndex(int x, const Sentinel1CalibrationStruct& calVec) const;

  /** Keywordlist export */
  void ToKeywordlist(MetaData::Keywordlist & kwl, const std::string & prefix) const override;
  /** Keywordlist import */
  void FromKeywordlist(const MetaData::Keywordlist & kwl, const std::string & prefix) override;

private:
  Sentinel1CalibrationLookupData(const Self&) = delete;
  void operator=(const Self&) = delete;

  double                                  firstLineTime;
  double                                  lastLineTime;
  int                                     numOfLines;
  int                                     count;
  std::vector<Sentinel1CalibrationStruct> calibrationVectorList;
  double                                  lineTimeInterval;
};

} // namespace otb

#endif
