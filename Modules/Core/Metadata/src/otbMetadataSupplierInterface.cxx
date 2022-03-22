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

#include "otbMetadataSupplierInterface.h"

namespace otb
{

bool MetadataSupplierInterface::HasValue(std::string const& path, int band) const
{
  bool hasValue;
  const std::string ret = GetMetadataValue(path, hasValue, band);
  return hasValue;
}

template <> bool MetadataSupplierInterface::GetAs(bool const& defaultValue, std::string const& path, int band) const
{
  auto ret = GetAs<std::string>(boost::lexical_cast<std::string>(defaultValue), path, band);
    if (std::any_of(TRUES.begin(), TRUES.end(), [ret](std::string s){return s == ret;}))
      return true;
    else if (std::any_of(FALSES.begin(), FALSES.end(), [ret](std::string s){return s == ret;}))
      return false;
    else
      return defaultValue;
}

}
