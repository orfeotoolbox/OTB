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

#include "otbTestTools.h"

namespace otb
{
namespace testtools
{

void PrintMetadataBase(ImageMetadataBase imdb, std::ostream& oss)
{
  std::unordered_map<std::string, std::string> kwl;
  imdb.ToKeywordlist(kwl);
  for (auto It = otb::MetaData::MDStrNames.left.begin() ; It != otb::MetaData::MDStrNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  for (auto It = otb::MetaData::MDNumNames.left.begin() ; It != otb::MetaData::MDNumNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  for (auto It = otb::MetaData::MDTimeNames.left.begin() ; It != otb::MetaData::MDTimeNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  for (auto It = otb::MetaData::MDGeomNames.left.begin() ; It != otb::MetaData::MDGeomNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  for (auto It = otb::MetaData::MDL1DNames.left.begin() ; It != otb::MetaData::MDL1DNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  for (auto It = otb::MetaData::MDL2DNames.left.begin() ; It != otb::MetaData::MDL2DNames.left.end() ; ++It)
  {
    auto Key = kwl.find(It->second);
    if (Key != kwl.end())
      oss << It->second << ' ' << Key->second << '\n';
  }

  std::string prefix("Extra.");
  for (const auto& kv : imdb.ExtraKeys)
    oss << prefix + kv.first << ' ' << kv.second << '\n';
}

void PrintMetadata(ImageMetadata imd, std::ostream& oss)
{
  PrintMetadataBase(imd, oss);
  for (const ImageMetadataBase & imdb : imd.Bands)
    PrintMetadataBase(imdb, oss);
}

}}
