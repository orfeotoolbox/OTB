/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbMetaDataKey.h"
#include "gdal.h"

#define NBKEYS  23

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
char const* CacheSizeInBytes = "CacheSizeInBytes";

char const* TileHintX = "TileHintX";
char const* TileHintY = "TileHintY";
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
  MetaDataKey::KeyTypeDef(MetaDataKey::CacheSizeInBytes,                  MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::TileHintX,                         MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::TileHintY,                         MetaDataKey::TENTIER)
  };

MetaDataKey::KeyType MetaDataKey::GetKeyType(const std::string& name)
{
  MetaDataKey::KeyType ktype(MetaDataKey::TSTRING);

  for (unsigned int i = 0; i < NBKEYS; ++i)
    {
    if ( name.find(Types[i].keyname) != std::string::npos )
      {
      ktype = Types[i].type;
      break;
      }
    }
  return (ktype);
}

OTB_GCP::OTB_GCP()
 : m_GCPCol(0),
   m_GCPRow(0),
   m_GCPX(0),
   m_GCPY(0),
   m_GCPZ(0)
{
}

OTB_GCP::OTB_GCP(const OTB_GCP &pGcp)
{
  m_Id = pGcp.m_Id;
  m_Info = pGcp.m_Info;
  m_GCPCol = pGcp.m_GCPCol;
  m_GCPRow = pGcp.m_GCPRow;
  m_GCPX = pGcp.m_GCPX;
  m_GCPY = pGcp.m_GCPY;
  m_GCPZ = pGcp.m_GCPZ;
}

OTB_GCP::~OTB_GCP() {};

void OTB_GCP::operator =(const OTB_GCP& pGcp)
{
  m_Id = pGcp.m_Id;
  m_Info = pGcp.m_Info;
  m_GCPCol = pGcp.m_GCPCol;
  m_GCPRow = pGcp.m_GCPRow;
  m_GCPX = pGcp.m_GCPX;
  m_GCPY = pGcp.m_GCPY;
  m_GCPZ = pGcp.m_GCPZ;
}
void OTB_GCP::Print(std::ostream& os) const
{
  os << "   GCP Id = " << this->m_Id << std::endl;
  os << "   GCP Info =  " << this->m_Info << std::endl;
  os << "   GCP (Row, Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
  os << "   GCP (X, Y, Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
}

} // end namespace otb
