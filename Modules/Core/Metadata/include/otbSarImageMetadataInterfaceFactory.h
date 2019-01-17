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

#ifndef otbSarImageMetadataInterfaceFactory_h
#define otbSarImageMetadataInterfaceFactory_h

#include "itkObject.h"
#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class SarImageMetadataInterfaceFactory
 * \brief Create instances of SarImageMetadataInterfaceFactory objects using an object factory.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT SarImageMetadataInterfaceFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SarImageMetadataInterfaceFactory Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarImageMetadataInterfaceFactory, Object);

  /** Convenient typedefs. */
  typedef SarImageMetadataInterface::ImageType                ImageType;
  typedef SarImageMetadataInterface::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef SarImageMetadataInterface::VectorType               VectorType;
  typedef SarImageMetadataInterface::VariableLengthVectorType VariableLengthVectorType;
  typedef SarImageMetadataInterface::ImageKeywordlistType     ImageKeywordlistType;
  typedef SarImageMetadataInterface::Pointer                  SarImageMetadataInterfacePointerType;

  /** Create the appropriate SarImageMetadataInterfaceFactory depending on the particulars of the file. */
  static SarImageMetadataInterfacePointerType CreateIMI(const MetaDataDictionaryType& dict);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  SarImageMetadataInterfaceFactory();
  ~SarImageMetadataInterfaceFactory() override;

private:
  SarImageMetadataInterfaceFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
