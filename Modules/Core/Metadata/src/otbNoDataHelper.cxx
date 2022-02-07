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

#include "otbNoDataHelper.h"

#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{
bool ReadNoDataFlags(const ImageMetadata & imd, std::vector<bool>& flags, std::vector<double>& values)
{
  flags.clear();
  values.clear();

  // at least one no data flag
  bool ret = false;

  // 1) Search for no data in bands
  for (const auto & band: imd.Bands)
  {
    if (band.Has(MDNum::NoData))
    {
      ret = true;
      flags.push_back(true);
      values.push_back(band[MDNum::NoData]);
    }
    else
    {
      flags.push_back(false);
      values.push_back(0.);
    }
  }
  
  if (ret)
  {
    return true;
  }

  // 2) Search in the main metadata domain
  // Handle the case where there is no band metadata
  unsigned int outputSize = imd.Bands.size() > 0 ? imd.Bands.size() : 1;

  if (imd.Has(MDNum::NoData))
  {
    ret = true;
    flags = std::vector<bool>(outputSize, true);
    values = std::vector<double>(outputSize, imd[MDNum::NoData]);
  }
  else
  {
    flags = std::vector<bool>(outputSize, false);
    values = std::vector<double>(outputSize, 0.);
  }

  return ret;
}

void WriteNoDataFlags(const std::vector<bool>& flags, const std::vector<double>& values, ImageMetadata & imd)
{
  auto size = flags.size();
  assert(size == values.size());

  if (imd.Bands.size() < size)
  {
    imd.Bands.resize(size);
  }

  auto itFlags = flags.cbegin();
  auto itValues = values.cbegin();
  auto itBands = imd.Bands.begin();

  for (; itFlags != flags.cend(); itFlags++, itValues++, itBands++)
  {
    if (*itFlags)
    {
      itBands->Add(MDNum::NoData, *itValues);
    }
  }
}

} // End namespace otb
