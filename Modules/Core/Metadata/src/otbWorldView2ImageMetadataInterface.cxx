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


#include "otbWorldView2ImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

#include "itksys/SystemTools.hxx"
#include "otbStringUtilities.h"

namespace otb
{

bool WorldView2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("WV02") != std::string::npos)
    return true;
  else
    return false;
}

void WorldView2ImageMetadataInterface::FetchSolarIrradianceWorldView2()
{
  // Reference:
  // Radiometric_Use_of_WorldView-2_Imagery, Technical Note, 2010 (Digital Globe)
  

  auto productType = m_Imd[MDStr::ProductType];

  std::string              panchro("P");
  std::string              multi("Multi");
  std::string              ms1("MS1");

  if (productType == panchro)
  {
    m_Imd.Bands[0].Add(MDNum::SolarIrradiance, 1580.8140);
  }
  else
  {
    const std::unordered_map<std::string, double> BandNameToSolarIrradianceTable{
       {"C", 1758.2229},
       {"B", 1974.2416},
       {"G", 1856.4104},
       {"Y", 1738.4791},
       {"R", 1559.4555},
       {"RE", 1342.0695},
       {"N", 1069.7302},
       {"N2", 861.2866}};

    for (auto & band: m_Imd.Bands)
    {
      auto solarIrradianceIt = BandNameToSolarIrradianceTable.find(band[MDStr::BandName] );
      if (solarIrradianceIt != BandNameToSolarIrradianceTable.end())
      {
        band.Add(MDNum::SolarIrradiance, solarIrradianceIt->second);
      }
      else
      {
        band.Add(MDNum::SolarIrradiance, 0.0);
      }
    }
  }
}

void WorldView2ImageMetadataInterface::FetchWavelengthsWorldView2()
{
  auto productType = m_Imd[MDStr::ProductType];

  std::string              panchro("P");
  std::string              multi("Multi");
  std::string              ms1("MS1");

  if (productType == panchro)
  {
    m_Imd.Bands[0].Add(MDNum::FirstWavelength, 0.464);
    m_Imd.Bands[0].Add(MDNum::LastWavelength, 0.801);
  }
  else
  {
    //TODO : check result, there might be an error in the old code : lastWavelength blue : 0.581
    const std::unordered_map<std::string, std::pair<double, double>> 
              BandNameToWavelengthTable{
       {"C", {0.401, 0.453}} ,
       {"B", {0.447, 0.508}},
       {"G", {0.511, 0.581}},
       {"Y", {0.588, 0.627}},
       {"R", {0.629, 0.689}},
       {"RE", {0.704, 0.744}},
       {"N", {0.772, 0.890}},
       {"N2", {0.862, 0.954}}};

    for (auto & band: m_Imd.Bands)
    {
      auto wavelengthIt = BandNameToWavelengthTable.find(band[MDStr::BandName] );
      if (wavelengthIt != BandNameToWavelengthTable.end())
      {
        band.Add(MDNum::FirstWavelength, wavelengthIt->second.first);
        band.Add(MDNum::LastWavelength, wavelengthIt->second.second);
      }
    }
  }
}

WorldView2ImageMetadataInterface::VariableLengthVectorType WorldView2ImageMetadataInterface::GetSolarIrradiance() const
{
  // Used the solari irradiance provided by DigitalGlobe here:
  // http://www.digitalglobe.com/downloads/Radiometric_Use_of_WorldView-2_Imagery.pdf

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  VariableLengthVectorType outputValuesVariableLengthVector;

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  std::string ms1("MS1");

  if (keywordStringBId == panchro)
  {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(1580.8140);
  }
  else
  {
    std::string              keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    outputValuesVariableLengthVector.SetSize(bandNameList.size());
    for (unsigned int i = 0; i < bandNameList.size(); ++i)
    {
      double SolarIrradianceValue = 0.0;
      if (bandNameList[i] == "C")
      {
        SolarIrradianceValue = 1758.2229;
      }
      else if (bandNameList[i] == "B")
      {
        SolarIrradianceValue = 1974.2416;
      }
      else if (bandNameList[i] == "G")
      {
        SolarIrradianceValue = 1856.4104;
      }
      else if (bandNameList[i] == "Y")
      {
        SolarIrradianceValue = 1738.4791;
      }
      else if (bandNameList[i] == "R")
      {
        SolarIrradianceValue = 1559.4555;
      }
      else if (bandNameList[i] == "RE")
      {
        SolarIrradianceValue = 1342.0695;
      }
      else if (bandNameList[i] == "N")
      {
        SolarIrradianceValue = 1069.7302;
      }
      else if (bandNameList[i] == "N2")
      {
        SolarIrradianceValue = 861.2866;
      }
      outputValuesVariableLengthVector[i] = SolarIrradianceValue;
    }
  }

  return outputValuesVariableLengthVector;
}

int WorldView2ImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.tlc_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.tlc_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.tlc_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.tlc_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[3].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.tlc_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[4].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.generation_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.generation_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int WorldView2ImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.generation_date");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string              valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2)
    itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType WorldView2ImageMetadataInterface::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  VariableLengthVectorType outputValuesVariableLengthVector;
  std::string              keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");

  std::string              panchro("P");
  std::string              multi("Multi");
  std::string              ms1("MS1");

  if (keywordStringBId == panchro)
  {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(0.0);
  }
  else if (keywordStringBId == multi || keywordStringBId == ms1)
  {
    std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    outputValuesVariableLengthVector.SetSize(bandNameList.size());
    outputValuesVariableLengthVector.Fill(0.0);
  }
  else
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  return outputValuesVariableLengthVector;
}


void WorldView2ImageMetadataInterface::FetchPhysicalBias()
{
  auto productType = m_Imd[MDStr::ProductType];

  std::string              panchro("P");
  std::string              multi("Multi");
  std::string              ms1("MS1");

  if (productType == panchro)
  {
    m_Imd.Bands[0].Add(MDNum::PhysicalBias, 0.0);
  }
  else if (productType == multi || productType == ms1)
  {
    for (auto & band : m_Imd.Bands)
    {
      band.Add(MDNum::PhysicalBias, 0.0);
    }
  }
  else
  {
    itkExceptionMacro(<< "Invalid bandID " << productType);
  }
}

WorldView2ImageMetadataInterface::VariableLengthVectorType WorldView2ImageMetadataInterface::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string keywordStringBitsPerPixel = imageKeywordlist.GetMetadataByKey("support_data.bits_per_pixel");
  int         bitsPerPixel              = atoi(keywordStringBitsPerPixel.c_str());
  if (bitsPerPixel != 16)
  {
    itkExceptionMacro(<< "Invalid bitsPerPixel " << bitsPerPixel);
  }

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  std::string ms1("MS1");

  if (keywordStringBId != panchro && keywordStringBId != multi && keywordStringBId != ms1)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  std::string keywordStringTDILevel = imageKeywordlist.GetMetadataByKey("support_data.TDI_level");
  int         TDILevel              = atoi(keywordStringTDILevel.c_str());
  if ((keywordStringBId == panchro && TDILevel > 64) || (keywordStringBId == panchro && TDILevel < 8))
  {
    itkExceptionMacro(<< "Invalid TDILevel " << TDILevel);
  }

  std::string              keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
  std::vector<std::string> bandNameList;
  boost::trim(keywordStringBandNameList);
  boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(bandNameList.size());

  if (keywordStringBId == panchro)
  {
    std::string keywordStringAbsCalFactor = imageKeywordlist.GetMetadataByKey("support_data.absCalFactor");
    double      absCalFactor              = atof(keywordStringAbsCalFactor.c_str());
    outputValuesVariableLengthVector[0]   = absCalFactor;
  }
  else
  {
    for (unsigned int i = 0; i < bandNameList.size(); ++i)
    {
      std::string key                     = "support_data." + bandNameList[i] + "_band_absCalFactor";
      std::string keywordStringCalFactor  = imageKeywordlist.GetMetadataByKey(key);
      double      calFactor               = atof(keywordStringCalFactor.c_str());
      outputValuesVariableLengthVector[i] = calFactor;
    }
  }

  if (keywordStringBId == panchro)
  {
    // In the case of WorldView-2, we need to divide the absolute calibration
    // factor by the effective bandwidth see for details:
    // http://www.digitalglobe.com/downloads/Radiometric_Use_of_WorldView-2_Imagery.pdf
    // These values are not retrieved by the ossimQuickBirdMetadata class as
    // there are specific to WV2 We did not retrieve those values in the ossim
    // class and consider them as constant values
    // FIXME: add the effectivebandwidth in the ossim keywordlist

    const double effectiveBandWidthPan  = 2.846000e-01;
    outputValuesVariableLengthVector[0] = effectiveBandWidthPan / outputValuesVariableLengthVector[0];
  }
  else
  {
    // Also need to divide by the effective bandwidth available
    // We consider the 2 cases with 4 or 8 bands in WV2 multispectral product
    VariableLengthVectorType effectiveBandwidthVariableLengthVector;
    effectiveBandwidthVariableLengthVector.SetSize(bandNameList.size());
    effectiveBandwidthVariableLengthVector.Fill(1.);

    if (bandNameList.size() == 8)
    {
      effectiveBandwidthVariableLengthVector[0] = 4.730000e-02;
      effectiveBandwidthVariableLengthVector[1] = 5.430000e-02;
      effectiveBandwidthVariableLengthVector[2] = 6.300000e-02;
      effectiveBandwidthVariableLengthVector[3] = 3.740000e-02;
      effectiveBandwidthVariableLengthVector[4] = 5.740000e-02;
      effectiveBandwidthVariableLengthVector[5] = 3.930000e-02;
      effectiveBandwidthVariableLengthVector[6] = 9.890000e-02;
      effectiveBandwidthVariableLengthVector[7] = 9.960000e-02;
    }
    else if (bandNameList.size() == 4)
    {
      effectiveBandwidthVariableLengthVector[0] = 5.430000e-02;
      effectiveBandwidthVariableLengthVector[1] = 6.300000e-02;
      effectiveBandwidthVariableLengthVector[2] = 5.740000e-02;
      effectiveBandwidthVariableLengthVector[3] = 9.890000e-02;
    }

    for (unsigned int i = 0; i < bandNameList.size(); ++i)
    {
      outputValuesVariableLengthVector[i] = effectiveBandwidthVariableLengthVector[i] / outputValuesVariableLengthVector[i];
    }
  }

  return outputValuesVariableLengthVector;
}

double WorldView2ImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }
  std::string key("support_data.sat_elevation_angle");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  double      value       = atof(valueString.c_str());
  return value;
}

double WorldView2ImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string key("support_data.sat_azimuth_angle");
  if (!imageKeywordlist.HasKey(key))
  {
    return -1;
  }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  double      value       = atof(valueString.c_str());
  return value;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType WorldView2ImageMetadataInterface::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  std::string ms1("MS1");

  if (keywordStringBId != panchro && keywordStringBId != multi && keywordStringBId != ms1)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  // Panchromatic case
  if (keywordStringBId == panchro)
  {
    wavel.SetSize(1);
    wavel.Fill(0.464);
  }
  else
  {
    std::string              keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    wavel.SetSize(bandNameList.size());
    for (unsigned int i = 0; i < bandNameList.size(); ++i)
    {
      double waveValue = 0.0;
      if (bandNameList[i] == "C")
      {
        waveValue = 0.401;
      }
      else if (bandNameList[i] == "B")
      {
        waveValue = 0.447;
      }
      else if (bandNameList[i] == "G")
      {
        waveValue = 0.511;
      }
      else if (bandNameList[i] == "Y")
      {
        waveValue = 0.588;
      }
      else if (bandNameList[i] == "R")
      {
        waveValue = 0.629;
      }
      else if (bandNameList[i] == "RE")
      {
        waveValue = 0.704;
      }
      else if (bandNameList[i] == "N")
      {
        waveValue = 0.772;
      }
      else if (bandNameList[i] == "N2")
      {
        waveValue = 0.862;
      }
      wavel[i] = waveValue;
    }
  }

  return wavel;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType WorldView2ImageMetadataInterface::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
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
  std::string ms1("MS1");

  if (keywordStringBId != panchro && keywordStringBId != multi && keywordStringBId != ms1)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  // Panchromatic case
  if (keywordStringBId == panchro)
  {
    wavel.SetSize(1);
    wavel.Fill(0.801);
  }
  else
  {
    std::string              keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    wavel.SetSize(bandNameList.size());
    for (unsigned int i = 0; i < bandNameList.size(); ++i)
    {
      double waveValue = 0.0;
      if (bandNameList[i] == "C")
      {
        waveValue = 0.453;
      }
      else if (bandNameList[i] == "B")
      {
        waveValue = 0.581;
      }
      else if (bandNameList[i] == "G")
      {
        waveValue = 0.581;
      }
      else if (bandNameList[i] == "Y")
      {
        waveValue = 0.627;
      }
      else if (bandNameList[i] == "R")
      {
        waveValue = 0.689;
      }
      else if (bandNameList[i] == "RE")
      {
        waveValue = 0.744;
      }
      else if (bandNameList[i] == "N")
      {
        waveValue = 0.890;
      }
      else if (bandNameList[i] == "N2")
      {
        waveValue = 0.954;
      }
      wavel[i] = waveValue;
    }
  }
  return wavel;
}

std::vector<unsigned int> WorldView2ImageMetadataInterface::GetDefaultDisplay() const
{
  // Handle both 4 bands and 8 bands wv2 products
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, not a WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string               key = "support_data.band_name_list";
  std::vector<unsigned int> rgb(3);
  if (imageKeywordlist.HasKey(key))
  {
    std::string              keywordStringBandNameList = imageKeywordlist.GetMetadataByKey(key);
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    if (bandNameList.size() > 4)
    {
      rgb[0] = 4;
      rgb[1] = 2;
      rgb[2] = 1;
    }
    else
    {
      rgb[0] = 2;
      rgb[1] = 1;
      rgb[2] = 0;
    }
  }
  else
  {
    rgb[0] = 2;
    rgb[1] = 1;
    rgb[2] = 0;
  }
  return rgb;
}

std::vector<std::string> WorldView2ImageMetadataInterface::GetEnhancedBandNames() const
{
  std::vector<std::string> enhBandNames;
  std::vector<std::string> rawBandNames = this->Superclass::GetBandName();

  if (rawBandNames.size())
  {
    for (std::vector<std::string>::iterator it = rawBandNames.begin(); it != rawBandNames.end(); ++it)
    {
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
      if (!(*it).compare("C"))
      {
        enhBandNames.push_back("Coastal");
      }
      else if (!(*it).compare("B"))
      {
        enhBandNames.push_back("Blue");
      }
      else if (!(*it).compare("G"))
      {
        enhBandNames.push_back("Green");
      }
      else if (!(*it).compare("Y"))
      {
        enhBandNames.push_back("Yellow");
      }
      else if (!(*it).compare("R"))
      {
        enhBandNames.push_back("Red");
      }
      else if (!(*it).compare("RE"))
      {
        enhBandNames.push_back("RedEdge");
      }
      else if (!(*it).compare("N"))
      {
        enhBandNames.push_back("NIR1");
      }
      else if (!(*it).compare("N2"))
      {
        enhBandNames.push_back("NIR2");
      }
      else
      {
        enhBandNames.push_back("Unknown");
      }
    }
  }

  return enhBandNames;
}

