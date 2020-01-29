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

#ifndef otbBandMetadata_h
#define otbBandMetadata_h

#include <string>
#include <vector>
#include "OTBMetadataExport.h"
#include "otbMetaDataKey.h"

namespace otb
{
/** \class BandMetadata
 *
 * \brief Generic structure containing channel specific metadatas
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT BandMetadata
{
// ----------- static part -------------------------

  std::string Name;

  std::string EnhancedName;
  
  bool NoDataFlag = false;

  double NoDataValue = 0.0;

// --------- dynamic part ----------------------------

  std::map<MetaData::NumericKeyType, double> NumericKeys;

  std::map<MetaData::StringKeyType, std::string> StringKeys;

  std::map<MetaData::LUT1DKeyType, MetaData::LUT1D > LUT1DKeys;

  std::map<MetaData::LUT2DKeyType, MetaData::LUT2D > LUT2DKeys;
};

extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::BandMetadata& bmd);

} // end namespace otb

#endif
