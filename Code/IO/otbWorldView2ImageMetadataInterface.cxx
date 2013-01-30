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

#include "otbWorldView2ImageMetadataInterface.h"

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

WorldView2ImageMetadataInterface
::WorldView2ImageMetadataInterface()
{
}

bool
WorldView2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("WV02") != std::string::npos) return true;
  else return false;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface::GetSolarIrradiance() const
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
    std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    outputValuesVariableLengthVector.SetSize(bandNameList.size());
    for(unsigned int i = 0; i < bandNameList.size(); ++i)
      {
      double SolarIrradianceValue = 0.0;
      if(bandNameList[i] == "C")
        {
        SolarIrradianceValue = 1758.2229;
        }
      else if(bandNameList[i] == "B")
        {
        SolarIrradianceValue = 1974.2416;
        }
      else if(bandNameList[i] == "G")
        {
        SolarIrradianceValue = 1856.4104;
        }
      else if(bandNameList[i] == "Y")
        {
        SolarIrradianceValue = 1738.4791;
        }
      else if(bandNameList[i] == "R")
        {
        SolarIrradianceValue = 1559.4555;
        }
      else if(bandNameList[i] == "RE")
        {
        SolarIrradianceValue = 1342.0695;
        }
      else if(bandNameList[i] == "N")
        {
        SolarIrradianceValue = 1069.7302;
        }
      else if(bandNameList[i] == "N2")
        {
        SolarIrradianceValue = 861.2866;
        }
      outputValuesVariableLengthVector[i] = SolarIrradianceValue;
      }
    }

  return outputValuesVariableLengthVector;
}

int
WorldView2ImageMetadataInterface::GetDay() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetMonth() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetYear() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetHour() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[3].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetMinute() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[4].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetProductionDay() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetProductionMonth() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
WorldView2ImageMetadataInterface::GetProductionYear() const
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

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetPhysicalBias() const
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
  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  std::string ms1("MS1");

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

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetPhysicalGain() const
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
  int         bitsPerPixel = atoi(keywordStringBitsPerPixel.c_str());
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
  int         TDILevel = atoi(keywordStringTDILevel.c_str());
  if ( (keywordStringBId == panchro && TDILevel > 64) ||
      (keywordStringBId == panchro && TDILevel < 8) )
    {
    itkExceptionMacro(<< "Invalid TDILevel " << TDILevel);
    }

  std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
  std::vector<std::string> bandNameList;
  boost::trim(keywordStringBandNameList);
  boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(bandNameList.size());

  if (keywordStringBId == panchro)
    {
    std::string keywordStringAbsCalFactor = imageKeywordlist.GetMetadataByKey("support_data.absCalFactor");
    double absCalFactor = atof(keywordStringAbsCalFactor.c_str());
    outputValuesVariableLengthVector[0] = absCalFactor;
    }
  else
    {
    for(unsigned int i = 0; i < bandNameList.size(); ++i)
      {
      std::string key = "support_data." + bandNameList[i] + "_band_absCalFactor";
      std::string keywordStringCalFactor = imageKeywordlist.GetMetadataByKey(key);
      double calFactor = atof(keywordStringCalFactor.c_str());
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

    const double effectiveBandWidthPan = 2.846000e-01;
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

    for(unsigned int i = 0; i < bandNameList.size(); ++i)
      {
      outputValuesVariableLengthVector[i] = effectiveBandwidthVariableLengthVector[i] / outputValuesVariableLengthVector[i];
      }
    }

  return outputValuesVariableLengthVector;
}

double
WorldView2ImageMetadataInterface::GetSatElevation() const
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
  double value = atof(valueString.c_str());
  return value;
}

double
WorldView2ImageMetadataInterface::GetSatAzimuth() const
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
  double value = atof(valueString.c_str());
  return value;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetFirstWavelengths() const
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
    std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    wavel.SetSize(bandNameList.size());
    for(unsigned int i = 0; i < bandNameList.size(); ++i)
      {
      double waveValue = 0.0;
      if(bandNameList[i] == "C")
        {
        waveValue = 0.401;
        }
      else if(bandNameList[i] == "B")
        {
        waveValue = 0.447;
        }
      else if(bandNameList[i] == "G")
        {
        waveValue = 0.511;
        }
      else if(bandNameList[i] == "Y")
        {
        waveValue = 0.588;
        }
      else if(bandNameList[i] == "R")
        {
        waveValue = 0.629;
        }
      else if(bandNameList[i] == "RE")
        {
        waveValue = 0.704;
        }
      else if(bandNameList[i] == "N")
        {
        waveValue = 0.772;
        }
      else if(bandNameList[i] == "N2")
        {
        waveValue = 0.862;
        }
      wavel[i] = waveValue;
      }
    }

  return wavel;
}

WorldView2ImageMetadataInterface::VariableLengthVectorType
WorldView2ImageMetadataInterface
::GetLastWavelengths() const
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
    std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey("support_data.band_name_list");
    std::vector<std::string> bandNameList;
    boost::trim(keywordStringBandNameList);
    boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

    wavel.SetSize(bandNameList.size());
    for(unsigned int i = 0; i < bandNameList.size(); ++i)
      {
      double waveValue = 0.0;
      if(bandNameList[i] == "C")
        {
        waveValue = 0.453;
        }
      else if(bandNameList[i] == "B")
        {
        waveValue = 0.581;
        }
      else if(bandNameList[i] == "G")
        {
        waveValue = 0.581;
        }
      else if(bandNameList[i] == "Y")
        {
        waveValue = 0.627;
        }
      else if(bandNameList[i] == "R")
        {
        waveValue = 0.689;
        }
      else if(bandNameList[i] == "RE")
        {
        waveValue = 0.744;
        }
      else if(bandNameList[i] == "N")
        {
        waveValue = 0.890;
        }
      else if(bandNameList[i] == "N2")
        {
        waveValue = 0.954;
        }
      wavel[i] = waveValue;
      }
    }
  return wavel;
}

