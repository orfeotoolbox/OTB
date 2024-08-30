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

#ifndef otbGeomMetadataSupplier_h
#define otbGeomMetadataSupplier_h

#include <boost/any.hpp>
#include <unordered_map>

#include "OTBMetadataExport.h"
#include "otbMetadataSupplierInterface.h"
#include "otbImageMetadata.h"


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
  using DictType = std::unordered_map<std::string, std::string>;

  GeomMetadataSupplier(const std::string & geomFile);
  GeomMetadataSupplier(const std::string & geomFile, const std::string & imageFile);
  GeomMetadataSupplier(const GeomMetadataSupplier &) = delete;
  GeomMetadataSupplier& operator=(const GeomMetadataSupplier&) = delete;
  ~GeomMetadataSupplier() = default;

  /**
   * @brief Get the metadata value corresponding to a given path
   *
   * @param path The path to look for
   * @param hasValue True if path is found
   * @param band not used
   * @return The value corresponding to path. Empty string if not found.
   */
  std::string GetMetadataValue(std::string const& path, bool& hasValue, int band=1) const override;

  std::string GetResourceFile(std::string const& s="") const override;
  std::vector<std::string> GetResourceFiles() const override;

  int GetNbBands() const override;

  /**
   * @brief Fill the ImageMetadata with the RPC model from the geom file
   *
   * @param imd The ImageMetadata to fill
   */
  bool FetchRPC(ImageMetadata & imd, const double lineOffset=0, const double sampleOffset=0);

  /**
   * @brief Fill the ImageMetadata with the GCP from the geom file
   *
   * @param imd The ImageMetadata to fill
   */
  bool FetchGCP(ImageMetadata & imd);

  /** Get the number of keys starting with path */
  unsigned int GetNumberOf(std::string const&) const override;

  /** If multiple keys have the same path, gives the position of the one with value value*/
  unsigned int GetAttributeId(std::string const&, std::string const&) const override
  {
    otbLogMacro(Error, << "GetAttributeId() not yet implemented in otbGeomMetadataSupplier");
    return 0;
  }

  /**
   * @brief Writes the content of the Geom file into a string
   *
   * @return A std::string
   */
  std::string PrintSelf() const;

protected:
  /**
  * @brief Parse a Geom file and fill the internal map
  */
  void ReadGeomFile();

private:
  /** List of resource files */
  std::unordered_map<std::string, std::string> m_FileNames;
  /** Dictionary containing the metadata */
  DictType m_MetadataDic;

};

} // end namespace otb

#endif
