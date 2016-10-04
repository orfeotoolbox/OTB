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

#include "otbQuickBirdImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

QuickBirdImageMetadataInterface
::QuickBirdImageMetadataInterface()
{
}

bool
QuickBirdImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("QB02") != std::string::npos) return true;
  else return false;
}

QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface::GetSolarIrradiance() const
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

  VariableLengthVectorType outputValuesVariableLengthVector;

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  if (keywordStringBId == panchro)
    {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(1381.79);
    }
  else if (keywordStringBId == multi)
    {
    outputValuesVariableLengthVector.SetSize(4);
    outputValuesVariableLengthVector[0] = 1924.59;
    outputValuesVariableLengthVector[1] = 1843.08;
    outputValuesVariableLengthVector[2] = 1574.77;
    outputValuesVariableLengthVector[3] = 1113.71;
    }
  else
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  return outputValuesVariableLengthVector;
}

int
QuickBirdImageMetadataInterface::GetDay() const
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
QuickBirdImageMetadataInterface::GetMonth() const
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
QuickBirdImageMetadataInterface::GetYear() const
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
QuickBirdImageMetadataInterface::GetHour() const
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
QuickBirdImageMetadataInterface::GetMinute() const
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
QuickBirdImageMetadataInterface::GetProductionDay() const
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
QuickBirdImageMetadataInterface::GetProductionMonth() const
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
QuickBirdImageMetadataInterface::GetProductionYear() const
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

QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetPhysicalBias() const
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

  VariableLengthVectorType outputValuesVariableLengthVector;
  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  if (keywordStringBId == panchro)
    {
    outputValuesVariableLengthVector.SetSize(1);
    outputValuesVariableLengthVector.Fill(0.0);
    }
  else if (keywordStringBId == multi)
    {
    outputValuesVariableLengthVector.SetSize(4);
    outputValuesVariableLengthVector.Fill(0.0);
    }
  else
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  return outputValuesVariableLengthVector;
}

QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetPhysicalGain() const
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

  int  productionYear = this->GetProductionYear(); //keywordStrings[0].toInt();
  int  productionMonth = this->GetProductionMonth(); //keywordStrings[1].toInt();
  int  productionDay = this->GetProductionDay(); //keywordStrings[2].toInt();
  bool isPost20030606 = false;
  if (productionYear > 2003) isPost20030606 = true;
  else
    {
    if (productionYear == 2003)
      {
      if (productionMonth > 6) isPost20030606 = true;
      else
        if (productionDay >= 6) isPost20030606 = true;
      }
    }

  //Value computed from
  // Radiance Conversion of QuickBird Data - Technical Note.
  std::string keywordStringBitsPerPixel = imageKeywordlist.GetMetadataByKey("support_data.bits_per_pixel");
  int         bitsPerPixel = atoi(keywordStringBitsPerPixel.c_str());
  if (bitsPerPixel != 16 && bitsPerPixel != 8)
    {
    itkExceptionMacro(<< "Invalid bitsPerPixel " << bitsPerPixel);
    }

  std::string keywordStringBId = imageKeywordlist.GetMetadataByKey("support_data.band_id");
  std::string panchro("P");
  std::string multi("Multi");
  if (keywordStringBId != panchro && keywordStringBId != multi)
    {
    itkExceptionMacro(<< "Invalid bandID " << keywordStringBId);
    }

  std::string keywordStringTDILevel = imageKeywordlist.GetMetadataByKey("support_data.TDI_level");
  int         TDILevel = atoi(keywordStringTDILevel.c_str());
  if (keywordStringBId == panchro && TDILevel != 10 && TDILevel != 13 && TDILevel != 18 && TDILevel != 24 &&
      TDILevel != 32)
    {
    itkExceptionMacro(<< "Invalid TDILevel " << TDILevel);
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  if (keywordStringBId == panchro) outputValuesVariableLengthVector.SetSize(1);
  else outputValuesVariableLengthVector.SetSize(4);
  outputValuesVariableLengthVector.Fill(1.);

  if (!isPost20030606)
    {
    if (bitsPerPixel == 16)
      {
      if (keywordStringBId == panchro)
        {
        if (TDILevel != 10) outputValuesVariableLengthVector[0] = 0.08381880;
        else if (TDILevel != 13) outputValuesVariableLengthVector[0] = 0.06447600;
        else if (TDILevel != 18) outputValuesVariableLengthVector[0] = 0.04656600;
        else if (TDILevel != 24) outputValuesVariableLengthVector[0] = 0.03494440;
        else if (TDILevel != 32) outputValuesVariableLengthVector[0] = 0.02618840;
        }
      else
        {
        outputValuesVariableLengthVector[0] = 0.01604120;
        outputValuesVariableLengthVector[1] = 0.01438470;
        outputValuesVariableLengthVector[2] = 0.01267350;
        outputValuesVariableLengthVector[3] = 0.01542420;
        }

      }
    else
      {
      if (keywordStringBId == panchro)
        {
        if (TDILevel != 10) outputValuesVariableLengthVector[0] = 1.02681367;
        else if (TDILevel != 13) outputValuesVariableLengthVector[0] = 1.02848939;
        else if (TDILevel != 18) outputValuesVariableLengthVector[0] = 1.02794702;
        else if (TDILevel != 24) outputValuesVariableLengthVector[0] = 1.02989685;
        else if (TDILevel != 32) outputValuesVariableLengthVector[0] = 1.02739898;

        std::string keywordStringAbsCalFactor = imageKeywordlist.GetMetadataByKey("support_data.absCalFactor");
        double absCalFactor = atof(keywordStringAbsCalFactor.c_str());
        outputValuesVariableLengthVector[0] *= absCalFactor;
        }
      else
        {
        std::string keywordStringBCalFactor = imageKeywordlist.GetMetadataByKey("support_data.B_band_absCalFactor");
        double bCalFactor = atof(keywordStringBCalFactor.c_str());
        std::string keywordStringGCalFactor = imageKeywordlist.GetMetadataByKey("support_data.G_band_absCalFactor");
        double gCalFactor = atof(keywordStringGCalFactor.c_str());
        std::string keywordStringRCalFactor = imageKeywordlist.GetMetadataByKey("support_data.R_band_absCalFactor");
        double rCalFactor = atof(keywordStringRCalFactor.c_str());
        std::string keywordStringNCalFactor = imageKeywordlist.GetMetadataByKey("support_data.N_band_absCalFactor");
        double nCalFactor = atof(keywordStringNCalFactor.c_str());

        outputValuesVariableLengthVector[0] = 1.12097834 * bCalFactor;
        outputValuesVariableLengthVector[1] = 1.37652632 * gCalFactor;
        outputValuesVariableLengthVector[2] = 1.30954587 * rCalFactor;
        outputValuesVariableLengthVector[3] = 0.98368622 * nCalFactor;
        }
      }

    }
  else
    {
    if (keywordStringBId == panchro)
      {
      std::string keywordStringAbsCalFactor = imageKeywordlist.GetMetadataByKey("support_data.absCalFactor");
      double absCalFactor = atof(keywordStringAbsCalFactor.c_str());
      outputValuesVariableLengthVector[0] = absCalFactor;
      }
    else
      {
      std::string keywordStringBCalFactor = imageKeywordlist.GetMetadataByKey("support_data.B_band_absCalFactor");
      double bCalFactor = atof(keywordStringBCalFactor.c_str());
      std::string keywordStringGCalFactor = imageKeywordlist.GetMetadataByKey("support_data.G_band_absCalFactor");
      double gCalFactor = atof(keywordStringGCalFactor.c_str());
      std::string keywordStringRCalFactor = imageKeywordlist.GetMetadataByKey("support_data.R_band_absCalFactor");
      double rCalFactor = atof(keywordStringRCalFactor.c_str());
      std::string keywordStringNCalFactor = imageKeywordlist.GetMetadataByKey("support_data.N_band_absCalFactor");
      double nCalFactor = atof(keywordStringNCalFactor.c_str());

      outputValuesVariableLengthVector[0] = bCalFactor;
      outputValuesVariableLengthVector[1] = gCalFactor;
      outputValuesVariableLengthVector[2] = rCalFactor;
      outputValuesVariableLengthVector[3] = nCalFactor;
      }
    }

  if (keywordStringBId == panchro)
    {
    outputValuesVariableLengthVector[0] = 0.398 / outputValuesVariableLengthVector[0];
    }
  else
    {
    outputValuesVariableLengthVector[0] = 0.068 / outputValuesVariableLengthVector[0];
    outputValuesVariableLengthVector[1] = 0.099 / outputValuesVariableLengthVector[1];
    outputValuesVariableLengthVector[2] = 0.071 / outputValuesVariableLengthVector[2];
    outputValuesVariableLengthVector[3] = 0.114 / outputValuesVariableLengthVector[3];
    }

  return outputValuesVariableLengthVector;
}

double
QuickBirdImageMetadataInterface::GetSatElevation() const
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
QuickBirdImageMetadataInterface::GetSatAzimuth() const
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

  std::string key("support_data.sat_azimuth_angle");
  if (!imageKeywordlist.HasKey(key))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey(key);
  double value = atof(valueString.c_str());
  return value;
}

QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetFirstWavelengths() const
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

QuickBirdImageMetadataInterface::VariableLengthVectorType
QuickBirdImageMetadataInterface
::GetLastWavelengths() const
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