WorldView2ImageMetadataInterface::WavelengthSpectralBandVectorType WorldView2ImageMetadataInterface::GetSpectralSensitivity() const
{
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::list<std::vector<float>> tmpSpectralBandList;

  // Read sensor image type (panchromatic or multispectral)
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
  {
    itkExceptionMacro(<< "Invalid Metadata, no WorldView2 Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  std::string ms1("MS1");

  if (keywordStringBId != panchro && keywordStringBId != multi && keywordStringBId != ms1)
  {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
  }

  // Panchromatic case
  if (keywordStringBId == panchro)
  {
    const float b0[301] = {
        5.53407e-06f, 6.18974e-06f, 6.77586e-06f, 4.47741e-06f, 2.18462e-06f, 2.35814e-06f, 2.74394e-06f, 3.35619e-06f, 4.12548e-06f, 3.76678e-06f,
        2.97253e-06f, 3.87911e-06f, 4.9452e-06f,  4.23503e-06f, 3.35741e-06f, 3.66642e-06f, 4.01401e-06f, 5.01097e-06f, 6.02754e-06f, 5.03579e-06f,
        3.95097e-06f, 3.61214e-06f, 3.26677e-06f, 3.72693e-06f, 4.18733e-06f, 5.20028e-06f, 6.2917e-06f,  4.84978e-06f, 3.39534e-06f, 6.07837e-06f,
        8.8389e-06f,  1.04941e-05f, 1.21421e-05f, 4.98988e-05f, 8.81089e-05f, 0.000272275f, 0.000459932f, 0.00169883f,  0.00295147f,  0.0559008f,
        0.109766f,    0.271344f,    0.436163f,    0.461822f,    0.487533f,    0.494856f,    0.504992f,    0.515215f,    0.525596f,    0.529589f,
        0.535583f,    0.529756f,    0.522233f,    0.527207f,    0.53366f,     0.528602f,    0.523089f,    0.541112f,    0.55728f,     0.55817f,
        0.558232f,    0.580899f,    0.605533f,    0.616208f,    0.626792f,    0.612118f,    0.595414f,    0.612008f,    0.630712f,    0.649153f,
        0.671541f,    0.663704f,    0.654733f,    0.672532f,    0.693098f,    0.691335f,    0.688237f,    0.67829f,     0.664973f,    0.683323f,
        0.70156f,     0.705992f,    0.713576f,    0.709848f,    0.709786f,    0.738422f,    0.765752f,    0.762756f,    0.757827f,    0.759162f,
        0.759818f,    0.779983f,    0.794085f,    0.797935f,    0.800715f,    0.811351f,    0.81929f,     0.820814f,    0.82303f,     0.829794f,
        0.838869f,    0.848913f,    0.860802f,    0.858037f,    0.858462f,    0.865197f,    0.870285f,    0.877932f,    0.889278f,    0.896475f,
        0.909253f,    0.91308f,     0.917885f,    0.914221f,    0.910762f,    0.916979f,    0.92489f,     0.93683f,     0.950791f,    0.943998f,
        0.936911f,    0.932311f,    0.929417f,    0.938274f,    0.951232f,    0.960012f,    0.962781f,    0.957501f,    0.953865f,    0.954752f,
        0.957376f,    0.968155f,    0.977735f,    0.961332f,    0.949217f,    0.946955f,    0.953334f,    0.946656f,    0.952193f,    0.96247f,
        0.969595f,    0.983968f,    1.0f,         0.99432f,     0.994003f,    0.980974f,    0.969563f,    0.968253f,    0.968397f,    0.973674f,
        0.978855f,    0.966847f,    0.963741f,    0.952252f,    0.946654f,    0.94592f,     0.952938f,    0.954759f,    0.962583f,    0.962142f,
        0.956637f,    0.950381f,    0.940962f,    0.936224f,    0.924596f,    0.923379f,    0.921684f,    0.933568f,    0.940831f,    0.957977f,
        0.963832f,    0.958143f,    0.941297f,    0.92426f,     0.903f,       0.895878f,    0.887152f,    0.910523f,    0.929402f,    0.744019f,
        0.553867f,    0.320246f,    0.0858164f,   0.0514292f,   0.0172983f,   0.0120102f,   0.00659981f,  0.00436796f,  0.00213134f,  0.00153565f,
        0.000922227f, 0.000721841f, 0.000528696f, 0.000489722f, 0.000449662f, 0.000431783f, 0.000409049f, 0.000407132f, 0.000396445f, 0.000420259f,
        0.000446526f, 0.000403311f, 0.000361257f, 0.00033034f,  0.000298026f, 0.000301128f, 0.000308537f, 0.000286243f, 0.000261497f, 0.000253604f,
        0.000242266f, 0.000274627f, 0.000307033f, 0.000295007f, 0.000283703f, 0.000261529f, 0.000237475f, 0.00024833f,  0.000258997f, 0.00027011f,
        0.000282485f, 0.000261654f, 0.000240819f, 0.000248561f, 0.00025802f,  0.000273541f, 0.000287576f, 0.000282785f, 0.000276218f, 0.000275306f,
        0.000273131f, 0.000256767f, 0.000242183f, 0.000239187f, 0.000234252f, 0.000223355f, 0.000213509f, 0.00021573f,  0.000218602f, 0.000204845f,
        0.000191131f, 0.000179884f, 0.000169031f, 0.000177057f, 0.000183172f, 0.00016757f,  0.0001513f,   0.000138821f, 0.000125806f, 0.000129696f,
        0.000133722f, 0.000122536f, 0.000111436f, 0.000102899f, 9.41714e-05f, 0.000104721f, 0.000115227f, 9.94755e-05f, 8.3857e-05f,  8.4497e-05f,
        8.52612e-05f, 7.76124e-05f, 7.00705e-05f, 7.06672e-05f, 7.1081e-05f,  6.5671e-05f,  6.00304e-05f, 6.21262e-05f, 6.42018e-05f, 5.57459e-05f,
        4.73181e-05f, 4.33172e-05f, 3.91675e-05f, 3.60807e-05f, 3.29177e-05f, 3.41277e-05f, 3.52334e-05f, 3.14413e-05f, 2.76314e-05f, 2.55335e-05f,
        2.34256e-05f, 2.1133e-05f,  1.88297e-05f, 1.80004e-05f, 1.71146e-05f, 1.51355e-05f, 1.31473e-05f, 1.21372e-05f, 1.11225e-05f, 9.97857e-06f,
        8.84718e-06f, 8.76091e-06f, 8.67416e-06f, 7.95176e-06f, 7.22607e-06f, 7.06298e-06f, 6.8991e-06f,  6.66214e-06f, 6.42406e-06f, 6.36617e-06f,
        6.32615e-06f};

    // add panchromatic band to the temporary list
    const std::vector<float> vb0(b0, b0 + sizeof(b0) / sizeof(float));
    tmpSpectralBandList.push_back(vb0);
  }
  else
  {
    const float b1[301] = {
        1.93924e-06f, 3.33156e-06f, 3.2732e-06f,  2.78199e-06f, 1.7488e-06f,  3.34801e-06f, 3.55785e-06f, 4.38919e-06f, 2.94112e-06f, 6.21317e-06f,
        5.37163e-06f, 5.00497e-06f, 7.41645e-06f, 1.35004e-05f, 4.53797e-05f, 0.000173069f, 0.000634651f, 0.00341498f,  0.0274284f,   0.132903f,
        0.355988f,    0.59258f,     0.700719f,    0.739625f,    0.765307f,    0.787842f,    0.817088f,    0.838687f,    0.86178f,     0.883513f,
        0.905055f,    0.917072f,    0.927208f,    0.947658f,    0.965088f,    0.979304f,    0.985921f,    0.989094f,    0.997236f,    0.974613f,
        0.85394f,     0.588032f,    0.281577f,    0.0805215f,   0.00894615f,  0.00133307f,  0.0003609f,   0.000145392f, 7.12168e-05f, 3.80253e-05f,
        1.15412e-05f, 9.59192e-06f, 5.14417e-06f, 1.11808e-05f, 1.09998e-05f, 1.07851e-05f, 9.08862e-06f, 1.0105e-05f,  1.17289e-05f, 6.3851e-06f,
        1.30646e-06f, 1.3751e-06f,  8.62503e-07f, 6.19172e-07f, 7.02247e-07f, 8.87865e-07f, 9.3924e-07f,  1.11657e-06f, 9.84522e-07f, 7.45006e-07f,
        8.99363e-07f, 1.24448e-06f, 3.62747e-06f, 1.27768e-06f, 4.00518e-07f, 5.21386e-07f, 2.69075e-07f, 4.85233e-07f, 3.69503e-07f, 1.25632e-05f,
        0.00014168f,  2.39565e-06f, 3.06503e-07f, 2.7473e-07f,  5.19969e-07f, 4.87974e-07f, 2.69412e-07f, 3.10803e-07f, 6.51482e-08f, 1.769e-10f,
        2.06764e-07f, 1.56239e-06f, 1.71434e-06f, 3.76485e-07f, 9.78272e-08f, 1.07281e-07f, 5.25843e-07f, 2.86289e-06f, 4.49334e-06f, 2.7912e-06f,
        9.77366e-07f, 1.65592e-06f, 1.25872e-06f, 1.35006e-06f, 2.26827e-06f, 3.08804e-06f, 6.08055e-06f, 1.15782e-05f, 1.00862e-05f, 5.55949e-06f,
        3.85934e-06f, 3.17286e-06f, 2.67182e-06f, 3.11772e-06f, 2.48961e-06f, 2.56662e-06f, 2.69687e-06f, 2.66657e-06f, 2.49631e-06f, 2.07413e-06f,
        2.21763e-06f, 1.82216e-06f, 1.73999e-06f, 1.79846e-06f, 1.78097e-06f, 2.08078e-06f, 2.41026e-06f, 2.95564e-06f, 4.37817e-06f, 9.26286e-06f,
        1.71525e-05f, 1.63404e-05f, 7.76378e-06f, 4.20687e-06f, 4.36152e-06f, 4.1979e-06f,  3.60385e-06f, 4.21227e-06f, 6.61165e-06f, 1.85337e-05f,
        2.63714e-05f, 1.23596e-05f, 8.08582e-06f, 7.62016e-06f, 8.54114e-06f, 9.63216e-06f, 1.21937e-05f, 2.92224e-05f, 9.75796e-05f, 9.35745e-05f,
        3.33406e-05f, 2.37882e-05f, 2.9829e-05f,  4.42465e-05f, 6.68887e-05f, 0.000152608f, 0.000422651f, 0.000256325f, 6.52584e-05f, 4.13991e-05f,
        5.7842e-05f,  0.000264601f, 0.000711195f, 0.000441052f, 8.93762e-05f, 3.04976e-05f, 1.31372e-05f, 8.13006e-06f, 5.95634e-06f, 5.94402e-06f,
        6.95574e-06f, 1.12493e-05f, 1.93408e-05f, 3.30614e-05f, 5.526e-05f,   0.000106194f, 0.000246237f, 0.000245793f, 0.000116183f, 6.90781e-05f,
        0.000121558f, 0.00012478f,  0.000160506f, 0.000195856f, 0.000163724f, 0.000116846f, 9.27976e-05f, 7.97493e-05f, 7.30327e-05f, 6.2535e-05f,
        7.15964e-05f, 6.92402e-05f, 6.98667e-05f, 7.20625e-05f, 5.92742e-05f, 4.73751e-05f, 5.11686e-05f, 3.8765e-05f,  2.87346e-05f, 2.82287e-05f,
        4.23112e-05f, 2.84265e-05f, 2.76262e-05f, 3.13753e-05f, 3.20692e-05f, 2.54603e-05f, 1.55049e-05f, 1.67992e-05f, 1.51677e-05f, 1.72863e-05f,
        1.82755e-05f, 1.62912e-05f, 1.63329e-05f, 2.11384e-05f, 1.68083e-05f, 1.32225e-05f, 9.90909e-06f, 9.57385e-06f, 9.22475e-06f, 1.59785e-05f,
        1.89273e-05f, 1.94756e-05f, 1.68079e-05f, 1.52813e-05f, 1.45048e-05f, 1.12089e-05f, 9.50048e-06f, 7.40732e-06f, 6.16214e-06f, 4.66982e-06f,
        4.04122e-06f, 3.96966e-06f, 3.02326e-06f, 3.30965e-06f, 2.53001e-06f, 3.00426e-06f, 3.01337e-06f, 3.36385e-06f, 3.56402e-06f, 4.18688e-06f,
        4.12602e-06f, 5.01737e-06f, 5.44329e-06f, 5.985e-06f,   5.40637e-06f, 6.44899e-06f, 5.42357e-06f, 4.91412e-06f, 4.3504e-06f,  3.89253e-06f,
        3.67736e-06f, 4.08168e-06f, 3.85234e-06f, 3.99802e-06f, 4.60479e-06f, 5.29422e-06f, 4.87849e-06f, 4.55674e-06f, 4.24992e-06f, 3.52154e-06f,
        3.22953e-06f, 2.58855e-06f, 2.42857e-06f, 2.34923e-06f, 2.36014e-06f, 2.33549e-06f, 2.55772e-06f, 3.03473e-06f, 3.14355e-06f, 3.65574e-06f,
        3.70734e-06f, 3.68159e-06f, 3.81222e-06f, 3.35656e-06f, 3.062e-06f,   2.69374e-06f, 2.45185e-06f, 2.09096e-06f, 1.87615e-06f, 1.59947e-06f,
        1.51572e-06f, 1.47543e-06f, 1.5459e-06f,  1.6819e-06f,  1.89924e-06f, 2.46062e-06f, 2.89706e-06f, 3.43049e-06f, 4.07493e-06f, 4.31785e-06f,
        4.56185e-06f, 4.84605e-06f, 4.70059e-06f, 5.04519e-06f, 5.03717e-06f, 5.58133e-06f, 5.772e-06f,   6.2806e-06f,  6.83109e-06f, 7.80214e-06f,
        8.13898e-06f};

    const float b2[301] = {
        7.50196e-07f, 3.64689e-07f, 3.0422e-07f,  2.19926e-07f, 2.95025e-07f, 1.36813e-07f, 2.46454e-07f, 3.07665e-07f, 4.35207e-07f, 4.54783e-07f,
        4.09999e-07f, 4.6799e-07f,  4.30817e-07f, 3.21329e-07f, 5.14891e-07f, 5.88871e-07f, 7.24472e-07f, 5.19291e-07f, 5.83071e-07f, 7.385e-07f,
        2.80484e-06f, 2.40132e-06f, 1.65424e-06f, 1.01535e-06f, 2.56678e-06f, 6.15462e-06f, 1.34813e-05f, 2.75384e-05f, 4.11764e-05f, 5.15236e-05f,
        7.01286e-05f, 0.000133268f, 0.000337419f, 0.000957927f, 0.00227712f,  0.00543291f,  0.0197821f,   0.0818229f,   0.2452f,      0.503309f,
        0.70097f,     0.757605f,    0.778255f,    0.788626f,    0.8f,         0.801737f,    0.809859f,    0.82107f,     0.826045f,    0.830925f,
        0.842262f,    0.862706f,    0.878482f,    0.876943f,    0.878267f,    0.887613f,    0.907576f,    0.930559f,    0.950192f,    0.976801f,
        1.0f,         0.97894f,     0.855779f,    0.593873f,    0.297629f,    0.125661f,    0.0503684f,   0.018199f,    0.00573411f,  0.00297352f,
        0.00165595f,  0.000980273f, 0.000604892f, 0.000391497f, 0.000259234f, 0.000168111f, 0.00010894f,  7.63081e-05f, 5.14226e-05f, 5.2013e-05f,
        6.6689e-05f,  6.23362e-05f, 6.06577e-05f, 5.93819e-05f, 6.25513e-05f, 5.48608e-05f, 5.86835e-05f, 4.88126e-05f, 4.93644e-05f, 4.14711e-05f,
        3.10164e-05f, 2.97216e-05f, 1.74405e-05f, 1.74672e-05f, 1.43447e-05f, 1.09051e-05f, 1.27791e-05f, 9.19063e-06f, 8.62276e-06f, 9.16788e-06f,
        8.06522e-06f, 8.53885e-06f, 8.9662e-06f,  7.58053e-06f, 8.12959e-06f, 9.11945e-06f, 7.37798e-06f, 7.16219e-06f, 7.98185e-06f, 6.06857e-06f,
        5.53839e-06f, 6.14124e-06f, 4.19494e-06f, 3.49052e-06f, 4.14312e-06f, 3.83884e-06f, 2.7725e-06f,  3.75738e-06f, 3.84359e-06f, 3.23508e-06f,
        4.16085e-06f, 5.43268e-06f, 5.65941e-06f, 5.66555e-06f, 1.00131e-05f, 1.62239e-05f, 1.75445e-05f, 2.14947e-05f, 3.58956e-05f, 4.56586e-05f,
        3.43405e-05f, 2.38686e-05f, 2.4332e-05f,  2.54789e-05f, 1.95079e-05f, 1.33965e-05f, 1.25793e-05f, 1.40937e-05f, 1.1979e-05f,  8.63893e-06f,
        7.58208e-06f, 8.83774e-06f, 1.00948e-05f, 9.00984e-06f, 7.04681e-06f, 8.09797e-06f, 1.08886e-05f, 1.33532e-05f, 1.23902e-05f, 1.19507e-05f,
        1.48562e-05f, 2.23955e-05f, 3.10266e-05f, 3.26266e-05f, 3.10397e-05f, 3.39779e-05f, 4.5376e-05f,  6.10958e-05f, 6.5279e-05f,  5.23129e-05f,
        4.14838e-05f, 4.19558e-05f, 5.09964e-05f, 6.37575e-05f, 6.45187e-05f, 4.91907e-05f, 3.53937e-05f, 3.00032e-05f, 3.20417e-05f, 4.00312e-05f,
        4.4359e-05f,  3.93208e-05f, 2.98789e-05f, 2.47787e-05f, 2.45129e-05f, 2.9428e-05f,  4.04968e-05f, 5.068e-05f,   5.14977e-05f, 4.47032e-05f,
        5.94662e-05f, 5.43218e-05f, 6.09036e-05f, 7.73321e-05f, 8.48403e-05f, 9.27208e-05f, 9.49843e-05f, 8.04592e-05f, 5.70392e-05f, 5.08953e-05f,
        5.09718e-05f, 4.29127e-05f, 5.66121e-05f, 6.40647e-05f, 6.35005e-05f, 6.45065e-05f, 6.8561e-05f,  6.61624e-05f, 7.80311e-05f, 7.84671e-05f,
        7.47972e-05f, 5.56929e-05f, 4.48557e-05f, 3.82956e-05f, 3.24203e-05f, 2.64364e-05f, 2.01874e-05f, 2.18951e-05f, 2.14424e-05f, 2.35802e-05f,
        2.69757e-05f, 3.50803e-05f, 3.50877e-05f, 3.08309e-05f, 2.96703e-05f, 2.80215e-05f, 2.15038e-05f, 2.20744e-05f, 1.92407e-05f, 1.75484e-05f,
        1.8481e-05f,  1.73708e-05f, 1.80909e-05f, 1.87169e-05f, 1.78419e-05f, 1.86544e-05f, 1.83542e-05f, 1.71676e-05f, 1.57509e-05f, 1.42299e-05f,
        1.48274e-05f, 1.35095e-05f, 1.29937e-05f, 1.31147e-05f, 1.17814e-05f, 1.09442e-05f, 1.06259e-05f, 9.91455e-06f, 8.8098e-06f,  7.903e-06f,
        7.02151e-06f, 6.14267e-06f, 6.40226e-06f, 6.90937e-06f, 7.40346e-06f, 9.0376e-06f,  1.04649e-05f, 1.20358e-05f, 1.35923e-05f, 1.45542e-05f,
        1.52353e-05f, 1.4587e-05f,  1.27967e-05f, 1.1387e-05f,  9.40921e-06f, 7.79714e-06f, 6.31225e-06f, 5.63816e-06f, 5.29451e-06f, 5.25947e-06f,
        5.15346e-06f, 5.42692e-06f, 5.31811e-06f, 4.77555e-06f, 3.83623e-06f, 3.20429e-06f, 2.71831e-06f, 2.16288e-06f, 1.99789e-06f, 2.14329e-06f,
        2.24263e-06f, 2.42711e-06f, 2.89596e-06f, 2.86037e-06f, 2.81914e-06f, 2.48083e-06f, 2.0678e-06f,  1.92076e-06f, 1.53978e-06f, 1.43754e-06f,
        1.37064e-06f, 1.76234e-06f, 2.22508e-06f, 2.69147e-06f, 3.00682e-06f, 3.16738e-06f, 2.99304e-06f, 2.70142e-06f, 2.48436e-06f, 2.08026e-06f,
        1.84657e-06f, 2.06456e-06f, 2.72316e-06f, 3.75312e-06f, 4.7306e-06f,  5.55564e-06f, 6.32699e-06f, 5.91106e-06f, 5.11932e-06f, 4.25178e-06f,
        3.32253e-06f};

    const float b3[301] = {
        4.15708e-07f, 2.60143e-07f, 1.32494e-07f, 1.5403e-07f,  1.64746e-07f, 9.97031e-08f, 1.50249e-07f, 2.02269e-07f, 2.84962e-07f, 2.57269e-07f,
        2.49117e-07f, 3.36282e-07f, 3.75568e-07f, 2.16954e-07f, 4.25352e-07f, 4.61622e-07f, 4.19126e-07f, 3.82693e-07f, 3.69658e-07f, 5.02627e-07f,
        4.42599e-07f, 4.84876e-07f, 4.95357e-07f, 4.94213e-07f, 5.20767e-07f, 3.90137e-07f, 3.59035e-07f, 5.27747e-07f, 3.37674e-07f, 4.88789e-07f,
        3.74123e-07f, 3.69271e-07f, 3.20795e-07f, 4.14981e-07f, 4.61504e-07f, 2.95452e-07f, 1.74563e-07f, 3.27091e-07f, 5.33897e-07f, 3.57089e-07f,
        5.66046e-07f, 6.04714e-07f, 4.58234e-07f, 5.14316e-07f, 2.30656e-06f, 4.42627e-06f, 8.51169e-06f, 1.27751e-05f, 1.41306e-05f, 1.59342e-05f,
        1.97713e-05f, 2.12166e-05f, 2.25515e-05f, 2.66048e-05f, 3.56016e-05f, 4.35119e-05f, 6.64286e-05f, 0.000143846f, 0.000396291f, 0.00113574f,
        0.00309186f,  0.0111042f,   0.0405198f,   0.133788f,    0.35151f,     0.606039f,    0.723007f,    0.760026f,    0.779389f,    0.788811f,
        0.80685f,     0.828035f,    0.840756f,    0.842039f,    0.844305f,    0.846857f,    0.850335f,    0.859723f,    0.863516f,    0.869143f,
        0.882571f,    0.90072f,     0.924469f,    0.942168f,    0.962285f,    0.976601f,    0.985726f,    0.994764f,    1.0f,         0.993053f,
        0.979121f,    0.907745f,    0.66404f,     0.301436f,    0.0929241f,   0.0252385f,   0.00556269f,  0.0024767f,   0.0011294f,   0.00050847f,
        0.000222899f, 0.00010723f,  5.39405e-05f, 2.56897e-05f, 6.85006e-06f, 3.52589e-06f, 1.71169e-06f, 2.75668e-07f, 1.82902e-06f, 1.67961e-06f,
        6.19122e-06f, 8.59549e-06f, 1.52263e-05f, 1.60512e-05f, 2.56929e-05f, 3.25443e-05f, 2.91571e-05f, 2.70292e-05f, 2.41446e-05f, 1.40807e-05f,
        2.13973e-07f, 1.02058e-07f, 1.9749e-07f,  1.75362e-07f, 2.57315e-07f, 4.77614e-07f, 6.77741e-07f, 7.65801e-07f, 9.90214e-07f, 7.51558e-07f,
        6.2059e-07f,  9.81957e-07f, 1.09702e-06f, 6.25215e-07f, 9.27757e-07f, 1.11919e-06f, 1.17872e-06f, 1.14779e-06f, 8.93355e-07f, 8.36189e-07f,
        1.05053e-06f, 1.08276e-06f, 1.04181e-06f, 9.27774e-07f, 1.07702e-06f, 9.37779e-07f, 9.88439e-07f, 1.13665e-06f, 1.09476e-06f, 1.05959e-06f,
        1.04925e-06f, 1.29595e-06f, 9.34329e-07f, 1.13077e-06f, 8.30981e-07f, 8.0781e-07f,  1.02709e-06f, 1.16311e-06f, 8.47734e-07f, 9.97851e-07f,
        1.11274e-06f, 1.33262e-06f, 1.05514e-06f, 9.58995e-07f, 1.08595e-06f, 1.15264e-06f, 1.41038e-06f, 1.22922e-06f, 1.70163e-06f, 3.4679e-06f,
        3.95449e-06f, 2.72695e-06f, 1.36732e-06f, 9.90143e-07f, 7.54806e-07f, 6.55742e-07f, 2.85239e-07f, 6.86364e-07f, 7.71553e-07f, 1.12635e-06f,
        9.62279e-07f, 2.59255e-06f, 9.30699e-07f, 1.62071e-07f, 1.00889e-10f, 1.00081e-10f, 0.0f,         0.0f,         0.0f,         3.23471e-08f,
        3.49174e-08f, 6.19205e-07f, 5.116e-06f,   4.16041e-06f, 1.78258e-06f, 5.02328e-07f, 2.68142e-07f, 3.8522e-07f,  7.72031e-07f, 2.61664e-07f,
        1.37503e-07f, 2.31254e-06f, 8.6085e-06f,  4.70061e-06f, 1.69519e-06f, 1.83093e-07f, 1.14424e-07f, 1.73792e-06f, 1.25833e-06f, 1.66069e-06f,
        9.83103e-07f, 1.83029e-07f, 1.17095e-05f, 9.47984e-06f, 1.12401e-05f, 9.14076e-06f, 9.03094e-06f, 8.49106e-06f, 7.68749e-06f, 6.74929e-06f,
        7.6733e-06f,  6.95369e-06f, 6.01684e-06f, 5.47181e-06f, 6.12431e-06f, 6.94762e-06f, 6.21639e-06f, 6.62084e-06f, 6.59998e-06f, 5.76706e-06f,
        6.04266e-06f, 5.40722e-06f, 5.40383e-06f, 4.91854e-06f, 4.02118e-06f, 3.62242e-06f, 3.52044e-06f, 3.49028e-06f, 2.9236e-06f,  2.56893e-06f,
        2.28989e-06f, 2.09968e-06f, 2.91966e-06f, 2.44435e-06f, 2.21268e-06f, 2.22573e-06f, 2.02219e-06f, 1.75423e-06f, 1.83833e-06f, 1.72076e-06f,
        1.51802e-06f, 1.53759e-06f, 1.24302e-06f, 1.44829e-06f, 1.35566e-06f, 1.31717e-06f, 1.18067e-06f, 1.12871e-06f, 1.2687e-06f,  1.33911e-06f,
        1.09356e-06f, 1.31515e-06f, 1.44675e-06f, 1.55647e-06f, 1.63886e-06f, 1.97263e-06f, 1.86758e-06f, 1.76664e-06f, 1.90731e-06f, 1.8888e-06f,
        1.74551e-06f, 1.59134e-06f, 1.71152e-06f, 1.35904e-06f, 1.32272e-06f, 1.27499e-06f, 1.1585e-06f,  1.26847e-06f, 1.33096e-06f, 1.48891e-06f,
        1.66013e-06f, 1.92885e-06f, 2.33777e-06f, 2.98726e-06f, 3.83011e-06f, 5.24983e-06f, 7.20736e-06f, 9.73918e-06f, 1.32221e-05f, 1.70002e-05f,
        2.10955e-05f, 2.549e-05f,   2.99685e-05f, 3.51168e-05f, 4.07206e-05f, 4.74368e-05f, 5.50521e-05f, 6.53211e-05f, 8.10989e-05f, 0.000105497f,
        0.000140101f};

    const float b4[301] = {
        3.12145e-07f, 8.28534e-08f, 9.83184e-08f, 8.55079e-08f, 4.71887e-08f, 3.85598e-08f, 6.13263e-08f, 1.09691e-07f, 8.19229e-08f, 8.18698e-08f,
        8.44374e-08f, 8.18784e-08f, 7.47651e-08f, 6.01096e-08f, 7.12291e-08f, 2.8315e-07f,  6.00765e-07f, 2.10864e-05f, 2.73104e-05f, 4.50671e-07f,
        1.06402e-08f, 6.94993e-08f, 3.13328e-08f, 1.09588e-07f, 1.74842e-07f, 1.9277e-07f,  1.34675e-07f, 1.92847e-07f, 2.02477e-07f, 2.6112e-07f,
        3.33295e-07f, 2.31552e-07f, 2.58338e-07f, 3.82314e-07f, 4.29747e-07f, 4.01449e-07f, 1.02894e-06f, 5.00318e-06f, 3.23438e-06f, 1.18679e-06f,
        5.38393e-07f, 3.44446e-07f, 3.9298e-07f,  3.47571e-07f, 7.25503e-07f, 6.40036e-07f, 5.09299e-07f, 4.9699e-07f,  4.32861e-07f, 6.78173e-07f,
        7.05903e-07f, 6.95095e-07f, 7.07479e-07f, 6.7639e-07f,  6.97946e-07f, 6.43371e-07f, 7.30317e-07f, 5.63357e-07f, 3.36905e-07f, 4.48422e-07f,
        1.74648e-07f, 3.13465e-07f, 2.23411e-07f, 2.26471e-07f, 1.67155e-07f, 1.6068e-07f,  1.35935e-07f, 2.80495e-07f, 1.38627e-07f, 2.43633e-07f,
        3.57075e-07f, 3.20907e-07f, 5.0935e-07f,  2.31732e-07f, 2.02004e-07f, 1.99908e-07f, 1.24175e-07f, 3.61372e-07f, 3.62811e-07f, 3.75752e-06f,
        7.00146e-06f, 6.23552e-06f, 4.3711e-06f,  4.90926e-06f, 3.82553e-06f, 1.1127e-05f,  2.66906e-05f, 5.75092e-05f, 0.00012712f,  0.000344491f,
        0.000895024f, 0.0022247f,   0.0058789f,   0.0265572f,   0.134652f,    0.38922f,     0.66355f,     0.848198f,    0.899806f,    0.920023f,
        0.932634f,    0.940181f,    0.947026f,    0.952441f,    0.966412f,    0.981469f,    0.989723f,    0.992706f,    1.0f,         0.969368f,
        0.759433f,    0.436651f,    0.134322f,    0.0308525f,   0.00738524f,  0.00223674f,  0.0006906f,   0.000285122f, 0.000157594f, 9.63247e-05f,
        4.43162e-05f, 2.30954e-05f, 1.28847e-05f, 7.45056e-06f, 4.97739e-06f, 3.29911e-06f, 2.15753e-06f, 1.42561e-06f, 8.30459e-07f, 9.51382e-07f,
        8.18661e-07f, 7.03925e-07f, 9.15122e-07f, 5.41677e-07f, 8.31395e-07f, 8.59648e-07f, 6.23981e-07f, 8.59012e-07f, 5.17849e-07f, 6.53052e-07f,
        9.60845e-07f, 5.49969e-07f, 8.65149e-07f, 8.47562e-07f, 7.28829e-07f, 1.14307e-06f, 9.13936e-07f, 8.44814e-07f, 1.18804e-06f, 1.11252e-06f,
        1.50364e-06f, 1.29736e-06f, 1.19152e-06f, 1.05178e-06f, 1.28277e-06f, 1.31103e-06f, 1.03605e-06f, 9.37231e-07f, 1.12941e-06f, 1.07726e-06f,
        1.01085e-06f, 8.19055e-07f, 8.60927e-07f, 8.22335e-07f, 7.66233e-07f, 8.69038e-07f, 7.82916e-07f, 7.56701e-07f, 6.00481e-07f, 4.7558e-07f,
        5.374e-07f,   6.07694e-07f, 6.55254e-07f, 2.03489e-07f, 3.94378e-07f, 5.87726e-07f, 3.09931e-07f, 4.14639e-07f, 5.61119e-07f, 6.43642e-07f,
        2.33671e-05f, 1.59193e-05f, 1.89268e-05f, 2.13128e-05f, 1.12554e-05f, 1.39639e-05f, 1.88608e-05f, 1.99851e-05f, 1.94386e-05f, 9.88685e-06f,
        1.82516e-05f, 1.15331e-05f, 9.26691e-06f, 1.352e-05f,   1.06333e-05f, 1.13419e-05f, 1.19485e-05f, 7.6223e-06f,  1.19821e-05f, 1.2663e-05f,
        1.70876e-05f, 1.1388e-05f,  6.7115e-06f,  8.61733e-06f, 8.92584e-06f, 6.81989e-06f, 4.43969e-06f, 6.43896e-06f, 4.89677e-06f, 5.81233e-06f,
        6.00927e-06f, 4.89342e-06f, 1.92265e-06f, 3.92241e-06f, 3.60834e-06f, 2.9064e-06f,  3.27708e-06f, 2.99611e-06f, 3.11394e-06f, 4.66012e-06f,
        3.91458e-06f, 4.29283e-06f, 3.86838e-06f, 3.83149e-06f, 3.27398e-06f, 2.83986e-06f, 2.47138e-06f, 2.0826e-06f,  2.02905e-06f, 1.68586e-06f,
        1.78242e-06f, 1.83929e-06f, 1.04638e-06f, 1.38024e-06f, 1.31731e-06f, 1.31521e-06f, 6.52398e-07f, 7.78377e-07f, 9.25714e-07f, 8.67073e-07f,
        8.22355e-07f, 7.92099e-07f, 8.62113e-07f, 8.90454e-07f, 6.33338e-07f, 9.92124e-07f, 8.63085e-07f, 7.34335e-07f, 9.03921e-07f, 7.04451e-07f,
        7.04449e-07f, 7.89631e-07f, 8.61175e-07f, 8.20682e-07f, 1.00168e-06f, 1.43427e-06f, 1.54572e-06f, 1.60886e-06f, 1.78755e-06f, 1.77994e-06f,
        1.53679e-06f, 1.41e-06f,    1.19628e-06f, 9.79616e-07f, 5.21729e-07f, 4.42824e-07f, 4.64902e-07f, 4.29654e-07f, 4.08101e-07f, 4.66717e-07f,
        4.10742e-07f, 3.40666e-07f, 3.24714e-07f, 4.197e-07f,   3.64468e-07f, 3.65553e-07f, 3.44671e-07f, 3.67198e-07f, 3.12263e-07f, 3.30082e-07f,
        1.99664e-07f, 1.72882e-07f, 1.60492e-07f, 1.2065e-07f,  7.23715e-08f, 1.43254e-07f, 1.12445e-07f, 8.94112e-08f, 9.22383e-08f, 8.79915e-08f,
        1.07076e-07f, 1.084e-07f,   1.27328e-07f, 1.69284e-07f, 2.04354e-07f, 2.73764e-07f, 3.37554e-07f, 4.28357e-07f, 4.83865e-07f, 5.33671e-07f,
        5.77712e-07f};

    const float b5[301] = {
        3.12286e-07f, 1.21265e-07f, 6.94181e-08f, 3.9853e-08f,  8.0826e-08f,  4.04409e-08f, 9.26526e-08f, 1.12003e-07f, 2.00442e-07f, 1.25411e-07f,
        1.38037e-07f, 1.68875e-07f, 1.62312e-07f, 9.81447e-08f, 2.36616e-07f, 2.99972e-07f, 3.57437e-07f, 1.73237e-07f, 2.51165e-07f, 3.80091e-07f,
        2.95126e-07f, 3.00839e-07f, 2.8738e-07f,  3.69689e-07f, 4.13567e-07f, 3.59864e-07f, 6.22912e-07f, 5.73343e-07f, 2.85253e-07f, 3.05785e-07f,
        2.84932e-07f, 2.7333e-07f,  3.14806e-07f, 3.26106e-07f, 1.62258e-07f, 1.93473e-07f, 2.7907e-07f,  1.95591e-07f, 3.78068e-07f, 2.19725e-07f,
        3.13698e-07f, 2.73155e-07f, 3.05173e-07f, 2.00063e-07f, 1.86757e-06f, 3.44125e-06f, 7.95772e-07f, 6.25956e-07f, 1.17606e-06f, 2.40044e-06f,
        9.54706e-07f, 9.25815e-07f, 5.61706e-07f, 1.3451e-06f,  1.28024e-06f, 1.36027e-06f, 2.94643e-06f, 1.86431e-06f, 2.88379e-06f, 2.16267e-06f,
        1.53977e-06f, 2.27062e-06f, 8.29005e-07f, 2.30761e-07f, 3.01119e-07f, 2.36372e-07f, 2.45071e-07f, 2.32871e-07f, 1.90817e-07f, 2.06172e-07f,
        1.14463e-07f, 3.52299e-08f, 2.27043e-07f, 3.45896e-08f, 2.42045e-08f, 1.21876e-08f, 6.62187e-08f, 6.09625e-08f, 1.41816e-07f, 8.79884e-08f,
        1.19441e-07f, 3.73189e-08f, 7.59332e-08f, 2.40439e-07f, 1.17802e-07f, 6.55622e-08f, 8.63856e-08f, 6.44464e-08f, 2.23776e-07f, 1.03047e-07f,
        1.32551e-07f, 2.79429e-07f, 2.07195e-07f, 3.67864e-07f, 5.11545e-07f, 5.89037e-07f, 8.2924e-07f,  8.56781e-07f, 1.3186e-06f,  9.10331e-06f,
        1.26163e-05f, 2.68254e-05f, 4.09503e-05f, 7.03781e-05f, 0.000122912f, 0.000268666f, 0.000628092f, 0.00164922f,  0.00486736f,  0.0227608f,
        0.0955659f,   0.294353f,    0.596071f,    0.822289f,    0.872776f,    0.885335f,    0.927928f,    0.955695f,    0.965042f,    0.967095f,
        0.965588f,    0.957776f,    0.9555f,      0.952831f,    0.955801f,    0.965253f,    0.970358f,    0.976493f,    0.987526f,    0.996575f,
        0.999085f,    0.992107f,    0.983694f,    0.978311f,    0.88528f,     0.632595f,    0.291823f,    0.0832547f,   0.0204412f,   0.00563548f,
        0.00191373f,  0.000843246f, 0.000403589f, 0.000203731f, 0.000101781f, 5.6897e-05f,  2.7195e-05f,  1.61825e-05f, 1.4231e-05f,  1.68343e-05f,
        1.3334e-05f,  1.44383e-05f, 1.32012e-05f, 1.24899e-05f, 1.05706e-05f, 6.08328e-06f, 3.05093e-06f, 3.42552e-06f, 1.54743e-06f, 1.36525e-06f,
        1.23105e-06f, 4.3642e-07f,  5.81041e-07f, 1.18259e-06f, 3.00501e-06f, 4.97549e-06f, 9.51182e-06f, 7.32646e-06f, 6.71037e-06f, 8.93449e-06f,
        6.85023e-06f, 4.8556e-06f,  5.46221e-06f, 2.19395e-06f, 1.5006e-06f,  4.9089e-07f,  2.22462e-06f, 6.60787e-07f, 6.09816e-07f, 9.03731e-07f,
        1.37929e-05f, 1.40411e-05f, 1.3406e-05f,  1.63301e-05f, 1.06067e-05f, 1.24196e-05f, 1.08121e-05f, 7.69758e-06f, 8.8836e-06f,  9.03241e-06f,
        7.87554e-06f, 4.2784e-06f,  1.07825e-05f, 4.51337e-06f, 6.97831e-06f, 6.53672e-06f, 6.20341e-06f, 6.41495e-06f, 8.42444e-06f, 6.85528e-06f,
        2.70354e-06f, 6.50522e-06f, 6.34091e-06f, 6.54655e-06f, 5.05613e-06f, 7.09478e-06f, 9.15616e-06f, 1.08124e-05f, 6.65697e-06f, 7.89558e-06f,
        4.14291e-06f, 4.32081e-06f, 8.59893e-06f, 6.22021e-06f, 7.2363e-06f,  6.14019e-06f, 3.87893e-06f, 5.4737e-06f,  4.80289e-06f, 5.01621e-06f,
        5.71604e-06f, 5.0985e-06f,  4.24175e-06f, 4.44819e-06f, 3.97763e-06f, 3.48069e-06f, 3.4912e-06f,  4.00016e-06f, 2.68235e-06f, 3.0765e-06f,
        3.20229e-06f, 2.41872e-06f, 3.06521e-06f, 2.72094e-06f, 2.13903e-06f, 1.70534e-06f, 1.72736e-06f, 1.5771e-06f,  1.31904e-06f, 1.10782e-06f,
        1.20611e-06f, 9.60313e-07f, 1.25562e-06f, 9.96851e-07f, 1.08323e-06f, 9.27816e-07f, 7.70516e-07f, 6.93097e-07f, 7.47027e-07f, 7.07703e-07f,
        8.10806e-07f, 7.26741e-07f, 6.80176e-07f, 6.20037e-07f, 6.39881e-07f, 5.95663e-07f, 4.7329e-07f,  5.43599e-07f, 5.47303e-07f, 6.05122e-07f,
        5.1772e-07f,  5.88033e-07f, 5.45848e-07f, 5.08645e-07f, 2.36035e-07f, 5.84744e-07f, 5.20728e-07f, 4.18312e-07f, 3.71858e-07f, 4.39561e-07f,
        3.37007e-07f, 2.56984e-07f, 3.79198e-07f, 4.64178e-07f, 5.27954e-07f, 6.02129e-07f, 6.16109e-07f, 8.1714e-07f,  7.41506e-07f, 6.23132e-07f,
        4.93815e-07f, 3.36936e-07f, 2.6114e-07f,  1.50937e-07f, 1.37661e-07f, 1.5674e-07f,  1.21087e-07f, 1.3344e-07f,  1.58735e-07f, 1.86711e-07f,
        1.80942e-07f, 2.25817e-07f, 2.17427e-07f, 2.35791e-07f, 2.14853e-07f, 1.85285e-07f, 1.48392e-07f, 1.1159e-07f,  8.25772e-08f, 7.09923e-08f,
        5.57264e-08f};

    const float b6[301] = {
        1.31873e-07f, 1.87926e-07f, 2.81592e-07f, 2.17374e-07f, 1.70877e-07f, 1.0562e-07f,  1.12979e-07f, 2.00636e-07f, 1.89536e-07f, 2.42299e-07f,
        1.75878e-07f, 1.93174e-07f, 2.64698e-07f, 2.57301e-07f, 3.40167e-07f, 3.55343e-07f, 7.60476e-07f, 2.25433e-06f, 1.29749e-06f, 1.29558e-06f,
        2.01722e-06f, 1.13819e-06f, 1.64078e-06f, 9.96598e-07f, 9.78403e-07f, 1.06118e-06f, 8.77215e-07f, 1.97241e-06f, 1.14645e-06f, 8.83069e-07f,
        7.40957e-07f, 1.02664e-06f, 9.67904e-06f, 5.091e-05f,   2.50784e-06f, 3.48606e-07f, 8.32043e-07f, 2.39095e-05f, 2.58811e-06f, 2.33621e-07f,
        1.98094e-07f, 2.85714e-07f, 1.27594e-07f, 1.7327e-07f,  1.82199e-07f, 1.5321e-07f,  1.63299e-07f, 1.01052e-07f, 1.49075e-07f, 5.47801e-07f,
        7.84427e-07f, 1.65041e-06f, 8.51018e-07f, 5.77883e-06f, 1.38337e-05f, 7.9076e-06f,  3.87846e-06f, 5.69445e-06f, 1.53368e-05f, 9.94259e-06f,
        1.2e-06f,     1.54553e-06f, 4.22091e-06f, 1.43071e-06f, 2.00613e-07f, 1.81045e-07f, 1.2783e-07f,  1.03067e-07f, 1.82997e-07f, 1.10773e-07f,
        1.1226e-07f,  1.16695e-07f, 2.16241e-07f, 1.05663e-07f, 1.614e-07f,   1.6817e-07f,  1.16441e-07f, 1.15124e-07f, 6.99069e-08f, 1.53185e-08f,
        2.72037e-08f, 1.23476e-07f, 1.55133e-07f, 1.49846e-07f, 3.58885e-07f, 2.02413e-06f, 2.0841e-06f,  8.13696e-07f, 9.02676e-07f, 2.00266e-06f,
        3.1147e-06f,  3.09139e-06f, 4.4865e-06f,  6.70102e-06f, 6.05048e-06f, 5.02704e-06f, 7.09114e-06f, 1.13053e-05f, 1.37502e-05f, 1.60627e-05f,
        2.40093e-05f, 3.44167e-05f, 3.74905e-05f, 3.98343e-05f, 5.38148e-05f, 7.94317e-05f, 8.83804e-05f, 7.59032e-05f, 9.03911e-05f, 0.000174361f,
        0.000184766f, 5.17257e-05f, 8.78977e-06f, 4.47753e-06f, 2.69144e-06f, 1.45718e-06f, 1.07089e-06f, 6.55321e-07f, 1.38442e-06f, 2.79808e-06f,
        5.03238e-06f, 6.41933e-06f, 8.09994e-06f, 1.05514e-05f, 1.15504e-05f, 1.14512e-05f, 1.17762e-05f, 1.04862e-05f, 1.12589e-05f, 8.43843e-06f,
        8.21063e-06f, 8.85197e-06f, 9.70519e-06f, 1.53186e-05f, 3.28e-05f,    7.87273e-05f, 0.000207342f, 0.000759089f, 0.00312753f,  0.0188368f,
        0.0996101f,   0.346835f,    0.622908f,    0.853785f,    0.952792f,    0.978422f,    0.987634f,    0.989026f,    0.993901f,    0.999858f,
        0.99928f,     0.989417f,    0.986813f,    0.980962f,    0.977116f,    0.944191f,    0.846399f,    0.615837f,    0.359763f,    0.138813f,
        0.0305442f,   0.00709174f,  0.00215848f,  0.000729986f, 0.000269875f, 0.000118803f, 5.53309e-05f, 2.39914e-05f, 9.56683e-06f, 6.93469e-06f,
        5.29459e-06f, 3.2974e-06f,  4.31265e-06f, 4.03836e-06f, 5.07602e-06f, 7.67396e-06f, 8.34915e-06f, 9.4325e-06f,  6.44795e-06f, 5.01789e-06f,
        6.71178e-08f, 3.03948e-07f, 3.1684e-07f,  2.65135e-07f, 2.09765e-07f, 3.00101e-07f, 1.67201e-07f, 1.60492e-07f, 2.13433e-07f, 1.29106e-07f,
        9.71011e-08f, 2.9819e-07f,  1.86975e-07f, 2.30772e-07f, 1.64834e-07f, 6.97091e-08f, 3.40321e-07f, 2.85903e-07f, 2.06352e-07f, 3.65446e-07f,
        4.38643e-07f, 1.65414e-07f, 8.01729e-07f, 7.59196e-07f, 1.87131e-06f, 3.94486e-06f, 8.7793e-06f,  1.12049e-05f, 1.14224e-05f, 7.65019e-06f,
        4.71997e-06f, 4.25463e-06f, 1.04158e-05f, 8.44869e-06f, 7.60545e-06f, 7.43366e-06f, 8.91167e-06f, 1.17538e-05f, 1.53146e-05f, 1.90496e-05f,
        1.73994e-05f, 1.71082e-05f, 1.83056e-05f, 1.86976e-05f, 1.80158e-05f, 1.89996e-05f, 1.5662e-05f,  1.44995e-05f, 1.26694e-05f, 1.02432e-05f,
        8.13754e-06f, 6.21556e-06f, 4.54308e-06f, 3.91257e-06f, 3.1588e-06f,  2.52225e-06f, 2.01583e-06f, 1.62728e-06f, 1.47903e-06f, 1.09533e-06f,
        1.03503e-06f, 9.97997e-07f, 7.70181e-07f, 7.25907e-07f, 8.38991e-07f, 5.98253e-07f, 7.1384e-07f,  4.89317e-07f, 6.07583e-07f, 5.97359e-07f,
        7.18152e-07f, 5.91987e-07f, 6.80961e-07f, 3.87465e-07f, 4.28846e-07f, 6.08958e-07f, 3.8379e-07f,  5.16479e-07f, 5.35559e-07f, 4.5999e-07f,
        4.5037e-07f,  4.34943e-07f, 3.90623e-07f, 3.55287e-07f, 3.677e-07f,   4.21178e-07f, 3.62045e-07f, 3.55015e-07f, 2.54848e-07f, 2.60974e-07f,
        2.42997e-07f, 1.96909e-07f, 2.38909e-07f, 1.73543e-07f, 2.20613e-07f, 1.73692e-07f, 1.66732e-07f, 1.40222e-07f, 1.22858e-07f, 1.39263e-07f,
        1.49344e-07f, 1.63372e-07f, 1.54912e-07f, 1.45368e-07f, 1.58958e-07f, 1.93923e-07f, 1.65487e-07f, 1.60786e-07f, 1.52013e-07f, 1.35784e-07f,
        9.24469e-08f, 9.92189e-08f, 7.60063e-08f, 9.85175e-08f, 7.22548e-08f, 4.57741e-08f, 6.97201e-08f, 8.69437e-08f, 1.13252e-07f, 2.58077e-07f,
        4.68462e-07f};

    const float b7[301] = {
        3.42494e-07f, 5.7563e-09f,  2.4306e-08f,  6.23161e-08f, 8.60169e-08f, 6.54447e-09f, 4.9819e-08f,  9.30485e-08f, 5.51852e-08f, 1.75292e-08f,
        2.87474e-08f, 2.73409e-08f, 2.83316e-08f, 3.82621e-09f, 3.07146e-08f, 4.55383e-08f, 1.04778e-07f, 1.23884e-09f, 8.25112e-09f, 5.0621e-08f,
        1.79225e-07f, 2.74552e-08f, 1.04373e-07f, 9.7114e-08f,  9.62771e-08f, 1.75398e-07f, 8.63102e-08f, 6.84208e-08f, 9.40778e-08f, 5.15181e-08f,
        1.02132e-07f, 1.42588e-07f, 8.08711e-08f, 2.16959e-07f, 1.03608e-07f, 9.2287e-08f,  1.59861e-06f, 6.11993e-06f, 3.68931e-07f, 9.24078e-08f,
        3.81349e-07f, 2.64627e-07f, 3.27124e-07f, 2.55578e-07f, 2.87094e-07f, 3.84e-07f,    5.21921e-07f, 6.14658e-07f, 3.75539e-07f, 3.80046e-07f,
        7.36489e-07f, 1.37786e-06f, 6.07935e-07f, 9.45777e-07f, 1.59554e-06f, 5.68797e-07f, 4.72783e-07f, 5.63447e-07f, 3.35752e-07f, 1.70326e-07f,
        3.17931e-07f, 1.41325e-07f, 2.49863e-07f, 3.96423e-07f, 2.16004e-07f, 3.37582e-07f, 2.90124e-07f, 3.80183e-07f, 3.11457e-07f, 3.98668e-07f,
        1.79901e-07f, 1.22001e-07f, 1.13002e-07f, 2.85022e-08f, 4.67327e-08f, 1.90183e-08f, 0.0f,         2.99103e-08f, 1.06093e-07f, 1.18422e-07f,
        1.07083e-08f, 7.91334e-08f, 9.95149e-08f, 1.54428e-07f, 9.12854e-08f, 2.15373e-08f, 7.48973e-08f, 7.40942e-08f, 0.0f,         3.73909e-08f,
        7.49644e-08f, 0.0f,         0.0f,         1.92802e-08f, 2.95975e-08f, 7.04334e-08f, 6.50404e-08f, 0.0f,         0.0f,         0.0f,
        2.0755e-07f,  1.04836e-10f, 7.80821e-09f, 1.57983e-07f, 1.0595e-10f,  1.13403e-07f, 3.312e-10f,   1.14176e-07f, 1.03109e-10f, 3.76115e-09f,
        7.45634e-09f, 1.19272e-07f, 1.07481e-10f, 1.10155e-10f, 1.1276e-10f,  1.40161e-07f, 1.16185e-10f, 7.139e-08f,   9.90442e-08f, 3.74204e-07f,
        5.05087e-07f, 4.6352e-07f,  5.97207e-07f, 5.4384e-07f,  6.81312e-07f, 6.90591e-07f, 1.00825e-06f, 8.18553e-07f, 1.07975e-06f, 8.27673e-07f,
        9.35167e-07f, 9.64121e-07f, 1.00596e-06f, 8.68247e-07f, 8.97409e-07f, 1.19586e-06f, 1.28982e-06f, 9.86575e-07f, 9.25037e-07f, 2.05008e-06f,
        1.71977e-06f, 3.51855e-06f, 4.80754e-06f, 6.35905e-06f, 5.22507e-06f, 4.29703e-06f, 3.92372e-06f, 4.42103e-06f, 4.97574e-06f, 5.96695e-06f,
        6.60085e-06f, 1.11374e-05f, 1.62372e-05f, 2.84438e-05f, 4.4002e-05f,  7.0506e-05f,  0.000106141f, 0.000165311f, 0.000256229f, 0.000412257f,
        0.000713036f, 0.00139941f,  0.00288571f,  0.00595325f,  0.0126868f,   0.0276938f,   0.0588599f,   0.123117f,    0.260204f,    0.506774f,
        0.759967f,    0.94058f,     0.995805f,    0.997622f,    0.988907f,    0.983091f,    0.972934f,    0.958312f,    0.941139f,    0.931907f,
        0.919382f,    0.90908f,     0.8976f,      0.88212f,     0.871191f,    0.863054f,    0.844284f,    0.836496f,    0.838036f,    0.841189f,
        0.835657f,    0.833819f,    0.842548f,    0.830777f,    0.818604f,    0.816837f,    0.806711f,    0.81076f,     0.794161f,    0.779884f,
        0.767313f,    0.749602f,    0.739271f,    0.738714f,    0.734145f,    0.711833f,    0.697141f,    0.680462f,    0.658635f,    0.649781f,
        0.632471f,    0.619908f,    0.60912f,     0.598992f,    0.590914f,    0.566107f,    0.507112f,    0.402799f,    0.275174f,    0.157667f,
        0.0738047f,   0.032787f,    0.0138147f,   0.00539084f,  0.00262463f,  0.00180863f,  0.00130633f,  0.00103281f,  0.000917177f, 0.000891162f,
        0.000798344f, 0.000745739f, 0.000727178f, 0.000783273f, 0.000701058f, 0.000655311f, 0.00060278f,  0.000557875f, 0.000281186f, 0.000274231f,
        4.37496e-06f, 3.22571e-06f, 2.88909e-06f, 2.24195e-06f, 1.98867e-06f, 1.47066e-06f, 1.12615e-06f, 1.04739e-06f, 1.1344e-06f,  8.90363e-07f,
        8.71592e-07f, 7.40614e-07f, 7.19518e-07f, 6.91579e-07f, 7.01778e-07f, 6.72029e-07f, 4.36737e-07f, 6.31103e-07f, 6.64941e-07f, 6.02511e-07f,
        3.22655e-07f, 5.32912e-07f, 4.71662e-07f, 4.99519e-07f, 3.35434e-07f, 4.46158e-07f, 4.48065e-07f, 3.19015e-07f, 2.39899e-07f, 4.55294e-07f,
        2.89741e-07f, 2.5021e-07f,  2.57359e-07f, 3.5683e-07f,  2.96983e-07f, 2.97873e-07f, 1.88219e-07f, 2.40558e-07f, 2.15601e-07f, 2.13278e-07f,
        1.40431e-07f, 1.60961e-07f, 1.70122e-07f, 1.10615e-07f, 1.0436e-07f,  1.222e-07f,   1.25283e-07f, 9.88137e-08f, 8.59799e-08f, 9.19016e-08f,
        7.99614e-08f, 8.73603e-08f, 7.68151e-08f, 6.51417e-08f, 7.38325e-08f, 6.48494e-08f, 5.64187e-08f, 6.5542e-08f,  5.90555e-08f, 4.77664e-08f,
        5.59409e-08f};

    const float b8[301] = {
        2.75157e-07f, 6.96359e-06f, 1.9874e-05f,  8.73924e-05f, 7.4835e-05f,  5.16098e-06f, 4.81496e-07f, 6.24369e-07f, 2.89158e-07f, 1.33848e-07f,
        1.61046e-07f, 2.69148e-07f, 1.41051e-07f, 1.93786e-07f, 2.0957e-07f,  2.34628e-07f, 2.36518e-07f, 2.47669e-07f, 2.25346e-07f, 2.01273e-07f,
        2.14493e-07f, 1.42204e-07f, 1.7784e-07f,  2.29739e-07f, 4.0488e-07f,  2.15782e-07f, 1.58074e-07f, 1.50063e-07f, 7.18437e-08f, 1.88801e-07f,
        2.3008e-07f,  1.64135e-07f, 1.27638e-07f, 1.9751e-07f,  1.13678e-07f, 7.76496e-07f, 1.1877e-06f,  1.47692e-07f, 2.12794e-07f, 1.89394e-06f,
        1.22e-06f,    8.28353e-07f, 5.66846e-07f, 1.55612e-07f, 3.89524e-08f, 6.08863e-08f, 4.13505e-07f, 1.57781e-07f, 8.45778e-09f, 9.45575e-07f,
        3.68309e-06f, 1.1502e-06f,  2.14722e-07f, 1.01022e-07f, 6.8025e-07f,  8.6337e-07f,  3.19639e-08f, 2.56285e-07f, 1.35378e-07f, 3.34694e-08f,
        7.36246e-07f, 8.36204e-07f, 2.65609e-08f, 8.10432e-08f, 2.05826e-07f, 3.69154e-07f, 8.02175e-07f, 8.89483e-07f, 4.4826e-07f,  2.42823e-07f,
        2.17011e-07f, 2.65704e-06f, 8.78188e-06f, 7.05779e-07f, 3.82597e-07f, 4.04534e-06f, 4.67549e-06f, 9.19411e-08f, 2.98568e-07f, 2.08203e-06f,
        1.03067e-06f, 9.72367e-08f, 1.50024e-07f, 1.2413e-07f,  1.4876e-07f,  2.35395e-08f, 1.65976e-10f, 1.65631e-10f, 8.45577e-09f, 1.59829e-10f,
        1.56488e-10f, 1.35137e-07f, 1.26482e-06f, 9.54683e-08f, 2.93604e-07f, 1.59714e-07f, 2.23112e-07f, 5.34018e-07f, 6.1337e-07f,  1.72864e-07f,
        5.77747e-07f, 4.80195e-07f, 5.14193e-07f, 5.97282e-07f, 1.16949e-06f, 5.3439e-07f,  2.8602e-07f,  2.20078e-07f, 5.34755e-07f, 5.51942e-07f,
        6.32398e-07f, 1.13151e-06f, 1.09476e-06f, 3.24873e-07f, 3.6263e-07f,  5.49154e-07f, 8.32561e-08f, 6.24779e-07f, 6.90714e-07f, 8.366e-07f,
        1.2346e-06f,  1.93899e-06f, 2.35572e-06f, 9.86481e-07f, 5.4618e-07f,  1.53631e-06f, 4.98064e-07f, 4.68967e-08f, 2.82103e-07f, 1.79437e-07f,
        2.82801e-07f, 5.47962e-07f, 7.69025e-07f, 1.62521e-07f, 2.32749e-07f, 5.04263e-07f, 8.32099e-07f, 2.27261e-07f, 4.08301e-07f, 3.43738e-07f,
        1.2715e-07f,  2.36481e-07f, 8.74008e-08f, 3.913e-07f,   4.88832e-07f, 6.9057e-07f,  7.13913e-07f, 8.95172e-07f, 8.96901e-07f, 5.53029e-07f,
        1.04012e-06f, 9.12699e-07f, 7.19448e-07f, 7.20192e-07f, 8.80887e-07f, 1.14393e-06f, 5.57443e-07f, 9.4312e-07f,  1.04838e-06f, 9.98724e-07f,
        1.23283e-06f, 8.72156e-07f, 8.03599e-07f, 5.35655e-07f, 8.46701e-07f, 6.91104e-07f, 8.55943e-07f, 6.31436e-07f, 4.51508e-07f, 1.11291e-06f,
        1.03136e-06f, 4.73296e-07f, 6.08156e-07f, 8.62452e-07f, 9.2854e-07f,  7.79087e-07f, 1.22534e-06f, 0.000114086f, 0.000453417f, 0.000504746f,
        0.000779556f, 0.0010461f,   0.000976304f, 0.00111328f,  0.0010687f,   0.00109088f,  0.00109552f,  0.00113106f,  0.00109447f,  0.00108912f,
        0.00104157f,  0.0011403f,   0.001127f,    0.00117057f,  0.00115094f,  0.00120607f,  0.00135466f,  0.00130018f,  0.00149181f,  0.002231f,
        0.00432839f,  0.0105836f,   0.0305957f,   0.107853f,    0.313746f,    0.571884f,    0.789269f,    0.951541f,    0.987144f,    0.991677f,
        0.993363f,    0.999691f,    0.996691f,    0.983327f,    0.970856f,    0.95236f,     0.93302f,     0.922376f,    0.910922f,    0.899767f,
        0.892112f,    0.878565f,    0.864645f,    0.850525f,    0.841298f,    0.827257f,    0.80818f,     0.794005f,    0.774377f,    0.760282f,
        0.746906f,    0.725848f,    0.708967f,    0.689184f,    0.66172f,     0.64063f,     0.620929f,    0.596457f,    0.574685f,    0.551871f,
        0.530905f,    0.51163f,     0.493667f,    0.474118f,    0.454311f,    0.437766f,    0.421449f,    0.406047f,    0.389741f,    0.376622f,
        0.363517f,    0.349855f,    0.336697f,    0.326962f,    0.31708f,     0.305806f,    0.29492f,     0.282203f,    0.271439f,    0.260807f,
        0.250912f,    0.243373f,    0.235734f,    0.227658f,    0.217559f,    0.208722f,    0.198576f,    0.188152f,    0.178705f,    0.168441f,
        0.159032f,    0.148125f,    0.13777f,     0.124557f,    0.109612f,    0.0917406f,   0.0715068f,   0.0503125f,   0.0328064f,   0.0198817f,
        0.00979572f,  0.00470763f,  0.00218491f,  0.00102923f,  0.000523709f, 0.000298469f, 0.000200007f, 0.000145961f, 0.000114257f, 0.000102038f,
        8.00865e-05f, 7.65951e-05f, 8.01237e-05f, 6.45166e-05f, 7.00539e-05f, 6.46441e-05f, 5.37976e-05f, 2.97925e-05f, 2.99793e-05f, 2.77311e-05f,
        6.76079e-07f};

    // Add multispectral bands to the temporary list
    const std::vector<float> vb1(b1, b1 + sizeof(b1) / sizeof(float));
    tmpSpectralBandList.push_back(vb1);
    const std::vector<float> vb2(b2, b2 + sizeof(b2) / sizeof(float));
    tmpSpectralBandList.push_back(vb2);
    const std::vector<float> vb3(b3, b3 + sizeof(b3) / sizeof(float));
    tmpSpectralBandList.push_back(vb3);
    const std::vector<float> vb4(b4, b4 + sizeof(b4) / sizeof(float));
    tmpSpectralBandList.push_back(vb4);
    const std::vector<float> vb5(b5, b5 + sizeof(b5) / sizeof(float));
    tmpSpectralBandList.push_back(vb5);
    const std::vector<float> vb6(b6, b6 + sizeof(b6) / sizeof(float));
    tmpSpectralBandList.push_back(vb6);
    const std::vector<float> vb7(b7, b7 + sizeof(b7) / sizeof(float));
    tmpSpectralBandList.push_back(vb7);
    const std::vector<float> vb8(b8, b8 + sizeof(b8) / sizeof(float));
    tmpSpectralBandList.push_back(vb8);
  }

  unsigned int j = 0;
  for (std::list<std::vector<float>>::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
  {
    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.350);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.1);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.0025);
    ++j;
  }

  return wavelengthSpectralBand;
}

