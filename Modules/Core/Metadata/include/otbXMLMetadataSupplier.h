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

#ifndef otbXMLMetadataSupplier_h
#define otbXMLMetadataSupplier_h

#include <gdal_mdreader.h>
#include "OTBMetadataExport.h"
//#include "otb_tinyxml.h"
#include "otbMetadataSupplierInterface.h"
#include "otbStringUtilities.h"


namespace otb
{

/** \class XMLMetadataSupplier
 *
 * \brief Class to access metadata information in a XML file
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT XMLMetadataSupplier
  : public MetadataSupplierInterface,
	public GDALMDReaderBase
{
public:
  XMLMetadataSupplier(const std::string &);

  /** Get the metadata value corresponding to a given path
   * Returns NULL when path is not found
   * If band >= 0, the metadata value is looked in the specified band*/
  const char * GetMetadataValue(const char * path, int band=1) const override;

  std::string GetResourceFile() const override;

  /**
  * @brief Determine whether the input parameter correspond to the particular
  *        provider of remote sensing data completely
  * @return True if all needed sources files found
  */
  bool HasRequiredFiles() const override;

  /**
  * @brief Get metadata file names. The caller become owner of returned list
  *        and have to free it via CSLDestroy.
  * @return A file name list
  */
  char** GetMetadataFiles() const override;

private:
  /** List of resource files */
  std::string m_FileName;
  /** Dictionary containing the metadata */
  char** m_MetadataDic = nullptr;
};

} // end namespace otb

#endif
