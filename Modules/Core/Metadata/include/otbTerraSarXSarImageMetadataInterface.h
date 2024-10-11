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


#ifndef otbTerraSarXSarImageMetadataInterface_h
#define otbTerraSarXSarImageMetadataInterface_h

#include <string>

#include "otbSarImageMetadataInterface.h"
#include "otbXMLMetadataSupplier.h"
#include "itkImageBase.h"

namespace otb
{
/** \class TerraSarXSarImageMetadataInterface
 *
 * \brief Creation of an "otb" TerraSarXSarImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT TerraSarXSarImageMetadataInterface : public SarImageMetadataInterface
{
public:
  typedef TerraSarXSarImageMetadataInterface Self;
  typedef SarImageMetadataInterface      Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TerraSarXSarImageMetadataInterface, otbSarImageMetadataInterface);

  typedef itk::ImageBase<2>                     ImageType;
  typedef ImageType::IndexType                  IndexType;
  typedef std::array<int, 2>                       ArrayIndexType;
  typedef std::vector<IndexType>                IndexVectorType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;

  typedef Superclass::PointSetType    PointSetType;
  typedef Superclass::PointSetPointer PointSetPointer;
  typedef double                      RealType;

  /** Get the number of noise records */
  unsigned int GetNumberOfNoiseRecords(const MetadataSupplierInterface&, const unsigned int) const;

  /** Get the number of corner incidence angles */
  unsigned int GetNumberOfCornerIncidenceAngles(const MetadataSupplierInterface&) const;

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle(const MetadataSupplierInterface& mds) const override;

  /** Get the center index */
  IndexType GetCenterIncidenceAngleIndex(const MetadataSupplierInterface& mds) const;

  /** Get the corners incidence angles */
  DoubleVectorType GetCornersIncidenceAngles(const MetadataSupplierInterface&mds) const;

  /** Get the corners index */
  IndexVectorType GetCornersIncidenceAnglesIndex(const MetadataSupplierInterface&) const;

  PointSetPointer GetRadiometricCalibrationNoise(const MetadataSupplierInterface&, const ImageMetadata&, const std::string& b="") const override;
  ArrayIndexType GetRadiometricCalibrationNoisePolynomialDegree() const override;

  PointSetPointer GetRadiometricCalibrationIncidenceAngle(const MetadataSupplierInterface&) const override;
  ArrayIndexType  GetRadiometricCalibrationIncidenceAnglePolynomialDegree() const override;

  /** Get an InfoSceneCoord from its path */
  InfoSceneCoord GetSceneCoord(const MetadataSupplierInterface &supplier, const std::string& path);

  void ParseGdal(ImageMetadata &) override;

  void ParseGeom(ImageMetadata &) override;
  
  void Parse(ImageMetadata &) override;

protected:
  TerraSarXSarImageMetadataInterface() = default;
  ~TerraSarXSarImageMetadataInterface() override = default;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Evaluate polynom with Horner scheme*/
  inline double Horner(std::vector<double>& coefficients, const double tauMinusTauRef) const;

  /** convert a TimeUTC string to a julian day */
  double ConvertStringTimeUTCToJulianDay(const std::string& value) const;

  /** Get the polynomial degree for a given noise record */
  unsigned int GetNoisePolynomialDegrees(const unsigned int noiseRecord, const MetadataSupplierInterface &mds, const unsigned int polLayer) const;

  /** Get the polynomial coefficient for a given noise record */
  DoubleVectorType GetNoisePolynomialCoefficients(const unsigned int noiseRecord, const MetadataSupplierInterface& mds, const unsigned int polLayer) const;

  /** Get timeUTC noise acquisition in Julian day for a given noise record*/
  double GetNoiseTimeUTC(const unsigned int noiseRecord, const MetadataSupplierInterface& mds, const unsigned int polLayer) const;

  /** Get noise reference point for a given noise record */
  double GetNoiseReferencePoint(const unsigned int noiseRecord, const MetadataSupplierInterface &mds, const unsigned int polLayer) const;

private:
  TerraSarXSarImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
