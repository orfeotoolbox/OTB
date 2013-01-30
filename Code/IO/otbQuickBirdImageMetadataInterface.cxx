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

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
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

  std::cout << rawBandNames.size() << std::endl;
  if(rawBandNames.size())
    {
    for (std::vector<std::string>::iterator it = rawBandNames.begin() ; it != rawBandNames.end(); ++it)
      {
      std::cout << *it << std::endl;

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
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0.0007015,   0.001403,   0.001291,   0.001179,   0.0011415,
     0.001104,   0.00101145,   0.0009189,   0.00104845,   0.001178,   0.0016055,
     0.002033,   0.0044495,   0.006866,   0.008873,   0.01088,   0.013805,
     0.01673,   0.021325,   0.02592,   0.03106,   0.0362,   0.043415,
     0.05063,   0.05879,   0.06695,   0.075765,   0.08458,   0.09389,
     0.1032,   0.11255,   0.1219,   0.13345,   0.145,   0.1567,
     0.1684,   0.17895,   0.1895,   0.2011,   0.2127,   0.21625,
     0.2198,   0.2392,   0.2586,   0.26695,   0.2753,   0.2854,
     0.2955,   0.30545,   0.3154,   0.3259,   0.3364,   0.341,
     0.3456,   0.3501,   0.3546,   0.35785,   0.3611,   0.3648,
     0.3685,   0.3733,   0.3781,   0.3875,   0.3969,   0.4055,
     0.4141,   0.42755,   0.441,   0.4543,   0.4676,   0.4829,
     0.4982,   0.5167,   0.5352,   0.5532,   0.5712,   0.59245,
     0.6137,   0.6382,   0.6627,   0.6839,   0.7051,   0.7275,
     0.7499,   0.77045,   0.791,   0.81055,   0.8301,   0.8465,
     0.8629,   0.877,   0.8911,   0.9081,   0.9251,   0.9293,
     0.9335,   0.94265,   0.9518,   0.95695,   0.9621,   0.96095,
     0.9598,   0.97525,   0.9907,   0.9941,   0.9975,   0.9922,
     0.9869,   0.99345,   1,   0.9938,   0.9876,   0.9877,
     0.9878,   0.977,   0.9662,   0.95725,   0.9483,   0.934,
     0.9197,   0.919,   0.9183,   0.90635,   0.8944,   0.8912,
     0.888,   0.88305,   0.8781,   0.8865,   0.8949,   0.8921,
     0.8893,   0.8858,   0.8823,   0.88695,   0.8916,   0.8848,
     0.878,   0.8879,   0.8978,   0.8933,   0.8888,   0.8801,
     0.8714,   0.86905,   0.8667,   0.8677,   0.8687,   0.86995,
     0.8712,   0.86685,   0.8625,   0.87175,   0.881,   0.8814,
     0.8818,   0.8897,   0.8976,   0.9024,   0.9072,   0.91835,
     0.9295,   0.93105,   0.9326,   0.9198,   0.907,   0.91305,
     0.9191,   0.9173,   0.9155,   0.9097,   0.9039,   0.89875,
     0.8936,   0.88705,   0.8805,   0.8761,   0.8717,   0.8605,
     0.8493,   0.8475,   0.8457,   0.8505,   0.8553,   0.85675,
     0.8582,   0.85825,   0.8583,   0.8627,   0.8671,   0.8505,
     0.8339,   0.83895,   0.844,   0.847,   0.85,   0.85265,
     0.8553,   0.8447,   0.8341,   0.83675,   0.8394,   0.8235,
     0.8076,   0.8091,   0.8106,   0.785,   0.7594,   0.7616,
     0.7638,   0.749,   0.7342,   0.7204,   0.7066,   0.70035,
     0.6941,   0.67875,   0.6634,   0.65895,   0.6545,   0.63655,
     0.6186,   0.60945,   0.6003,   0.5961,   0.5919,   0.58315,
     0.5744,   0.55265,   0.5309,   0.5139,   0.4969,   0.48835,
     0.4798,   0.46305,   0.4463,   0.4305,   0.4147,   0.40975,
     0.4048,   0.3893,   0.3738,   0.3593,   0.3448,   0.3283,
     0.3118,   0.3035,   0.2952,   0.28405,   0.2729,   0.2648,
     0.2567,   0.24975,   0.2428,   0.2322,   0.2216,   0.21615,
     0.2107,   0.2039,   0.1971,   0.18735,   0.1776,   0.1703,
     0.163,   0.15575,   0.1485,   0.14215,   0.1358,   0.1299,
     0.124,   0.11625,   0.1085,   0.10197,   0.09544,   0.08927,
     0.0831,   0.077305,   0.07151,   0.06746,   0.06341,   0.05911,
     0.05481,   0.051195,   0.04758,   0.02882,   0.01006,   0.00503,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0
    };

    //add panchromatic band to the temporary list
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
    tmpSpectralBandList.push_back(vb0);
    }
  else
    {
    const float b1[321] =
    {
     0,   4.816e-11,   9.632e-11,   1.4606e-10,   1.958e-10,   2.4715e-10,
     2.985e-10,   3.514e-10,   4.043e-10,   0.000505,   0.00101,   0.0015305,
     0.002051,   0.002587,   0.003123,   0.0036745,   0.004226,   0.0043815,
     0.004537,   0.0049295,   0.005322,   0.0046895,   0.004057,   0.003661,
     0.003265,   0.002984,   0.002703,   0.002503,   0.002303,   0.0017815,
     0.00126,   0.0015305,   0.001801,   0.0020025,   0.002204,   0.0022995,
     0.002395,   0.0024735,   0.002552,   0.002512,   0.002472,   0.0026895,
     0.002907,   0.003455,   0.004003,   0.0052815,   0.00656,   0.00985,
     0.01314,   0.02076,   0.02838,   0.04826,   0.06814,   0.11167,
     0.1552,   0.2164,   0.2776,   0.35445,   0.4313,   0.5306,
     0.6299,   0.677,   0.7241,   0.75775,   0.7914,   0.81765,
     0.8439,   0.86675,   0.8896,   0.9037,   0.9178,   0.92355,
     0.9293,   0.94475,   0.9602,   0.9801,   1,   0.9958,
     0.9916,   0.95475,   0.9179,   0.84515,   0.7724,   0.6729,
     0.5734,   0.4787,   0.384,   0.32465,   0.2653,   0.21225,
     0.1592,   0.120415,   0.08163,   0.075435,   0.06924,   0.06627,
     0.0633,   0.055115,   0.04693,   0.03814,   0.02935,   0.023495,
     0.01764,   0.015185,   0.01273,   0.012595,   0.01246,   0.012675,
     0.01289,   0.012755,   0.01262,   0.011865,   0.01111,   0.0106,
     0.01009,   0.0098255,   0.009561,   0.0094225,   0.009284,   0.0095255,
     0.009767,   0.0102035,   0.01064,   0.010515,   0.01039,   0.0091415,
     0.007893,   0.006632,   0.005371,   0.004464,   0.003557,   0.0030945,
     0.002632,   0.0025675,   0.002503,   0.0024945,   0.002486,   0.0025155,
     0.002545,   0.002665,   0.002785,   0.0027235,   0.002662,   0.0027025,
     0.002743,   0.0030155,   0.003288,   0.003867,   0.004446,   0.006651,
     0.008856,   0.012518,   0.01618,   0.020175,   0.02417,   0.02563,
     0.02709,   0.02308,   0.01907,   0.0136865,   0.008303,   0.006957,
     0.005611,   0.0063045,   0.006998,   0.008584,   0.01017,   0.011135,
     0.0121,   0.0106205,   0.009141,   0.006401,   0.003661,   0.003178,
     0.002695,   0.002812,   0.002929,   0.0027945,   0.00266,   0.002858,
     0.003056,   0.0031515,   0.003247,   0.0034115,   0.003576,   0.003427,
     0.003278,   0.0034455,   0.003613,   0.0036285,   0.003644,   0.003367,
     0.00309,   0.003232,   0.003374,   0.003267,   0.00316,   0.0031495,
     0.003139,   0.003062,   0.002985,   0.003139,   0.003293,   0.0033135,
     0.003334,   0.00345,   0.003566,   0.003647,   0.003728,   0.003857,
     0.003986,   0.0040515,   0.004117,   0.004197,   0.004277,   0.004226,
     0.004175,   0.0045645,   0.004954,   0.0048685,   0.004783,   0.005318,
     0.005853,   0.005788,   0.005723,   0.00567,   0.005617,   0.00546,
     0.005303,   0.005252,   0.005201,   0.005183,   0.005165,   0.0051595,
     0.005154,   0.0052175,   0.005281,   0.00549,   0.005699,   0.0062505,
     0.006802,   0.007529,   0.008256,   0.0090055,   0.009755,   0.0101475,
     0.01054,   0.010745,   0.01095,   0.010137,   0.009324,   0.0084725,
     0.007621,   0.0068735,   0.006126,   0.0055815,   0.005037,   0.0046315,
     0.004226,   0.003881,   0.003536,   0.0032265,   0.002917,   0.002516,
     0.002115,   0.0019205,   0.001726,   0.001647,   0.001568,   0.0014765,
     0.001385,   0.001294,   0.001203,   0.001205,   0.001207,   0.0012145,
     0.001222,   0.0011705,   0.001119,   0.0011195,   0.00112,   0.001082,
     0.001044,   0.0010645,   0.001085,   0.001156,   0.001227,   0.001233,
     0.001239,   0.00135,   0.001461,   0.001648,   0.001835,   0.001667,
     0.001499,   0.001511,   0.001523,   0.0014645,   0.001406,   0.0013125,
     0.001219,   0.0006095,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0
    };

    const float b2[321] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0.001543,   0.003086,   0.0035565,   0.004027,   0.0031075,
     0.002188,   0.0022535,   0.002319,   0.001899,   0.001479,   0.0014535,
     0.001428,   0.00112715,   0.0008263,   0.00098315,   0.00114,   0.001191,
     0.001242,   0.0011965,   0.001151,   0.001195,   0.001239,   0.00119,
     0.001141,   0.001164,   0.001187,   0.001289,   0.001391,   0.0016425,
     0.001894,   0.0021145,   0.002335,   0.002574,   0.002813,   0.0032595,
     0.003706,   0.0046255,   0.005545,   0.0076825,   0.00982,   0.013835,
     0.01785,   0.02064,   0.02343,   0.026765,   0.0301,   0.03563,
     0.04116,   0.050415,   0.05967,   0.078915,   0.09816,   0.12738,
     0.1566,   0.1924,   0.2282,   0.2739,   0.3196,   0.36855,
     0.4175,   0.4726,   0.5277,   0.5928,   0.6579,   0.71385,
     0.7698,   0.8005,   0.8312,   0.83785,   0.8445,   0.8575,
     0.8705,   0.88935,   0.9082,   0.9271,   0.946,   0.96395,
     0.9819,   0.99095,   1,   0.9967,   0.9934,   0.9876,
     0.9818,   0.97545,   0.9691,   0.97325,   0.9774,   0.98215,
     0.9869,   0.97605,   0.9652,   0.93155,   0.8979,   0.84445,
     0.791,   0.72305,   0.6551,   0.56865,   0.4822,   0.3973,
     0.3124,   0.25785,   0.2033,   0.17055,   0.1378,   0.11394,
     0.09008,   0.072895,   0.05571,   0.046095,   0.03648,   0.032155,
     0.02783,   0.026995,   0.02616,   0.02677,   0.02738,   0.02745,
     0.02752,   0.02585,   0.02418,   0.02138,   0.01858,   0.016675,
     0.01477,   0.01398,   0.01319,   0.013215,   0.01324,   0.01321,
     0.01318,   0.01241,   0.01164,   0.0103975,   0.009155,   0.0076035,
     0.006052,   0.005198,   0.004344,   0.003775,   0.003206,   0.0030305,
     0.002855,   0.002863,   0.002871,   0.0034605,   0.00405,   0.0051125,
     0.006175,   0.006901,   0.007627,   0.007632,   0.007637,   0.006665,
     0.005693,   0.004709,   0.003725,   0.003184,   0.002643,   0.002404,
     0.002165,   0.002389,   0.002613,   0.0028265,   0.00304,   0.0029335,
     0.002827,   0.0029585,   0.00309,   0.003231,   0.003372,   0.0036625,
     0.003953,   0.003888,   0.003823,   0.004107,   0.004391,   0.004264,
     0.004137,   0.003973,   0.003809,   0.003648,   0.003487,   0.003354,
     0.003221,   0.0031325,   0.003044,   0.0030325,   0.003021,   0.0030705,
     0.00312,   0.0031665,   0.003213,   0.0032575,   0.003302,   0.0035005,
     0.003699,   0.0040635,   0.004428,   0.004754,   0.00508,   0.0059895,
     0.006899,   0.0073485,   0.007798,   0.008437,   0.009076,   0.0091385,
     0.009201,   0.0091225,   0.009044,   0.0088315,   0.008619,   0.0082995,
     0.00798,   0.00779,   0.0076,   0.007548,   0.007496,   0.007548,
     0.0076,   0.007711,   0.007822,   0.0079615,   0.008101,   0.008127,
     0.008153,   0.007961,   0.007769,   0.007238,   0.006707,   0.0061495,
     0.005592,   0.005117,   0.004642,   0.0042505,   0.003859,   0.0034755,
     0.003092,   0.00288,   0.002668,   0.0023465,   0.002025,   0.0018485,
     0.001672,   0.001577,   0.001482,   0.001362,   0.001242,   0.001247,
     0.001252,   0.001152,   0.001052,   0.00099725,   0.0009425,   0.0009543,
     0.0009661,   0.00091635,   0.0008666,   0.0008281,   0.0007896,   0.00083065,
     0.0008717,   0.00088005,   0.0008884,   0.00092515,   0.0009619,   0.00103995,
     0.001118,   0.001196,   0.001274,   0.0011625,   0.001051,   0.0010565,
     0.001062,   0.00096085,   0.0008597,   0.00091815,   0.0009766,   0.0009189,
     0.0008612,   0.0004306,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0
    };

    const float b3[321] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0.0018575,   0.003715,   0.003079,   0.002443,   0.0020455,
     0.001648,   0.0017865,   0.001925,   0.001553,   0.001181,   0.001127,
     0.001073,   0.0009881,   0.0009032,   0.00093675,   0.0009703,   0.00094635,
     0.0009224,   0.0011177,   0.001313,   0.000995,   0.000677,   0.0009465,
     0.001216,   0.0012365,   0.001257,   0.001289,   0.001321,   0.0012365,
     0.001152,   0.00107375,   0.0009955,   0.00125725,   0.001519,   0.001803,
     0.002087,   0.002445,   0.002803,   0.0029485,   0.003094,   0.002991,
     0.002888,   0.0028545,   0.002821,   0.002901,   0.002981,   0.0032205,
     0.00346,   0.003574,   0.003688,   0.003422,   0.003156,   0.0026165,
     0.002077,   0.002126,   0.002175,   0.0022785,   0.002382,   0.002534,
     0.002686,   0.0028785,   0.003071,   0.003181,   0.003291,   0.0033965,
     0.003502,   0.003615,   0.003728,   0.004149,   0.00457,   0.00494,
     0.00531,   0.00563,   0.00595,   0.0061365,   0.006323,   0.006458,
     0.006593,   0.0067545,   0.006916,   0.007343,   0.00777,   0.008377,
     0.008984,   0.009607,   0.01023,   0.01101,   0.01179,   0.01283,
     0.01387,   0.015775,   0.01768,   0.020725,   0.02377,   0.028595,
     0.03342,   0.03993,   0.04644,   0.054755,   0.06307,   0.07513,
     0.08719,   0.107645,   0.1281,   0.16655,   0.205,   0.26755,
     0.3301,   0.41305,   0.496,   0.579,   0.662,   0.7337,
     0.8054,   0.8447,   0.884,   0.8976,   0.9112,   0.9265,
     0.9418,   0.9608,   0.9798,   0.9878,   0.9958,   0.9979,
     1,   0.99615,   0.9923,   0.97235,   0.9524,   0.9125,
     0.8726,   0.8128,   0.753,   0.6767,   0.6004,   0.51195,
     0.4235,   0.35205,   0.2806,   0.22285,   0.1651,   0.130405,
     0.09571,   0.07443,   0.05315,   0.041695,   0.03024,   0.026095,
     0.02195,   0.021015,   0.02008,   0.020405,   0.02073,   0.021105,
     0.02148,   0.02055,   0.01962,   0.01719,   0.01476,   0.0121415,
     0.009523,   0.0080775,   0.006632,   0.0058285,   0.005025,   0.004769,
     0.004513,   0.004338,   0.004163,   0.004345,   0.004527,   0.0047955,
     0.005064,   0.005541,   0.006018,   0.0063415,   0.006665,   0.006433,
     0.006201,   0.005328,   0.004455,   0.003759,   0.003063,   0.002884,
     0.002705,   0.00257,   0.002435,   0.002453,   0.002471,   0.0024775,
     0.002484,   0.0024255,   0.002367,   0.0024915,   0.002616,   0.00253,
     0.002444,   0.002528,   0.002612,   0.0025575,   0.002503,   0.00255,
     0.002597,   0.002551,   0.002505,   0.002426,   0.002347,   0.0023605,
     0.002374,   0.0022935,   0.002213,   0.002167,   0.002121,   0.002053,
     0.001985,   0.001928,   0.001871,   0.00194,   0.002009,   0.002071,
     0.002133,   0.002293,   0.002453,   0.0026255,   0.002798,   0.0027925,
     0.002787,   0.002651,   0.002515,   0.0020235,   0.001532,   0.001344,
     0.001156,   0.001118,   0.00108,   0.0010297,   0.0009794,   0.0009771,
     0.0009748,   0.0010049,   0.001035,   0.00092405,   0.0008131,   0.00082065,
     0.0008282,   0.0007723,   0.0007164,   0.00070735,   0.0006983,   0.00070225,
     0.0007062,   0.0006895,   0.0006728,   0.0006287,   0.0005846,   0.0006068,
     0.000629,   0.00065395,   0.0006789,   0.00071925,   0.0007596,   0.000719,
     0.0006784,   0.00066125,   0.0006441,   0.0007476,   0.0008511,   0.0008908,
     0.0009305,   0.0008436,   0.0007567,   0.0007948,   0.0008329,   0.000872,
     0.0009111,   0.00089545,   0.0008798,   0.0007618,   0.0006438,   0.00073805,
     0.0008323,   0.00041615,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0
    };

    const float b4[321] =
    {
     0,   6.72e-11,   1.344e-10,   2.038e-10,   2.732e-10,   3.4475e-10,
     4.163e-10,   4.901e-10,   5.639e-10,   0.0007045,   0.001409,   0.002135,
     0.002861,   0.0036085,   0.004356,   0.0051255,   0.005895,   0.006112,
     0.006329,   0.0061655,   0.006002,   0.005693,   0.005384,   0.0044505,
     0.003517,   0.0028125,   0.002108,   0.002272,   0.002436,   0.002068,
     0.0017,   0.0019855,   0.002271,   0.0020705,   0.00187,   0.0017455,
     0.001621,   0.0017855,   0.00195,   0.0018905,   0.001831,   0.0016705,
     0.00151,   0.0015045,   0.001499,   0.001721,   0.001943,   0.0018305,
     0.001718,   0.002082,   0.002446,   0.002492,   0.002538,   0.002833,
     0.003128,   0.003589,   0.00405,   0.0043915,   0.004733,   0.0047405,
     0.004748,   0.004824,   0.0049,   0.004541,   0.004182,   0.004313,
     0.004444,   0.0046115,   0.004779,   0.005007,   0.005235,   0.005396,
     0.005557,   0.0054025,   0.005248,   0.0047195,   0.004191,   0.0035905,
     0.00299,   0.0028175,   0.002645,   0.0026115,   0.002578,   0.002558,
     0.002538,   0.0027,   0.002862,   0.002721,   0.00258,   0.002973,
     0.003366,   0.004324,   0.005282,   0.006576,   0.00787,   0.008871,
     0.009872,   0.010051,   0.01023,   0.0090465,   0.007863,   0.006367,
     0.004871,   0.0048565,   0.004842,   0.0055935,   0.006345,   0.0069985,
     0.007652,   0.0077885,   0.007925,   0.007197,   0.006469,   0.0054185,
     0.004368,   0.004082,   0.003796,   0.003768,   0.00374,   0.0036695,
     0.003599,   0.0036165,   0.003634,   0.0037785,   0.003923,   0.0042145,
     0.004506,   0.004688,   0.00487,   0.0052705,   0.005671,   0.005878,
     0.006085,   0.006441,   0.006797,   0.007044,   0.007291,   0.007926,
     0.008561,   0.0092905,   0.01002,   0.010625,   0.01123,   0.01174,
     0.01225,   0.012595,   0.01294,   0.01323,   0.01352,   0.01396,
     0.0144,   0.01499,   0.01558,   0.01641,   0.01724,   0.01842,
     0.0196,   0.021835,   0.02407,   0.026755,   0.02944,   0.03189,
     0.03434,   0.03751,   0.04068,   0.04483,   0.04898,   0.05445,
     0.05992,   0.066405,   0.07289,   0.081705,   0.09052,   0.10501,
     0.1195,   0.141,   0.1625,   0.20245,   0.2424,   0.3015,
     0.3606,   0.4268,   0.493,   0.564,   0.635,   0.71665,
     0.7983,   0.8726,   0.9469,   0.97345,   1,   0.99855,
     0.9971,   0.98365,   0.9702,   0.9526,   0.935,   0.91795,
     0.9009,   0.88535,   0.8698,   0.86065,   0.8515,   0.84485,
     0.8382,   0.8352,   0.8322,   0.8289,   0.8256,   0.823,
     0.8204,   0.8137,   0.807,   0.7957,   0.7844,   0.76695,
     0.7495,   0.72675,   0.704,   0.6911,   0.6782,   0.6638,
     0.6494,   0.6313,   0.6132,   0.59005,   0.5669,   0.5493,
     0.5317,   0.5117,   0.4917,   0.4644,   0.4371,   0.3943,
     0.3515,   0.3134,   0.2753,   0.24845,   0.2216,   0.191,
     0.1604,   0.1385,   0.1166,   0.1003,   0.084,   0.07559,
     0.06718,   0.054015,   0.04085,   0.030665,   0.02048,   0.017005,
     0.01353,   0.01178,   0.01003,   0.0089005,   0.007771,   0.007077,
     0.006383,   0.0058745,   0.005366,   0.0049605,   0.004555,   0.0040195,
     0.003484,   0.0031455,   0.002807,   0.002621,   0.002435,   0.0023315,
     0.002228,   0.002158,   0.002088,   0.001973,   0.001858,   0.001839,
     0.00182,   0.0018615,   0.001903,   0.001759,   0.001615,   0.0016735,
     0.001732,   0.0017285,   0.001725,   0.001884,   0.002043,   0.0020925,
     0.002142,   0.0020825,   0.002023,   0.001963,   0.001903,   0.0018045,
     0.001706,   0.0017565,   0.001807,   0.0017525,   0.001698,   0.001849,
     0.002,   0.001,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0
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