void WorldView2ImageMetadataInterface::FetchSpectralSensitivityWorldView2()
{
  const std::unordered_map<std::string, std::vector<double>> BandNameToSpectralSensitivityTable{
      {"P", {
      5.53407e-06f, 6.18974e-06f, 6.77586e-06f, 4.47741e-06f, 2.18462e-06f, 2.35814e-06f, 2.74394e-06f, 3.35619e-06f, 4.12548e-06f, 3.76678e-06f,
      2.97253e-06f, 3.87911e-06f, 4.9452e-06f,  4.23503e-06f, 3.35741e-06f, 3.66642e-06f, 4.01401e-06f, 5.01097e-06f, 6.02754e-06f, 5.03579e-06f,
      3.95097e-06f, 3.61214e-06f, 3.26677e-06f, 3.72693e-06f, 4.18733e-06f, 5.20028e-06f, 6.2917e-06f,  4.84978e-06f, 3.39534e-06f, 6.07837e-06f,
      8.8389e-06f,  1.04941e-05f, 1.21421e-05f, 4.98988e-05f, 8.81089e-05f, 0.000272275f, 0.000459932f, 0.00169883f,  0.00295147f,  0.0559008f,
      0.109766f,    0.271344f,    0.436163f,    0.461822f,    0.487533f,    0.494856f,    0.504992f,    0.515215f,    0.525596f,    0.529589f,
      0.535583f,    0.529756f,    0.522233f,    0.527207f,    0.53366f,     0.528602f,    0.523089f,    0.541112f,    0.55728f,     0.55817f,
      0.558232f,    0.580899f,    0.605533f,    0.616208f,    0.626792f,    0.612118f,    0.595414f,    0.612008f,    0.630712f,    0.649153f,
      0.671541f,    0.663704f,    0.654733f,    0.672532f,    0.693098f,    0.691335f,    0.688237f,    0.67829f,     0.664973f,    0.683323f,
      0.70156f,     0.705992f,    0.713576f,    0.709848f,    0.709786f,    0.738422f,    0.765752f,    0.762756f,    0.757827f,    0.759162f,
      0.759818f,    0.779983f,    0.794085f,    0.797935f,    0.800715f,    0.811351f,    0.81929f,     0.820814f,    0.82303f,     0.829794f,
      0.838869f,    0.848913f,    0.860802f,    0.858037f,    0.858462f,    0.865197f,    0.870285f,    0.877932f,    0.889278f,    0.896475f,
      0.909253f,    0.91308f,     0.917885f,    0.914221f,    0.910762f,    0.916979f,    0.92489f,     0.93683f,     0.950791f,    0.943998f,
      0.936911f,    0.932311f,    0.929417f,    0.938274f,    0.951232f,    0.960012f,    0.962781f,    0.957501f,    0.953865f,    0.954752f,
      0.957376f,    0.968155f,    0.977735f,    0.961332f,    0.949217f,    0.946955f,    0.953334f,    0.946656f,    0.952193f,    0.96247f,
      0.969595f,    0.983968f,    1.0f,         0.99432f,     0.994003f,    0.980974f,    0.969563f,    0.968253f,    0.968397f,    0.973674f,
      0.978855f,    0.966847f,    0.963741f,    0.952252f,    0.946654f,    0.94592f,     0.952938f,    0.954759f,    0.962583f,    0.962142f,
      0.956637f,    0.950381f,    0.940962f,    0.936224f,    0.924596f,    0.923379f,    0.921684f,    0.933568f,    0.940831f,    0.957977f,
      0.963832f,    0.958143f,    0.941297f,    0.92426f,     0.903f,       0.895878f,    0.887152f,    0.910523f,    0.929402f,    0.744019f,
      0.553867f,    0.320246f,    0.0858164f,   0.0514292f,   0.0172983f,   0.0120102f,   0.00659981f,  0.00436796f,  0.00213134f,  0.00153565f,
      0.000922227f, 0.000721841f, 0.000528696f, 0.000489722f, 0.000449662f, 0.000431783f, 0.000409049f, 0.000407132f, 0.000396445f, 0.000420259f,
      0.000446526f, 0.000403311f, 0.000361257f, 0.00033034f,  0.000298026f, 0.000301128f, 0.000308537f, 0.000286243f, 0.000261497f, 0.000253604f,
      0.000242266f, 0.000274627f, 0.000307033f, 0.000295007f, 0.000283703f, 0.000261529f, 0.000237475f, 0.00024833f,  0.000258997f, 0.00027011f,
      0.000282485f, 0.000261654f, 0.000240819f, 0.000248561f, 0.00025802f,  0.000273541f, 0.000287576f, 0.000282785f, 0.000276218f, 0.000275306f,
      0.000273131f, 0.000256767f, 0.000242183f, 0.000239187f, 0.000234252f, 0.000223355f, 0.000213509f, 0.00021573f,  0.000218602f, 0.000204845f,
      0.000191131f, 0.000179884f, 0.000169031f, 0.000177057f, 0.000183172f, 0.00016757f,  0.0001513f,   0.000138821f, 0.000125806f, 0.000129696f,
      0.000133722f, 0.000122536f, 0.000111436f, 0.000102899f, 9.41714e-05f, 0.000104721f, 0.000115227f, 9.94755e-05f, 8.3857e-05f,  8.4497e-05f,
      8.52612e-05f, 7.76124e-05f, 7.00705e-05f, 7.06672e-05f, 7.1081e-05f,  6.5671e-05f,  6.00304e-05f, 6.21262e-05f, 6.42018e-05f, 5.57459e-05f,
      4.73181e-05f, 4.33172e-05f, 3.91675e-05f, 3.60807e-05f, 3.29177e-05f, 3.41277e-05f, 3.52334e-05f, 3.14413e-05f, 2.76314e-05f, 2.55335e-05f,
      2.34256e-05f, 2.1133e-05f,  1.88297e-05f, 1.80004e-05f, 1.71146e-05f, 1.51355e-05f, 1.31473e-05f, 1.21372e-05f, 1.11225e-05f, 9.97857e-06f,
      8.84718e-06f, 8.76091e-06f, 8.67416e-06f, 7.95176e-06f, 7.22607e-06f, 7.06298e-06f, 6.8991e-06f,  6.66214e-06f, 6.42406e-06f, 6.36617e-06f,
      6.32615e-06f}},


     {"C", {
      1.93924e-06f, 3.33156e-06f, 3.2732e-06f,  2.78199e-06f, 1.7488e-06f,  3.34801e-06f, 3.55785e-06f, 4.38919e-06f, 2.94112e-06f, 6.21317e-06f,
      5.37163e-06f, 5.00497e-06f, 7.41645e-06f, 1.35004e-05f, 4.53797e-05f, 0.000173069f, 0.000634651f, 0.00341498f,  0.0274284f,   0.132903f,
      0.355988f,    0.59258f,     0.700719f,    0.739625f,    0.765307f,    0.787842f,    0.817088f,    0.838687f,    0.86178f,     0.883513f,
      0.905055f,    0.917072f,    0.927208f,    0.947658f,    0.965088f,    0.979304f,    0.985921f,    0.989094f,    0.997236f,    0.974613f,
      0.85394f,     0.588032f,    0.281577f,    0.0805215f,   0.00894615f,  0.00133307f,  0.0003609f,   0.000145392f, 7.12168e-05f, 3.80253e-05f,
      1.15412e-05f, 9.59192e-06f, 5.14417e-06f, 1.11808e-05f, 1.09998e-05f, 1.07851e-05f, 9.08862e-06f, 1.0105e-05f,  1.17289e-05f, 6.3851e-06f,
      1.30646e-06f, 1.3751e-06f,  8.62503e-07f, 6.19172e-07f, 7.02247e-07f, 8.87865e-07f, 9.3924e-07f,  1.11657e-06f, 9.84522e-07f, 7.45006e-07f,
      8.99363e-07f, 1.24448e-06f, 3.62747e-06f, 1.27768e-06f, 4.00518e-07f, 5.21386e-07f, 2.69075e-07f, 4.85233e-07f, 3.69503e-07f, 1.25632e-05f,
      0.00014168f,  2.39565e-06f, 3.06503e-07f, 2.7473e-07f,  5.19969e-07f, 4.87974e-07f, 2.69412e-07f, 3.10803e-07f, 6.51482e-08f, 1.769e-10f,
      2.06764e-07f, 1.56239e-06f, 1.71434e-06f, 3.76485e-07f, 9.78272e-08f, 1.07281e-07f, 5.25843e-07f, 2.86289e-06f, 4.49334e-06f, 2.7912e-06f,
      9.77366e-07f, 1.65592e-06f, 1.25872e-06f, 1.35006e-06f, 2.26827e-06f, 3.08804e-06f, 6.08055e-06f, 1.15782e-05f, 1.00862e-05f, 5.55949e-06f,
      3.85934e-06f, 3.17286e-06f, 2.67182e-06f, 3.11772e-06f, 2.48961e-06f, 2.56662e-06f, 2.69687e-06f, 2.66657e-06f, 2.49631e-06f, 2.07413e-06f,
      2.21763e-06f, 1.82216e-06f, 1.73999e-06f, 1.79846e-06f, 1.78097e-06f, 2.08078e-06f, 2.41026e-06f, 2.95564e-06f, 4.37817e-06f, 9.26286e-06f,
      1.71525e-05f, 1.63404e-05f, 7.76378e-06f, 4.20687e-06f, 4.36152e-06f, 4.1979e-06f,  3.60385e-06f, 4.21227e-06f, 6.61165e-06f, 1.85337e-05f,
      2.63714e-05f, 1.23596e-05f, 8.08582e-06f, 7.62016e-06f, 8.54114e-06f, 9.63216e-06f, 1.21937e-05f, 2.92224e-05f, 9.75796e-05f, 9.35745e-05f,
      3.33406e-05f, 2.37882e-05f, 2.9829e-05f,  4.42465e-05f, 6.68887e-05f, 0.000152608f, 0.000422651f, 0.000256325f, 6.52584e-05f, 4.13991e-05f,
      5.7842e-05f,  0.000264601f, 0.000711195f, 0.000441052f, 8.93762e-05f, 3.04976e-05f, 1.31372e-05f, 8.13006e-06f, 5.95634e-06f, 5.94402e-06f,
      6.95574e-06f, 1.12493e-05f, 1.93408e-05f, 3.30614e-05f, 5.526e-05f,   0.000106194f, 0.000246237f, 0.000245793f, 0.000116183f, 6.90781e-05f,
      0.000121558f, 0.00012478f,  0.000160506f, 0.000195856f, 0.000163724f, 0.000116846f, 9.27976e-05f, 7.97493e-05f, 7.30327e-05f, 6.2535e-05f,
      7.15964e-05f, 6.92402e-05f, 6.98667e-05f, 7.20625e-05f, 5.92742e-05f, 4.73751e-05f, 5.11686e-05f, 3.8765e-05f,  2.87346e-05f, 2.82287e-05f,
      4.23112e-05f, 2.84265e-05f, 2.76262e-05f, 3.13753e-05f, 3.20692e-05f, 2.54603e-05f, 1.55049e-05f, 1.67992e-05f, 1.51677e-05f, 1.72863e-05f,
      1.82755e-05f, 1.62912e-05f, 1.63329e-05f, 2.11384e-05f, 1.68083e-05f, 1.32225e-05f, 9.90909e-06f, 9.57385e-06f, 9.22475e-06f, 1.59785e-05f,
      1.89273e-05f, 1.94756e-05f, 1.68079e-05f, 1.52813e-05f, 1.45048e-05f, 1.12089e-05f, 9.50048e-06f, 7.40732e-06f, 6.16214e-06f, 4.66982e-06f,
      4.04122e-06f, 3.96966e-06f, 3.02326e-06f, 3.30965e-06f, 2.53001e-06f, 3.00426e-06f, 3.01337e-06f, 3.36385e-06f, 3.56402e-06f, 4.18688e-06f,
      4.12602e-06f, 5.01737e-06f, 5.44329e-06f, 5.985e-06f,   5.40637e-06f, 6.44899e-06f, 5.42357e-06f, 4.91412e-06f, 4.3504e-06f,  3.89253e-06f,
      3.67736e-06f, 4.08168e-06f, 3.85234e-06f, 3.99802e-06f, 4.60479e-06f, 5.29422e-06f, 4.87849e-06f, 4.55674e-06f, 4.24992e-06f, 3.52154e-06f,
      3.22953e-06f, 2.58855e-06f, 2.42857e-06f, 2.34923e-06f, 2.36014e-06f, 2.33549e-06f, 2.55772e-06f, 3.03473e-06f, 3.14355e-06f, 3.65574e-06f,
      3.70734e-06f, 3.68159e-06f, 3.81222e-06f, 3.35656e-06f, 3.062e-06f,   2.69374e-06f, 2.45185e-06f, 2.09096e-06f, 1.87615e-06f, 1.59947e-06f,
      1.51572e-06f, 1.47543e-06f, 1.5459e-06f,  1.6819e-06f,  1.89924e-06f, 2.46062e-06f, 2.89706e-06f, 3.43049e-06f, 4.07493e-06f, 4.31785e-06f,
      4.56185e-06f, 4.84605e-06f, 4.70059e-06f, 5.04519e-06f, 5.03717e-06f, 5.58133e-06f, 5.772e-06f,   6.2806e-06f,  6.83109e-06f, 7.80214e-06f,
      8.13898e-06f}},
     
     {"B", {
      7.50196e-07f, 3.64689e-07f, 3.0422e-07f,  2.19926e-07f, 2.95025e-07f, 1.36813e-07f, 2.46454e-07f, 3.07665e-07f, 4.35207e-07f, 4.54783e-07f,
      4.09999e-07f, 4.6799e-07f,  4.30817e-07f, 3.21329e-07f, 5.14891e-07f, 5.88871e-07f, 7.24472e-07f, 5.19291e-07f, 5.83071e-07f, 7.385e-07f,
      2.80484e-06f, 2.40132e-06f, 1.65424e-06f, 1.01535e-06f, 2.56678e-06f, 6.15462e-06f, 1.34813e-05f, 2.75384e-05f, 4.11764e-05f, 5.15236e-05f,
      7.01286e-05f, 0.000133268f, 0.000337419f, 0.000957927f, 0.00227712f,  0.00543291f,  0.0197821f,   0.0818229f,   0.2452f,      0.503309f,
      0.70097f,     0.757605f,    0.778255f,    0.788626f,    0.8f,         0.801737f,    0.809859f,    0.82107f,     0.826045f,    0.830925f,
      0.842262f,    0.862706f,    0.878482f,    0.876943f,    0.878267f,    0.887613f,    0.907576f,    0.930559f,    0.950192f,    0.976801f,
      1.0f,         0.97894f,     0.855779f,    0.593873f,    0.297629f,    0.125661f,    0.0503684f,   0.018199f,    0.00573411f,  0.00297352f,
      0.00165595f,  0.000980273f, 0.000604892f, 0.000391497f, 0.000259234f, 0.000168111f, 0.00010894f,  7.63081e-05f, 5.14226e-05f, 5.2013e-05f,
      6.6689e-05f,  6.23362e-05f, 6.06577e-05f, 5.93819e-05f, 6.25513e-05f, 5.48608e-05f, 5.86835e-05f, 4.88126e-05f, 4.93644e-05f, 4.14711e-05f,
      3.10164e-05f, 2.97216e-05f, 1.74405e-05f, 1.74672e-05f, 1.43447e-05f, 1.09051e-05f, 1.27791e-05f, 9.19063e-06f, 8.62276e-06f, 9.16788e-06f,
      8.06522e-06f, 8.53885e-06f, 8.9662e-06f,  7.58053e-06f, 8.12959e-06f, 9.11945e-06f, 7.37798e-06f, 7.16219e-06f, 7.98185e-06f, 6.06857e-06f,
      5.53839e-06f, 6.14124e-06f, 4.19494e-06f, 3.49052e-06f, 4.14312e-06f, 3.83884e-06f, 2.7725e-06f,  3.75738e-06f, 3.84359e-06f, 3.23508e-06f,
      4.16085e-06f, 5.43268e-06f, 5.65941e-06f, 5.66555e-06f, 1.00131e-05f, 1.62239e-05f, 1.75445e-05f, 2.14947e-05f, 3.58956e-05f, 4.56586e-05f,
      3.43405e-05f, 2.38686e-05f, 2.4332e-05f,  2.54789e-05f, 1.95079e-05f, 1.33965e-05f, 1.25793e-05f, 1.40937e-05f, 1.1979e-05f,  8.63893e-06f,
      7.58208e-06f, 8.83774e-06f, 1.00948e-05f, 9.00984e-06f, 7.04681e-06f, 8.09797e-06f, 1.08886e-05f, 1.33532e-05f, 1.23902e-05f, 1.19507e-05f,
      1.48562e-05f, 2.23955e-05f, 3.10266e-05f, 3.26266e-05f, 3.10397e-05f, 3.39779e-05f, 4.5376e-05f,  6.10958e-05f, 6.5279e-05f,  5.23129e-05f,
      4.14838e-05f, 4.19558e-05f, 5.09964e-05f, 6.37575e-05f, 6.45187e-05f, 4.91907e-05f, 3.53937e-05f, 3.00032e-05f, 3.20417e-05f, 4.00312e-05f,
      4.4359e-05f,  3.93208e-05f, 2.98789e-05f, 2.47787e-05f, 2.45129e-05f, 2.9428e-05f,  4.04968e-05f, 5.068e-05f,   5.14977e-05f, 4.47032e-05f,
      5.94662e-05f, 5.43218e-05f, 6.09036e-05f, 7.73321e-05f, 8.48403e-05f, 9.27208e-05f, 9.49843e-05f, 8.04592e-05f, 5.70392e-05f, 5.08953e-05f,
      5.09718e-05f, 4.29127e-05f, 5.66121e-05f, 6.40647e-05f, 6.35005e-05f, 6.45065e-05f, 6.8561e-05f,  6.61624e-05f, 7.80311e-05f, 7.84671e-05f,
      7.47972e-05f, 5.56929e-05f, 4.48557e-05f, 3.82956e-05f, 3.24203e-05f, 2.64364e-05f, 2.01874e-05f, 2.18951e-05f, 2.14424e-05f, 2.35802e-05f,
      2.69757e-05f, 3.50803e-05f, 3.50877e-05f, 3.08309e-05f, 2.96703e-05f, 2.80215e-05f, 2.15038e-05f, 2.20744e-05f, 1.92407e-05f, 1.75484e-05f,
      1.8481e-05f,  1.73708e-05f, 1.80909e-05f, 1.87169e-05f, 1.78419e-05f, 1.86544e-05f, 1.83542e-05f, 1.71676e-05f, 1.57509e-05f, 1.42299e-05f,
      1.48274e-05f, 1.35095e-05f, 1.29937e-05f, 1.31147e-05f, 1.17814e-05f, 1.09442e-05f, 1.06259e-05f, 9.91455e-06f, 8.8098e-06f,  7.903e-06f,
      7.02151e-06f, 6.14267e-06f, 6.40226e-06f, 6.90937e-06f, 7.40346e-06f, 9.0376e-06f,  1.04649e-05f, 1.20358e-05f, 1.35923e-05f, 1.45542e-05f,
      1.52353e-05f, 1.4587e-05f,  1.27967e-05f, 1.1387e-05f,  9.40921e-06f, 7.79714e-06f, 6.31225e-06f, 5.63816e-06f, 5.29451e-06f, 5.25947e-06f,
      5.15346e-06f, 5.42692e-06f, 5.31811e-06f, 4.77555e-06f, 3.83623e-06f, 3.20429e-06f, 2.71831e-06f, 2.16288e-06f, 1.99789e-06f, 2.14329e-06f,
      2.24263e-06f, 2.42711e-06f, 2.89596e-06f, 2.86037e-06f, 2.81914e-06f, 2.48083e-06f, 2.0678e-06f,  1.92076e-06f, 1.53978e-06f, 1.43754e-06f,
      1.37064e-06f, 1.76234e-06f, 2.22508e-06f, 2.69147e-06f, 3.00682e-06f, 3.16738e-06f, 2.99304e-06f, 2.70142e-06f, 2.48436e-06f, 2.08026e-06f,
      1.84657e-06f, 2.06456e-06f, 2.72316e-06f, 3.75312e-06f, 4.7306e-06f,  5.55564e-06f, 6.32699e-06f, 5.91106e-06f, 5.11932e-06f, 4.25178e-06f,
      3.32253e-06f}},
     
     {"G", {
      4.15708e-07f, 2.60143e-07f, 1.32494e-07f, 1.5403e-07f,  1.64746e-07f, 9.97031e-08f, 1.50249e-07f, 2.02269e-07f, 2.84962e-07f, 2.57269e-07f,
      2.49117e-07f, 3.36282e-07f, 3.75568e-07f, 2.16954e-07f, 4.25352e-07f, 4.61622e-07f, 4.19126e-07f, 3.82693e-07f, 3.69658e-07f, 5.02627e-07f,
      4.42599e-07f, 4.84876e-07f, 4.95357e-07f, 4.94213e-07f, 5.20767e-07f, 3.90137e-07f, 3.59035e-07f, 5.27747e-07f, 3.37674e-07f, 4.88789e-07f,
      3.74123e-07f, 3.69271e-07f, 3.20795e-07f, 4.14981e-07f, 4.61504e-07f, 2.95452e-07f, 1.74563e-07f, 3.27091e-07f, 5.33897e-07f, 3.57089e-07f,
      5.66046e-07f, 6.04714e-07f, 4.58234e-07f, 5.14316e-07f, 2.30656e-06f, 4.42627e-06f, 8.51169e-06f, 1.27751e-05f, 1.41306e-05f, 1.59342e-05f,
      1.97713e-05f, 2.12166e-05f, 2.25515e-05f, 2.66048e-05f, 3.56016e-05f, 4.35119e-05f, 6.64286e-05f, 0.000143846f, 0.000396291f, 0.00113574f,
      0.00309186f,  0.0111042f,   0.0405198f,   0.133788f,    0.35151f,     0.606039f,    0.723007f,    0.760026f,    0.779389f,    0.788811f,
      0.80685f,     0.828035f,    0.840756f,    0.842039f,    0.844305f,    0.846857f,    0.850335f,    0.859723f,    0.863516f,    0.869143f,
      0.882571f,    0.90072f,     0.924469f,    0.942168f,    0.962285f,    0.976601f,    0.985726f,    0.994764f,    1.0f,         0.993053f,
      0.979121f,    0.907745f,    0.66404f,     0.301436f,    0.0929241f,   0.0252385f,   0.00556269f,  0.0024767f,   0.0011294f,   0.00050847f,
      0.000222899f, 0.00010723f,  5.39405e-05f, 2.56897e-05f, 6.85006e-06f, 3.52589e-06f, 1.71169e-06f, 2.75668e-07f, 1.82902e-06f, 1.67961e-06f,
      6.19122e-06f, 8.59549e-06f, 1.52263e-05f, 1.60512e-05f, 2.56929e-05f, 3.25443e-05f, 2.91571e-05f, 2.70292e-05f, 2.41446e-05f, 1.40807e-05f,
      2.13973e-07f, 1.02058e-07f, 1.9749e-07f,  1.75362e-07f, 2.57315e-07f, 4.77614e-07f, 6.77741e-07f, 7.65801e-07f, 9.90214e-07f, 7.51558e-07f,
      6.2059e-07f,  9.81957e-07f, 1.09702e-06f, 6.25215e-07f, 9.27757e-07f, 1.11919e-06f, 1.17872e-06f, 1.14779e-06f, 8.93355e-07f, 8.36189e-07f,
      1.05053e-06f, 1.08276e-06f, 1.04181e-06f, 9.27774e-07f, 1.07702e-06f, 9.37779e-07f, 9.88439e-07f, 1.13665e-06f, 1.09476e-06f, 1.05959e-06f,
      1.04925e-06f, 1.29595e-06f, 9.34329e-07f, 1.13077e-06f, 8.30981e-07f, 8.0781e-07f,  1.02709e-06f, 1.16311e-06f, 8.47734e-07f, 9.97851e-07f,
      1.11274e-06f, 1.33262e-06f, 1.05514e-06f, 9.58995e-07f, 1.08595e-06f, 1.15264e-06f, 1.41038e-06f, 1.22922e-06f, 1.70163e-06f, 3.4679e-06f,
      3.95449e-06f, 2.72695e-06f, 1.36732e-06f, 9.90143e-07f, 7.54806e-07f, 6.55742e-07f, 2.85239e-07f, 6.86364e-07f, 7.71553e-07f, 1.12635e-06f,
      9.62279e-07f, 2.59255e-06f, 9.30699e-07f, 1.62071e-07f, 1.00889e-10f, 1.00081e-10f, 0.0f,         0.0f,         0.0f,         3.23471e-08f,
      3.49174e-08f, 6.19205e-07f, 5.116e-06f,   4.16041e-06f, 1.78258e-06f, 5.02328e-07f, 2.68142e-07f, 3.8522e-07f,  7.72031e-07f, 2.61664e-07f,
      1.37503e-07f, 2.31254e-06f, 8.6085e-06f,  4.70061e-06f, 1.69519e-06f, 1.83093e-07f, 1.14424e-07f, 1.73792e-06f, 1.25833e-06f, 1.66069e-06f,
      9.83103e-07f, 1.83029e-07f, 1.17095e-05f, 9.47984e-06f, 1.12401e-05f, 9.14076e-06f, 9.03094e-06f, 8.49106e-06f, 7.68749e-06f, 6.74929e-06f,
      7.6733e-06f,  6.95369e-06f, 6.01684e-06f, 5.47181e-06f, 6.12431e-06f, 6.94762e-06f, 6.21639e-06f, 6.62084e-06f, 6.59998e-06f, 5.76706e-06f,
      6.04266e-06f, 5.40722e-06f, 5.40383e-06f, 4.91854e-06f, 4.02118e-06f, 3.62242e-06f, 3.52044e-06f, 3.49028e-06f, 2.9236e-06f,  2.56893e-06f,
      2.28989e-06f, 2.09968e-06f, 2.91966e-06f, 2.44435e-06f, 2.21268e-06f, 2.22573e-06f, 2.02219e-06f, 1.75423e-06f, 1.83833e-06f, 1.72076e-06f,
      1.51802e-06f, 1.53759e-06f, 1.24302e-06f, 1.44829e-06f, 1.35566e-06f, 1.31717e-06f, 1.18067e-06f, 1.12871e-06f, 1.2687e-06f,  1.33911e-06f,
      1.09356e-06f, 1.31515e-06f, 1.44675e-06f, 1.55647e-06f, 1.63886e-06f, 1.97263e-06f, 1.86758e-06f, 1.76664e-06f, 1.90731e-06f, 1.8888e-06f,
      1.74551e-06f, 1.59134e-06f, 1.71152e-06f, 1.35904e-06f, 1.32272e-06f, 1.27499e-06f, 1.1585e-06f,  1.26847e-06f, 1.33096e-06f, 1.48891e-06f,
      1.66013e-06f, 1.92885e-06f, 2.33777e-06f, 2.98726e-06f, 3.83011e-06f, 5.24983e-06f, 7.20736e-06f, 9.73918e-06f, 1.32221e-05f, 1.70002e-05f,
      2.10955e-05f, 2.549e-05f,   2.99685e-05f, 3.51168e-05f, 4.07206e-05f, 4.74368e-05f, 5.50521e-05f, 6.53211e-05f, 8.10989e-05f, 0.000105497f,
      0.000140101f}},
     
     {"Y", {
      3.12145e-07f, 8.28534e-08f, 9.83184e-08f, 8.55079e-08f, 4.71887e-08f, 3.85598e-08f, 6.13263e-08f, 1.09691e-07f, 8.19229e-08f, 8.18698e-08f,
      8.44374e-08f, 8.18784e-08f, 7.47651e-08f, 6.01096e-08f, 7.12291e-08f, 2.8315e-07f,  6.00765e-07f, 2.10864e-05f, 2.73104e-05f, 4.50671e-07f,
      1.06402e-08f, 6.94993e-08f, 3.13328e-08f, 1.09588e-07f, 1.74842e-07f, 1.9277e-07f,  1.34675e-07f, 1.92847e-07f, 2.02477e-07f, 2.6112e-07f,
      3.33295e-07f, 2.31552e-07f, 2.58338e-07f, 3.82314e-07f, 4.29747e-07f, 4.01449e-07f, 1.02894e-06f, 5.00318e-06f, 3.23438e-06f, 1.18679e-06f,
      5.38393e-07f, 3.44446e-07f, 3.9298e-07f,  3.47571e-07f, 7.25503e-07f, 6.40036e-07f, 5.09299e-07f, 4.9699e-07f,  4.32861e-07f, 6.78173e-07f,
      7.05903e-07f, 6.95095e-07f, 7.07479e-07f, 6.7639e-07f,  6.97946e-07f, 6.43371e-07f, 7.30317e-07f, 5.63357e-07f, 3.36905e-07f, 4.48422e-07f,
      1.74648e-07f, 3.13465e-07f, 2.23411e-07f, 2.26471e-07f, 1.67155e-07f, 1.6068e-07f,  1.35935e-07f, 2.80495e-07f, 1.38627e-07f, 2.43633e-07f,
      3.57075e-07f, 3.20907e-07f, 5.0935e-07f,  2.31732e-07f, 2.02004e-07f, 1.99908e-07f, 1.24175e-07f, 3.61372e-07f, 3.62811e-07f, 3.75752e-06f,
      7.00146e-06f, 6.23552e-06f, 4.3711e-06f,  4.90926e-06f, 3.82553e-06f, 1.1127e-05f,  2.66906e-05f, 5.75092e-05f, 0.00012712f,  0.000344491f,
      0.000895024f, 0.0022247f,   0.0058789f,   0.0265572f,   0.134652f,    0.38922f,     0.66355f,     0.848198f,    0.899806f,    0.920023f,
      0.932634f,    0.940181f,    0.947026f,    0.952441f,    0.966412f,    0.981469f,    0.989723f,    0.992706f,    1.0f,         0.969368f,
      0.759433f,    0.436651f,    0.134322f,    0.0308525f,   0.00738524f,  0.00223674f,  0.0006906f,   0.000285122f, 0.000157594f, 9.63247e-05f,
      4.43162e-05f, 2.30954e-05f, 1.28847e-05f, 7.45056e-06f, 4.97739e-06f, 3.29911e-06f, 2.15753e-06f, 1.42561e-06f, 8.30459e-07f, 9.51382e-07f,
      8.18661e-07f, 7.03925e-07f, 9.15122e-07f, 5.41677e-07f, 8.31395e-07f, 8.59648e-07f, 6.23981e-07f, 8.59012e-07f, 5.17849e-07f, 6.53052e-07f,
      9.60845e-07f, 5.49969e-07f, 8.65149e-07f, 8.47562e-07f, 7.28829e-07f, 1.14307e-06f, 9.13936e-07f, 8.44814e-07f, 1.18804e-06f, 1.11252e-06f,
      1.50364e-06f, 1.29736e-06f, 1.19152e-06f, 1.05178e-06f, 1.28277e-06f, 1.31103e-06f, 1.03605e-06f, 9.37231e-07f, 1.12941e-06f, 1.07726e-06f,
      1.01085e-06f, 8.19055e-07f, 8.60927e-07f, 8.22335e-07f, 7.66233e-07f, 8.69038e-07f, 7.82916e-07f, 7.56701e-07f, 6.00481e-07f, 4.7558e-07f,
      5.374e-07f,   6.07694e-07f, 6.55254e-07f, 2.03489e-07f, 3.94378e-07f, 5.87726e-07f, 3.09931e-07f, 4.14639e-07f, 5.61119e-07f, 6.43642e-07f,
      2.33671e-05f, 1.59193e-05f, 1.89268e-05f, 2.13128e-05f, 1.12554e-05f, 1.39639e-05f, 1.88608e-05f, 1.99851e-05f, 1.94386e-05f, 9.88685e-06f,
      1.82516e-05f, 1.15331e-05f, 9.26691e-06f, 1.352e-05f,   1.06333e-05f, 1.13419e-05f, 1.19485e-05f, 7.6223e-06f,  1.19821e-05f, 1.2663e-05f,
      1.70876e-05f, 1.1388e-05f,  6.7115e-06f,  8.61733e-06f, 8.92584e-06f, 6.81989e-06f, 4.43969e-06f, 6.43896e-06f, 4.89677e-06f, 5.81233e-06f,
      6.00927e-06f, 4.89342e-06f, 1.92265e-06f, 3.92241e-06f, 3.60834e-06f, 2.9064e-06f,  3.27708e-06f, 2.99611e-06f, 3.11394e-06f, 4.66012e-06f,
      3.91458e-06f, 4.29283e-06f, 3.86838e-06f, 3.83149e-06f, 3.27398e-06f, 2.83986e-06f, 2.47138e-06f, 2.0826e-06f,  2.02905e-06f, 1.68586e-06f,
      1.78242e-06f, 1.83929e-06f, 1.04638e-06f, 1.38024e-06f, 1.31731e-06f, 1.31521e-06f, 6.52398e-07f, 7.78377e-07f, 9.25714e-07f, 8.67073e-07f,
      8.22355e-07f, 7.92099e-07f, 8.62113e-07f, 8.90454e-07f, 6.33338e-07f, 9.92124e-07f, 8.63085e-07f, 7.34335e-07f, 9.03921e-07f, 7.04451e-07f,
      7.04449e-07f, 7.89631e-07f, 8.61175e-07f, 8.20682e-07f, 1.00168e-06f, 1.43427e-06f, 1.54572e-06f, 1.60886e-06f, 1.78755e-06f, 1.77994e-06f,
      1.53679e-06f, 1.41e-06f,    1.19628e-06f, 9.79616e-07f, 5.21729e-07f, 4.42824e-07f, 4.64902e-07f, 4.29654e-07f, 4.08101e-07f, 4.66717e-07f,
      4.10742e-07f, 3.40666e-07f, 3.24714e-07f, 4.197e-07f,   3.64468e-07f, 3.65553e-07f, 3.44671e-07f, 3.67198e-07f, 3.12263e-07f, 3.30082e-07f,
      1.99664e-07f, 1.72882e-07f, 1.60492e-07f, 1.2065e-07f,  7.23715e-08f, 1.43254e-07f, 1.12445e-07f, 8.94112e-08f, 9.22383e-08f, 8.79915e-08f,
      1.07076e-07f, 1.084e-07f,   1.27328e-07f, 1.69284e-07f, 2.04354e-07f, 2.73764e-07f, 3.37554e-07f, 4.28357e-07f, 4.83865e-07f, 5.33671e-07f,
      5.77712e-07f}},
     
     {"R", {
      3.12286e-07f, 1.21265e-07f, 6.94181e-08f, 3.9853e-08f,  8.0826e-08f,  4.04409e-08f, 9.26526e-08f, 1.12003e-07f, 2.00442e-07f, 1.25411e-07f,
      1.38037e-07f, 1.68875e-07f, 1.62312e-07f, 9.81447e-08f, 2.36616e-07f, 2.99972e-07f, 3.57437e-07f, 1.73237e-07f, 2.51165e-07f, 3.80091e-07f,
      2.95126e-07f, 3.00839e-07f, 2.8738e-07f,  3.69689e-07f, 4.13567e-07f, 3.59864e-07f, 6.22912e-07f, 5.73343e-07f, 2.85253e-07f, 3.05785e-07f,
      2.84932e-07f, 2.7333e-07f,  3.14806e-07f, 3.26106e-07f, 1.62258e-07f, 1.93473e-07f, 2.7907e-07f,  1.95591e-07f, 3.78068e-07f, 2.19725e-07f,
      3.13698e-07f, 2.73155e-07f, 3.05173e-07f, 2.00063e-07f, 1.86757e-06f, 3.44125e-06f, 7.95772e-07f, 6.25956e-07f, 1.17606e-06f, 2.40044e-06f,
      9.54706e-07f, 9.25815e-07f, 5.61706e-07f, 1.3451e-06f,  1.28024e-06f, 1.36027e-06f, 2.94643e-06f, 1.86431e-06f, 2.88379e-06f, 2.16267e-06f,
      1.53977e-06f, 2.27062e-06f, 8.29005e-07f, 2.30761e-07f, 3.01119e-07f, 2.36372e-07f, 2.45071e-07f, 2.32871e-07f, 1.90817e-07f, 2.06172e-07f,
      1.14463e-07f, 3.52299e-08f, 2.27043e-07f, 3.45896e-08f, 2.42045e-08f, 1.21876e-08f, 6.62187e-08f, 6.09625e-08f, 1.41816e-07f, 8.79884e-08f,
      1.19441e-07f, 3.73189e-08f, 7.59332e-08f, 2.40439e-07f, 1.17802e-07f, 6.55622e-08f, 8.63856e-08f, 6.44464e-08f, 2.23776e-07f, 1.03047e-07f,
      1.32551e-07f, 2.79429e-07f, 2.07195e-07f, 3.67864e-07f, 5.11545e-07f, 5.89037e-07f, 8.2924e-07f,  8.56781e-07f, 1.3186e-06f,  9.10331e-06f,
      1.26163e-05f, 2.68254e-05f, 4.09503e-05f, 7.03781e-05f, 0.000122912f, 0.000268666f, 0.000628092f, 0.00164922f,  0.00486736f,  0.0227608f,
      0.0955659f,   0.294353f,    0.596071f,    0.822289f,    0.872776f,    0.885335f,    0.927928f,    0.955695f,    0.965042f,    0.967095f,
      0.965588f,    0.957776f,    0.9555f,      0.952831f,    0.955801f,    0.965253f,    0.970358f,    0.976493f,    0.987526f,    0.996575f,
      0.999085f,    0.992107f,    0.983694f,    0.978311f,    0.88528f,     0.632595f,    0.291823f,    0.0832547f,   0.0204412f,   0.00563548f,
      0.00191373f,  0.000843246f, 0.000403589f, 0.000203731f, 0.000101781f, 5.6897e-05f,  2.7195e-05f,  1.61825e-05f, 1.4231e-05f,  1.68343e-05f,
      1.3334e-05f,  1.44383e-05f, 1.32012e-05f, 1.24899e-05f, 1.05706e-05f, 6.08328e-06f, 3.05093e-06f, 3.42552e-06f, 1.54743e-06f, 1.36525e-06f,
      1.23105e-06f, 4.3642e-07f,  5.81041e-07f, 1.18259e-06f, 3.00501e-06f, 4.97549e-06f, 9.51182e-06f, 7.32646e-06f, 6.71037e-06f, 8.93449e-06f,
      6.85023e-06f, 4.8556e-06f,  5.46221e-06f, 2.19395e-06f, 1.5006e-06f,  4.9089e-07f,  2.22462e-06f, 6.60787e-07f, 6.09816e-07f, 9.03731e-07f,
      1.37929e-05f, 1.40411e-05f, 1.3406e-05f,  1.63301e-05f, 1.06067e-05f, 1.24196e-05f, 1.08121e-05f, 7.69758e-06f, 8.8836e-06f,  9.03241e-06f,
      7.87554e-06f, 4.2784e-06f,  1.07825e-05f, 4.51337e-06f, 6.97831e-06f, 6.53672e-06f, 6.20341e-06f, 6.41495e-06f, 8.42444e-06f, 6.85528e-06f,
      2.70354e-06f, 6.50522e-06f, 6.34091e-06f, 6.54655e-06f, 5.05613e-06f, 7.09478e-06f, 9.15616e-06f, 1.08124e-05f, 6.65697e-06f, 7.89558e-06f,
      4.14291e-06f, 4.32081e-06f, 8.59893e-06f, 6.22021e-06f, 7.2363e-06f,  6.14019e-06f, 3.87893e-06f, 5.4737e-06f,  4.80289e-06f, 5.01621e-06f,
      5.71604e-06f, 5.0985e-06f,  4.24175e-06f, 4.44819e-06f, 3.97763e-06f, 3.48069e-06f, 3.4912e-06f,  4.00016e-06f, 2.68235e-06f, 3.0765e-06f,
      3.20229e-06f, 2.41872e-06f, 3.06521e-06f, 2.72094e-06f, 2.13903e-06f, 1.70534e-06f, 1.72736e-06f, 1.5771e-06f,  1.31904e-06f, 1.10782e-06f,
      1.20611e-06f, 9.60313e-07f, 1.25562e-06f, 9.96851e-07f, 1.08323e-06f, 9.27816e-07f, 7.70516e-07f, 6.93097e-07f, 7.47027e-07f, 7.07703e-07f,
      8.10806e-07f, 7.26741e-07f, 6.80176e-07f, 6.20037e-07f, 6.39881e-07f, 5.95663e-07f, 4.7329e-07f,  5.43599e-07f, 5.47303e-07f, 6.05122e-07f,
      5.1772e-07f,  5.88033e-07f, 5.45848e-07f, 5.08645e-07f, 2.36035e-07f, 5.84744e-07f, 5.20728e-07f, 4.18312e-07f, 3.71858e-07f, 4.39561e-07f,
      3.37007e-07f, 2.56984e-07f, 3.79198e-07f, 4.64178e-07f, 5.27954e-07f, 6.02129e-07f, 6.16109e-07f, 8.1714e-07f,  7.41506e-07f, 6.23132e-07f,
      4.93815e-07f, 3.36936e-07f, 2.6114e-07f,  1.50937e-07f, 1.37661e-07f, 1.5674e-07f,  1.21087e-07f, 1.3344e-07f,  1.58735e-07f, 1.86711e-07f,
      1.80942e-07f, 2.25817e-07f, 2.17427e-07f, 2.35791e-07f, 2.14853e-07f, 1.85285e-07f, 1.48392e-07f, 1.1159e-07f,  8.25772e-08f, 7.09923e-08f,
      5.57264e-08f}},
     
     {"RE", {
      1.31873e-07f, 1.87926e-07f, 2.81592e-07f, 2.17374e-07f, 1.70877e-07f, 1.0562e-07f,  1.12979e-07f, 2.00636e-07f, 1.89536e-07f, 2.42299e-07f,
      1.75878e-07f, 1.93174e-07f, 2.64698e-07f, 2.57301e-07f, 3.40167e-07f, 3.55343e-07f, 7.60476e-07f, 2.25433e-06f, 1.29749e-06f, 1.29558e-06f,
      2.01722e-06f, 1.13819e-06f, 1.64078e-06f, 9.96598e-07f, 9.78403e-07f, 1.06118e-06f, 8.77215e-07f, 1.97241e-06f, 1.14645e-06f, 8.83069e-07f,
      7.40957e-07f, 1.02664e-06f, 9.67904e-06f, 5.091e-05f,   2.50784e-06f, 3.48606e-07f, 8.32043e-07f, 2.39095e-05f, 2.58811e-06f, 2.33621e-07f,
      1.98094e-07f, 2.85714e-07f, 1.27594e-07f, 1.7327e-07f,  1.82199e-07f, 1.5321e-07f,  1.63299e-07f, 1.01052e-07f, 1.49075e-07f, 5.47801e-07f,
      7.84427e-07f, 1.65041e-06f, 8.51018e-07f, 5.77883e-06f, 1.38337e-05f, 7.9076e-06f,  3.87846e-06f, 5.69445e-06f, 1.53368e-05f, 9.94259e-06f,
      1.2e-06f,     1.54553e-06f, 4.22091e-06f, 1.43071e-06f, 2.00613e-07f, 1.81045e-07f, 1.2783e-07f,  1.03067e-07f, 1.82997e-07f, 1.10773e-07f,
      1.1226e-07f,  1.16695e-07f, 2.16241e-07f, 1.05663e-07f, 1.614e-07f,   1.6817e-07f,  1.16441e-07f, 1.15124e-07f, 6.99069e-08f, 1.53185e-08f,
      2.72037e-08f, 1.23476e-07f, 1.55133e-07f, 1.49846e-07f, 3.58885e-07f, 2.02413e-06f, 2.0841e-06f,  8.13696e-07f, 9.02676e-07f, 2.00266e-06f,
      3.1147e-06f,  3.09139e-06f, 4.4865e-06f,  6.70102e-06f, 6.05048e-06f, 5.02704e-06f, 7.09114e-06f, 1.13053e-05f, 1.37502e-05f, 1.60627e-05f,
      2.40093e-05f, 3.44167e-05f, 3.74905e-05f, 3.98343e-05f, 5.38148e-05f, 7.94317e-05f, 8.83804e-05f, 7.59032e-05f, 9.03911e-05f, 0.000174361f,
      0.000184766f, 5.17257e-05f, 8.78977e-06f, 4.47753e-06f, 2.69144e-06f, 1.45718e-06f, 1.07089e-06f, 6.55321e-07f, 1.38442e-06f, 2.79808e-06f,
      5.03238e-06f, 6.41933e-06f, 8.09994e-06f, 1.05514e-05f, 1.15504e-05f, 1.14512e-05f, 1.17762e-05f, 1.04862e-05f, 1.12589e-05f, 8.43843e-06f,
      8.21063e-06f, 8.85197e-06f, 9.70519e-06f, 1.53186e-05f, 3.28e-05f,    7.87273e-05f, 0.000207342f, 0.000759089f, 0.00312753f,  0.0188368f,
      0.0996101f,   0.346835f,    0.622908f,    0.853785f,    0.952792f,    0.978422f,    0.987634f,    0.989026f,    0.993901f,    0.999858f,
      0.99928f,     0.989417f,    0.986813f,    0.980962f,    0.977116f,    0.944191f,    0.846399f,    0.615837f,    0.359763f,    0.138813f,
      0.0305442f,   0.00709174f,  0.00215848f,  0.000729986f, 0.000269875f, 0.000118803f, 5.53309e-05f, 2.39914e-05f, 9.56683e-06f, 6.93469e-06f,
      5.29459e-06f, 3.2974e-06f,  4.31265e-06f, 4.03836e-06f, 5.07602e-06f, 7.67396e-06f, 8.34915e-06f, 9.4325e-06f,  6.44795e-06f, 5.01789e-06f,
      6.71178e-08f, 3.03948e-07f, 3.1684e-07f,  2.65135e-07f, 2.09765e-07f, 3.00101e-07f, 1.67201e-07f, 1.60492e-07f, 2.13433e-07f, 1.29106e-07f,
      9.71011e-08f, 2.9819e-07f,  1.86975e-07f, 2.30772e-07f, 1.64834e-07f, 6.97091e-08f, 3.40321e-07f, 2.85903e-07f, 2.06352e-07f, 3.65446e-07f,
      4.38643e-07f, 1.65414e-07f, 8.01729e-07f, 7.59196e-07f, 1.87131e-06f, 3.94486e-06f, 8.7793e-06f,  1.12049e-05f, 1.14224e-05f, 7.65019e-06f,
      4.71997e-06f, 4.25463e-06f, 1.04158e-05f, 8.44869e-06f, 7.60545e-06f, 7.43366e-06f, 8.91167e-06f, 1.17538e-05f, 1.53146e-05f, 1.90496e-05f,
      1.73994e-05f, 1.71082e-05f, 1.83056e-05f, 1.86976e-05f, 1.80158e-05f, 1.89996e-05f, 1.5662e-05f,  1.44995e-05f, 1.26694e-05f, 1.02432e-05f,
      8.13754e-06f, 6.21556e-06f, 4.54308e-06f, 3.91257e-06f, 3.1588e-06f,  2.52225e-06f, 2.01583e-06f, 1.62728e-06f, 1.47903e-06f, 1.09533e-06f,
      1.03503e-06f, 9.97997e-07f, 7.70181e-07f, 7.25907e-07f, 8.38991e-07f, 5.98253e-07f, 7.1384e-07f,  4.89317e-07f, 6.07583e-07f, 5.97359e-07f,
      7.18152e-07f, 5.91987e-07f, 6.80961e-07f, 3.87465e-07f, 4.28846e-07f, 6.08958e-07f, 3.8379e-07f,  5.16479e-07f, 5.35559e-07f, 4.5999e-07f,
      4.5037e-07f,  4.34943e-07f, 3.90623e-07f, 3.55287e-07f, 3.677e-07f,   4.21178e-07f, 3.62045e-07f, 3.55015e-07f, 2.54848e-07f, 2.60974e-07f,
      2.42997e-07f, 1.96909e-07f, 2.38909e-07f, 1.73543e-07f, 2.20613e-07f, 1.73692e-07f, 1.66732e-07f, 1.40222e-07f, 1.22858e-07f, 1.39263e-07f,
      1.49344e-07f, 1.63372e-07f, 1.54912e-07f, 1.45368e-07f, 1.58958e-07f, 1.93923e-07f, 1.65487e-07f, 1.60786e-07f, 1.52013e-07f, 1.35784e-07f,
      9.24469e-08f, 9.92189e-08f, 7.60063e-08f, 9.85175e-08f, 7.22548e-08f, 4.57741e-08f, 6.97201e-08f, 8.69437e-08f, 1.13252e-07f, 2.58077e-07f,
      4.68462e-07f}},
     
     {"N", {
      3.42494e-07f, 5.7563e-09f,  2.4306e-08f,  6.23161e-08f, 8.60169e-08f, 6.54447e-09f, 4.9819e-08f,  9.30485e-08f, 5.51852e-08f, 1.75292e-08f,
      2.87474e-08f, 2.73409e-08f, 2.83316e-08f, 3.82621e-09f, 3.07146e-08f, 4.55383e-08f, 1.04778e-07f, 1.23884e-09f, 8.25112e-09f, 5.0621e-08f,
      1.79225e-07f, 2.74552e-08f, 1.04373e-07f, 9.7114e-08f,  9.62771e-08f, 1.75398e-07f, 8.63102e-08f, 6.84208e-08f, 9.40778e-08f, 5.15181e-08f,
      1.02132e-07f, 1.42588e-07f, 8.08711e-08f, 2.16959e-07f, 1.03608e-07f, 9.2287e-08f,  1.59861e-06f, 6.11993e-06f, 3.68931e-07f, 9.24078e-08f,
      3.81349e-07f, 2.64627e-07f, 3.27124e-07f, 2.55578e-07f, 2.87094e-07f, 3.84e-07f,    5.21921e-07f, 6.14658e-07f, 3.75539e-07f, 3.80046e-07f,
      7.36489e-07f, 1.37786e-06f, 6.07935e-07f, 9.45777e-07f, 1.59554e-06f, 5.68797e-07f, 4.72783e-07f, 5.63447e-07f, 3.35752e-07f, 1.70326e-07f,
      3.17931e-07f, 1.41325e-07f, 2.49863e-07f, 3.96423e-07f, 2.16004e-07f, 3.37582e-07f, 2.90124e-07f, 3.80183e-07f, 3.11457e-07f, 3.98668e-07f,
      1.79901e-07f, 1.22001e-07f, 1.13002e-07f, 2.85022e-08f, 4.67327e-08f, 1.90183e-08f, 0.0f,         2.99103e-08f, 1.06093e-07f, 1.18422e-07f,
      1.07083e-08f, 7.91334e-08f, 9.95149e-08f, 1.54428e-07f, 9.12854e-08f, 2.15373e-08f, 7.48973e-08f, 7.40942e-08f, 0.0f,         3.73909e-08f,
      7.49644e-08f, 0.0f,         0.0f,         1.92802e-08f, 2.95975e-08f, 7.04334e-08f, 6.50404e-08f, 0.0f,         0.0f,         0.0f,
      2.0755e-07f,  1.04836e-10f, 7.80821e-09f, 1.57983e-07f, 1.0595e-10f,  1.13403e-07f, 3.312e-10f,   1.14176e-07f, 1.03109e-10f, 3.76115e-09f,
      7.45634e-09f, 1.19272e-07f, 1.07481e-10f, 1.10155e-10f, 1.1276e-10f,  1.40161e-07f, 1.16185e-10f, 7.139e-08f,   9.90442e-08f, 3.74204e-07f,
      5.05087e-07f, 4.6352e-07f,  5.97207e-07f, 5.4384e-07f,  6.81312e-07f, 6.90591e-07f, 1.00825e-06f, 8.18553e-07f, 1.07975e-06f, 8.27673e-07f,
      9.35167e-07f, 9.64121e-07f, 1.00596e-06f, 8.68247e-07f, 8.97409e-07f, 1.19586e-06f, 1.28982e-06f, 9.86575e-07f, 9.25037e-07f, 2.05008e-06f,
      1.71977e-06f, 3.51855e-06f, 4.80754e-06f, 6.35905e-06f, 5.22507e-06f, 4.29703e-06f, 3.92372e-06f, 4.42103e-06f, 4.97574e-06f, 5.96695e-06f,
      6.60085e-06f, 1.11374e-05f, 1.62372e-05f, 2.84438e-05f, 4.4002e-05f,  7.0506e-05f,  0.000106141f, 0.000165311f, 0.000256229f, 0.000412257f,
      0.000713036f, 0.00139941f,  0.00288571f,  0.00595325f,  0.0126868f,   0.0276938f,   0.0588599f,   0.123117f,    0.260204f,    0.506774f,
      0.759967f,    0.94058f,     0.995805f,    0.997622f,    0.988907f,    0.983091f,    0.972934f,    0.958312f,    0.941139f,    0.931907f,
      0.919382f,    0.90908f,     0.8976f,      0.88212f,     0.871191f,    0.863054f,    0.844284f,    0.836496f,    0.838036f,    0.841189f,
      0.835657f,    0.833819f,    0.842548f,    0.830777f,    0.818604f,    0.816837f,    0.806711f,    0.81076f,     0.794161f,    0.779884f,
      0.767313f,    0.749602f,    0.739271f,    0.738714f,    0.734145f,    0.711833f,    0.697141f,    0.680462f,    0.658635f,    0.649781f,
      0.632471f,    0.619908f,    0.60912f,     0.598992f,    0.590914f,    0.566107f,    0.507112f,    0.402799f,    0.275174f,    0.157667f,
      0.0738047f,   0.032787f,    0.0138147f,   0.00539084f,  0.00262463f,  0.00180863f,  0.00130633f,  0.00103281f,  0.000917177f, 0.000891162f,
      0.000798344f, 0.000745739f, 0.000727178f, 0.000783273f, 0.000701058f, 0.000655311f, 0.00060278f,  0.000557875f, 0.000281186f, 0.000274231f,
      4.37496e-06f, 3.22571e-06f, 2.88909e-06f, 2.24195e-06f, 1.98867e-06f, 1.47066e-06f, 1.12615e-06f, 1.04739e-06f, 1.1344e-06f,  8.90363e-07f,
      8.71592e-07f, 7.40614e-07f, 7.19518e-07f, 6.91579e-07f, 7.01778e-07f, 6.72029e-07f, 4.36737e-07f, 6.31103e-07f, 6.64941e-07f, 6.02511e-07f,
      3.22655e-07f, 5.32912e-07f, 4.71662e-07f, 4.99519e-07f, 3.35434e-07f, 4.46158e-07f, 4.48065e-07f, 3.19015e-07f, 2.39899e-07f, 4.55294e-07f,
      2.89741e-07f, 2.5021e-07f,  2.57359e-07f, 3.5683e-07f,  2.96983e-07f, 2.97873e-07f, 1.88219e-07f, 2.40558e-07f, 2.15601e-07f, 2.13278e-07f,
      1.40431e-07f, 1.60961e-07f, 1.70122e-07f, 1.10615e-07f, 1.0436e-07f,  1.222e-07f,   1.25283e-07f, 9.88137e-08f, 8.59799e-08f, 9.19016e-08f,
      7.99614e-08f, 8.73603e-08f, 7.68151e-08f, 6.51417e-08f, 7.38325e-08f, 6.48494e-08f, 5.64187e-08f, 6.5542e-08f,  5.90555e-08f, 4.77664e-08f,
      5.59409e-08f}},
     
     {"N2", {
      2.75157e-07f, 6.96359e-06f, 1.9874e-05f,  8.73924e-05f, 7.4835e-05f,  5.16098e-06f, 4.81496e-07f, 6.24369e-07f, 2.89158e-07f, 1.33848e-07f,
      1.61046e-07f, 2.69148e-07f, 1.41051e-07f, 1.93786e-07f, 2.0957e-07f,  2.34628e-07f, 2.36518e-07f, 2.47669e-07f, 2.25346e-07f, 2.01273e-07f,
      2.14493e-07f, 1.42204e-07f, 1.7784e-07f,  2.29739e-07f, 4.0488e-07f,  2.15782e-07f, 1.58074e-07f, 1.50063e-07f, 7.18437e-08f, 1.88801e-07f,
      2.3008e-07f,  1.64135e-07f, 1.27638e-07f, 1.9751e-07f,  1.13678e-07f, 7.76496e-07f, 1.1877e-06f,  1.47692e-07f, 2.12794e-07f, 1.89394e-06f,
      1.22e-06f,    8.28353e-07f, 5.66846e-07f, 1.55612e-07f, 3.89524e-08f, 6.08863e-08f, 4.13505e-07f, 1.57781e-07f, 8.45778e-09f, 9.45575e-07f,
      3.68309e-06f, 1.1502e-06f,  2.14722e-07f, 1.01022e-07f, 6.8025e-07f,  8.6337e-07f,  3.19639e-08f, 2.56285e-07f, 1.35378e-07f, 3.34694e-08f,
      7.36246e-07f, 8.36204e-07f, 2.65609e-08f, 8.10432e-08f, 2.05826e-07f, 3.69154e-07f, 8.02175e-07f, 8.89483e-07f, 4.4826e-07f,  2.42823e-07f,
      2.17011e-07f, 2.65704e-06f, 8.78188e-06f, 7.05779e-07f, 3.82597e-07f, 4.04534e-06f, 4.67549e-06f, 9.19411e-08f, 2.98568e-07f, 2.08203e-06f,
      1.03067e-06f, 9.72367e-08f, 1.50024e-07f, 1.2413e-07f,  1.4876e-07f,  2.35395e-08f, 1.65976e-10f, 1.65631e-10f, 8.45577e-09f, 1.59829e-10f,
      1.56488e-10f, 1.35137e-07f, 1.26482e-06f, 9.54683e-08f, 2.93604e-07f, 1.59714e-07f, 2.23112e-07f, 5.34018e-07f, 6.1337e-07f,  1.72864e-07f,
      5.77747e-07f, 4.80195e-07f, 5.14193e-07f, 5.97282e-07f, 1.16949e-06f, 5.3439e-07f,  2.8602e-07f,  2.20078e-07f, 5.34755e-07f, 5.51942e-07f,
      6.32398e-07f, 1.13151e-06f, 1.09476e-06f, 3.24873e-07f, 3.6263e-07f,  5.49154e-07f, 8.32561e-08f, 6.24779e-07f, 6.90714e-07f, 8.366e-07f,
      1.2346e-06f,  1.93899e-06f, 2.35572e-06f, 9.86481e-07f, 5.4618e-07f,  1.53631e-06f, 4.98064e-07f, 4.68967e-08f, 2.82103e-07f, 1.79437e-07f,
      2.82801e-07f, 5.47962e-07f, 7.69025e-07f, 1.62521e-07f, 2.32749e-07f, 5.04263e-07f, 8.32099e-07f, 2.27261e-07f, 4.08301e-07f, 3.43738e-07f,
      1.2715e-07f,  2.36481e-07f, 8.74008e-08f, 3.913e-07f,   4.88832e-07f, 6.9057e-07f,  7.13913e-07f, 8.95172e-07f, 8.96901e-07f, 5.53029e-07f,
      1.04012e-06f, 9.12699e-07f, 7.19448e-07f, 7.20192e-07f, 8.80887e-07f, 1.14393e-06f, 5.57443e-07f, 9.4312e-07f,  1.04838e-06f, 9.98724e-07f,
      1.23283e-06f, 8.72156e-07f, 8.03599e-07f, 5.35655e-07f, 8.46701e-07f, 6.91104e-07f, 8.55943e-07f, 6.31436e-07f, 4.51508e-07f, 1.11291e-06f,
      1.03136e-06f, 4.73296e-07f, 6.08156e-07f, 8.62452e-07f, 9.2854e-07f,  7.79087e-07f, 1.22534e-06f, 0.000114086f, 0.000453417f, 0.000504746f,
      0.000779556f, 0.0010461f,   0.000976304f, 0.00111328f,  0.0010687f,   0.00109088f,  0.00109552f,  0.00113106f,  0.00109447f,  0.00108912f,
      0.00104157f,  0.0011403f,   0.001127f,    0.00117057f,  0.00115094f,  0.00120607f,  0.00135466f,  0.00130018f,  0.00149181f,  0.002231f,
      0.00432839f,  0.0105836f,   0.0305957f,   0.107853f,    0.313746f,    0.571884f,    0.789269f,    0.951541f,    0.987144f,    0.991677f,
      0.993363f,    0.999691f,    0.996691f,    0.983327f,    0.970856f,    0.95236f,     0.93302f,     0.922376f,    0.910922f,    0.899767f,
      0.892112f,    0.878565f,    0.864645f,    0.850525f,    0.841298f,    0.827257f,    0.80818f,     0.794005f,    0.774377f,    0.760282f,
      0.746906f,    0.725848f,    0.708967f,    0.689184f,    0.66172f,     0.64063f,     0.620929f,    0.596457f,    0.574685f,    0.551871f,
      0.530905f,    0.51163f,     0.493667f,    0.474118f,    0.454311f,    0.437766f,    0.421449f,    0.406047f,    0.389741f,    0.376622f,
      0.363517f,    0.349855f,    0.336697f,    0.326962f,    0.31708f,     0.305806f,    0.29492f,     0.282203f,    0.271439f,    0.260807f,
      0.250912f,    0.243373f,    0.235734f,    0.227658f,    0.217559f,    0.208722f,    0.198576f,    0.188152f,    0.178705f,    0.168441f,
      0.159032f,    0.148125f,    0.13777f,     0.124557f,    0.109612f,    0.0917406f,   0.0715068f,   0.0503125f,   0.0328064f,   0.0198817f,
      0.00979572f,  0.00470763f,  0.00218491f,  0.00102923f,  0.000523709f, 0.000298469f, 0.000200007f, 0.000145961f, 0.000114257f, 0.000102038f,
      8.00865e-05f, 7.65951e-05f, 8.01237e-05f, 6.45166e-05f, 7.00539e-05f, 6.46441e-05f, 5.37976e-05f, 2.97925e-05f, 2.99793e-05f, 2.77311e-05f,
      6.76079e-07f}}
    };

  for (auto & band: m_Imd.Bands)
  {
    auto SpectralSensitivityIt = BandNameToSpectralSensitivityTable.find(band[MDStr::BandName] );
    if (SpectralSensitivityIt != BandNameToSpectralSensitivityTable.end())
    {
      otb::MetaData::LUT1D spectralSensitivity;

      spectralSensitivity.Axis[0].Size = 301;
      spectralSensitivity.Axis[0].Origin = 0.35;
      spectralSensitivity.Axis[0].Spacing = 0.0025;
      spectralSensitivity.Array = SpectralSensitivityIt->second;

      band.Add(MDL1D::SpectralSensitivity, spectralSensitivity);
    }
  }
}

