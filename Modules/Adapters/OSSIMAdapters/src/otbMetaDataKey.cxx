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


#include "otbMetaDataKey.h"

namespace otb
{
namespace MetaDataKey
{
char const* DriverShortNameKey = "DriverShortName";
char const* DriverLongNameKey = "DriverLongName";

char const* ProjectionRefKey = "ProjectionRef";

char const* GCPProjectionKey = "GCPProjection";
char const* GCPParametersKey = "GCP_";
char const* GCPCountKey = "GCPCount";

char const* GeoTransformKey = "GeoTransform";

char const* MetadataKey = "Metadata_";
char const* SubMetadataKey = "SubMetadata_";

char const* UpperLeftCornerKey = "UpperLeftCorner";
char const* UpperRightCornerKey = "UpperRightCorner";
char const* LowerLeftCornerKey = "LowerLeftCorner";
char const* LowerRightCornerKey = "LowerRightCorner";

char const* ColorTableNameKey = "ColorTableName";
char const* ColorEntryCountKey = "ColorEntryCount";
char const* ColorEntryAsRGBKey = "ColorEntryAsRGB";

char const* OSSIMKeywordlistKey = "OSSIMKeywordlist";
char const* OSSIMKeywordlistDelimiterKey = "OSSIMKeywordlistDelimiter";

char const* VectorDataKeywordlistKey = "VectorDataKeywordlist";
char const* VectorDataKeywordlistDelimiterKey = "VectorDataKeywordlistDelimiter";

char const* ResolutionFactor = "ResolutionFactor";
char const* SubDatasetIndex = "SubDatasetIndex";
char const* CacheSizeInBytes = "CacheSizeInBytes";

char const* TileHintX = "TileHintX";
char const* TileHintY = "TileHintY";

char const * NoDataValueAvailable = "NoDataValueAvailable";
char const * NoDataValue = "NoDataValue";

char const * DataType = "DataType";
}

const MetaDataKey::KeyTypeDef Types[] =
  {
  MetaDataKey::KeyTypeDef(MetaDataKey::DriverShortNameKey,                MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::DriverLongNameKey,                 MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::ProjectionRefKey,                  MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPProjectionKey,                  MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPParametersKey,                  MetaDataKey::TOTB_GCP),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPCountKey,                       MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::GeoTransformKey,                   MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::MetadataKey,                       MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::SubMetadataKey,                    MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::UpperLeftCornerKey,                MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::UpperRightCornerKey,               MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::LowerLeftCornerKey,                MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::LowerRightCornerKey,               MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorTableNameKey,                 MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryCountKey,                MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryAsRGBKey,                MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::OSSIMKeywordlistKey,               MetaDataKey::TOSSIMKEYWORDLIST),
  MetaDataKey::KeyTypeDef(MetaDataKey::OSSIMKeywordlistDelimiterKey,      MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistKey,          MetaDataKey::TVECTORDATAKEYWORDLIST),
  MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistDelimiterKey, MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::ResolutionFactor,                  MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::SubDatasetIndex,                   MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::CacheSizeInBytes,                  MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::TileHintX,                         MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::TileHintY,                         MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::NoDataValueAvailable,              MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::NoDataValue,                       MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::DataType,                          MetaDataKey::TENTIER),
  };

MetaDataKey::KeyType MetaDataKey::GetKeyType(const std::string& name)
{
  // MetaDataKey::KeyType ktype(MetaDataKey::TSTRING);

  for ( const auto & key : Types )
    {
    if ( name.find(key.keyname) != std::string::npos )
      {
      return MetaDataKey::KeyType(key.type);
      // ktype = key.type;
      // return ktype;
      }
    }
  return MetaDataKey::KeyType(MetaDataKey::TSTRING);
}

OTB_GCP::OTB_GCP()
 : m_GCPCol(0),
   m_GCPRow(0),
   m_GCPX(0),
   m_GCPY(0),
   m_GCPZ(0)
{
}

OTB_GCP::~OTB_GCP()
{
}

void OTB_GCP::Print(std::ostream& os) const
{
  os << "   GCP Id = " << this->m_Id << std::endl;
  os << "   GCP Info =  " << this->m_Info << std::endl;
  os << "   GCP (Row, Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
  os << "   GCP (X, Y, Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
}

} // end namespace otb
