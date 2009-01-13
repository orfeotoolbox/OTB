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


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbMetaDataKey.h"

#define NBKEYS  15

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
    MetaDataKey::KeyTypeDef(MetaDataKey::GCPParametersKey ,       MetaDataKey::TOTB_GCP),
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
    MetaDataKey::KeyTypeDef(MetaDataKey::OSSIMKeywordlistDelimiterKey,MetaDataKey::TSTRING)
  };


MetaDataKey::KeyType MetaDataKey::GetKeyType(std::string name)
{
  MetaDataKey::KeyType ktype(MetaDataKey::TSTRING);

  for ( unsigned int i=0; i<NBKEYS; i++)
  {
//      if ( strcmp(name.c_str(),Types[i].keyname.c_str()) == 0 )


    if ( (name.find(Types[i].keyname) >= 0 ) &&
          (name.find(Types[i].keyname) <= name.size() ) )
    {
      ktype = Types[i].type;
      break;
    }

  }
  return(ktype);
}


} // end namespace otb
