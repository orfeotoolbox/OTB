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

#ifndef otbMissingMetadataException_h
#define otbMissingMetadataException_h

// "Do not include itkExceptionObject.h directly,  include itkMacro.h instead."
#include "otbMacro.h"

namespace otb
{

/** \class MissingMetadataException
 *
 * \brief Exception to be used when metadata parsing fails
 *
 * \ingroup OTBMetadata
 */
class MissingMetadataException : public itk::ExceptionObject
{
public:
  using itk::ExceptionObject::ExceptionObject;
};

} // namespace otb

#endif