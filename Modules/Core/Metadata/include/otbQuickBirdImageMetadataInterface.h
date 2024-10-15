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

#ifndef otbQuickBirdImageMetadataInterface_h
#define otbQuickBirdImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class QuickBirdImageMetadataInterface
 *
 * \brief Creation of an "otb" QuickBirdImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT QuickBirdImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef QuickBirdImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase      Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QuickBirdImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

protected:
  QuickBirdImageMetadataInterface();
  ~QuickBirdImageMetadataInterface() override = default;

private:
  QuickBirdImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