void WorldView2ImageMetadataInterface::FetchSpectralSensitivityQuickBird()
{
  const std::unordered_map<std::string, std::vector<double>> BandNameToSpectralSensitivityTable{
      {"P",{
        0.0f,       0.0f,        0.0f,      0.0f,       0.0f,      0.0f,       0.0f,       0.0f,      0.0f,      0.0f,      0.0f,       0.0f,      0.0f,
        0.0f,       0.0f,        0.0f,      0.0f,       0.0f,      0.0f,       0.0007015f, 0.001403f, 0.001291f, 0.001179f, 0.0011415f, 0.001104f, 0.00101145f,
        0.0009189f, 0.00104845f, 0.001178f, 0.0016055f, 0.002033f, 0.0044495f, 0.006866f,  0.008873f, 0.01088f,  0.013805f, 0.01673f,   0.021325f, 0.02592f,
        0.03106f,   0.0362f,     0.043415f, 0.05063f,   0.05879f,  0.06695f,   0.075765f,  0.08458f,  0.09389f,  0.1032f,   0.11255f,   0.1219f,   0.13345f,
        0.145f,     0.1567f,     0.1684f,   0.17895f,   0.1895f,   0.2011f,    0.2127f,    0.21625f,  0.2198f,   0.2392f,   0.2586f,    0.26695f,  0.2753f,
        0.2854f,    0.2955f,     0.30545f,  0.3154f,    0.3259f,   0.3364f,    0.341f,     0.3456f,   0.3501f,   0.3546f,   0.35785f,   0.3611f,   0.3648f,
        0.3685f,    0.3733f,     0.3781f,   0.3875f,    0.3969f,   0.4055f,    0.4141f,    0.42755f,  0.441f,    0.4543f,   0.4676f,    0.4829f,   0.4982f,
        0.5167f,    0.5352f,     0.5532f,   0.5712f,    0.59245f,  0.6137f,    0.6382f,    0.6627f,   0.6839f,   0.7051f,   0.7275f,    0.7499f,   0.77045f,
        0.791f,     0.81055f,    0.8301f,   0.8465f,    0.8629f,   0.877f,     0.8911f,    0.9081f,   0.9251f,   0.9293f,   0.9335f,    0.94265f,  0.9518f,
        0.95695f,   0.9621f,     0.96095f,  0.9598f,    0.97525f,  0.9907f,    0.9941f,    0.9975f,   0.9922f,   0.9869f,   0.99345f,   1.0f,      0.9938f,
        0.9876f,    0.9877f,     0.9878f,   0.977f,     0.9662f,   0.95725f,   0.9483f,    0.934f,    0.9197f,   0.919f,    0.9183f,    0.90635f,  0.8944f,
        0.8912f,    0.888f,      0.88305f,  0.8781f,    0.8865f,   0.8949f,    0.8921f,    0.8893f,   0.8858f,   0.8823f,   0.88695f,   0.8916f,   0.8848f,
        0.878f,     0.8879f,     0.8978f,   0.8933f,    0.8888f,   0.8801f,    0.8714f,    0.86905f,  0.8667f,   0.8677f,   0.8687f,    0.86995f,  0.8712f,
        0.86685f,   0.8625f,     0.87175f,  0.881f,     0.8814f,   0.8818f,    0.8897f,    0.8976f,   0.9024f,   0.9072f,   0.91835f,   0.9295f,   0.93105f,
        0.9326f,    0.9198f,     0.907f,    0.91305f,   0.9191f,   0.9173f,    0.9155f,    0.9097f,   0.9039f,   0.89875f,  0.8936f,    0.88705f,  0.8805f,
        0.8761f,    0.8717f,     0.8605f,   0.8493f,    0.8475f,   0.8457f,    0.8505f,    0.8553f,   0.85675f,  0.8582f,   0.85825f,   0.8583f,   0.8627f,
        0.8671f,    0.8505f,     0.8339f,   0.83895f,   0.844f,    0.847f,     0.85f,      0.85265f,  0.8553f,   0.8447f,   0.8341f,    0.83675f,  0.8394f,
        0.8235f,    0.8076f,     0.8091f,   0.8106f,    0.785f,    0.7594f,    0.7616f,    0.7638f,   0.749f,    0.7342f,   0.7204f,    0.7066f,   0.70035f,
        0.6941f,    0.67875f,    0.6634f,   0.65895f,   0.6545f,   0.63655f,   0.6186f,    0.60945f,  0.6003f,   0.5961f,   0.5919f,    0.58315f,  0.5744f,
        0.55265f,   0.5309f,     0.5139f,   0.4969f,    0.48835f,  0.4798f,    0.46305f,   0.4463f,   0.4305f,   0.4147f,   0.40975f,   0.4048f,   0.3893f,
        0.3738f,    0.3593f,     0.3448f,   0.3283f,    0.3118f,   0.3035f,    0.2952f,    0.28405f,  0.2729f,   0.2648f,   0.2567f,    0.24975f,  0.2428f,
        0.2322f,    0.2216f,     0.21615f,  0.2107f,    0.2039f,   0.1971f,    0.18735f,   0.1776f,   0.1703f,   0.163f,    0.15575f,   0.1485f,   0.14215f,
        0.1358f,    0.1299f,     0.124f,    0.11625f,   0.1085f,   0.10197f,   0.09544f,   0.08927f,  0.0831f,   0.077305f, 0.07151f,   0.06746f,  0.06341f,
        0.05911f,   0.05481f,    0.051195f, 0.04758f,   0.02882f,  0.01006f,   0.00503f,   0.0f,      0.0f,      0.0f,      0.0f,       0.0f,      0.0f,
        0.0f,       0.0f,        0.0f,      0.0f,       0.0f,      0.0f,       0.0f,       0.0f,      0.0f}},
      {"B",{
        0.0f,      4.816e-11f, 9.632e-11f, 1.4606e-10f, 1.958e-10f, 2.4715e-10f, 2.985e-10f, 3.514e-10f, 4.043e-10f, 0.000505f,  0.00101f,  0.0015305f,
        0.002051f, 0.002587f,  0.003123f,  0.0036745f,  0.004226f,  0.0043815f,  0.004537f,  0.0049295f, 0.005322f,  0.0046895f, 0.004057f, 0.003661f,
        0.003265f, 0.002984f,  0.002703f,  0.002503f,   0.002303f,  0.0017815f,  0.00126f,   0.0015305f, 0.001801f,  0.0020025f, 0.002204f, 0.0022995f,
        0.002395f, 0.0024735f, 0.002552f,  0.002512f,   0.002472f,  0.0026895f,  0.002907f,  0.003455f,  0.004003f,  0.0052815f, 0.00656f,  0.00985f,
        0.01314f,  0.02076f,   0.02838f,   0.04826f,    0.06814f,   0.11167f,    0.1552f,    0.2164f,    0.2776f,    0.35445f,   0.4313f,   0.5306f,
        0.6299f,   0.677f,     0.7241f,    0.75775f,    0.7914f,    0.81765f,    0.8439f,    0.86675f,   0.8896f,    0.9037f,    0.9178f,   0.92355f,
        0.9293f,   0.94475f,   0.9602f,    0.9801f,     1.0f,       0.9958f,     0.9916f,    0.95475f,   0.9179f,    0.84515f,   0.7724f,   0.6729f,
        0.5734f,   0.4787f,    0.384f,     0.32465f,    0.2653f,    0.21225f,    0.1592f,    0.120415f,  0.08163f,   0.075435f,  0.06924f,  0.06627f,
        0.0633f,   0.055115f,  0.04693f,   0.03814f,    0.02935f,   0.023495f,   0.01764f,   0.015185f,  0.01273f,   0.012595f,  0.01246f,  0.012675f,
        0.01289f,  0.012755f,  0.01262f,   0.011865f,   0.01111f,   0.0106f,     0.01009f,   0.0098255f, 0.009561f,  0.0094225f, 0.009284f, 0.0095255f,
        0.009767f, 0.0102035f, 0.01064f,   0.010515f,   0.01039f,   0.0091415f,  0.007893f,  0.006632f,  0.005371f,  0.004464f,  0.003557f, 0.0030945f,
        0.002632f, 0.0025675f, 0.002503f,  0.0024945f,  0.002486f,  0.0025155f,  0.002545f,  0.002665f,  0.002785f,  0.0027235f, 0.002662f, 0.0027025f,
        0.002743f, 0.0030155f, 0.003288f,  0.003867f,   0.004446f,  0.006651f,   0.008856f,  0.012518f,  0.01618f,   0.020175f,  0.02417f,  0.02563f,
        0.02709f,  0.02308f,   0.01907f,   0.0136865f,  0.008303f,  0.006957f,   0.005611f,  0.0063045f, 0.006998f,  0.008584f,  0.01017f,  0.011135f,
        0.0121f,   0.0106205f, 0.009141f,  0.006401f,   0.003661f,  0.003178f,   0.002695f,  0.002812f,  0.002929f,  0.0027945f, 0.00266f,  0.002858f,
        0.003056f, 0.0031515f, 0.003247f,  0.0034115f,  0.003576f,  0.003427f,   0.003278f,  0.0034455f, 0.003613f,  0.0036285f, 0.003644f, 0.003367f,
        0.00309f,  0.003232f,  0.003374f,  0.003267f,   0.00316f,   0.0031495f,  0.003139f,  0.003062f,  0.002985f,  0.003139f,  0.003293f, 0.0033135f,
        0.003334f, 0.00345f,   0.003566f,  0.003647f,   0.003728f,  0.003857f,   0.003986f,  0.0040515f, 0.004117f,  0.004197f,  0.004277f, 0.004226f,
        0.004175f, 0.0045645f, 0.004954f,  0.0048685f,  0.004783f,  0.005318f,   0.005853f,  0.005788f,  0.005723f,  0.00567f,   0.005617f, 0.00546f,
        0.005303f, 0.005252f,  0.005201f,  0.005183f,   0.005165f,  0.0051595f,  0.005154f,  0.0052175f, 0.005281f,  0.00549f,   0.005699f, 0.0062505f,
        0.006802f, 0.007529f,  0.008256f,  0.0090055f,  0.009755f,  0.0101475f,  0.01054f,   0.010745f,  0.01095f,   0.010137f,  0.009324f, 0.0084725f,
        0.007621f, 0.0068735f, 0.006126f,  0.0055815f,  0.005037f,  0.0046315f,  0.004226f,  0.003881f,  0.003536f,  0.0032265f, 0.002917f, 0.002516f,
        0.002115f, 0.0019205f, 0.001726f,  0.001647f,   0.001568f,  0.0014765f,  0.001385f,  0.001294f,  0.001203f,  0.001205f,  0.001207f, 0.0012145f,
        0.001222f, 0.0011705f, 0.001119f,  0.0011195f,  0.00112f,   0.001082f,   0.001044f,  0.0010645f, 0.001085f,  0.001156f,  0.001227f, 0.001233f,
        0.001239f, 0.00135f,   0.001461f,  0.001648f,   0.001835f,  0.001667f,   0.001499f,  0.001511f,  0.001523f,  0.0014645f, 0.001406f, 0.0013125f,
        0.001219f, 0.0006095f, 0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,       0.0f,       0.0f,       0.0f,      0.0f,
        0.0f,      0.0f,       0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,       0}},
      {"G",{
        0.0f,       0.0f,        0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,
        0.0f,       0.0f,        0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.001543f,   0.003086f,  0.0035565f,  0.004027f,  0.0031075f,
        0.002188f,  0.0022535f,  0.002319f,  0.001899f,  0.001479f,  0.0014535f,  0.001428f,  0.00112715f, 0.0008263f, 0.00098315f, 0.00114f,   0.001191f,
        0.001242f,  0.0011965f,  0.001151f,  0.001195f,  0.001239f,  0.00119f,    0.001141f,  0.001164f,   0.001187f,  0.001289f,   0.001391f,  0.0016425f,
        0.001894f,  0.0021145f,  0.002335f,  0.002574f,  0.002813f,  0.0032595f,  0.003706f,  0.0046255f,  0.005545f,  0.0076825f,  0.00982f,   0.013835f,
        0.01785f,   0.02064f,    0.02343f,   0.026765f,  0.0301f,    0.03563f,    0.04116f,   0.050415f,   0.05967f,   0.078915f,   0.09816f,   0.12738f,
        0.1566f,    0.1924f,     0.2282f,    0.2739f,    0.3196f,    0.36855f,    0.4175f,    0.4726f,     0.5277f,    0.5928f,     0.6579f,    0.71385f,
        0.7698f,    0.8005f,     0.8312f,    0.83785f,   0.8445f,    0.8575f,     0.8705f,    0.88935f,    0.9082f,    0.9271f,     0.946f,     0.96395f,
        0.9819f,    0.99095f,    1.0f,       0.9967f,    0.9934f,    0.9876f,     0.9818f,    0.97545f,    0.9691f,    0.97325f,    0.9774f,    0.98215f,
        0.9869f,    0.97605f,    0.9652f,    0.93155f,   0.8979f,    0.84445f,    0.791f,     0.72305f,    0.6551f,    0.56865f,    0.4822f,    0.3973f,
        0.3124f,    0.25785f,    0.2033f,    0.17055f,   0.1378f,    0.11394f,    0.09008f,   0.072895f,   0.05571f,   0.046095f,   0.03648f,   0.032155f,
        0.02783f,   0.026995f,   0.02616f,   0.02677f,   0.02738f,   0.02745f,    0.02752f,   0.02585f,    0.02418f,   0.02138f,    0.01858f,   0.016675f,
        0.01477f,   0.01398f,    0.01319f,   0.013215f,  0.01324f,   0.01321f,    0.01318f,   0.01241f,    0.01164f,   0.0103975f,  0.009155f,  0.0076035f,
        0.006052f,  0.005198f,   0.004344f,  0.003775f,  0.003206f,  0.0030305f,  0.002855f,  0.002863f,   0.002871f,  0.0034605f,  0.00405f,   0.0051125f,
        0.006175f,  0.006901f,   0.007627f,  0.007632f,  0.007637f,  0.006665f,   0.005693f,  0.004709f,   0.003725f,  0.003184f,   0.002643f,  0.002404f,
        0.002165f,  0.002389f,   0.002613f,  0.0028265f, 0.00304f,   0.0029335f,  0.002827f,  0.0029585f,  0.00309f,   0.003231f,   0.003372f,  0.0036625f,
        0.003953f,  0.003888f,   0.003823f,  0.004107f,  0.004391f,  0.004264f,   0.004137f,  0.003973f,   0.003809f,  0.003648f,   0.003487f,  0.003354f,
        0.003221f,  0.0031325f,  0.003044f,  0.0030325f, 0.003021f,  0.0030705f,  0.00312f,   0.0031665f,  0.003213f,  0.0032575f,  0.003302f,  0.0035005f,
        0.003699f,  0.0040635f,  0.004428f,  0.004754f,  0.00508f,   0.0059895f,  0.006899f,  0.0073485f,  0.007798f,  0.008437f,   0.009076f,  0.0091385f,
        0.009201f,  0.0091225f,  0.009044f,  0.0088315f, 0.008619f,  0.0082995f,  0.00798f,   0.00779f,    0.0076f,    0.007548f,   0.007496f,  0.007548f,
        0.0076f,    0.007711f,   0.007822f,  0.0079615f, 0.008101f,  0.008127f,   0.008153f,  0.007961f,   0.007769f,  0.007238f,   0.006707f,  0.0061495f,
        0.005592f,  0.005117f,   0.004642f,  0.0042505f, 0.003859f,  0.0034755f,  0.003092f,  0.00288f,    0.002668f,  0.0023465f,  0.002025f,  0.0018485f,
        0.001672f,  0.001577f,   0.001482f,  0.001362f,  0.001242f,  0.001247f,   0.001252f,  0.001152f,   0.001052f,  0.00099725f, 0.0009425f, 0.0009543f,
        0.0009661f, 0.00091635f, 0.0008666f, 0.0008281f, 0.0007896f, 0.00083065f, 0.0008717f, 0.00088005f, 0.0008884f, 0.00092515f, 0.0009619f, 0.00103995f,
        0.001118f,  0.001196f,   0.001274f,  0.0011625f, 0.001051f,  0.0010565f,  0.001062f,  0.00096085f, 0.0008597f, 0.00091815f, 0.0009766f, 0.0009189f,
        0.0008612f, 0.0004306f,  0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,
        0.0f,       0.0f,        0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.0f,        0.0f,}},
      {"R",{
        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,
        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0018575f,  0.003715f,  0.003079f,   0.002443f,  0.0020455f,
        0.001648f,  0.0017865f,  0.001925f,  0.001553f,   0.001181f,  0.001127f,   0.001073f,  0.0009881f,  0.0009032f, 0.00093675f, 0.0009703f, 0.00094635f,
        0.0009224f, 0.0011177f,  0.001313f,  0.000995f,   0.000677f,  0.0009465f,  0.001216f,  0.0012365f,  0.001257f,  0.001289f,   0.001321f,  0.0012365f,
        0.001152f,  0.00107375f, 0.0009955f, 0.00125725f, 0.001519f,  0.001803f,   0.002087f,  0.002445f,   0.002803f,  0.0029485f,  0.003094f,  0.002991f,
        0.002888f,  0.0028545f,  0.002821f,  0.002901f,   0.002981f,  0.0032205f,  0.00346f,   0.003574f,   0.003688f,  0.003422f,   0.003156f,  0.0026165f,
        0.002077f,  0.002126f,   0.002175f,  0.0022785f,  0.002382f,  0.002534f,   0.002686f,  0.0028785f,  0.003071f,  0.003181f,   0.003291f,  0.0033965f,
        0.003502f,  0.003615f,   0.003728f,  0.004149f,   0.00457f,   0.00494f,    0.00531f,   0.00563f,    0.00595f,   0.0061365f,  0.006323f,  0.006458f,
        0.006593f,  0.0067545f,  0.006916f,  0.007343f,   0.00777f,   0.008377f,   0.008984f,  0.009607f,   0.01023f,   0.01101f,    0.01179f,   0.01283f,
        0.01387f,   0.015775f,   0.01768f,   0.020725f,   0.02377f,   0.028595f,   0.03342f,   0.03993f,    0.04644f,   0.054755f,   0.06307f,   0.07513f,
        0.08719f,   0.107645f,   0.1281f,    0.16655f,    0.205f,     0.26755f,    0.3301f,    0.41305f,    0.496f,     0.579f,      0.662f,     0.7337f,
        0.8054f,    0.8447f,     0.884f,     0.8976f,     0.9112f,    0.9265f,     0.9418f,    0.9608f,     0.9798f,    0.9878f,     0.9958f,    0.9979f,
        1.0f,       0.99615f,    0.9923f,    0.97235f,    0.9524f,    0.9125f,     0.8726f,    0.8128f,     0.753f,     0.6767f,     0.6004f,    0.51195f,
        0.4235f,    0.35205f,    0.2806f,    0.22285f,    0.1651f,    0.130405f,   0.09571f,   0.07443f,    0.05315f,   0.041695f,   0.03024f,   0.026095f,
        0.02195f,   0.021015f,   0.02008f,   0.020405f,   0.02073f,   0.021105f,   0.02148f,   0.02055f,    0.01962f,   0.01719f,    0.01476f,   0.0121415f,
        0.009523f,  0.0080775f,  0.006632f,  0.0058285f,  0.005025f,  0.004769f,   0.004513f,  0.004338f,   0.004163f,  0.004345f,   0.004527f,  0.0047955f,
        0.005064f,  0.005541f,   0.006018f,  0.0063415f,  0.006665f,  0.006433f,   0.006201f,  0.005328f,   0.004455f,  0.003759f,   0.003063f,  0.002884f,
        0.002705f,  0.00257f,    0.002435f,  0.002453f,   0.002471f,  0.0024775f,  0.002484f,  0.0024255f,  0.002367f,  0.0024915f,  0.002616f,  0.00253f,
        0.002444f,  0.002528f,   0.002612f,  0.0025575f,  0.002503f,  0.00255f,    0.002597f,  0.002551f,   0.002505f,  0.002426f,   0.002347f,  0.0023605f,
        0.002374f,  0.0022935f,  0.002213f,  0.002167f,   0.002121f,  0.002053f,   0.001985f,  0.001928f,   0.001871f,  0.00194f,    0.002009f,  0.002071f,
        0.002133f,  0.002293f,   0.002453f,  0.0026255f,  0.002798f,  0.0027925f,  0.002787f,  0.002651f,   0.002515f,  0.0020235f,  0.001532f,  0.001344f,
        0.001156f,  0.001118f,   0.00108f,   0.0010297f,  0.0009794f, 0.0009771f,  0.0009748f, 0.0010049f,  0.001035f,  0.00092405f, 0.0008131f, 0.00082065f,
        0.0008282f, 0.0007723f,  0.0007164f, 0.00070735f, 0.0006983f, 0.00070225f, 0.0007062f, 0.0006895f,  0.0006728f, 0.0006287f,  0.0005846f, 0.0006068f,
        0.000629f,  0.00065395f, 0.0006789f, 0.00071925f, 0.0007596f, 0.000719f,   0.0006784f, 0.00066125f, 0.0006441f, 0.0007476f,  0.0008511f, 0.0008908f,
        0.0009305f, 0.0008436f,  0.0007567f, 0.0007948f,  0.0008329f, 0.000872f,   0.0009111f, 0.00089545f, 0.0008798f, 0.0007618f,  0.0006438f, 0.00073805f,
        0.0008323f, 0.00041615f, 0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,
        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f,       0.0f,        0.0f}},
      {"N",{
        0.0f,      6.72e-11f,  1.344e-10f, 2.038e-10f, 2.732e-10f, 3.4475e-10f, 4.163e-10f, 4.901e-10f, 5.639e-10f, 0.0007045f, 0.001409f, 0.002135f,
        0.002861f, 0.0036085f, 0.004356f,  0.0051255f, 0.005895f,  0.006112f,   0.006329f,  0.0061655f, 0.006002f,  0.005693f,  0.005384f, 0.0044505f,
        0.003517f, 0.0028125f, 0.002108f,  0.002272f,  0.002436f,  0.002068f,   0.0017f,    0.0019855f, 0.002271f,  0.0020705f, 0.00187f,  0.0017455f,
        0.001621f, 0.0017855f, 0.00195f,   0.0018905f, 0.001831f,  0.0016705f,  0.00151f,   0.0015045f, 0.001499f,  0.001721f,  0.001943f, 0.0018305f,
        0.001718f, 0.002082f,  0.002446f,  0.002492f,  0.002538f,  0.002833f,   0.003128f,  0.003589f,  0.00405f,   0.0043915f, 0.004733f, 0.0047405f,
        0.004748f, 0.004824f,  0.0049f,    0.004541f,  0.004182f,  0.004313f,   0.004444f,  0.0046115f, 0.004779f,  0.005007f,  0.005235f, 0.005396f,
        0.005557f, 0.0054025f, 0.005248f,  0.0047195f, 0.004191f,  0.0035905f,  0.00299f,   0.0028175f, 0.002645f,  0.0026115f, 0.002578f, 0.002558f,
        0.002538f, 0.0027f,    0.002862f,  0.002721f,  0.00258f,   0.002973f,   0.003366f,  0.004324f,  0.005282f,  0.006576f,  0.00787f,  0.008871f,
        0.009872f, 0.010051f,  0.01023f,   0.0090465f, 0.007863f,  0.006367f,   0.004871f,  0.0048565f, 0.004842f,  0.0055935f, 0.006345f, 0.0069985f,
        0.007652f, 0.0077885f, 0.007925f,  0.007197f,  0.006469f,  0.0054185f,  0.004368f,  0.004082f,  0.003796f,  0.003768f,  0.00374f,  0.0036695f,
        0.003599f, 0.0036165f, 0.003634f,  0.0037785f, 0.003923f,  0.0042145f,  0.004506f,  0.004688f,  0.00487f,   0.0052705f, 0.005671f, 0.005878f,
        0.006085f, 0.006441f,  0.006797f,  0.007044f,  0.007291f,  0.007926f,   0.008561f,  0.0092905f, 0.01002f,   0.010625f,  0.01123f,  0.01174f,
        0.01225f,  0.012595f,  0.01294f,   0.01323f,   0.01352f,   0.01396f,    0.0144f,    0.01499f,   0.01558f,   0.01641f,   0.01724f,  0.01842f,
        0.0196f,   0.021835f,  0.02407f,   0.026755f,  0.02944f,   0.03189f,    0.03434f,   0.03751f,   0.04068f,   0.04483f,   0.04898f,  0.05445f,
        0.05992f,  0.066405f,  0.07289f,   0.081705f,  0.09052f,   0.10501f,    0.1195f,    0.141f,     0.1625f,    0.20245f,   0.2424f,   0.3015f,
        0.3606f,   0.4268f,    0.493f,     0.564f,     0.635f,     0.71665f,    0.7983f,    0.8726f,    0.9469f,    0.97345f,   1.0f,      0.99855f,
        0.9971f,   0.98365f,   0.9702f,    0.9526f,    0.935f,     0.91795f,    0.9009f,    0.88535f,   0.8698f,    0.86065f,   0.8515f,   0.84485f,
        0.8382f,   0.8352f,    0.8322f,    0.8289f,    0.8256f,    0.823f,      0.8204f,    0.8137f,    0.807f,     0.7957f,    0.7844f,   0.76695f,
        0.7495f,   0.72675f,   0.704f,     0.6911f,    0.6782f,    0.6638f,     0.6494f,    0.6313f,    0.6132f,    0.59005f,   0.5669f,   0.5493f,
        0.5317f,   0.5117f,    0.4917f,    0.4644f,    0.4371f,    0.3943f,     0.3515f,    0.3134f,    0.2753f,    0.24845f,   0.2216f,   0.191f,
        0.1604f,   0.1385f,    0.1166f,    0.1003f,    0.084f,     0.07559f,    0.06718f,   0.054015f,  0.04085f,   0.030665f,  0.02048f,  0.017005f,
        0.01353f,  0.01178f,   0.01003f,   0.0089005f, 0.007771f,  0.007077f,   0.006383f,  0.0058745f, 0.005366f,  0.0049605f, 0.004555f, 0.0040195f,
        0.003484f, 0.0031455f, 0.002807f,  0.002621f,  0.002435f,  0.0023315f,  0.002228f,  0.002158f,  0.002088f,  0.001973f,  0.001858f, 0.001839f,
        0.00182f,  0.0018615f, 0.001903f,  0.001759f,  0.001615f,  0.0016735f,  0.001732f,  0.0017285f, 0.001725f,  0.001884f,  0.002043f, 0.0020925f,
        0.002142f, 0.0020825f, 0.002023f,  0.001963f,  0.001903f,  0.0018045f,  0.001706f,  0.0017565f, 0.001807f,  0.0017525f, 0.001698f, 0.001849f,
        0.002f,    0.001f,     0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.0f,       0.0f,       0.0f,       0.0f,      0.0f,
        0.0f,      0.0f,       0.0f,       0.0f,       0.0f,       0.0f,        0.0f,       0.0f,       0.0f}}};

  otb::MetaData::LUT1D spectralSensitivity;

  spectralSensitivity.Axis[0].Origin = 0.3;
  spectralSensitivity.Axis[0].Spacing = 0.0025;
  spectralSensitivity.Axis[0].Size = 321;

  for (auto & band: m_Imd.Bands)
  {
    auto SpectralSensitivityIt = BandNameToSpectralSensitivityTable.find(band[MDStr::BandName] );
    if (SpectralSensitivityIt != BandNameToSpectralSensitivityTable.end())
    {
      spectralSensitivity.Array = SpectralSensitivityIt->second;

      band.Add(MDL1D::SpectralSensitivity, spectralSensitivity);
    }
  }
}

