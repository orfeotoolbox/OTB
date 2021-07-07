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


#ifndef otbTerraSarXSarImageMetadataInterface_h
#define otbTerraSarXSarImageMetadataInterface_h

#include <string>

#include "otbSarImageMetadataInterface.h"
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
  typedef std::vector<IndexType>                IndexVectorType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType      ImageKeywordlistType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;

  typedef Superclass::PointSetType    PointSetType;
  typedef Superclass::PointSetPointer PointSetPointer;
  typedef double                      RealType;

  /** Get the imaging start acquisition day from the ossim metadata
   * \deprecated
   */
  int GetDay() const override;

  /** Get the imaging start acquisition month from the ossim metadata
   * \deprecated
   */
  int GetMonth() const override;

  /** Get the imaging start acquisition year from the ossim metadata
   * \deprecated
   */
  int GetYear() const override;

  /** Get the imaging start acquisition hour from the ossim metadata
   * \deprecated
   */
  int GetHour() const override;

  /** Get the imaging start acquisition minute from the ossim metadata
   * \deprecated
   */
  int GetMinute() const override;

  /** Get the imaging production day from the ossim metadata : generationTime variable
   * \deprecated
   */
  int GetProductionDay() const override;

  /** Get the imaging production month from the ossim metadata : generationTime variable
   * \deprecated
   */
  int GetProductionMonth() const override;

  /** Get the imaging production year from the ossim metadata : generationTime variable
   * \deprecated
   */
  int GetProductionYear() const override;

  /** Get the radar frequency */
  double GetRadarFrequency() const override;

  /** Get the PRF */
  double GetPRF() const override;

  /** Get the RSF */
  double GetRSF() const override;

  /** Get the center incidence angle */
  double GetCenterIncidenceAngle() const override;
  
  bool CanRead() const override;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const override;

protected:
  TerraSarXSarImageMetadataInterface() = default;
  ~TerraSarXSarImageMetadataInterface() = default;
 

private:
  TerraSarXSarImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
