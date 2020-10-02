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


#include "otbOpticalImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

OpticalImageMetadataInterface::OpticalImageMetadataInterface()
{
}

double OpticalImageMetadataInterface::GetSunElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  ImageKeywordlistType          imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (imageKeywordlist.HasKey("support_data.elevation_angle"))
  {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.elevation_angle");
    double      value       = atof(valueString.c_str());
    return value;
  }

  return 0;
}

double OpticalImageMetadataInterface::GetSunAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  ImageKeywordlistType          imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  if (imageKeywordlist.HasKey("support_data.azimuth_angle"))
  {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.azimuth_angle");
    double      value       = atof(valueString.c_str());
    return value;
  }

  return 0;
}


unsigned int OpticalImageMetadataInterface::BandIndexToWavelengthPosition(unsigned int i) const
{
  return i;
}


void OpticalImageMetadataInterface::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);

  if (this->CanRead())
  {
    os << indent << "GetSunElevation:     " << this->GetSunElevation() << std::endl;
    os << indent << "GetSunAzimuth:       " << this->GetSunAzimuth() << std::endl;
    os << indent << "GetSatElevation:     " << this->GetSatElevation() << std::endl;
    os << indent << "GetSatAzimuth:       " << this->GetSatAzimuth() << std::endl;
    os << indent << "GetPhysicalBias:     " << this->GetPhysicalBias() << std::endl;
    os << indent << "GetPhysicalGain:     " << this->GetPhysicalGain() << std::endl;
    os << indent << "GetSolarIrradiance:  " << this->GetSolarIrradiance() << std::endl;
    os << indent << "GetFirstWavelengths: " << this->GetFirstWavelengths() << std::endl;
    os << indent << "GetLastWavelengths:  " << this->GetLastWavelengths() << std::endl;
  }
}


} // end namespace otb