void WorldView2ImageMetadataInterface::FetchSolarIrradianceQuickBird()
{
  if(m_Imd[MDStr::SensorID] != "QB02")
  {
    itkExceptionMacro(<< "Invalid Metadata, not a Quickbird product");
  }

  auto productType = m_Imd[MDStr::ProductType];

  std::string              panchro("P");
  std::string              multi("Multi");

  if (productType == panchro)
  {
    m_Imd.Bands[0].Add(MDNum::SolarIrradiance, 1381.79);
  }
  else
  {
    const std::unordered_map<std::string, double> BandNameToSolarIrradianceTable{
       {"B", 1924.59},
       {"G", 1843.08},
       {"R", 1574.77},
       {"N", 1113.71}};

    for (auto & band: m_Imd.Bands)
    {
      auto solarIrradianceIt = BandNameToSolarIrradianceTable.find(band[MDStr::BandName] );
      if (solarIrradianceIt != BandNameToSolarIrradianceTable.end())
      {
        band.Add(MDNum::SolarIrradiance, solarIrradianceIt->second);
      }
      else
      {
        band.Add(MDNum::SolarIrradiance, 0.0);
      }
    }
  }
}

void WorldView2ImageMetadataInterface::FetchPhysicalGainQuickBird(int bitsPerPixel, 
                                                                  const std::unordered_map<std::string, double> & absCalFactor,
                                                                  const std::unordered_map<std::string, int> & TDILevels)
{
  if(m_Imd[MDStr::SensorID] != "QB02")
  {
    itkExceptionMacro(<< "Invalid Metadata, not a Quickbird product");
  }

  bool isPost20030606 = false;

  otb::MetaData::Time date;
  date.tm_year = 101;
  date.tm_mon = 0;
  date.tm_mday = 22;
  date.tm_hour = 0;
  date.tm_min = 0;
  date.tm_sec = 0;
  date.frac_sec = 0;

  if (m_Imd[MDTime::ProductionDate] > date)
  {
    isPost20030606 = true;
  }

  if ((bitsPerPixel != 16 && bitsPerPixel != 8))
  {
    itkExceptionMacro(<< "Invalid bitsPerPixel " << bitsPerPixel);
  }

  auto productType = m_Imd[MDStr::ProductType];

  std::string panchro("P");
  std::string multi("Multi");

  if (productType != panchro && productType != multi)
  {
    itkExceptionMacro(<< "Invalid bandID " << productType);
  }

  std::unordered_map<std::string, double> BandNameToPysicalGainNum = {
    {"P", 0.398},
    {"B", 0.068},
    {"G", 0.099},
    {"R", 0.071},
    {"N", 0.114}};

  std::unordered_map<std::string, double> BandNameToPysicalGainDen;

  // In this case the, absCalFactor are correct (k factor)
  if (isPost20030606)
  {
    if (productType == panchro)
    {
      BandNameToPysicalGainDen["P"] = absCalFactor.at("P");
    }
    else
    {
      BandNameToPysicalGainDen["B"] = absCalFactor.at("B");
      BandNameToPysicalGainDen["G"] = absCalFactor.at("G");
      BandNameToPysicalGainDen["R"] = absCalFactor.at("R");
      BandNameToPysicalGainDen["N"] = absCalFactor.at("N");
    }
  }
  // k factor should be adapted
  else
  {
    // 16 bits products. The revised K factor is used instead of the original absCalFactors
    if (bitsPerPixel == 16)
    {
      if (productType == panchro)
      {
        auto TDILevel = TDILevels.at("P");

        if (TDILevel == 10) 
          BandNameToPysicalGainDen["P"] = 0.08381880;
        else if (TDILevel == 13) 
          BandNameToPysicalGainDen["P"] = 0.06447600;
        else if (TDILevel == 18) 
          BandNameToPysicalGainDen["P"] = 0.04656600;
        else if (TDILevel == 24) 
          BandNameToPysicalGainDen["P"] = 0.03494440;
        else if (TDILevel == 32)
          BandNameToPysicalGainDen["P"] = 0.02618840;
        else
        {
          otbGenericExceptionMacro(MissingMetadataException, "Invalid number of TDI in the Quickbird product")
        }
      }
      else
      {
        BandNameToPysicalGainDen["B"] = 0.01604120;
        BandNameToPysicalGainDen["G"] = 0.01438470;
        BandNameToPysicalGainDen["R"] = 0.01267350;
        BandNameToPysicalGainDen["N"] = 0.01542420;
      }
    }
    // 8 bit product case
    else
    {
      if (productType == panchro)
      {
        auto TDILevel = TDILevels.at("P");

        if (TDILevel == 10) 
          BandNameToPysicalGainDen["P"] = 1.02681367;
        else if (TDILevel == 13) 
          BandNameToPysicalGainDen["P"] = 1.02848939;
        else if (TDILevel == 18) 
          BandNameToPysicalGainDen["P"] = 1.02794702;
        else if (TDILevel == 24) 
          BandNameToPysicalGainDen["P"] = 1.02989685;
        else if (TDILevel == 32)
          BandNameToPysicalGainDen["P"] = 1.02739898;
        else
        {
          otbGenericExceptionMacro(MissingMetadataException, "Invalid number of TDI in the Quickbird product")
        }
        BandNameToPysicalGainDen["P"] *= absCalFactor.at("P");
      }
      else
      {
        BandNameToPysicalGainDen["B"] = 1.12097834 * absCalFactor.at("B");
        BandNameToPysicalGainDen["G"] = 1.37652632 * absCalFactor.at("G");
        BandNameToPysicalGainDen["R"] = 1.30954587 * absCalFactor.at("R");
        BandNameToPysicalGainDen["N"] = 0.98368622 * absCalFactor.at("N");
      }
    }
  }

  for (auto & band: m_Imd.Bands)
  {
    const auto & bandName = band[MDStr::BandName];
    double physicalGain = BandNameToPysicalGainNum[bandName] / BandNameToPysicalGainDen[bandName];
    band.Add(MDNum::PhysicalGain, physicalGain);
  }

}

