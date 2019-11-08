/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef itk::PointSet<double, 2> PointSetType;
  typedef PointSetType::Pointer   PointSetPointer;
  typedef double                  RealType;
  typedef PointSetType::PointType PointType;

  RealType GetRadiometricCalibrationScale() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationScale() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternNewGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationAntennaPatternOldGain() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationIncidenceAngle() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAngle() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLoss() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  PointSetPointer GetRadiometricCalibrationNoise() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationNoise() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const override
  {
    itkExceptionMacro(
        "GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const override
  {
    itkExceptionMacro(
        "GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationIncidenceAnglePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  IndexType GetRadiometricCalibrationNoisePolynomialDegree() const override
  {
    itkExceptionMacro("GetRadiometricCalibrationNoisePolynomialDegree() not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const override
  {
    itkExceptionMacro("GetDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const override
  {
    itkExceptionMacro("GetMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const override
  {
    itkExceptionMacro("GetYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const override
  {
    itkExceptionMacro("GetHour not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const override
  {
    itkExceptionMacro("GetMinute not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const override
  {
    itkExceptionMacro("GetProductionDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const override
  {
    itkExceptionMacro("GetProductionMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const override
  {
    itkExceptionMacro("GetProductionYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the PRF */
  double GetPRF() const override
  {
    itkExceptionMacro("GetPRF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the RSF */
  double GetRSF() const override
  {
    itkExceptionMacro("GetRSF not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radar frequency */
  double GetRadarFrequency() const override
  {
    itkExceptionMacro("GetRadarFrequency not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle() const override
  {
    itkExceptionMacro("GetCenterIncidenceAngle not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  bool CanRead() const override
  {
    // This class is the default one, it has to be able to call every metadata
    return false;
  }

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  UIntVectorType GetDefaultDisplay() const override
  {
    UIntVectorType rgb(3);
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
    return rgb;
  }

protected:
  SarDefaultImageMetadataInterface(){};
  ~SarDefaultImageMetadataInterface() override
  {
  }

private:
  SarDefaultImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
