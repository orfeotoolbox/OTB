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

#ifndef otbMetadataSupplierInterface_h
#define otbMetadataSupplierInterface_h

#include "OTBMetadataExport.h"
#include <vector>
#include <string>
//#include "otbStringUtilities.h"

namespace otb
{
/** \class MetadataSupplierInterface
 *
 * \brief Base class to access metadata information in files/images
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT MetadataSupplierInterface
{
public:

  virtual std::string GetResourceFile() = 0;

  // Maybe not needed
//  virtual std::vector<std::string> GetResourceFiles() = 0;

  //~ virtual bool HasValue(const char * path) = 0;

  /** Get the metadata value corresponding to a given path (meaning of this path
   * depends on the specific implementation. Returns NULL when path is not found */
  virtual const char * GetMetadataValue(const char * path) const = 0;

  // probably not needed
  //~ virtual std::vector<std::string> GetValuesList(const std::string& path) = 0;
  
};

} // end namespace otb

#endif

