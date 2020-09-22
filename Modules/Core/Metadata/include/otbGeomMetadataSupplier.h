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

#ifndef otbGeomMetadataSupplier_h
#define otbGeomMetadataSupplier_h

#include <fstream>
#include <boost/algorithm/string.hpp>

#include "OTBMetadataExport.h"
#include "otbMetadataSupplierInterface.h"


namespace otb
{

/** \class GeomMetadataSupplier
 *
 * \brief Class to access metadata information in a Geom file
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT GeomMetadataSupplier
  : public MetadataSupplierInterface
{
public:
  using DictType = std::map<std::string, std::string>;

  GeomMetadataSupplier(const std::string &);
  GeomMetadataSupplier(const GeomMetadataSupplier &) = delete;
  GeomMetadataSupplier& operator=(const GeomMetadataSupplier&) = delete;

  /**
   * @brief Get the metadata value corresponding to a given path
   *
   * @param path The path to look for
   * @param hasValue True if path is found
   * @param band not used
   * @return The value corresponding to path. Empty string if not found.
   */
  std::string GetMetadataValue(const std::string path, bool& hasValue, int band=1) const override;

  std::string GetResourceFile(std::string="") const override;

  int GetNbBands() const override;

  /**
   * @brief Writes the content of the Geom file into a string
   *
   * @return A std::string
   */
  std::string PrintSelf();

protected:
  /**
  * @brief Parse a Geom file and fill the internal map
  */
  void ReadGeomFile();

private:
  /** List of resource files */
  std::string m_FileName;
  /** Dictionary containing the metadata */
  DictType m_MetadataDic;
};

} // end namespace otb

#endif
