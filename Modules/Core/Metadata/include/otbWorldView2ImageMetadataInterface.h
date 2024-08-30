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

#ifndef otbWorldView2ImageMetadataInterface_h
#define otbWorldView2ImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class WorldView2ImageMetadataInterface
 *
 * \brief Creation of an "otb" WorldView2ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT WorldView2ImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef WorldView2ImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase       Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WorldView2ImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  void Parse(ImageMetadata &) override;

protected:
  WorldView2ImageMetadataInterface() = default;
  ~WorldView2ImageMetadataInterface() = default;

private:
  WorldView2ImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;

  void FetchPhysicalBias(ImageMetadata&);
  
  void FetchSolarIrradianceWorldView2(ImageMetadata &);
  void FetchSpectralSensitivityWorldView2(ImageMetadata &);

  void FetchSolarIrradianceQuickBird(ImageMetadata &);
  void FetchSpectralSensitivityQuickBird(ImageMetadata &);
  void FetchPhysicalGainQuickBird(int bitsPerPixel,
                                  const std::unordered_map<std::string, double> & absCalFactor,
                                  const std::unordered_map<std::string, int> & TDILevels,
                                  ImageMetadata &);
};

} // end namespace otb

#endif
