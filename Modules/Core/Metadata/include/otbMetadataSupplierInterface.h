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

#ifndef otbMetadataSupplierInterface_h
#define otbMetadataSupplierInterface_h

#include "OTBMetadataExport.h"
#include <vector>
#include <string>
#include "otbStringUtils.h"
#include "otbStringUtilities.h"
#include <boost/algorithm/string.hpp>
#include "otbMissingMetadataException.h"

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
  const std::array<std::string, 3> TRUES = {"1", "true", "True"};
  const std::array<std::string, 3> FALSES = {"0", "false", "False"};
  virtual std::string GetResourceFile(std::string const& ="") const = 0;
  virtual std::vector<std::string> GetResourceFiles() const
  {
    return std::vector<std::string>{this->GetResourceFile()};
  }


  /** Get the metadata value corresponding to a given path (meaning of this path
   * depends on the specific implementation. Returns empty string when path is not found,
   * and hasValue is set to False.
   * If band >= 0, the metadata value is looked in the specified band*/
  virtual std::string GetMetadataValue(std::string const& path, bool& hasValue, int band=-1) const = 0;

  bool HasValue(std::string const& path, int band=-1) const;

  virtual int GetNbBands() const = 0;

  template <typename T> T GetAs(T const& defaultValue, std::string const& path, int band=-1) const;

  template <typename T> T GetAs(std::string const& path, int band=-1) const;

  /** Parse a metadata value to a std::vector,
   *  If size>=0, then the final std::vector size is checked and an exception
   *  is raised if it doesn't match the given size.*/
  template < typename T> std::vector<T> GetAsVector(std::string const& path, char sep=' ', int size=-1, int band=-1) const;

  /** Get the number of keys starting with path */
  virtual unsigned int GetNumberOf(std::string const& path) const = 0;

  /** If multiple keys have the same path, gives the position of the one with value value*/
  virtual unsigned int GetAttributeId(std::string const& path, std::string const& value) const = 0;

  virtual ~MetadataSupplierInterface() = default;
};

template <> bool MetadataSupplierInterface::GetAs(bool const& defaultValue, std::string const& path, int band) const;
// TODO : for complex types ...

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMetadataSupplierInterface.hxx"
#endif

#endif
