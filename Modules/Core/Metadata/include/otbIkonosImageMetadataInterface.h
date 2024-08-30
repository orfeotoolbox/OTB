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

#ifndef otbIkonosImageMetadataInterface_h
#define otbIkonosImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class IkonosImageMetadataInterface
 *
 * \brief Creation of an "otb" IkonosImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT IkonosImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef IkonosImageMetadataInterface  Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IkonosImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  void Parse(ImageMetadata &) override;

protected:
  IkonosImageMetadataInterface() = default;
  ~IkonosImageMetadataInterface() override = default;

private:
  IkonosImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;


  void FetchProductionDate(const std::string & productionDate, ImageMetadata &imd);

  void FetchAcquisitionDate(const std::string & acquisitionDate,
                            const std::string & acquisitionTime,
                            ImageMetadata &imd);

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
     * There values a computed by 6S. */
  void FetchSpectralSensitivity(const std::string & bandName, ImageMetadata &imd);

};

} // end namespace otb

#endif