namespace
{
  // Worldview 2 String metadata parsed by GDAL are quoted ("md")
  // this helper function removes the quotes
  void Unquote(std::string & s)
  {
    if ( s.front() == '"' ) {
      s.erase( 0, 1 ); // erase the first character
      s.erase( s.size() - 1 ); // erase the last character
    }
  }

  struct DigitalGlobeMetadata
  {
    std::string sensorId;
    std::string bandId;
    
    std::string acquisitionDateStr;
    std::string productionDateStr;

    double sunElevation;
    double sunAzimuth;
    double satElevation;
    double satAzimuth;
    
    int bitsPerPixel;
    
    std::vector<std::string> bandNameList;
    std::unordered_map<std::string, double> absCalFactor;
    std::unordered_map<std::string, int> TDILevel;
    std::unordered_map<std::string, double> effectiveBandwidth;
  };

  void ParseGeomMetadata(const MetadataSupplierInterface & mds, DigitalGlobeMetadata & outMetadata)
  {
    outMetadata.sensorId = mds.GetAs<std::string>("sensor");
    outMetadata.bandId = mds.GetAs<std::string>("support_data.band_id");
    
    // the date might have a semicolon at its end in the geom, e.g.
    // support_data.tlc_date:  2009-12-10T10:30:18.142149Z;
    outMetadata.acquisitionDateStr = mds.GetAs<std::string>("support_data.tlc_date");
    if (outMetadata.acquisitionDateStr.back() == ';')
    {
      outMetadata.acquisitionDateStr.pop_back();
    }

    outMetadata.productionDateStr = mds.GetAs<std::string>("support_data.generation_date");
    
    outMetadata.sunElevation = mds.GetAs<double>("support_data.elevation_angle");
    outMetadata.sunAzimuth = mds.GetAs<double>("support_data.azimuth_angle");
    outMetadata.satElevation = mds.GetAs<double>("support_data.sat_elevation_angle");
    outMetadata.satAzimuth = mds.GetAs<double>("support_data.sat_azimuth_angle");

    outMetadata.bitsPerPixel = mds.GetAs<int>("support_data.bits_per_pixel");
    outMetadata.bandNameList = mds.GetAsVector<std::string>("support_data.band_name_list");

    // In the case of WorldView-2, we need to divide the absolute calibration
    // factor by the effective bandwidth see for details:
    // http://www.digitalglobe.com/downloads/Radiometric_Use_of_WorldView-2_Imagery.pdf
    // These values are not written in the geom file by OSSIM as
    // there are specific to WV2 We did not retrieve those values in the ossim
    // class and consider them as constant values
    outMetadata.effectiveBandwidth = {
      {"P", 2.846000e-01},
      {"C", 4.730000e-02},
      {"B", 5.430000e-02},
      {"G", 6.300000e-02},
      {"Y", 3.740000e-02},
      {"R", 5.740000e-02},
      {"RE", 3.930000e-02},
      {"N", 9.890000e-02},
      {"N2", 9.960000e-02}
    };

    auto TDILevel = mds.GetAs<int>("support_data.TDI_level");

    for (const auto & bandName : outMetadata.bandNameList)
    {
      outMetadata.TDILevel[bandName] = TDILevel;
      outMetadata.absCalFactor[bandName] = mds.GetAs<double>(0., "support_data." + bandName + "_band_absCalFactor");
      if (outMetadata.absCalFactor[bandName] == 0.)
      {
        outMetadata.absCalFactor[bandName] = mds.GetAs<double>("support_data.absCalFactor");
      }
    }
  }

