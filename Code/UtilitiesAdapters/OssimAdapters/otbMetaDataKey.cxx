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

#define NBKEYS  19

namespace otb
{

const std::string MetaDataKey::DriverShortNameKey = "DriverShortName";
const std::string MetaDataKey::DriverLongNameKey = "DriverLongName";

const std::string MetaDataKey::ProjectionRefKey = "ProjectionRef";

const std::string MetaDataKey::GCPProjectionKey = "GCPProjection";
const std::string MetaDataKey::GCPParametersKey = "GCP_";
const std::string MetaDataKey::GCPCountKey = "GCPCount";

const std::string MetaDataKey::GeoTransformKey = "GeoTransform";

const std::string MetaDataKey::MetadataKey = "Metadata_";
const std::string MetaDataKey::SubMetadataKey = "SubMetadata_";

const std::string MetaDataKey::UpperLeftCornerKey = "UpperLeftCorner";
const std::string MetaDataKey::UpperRightCornerKey = "UpperRightCorner";
const std::string MetaDataKey::LowerLeftCornerKey = "LowerLeftCorner";
const std::string MetaDataKey::LowerRightCornerKey = "LowerRightCorner";

const std::string MetaDataKey::ColorTableNameKey = "ColorTableName";
const std::string MetaDataKey::ColorEntryCountKey = "ColorEntryCount";
const std::string MetaDataKey::ColorEntryAsRGBKey = "ColorEntryAsRGB";

const std::string MetaDataKey::OSSIMKeywordlistKey = "OSSIMKeywordlist";
const std::string MetaDataKey::OSSIMKeywordlistDelimiterKey = "OSSIMKeywordlistDelimiter";

const std::string MetaDataKey::VectorDataKeywordlistKey = "VectorDataKeywordlist";
const std::string MetaDataKey::VectorDataKeywordlistDelimiterKey = "VectorDataKeywordlistDelimiter";

/*const MetaDataKey::KeyTypeDef Types[] =
  {
    {MetaDataKey::DriverShortNameKey,     MetaDataKey::TSTRING},
    {MetaDataKey::DriverLongNameKey,     MetaDataKey::TSTRING},
    {MetaDataKey::ProjectionRefKey,     MetaDataKey::TSTRING},
    {MetaDataKey::GCPProjectionKey,     MetaDataKey::TSTRING},
    {MetaDataKey::GCPParametersKey ,        MetaDataKey::TOTB_GCP},
    {MetaDataKey::GCPCountKey,          MetaDataKey::TENTIER},
    {MetaDataKey::GeoTransformKey,        MetaDataKey::TSTRING},
    {MetaDataKey::MetadataKey,          MetaDataKey::TSTRING},
    {MetaDataKey::UpperLeftCornerKey,        MetaDataKey::TVECTOR},
    {MetaDataKey::UpperRightCornerKey,      MetaDataKey::TVECTOR},
    {MetaDataKey::LowerLeftCornerKey,        MetaDataKey::TVECTOR},
    {MetaDataKey::LowerRightCornerKey,      MetaDataKey::TVECTOR},
    {MetaDataKey::ColorTableNameKey,        MetaDataKey::TSTRING},
    {MetaDataKey::ColorEntryCountKey,       MetaDataKey::TENTIER},
    {MetaDataKey::ColorEntryAsRGBKey,        MetaDataKey::TVECTOR}


  }; */
const MetaDataKey::KeyTypeDef Types[] =
  {
  MetaDataKey::KeyTypeDef(MetaDataKey::DriverShortNameKey,    MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::DriverLongNameKey,     MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::ProjectionRefKey,     MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPProjectionKey,     MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPParametersKey,       MetaDataKey::TOTB_GCP),
  MetaDataKey::KeyTypeDef(MetaDataKey::GCPCountKey,          MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::GeoTransformKey,        MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::MetadataKey,          MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::UpperLeftCornerKey,      MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::UpperRightCornerKey,      MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::LowerLeftCornerKey,      MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::LowerRightCornerKey,      MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorTableNameKey,      MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryCountKey,     MetaDataKey::TENTIER),
  MetaDataKey::KeyTypeDef(MetaDataKey::ColorEntryAsRGBKey,      MetaDataKey::TVECTOR),
  MetaDataKey::KeyTypeDef(MetaDataKey::OSSIMKeywordlistKey,    MetaDataKey::TOSSIMKEYWORDLIST),
  MetaDataKey::KeyTypeDef(MetaDataKey::OSSIMKeywordlistDelimiterKey, MetaDataKey::TSTRING),
  MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistKey,    MetaDataKey::TVECTORDATAKEYWORDLIST),
  MetaDataKey::KeyTypeDef(MetaDataKey::VectorDataKeywordlistDelimiterKey, MetaDataKey::TSTRING)
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

OTB_GCP::OTB_GCP() {};
OTB_GCP::OTB_GCP(const GDAL_GCP * psGCP)
{
  m_Id = std::string(psGCP->pszId);
  m_Info = std::string(psGCP->pszInfo);
  m_GCPRow = psGCP->dfGCPLine;
  m_GCPCol = psGCP->dfGCPPixel;
  m_GCPX = psGCP->dfGCPX;
  m_GCPY = psGCP->dfGCPY;
  m_GCPZ = psGCP->dfGCPZ;
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
