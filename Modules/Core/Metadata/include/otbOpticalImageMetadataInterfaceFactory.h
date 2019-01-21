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

#ifndef otbOpticalImageMetadataInterfaceFactory_h
#define otbOpticalImageMetadataInterfaceFactory_h

#include "itkObject.h"
#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class OpticalImageMetadataInterfaceFactory
 * \brief Create instances of OpticalImageMetadataInterfaceFactory objects using an object factory.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT OpticalImageMetadataInterfaceFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef OpticalImageMetadataInterfaceFactory Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalImageMetadataInterfaceFactory, Object);

  /** Convenient typedefs. */
  typedef OpticalImageMetadataInterface::ImageType                ImageType;
  typedef OpticalImageMetadataInterface::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef OpticalImageMetadataInterface::VectorType               VectorType;
  typedef OpticalImageMetadataInterface::VariableLengthVectorType VariableLengthVectorType;
  typedef OpticalImageMetadataInterface::ImageKeywordlistType     ImageKeywordlistType;
  typedef OpticalImageMetadataInterface::Pointer                  OpticalImageMetadataInterfacePointerType;

  /** Create the appropriate OpticalImageMetadataInterfaceFactory depending on the particulars of the file. */
  static OpticalImageMetadataInterfacePointerType CreateIMI(const MetaDataDictionaryType& dict);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  OpticalImageMetadataInterfaceFactory();
  ~OpticalImageMetadataInterfaceFactory() override;

private:
  OpticalImageMetadataInterfaceFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
