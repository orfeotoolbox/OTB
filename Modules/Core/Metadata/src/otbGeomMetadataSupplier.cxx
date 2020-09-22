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

#include "otbGeomMetadataSupplier.h"

namespace otb
{
GeomMetadataSupplier::GeomMetadataSupplier(const std::string & fileName)
  : m_FileName(fileName)
{
  this->ReadGeomFile();
}

std::string GeomMetadataSupplier::GetMetadataValue(const std::string path, bool& hasValue, int band) const
{
  auto value = this->m_MetadataDic.find(path);
  if(value != this->m_MetadataDic.end())
  {
    hasValue = true;
    return value->second;
  }
  hasValue = false;
  return "";
}

std::string GeomMetadataSupplier::GetResourceFile(std::string) const
{
  return this->m_FileName;
}

int GeomMetadataSupplier::GetNbBands() const
{
  return 0;
}

std::string GeomMetadataSupplier::PrintSelf()
{
  std::ostringstream oss;
  oss << "GeomMetadataSupplier: " << this->m_FileName << '\n';
  for (const auto& kv : this->m_MetadataDic)
    oss << kv.first << " : " << kv.second << '\n';
  return oss.str();
}

void GeomMetadataSupplier::ReadGeomFile()
{
  std::ifstream inputFile(this->m_FileName);
  std::string line;
  std::vector< std::string > keyVal;
  while (std::getline(inputFile, line))
  {
    boost::split(keyVal, line, [](char c){return c == ':';});
    boost::trim(keyVal[0]);
    boost::trim(keyVal[1]);
    this->m_MetadataDic[keyVal[0]] = keyVal[1];
  }
}

} // end namespace otb
