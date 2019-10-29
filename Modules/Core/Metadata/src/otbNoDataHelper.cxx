/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

bool ReadNoDataFlags(const itk::MetaDataDictionary& dict, std::vector<bool>& flags, std::vector<double>& values)
{
  bool ret = itk::ExposeMetaData<std::vector<bool>>(dict, MetaDataKey::NoDataValueAvailable, flags);

  if (ret)
    ret = itk::ExposeMetaData<std::vector<double>>(dict, MetaDataKey::NoDataValue, values);

  return ret;
}

void WriteNoDataFlags(const std::vector<bool>& flags, const std::vector<double>& values, itk::MetaDataDictionary& dict)
{
  itk::EncapsulateMetaData<std::vector<bool>>(dict, MetaDataKey::NoDataValueAvailable, flags);
  itk::EncapsulateMetaData<std::vector<double>>(dict, MetaDataKey::NoDataValue, values);
}

} // End namespace otb
