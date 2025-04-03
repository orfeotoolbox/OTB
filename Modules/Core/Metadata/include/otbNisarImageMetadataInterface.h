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

#ifndef otbNisarImageMetadataInterface_h
#define otbNisarImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"
#include "otbXMLMetadataSupplier.h"

namespace otb
{
/** \class NisarImageMetadataInterface
 *
 * \brief Creation of an "otb" NisarImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class OTBMetadata_EXPORT NisarImageMetadataInterface : public SarImageMetadataInterface
{
public:
  using Self = NisarImageMetadataInterface;
  using Superclass = SarImageMetadataInterface;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NisarImageMetadataInterface, SarImageMetadataInterface);

  using ImageType = Superclass::ImageType;
  using MetaDataDictionaryType = Superclass::MetaDataDictionaryType;
  using VectorType = Superclass::VectorType;
  using VariableLengthVectorType = Superclass::VariableLengthVectorType;
  using RealType = Superclass::RealType;
  using LookupDataPointerType = Superclass::LookupDataPointerType;

  double GetCenterIncidenceAngle(const MetadataSupplierInterface&) const override;
  void ParseGdal(ImageMetadata &) override;
  void ParseGeom(ImageMetadata &) override;
  void Parse(ImageMetadata &) override;

protected:
  /* class ctor */
  NisarImageMetadataInterface() = default;

  /* class dtor */
  ~NisarImageMetadataInterface() override = default;

private:
  NisarImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};




} // end namespace otb

#endif
