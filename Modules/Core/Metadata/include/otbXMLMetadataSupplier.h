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

#include "cpl_minixml.h"
#include "cpl_string.h"

#include "OTBMetadataExport.h"
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
  : public MetadataSupplierInterface
{
public:
  XMLMetadataSupplier(const std::string &);

  /** Get the metadata value corresponding to a given path
   * Returns NULL when path is not found
   * If band >= 0, the metadata value is looked in the specified band*/
  const std::string GetMetadataValue(const std::string path, bool& hasValue, int band=1) const override;

  std::string GetResourceFile(std::string="") const override;

  std::string PrintSelf();

protected:
  /**
  * @brief ReadXMLToList Transform xml to list of NULL terminated name=value
  *        strings
  * @param psNode A xml node to process
  * @param papszList A list to fill with name=value strings
  * @param pszName A name of parent node. For root xml node should be empty.
  *        If name is not empty, the sibling nodes will not proceed
  * @return An input list filled with values
  *
  * This method originates from a work by GDAL in the class GDALMDReaderBase.
  */
  virtual char** ReadXMLToList(CPLXMLNode* psNode, char** papszList,
                               const char* pszName = "");

private:
  /** List of resource files */
  std::string m_FileName;
  /** Dictionary containing the metadata */
  char** m_MetadataDic = nullptr;
};

} // end namespace otb

#endif