  void ParseProductMetadata(const MetadataSupplierInterface & mds, DigitalGlobeMetadata & outMetadata)
  {
    // Open the metadata file to find the band names, as it is not parsed by GDAL
    auto ressourceFiles = mds.GetResourceFiles();

    // return true if the file extension is .IMD
    auto lambda = [](const std::string & filename){return itksys::SystemTools::GetFilenameLastExtension(filename) == ".IMD";};
    auto filename = std::find_if(ressourceFiles.begin(), ressourceFiles.end(), lambda);
    if (filename == ressourceFiles.end())
    {
      otbGenericExceptionMacro(MissingMetadataException, 
        << "Cannot find the .IMD file associated with the Digital Globe dataset");
    }
  
    std::ifstream file(*filename);

    if (file.is_open()) 
    {
      // Find band names at group definitions : 
      // BEGIN_GROUP = BAND_N
      const auto key = "BEGIN_GROUP";
      const auto key_size = strlen(key);

      const auto prefix = "BAND_";
      const auto prefix_size = strlen(prefix);

      std::string line;
      while (std::getline(file, line)) 
      {
        if (starts_with(line, "BEGIN_GROUP"))
        {
          auto pos = line.find(prefix, key_size);
          if (pos != std::string::npos)
          {
            outMetadata.bandNameList.push_back(line.substr(pos+prefix_size, line.size()-pos-prefix_size));
          }
        }
      }
      file.close();
    }

    outMetadata.sensorId = mds.GetAs<std::string>("IMD/IMAGE_1.satId");
    outMetadata.bandId = mds.GetAs<std::string>("IMD/bandId");
    
    Unquote(outMetadata.sensorId);
    Unquote(outMetadata.bandId);

    // Define a default value in case metadata are not found
    auto noDataValue = std::numeric_limits<double>::min();

    outMetadata.sunElevation = mds.GetAs<double>(noDataValue, "IMD/IMAGE_1.meanSunEl");
    if (outMetadata.sunElevation == noDataValue)
    {
      outMetadata.sunElevation = mds.GetAs<double>("IMD/IMAGE_1.sunEl");
    }

    outMetadata.sunAzimuth = mds.GetAs<double>(noDataValue, "IMD/IMAGE_1.meanSunAz");
    if (outMetadata.sunAzimuth == noDataValue)
    {
      outMetadata.sunAzimuth = mds.GetAs<double>("IMD/IMAGE_1.sunAz");
    }

    outMetadata.satElevation = mds.GetAs<double>(noDataValue, "IMD/IMAGE_1.meanSatEl");
    if (outMetadata.satElevation == noDataValue)
    {
      outMetadata.satElevation = mds.GetAs<double>("IMD/IMAGE_1.satEl");
    }

    outMetadata.satAzimuth = mds.GetAs<double>(noDataValue, "IMD/IMAGE_1.meanSatAz");
    if (outMetadata.satAzimuth == noDataValue)
    {
      outMetadata.satAzimuth = mds.GetAs<double>("IMD/IMAGE_1.satAz");
    }

    outMetadata.productionDateStr = mds.GetAs<std::string>("IMD/generationTime");
    
    // Use TLC time in priority, only available for level 1B products.
    // If it is not available use firstLineTime instead.
    outMetadata.acquisitionDateStr = mds.GetAs<std::string>("", "IMAGE_1.TLCTime");
    if (outMetadata.acquisitionDateStr == "")
    {
      outMetadata.acquisitionDateStr = mds.GetAs<std::string>("IMD/IMAGE_1.firstLineTime");
    }

    outMetadata.bitsPerPixel = mds.GetAs<int>("IMD/bitsPerPixel");

    if (outMetadata.bitsPerPixel != 16)
    {
      otbGenericExceptionMacro(MissingMetadataException,<< "Invalid bitsPerPixel " << outMetadata.bitsPerPixel);
    }

    for (auto & bandName: outMetadata.bandNameList)
    {
      auto prefix = "IMD/BAND_" + bandName + ".";
      auto TDILevel = mds.GetAs<int>(-1, prefix + "TDILevel");
      if (TDILevel == -1)
      {
        TDILevel = mds.GetAs<int>("IMD/IMAGE_1.TDILevel");
      }
      outMetadata.TDILevel[bandName] = TDILevel;

      // Verify if TDI level is correct, according to the document 
      // Radiometric_Use_of_WorldView-2_Imagery, Technical Note, 2010 (Digital Globe)
      if ( (outMetadata.bandId == "P" && (TDILevel <8 || TDILevel > 64))
            || ( (outMetadata.bandId == "Multi" || outMetadata.bandId == "MS1" ) && (TDILevel <3 || TDILevel > 24)) )
      {
          otbGenericExceptionMacro(MissingMetadataException, 
            << "Invalid TDI settings for band " << bandName);
      }

      // In previous version of OTB the effective bandwith was tabulated
      // because Ossim did not read this specific metadata.
      outMetadata.effectiveBandwidth[bandName] = mds.GetAs<double>(prefix + "effectiveBandwidth");

      outMetadata.absCalFactor[bandName] = mds.GetAs<double>(prefix + "absCalFactor");

      
    }
  }

}

