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

#include "otbXMLMetadataSupplier.h"

namespace otb
{
XMLMetadataSupplier::XMLMetadataSupplier(const std::string & fileName)
  : GDALMDReaderBase(nullptr, nullptr), m_FileName(fileName)
{
  CPLXMLNode* psNode = CPLParseXMLFile(m_FileName.c_str());
  if(psNode != nullptr)
  {
    //if(std::string(psNode->pszValue) == "?xml")
//    if(EQUAL(psNode->pszValue, "?xml"))
//      psNode = psNode->psNext;
    m_MetadataDic = ReadXMLToList(psNode, m_MetadataDic);
  }
  else
  {
    otbLogMacro(Warning, <<"Unable to parse XML file " << fileName);
  }
  CPLDestroyXMLNode(psNode);
}

const char * XMLMetadataSupplier::GetMetadataValue(const char * path, int band) const
{
  char** fetched;
  fetched = CSLFetchNameValueMultiple(m_MetadataDic, path);
  if (fetched == nullptr)
    return nullptr;
  std::ostringstream oss;
  oss << *fetched;
  ++fetched;
  for ( ; *fetched != nullptr ; ++fetched )
    oss << " " << *fetched;
  return oss.str().c_str();
}

std::string XMLMetadataSupplier::GetResourceFile() const
{
  return m_FileName;
}

bool XMLMetadataSupplier::HasRequiredFiles() const
{
  return false; // TODO
}

char** XMLMetadataSupplier::GetMetadataFiles() const
{
  char ** todo;
  return todo; // TODO
}


} // end namespace otb
