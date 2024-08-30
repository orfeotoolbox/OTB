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

#ifndef otbSarDefaultImageMetadataInterface_h
#define otbSarDefaultImageMetadataInterface_h


#include "otbMacro.h"

#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class SarDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT SarDefaultImageMetadataInterface : public SarImageMetadataInterface
{
public:
  typedef SarDefaultImageMetadataInterface Self;
  typedef SarImageMetadataInterface        Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarDefaultImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef ImageType::IndexType                 IndexType;
  typedef std::array<int, 2>                   ArrayIndexType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef itk::PointSet<double, 2> PointSetType;
  typedef PointSetType::Pointer   PointSetPointer;
  typedef double                  RealType;
  typedef PointSetType::PointType PointType;

  PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface&) const override
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface&) not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLoss() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationNoise(const MetadataSupplierInterface&, const ImageMetadata&, const std::string&) const override
  {
    itkExceptionMacro("GetRadiometricCalibrationNoise(const MetadataSupplierInterface&) not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  ArrayIndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const override
  {
    itkExceptionMacro(
        "GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  ArrayIndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const override
  {
    itkExceptionMacro(
        "GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  ArrayIndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAnglePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  ArrayIndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  ArrayIndexType GetRadiometricCalibrationNoisePolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationNoisePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle(const MetadataSupplierInterface&) const override
  {
    itkExceptionMacro("GetCenterIncidenceAngle not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  void ParseGdal(ImageMetadata &) override
  {
    itkExceptionMacro("ParseGdal not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  void ParseGeom(ImageMetadata &) override
  {
    itkExceptionMacro("ParseGeom not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }


protected:
  SarDefaultImageMetadataInterface() = default;
  ~SarDefaultImageMetadataInterface() override = default;

private:
  SarDefaultImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
