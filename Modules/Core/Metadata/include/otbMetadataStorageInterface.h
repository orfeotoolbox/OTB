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
#include "itkMacro.h"
#include <vector>
#include <string>
#include <sstream>
#include "otbStringUtils.h"
#include "otbMacro.h"
#include "otbStringUtilities.h"
#include "otbJoinContainer.h"

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
  virtual void SetMetadataValue(const char * path, const char * value, int band=-1) = 0;

  // TODO : check precision settings
  void SetAs(const std::string & path, double value, int band=-1);

  void SetAs(const std::string & path, const std::string & value, int band=-1);

  /** Parse a std::vector to a metadata value */
  template < typename T> void SetAsVector(const char *path, std::vector<T> value, const char sep=' ', int band=-1)
  {
    std::ostringstream oss;
    oss << "[";
    otb::Join(oss, value, std::string(&sep));
    oss << "]";
    SetMetadataValue(path, oss.str().c_str(), band);
  }
  template < typename T> void SetAsVector(const std::string& path, std::vector<T> value, const char sep=' ', int band=-1)
  {
    SetAsVector(path.c_str(), value, sep, band);
  }
};


} // end namespace otb

#endif
