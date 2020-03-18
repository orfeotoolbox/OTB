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

#ifndef otbSarImageMetadataInterface_h
#define otbSarImageMetadataInterface_h

#include <string>
#include <vector>
#include "OTBMetadataExport.h"
#include "otbImageMetadataInterfaceBase.h"
#include "itkPointSet.h"
#include "otbSarCalibrationLookupData.h"
#include "otbStringUtils.h"

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
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::UIntVectorType           UIntVectorType;
  typedef Superclass::StringVectorType         StringVectorType;
  typedef itk::PointSet<double, 2> PointSetType;
  typedef PointSetType::Pointer    PointSetPointer;
  typedef double                   RealType;
  typedef PointSetType::PointType  PointType;
  typedef SarCalibrationLookupData LookupDataType;
  typedef LookupDataType::Pointer  LookupDataPointerType;

  virtual void CreateCalibrationLookupData(const short t);

  const LookupDataPointerType GetCalibrationLookupData(const short type);

  bool HasCalibrationLookupDataFlag() const;

  void SetCalibrationLookupData(LookupDataType* lut)
  {
    m_SarLut = lut;
  }

  virtual RealType GetRadiometricCalibrationScale() const;

  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternNewGain() const;
  virtual PointSetPointer GetRadiometricCalibrationAntennaPatternOldGain() const;
  virtual PointSetPointer GetRadiometricCalibrationIncidenceAngle() const;
  virtual PointSetPointer GetRadiometricCalibrationRangeSpreadLoss() const;
  virtual PointSetPointer GetRadiometricCalibrationNoise() const;

  virtual IndexType GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() const;
  virtual IndexType GetRadiometricCalibrationNoisePolynomialDegree() const;

  virtual double GetPRF() const                  = 0;
  virtual double GetRSF() const                  = 0;
  virtual double GetRadarFrequency() const       = 0;
  virtual double GetCenterIncidenceAngle() const = 0;


  virtual double GetRescalingFactor() const;

  virtual const std::string GetProductType() const;

  virtual const std::string GetAcquisitionMode() const;

  /** Get the enhanced band names (No enhanced band name support for SAR) */
  StringVectorType GetEnhancedBandNames() const override
  {
    StringVectorType nothing;
    return nothing;
  }

  /** Reads into the MetaDataDictionary to find an OSSIM ImageKeywordlist,
   * then translate it into ImageMetadata. Handles most SAR sensors.
   * Returns true if succeed. */
  bool ConvertImageKeywordlistToImageMetadata() override;
  
protected:
  SarImageMetadataInterface();
  ~SarImageMetadataInterface() override
  {
  }

  PointSetPointer GetConstantValuePointSet(const RealType& value) const;
  IndexType GetConstantPolynomialDegree() const;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  LookupDataPointerType m_SarLut;


private:
  SarImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
