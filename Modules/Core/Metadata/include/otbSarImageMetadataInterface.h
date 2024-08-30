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

#ifndef otbSarImageMetadataInterface_h
#define otbSarImageMetadataInterface_h

#include <string>
#include <vector>
#include "OTBMetadataExport.h"
#include "otbImageMetadataInterfaceBase.h"
#include "itkPointSet.h"
#include "otbSarCalibrationLookupData.h"
#include "otbStringUtils.h"
#include "otbSARMetadata.h"

namespace otb
{
/** \class SarImageMetadataInterface
 *
 * \brief Class for SAR captor metadata reading.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT SarImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:
  typedef SarImageMetadataInterface     Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef ImageType::IndexType                 IndexType;
  typedef std::array<int, 2>                   ArrayIndexType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::UIntVectorType           UIntVectorType;
  typedef Superclass::StringVectorType         StringVectorType;
  typedef itk::PointSet<double, 2> PointSetType;
  typedef PointSetType::Pointer    PointSetPointer;
  typedef double                   RealType;
  typedef PointSetType::PointType  PointType;
  typedef SarCalibrationLookupData LookupDataType;
  typedef LookupDataType::Pointer  LookupDataPointerType;

  virtual bool CreateCalibrationLookupData(SARCalib&, const ImageMetadata&, const MetadataSupplierInterface&, const bool) const;

  virtual bool HasCalibrationLookupDataFlag(const MetadataSupplierInterface&) const;

  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const;
  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const;
  virtual PointSetPointer GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface &) const;
  virtual PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const;
  virtual PointSetPointer GetRadiometricCalibrationNoise(const MetadataSupplierInterface&, const ImageMetadata&, const std::string& b="") const;

  virtual ArrayIndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const;
  virtual ArrayIndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const;
  virtual ArrayIndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const;
  virtual ArrayIndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const;
  virtual ArrayIndexType GetRadiometricCalibrationNoisePolynomialDegree() const;

  virtual double GetCenterIncidenceAngle(const MetadataSupplierInterface& mds) const = 0;


  virtual double GetRescalingFactor() const;

  virtual void ParseGdal(ImageMetadata &) =0;
  virtual void ParseGeom(ImageMetadata &) =0;

  bool GetSAR(SARParam &) const;
  std::vector<AzimuthFmRate> GetAzimuthFmRateGeom() const;
  std::vector<DopplerCentroid> GetDopplerCentroidGeom() const;
  std::vector<Orbit> GetOrbitsGeom() const;
  std::vector<BurstRecord> GetBurstRecordsGeom() const;

  /**
   * @brief Loads the radiometric calibration data to the SARCalib
   * @param sarCalib The SARCalib objct to complete
   * @param mds The MetadataSupplierInterface containing the calibration data
   * @param imd The ImageMetadata containing the calibration data
   */
  void LoadRadiometricCalibrationData(SARCalib& sarCalib, const MetadataSupplierInterface& mds, const ImageMetadata& imd, const std::string &band="") const;

protected:
  SarImageMetadataInterface();
  ~SarImageMetadataInterface() override = default;

  PointSetPointer GetConstantValuePointSet(const RealType& value) const;
  ArrayIndexType GetConstantPolynomialDegree() const;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SarImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