std::vector<unsigned int>
QuickBirdImageMetadataInterface
::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 2;
  rgb[1] = 1;
  rgb[2] = 0;
  return rgb;
}

std::vector<std::string>
QuickBirdImageMetadataInterface
::GetEnhancedBandNames() const
{
  std::vector<std::string> enhBandNames;
  std::vector<std::string> rawBandNames = this->Superclass::GetBandName();

  //std::cout << rawBandNames.size() << std::endl;
  if(rawBandNames.size())
    {
    for (std::vector<std::string>::iterator it = rawBandNames.begin(); it != rawBandNames.end(); ++it)
      {
      //std::cout << *it << std::endl;

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
      if ( !(*it).compare("R") )
        {
        enhBandNames.push_back("Red");
        }
      else if ( !(*it).compare("G") )
        {
        enhBandNames.push_back("Green");
        }
      else if ( !(*it).compare("B") )
        {
        enhBandNames.push_back("Blue");
        }
      else if ( !(*it).compare("N") )
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

QuickBirdImageMetadataInterface::WavelengthSpectralBandVectorType
QuickBirdImageMetadataInterface
::GetSpectralSensitivity()  const
{
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::list <std::vector<float> > tmpSpectralBandList;

  //Read sensor image type (panchromatic or multispectral)
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
    const float b0[321] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0007015f,   0.001403f,   0.001291f,   0.001179f,   0.0011415f,
     0.001104f,   0.00101145f,   0.0009189f,   0.00104845f,   0.001178f,   0.0016055f,
     0.002033f,   0.0044495f,   0.006866f,   0.008873f,   0.01088f,   0.013805f,
     0.01673f,   0.021325f,   0.02592f,   0.03106f,   0.0362f,   0.043415f,
     0.05063f,   0.05879f,   0.06695f,   0.075765f,   0.08458f,   0.09389f,
     0.1032f,   0.11255f,   0.1219f,   0.13345f,   0.145f,   0.1567f,
     0.1684f,   0.17895f,   0.1895f,   0.2011f,   0.2127f,   0.21625f,
     0.2198f,   0.2392f,   0.2586f,   0.26695f,   0.2753f,   0.2854f,
     0.2955f,   0.30545f,   0.3154f,   0.3259f,   0.3364f,   0.341f,
     0.3456f,   0.3501f,   0.3546f,   0.35785f,   0.3611f,   0.3648f,
     0.3685f,   0.3733f,   0.3781f,   0.3875f,   0.3969f,   0.4055f,
     0.4141f,   0.42755f,   0.441f,   0.4543f,   0.4676f,   0.4829f,
     0.4982f,   0.5167f,   0.5352f,   0.5532f,   0.5712f,   0.59245f,
     0.6137f,   0.6382f,   0.6627f,   0.6839f,   0.7051f,   0.7275f,
     0.7499f,   0.77045f,   0.791f,   0.81055f,   0.8301f,   0.8465f,
     0.8629f,   0.877f,   0.8911f,   0.9081f,   0.9251f,   0.9293f,
     0.9335f,   0.94265f,   0.9518f,   0.95695f,   0.9621f,   0.96095f,
     0.9598f,   0.97525f,   0.9907f,   0.9941f,   0.9975f,   0.9922f,
     0.9869f,   0.99345f,   1.0f,   0.9938f,   0.9876f,   0.9877f,
     0.9878f,   0.977f,   0.9662f,   0.95725f,   0.9483f,   0.934f,
     0.9197f,   0.919f,   0.9183f,   0.90635f,   0.8944f,   0.8912f,
     0.888f,   0.88305f,   0.8781f,   0.8865f,   0.8949f,   0.8921f,
     0.8893f,   0.8858f,   0.8823f,   0.88695f,   0.8916f,   0.8848f,
     0.878f,   0.8879f,   0.8978f,   0.8933f,   0.8888f,   0.8801f,
     0.8714f,   0.86905f,   0.8667f,   0.8677f,   0.8687f,   0.86995f,
     0.8712f,   0.86685f,   0.8625f,   0.87175f,   0.881f,   0.8814f,
     0.8818f,   0.8897f,   0.8976f,   0.9024f,   0.9072f,   0.91835f,
     0.9295f,   0.93105f,   0.9326f,   0.9198f,   0.907f,   0.91305f,
     0.9191f,   0.9173f,   0.9155f,   0.9097f,   0.9039f,   0.89875f,
     0.8936f,   0.88705f,   0.8805f,   0.8761f,   0.8717f,   0.8605f,
     0.8493f,   0.8475f,   0.8457f,   0.8505f,   0.8553f,   0.85675f,
     0.8582f,   0.85825f,   0.8583f,   0.8627f,   0.8671f,   0.8505f,
     0.8339f,   0.83895f,   0.844f,   0.847f,   0.85f,   0.85265f,
     0.8553f,   0.8447f,   0.8341f,   0.83675f,   0.8394f,   0.8235f,
     0.8076f,   0.8091f,   0.8106f,   0.785f,   0.7594f,   0.7616f,
     0.7638f,   0.749f,   0.7342f,   0.7204f,   0.7066f,   0.70035f,
     0.6941f,   0.67875f,   0.6634f,   0.65895f,   0.6545f,   0.63655f,
     0.6186f,   0.60945f,   0.6003f,   0.5961f,   0.5919f,   0.58315f,
     0.5744f,   0.55265f,   0.5309f,   0.5139f,   0.4969f,   0.48835f,
     0.4798f,   0.46305f,   0.4463f,   0.4305f,   0.4147f,   0.40975f,
     0.4048f,   0.3893f,   0.3738f,   0.3593f,   0.3448f,   0.3283f,
     0.3118f,   0.3035f,   0.2952f,   0.28405f,   0.2729f,   0.2648f,
     0.2567f,   0.24975f,   0.2428f,   0.2322f,   0.2216f,   0.21615f,
     0.2107f,   0.2039f,   0.1971f,   0.18735f,   0.1776f,   0.1703f,
     0.163f,   0.15575f,   0.1485f,   0.14215f,   0.1358f,   0.1299f,
     0.124f,   0.11625f,   0.1085f,   0.10197f,   0.09544f,   0.08927f,
     0.0831f,   0.077305f,   0.07151f,   0.06746f,   0.06341f,   0.05911f,
     0.05481f,   0.051195f,   0.04758f,   0.02882f,   0.01006f,   0.00503f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f
    };

    //add panchromatic band to the temporary list
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
    tmpSpectralBandList.push_back(vb0);
    }
  else
    {
    const float b1[321] =
    {
     0.0f,   4.816e-11f,   9.632e-11f,   1.4606e-10f,   1.958e-10f,   2.4715e-10f,
     2.985e-10f,   3.514e-10f,   4.043e-10f,   0.000505f,   0.00101f,   0.0015305f,
     0.002051f,   0.002587f,   0.003123f,   0.0036745f,   0.004226f,   0.0043815f,
     0.004537f,   0.0049295f,   0.005322f,   0.0046895f,   0.004057f,   0.003661f,
     0.003265f,   0.002984f,   0.002703f,   0.002503f,   0.002303f,   0.0017815f,
     0.00126f,   0.0015305f,   0.001801f,   0.0020025f,   0.002204f,   0.0022995f,
     0.002395f,   0.0024735f,   0.002552f,   0.002512f,   0.002472f,   0.0026895f,
     0.002907f,   0.003455f,   0.004003f,   0.0052815f,   0.00656f,   0.00985f,
     0.01314f,   0.02076f,   0.02838f,   0.04826f,   0.06814f,   0.11167f,
     0.1552f,   0.2164f,   0.2776f,   0.35445f,   0.4313f,   0.5306f,
     0.6299f,   0.677f,   0.7241f,   0.75775f,   0.7914f,   0.81765f,
     0.8439f,   0.86675f,   0.8896f,   0.9037f,   0.9178f,   0.92355f,
     0.9293f,   0.94475f,   0.9602f,   0.9801f,   1.0f,   0.9958f,
     0.9916f,   0.95475f,   0.9179f,   0.84515f,   0.7724f,   0.6729f,
     0.5734f,   0.4787f,   0.384f,   0.32465f,   0.2653f,   0.21225f,
     0.1592f,   0.120415f,   0.08163f,   0.075435f,   0.06924f,   0.06627f,
     0.0633f,   0.055115f,   0.04693f,   0.03814f,   0.02935f,   0.023495f,
     0.01764f,   0.015185f,   0.01273f,   0.012595f,   0.01246f,   0.012675f,
     0.01289f,   0.012755f,   0.01262f,   0.011865f,   0.01111f,   0.0106f,
     0.01009f,   0.0098255f,   0.009561f,   0.0094225f,   0.009284f,   0.0095255f,
     0.009767f,   0.0102035f,   0.01064f,   0.010515f,   0.01039f,   0.0091415f,
     0.007893f,   0.006632f,   0.005371f,   0.004464f,   0.003557f,   0.0030945f,
     0.002632f,   0.0025675f,   0.002503f,   0.0024945f,   0.002486f,   0.0025155f,
     0.002545f,   0.002665f,   0.002785f,   0.0027235f,   0.002662f,   0.0027025f,
     0.002743f,   0.0030155f,   0.003288f,   0.003867f,   0.004446f,   0.006651f,
     0.008856f,   0.012518f,   0.01618f,   0.020175f,   0.02417f,   0.02563f,
     0.02709f,   0.02308f,   0.01907f,   0.0136865f,   0.008303f,   0.006957f,
     0.005611f,   0.0063045f,   0.006998f,   0.008584f,   0.01017f,   0.011135f,
     0.0121f,   0.0106205f,   0.009141f,   0.006401f,   0.003661f,   0.003178f,
     0.002695f,   0.002812f,   0.002929f,   0.0027945f,   0.00266f,   0.002858f,
     0.003056f,   0.0031515f,   0.003247f,   0.0034115f,   0.003576f,   0.003427f,
     0.003278f,   0.0034455f,   0.003613f,   0.0036285f,   0.003644f,   0.003367f,
     0.00309f,   0.003232f,   0.003374f,   0.003267f,   0.00316f,   0.0031495f,
     0.003139f,   0.003062f,   0.002985f,   0.003139f,   0.003293f,   0.0033135f,
     0.003334f,   0.00345f,   0.003566f,   0.003647f,   0.003728f,   0.003857f,
     0.003986f,   0.0040515f,   0.004117f,   0.004197f,   0.004277f,   0.004226f,
     0.004175f,   0.0045645f,   0.004954f,   0.0048685f,   0.004783f,   0.005318f,
     0.005853f,   0.005788f,   0.005723f,   0.00567f,   0.005617f,   0.00546f,
     0.005303f,   0.005252f,   0.005201f,   0.005183f,   0.005165f,   0.0051595f,
     0.005154f,   0.0052175f,   0.005281f,   0.00549f,   0.005699f,   0.0062505f,
     0.006802f,   0.007529f,   0.008256f,   0.0090055f,   0.009755f,   0.0101475f,
     0.01054f,   0.010745f,   0.01095f,   0.010137f,   0.009324f,   0.0084725f,
     0.007621f,   0.0068735f,   0.006126f,   0.0055815f,   0.005037f,   0.0046315f,
     0.004226f,   0.003881f,   0.003536f,   0.0032265f,   0.002917f,   0.002516f,
     0.002115f,   0.0019205f,   0.001726f,   0.001647f,   0.001568f,   0.0014765f,
     0.001385f,   0.001294f,   0.001203f,   0.001205f,   0.001207f,   0.0012145f,
     0.001222f,   0.0011705f,   0.001119f,   0.0011195f,   0.00112f,   0.001082f,
     0.001044f,   0.0010645f,   0.001085f,   0.001156f,   0.001227f,   0.001233f,
     0.001239f,   0.00135f,   0.001461f,   0.001648f,   0.001835f,   0.001667f,
     0.001499f,   0.001511f,   0.001523f,   0.0014645f,   0.001406f,   0.0013125f,
     0.001219f,   0.0006095f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0
    };

    const float b2[321] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.001543f,   0.003086f,   0.0035565f,   0.004027f,   0.0031075f,
     0.002188f,   0.0022535f,   0.002319f,   0.001899f,   0.001479f,   0.0014535f,
     0.001428f,   0.00112715f,   0.0008263f,   0.00098315f,   0.00114f,   0.001191f,
     0.001242f,   0.0011965f,   0.001151f,   0.001195f,   0.001239f,   0.00119f,
     0.001141f,   0.001164f,   0.001187f,   0.001289f,   0.001391f,   0.0016425f,
     0.001894f,   0.0021145f,   0.002335f,   0.002574f,   0.002813f,   0.0032595f,
     0.003706f,   0.0046255f,   0.005545f,   0.0076825f,   0.00982f,   0.013835f,
     0.01785f,   0.02064f,   0.02343f,   0.026765f,   0.0301f,   0.03563f,
     0.04116f,   0.050415f,   0.05967f,   0.078915f,   0.09816f,   0.12738f,
     0.1566f,   0.1924f,   0.2282f,   0.2739f,   0.3196f,   0.36855f,
     0.4175f,   0.4726f,   0.5277f,   0.5928f,   0.6579f,   0.71385f,
     0.7698f,   0.8005f,   0.8312f,   0.83785f,   0.8445f,   0.8575f,
     0.8705f,   0.88935f,   0.9082f,   0.9271f,   0.946f,   0.96395f,
     0.9819f,   0.99095f,   1.0f,   0.9967f,   0.9934f,   0.9876f,
     0.9818f,   0.97545f,   0.9691f,   0.97325f,   0.9774f,   0.98215f,
     0.9869f,   0.97605f,   0.9652f,   0.93155f,   0.8979f,   0.84445f,
     0.791f,   0.72305f,   0.6551f,   0.56865f,   0.4822f,   0.3973f,
     0.3124f,   0.25785f,   0.2033f,   0.17055f,   0.1378f,   0.11394f,
     0.09008f,   0.072895f,   0.05571f,   0.046095f,   0.03648f,   0.032155f,
     0.02783f,   0.026995f,   0.02616f,   0.02677f,   0.02738f,   0.02745f,
     0.02752f,   0.02585f,   0.02418f,   0.02138f,   0.01858f,   0.016675f,
     0.01477f,   0.01398f,   0.01319f,   0.013215f,   0.01324f,   0.01321f,
     0.01318f,   0.01241f,   0.01164f,   0.0103975f,   0.009155f,   0.0076035f,
     0.006052f,   0.005198f,   0.004344f,   0.003775f,   0.003206f,   0.0030305f,
     0.002855f,   0.002863f,   0.002871f,   0.0034605f,   0.00405f,   0.0051125f,
     0.006175f,   0.006901f,   0.007627f,   0.007632f,   0.007637f,   0.006665f,
     0.005693f,   0.004709f,   0.003725f,   0.003184f,   0.002643f,   0.002404f,
     0.002165f,   0.002389f,   0.002613f,   0.0028265f,   0.00304f,   0.0029335f,
     0.002827f,   0.0029585f,   0.00309f,   0.003231f,   0.003372f,   0.0036625f,
     0.003953f,   0.003888f,   0.003823f,   0.004107f,   0.004391f,   0.004264f,
     0.004137f,   0.003973f,   0.003809f,   0.003648f,   0.003487f,   0.003354f,
     0.003221f,   0.0031325f,   0.003044f,   0.0030325f,   0.003021f,   0.0030705f,
     0.00312f,   0.0031665f,   0.003213f,   0.0032575f,   0.003302f,   0.0035005f,
     0.003699f,   0.0040635f,   0.004428f,   0.004754f,   0.00508f,   0.0059895f,
     0.006899f,   0.0073485f,   0.007798f,   0.008437f,   0.009076f,   0.0091385f,
     0.009201f,   0.0091225f,   0.009044f,   0.0088315f,   0.008619f,   0.0082995f,
     0.00798f,   0.00779f,   0.0076f,   0.007548f,   0.007496f,   0.007548f,
     0.0076f,   0.007711f,   0.007822f,   0.0079615f,   0.008101f,   0.008127f,
     0.008153f,   0.007961f,   0.007769f,   0.007238f,   0.006707f,   0.0061495f,
     0.005592f,   0.005117f,   0.004642f,   0.0042505f,   0.003859f,   0.0034755f,
     0.003092f,   0.00288f,   0.002668f,   0.0023465f,   0.002025f,   0.0018485f,
     0.001672f,   0.001577f,   0.001482f,   0.001362f,   0.001242f,   0.001247f,
     0.001252f,   0.001152f,   0.001052f,   0.00099725f,   0.0009425f,   0.0009543f,
     0.0009661f,   0.00091635f,   0.0008666f,   0.0008281f,   0.0007896f,   0.00083065f,
     0.0008717f,   0.00088005f,   0.0008884f,   0.00092515f,   0.0009619f,   0.00103995f,
     0.001118f,   0.001196f,   0.001274f,   0.0011625f,   0.001051f,   0.0010565f,
     0.001062f,   0.00096085f,   0.0008597f,   0.00091815f,   0.0009766f,   0.0009189f,
     0.0008612f,   0.0004306f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,
    };

    const float b3[321] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0018575f,   0.003715f,   0.003079f,   0.002443f,   0.0020455f,
     0.001648f,   0.0017865f,   0.001925f,   0.001553f,   0.001181f,   0.001127f,
     0.001073f,   0.0009881f,   0.0009032f,   0.00093675f,   0.0009703f,   0.00094635f,
     0.0009224f,   0.0011177f,   0.001313f,   0.000995f,   0.000677f,   0.0009465f,
     0.001216f,   0.0012365f,   0.001257f,   0.001289f,   0.001321f,   0.0012365f,
     0.001152f,   0.00107375f,   0.0009955f,   0.00125725f,   0.001519f,   0.001803f,
     0.002087f,   0.002445f,   0.002803f,   0.0029485f,   0.003094f,   0.002991f,
     0.002888f,   0.0028545f,   0.002821f,   0.002901f,   0.002981f,   0.0032205f,
     0.00346f,   0.003574f,   0.003688f,   0.003422f,   0.003156f,   0.0026165f,
     0.002077f,   0.002126f,   0.002175f,   0.0022785f,   0.002382f,   0.002534f,
     0.002686f,   0.0028785f,   0.003071f,   0.003181f,   0.003291f,   0.0033965f,
     0.003502f,   0.003615f,   0.003728f,   0.004149f,   0.00457f,   0.00494f,
     0.00531f,   0.00563f,   0.00595f,   0.0061365f,   0.006323f,   0.006458f,
     0.006593f,   0.0067545f,   0.006916f,   0.007343f,   0.00777f,   0.008377f,
     0.008984f,   0.009607f,   0.01023f,   0.01101f,   0.01179f,   0.01283f,
     0.01387f,   0.015775f,   0.01768f,   0.020725f,   0.02377f,   0.028595f,
     0.03342f,   0.03993f,   0.04644f,   0.054755f,   0.06307f,   0.07513f,
     0.08719f,   0.107645f,   0.1281f,   0.16655f,   0.205f,   0.26755f,
     0.3301f,   0.41305f,   0.496f,   0.579f,   0.662f,   0.7337f,
     0.8054f,   0.8447f,   0.884f,   0.8976f,   0.9112f,   0.9265f,
     0.9418f,   0.9608f,   0.9798f,   0.9878f,   0.9958f,   0.9979f,
     1.0f,   0.99615f,   0.9923f,   0.97235f,   0.9524f,   0.9125f,
     0.8726f,   0.8128f,   0.753f,   0.6767f,   0.6004f,   0.51195f,
     0.4235f,   0.35205f,   0.2806f,   0.22285f,   0.1651f,   0.130405f,
     0.09571f,   0.07443f,   0.05315f,   0.041695f,   0.03024f,   0.026095f,
     0.02195f,   0.021015f,   0.02008f,   0.020405f,   0.02073f,   0.021105f,
     0.02148f,   0.02055f,   0.01962f,   0.01719f,   0.01476f,   0.0121415f,
     0.009523f,   0.0080775f,   0.006632f,   0.0058285f,   0.005025f,   0.004769f,
     0.004513f,   0.004338f,   0.004163f,   0.004345f,   0.004527f,   0.0047955f,
     0.005064f,   0.005541f,   0.006018f,   0.0063415f,   0.006665f,   0.006433f,
     0.006201f,   0.005328f,   0.004455f,   0.003759f,   0.003063f,   0.002884f,
     0.002705f,   0.00257f,   0.002435f,   0.002453f,   0.002471f,   0.0024775f,
     0.002484f,   0.0024255f,   0.002367f,   0.0024915f,   0.002616f,   0.00253f,
     0.002444f,   0.002528f,   0.002612f,   0.0025575f,   0.002503f,   0.00255f,
     0.002597f,   0.002551f,   0.002505f,   0.002426f,   0.002347f,   0.0023605f,
     0.002374f,   0.0022935f,   0.002213f,   0.002167f,   0.002121f,   0.002053f,
     0.001985f,   0.001928f,   0.001871f,   0.00194f,   0.002009f,   0.002071f,
     0.002133f,   0.002293f,   0.002453f,   0.0026255f,   0.002798f,   0.0027925f,
     0.002787f,   0.002651f,   0.002515f,   0.0020235f,   0.001532f,   0.001344f,
     0.001156f,   0.001118f,   0.00108f,   0.0010297f,   0.0009794f,   0.0009771f,
     0.0009748f,   0.0010049f,   0.001035f,   0.00092405f,   0.0008131f,   0.00082065f,
     0.0008282f,   0.0007723f,   0.0007164f,   0.00070735f,   0.0006983f,   0.00070225f,
     0.0007062f,   0.0006895f,   0.0006728f,   0.0006287f,   0.0005846f,   0.0006068f,
     0.000629f,   0.00065395f,   0.0006789f,   0.00071925f,   0.0007596f,   0.000719f,
     0.0006784f,   0.00066125f,   0.0006441f,   0.0007476f,   0.0008511f,   0.0008908f,
     0.0009305f,   0.0008436f,   0.0007567f,   0.0007948f,   0.0008329f,   0.000872f,
     0.0009111f,   0.00089545f,   0.0008798f,   0.0007618f,   0.0006438f,   0.00073805f,
     0.0008323f,   0.00041615f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f
    };

    const float b4[321] =
    {
     0.0f,   6.72e-11f,   1.344e-10f,   2.038e-10f,   2.732e-10f,   3.4475e-10f,
     4.163e-10f,   4.901e-10f,   5.639e-10f,   0.0007045f,   0.001409f,   0.002135f,
     0.002861f,   0.0036085f,   0.004356f,   0.0051255f,   0.005895f,   0.006112f,
     0.006329f,   0.0061655f,   0.006002f,   0.005693f,   0.005384f,   0.0044505f,
     0.003517f,   0.0028125f,   0.002108f,   0.002272f,   0.002436f,   0.002068f,
     0.0017f,   0.0019855f,   0.002271f,   0.0020705f,   0.00187f,   0.0017455f,
     0.001621f,   0.0017855f,   0.00195f,   0.0018905f,   0.001831f,   0.0016705f,
     0.00151f,   0.0015045f,   0.001499f,   0.001721f,   0.001943f,   0.0018305f,
     0.001718f,   0.002082f,   0.002446f,   0.002492f,   0.002538f,   0.002833f,
     0.003128f,   0.003589f,   0.00405f,   0.0043915f,   0.004733f,   0.0047405f,
     0.004748f,   0.004824f,   0.0049f,   0.004541f,   0.004182f,   0.004313f,
     0.004444f,   0.0046115f,   0.004779f,   0.005007f,   0.005235f,   0.005396f,
     0.005557f,   0.0054025f,   0.005248f,   0.0047195f,   0.004191f,   0.0035905f,
     0.00299f,   0.0028175f,   0.002645f,   0.0026115f,   0.002578f,   0.002558f,
     0.002538f,   0.0027f,   0.002862f,   0.002721f,   0.00258f,   0.002973f,
     0.003366f,   0.004324f,   0.005282f,   0.006576f,   0.00787f,   0.008871f,
     0.009872f,   0.010051f,   0.01023f,   0.0090465f,   0.007863f,   0.006367f,
     0.004871f,   0.0048565f,   0.004842f,   0.0055935f,   0.006345f,   0.0069985f,
     0.007652f,   0.0077885f,   0.007925f,   0.007197f,   0.006469f,   0.0054185f,
     0.004368f,   0.004082f,   0.003796f,   0.003768f,   0.00374f,   0.0036695f,
     0.003599f,   0.0036165f,   0.003634f,   0.0037785f,   0.003923f,   0.0042145f,
     0.004506f,   0.004688f,   0.00487f,   0.0052705f,   0.005671f,   0.005878f,
     0.006085f,   0.006441f,   0.006797f,   0.007044f,   0.007291f,   0.007926f,
     0.008561f,   0.0092905f,   0.01002f,   0.010625f,   0.01123f,   0.01174f,
     0.01225f,   0.012595f,   0.01294f,   0.01323f,   0.01352f,   0.01396f,
     0.0144f,   0.01499f,   0.01558f,   0.01641f,   0.01724f,   0.01842f,
     0.0196f,   0.021835f,   0.02407f,   0.026755f,   0.02944f,   0.03189f,
     0.03434f,   0.03751f,   0.04068f,   0.04483f,   0.04898f,   0.05445f,
     0.05992f,   0.066405f,   0.07289f,   0.081705f,   0.09052f,   0.10501f,
     0.1195f,   0.141f,   0.1625f,   0.20245f,   0.2424f,   0.3015f,
     0.3606f,   0.4268f,   0.493f,   0.564f,   0.635f,   0.71665f,
     0.7983f,   0.8726f,   0.9469f,   0.97345f,   1.0f,   0.99855f,
     0.9971f,   0.98365f,   0.9702f,   0.9526f,   0.935f,   0.91795f,
     0.9009f,   0.88535f,   0.8698f,   0.86065f,   0.8515f,   0.84485f,
     0.8382f,   0.8352f,   0.8322f,   0.8289f,   0.8256f,   0.823f,
     0.8204f,   0.8137f,   0.807f,   0.7957f,   0.7844f,   0.76695f,
     0.7495f,   0.72675f,   0.704f,   0.6911f,   0.6782f,   0.6638f,
     0.6494f,   0.6313f,   0.6132f,   0.59005f,   0.5669f,   0.5493f,
     0.5317f,   0.5117f,   0.4917f,   0.4644f,   0.4371f,   0.3943f,
     0.3515f,   0.3134f,   0.2753f,   0.24845f,   0.2216f,   0.191f,
     0.1604f,   0.1385f,   0.1166f,   0.1003f,   0.084f,   0.07559f,
     0.06718f,   0.054015f,   0.04085f,   0.030665f,   0.02048f,   0.017005f,
     0.01353f,   0.01178f,   0.01003f,   0.0089005f,   0.007771f,   0.007077f,
     0.006383f,   0.0058745f,   0.005366f,   0.0049605f,   0.004555f,   0.0040195f,
     0.003484f,   0.0031455f,   0.002807f,   0.002621f,   0.002435f,   0.0023315f,
     0.002228f,   0.002158f,   0.002088f,   0.001973f,   0.001858f,   0.001839f,
     0.00182f,   0.0018615f,   0.001903f,   0.001759f,   0.001615f,   0.0016735f,
     0.001732f,   0.0017285f,   0.001725f,   0.001884f,   0.002043f,   0.0020925f,
     0.002142f,   0.0020825f,   0.002023f,   0.001963f,   0.001903f,   0.0018045f,
     0.001706f,   0.0017565f,   0.001807f,   0.0017525f,   0.001698f,   0.001849f,
     0.002f,   0.001f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f
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
    }

  unsigned int j = 0;
  for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
    {
    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.300);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.1);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.0025);
    ++j;
    }

  return wavelengthSpectralBand;
}

} // end namespace otb
