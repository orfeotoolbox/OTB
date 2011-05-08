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
FormosatImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("Formosat") != std::string::npos) return true;
  else return false;
}

std::string
FormosatImageMetadataInterface::GetInstrument() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetInstrumentIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
  //In the case of FORMOSAT-2, the bands are in a different order:
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
::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
FormosatImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid metadata, no Formosat2 Image");
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
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "SPOT4") wavel.Fill(0.610);
    else if (sensorId == "SPOT5") wavel.Fill(0.480);
    else itkExceptionMacro(<< "Invalid Formosat2 Sensor ID");
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
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid metadata, no Formosat2 Image");
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
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "SPOT4") wavel.Fill(0.680);
    else if (sensorId == "SPOT5") wavel.Fill(0.710);
    else itkExceptionMacro(<< "Invalid Formosat2 Sensor ID");
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
  otbMsgDevMacro(<< "Formosat2 detected: band 0 and 2 inverted");
  if (i == 0) return 2;
  if (i == 2) return 0;
  return i;
}

std::vector<unsigned int>
FormosatImageMetadataInterface::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 1;
  rgb[2] = 2;
  return rgb;
}

FormosatImageMetadataInterface::WavelengthSpectralBandVectorType
FormosatImageMetadataInterface
::GetSpectralSensitivity()  const
{
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();
  std::list <std::vector<float> > tmpSpectralBandList;

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
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
  std::string key = "support_data.number_bands";
  int         nbBands = ossimString(kwl.find(key.c_str())).toInt();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    const float b0[281] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0.000219376,   0.000219425,   0.000231534,   0.000284519,
     0.000550079,   0.001,   0.003,   0.006,   0.012,   0.0235,
     0.035,   0.043,   0.0503893,   0.0525,   0.055,   0.0565,
     0.058,   0.059,   0.059,   0.0608594,   0.061,   0.0615,
     0.062,   0.063,   0.063,   0.064,   0.063,   0.064,
     0.065,   0.066,   0.066,   0.066,   0.066,   0.0675,
     0.069,   0.0708104,   0.0709615,   0.0709001,   0.071,   0.0715,
     0.073,   0.074,   0.074,   0.073,   0.073,   0.0735,
     0.075,   0.0765,   0.078,   0.078,   0.078,   0.0785,
     0.0800145,   0.0815,   0.083,   0.083,   0.083,   0.083,
     0.083,   0.0835,   0.085,   0.0865,   0.087,   0.088,
     0.087,   0.087,   0.088,   0.0885,   0.0907023,   0.0915,
     0.092,   0.092,   0.092,   0.092,   0.093,   0.094,
     0.095,   0.096,   0.096,   0.096,   0.095,   0.095,
     0.095,   0.095,   0.096,   0.0975,   0.098,   0.099,
     0.099,   0.098,   0.098,   0.0965,   0.096,   0.0965,
     0.098,   0.0985,   0.099,   0.0995,   0.099,   0.0975,
     0.097,   0.0955,   0.095,   0.095,   0.095,   0.0965,
     0.097,   0.098,   0.098,   0.0975,   0.097,   0.096,
     0.095,   0.095,   0.094,   0.094,   0.095,   0.0955,
     0.096,   0.097,   0.097,   0.097,   0.097,   0.097,
     0.097,   0.096,   0.097,   0.097,   0.097,   0.098,
     0.098,   0.096,   0.095,   0.094,   0.093,   0.092,
     0.091,   0.0905658,   0.089,   0.089,   0.089,   0.088,
     0.088,   0.0875,   0.087,   0.086,   0.085,   0.084,
     0.083,   0.0825,   0.082,   0.081,   0.081,   0.0804051,
     0.079,   0.078,   0.077,   0.077,   0.076,   0.075,
     0.074,   0.0735,   0.073,   0.0725,   0.072,   0.071,
     0.071,   0.0704136,   0.069,   0.0685,   0.068,   0.067,
     0.066,   0.0645,   0.063,   0.062,   0.0607892,   0.059,
     0.058,   0.057,   0.056,   0.0545,   0.052,   0.0455,
     0.038,   0.0286994,   0.0202138,   0.0125,   0.007,   0.004,
     0.002,   0.001,   0.000909856,   0.000512159,   0.000357051,   0.00029112,
     0.000215752,   0.000187213,   0.000171918,   0.000169724,   0.000166392,   0.000163058,
     0.000159726,   0.000156393,   0.000153061,   0.000149728,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0
    };

    //add panchromatic band to the temporary list
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
    tmpSpectralBandList.push_back(vb0);
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    const float b1[281] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0.000354959,   0.000354603,   0.000376745,   0.000478212,
     0.0009718,   0.002,   0.005,   0.0124297,   0.024,   0.0415,
     0.057,   0.0685,   0.074,   0.074,   0.076,   0.0795695,
     0.082,   0.084,   0.083,   0.0825,   0.086,   0.0910953,
     0.094,   0.096,   0.096,   0.094,   0.094,   0.096,
     0.099,   0.0995,   0.099,   0.098,   0.095,   0.092,
     0.095,   0.0985,   0.092,   0.0695,   0.037,   0.025,
     0.009,   0.0025,   0.001,   0.000847053,   0.000588401,   0.00051966,
     0.00050602,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0
    };

    const float b2[281] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0.000324677,   0.000326475,   0.000331943,   0.000386303,
     0.000447683,   0.001,   0.003,   0.0045,   0.012,   0.0205,
     0.036,   0.0615,   0.075,   0.081,   0.082,   0.0845,
     0.087,   0.0885,   0.088,   0.087,   0.086,   0.0875,
     0.0908484,   0.0925,   0.094,   0.094,   0.093,   0.0925,
     0.093,   0.0955,   0.097,   0.098,   0.099,   0.099,
     0.099,   0.099,   0.099,   0.099,   0.095,   0.0815,
     0.057,   0.032,   0.018,   0.0112534,   0.005,   0.0015,
     0.000758484,   0.000604297,   0.000512471,   0.000475316,   0.000453283,   0.00044559,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0
    };

    const float b3[281] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0.000461752,
     0.001,   0.001,   0.003,   0.006,   0.013,   0.0335,
     0.063,   0.089,   0.098,   0.099,   0.099,   0.099,
     0.099,   0.0985,   0.097,   0.0945,   0.092,   0.0906796,
     0.089,   0.0907659,   0.093,   0.0965,   0.1,   0.097,
     0.091,   0.0865,   0.086,   0.0910438,   0.094,   0.092,
     0.093,   0.088,   0.064,   0.034,   0.015,   0.0075,
     0.006,   0.0045,   0.003,   0.001,   0.000607601,   0.000202927,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0
    };

    const float b4[281] =
    {
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0.000481456,   0.00071923,   0.00098303,   0.001,   0.000947915,
     0.00090899,   0.000887367,   0.000576847,   0.000470858,   0.00052246,   0.000713811,
     0.001,   0.0015,   0.003,   0.005,   0.008,   0.0145,
     0.022,   0.035,   0.049,   0.0595,   0.075,   0.0905,
     0.098,   0.0995,   0.097,   0.093,   0.091,   0.0925,
     0.094,   0.096,   0.096,   0.0955,   0.094,   0.092,
     0.0907811,   0.089,   0.088,   0.088,   0.088,   0.088,
     0.088,   0.0875,   0.087,   0.086,   0.085,   0.084,
     0.083,   0.083,   0.082,   0.081,   0.0806396,   0.079,
     0.078,   0.077,   0.076,   0.075,   0.074,   0.073,
     0.072,   0.071,   0.0700369,   0.0685,   0.067,   0.0655,
     0.064,   0.063,   0.063,   0.062,   0.059,   0.054,
     0.043,   0.034,   0.025,   0.016,   0.009,   0.0055,
     0.003,   0.0015,   0.001,   0.000691333,   0.000432126,   0.000356974,
     0.000265441,   0.000219773,   0.000195346,   0.000192716,   0.000188932,   0.000185148,
     0.000181364,   0.00017758,   0.000173796,   0.000170011,   0.000166227,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0
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

    unsigned int j = 0;
    for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
      {
      wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
      wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
      wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.300);
      wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.0);
      wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.0025);
      ++j;
      }
    }

  return wavelengthSpectralBand;
}

} // end namespace otb
