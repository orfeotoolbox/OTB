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

#ifndef otbSpot6ImageMetadataInterface_h
#define otbSpot6ImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"
#include <string>

namespace otb
{
/** \class Spot6ImageMetadataInterface
 *
 * \brief Creation of an "otb" Spot6ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT Spot6ImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef Spot6ImageMetadataInterface   Self;
  typedef OpticalImageMetadataInterface Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Spot6ImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  void Parse(ImageMetadata&) override;

protected:
  Spot6ImageMetadataInterface();
  ~Spot6ImageMetadataInterface() = default;

private:
  Spot6ImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
     * There values a computed by 6S. */
  void FetchSpectralSensitivity(const std::string & sensorId, ImageMetadata& imd);
};

} // end namespace otb

#endif
