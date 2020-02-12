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

#ifndef otbMetadataStorageInterface_h
#define otbMetadataStorageInterface_h

#include "OTBMetadataExport.h"
#include "itkExceptionObject.h"
#include <vector>
#include <string>
#include "otbStringUtils.h"
#include "otbMacro.h"
#include "otbStringUtilities.h"

namespace otb
{

/** \class MetadataStorageInterface
 *
 * \brief Base class to store metadata information in files/images
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT MetadataStorageInterface
{
public:

  /** Set the metadata value corresponding to a given path (meaning of this path
   * depends on the specific implementation. Returns NULL when path is not found */
  virtual void SetMetadataValue(const char * path, const char * value) = 0;

  // TODO
  // void SetAs<>
};


} // end namespace otb

#endif
