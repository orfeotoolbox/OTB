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

  /**
   * @brief Get the metadata value corresponding to a given path
   *
   * @param path The path to look for
   * @param hasValue True if path is found
   * @param band not used
   * @return The value corresponding to path. Empty string if not found.
   */
  std::string GetMetadataValue(const std::string path, bool& hasValue, int band=1) const override;

  /**
   * @brief Get the first metadata value corresponding to a given path
   *
   * @param path The path to look for
   * @param hasValue True if path is found
   * @return The value corresponding to path. Empty string if not found.
   */
  std::string GetFirstMetadataValue(const std::string paths, bool& hasValue) const;

  /**
   * @brief Get the metadata value corresponding to a given path
   * converted to the given type
   *
   * This method can look for a value in a list, using the _# jocker. For exemple,
   * looking for "value" in a dictionary like this :
   * foo_1.bar=42
   * foo_1.doo=99
   * foo_2.value=8
   * One can specify this path : foo_#.value, the method will then return 8.
   *
   * @param path The path to look for.
   * @return The value corresponding to path.
   * @raises otb::Error if path not found
   */
  template <typename T> T GetFirstAs(const std::string &path) const
  {
    bool hasValue;
    std::string ret = GetFirstMetadataValue(path, hasValue);
    if (!hasValue)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Missing metadata '"<<path<<"'")
    }
    try
    {
      return boost::lexical_cast<T>(ret);
    }
    catch (boost::bad_lexical_cast&)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<path<<"', got: "<<ret)
    }
  }

  std::string GetResourceFile(std::string="") const override;

  int GetNbBands() const override;

  /**
   * @brief Writes the content of the XML file into a string
   *
   * @return A std::string
   */
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

  /**
   * @brief In a StringList of “Name=Value” pairs, look for the values
   *        associated with a name containing the specified string
   *
   * @param papszStrList A StringList that will be searched
   * @param pszName A string that will be looked for in the keys
   * @return A StringList containing only the pairs from papszStrList whose key
   *         contain pszName
   */
  std::vector<std::string> FetchPartialNameValueMultiple(char** papszStrList, const char *pszName) const;

  /**
   * @brief In a StringList of “Name=Value” pairs, look for the values
   *        associated with a name containing the specified string
   *
   * @param StringVector A std::vector of std::string that will be searched
   * @param Name A std::string that will be looked for in the keys
   * @return A std::vector of std::string containing only the pairs from StringVector whose key
   *         contain Name
   */
  std::vector<std::string> FetchPartialNameValueMultiple(const std::vector<std::string> &StringVector,
		                                                 const std::string &Name) const;

private:
  /** List of resource files */
  std::string m_FileName;
  /** Dictionary containing the metadata */
  char** m_MetadataDic = nullptr;
};

} // end namespace otb

#endif
