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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "otbGeomMetadataSupplier.h"
#include "otbMetaDataKey.h"
#include "otbGeometryMetadata.h"
#include "otbStringUtils.h"

namespace otb
{
GeomMetadataSupplier::GeomMetadataSupplier(std::string const& fileName)
  : m_FileName(fileName)
{
  this->ReadGeomFile();
}

std::string GeomMetadataSupplier::GetMetadataValue(std::string const& path, bool& hasValue, int /*band*/) const
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

std::string GeomMetadataSupplier::GetResourceFile(std::string const& /*s*/) const
{
  return this->m_FileName;
}

int GeomMetadataSupplier::GetNbBands() const
{
  bool hasValue;
  std::string ret = this->GetMetadataValue("support_data.number_bands", hasValue);
  if (hasValue)
  {
    boost::algorithm::trim_if(ret, boost::algorithm::is_any_of("\" "));
    return std::stoi(ret);
  }

  ret = this->GetMetadataValue("support_data.band_name_list", hasValue);
  boost::algorithm::trim_if(ret, boost::algorithm::is_any_of("\" "));
  if (hasValue)
  {
    std::vector<std::string> ret_vect;
    otb::Utils::ConvertStringToVector(ret, ret_vect, "band name");
    return ret_vect.size();
  }

  return 1;
}

bool GeomMetadataSupplier::FetchRPC(ImageMetadata & imd)
{
  bool hasValue;
  GetMetadataValue("polynomial_format", hasValue);
  if (!hasValue)
    return false;

  Projection::RPCParam rpcStruct;
  rpcStruct.LineOffset    = this->GetAs<double>("line_off");
  rpcStruct.SampleOffset  = this->GetAs<double>("samp_off");
  rpcStruct.LatOffset     = this->GetAs<double>("lat_off");
  rpcStruct.LonOffset     = this->GetAs<double>("long_off");
  rpcStruct.HeightOffset  = this->GetAs<double>("height_off");

  rpcStruct.LineScale    = this->GetAs<double>("line_scale");
  rpcStruct.SampleScale  = this->GetAs<double>("samp_scale");
  rpcStruct.LatScale     = this->GetAs<double>("lat_scale");
  rpcStruct.LonScale     = this->GetAs<double>("long_scale");
  rpcStruct.HeightScale  = this->GetAs<double>("height_scale");

  std::vector<double> coeffs;
  int loop = 0;
  std::stringstream path;
  for (auto & coeff : rpcStruct.LineNum)
  {
    path.str("");
    path << "line_num_coeff_" << std::setfill('0') << std::setw(2) << loop++;
    coeff = this->GetAs<double>(path.str());
  }
  loop = 0;
  for (auto & coeff : rpcStruct.LineDen)
  {
    path.str("");
    path << "line_den_coeff_" << std::setfill('0') << std::setw(2) << loop++;
    coeff = this->GetAs<double>(path.str());
  }
  loop = 0;
  for (auto & coeff : rpcStruct.SampleNum)
  {
    path.str("");
    path << "samp_num_coeff_" << std::setfill('0') << std::setw(2) << loop++;
    coeff = this->GetAs<double>(path.str());
  }
  loop = 0;
  for (auto & coeff : rpcStruct.SampleDen)
  {
    path.str("");
    path << "samp_den_coeff_" << std::setfill('0') << std::setw(2) << loop++;
    coeff = this->GetAs<double>(path.str());
  }

  imd.Add(MDGeom::RPC, rpcStruct);
  assert(imd.Has(MDGeom::RPC));
  assert(rpcStruct == boost::any_cast<Projection::RPCParam>(imd[MDGeom::RPC]));
  return true;
}

std::string GeomMetadataSupplier::PrintSelf() const
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
    auto pos = line.find(":");
    if (pos != std::string::npos)
    {
      auto key = line.substr(0,pos);
      auto value = line.substr(pos+1, line.size());
      boost::trim(key);
      boost::trim(value);
      m_MetadataDic[key] = value;
    }
  }
}

} // end namespace otb