void WorldView2ImageMetadataInterface::Parse(const MetadataSupplierInterface & mds)
{
  DigitalGlobeMetadata metadata;

  // Check if the sensor is a digital globe sensor
  //if (mds.GetAs<std::string>("", "IMD/IMAGE_1.satId").find("WV02") != std::string::npos)
  if (mds.GetAs<std::string>("", "METADATATYPE") == "DG")
  {
    ParseProductMetadata(mds, metadata);
    FetchRPC(mds);
  }
  else if (mds.GetAs<std::string>("", "support_data.sat_id").find("WV02") != std::string::npos
           || mds.GetAs<std::string>("", "support_data.sat_id").find("QB02") != std::string::npos)
  {
    ParseGeomMetadata(mds, metadata);
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Not a Worldview 2 sensor")
  }

  // Product type (Multi, MS1 or P)
  m_Imd.Add(MDStr::ProductType, metadata.bandId);

  if (m_Imd.Bands.size() == metadata.bandNameList.size())
  {
    auto bandIt = m_Imd.Bands.begin();
    for (const auto & bandName : metadata.bandNameList)
    {
      bandIt->Add(MDStr::BandName, bandName);
      bandIt++;
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, 
      << "The Number of bands in the metadata is inconsistent with the number of bands of the image");
  }

  // Acquisition and production dates
  try
  {
    m_Imd.Add(MDTime::AcquisitionDate, 
                boost::lexical_cast<MetaData::Time>(metadata.acquisitionDateStr));
  }
  catch (boost::bad_lexical_cast&)
  {
    otbGenericExceptionMacro(MissingMetadataException,
            << "Invalid value for the acquisition date, got: "
            << metadata.acquisitionDateStr);
  }

  try
  {
    m_Imd.Add(MDTime::ProductionDate, 
                boost::lexical_cast<MetaData::Time>(metadata.productionDateStr));
  }
  catch (boost::bad_lexical_cast&)
  {
    otbGenericExceptionMacro(MissingMetadataException,
            << "Invalid value for the production date, got: "
            << metadata.productionDateStr);
  }

  //Radiometry
  m_Imd.Add(MDNum::SunElevation, metadata.sunElevation);
  m_Imd.Add(MDNum::SunAzimuth, metadata.sunAzimuth);
  m_Imd.Add(MDNum::SatElevation, metadata.satElevation);
  m_Imd.Add(MDNum::SatAzimuth, metadata.satAzimuth);

  //Physical Bias
  FetchPhysicalBias();
  
  if (metadata.sensorId == "WV02")
  {
    m_Imd.Add(MDStr::SensorID, "WV02");
    m_Imd.Add(MDStr::Mission, "WorldView-2");

    for (auto & band : m_Imd.Bands)
    {
      auto bandName = band[MDStr::BandName];
      band.Add(MDNum::PhysicalGain, metadata.effectiveBandwidth[bandName] / metadata.absCalFactor[bandName]);
    }

    FetchSolarIrradianceWorldView2();
    FetchWavelengthsWorldView2();
    FetchSpectralSensitivityWorldView2();
  }
  else if (metadata.sensorId == "QB02")
  {
    m_Imd.Add(MDStr::SensorID, "QB02");
    m_Imd.Add(MDStr::Mission, "Quickbird");

    FetchSolarIrradianceQuickBird();
    FetchPhysicalGainQuickBird(metadata.bitsPerPixel,
                               metadata.absCalFactor,
                               metadata.TDILevel);
    FetchSpectralSensitivityQuickBird();
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, 
      << "Unknown Digital Globe sensor:" << metadata.sensorId)
  }
}

} // end namespace otb
