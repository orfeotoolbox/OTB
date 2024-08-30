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

#ifndef otbImageMetadataInterfaceFactory_h
#define otbImageMetadataInterfaceFactory_h

#include "itkObject.h"
#include "otbImageMetadataInterfaceBase.h"
#include "otbMetadataSupplierInterface.h"
#include "otbImageMetadata.h"

namespace otb
{
/** \class ImageMetadataInterfaceFactory
 * \brief Create instances of ImageMetadataInterfaceFactory objects using an object factory.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataInterfaceFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ImageMetadataInterfaceFactory Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadataInterfaceFactory, Object);

  /** Convenient typedefs. */
  typedef ImageMetadataInterfaceBase::ImageType                ImageType;
  typedef ImageMetadataInterfaceBase::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef ImageMetadataInterfaceBase::VectorType               VectorType;
  typedef ImageMetadataInterfaceBase::VariableLengthVectorType VariableLengthVectorType;
  typedef ImageMetadataInterfaceBase::Pointer                  ImageMetadataInterfaceBasePointerType;

  // TODO: the input ImageMetadata is here to inject some metadatas parsed by
  // GDALImageIO and initialize the correct number of bands. It should not be
  // needed once we have a merge() function,
  /** Create the appropriate IMI based on a MetadataSupplier */
  static ImageMetadataInterfaceBasePointerType CreateIMI(ImageMetadata & imd, const MetadataSupplierInterface & mds);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  ImageMetadataInterfaceFactory();
  ~ImageMetadataInterfaceFactory() override;

private:
  ImageMetadataInterfaceFactory(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
