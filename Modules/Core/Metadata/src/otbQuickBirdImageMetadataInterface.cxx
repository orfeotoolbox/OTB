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

std::vector<unsigned int> QuickBirdImageMetadataInterface::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 2;
  rgb[1] = 1;
  rgb[2] = 0;
  return rgb;
}

std::vector<std::string> QuickBirdImageMetadataInterface::GetEnhancedBandNames() const
{
  std::vector<std::string> enhBandNames;
  std::vector<std::string> rawBandNames = this->Superclass::GetBandName();

  // std::cout << rawBandNames.size() << std::endl;
  if (rawBandNames.size())
  {
    for (std::vector<std::string>::iterator it = rawBandNames.begin(); it != rawBandNames.end(); ++it)
    {
      // std::cout << *it << std::endl;

      // Manage Panchro case
      if ((rawBandNames.size() == 1) && !(*it).compare("P"))
      {
        enhBandNames.push_back("PAN");
        break;
      }
      else if ((rawBandNames.size() != 1) && !(*it).compare("P"))
      {
        /* Launch exception situation not valid*/
        itkExceptionMacro(<< "Invalid Metadata, we cannot provide an consistent name to the band");
      }

      // Manage MS case
      if (!(*it).compare("R"))
      {
        enhBandNames.push_back("Red");
      }
      else if (!(*it).compare("G"))
      {
        enhBandNames.push_back("Green");
      }
      else if (!(*it).compare("B"))
      {
        enhBandNames.push_back("Blue");
      }
      else if (!(*it).compare("N"))
      {
        enhBandNames.push_back("NIR");
      }
      else
      {
        enhBandNames.push_back("Unknown");
      }
    }
  }

  return enhBandNames;
}


} // end namespace otb
