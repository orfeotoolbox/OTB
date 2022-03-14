/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#include <unordered_map>
#include <algorithm>

#include "otbGeomMetadataSupplier.h"
#include "otbMetaDataKey.h"
#include "otbGeometryMetadata.h"
#include "otbStringUtils.h"
#include "otbSARMetadata.h"
#include "otbDateTime.h"
#include "otbSpatialReference.h"

namespace otb
{
GeomMetadataSupplier::GeomMetadataSupplier(std::string const& geomFile)
  : GeomMetadataSupplier(geomFile, "")
{}

GeomMetadataSupplier::GeomMetadataSupplier(std::string const& geomFile, const std::string & imageFile)
{
  this->m_FileNames["geom"] = geomFile;
  this->m_FileNames["image"] = imageFile;
  this->ReadGeomFile();
}

std::string GeomMetadataSupplier::GetMetadataValue(std::string const& path, bool& hasValue, int) const
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

std::string GeomMetadataSupplier::GetResourceFile(std::string const& name) const
{
  assert((name == "") || (name == "geom") || (name == "image"));
  if (name.empty())
    return this->m_FileNames.at("geom");
  return this->m_FileNames.at(name);
}

std::vector<std::string> GeomMetadataSupplier::GetResourceFiles() const
{
  return std::vector<std::string>({this->m_FileNames.at("geom"), this->m_FileNames.at("image")});
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

bool GeomMetadataSupplier::FetchGCP(ImageMetadata & imd)
{
  int numberOfGcp = GetAs<int>(0, "support_data.geom.gcp.number");
  if (numberOfGcp == 0)
    return false;

  double delta_x = 0;
  double delta_y = 0;
  // TerrasarX GCP are shifted
  if (imd[MDStr::Instrument] == "TSX-SAR")
  {
      delta_x = 1;
      delta_y = 1;
  }

  Projection::GCPParam gcpPrm;
  std::unordered_map<std::string, GCPTime> gcpTimes;
  std::ostringstream oss;
  // GCP id starts at 1
  for(int i = 1 ; i < numberOfGcp+1 ; ++i)
  {
    oss.str("");
    oss << "support_data.geom.gcp[" << i-1 << "].";
    gcpPrm.GCPs.emplace_back(std::to_string(i),                              // ID
                             "",                                             // Comment
                             GetAs<double>(oss.str() + "im_pt.x") + delta_x, // col
                             GetAs<double>(oss.str() + "im_pt.y") + delta_y, // row
                             GetAs<double>(oss.str() + "world_pt.lon"),      // px
                             GetAs<double>(oss.str() + "world_pt.lat"),      // py
                             GetAs<double>(oss.str() + "world_pt.hgt"));     // pz
    GCPTime time;
    time.azimuthTime = MetaData::ReadFormattedDate(GetAs<std::string>(oss.str() + "azimuthTime"));
    time.slantRangeTime = GetAs<double>((oss.str() + "slant_range_time"));
    gcpTimes[std::to_string(i)] = time;
  }
  gcpPrm.GCPProjection = otb::SpatialReference::FromWGS84().ToWkt();
  imd.Add(MDGeom::GCP, gcpPrm);  // This step will erase the GCP read by GDAL if any.
  if(imd.Has(MDGeom::SAR))
  {
    auto sarParam = boost::any_cast<SARParam>(imd[MDGeom::SAR]);
    sarParam.gcpTimes = std::move(gcpTimes);
    imd.Add(MDGeom::SAR, sarParam);
  }
  else
  {
    SARParam sarParam;
    sarParam.gcpTimes = std::move(gcpTimes);
    imd.Add(MDGeom::SAR, sarParam);
  }

  assert(imd.Has(MDGeom::GCP));
  return true;
}

unsigned int GeomMetadataSupplier::GetNumberOf(std::string const& pattern) const
{
  return std::count_if(m_MetadataDic.cbegin(),
                       m_MetadataDic.cend(),
                       [pattern](DictType::value_type u){return u.first.rfind(pattern, 0) == 0;}
                      );
}

std::string GeomMetadataSupplier::PrintSelf() const
{
  std::ostringstream oss;
  oss << "GeomMetadataSupplier: " << this->m_FileNames.at("geom") << '\t' << this->m_FileNames.at("image") << '\n';
  for (const auto& kv : this->m_MetadataDic)
    oss << kv.first << " : " << kv.second << '\n';
  return oss.str();
}

void GeomMetadataSupplier::ReadGeomFile()
{
  std::ifstream inputFile(this->m_FileNames.at("geom"));
  std::string line, key, value;
  std::string::size_type pos;
  while (std::getline(inputFile, line))
  {
    pos = line.find(":");
    if (pos != std::string::npos)
    {
      key = line.substr(0,pos);
      value = line.substr(pos+1, line.size());
      boost::trim(key);
      boost::trim(value);
      m_MetadataDic[key] = value;
    }
  }
}

} // end namespace otb
