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

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbFormosatImageMetadataInterface.h"
#include "itkMetaDataObject.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

FormosatImageMetadataInterface
::FormosatImageMetadataInterface()
{
}

bool
FormosatImageMetadataInterface::CanRead(const MetaDataDictionaryType& dict) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("Formosat") != std::string::npos) return true;
  else return false;
}

std::string
FormosatImageMetadataInterface::GetInstrument(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key = "support_data.instrument";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString;
}

unsigned int
FormosatImageMetadataInterface::GetInstrumentIndex(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key = "support_data.instrument_index";
  ossimString keywordString = kwl.find(key.c_str());

  return static_cast<unsigned int>(keywordString.toUInt32());
}

FormosatImageMetadataInterface::VariableLengthVectorType
FormosatImageMetadataInterface::GetSolarIrradiance(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  ossimKeywordlist         kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string              key = "support_data.solar_irradiance";
  ossimString              keywordString = kwl.find(key.c_str());
  ossimString              separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<double>      outputValues;
  for (unsigned int i = 0; i < keywordStrings.size(); ++i)
    {
    if (!keywordStrings[i].empty())
      {
      outputValues.push_back(keywordStrings[i].toDouble());
      }
    }

  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);
  //In the case of SPOT, the bands are in a different order:
  // XS3, XS2. XS1, SWIR in the tif file.
  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

int
FormosatImageMetadataInterface::GetDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.image_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
FormosatImageMetadataInterface::GetMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.image_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
FormosatImageMetadataInterface::GetYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.image_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

int
FormosatImageMetadataInterface::GetHour(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.image_date";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Hour");

  ossimString hour = keywordStrings[3];

  return hour.toInt();
}

int
FormosatImageMetadataInterface::GetMinute(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.image_date";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Minute");

  ossimString minute = keywordStrings[4];

  return minute.toInt();
}

int
FormosatImageMetadataInterface::GetProductionDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.production_date";
  separatorList = "-T:";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
FormosatImageMetadataInterface::GetProductionMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.production_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
FormosatImageMetadataInterface::GetProductionYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "support_data.production_date";
  separatorList = "-T";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

FormosatImageMetadataInterface::VariableLengthVectorType
FormosatImageMetadataInterface
::GetPhysicalBias(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string              key = "support_data.physical_bias";
  ossimString              keywordString = kwl.find(key.c_str());
  ossimString              separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<double>      outputValues;
  for (unsigned int i = 0; i < keywordStrings.size(); ++i)
    {
    if (!keywordStrings[i].empty())
      {
      outputValues.push_back(keywordStrings[i].toDouble());
      }
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);
  //In the case of SPOT, the bands are in a different order:
  // XS3, XS2. XS1, SWIR in the tif file.

  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

FormosatImageMetadataInterface::VariableLengthVectorType
FormosatImageMetadataInterface
::GetPhysicalGain(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string              key = "support_data.physical_gain";
  ossimString              keywordString = kwl.find(key.c_str());
  ossimString              separatorList = " ";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  std::vector<double>      outputValues;
  for (unsigned int i = 0; i < keywordStrings.size(); ++i)
    {
    if (!keywordStrings[i].empty())
      {
      outputValues.push_back(keywordStrings[i].toDouble());
      }
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);
  //In the case of SPOT, the bands are in a different order:
  // XS3, XS2. XS1, SWIR in the tif file.

  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

double
FormosatImageMetadataInterface::GetSatElevation(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.incident_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return (90. - keywordString.toDouble());
}

double
FormosatImageMetadataInterface::GetSatAzimuth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.step_count";
  ossimString keywordString = kwl.find(key.c_str());
  int         step = keywordString.toInt();

  key = "support_data.scene_orientation";
  keywordString = kwl.find(key.c_str());
  double satAz = keywordString.toDouble();

  if ((step - 48) < 0)
    {
    satAz += 90.;
    }
  else satAz = satAz - 90.;

  return satAz;
}

FormosatImageMetadataInterface::VariableLengthVectorType
FormosatImageMetadataInterface
::GetFirstWavelengths(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.number_bands";
  int         nbBands = ossimString(kwl.find(key.c_str())).toInt();
  std::string sensorId = this->GetSensorID(dict);

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "SPOT4") wavel.Fill(0.610);
    else if (sensorId == "SPOT5") wavel.Fill(0.480);
    else itkExceptionMacro(<< "Invalid Formosat Sensor ID");
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    wavel.SetSize(4);
    //FIXME is that supposed to correspond to the bands in the files?
    wavel[0] = 0.500;
    wavel[1] = 0.610;
    wavel[2] = 0.780;
    wavel[3] = 1.580;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

FormosatImageMetadataInterface::VariableLengthVectorType
FormosatImageMetadataInterface
::GetLastWavelengths(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Formosat Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.number_bands";
  int         nbBands = ossimString(kwl.find(key.c_str())).toInt();
  std::string sensorId = this->GetSensorID(dict);

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "SPOT4") wavel.Fill(0.680);
    else if (sensorId == "SPOT5") wavel.Fill(0.710);
    else itkExceptionMacro(<< "Invalid Formosat Sensor ID");
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    //FIXME is that supposed to correspond to the bands in the files?
    wavel.SetSize(4);
    wavel[0] = 0.590;
    wavel[1] = 0.680;
    wavel[2] = 0.890;
    wavel[3] = 1.750;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

unsigned int
FormosatImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const
{
  otbMsgDevMacro(<< "SPOT detected: band 0 and 2 inverted");
  if (i == 0) return 2;
  if (i == 2) return 0;
  return i;
}

unsigned int
FormosatImageMetadataInterface
::GetDefaultRBand() const
{
  return 0;
}

unsigned int
FormosatImageMetadataInterface
::GetDefaultGBand() const
{
  return 1;
}

unsigned int
FormosatImageMetadataInterface
::GetDefaultBBand() const
{
  return 2;
}

} // end namespace otb