std::vector<unsigned int>
WorldView2ImageMetadataInterface::GetDefaultDisplay() const
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

  std::string key = "support_data.band_name_list";
  std::vector<unsigned int> rgb(3);
  if (imageKeywordlist.HasKey(key))
    {
    std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey(key);
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

std::vector<std::string>
WorldView2ImageMetadataInterface
::GetEnhancedBandNames() const
{
  std::vector<std::string> enhBandNames;
  std::vector<std::string> rawBandNames = this->Superclass::GetBandName();

  if(rawBandNames.size())
    {
    for (std::vector<std::string>::iterator it = rawBandNames.begin(); it != rawBandNames.end(); ++it)
      {
      // Manage Panchro case
      if ( (rawBandNames.size() == 1) && !(*it).compare("P") )
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
      if ( !(*it).compare("C") )
        {
        enhBandNames.push_back("Costal");
        }
      else if ( !(*it).compare("B") )
        {
        enhBandNames.push_back("Blue");
        }
      else if ( !(*it).compare("G") )
        {
        enhBandNames.push_back("Green");
        }
      else if ( !(*it).compare("Y") )
        {
        enhBandNames.push_back("Yellow");
        }
      else if ( !(*it).compare("R") )
        {
        enhBandNames.push_back("Red");
        }
      else if ( !(*it).compare("RE") )
        {
        enhBandNames.push_back("RedEdge");
        }
      else if ( !(*it).compare("N") )
        {
        enhBandNames.push_back("NIR1");
        }
      else if ( !(*it).compare("N2") )
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

WorldView2ImageMetadataInterface::WavelengthSpectralBandVectorType
WorldView2ImageMetadataInterface
::GetSpectralSensitivity()  const
{
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::list <std::vector<float> > tmpSpectralBandList;

  //Read sensor image type (panchromatic or multispectral)
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
    const float b0[301] =
    {
     5.53407e-06,   6.18974e-06,   6.77586e-06,   4.47741e-06,   2.18462e-06,   2.35814e-06,
     2.74394e-06,   3.35619e-06,   4.12548e-06,   3.76678e-06,   2.97253e-06,   3.87911e-06,
     4.9452e-06,   4.23503e-06,   3.35741e-06,   3.66642e-06,   4.01401e-06,   5.01097e-06,
     6.02754e-06,   5.03579e-06,   3.95097e-06,   3.61214e-06,   3.26677e-06,   3.72693e-06,
     4.18733e-06,   5.20028e-06,   6.2917e-06,   4.84978e-06,   3.39534e-06,   6.07837e-06,
     8.8389e-06,   1.04941e-05,   1.21421e-05,   4.98988e-05,   8.81089e-05,   0.000272275,
     0.000459932,   0.00169883,   0.00295147,   0.0559008,   0.109766,   0.271344,
     0.436163,   0.461822,   0.487533,   0.494856,   0.504992,   0.515215,
     0.525596,   0.529589,   0.535583,   0.529756,   0.522233,   0.527207,
     0.53366,   0.528602,   0.523089,   0.541112,   0.55728,   0.55817,
     0.558232,   0.580899,   0.605533,   0.616208,   0.626792,   0.612118,
     0.595414,   0.612008,   0.630712,   0.649153,   0.671541,   0.663704,
     0.654733,   0.672532,   0.693098,   0.691335,   0.688237,   0.67829,
     0.664973,   0.683323,   0.70156,   0.705992,   0.713576,   0.709848,
     0.709786,   0.738422,   0.765752,   0.762756,   0.757827,   0.759162,
     0.759818,   0.779983,   0.794085,   0.797935,   0.800715,   0.811351,
     0.81929,   0.820814,   0.82303,   0.829794,   0.838869,   0.848913,
     0.860802,   0.858037,   0.858462,   0.865197,   0.870285,   0.877932,
     0.889278,   0.896475,   0.909253,   0.91308,   0.917885,   0.914221,
     0.910762,   0.916979,   0.92489,   0.93683,   0.950791,   0.943998,
     0.936911,   0.932311,   0.929417,   0.938274,   0.951232,   0.960012,
     0.962781,   0.957501,   0.953865,   0.954752,   0.957376,   0.968155,
     0.977735,   0.961332,   0.949217,   0.946955,   0.953334,   0.946656,
     0.952193,   0.96247,   0.969595,   0.983968,   1,   0.99432,
     0.994003,   0.980974,   0.969563,   0.968253,   0.968397,   0.973674,
     0.978855,   0.966847,   0.963741,   0.952252,   0.946654,   0.94592,
     0.952938,   0.954759,   0.962583,   0.962142,   0.956637,   0.950381,
     0.940962,   0.936224,   0.924596,   0.923379,   0.921684,   0.933568,
     0.940831,   0.957977,   0.963832,   0.958143,   0.941297,   0.92426,
     0.903,   0.895878,   0.887152,   0.910523,   0.929402,   0.744019,
     0.553867,   0.320246,   0.0858164,   0.0514292,   0.0172983,   0.0120102,
     0.00659981,   0.00436796,   0.00213134,   0.00153565,   0.000922227,   0.000721841,
     0.000528696,   0.000489722,   0.000449662,   0.000431783,   0.000409049,   0.000407132,
     0.000396445,   0.000420259,   0.000446526,   0.000403311,   0.000361257,   0.00033034,
     0.000298026,   0.000301128,   0.000308537,   0.000286243,   0.000261497,   0.000253604,
     0.000242266,   0.000274627,   0.000307033,   0.000295007,   0.000283703,   0.000261529,
     0.000237475,   0.00024833,   0.000258997,   0.00027011,   0.000282485,   0.000261654,
     0.000240819,   0.000248561,   0.00025802,   0.000273541,   0.000287576,   0.000282785,
     0.000276218,   0.000275306,   0.000273131,   0.000256767,   0.000242183,   0.000239187,
     0.000234252,   0.000223355,   0.000213509,   0.00021573,   0.000218602,   0.000204845,
     0.000191131,   0.000179884,   0.000169031,   0.000177057,   0.000183172,   0.00016757,
     0.0001513,   0.000138821,   0.000125806,   0.000129696,   0.000133722,   0.000122536,
     0.000111436,   0.000102899,   9.41714e-05,   0.000104721,   0.000115227,   9.94755e-05,
     8.3857e-05,   8.4497e-05,   8.52612e-05,   7.76124e-05,   7.00705e-05,   7.06672e-05,
     7.1081e-05,   6.5671e-05,   6.00304e-05,   6.21262e-05,   6.42018e-05,   5.57459e-05,
     4.73181e-05,   4.33172e-05,   3.91675e-05,   3.60807e-05,   3.29177e-05,   3.41277e-05,
     3.52334e-05,   3.14413e-05,   2.76314e-05,   2.55335e-05,   2.34256e-05,   2.1133e-05,
     1.88297e-05,   1.80004e-05,   1.71146e-05,   1.51355e-05,   1.31473e-05,   1.21372e-05,
     1.11225e-05,   9.97857e-06,   8.84718e-06,   8.76091e-06,   8.67416e-06,   7.95176e-06,
     7.22607e-06,   7.06298e-06,   6.8991e-06,   6.66214e-06,   6.42406e-06,   6.36617e-06,
     6.32615e-06
    };

    //add panchromatic band to the temporary list
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
    tmpSpectralBandList.push_back(vb0);
    }
  else
    {
    const float b1[301] =
    {
     1.93924e-06,   3.33156e-06,   3.2732e-06,   2.78199e-06,   1.7488e-06,   3.34801e-06,
     3.55785e-06,   4.38919e-06,   2.94112e-06,   6.21317e-06,   5.37163e-06,   5.00497e-06,
     7.41645e-06,   1.35004e-05,   4.53797e-05,   0.000173069,   0.000634651,   0.00341498,
     0.0274284,   0.132903,   0.355988,   0.59258,   0.700719,   0.739625,
     0.765307,   0.787842,   0.817088,   0.838687,   0.86178,   0.883513,
     0.905055,   0.917072,   0.927208,   0.947658,   0.965088,   0.979304,
     0.985921,   0.989094,   0.997236,   0.974613,   0.85394,   0.588032,
     0.281577,   0.0805215,   0.00894615,   0.00133307,   0.0003609,   0.000145392,
     7.12168e-05,   3.80253e-05,   1.15412e-05,   9.59192e-06,   5.14417e-06,   1.11808e-05,
     1.09998e-05,   1.07851e-05,   9.08862e-06,   1.0105e-05,   1.17289e-05,   6.3851e-06,
     1.30646e-06,   1.3751e-06,   8.62503e-07,   6.19172e-07,   7.02247e-07,   8.87865e-07,
     9.3924e-07,   1.11657e-06,   9.84522e-07,   7.45006e-07,   8.99363e-07,   1.24448e-06,
     3.62747e-06,   1.27768e-06,   4.00518e-07,   5.21386e-07,   2.69075e-07,   4.85233e-07,
     3.69503e-07,   1.25632e-05,   0.00014168,   2.39565e-06,   3.06503e-07,   2.7473e-07,
     5.19969e-07,   4.87974e-07,   2.69412e-07,   3.10803e-07,   6.51482e-08,   1.769e-10,
     2.06764e-07,   1.56239e-06,   1.71434e-06,   3.76485e-07,   9.78272e-08,   1.07281e-07,
     5.25843e-07,   2.86289e-06,   4.49334e-06,   2.7912e-06,   9.77366e-07,   1.65592e-06,
     1.25872e-06,   1.35006e-06,   2.26827e-06,   3.08804e-06,   6.08055e-06,   1.15782e-05,
     1.00862e-05,   5.55949e-06,   3.85934e-06,   3.17286e-06,   2.67182e-06,   3.11772e-06,
     2.48961e-06,   2.56662e-06,   2.69687e-06,   2.66657e-06,   2.49631e-06,   2.07413e-06,
     2.21763e-06,   1.82216e-06,   1.73999e-06,   1.79846e-06,   1.78097e-06,   2.08078e-06,
     2.41026e-06,   2.95564e-06,   4.37817e-06,   9.26286e-06,   1.71525e-05,   1.63404e-05,
     7.76378e-06,   4.20687e-06,   4.36152e-06,   4.1979e-06,   3.60385e-06,   4.21227e-06,
     6.61165e-06,   1.85337e-05,   2.63714e-05,   1.23596e-05,   8.08582e-06,   7.62016e-06,
     8.54114e-06,   9.63216e-06,   1.21937e-05,   2.92224e-05,   9.75796e-05,   9.35745e-05,
     3.33406e-05,   2.37882e-05,   2.9829e-05,   4.42465e-05,   6.68887e-05,   0.000152608,
     0.000422651,   0.000256325,   6.52584e-05,   4.13991e-05,   5.7842e-05,   0.000264601,
     0.000711195,   0.000441052,   8.93762e-05,   3.04976e-05,   1.31372e-05,   8.13006e-06,
     5.95634e-06,   5.94402e-06,   6.95574e-06,   1.12493e-05,   1.93408e-05,   3.30614e-05,
     5.526e-05,   0.000106194,   0.000246237,   0.000245793,   0.000116183,   6.90781e-05,
     0.000121558,   0.00012478,   0.000160506,   0.000195856,   0.000163724,   0.000116846,
     9.27976e-05,   7.97493e-05,   7.30327e-05,   6.2535e-05,   7.15964e-05,   6.92402e-05,
     6.98667e-05,   7.20625e-05,   5.92742e-05,   4.73751e-05,   5.11686e-05,   3.8765e-05,
     2.87346e-05,   2.82287e-05,   4.23112e-05,   2.84265e-05,   2.76262e-05,   3.13753e-05,
     3.20692e-05,   2.54603e-05,   1.55049e-05,   1.67992e-05,   1.51677e-05,   1.72863e-05,
     1.82755e-05,   1.62912e-05,   1.63329e-05,   2.11384e-05,   1.68083e-05,   1.32225e-05,
     9.90909e-06,   9.57385e-06,   9.22475e-06,   1.59785e-05,   1.89273e-05,   1.94756e-05,
     1.68079e-05,   1.52813e-05,   1.45048e-05,   1.12089e-05,   9.50048e-06,   7.40732e-06,
     6.16214e-06,   4.66982e-06,   4.04122e-06,   3.96966e-06,   3.02326e-06,   3.30965e-06,
     2.53001e-06,   3.00426e-06,   3.01337e-06,   3.36385e-06,   3.56402e-06,   4.18688e-06,
     4.12602e-06,   5.01737e-06,   5.44329e-06,   5.985e-06,   5.40637e-06,   6.44899e-06,
     5.42357e-06,   4.91412e-06,   4.3504e-06,   3.89253e-06,   3.67736e-06,   4.08168e-06,
     3.85234e-06,   3.99802e-06,   4.60479e-06,   5.29422e-06,   4.87849e-06,   4.55674e-06,
     4.24992e-06,   3.52154e-06,   3.22953e-06,   2.58855e-06,   2.42857e-06,   2.34923e-06,
     2.36014e-06,   2.33549e-06,   2.55772e-06,   3.03473e-06,   3.14355e-06,   3.65574e-06,
     3.70734e-06,   3.68159e-06,   3.81222e-06,   3.35656e-06,   3.062e-06,   2.69374e-06,
     2.45185e-06,   2.09096e-06,   1.87615e-06,   1.59947e-06,   1.51572e-06,   1.47543e-06,
     1.5459e-06,   1.6819e-06,   1.89924e-06,   2.46062e-06,   2.89706e-06,   3.43049e-06,
     4.07493e-06,   4.31785e-06,   4.56185e-06,   4.84605e-06,   4.70059e-06,   5.04519e-06,
     5.03717e-06,   5.58133e-06,   5.772e-06,   6.2806e-06,   6.83109e-06,   7.80214e-06,
     8.13898e-06
    };

    const float b2[301] =
    {
     7.50196e-07,   3.64689e-07,   3.0422e-07,   2.19926e-07,   2.95025e-07,   1.36813e-07,
     2.46454e-07,   3.07665e-07,   4.35207e-07,   4.54783e-07,   4.09999e-07,   4.6799e-07,
     4.30817e-07,   3.21329e-07,   5.14891e-07,   5.88871e-07,   7.24472e-07,   5.19291e-07,
     5.83071e-07,   7.385e-07,   2.80484e-06,   2.40132e-06,   1.65424e-06,   1.01535e-06,
     2.56678e-06,   6.15462e-06,   1.34813e-05,   2.75384e-05,   4.11764e-05,   5.15236e-05,
     7.01286e-05,   0.000133268,   0.000337419,   0.000957927,   0.00227712,   0.00543291,
     0.0197821,   0.0818229,   0.2452,   0.503309,   0.70097,   0.757605,
     0.778255,   0.788626,   0.8,   0.801737,   0.809859,   0.82107,
     0.826045,   0.830925,   0.842262,   0.862706,   0.878482,   0.876943,
     0.878267,   0.887613,   0.907576,   0.930559,   0.950192,   0.976801,
     1,   0.97894,   0.855779,   0.593873,   0.297629,   0.125661,
     0.0503684,   0.018199,   0.00573411,   0.00297352,   0.00165595,   0.000980273,
     0.000604892,   0.000391497,   0.000259234,   0.000168111,   0.00010894,   7.63081e-05,
     5.14226e-05,   5.2013e-05,   6.6689e-05,   6.23362e-05,   6.06577e-05,   5.93819e-05,
     6.25513e-05,   5.48608e-05,   5.86835e-05,   4.88126e-05,   4.93644e-05,   4.14711e-05,
     3.10164e-05,   2.97216e-05,   1.74405e-05,   1.74672e-05,   1.43447e-05,   1.09051e-05,
     1.27791e-05,   9.19063e-06,   8.62276e-06,   9.16788e-06,   8.06522e-06,   8.53885e-06,
     8.9662e-06,   7.58053e-06,   8.12959e-06,   9.11945e-06,   7.37798e-06,   7.16219e-06,
     7.98185e-06,   6.06857e-06,   5.53839e-06,   6.14124e-06,   4.19494e-06,   3.49052e-06,
     4.14312e-06,   3.83884e-06,   2.7725e-06,   3.75738e-06,   3.84359e-06,   3.23508e-06,
     4.16085e-06,   5.43268e-06,   5.65941e-06,   5.66555e-06,   1.00131e-05,   1.62239e-05,
     1.75445e-05,   2.14947e-05,   3.58956e-05,   4.56586e-05,   3.43405e-05,   2.38686e-05,
     2.4332e-05,   2.54789e-05,   1.95079e-05,   1.33965e-05,   1.25793e-05,   1.40937e-05,
     1.1979e-05,   8.63893e-06,   7.58208e-06,   8.83774e-06,   1.00948e-05,   9.00984e-06,
     7.04681e-06,   8.09797e-06,   1.08886e-05,   1.33532e-05,   1.23902e-05,   1.19507e-05,
     1.48562e-05,   2.23955e-05,   3.10266e-05,   3.26266e-05,   3.10397e-05,   3.39779e-05,
     4.5376e-05,   6.10958e-05,   6.5279e-05,   5.23129e-05,   4.14838e-05,   4.19558e-05,
     5.09964e-05,   6.37575e-05,   6.45187e-05,   4.91907e-05,   3.53937e-05,   3.00032e-05,
     3.20417e-05,   4.00312e-05,   4.4359e-05,   3.93208e-05,   2.98789e-05,   2.47787e-05,
     2.45129e-05,   2.9428e-05,   4.04968e-05,   5.068e-05,   5.14977e-05,   4.47032e-05,
     5.94662e-05,   5.43218e-05,   6.09036e-05,   7.73321e-05,   8.48403e-05,   9.27208e-05,
     9.49843e-05,   8.04592e-05,   5.70392e-05,   5.08953e-05,   5.09718e-05,   4.29127e-05,
     5.66121e-05,   6.40647e-05,   6.35005e-05,   6.45065e-05,   6.8561e-05,   6.61624e-05,
     7.80311e-05,   7.84671e-05,   7.47972e-05,   5.56929e-05,   4.48557e-05,   3.82956e-05,
     3.24203e-05,   2.64364e-05,   2.01874e-05,   2.18951e-05,   2.14424e-05,   2.35802e-05,
     2.69757e-05,   3.50803e-05,   3.50877e-05,   3.08309e-05,   2.96703e-05,   2.80215e-05,
     2.15038e-05,   2.20744e-05,   1.92407e-05,   1.75484e-05,   1.8481e-05,   1.73708e-05,
     1.80909e-05,   1.87169e-05,   1.78419e-05,   1.86544e-05,   1.83542e-05,   1.71676e-05,
     1.57509e-05,   1.42299e-05,   1.48274e-05,   1.35095e-05,   1.29937e-05,   1.31147e-05,
     1.17814e-05,   1.09442e-05,   1.06259e-05,   9.91455e-06,   8.8098e-06,   7.903e-06,
     7.02151e-06,   6.14267e-06,   6.40226e-06,   6.90937e-06,   7.40346e-06,   9.0376e-06,
     1.04649e-05,   1.20358e-05,   1.35923e-05,   1.45542e-05,   1.52353e-05,   1.4587e-05,
     1.27967e-05,   1.1387e-05,   9.40921e-06,   7.79714e-06,   6.31225e-06,   5.63816e-06,
     5.29451e-06,   5.25947e-06,   5.15346e-06,   5.42692e-06,   5.31811e-06,   4.77555e-06,
     3.83623e-06,   3.20429e-06,   2.71831e-06,   2.16288e-06,   1.99789e-06,   2.14329e-06,
     2.24263e-06,   2.42711e-06,   2.89596e-06,   2.86037e-06,   2.81914e-06,   2.48083e-06,
     2.0678e-06,   1.92076e-06,   1.53978e-06,   1.43754e-06,   1.37064e-06,   1.76234e-06,
     2.22508e-06,   2.69147e-06,   3.00682e-06,   3.16738e-06,   2.99304e-06,   2.70142e-06,
     2.48436e-06,   2.08026e-06,   1.84657e-06,   2.06456e-06,   2.72316e-06,   3.75312e-06,
     4.7306e-06,   5.55564e-06,   6.32699e-06,   5.91106e-06,   5.11932e-06,   4.25178e-06,
     3.32253e-06
    };

    const float b3[301] =
    {
     4.15708e-07,   2.60143e-07,   1.32494e-07,   1.5403e-07,   1.64746e-07,   9.97031e-08,
     1.50249e-07,   2.02269e-07,   2.84962e-07,   2.57269e-07,   2.49117e-07,   3.36282e-07,
     3.75568e-07,   2.16954e-07,   4.25352e-07,   4.61622e-07,   4.19126e-07,   3.82693e-07,
     3.69658e-07,   5.02627e-07,   4.42599e-07,   4.84876e-07,   4.95357e-07,   4.94213e-07,
     5.20767e-07,   3.90137e-07,   3.59035e-07,   5.27747e-07,   3.37674e-07,   4.88789e-07,
     3.74123e-07,   3.69271e-07,   3.20795e-07,   4.14981e-07,   4.61504e-07,   2.95452e-07,
     1.74563e-07,   3.27091e-07,   5.33897e-07,   3.57089e-07,   5.66046e-07,   6.04714e-07,
     4.58234e-07,   5.14316e-07,   2.30656e-06,   4.42627e-06,   8.51169e-06,   1.27751e-05,
     1.41306e-05,   1.59342e-05,   1.97713e-05,   2.12166e-05,   2.25515e-05,   2.66048e-05,
     3.56016e-05,   4.35119e-05,   6.64286e-05,   0.000143846,   0.000396291,   0.00113574,
     0.00309186,   0.0111042,   0.0405198,   0.133788,   0.35151,   0.606039,
     0.723007,   0.760026,   0.779389,   0.788811,   0.80685,   0.828035,
     0.840756,   0.842039,   0.844305,   0.846857,   0.850335,   0.859723,
     0.863516,   0.869143,   0.882571,   0.90072,   0.924469,   0.942168,
     0.962285,   0.976601,   0.985726,   0.994764,   1,   0.993053,
     0.979121,   0.907745,   0.66404,   0.301436,   0.0929241,   0.0252385,
     0.00556269,   0.0024767,   0.0011294,   0.00050847,   0.000222899,   0.00010723,
     5.39405e-05,   2.56897e-05,   6.85006e-06,   3.52589e-06,   1.71169e-06,   2.75668e-07,
     1.82902e-06,   1.67961e-06,   6.19122e-06,   8.59549e-06,   1.52263e-05,   1.60512e-05,
     2.56929e-05,   3.25443e-05,   2.91571e-05,   2.70292e-05,   2.41446e-05,   1.40807e-05,
     2.13973e-07,   1.02058e-07,   1.9749e-07,   1.75362e-07,   2.57315e-07,   4.77614e-07,
     6.77741e-07,   7.65801e-07,   9.90214e-07,   7.51558e-07,   6.2059e-07,   9.81957e-07,
     1.09702e-06,   6.25215e-07,   9.27757e-07,   1.11919e-06,   1.17872e-06,   1.14779e-06,
     8.93355e-07,   8.36189e-07,   1.05053e-06,   1.08276e-06,   1.04181e-06,   9.27774e-07,
     1.07702e-06,   9.37779e-07,   9.88439e-07,   1.13665e-06,   1.09476e-06,   1.05959e-06,
     1.04925e-06,   1.29595e-06,   9.34329e-07,   1.13077e-06,   8.30981e-07,   8.0781e-07,
     1.02709e-06,   1.16311e-06,   8.47734e-07,   9.97851e-07,   1.11274e-06,   1.33262e-06,
     1.05514e-06,   9.58995e-07,   1.08595e-06,   1.15264e-06,   1.41038e-06,   1.22922e-06,
     1.70163e-06,   3.4679e-06,   3.95449e-06,   2.72695e-06,   1.36732e-06,   9.90143e-07,
     7.54806e-07,   6.55742e-07,   2.85239e-07,   6.86364e-07,   7.71553e-07,   1.12635e-06,
     9.62279e-07,   2.59255e-06,   9.30699e-07,   1.62071e-07,   1.00889e-10,   1.00081e-10,
     0,   0,   0,   3.23471e-08,   3.49174e-08,   6.19205e-07,
     5.116e-06,   4.16041e-06,   1.78258e-06,   5.02328e-07,   2.68142e-07,   3.8522e-07,
     7.72031e-07,   2.61664e-07,   1.37503e-07,   2.31254e-06,   8.6085e-06,   4.70061e-06,
     1.69519e-06,   1.83093e-07,   1.14424e-07,   1.73792e-06,   1.25833e-06,   1.66069e-06,
     9.83103e-07,   1.83029e-07,   1.17095e-05,   9.47984e-06,   1.12401e-05,   9.14076e-06,
     9.03094e-06,   8.49106e-06,   7.68749e-06,   6.74929e-06,   7.6733e-06,   6.95369e-06,
     6.01684e-06,   5.47181e-06,   6.12431e-06,   6.94762e-06,   6.21639e-06,   6.62084e-06,
     6.59998e-06,   5.76706e-06,   6.04266e-06,   5.40722e-06,   5.40383e-06,   4.91854e-06,
     4.02118e-06,   3.62242e-06,   3.52044e-06,   3.49028e-06,   2.9236e-06,   2.56893e-06,
     2.28989e-06,   2.09968e-06,   2.91966e-06,   2.44435e-06,   2.21268e-06,   2.22573e-06,
     2.02219e-06,   1.75423e-06,   1.83833e-06,   1.72076e-06,   1.51802e-06,   1.53759e-06,
     1.24302e-06,   1.44829e-06,   1.35566e-06,   1.31717e-06,   1.18067e-06,   1.12871e-06,
     1.2687e-06,   1.33911e-06,   1.09356e-06,   1.31515e-06,   1.44675e-06,   1.55647e-06,
     1.63886e-06,   1.97263e-06,   1.86758e-06,   1.76664e-06,   1.90731e-06,   1.8888e-06,
     1.74551e-06,   1.59134e-06,   1.71152e-06,   1.35904e-06,   1.32272e-06,   1.27499e-06,
     1.1585e-06,   1.26847e-06,   1.33096e-06,   1.48891e-06,   1.66013e-06,   1.92885e-06,
     2.33777e-06,   2.98726e-06,   3.83011e-06,   5.24983e-06,   7.20736e-06,   9.73918e-06,
     1.32221e-05,   1.70002e-05,   2.10955e-05,   2.549e-05,   2.99685e-05,   3.51168e-05,
     4.07206e-05,   4.74368e-05,   5.50521e-05,   6.53211e-05,   8.10989e-05,   0.000105497,
     0.000140101
    };

    const float b4[301] =
    {
     3.12145e-07,   8.28534e-08,   9.83184e-08,   8.55079e-08,   4.71887e-08,   3.85598e-08,
     6.13263e-08,   1.09691e-07,   8.19229e-08,   8.18698e-08,   8.44374e-08,   8.18784e-08,
     7.47651e-08,   6.01096e-08,   7.12291e-08,   2.8315e-07,   6.00765e-07,   2.10864e-05,
     2.73104e-05,   4.50671e-07,   1.06402e-08,   6.94993e-08,   3.13328e-08,   1.09588e-07,
     1.74842e-07,   1.9277e-07,   1.34675e-07,   1.92847e-07,   2.02477e-07,   2.6112e-07,
     3.33295e-07,   2.31552e-07,   2.58338e-07,   3.82314e-07,   4.29747e-07,   4.01449e-07,
     1.02894e-06,   5.00318e-06,   3.23438e-06,   1.18679e-06,   5.38393e-07,   3.44446e-07,
     3.9298e-07,   3.47571e-07,   7.25503e-07,   6.40036e-07,   5.09299e-07,   4.9699e-07,
     4.32861e-07,   6.78173e-07,   7.05903e-07,   6.95095e-07,   7.07479e-07,   6.7639e-07,
     6.97946e-07,   6.43371e-07,   7.30317e-07,   5.63357e-07,   3.36905e-07,   4.48422e-07,
     1.74648e-07,   3.13465e-07,   2.23411e-07,   2.26471e-07,   1.67155e-07,   1.6068e-07,
     1.35935e-07,   2.80495e-07,   1.38627e-07,   2.43633e-07,   3.57075e-07,   3.20907e-07,
     5.0935e-07,   2.31732e-07,   2.02004e-07,   1.99908e-07,   1.24175e-07,   3.61372e-07,
     3.62811e-07,   3.75752e-06,   7.00146e-06,   6.23552e-06,   4.3711e-06,   4.90926e-06,
     3.82553e-06,   1.1127e-05,   2.66906e-05,   5.75092e-05,   0.00012712,   0.000344491,
     0.000895024,   0.0022247,   0.0058789,   0.0265572,   0.134652,   0.38922,
     0.66355,   0.848198,   0.899806,   0.920023,   0.932634,   0.940181,
     0.947026,   0.952441,   0.966412,   0.981469,   0.989723,   0.992706,
     1,   0.969368,   0.759433,   0.436651,   0.134322,   0.0308525,
     0.00738524,   0.00223674,   0.0006906,   0.000285122,   0.000157594,   9.63247e-05,
     4.43162e-05,   2.30954e-05,   1.28847e-05,   7.45056e-06,   4.97739e-06,   3.29911e-06,
     2.15753e-06,   1.42561e-06,   8.30459e-07,   9.51382e-07,   8.18661e-07,   7.03925e-07,
     9.15122e-07,   5.41677e-07,   8.31395e-07,   8.59648e-07,   6.23981e-07,   8.59012e-07,
     5.17849e-07,   6.53052e-07,   9.60845e-07,   5.49969e-07,   8.65149e-07,   8.47562e-07,
     7.28829e-07,   1.14307e-06,   9.13936e-07,   8.44814e-07,   1.18804e-06,   1.11252e-06,
     1.50364e-06,   1.29736e-06,   1.19152e-06,   1.05178e-06,   1.28277e-06,   1.31103e-06,
     1.03605e-06,   9.37231e-07,   1.12941e-06,   1.07726e-06,   1.01085e-06,   8.19055e-07,
     8.60927e-07,   8.22335e-07,   7.66233e-07,   8.69038e-07,   7.82916e-07,   7.56701e-07,
     6.00481e-07,   4.7558e-07,   5.374e-07,   6.07694e-07,   6.55254e-07,   2.03489e-07,
     3.94378e-07,   5.87726e-07,   3.09931e-07,   4.14639e-07,   5.61119e-07,   6.43642e-07,
     2.33671e-05,   1.59193e-05,   1.89268e-05,   2.13128e-05,   1.12554e-05,   1.39639e-05,
     1.88608e-05,   1.99851e-05,   1.94386e-05,   9.88685e-06,   1.82516e-05,   1.15331e-05,
     9.26691e-06,   1.352e-05,   1.06333e-05,   1.13419e-05,   1.19485e-05,   7.6223e-06,
     1.19821e-05,   1.2663e-05,   1.70876e-05,   1.1388e-05,   6.7115e-06,   8.61733e-06,
     8.92584e-06,   6.81989e-06,   4.43969e-06,   6.43896e-06,   4.89677e-06,   5.81233e-06,
     6.00927e-06,   4.89342e-06,   1.92265e-06,   3.92241e-06,   3.60834e-06,   2.9064e-06,
     3.27708e-06,   2.99611e-06,   3.11394e-06,   4.66012e-06,   3.91458e-06,   4.29283e-06,
     3.86838e-06,   3.83149e-06,   3.27398e-06,   2.83986e-06,   2.47138e-06,   2.0826e-06,
     2.02905e-06,   1.68586e-06,   1.78242e-06,   1.83929e-06,   1.04638e-06,   1.38024e-06,
     1.31731e-06,   1.31521e-06,   6.52398e-07,   7.78377e-07,   9.25714e-07,   8.67073e-07,
     8.22355e-07,   7.92099e-07,   8.62113e-07,   8.90454e-07,   6.33338e-07,   9.92124e-07,
     8.63085e-07,   7.34335e-07,   9.03921e-07,   7.04451e-07,   7.04449e-07,   7.89631e-07,
     8.61175e-07,   8.20682e-07,   1.00168e-06,   1.43427e-06,   1.54572e-06,   1.60886e-06,
     1.78755e-06,   1.77994e-06,   1.53679e-06,   1.41e-06,   1.19628e-06,   9.79616e-07,
     5.21729e-07,   4.42824e-07,   4.64902e-07,   4.29654e-07,   4.08101e-07,   4.66717e-07,
     4.10742e-07,   3.40666e-07,   3.24714e-07,   4.197e-07,   3.64468e-07,   3.65553e-07,
     3.44671e-07,   3.67198e-07,   3.12263e-07,   3.30082e-07,   1.99664e-07,   1.72882e-07,
     1.60492e-07,   1.2065e-07,   7.23715e-08,   1.43254e-07,   1.12445e-07,   8.94112e-08,
     9.22383e-08,   8.79915e-08,   1.07076e-07,   1.084e-07,   1.27328e-07,   1.69284e-07,
     2.04354e-07,   2.73764e-07,   3.37554e-07,   4.28357e-07,   4.83865e-07,   5.33671e-07,
     5.77712e-07
    };

    const float b5[301] =
    {
     3.12286e-07,   1.21265e-07,   6.94181e-08,   3.9853e-08,   8.0826e-08,   4.04409e-08,
     9.26526e-08,   1.12003e-07,   2.00442e-07,   1.25411e-07,   1.38037e-07,   1.68875e-07,
     1.62312e-07,   9.81447e-08,   2.36616e-07,   2.99972e-07,   3.57437e-07,   1.73237e-07,
     2.51165e-07,   3.80091e-07,   2.95126e-07,   3.00839e-07,   2.8738e-07,   3.69689e-07,
     4.13567e-07,   3.59864e-07,   6.22912e-07,   5.73343e-07,   2.85253e-07,   3.05785e-07,
     2.84932e-07,   2.7333e-07,   3.14806e-07,   3.26106e-07,   1.62258e-07,   1.93473e-07,
     2.7907e-07,   1.95591e-07,   3.78068e-07,   2.19725e-07,   3.13698e-07,   2.73155e-07,
     3.05173e-07,   2.00063e-07,   1.86757e-06,   3.44125e-06,   7.95772e-07,   6.25956e-07,
     1.17606e-06,   2.40044e-06,   9.54706e-07,   9.25815e-07,   5.61706e-07,   1.3451e-06,
     1.28024e-06,   1.36027e-06,   2.94643e-06,   1.86431e-06,   2.88379e-06,   2.16267e-06,
     1.53977e-06,   2.27062e-06,   8.29005e-07,   2.30761e-07,   3.01119e-07,   2.36372e-07,
     2.45071e-07,   2.32871e-07,   1.90817e-07,   2.06172e-07,   1.14463e-07,   3.52299e-08,
     2.27043e-07,   3.45896e-08,   2.42045e-08,   1.21876e-08,   6.62187e-08,   6.09625e-08,
     1.41816e-07,   8.79884e-08,   1.19441e-07,   3.73189e-08,   7.59332e-08,   2.40439e-07,
     1.17802e-07,   6.55622e-08,   8.63856e-08,   6.44464e-08,   2.23776e-07,   1.03047e-07,
     1.32551e-07,   2.79429e-07,   2.07195e-07,   3.67864e-07,   5.11545e-07,   5.89037e-07,
     8.2924e-07,   8.56781e-07,   1.3186e-06,   9.10331e-06,   1.26163e-05,   2.68254e-05,
     4.09503e-05,   7.03781e-05,   0.000122912,   0.000268666,   0.000628092,   0.00164922,
     0.00486736,   0.0227608,   0.0955659,   0.294353,   0.596071,   0.822289,
     0.872776,   0.885335,   0.927928,   0.955695,   0.965042,   0.967095,
     0.965588,   0.957776,   0.9555,   0.952831,   0.955801,   0.965253,
     0.970358,   0.976493,   0.987526,   0.996575,   0.999085,   0.992107,
     0.983694,   0.978311,   0.88528,   0.632595,   0.291823,   0.0832547,
     0.0204412,   0.00563548,   0.00191373,   0.000843246,   0.000403589,   0.000203731,
     0.000101781,   5.6897e-05,   2.7195e-05,   1.61825e-05,   1.4231e-05,   1.68343e-05,
     1.3334e-05,   1.44383e-05,   1.32012e-05,   1.24899e-05,   1.05706e-05,   6.08328e-06,
     3.05093e-06,   3.42552e-06,   1.54743e-06,   1.36525e-06,   1.23105e-06,   4.3642e-07,
     5.81041e-07,   1.18259e-06,   3.00501e-06,   4.97549e-06,   9.51182e-06,   7.32646e-06,
     6.71037e-06,   8.93449e-06,   6.85023e-06,   4.8556e-06,   5.46221e-06,   2.19395e-06,
     1.5006e-06,   4.9089e-07,   2.22462e-06,   6.60787e-07,   6.09816e-07,   9.03731e-07,
     1.37929e-05,   1.40411e-05,   1.3406e-05,   1.63301e-05,   1.06067e-05,   1.24196e-05,
     1.08121e-05,   7.69758e-06,   8.8836e-06,   9.03241e-06,   7.87554e-06,   4.2784e-06,
     1.07825e-05,   4.51337e-06,   6.97831e-06,   6.53672e-06,   6.20341e-06,   6.41495e-06,
     8.42444e-06,   6.85528e-06,   2.70354e-06,   6.50522e-06,   6.34091e-06,   6.54655e-06,
     5.05613e-06,   7.09478e-06,   9.15616e-06,   1.08124e-05,   6.65697e-06,   7.89558e-06,
     4.14291e-06,   4.32081e-06,   8.59893e-06,   6.22021e-06,   7.2363e-06,   6.14019e-06,
     3.87893e-06,   5.4737e-06,   4.80289e-06,   5.01621e-06,   5.71604e-06,   5.0985e-06,
     4.24175e-06,   4.44819e-06,   3.97763e-06,   3.48069e-06,   3.4912e-06,   4.00016e-06,
     2.68235e-06,   3.0765e-06,   3.20229e-06,   2.41872e-06,   3.06521e-06,   2.72094e-06,
     2.13903e-06,   1.70534e-06,   1.72736e-06,   1.5771e-06,   1.31904e-06,   1.10782e-06,
     1.20611e-06,   9.60313e-07,   1.25562e-06,   9.96851e-07,   1.08323e-06,   9.27816e-07,
     7.70516e-07,   6.93097e-07,   7.47027e-07,   7.07703e-07,   8.10806e-07,   7.26741e-07,
     6.80176e-07,   6.20037e-07,   6.39881e-07,   5.95663e-07,   4.7329e-07,   5.43599e-07,
     5.47303e-07,   6.05122e-07,   5.1772e-07,   5.88033e-07,   5.45848e-07,   5.08645e-07,
     2.36035e-07,   5.84744e-07,   5.20728e-07,   4.18312e-07,   3.71858e-07,   4.39561e-07,
     3.37007e-07,   2.56984e-07,   3.79198e-07,   4.64178e-07,   5.27954e-07,   6.02129e-07,
     6.16109e-07,   8.1714e-07,   7.41506e-07,   6.23132e-07,   4.93815e-07,   3.36936e-07,
     2.6114e-07,   1.50937e-07,   1.37661e-07,   1.5674e-07,   1.21087e-07,   1.3344e-07,
     1.58735e-07,   1.86711e-07,   1.80942e-07,   2.25817e-07,   2.17427e-07,   2.35791e-07,
     2.14853e-07,   1.85285e-07,   1.48392e-07,   1.1159e-07,   8.25772e-08,   7.09923e-08,
     5.57264e-08
    };

    const float b6[301] =
    {
     1.31873e-07,   1.87926e-07,   2.81592e-07,   2.17374e-07,   1.70877e-07,   1.0562e-07,
     1.12979e-07,   2.00636e-07,   1.89536e-07,   2.42299e-07,   1.75878e-07,   1.93174e-07,
     2.64698e-07,   2.57301e-07,   3.40167e-07,   3.55343e-07,   7.60476e-07,   2.25433e-06,
     1.29749e-06,   1.29558e-06,   2.01722e-06,   1.13819e-06,   1.64078e-06,   9.96598e-07,
     9.78403e-07,   1.06118e-06,   8.77215e-07,   1.97241e-06,   1.14645e-06,   8.83069e-07,
     7.40957e-07,   1.02664e-06,   9.67904e-06,   5.091e-05,   2.50784e-06,   3.48606e-07,
     8.32043e-07,   2.39095e-05,   2.58811e-06,   2.33621e-07,   1.98094e-07,   2.85714e-07,
     1.27594e-07,   1.7327e-07,   1.82199e-07,   1.5321e-07,   1.63299e-07,   1.01052e-07,
     1.49075e-07,   5.47801e-07,   7.84427e-07,   1.65041e-06,   8.51018e-07,   5.77883e-06,
     1.38337e-05,   7.9076e-06,   3.87846e-06,   5.69445e-06,   1.53368e-05,   9.94259e-06,
     1.2e-06,   1.54553e-06,   4.22091e-06,   1.43071e-06,   2.00613e-07,   1.81045e-07,
     1.2783e-07,   1.03067e-07,   1.82997e-07,   1.10773e-07,   1.1226e-07,   1.16695e-07,
     2.16241e-07,   1.05663e-07,   1.614e-07,   1.6817e-07,   1.16441e-07,   1.15124e-07,
     6.99069e-08,   1.53185e-08,   2.72037e-08,   1.23476e-07,   1.55133e-07,   1.49846e-07,
     3.58885e-07,   2.02413e-06,   2.0841e-06,   8.13696e-07,   9.02676e-07,   2.00266e-06,
     3.1147e-06,   3.09139e-06,   4.4865e-06,   6.70102e-06,   6.05048e-06,   5.02704e-06,
     7.09114e-06,   1.13053e-05,   1.37502e-05,   1.60627e-05,   2.40093e-05,   3.44167e-05,
     3.74905e-05,   3.98343e-05,   5.38148e-05,   7.94317e-05,   8.83804e-05,   7.59032e-05,
     9.03911e-05,   0.000174361,   0.000184766,   5.17257e-05,   8.78977e-06,   4.47753e-06,
     2.69144e-06,   1.45718e-06,   1.07089e-06,   6.55321e-07,   1.38442e-06,   2.79808e-06,
     5.03238e-06,   6.41933e-06,   8.09994e-06,   1.05514e-05,   1.15504e-05,   1.14512e-05,
     1.17762e-05,   1.04862e-05,   1.12589e-05,   8.43843e-06,   8.21063e-06,   8.85197e-06,
     9.70519e-06,   1.53186e-05,   3.28e-05,   7.87273e-05,   0.000207342,   0.000759089,
     0.00312753,   0.0188368,   0.0996101,   0.346835,   0.622908,   0.853785,
     0.952792,   0.978422,   0.987634,   0.989026,   0.993901,   0.999858,
     0.99928,   0.989417,   0.986813,   0.980962,   0.977116,   0.944191,
     0.846399,   0.615837,   0.359763,   0.138813,   0.0305442,   0.00709174,
     0.00215848,   0.000729986,   0.000269875,   0.000118803,   5.53309e-05,   2.39914e-05,
     9.56683e-06,   6.93469e-06,   5.29459e-06,   3.2974e-06,   4.31265e-06,   4.03836e-06,
     5.07602e-06,   7.67396e-06,   8.34915e-06,   9.4325e-06,   6.44795e-06,   5.01789e-06,
     6.71178e-08,   3.03948e-07,   3.1684e-07,   2.65135e-07,   2.09765e-07,   3.00101e-07,
     1.67201e-07,   1.60492e-07,   2.13433e-07,   1.29106e-07,   9.71011e-08,   2.9819e-07,
     1.86975e-07,   2.30772e-07,   1.64834e-07,   6.97091e-08,   3.40321e-07,   2.85903e-07,
     2.06352e-07,   3.65446e-07,   4.38643e-07,   1.65414e-07,   8.01729e-07,   7.59196e-07,
     1.87131e-06,   3.94486e-06,   8.7793e-06,   1.12049e-05,   1.14224e-05,   7.65019e-06,
     4.71997e-06,   4.25463e-06,   1.04158e-05,   8.44869e-06,   7.60545e-06,   7.43366e-06,
     8.91167e-06,   1.17538e-05,   1.53146e-05,   1.90496e-05,   1.73994e-05,   1.71082e-05,
     1.83056e-05,   1.86976e-05,   1.80158e-05,   1.89996e-05,   1.5662e-05,   1.44995e-05,
     1.26694e-05,   1.02432e-05,   8.13754e-06,   6.21556e-06,   4.54308e-06,   3.91257e-06,
     3.1588e-06,   2.52225e-06,   2.01583e-06,   1.62728e-06,   1.47903e-06,   1.09533e-06,
     1.03503e-06,   9.97997e-07,   7.70181e-07,   7.25907e-07,   8.38991e-07,   5.98253e-07,
     7.1384e-07,   4.89317e-07,   6.07583e-07,   5.97359e-07,   7.18152e-07,   5.91987e-07,
     6.80961e-07,   3.87465e-07,   4.28846e-07,   6.08958e-07,   3.8379e-07,   5.16479e-07,
     5.35559e-07,   4.5999e-07,   4.5037e-07,   4.34943e-07,   3.90623e-07,   3.55287e-07,
     3.677e-07,   4.21178e-07,   3.62045e-07,   3.55015e-07,   2.54848e-07,   2.60974e-07,
     2.42997e-07,   1.96909e-07,   2.38909e-07,   1.73543e-07,   2.20613e-07,   1.73692e-07,
     1.66732e-07,   1.40222e-07,   1.22858e-07,   1.39263e-07,   1.49344e-07,   1.63372e-07,
     1.54912e-07,   1.45368e-07,   1.58958e-07,   1.93923e-07,   1.65487e-07,   1.60786e-07,
     1.52013e-07,   1.35784e-07,   9.24469e-08,   9.92189e-08,   7.60063e-08,   9.85175e-08,
     7.22548e-08,   4.57741e-08,   6.97201e-08,   8.69437e-08,   1.13252e-07,   2.58077e-07,
     4.68462e-07
    };

    const float b7[301] =
    {
     3.42494e-07,   5.7563e-09,   2.4306e-08,   6.23161e-08,   8.60169e-08,   6.54447e-09,
     4.9819e-08,   9.30485e-08,   5.51852e-08,   1.75292e-08,   2.87474e-08,   2.73409e-08,
     2.83316e-08,   3.82621e-09,   3.07146e-08,   4.55383e-08,   1.04778e-07,   1.23884e-09,
     8.25112e-09,   5.0621e-08,   1.79225e-07,   2.74552e-08,   1.04373e-07,   9.7114e-08,
     9.62771e-08,   1.75398e-07,   8.63102e-08,   6.84208e-08,   9.40778e-08,   5.15181e-08,
     1.02132e-07,   1.42588e-07,   8.08711e-08,   2.16959e-07,   1.03608e-07,   9.2287e-08,
     1.59861e-06,   6.11993e-06,   3.68931e-07,   9.24078e-08,   3.81349e-07,   2.64627e-07,
     3.27124e-07,   2.55578e-07,   2.87094e-07,   3.84e-07,   5.21921e-07,   6.14658e-07,
     3.75539e-07,   3.80046e-07,   7.36489e-07,   1.37786e-06,   6.07935e-07,   9.45777e-07,
     1.59554e-06,   5.68797e-07,   4.72783e-07,   5.63447e-07,   3.35752e-07,   1.70326e-07,
     3.17931e-07,   1.41325e-07,   2.49863e-07,   3.96423e-07,   2.16004e-07,   3.37582e-07,
     2.90124e-07,   3.80183e-07,   3.11457e-07,   3.98668e-07,   1.79901e-07,   1.22001e-07,
     1.13002e-07,   2.85022e-08,   4.67327e-08,   1.90183e-08,   0,   2.99103e-08,
     1.06093e-07,   1.18422e-07,   1.07083e-08,   7.91334e-08,   9.95149e-08,   1.54428e-07,
     9.12854e-08,   2.15373e-08,   7.48973e-08,   7.40942e-08,   0,   3.73909e-08,
     7.49644e-08,   0,   0,   1.92802e-08,   2.95975e-08,   7.04334e-08,
     6.50404e-08,   0,   0,   0,   2.0755e-07,   1.04836e-10,
     7.80821e-09,   1.57983e-07,   1.0595e-10,   1.13403e-07,   3.312e-10,   1.14176e-07,
     1.03109e-10,   3.76115e-09,   7.45634e-09,   1.19272e-07,   1.07481e-10,   1.10155e-10,
     1.1276e-10,   1.40161e-07,   1.16185e-10,   7.139e-08,   9.90442e-08,   3.74204e-07,
     5.05087e-07,   4.6352e-07,   5.97207e-07,   5.4384e-07,   6.81312e-07,   6.90591e-07,
     1.00825e-06,   8.18553e-07,   1.07975e-06,   8.27673e-07,   9.35167e-07,   9.64121e-07,
     1.00596e-06,   8.68247e-07,   8.97409e-07,   1.19586e-06,   1.28982e-06,   9.86575e-07,
     9.25037e-07,   2.05008e-06,   1.71977e-06,   3.51855e-06,   4.80754e-06,   6.35905e-06,
     5.22507e-06,   4.29703e-06,   3.92372e-06,   4.42103e-06,   4.97574e-06,   5.96695e-06,
     6.60085e-06,   1.11374e-05,   1.62372e-05,   2.84438e-05,   4.4002e-05,   7.0506e-05,
     0.000106141,   0.000165311,   0.000256229,   0.000412257,   0.000713036,   0.00139941,
     0.00288571,   0.00595325,   0.0126868,   0.0276938,   0.0588599,   0.123117,
     0.260204,   0.506774,   0.759967,   0.94058,   0.995805,   0.997622,
     0.988907,   0.983091,   0.972934,   0.958312,   0.941139,   0.931907,
     0.919382,   0.90908,   0.8976,   0.88212,   0.871191,   0.863054,
     0.844284,   0.836496,   0.838036,   0.841189,   0.835657,   0.833819,
     0.842548,   0.830777,   0.818604,   0.816837,   0.806711,   0.81076,
     0.794161,   0.779884,   0.767313,   0.749602,   0.739271,   0.738714,
     0.734145,   0.711833,   0.697141,   0.680462,   0.658635,   0.649781,
     0.632471,   0.619908,   0.60912,   0.598992,   0.590914,   0.566107,
     0.507112,   0.402799,   0.275174,   0.157667,   0.0738047,   0.032787,
     0.0138147,   0.00539084,   0.00262463,   0.00180863,   0.00130633,   0.00103281,
     0.000917177,   0.000891162,   0.000798344,   0.000745739,   0.000727178,   0.000783273,
     0.000701058,   0.000655311,   0.00060278,   0.000557875,   0.000281186,   0.000274231,
     4.37496e-06,   3.22571e-06,   2.88909e-06,   2.24195e-06,   1.98867e-06,   1.47066e-06,
     1.12615e-06,   1.04739e-06,   1.1344e-06,   8.90363e-07,   8.71592e-07,   7.40614e-07,
     7.19518e-07,   6.91579e-07,   7.01778e-07,   6.72029e-07,   4.36737e-07,   6.31103e-07,
     6.64941e-07,   6.02511e-07,   3.22655e-07,   5.32912e-07,   4.71662e-07,   4.99519e-07,
     3.35434e-07,   4.46158e-07,   4.48065e-07,   3.19015e-07,   2.39899e-07,   4.55294e-07,
     2.89741e-07,   2.5021e-07,   2.57359e-07,   3.5683e-07,   2.96983e-07,   2.97873e-07,
     1.88219e-07,   2.40558e-07,   2.15601e-07,   2.13278e-07,   1.40431e-07,   1.60961e-07,
     1.70122e-07,   1.10615e-07,   1.0436e-07,   1.222e-07,   1.25283e-07,   9.88137e-08,
     8.59799e-08,   9.19016e-08,   7.99614e-08,   8.73603e-08,   7.68151e-08,   6.51417e-08,
     7.38325e-08,   6.48494e-08,   5.64187e-08,   6.5542e-08,   5.90555e-08,   4.77664e-08,
     5.59409e-08
    };

    const float b8[301] =
    {
     2.75157e-07,   6.96359e-06,   1.9874e-05,   8.73924e-05,   7.4835e-05,   5.16098e-06,
     4.81496e-07,   6.24369e-07,   2.89158e-07,   1.33848e-07,   1.61046e-07,   2.69148e-07,
     1.41051e-07,   1.93786e-07,   2.0957e-07,   2.34628e-07,   2.36518e-07,   2.47669e-07,
     2.25346e-07,   2.01273e-07,   2.14493e-07,   1.42204e-07,   1.7784e-07,   2.29739e-07,
     4.0488e-07,   2.15782e-07,   1.58074e-07,   1.50063e-07,   7.18437e-08,   1.88801e-07,
     2.3008e-07,   1.64135e-07,   1.27638e-07,   1.9751e-07,   1.13678e-07,   7.76496e-07,
     1.1877e-06,   1.47692e-07,   2.12794e-07,   1.89394e-06,   1.22e-06,   8.28353e-07,
     5.66846e-07,   1.55612e-07,   3.89524e-08,   6.08863e-08,   4.13505e-07,   1.57781e-07,
     8.45778e-09,   9.45575e-07,   3.68309e-06,   1.1502e-06,   2.14722e-07,   1.01022e-07,
     6.8025e-07,   8.6337e-07,   3.19639e-08,   2.56285e-07,   1.35378e-07,   3.34694e-08,
     7.36246e-07,   8.36204e-07,   2.65609e-08,   8.10432e-08,   2.05826e-07,   3.69154e-07,
     8.02175e-07,   8.89483e-07,   4.4826e-07,   2.42823e-07,   2.17011e-07,   2.65704e-06,
     8.78188e-06,   7.05779e-07,   3.82597e-07,   4.04534e-06,   4.67549e-06,   9.19411e-08,
     2.98568e-07,   2.08203e-06,   1.03067e-06,   9.72367e-08,   1.50024e-07,   1.2413e-07,
     1.4876e-07,   2.35395e-08,   1.65976e-10,   1.65631e-10,   8.45577e-09,   1.59829e-10,
     1.56488e-10,   1.35137e-07,   1.26482e-06,   9.54683e-08,   2.93604e-07,   1.59714e-07,
     2.23112e-07,   5.34018e-07,   6.1337e-07,   1.72864e-07,   5.77747e-07,   4.80195e-07,
     5.14193e-07,   5.97282e-07,   1.16949e-06,   5.3439e-07,   2.8602e-07,   2.20078e-07,
     5.34755e-07,   5.51942e-07,   6.32398e-07,   1.13151e-06,   1.09476e-06,   3.24873e-07,
     3.6263e-07,   5.49154e-07,   8.32561e-08,   6.24779e-07,   6.90714e-07,   8.366e-07,
     1.2346e-06,   1.93899e-06,   2.35572e-06,   9.86481e-07,   5.4618e-07,   1.53631e-06,
     4.98064e-07,   4.68967e-08,   2.82103e-07,   1.79437e-07,   2.82801e-07,   5.47962e-07,
     7.69025e-07,   1.62521e-07,   2.32749e-07,   5.04263e-07,   8.32099e-07,   2.27261e-07,
     4.08301e-07,   3.43738e-07,   1.2715e-07,   2.36481e-07,   8.74008e-08,   3.913e-07,
     4.88832e-07,   6.9057e-07,   7.13913e-07,   8.95172e-07,   8.96901e-07,   5.53029e-07,
     1.04012e-06,   9.12699e-07,   7.19448e-07,   7.20192e-07,   8.80887e-07,   1.14393e-06,
     5.57443e-07,   9.4312e-07,   1.04838e-06,   9.98724e-07,   1.23283e-06,   8.72156e-07,
     8.03599e-07,   5.35655e-07,   8.46701e-07,   6.91104e-07,   8.55943e-07,   6.31436e-07,
     4.51508e-07,   1.11291e-06,   1.03136e-06,   4.73296e-07,   6.08156e-07,   8.62452e-07,
     9.2854e-07,   7.79087e-07,   1.22534e-06,   0.000114086,   0.000453417,   0.000504746,
     0.000779556,   0.0010461,   0.000976304,   0.00111328,   0.0010687,   0.00109088,
     0.00109552,   0.00113106,   0.00109447,   0.00108912,   0.00104157,   0.0011403,
     0.001127,   0.00117057,   0.00115094,   0.00120607,   0.00135466,   0.00130018,
     0.00149181,   0.002231,   0.00432839,   0.0105836,   0.0305957,   0.107853,
     0.313746,   0.571884,   0.789269,   0.951541,   0.987144,   0.991677,
     0.993363,   0.999691,   0.996691,   0.983327,   0.970856,   0.95236,
     0.93302,   0.922376,   0.910922,   0.899767,   0.892112,   0.878565,
     0.864645,   0.850525,   0.841298,   0.827257,   0.80818,   0.794005,
     0.774377,   0.760282,   0.746906,   0.725848,   0.708967,   0.689184,
     0.66172,   0.64063,   0.620929,   0.596457,   0.574685,   0.551871,
     0.530905,   0.51163,   0.493667,   0.474118,   0.454311,   0.437766,
     0.421449,   0.406047,   0.389741,   0.376622,   0.363517,   0.349855,
     0.336697,   0.326962,   0.31708,   0.305806,   0.29492,   0.282203,
     0.271439,   0.260807,   0.250912,   0.243373,   0.235734,   0.227658,
     0.217559,   0.208722,   0.198576,   0.188152,   0.178705,   0.168441,
     0.159032,   0.148125,   0.13777,   0.124557,   0.109612,   0.0917406,
     0.0715068,   0.0503125,   0.0328064,   0.0198817,   0.00979572,   0.00470763,
     0.00218491,   0.00102923,   0.000523709,   0.000298469,   0.000200007,   0.000145961,
     0.000114257,   0.000102038,   8.00865e-05,   7.65951e-05,   8.01237e-05,   6.45166e-05,
     7.00539e-05,   6.46441e-05,   5.37976e-05,   2.97925e-05,   2.99793e-05,   2.77311e-05,
     6.76079e-07
    };

    //Add multispectral bands to the temporary list
    const std::vector<float> vb1 (b1, b1 + sizeof(b1) / sizeof(float) );
    tmpSpectralBandList.push_back(vb1);
    const std::vector<float> vb2 (b2, b2 + sizeof(b2) / sizeof(float) );
    tmpSpectralBandList.push_back(vb2);
    const std::vector<float> vb3 (b3, b3 + sizeof(b3) / sizeof(float) );
    tmpSpectralBandList.push_back(vb3);
    const std::vector<float> vb4 (b4, b4 + sizeof(b4) / sizeof(float) );
    tmpSpectralBandList.push_back(vb4);
    const std::vector<float> vb5 (b5, b5 + sizeof(b5) / sizeof(float) );
    tmpSpectralBandList.push_back(vb5);
    const std::vector<float> vb6 (b6, b6 + sizeof(b6) / sizeof(float) );
    tmpSpectralBandList.push_back(vb6);
    const std::vector<float> vb7 (b7, b7 + sizeof(b7) / sizeof(float) );
    tmpSpectralBandList.push_back(vb7);
    const std::vector<float> vb8 (b8, b8 + sizeof(b8) / sizeof(float) );
    tmpSpectralBandList.push_back(vb8);
    }

  unsigned int j = 0;
  for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
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

} // end namespace otb
