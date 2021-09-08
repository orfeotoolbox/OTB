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


#include "otbQuickBirdImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

#include "itksys/SystemTools.hxx"
#include "otbStringUtilities.h"

namespace otb
{

QuickBirdImageMetadataInterface::QuickBirdImageMetadataInterface()
{
}

bool QuickBirdImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("QB02") != std::string::npos)
    return true;
  else
    return false;
}

QuickBirdImageMetadataInterface::VariableLengthVectorType QuickBirdImageMetadataInterface::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no QuickBird Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  if (keywordStringBId != panchro && keywordStringBId != multi)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  // Panchromatic case
  if (keywordStringBId == panchro)
  {
    wavel.SetSize(1);
    wavel.Fill(0.450);
  }
  else
  {
    wavel.SetSize(4);
    wavel[0] = 0.450;
    wavel[1] = 0.520;
    wavel[2] = 0.630;
    wavel[3] = 0.760;
  }

  return wavel;
}

QuickBirdImageMetadataInterface::VariableLengthVectorType QuickBirdImageMetadataInterface::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no QuickBird Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  if (keywordStringBId != panchro && keywordStringBId != multi)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  // Panchromatic case
  if (keywordStringBId == panchro)
  {
    wavel.SetSize(1);
    wavel.Fill(0.900);
  }
  else
  {
    wavel.SetSize(4);
    wavel[0] = 0.520;
    wavel[1] = 0.600;
    wavel[2] = 0.690;
    wavel[3] = 0.900;
  }

  return wavel;
}

} // end namespace otb
