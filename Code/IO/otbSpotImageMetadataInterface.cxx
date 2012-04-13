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

#include "otbSpotImageMetadataInterface.h"

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

SpotImageMetadataInterface
::SpotImageMetadataInterface()
{
}

bool
SpotImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("Spot") != std::string::npos) return true;
  else return false;
}

std::string
SpotImageMetadataInterface::GetInstrument() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (imageKeywordlist.HasKey("support_data.instrument"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument");
    return valueString;
    }

  return "";
}

unsigned int
SpotImageMetadataInterface::GetInstrumentIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (imageKeywordlist.HasKey("support_data.instrument_index"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument_index");
    unsigned int value = atoi(valueString.c_str());
    return value;
    }

  return -1; // Invalid value
}

SpotImageMetadataInterface::VariableLengthVectorType
SpotImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.solar_irradiance"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.solar_irradiance");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
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
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Irradiance");
    }

  return outputValuesVariableLengthVector;
}

int
SpotImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() < 4) itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[3].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.image_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.image_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() < 5) itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[4].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
SpotImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.production_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.production_date");
  std::vector<std::string> outputValues;
  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}

SpotImageMetadataInterface::VariableLengthVectorType
SpotImageMetadataInterface
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.physical_bias"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.physical_bias");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
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
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

SpotImageMetadataInterface::VariableLengthVectorType
SpotImageMetadataInterface
::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  std::vector<double>      outputValues;
  if (imageKeywordlist.HasKey("support_data.physical_gain"))
    {
    std::vector<std::string> outputValuesString;
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.physical_gain");
    boost::trim(valueString);
    boost::split(outputValuesString, valueString, boost::is_any_of(" "));
    for (unsigned int i = 0; i < outputValuesString.size(); ++i)
      {
      outputValues.push_back(atof(outputValuesString[i].c_str()));
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
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[2];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[0];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

double
SpotImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.incident_angle"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.incident_angle");
  double value = 90 - atof(valueString.c_str());
  return value;
}

double
SpotImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  if (!imageKeywordlist.HasKey("support_data.step_count") ||
      !imageKeywordlist.HasKey("support_data.scene_orientation"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.step_count");
  int step = atoi(valueString.c_str());
  valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");
  double satAz = atof(valueString.c_str());

  if ((step - 48) < 0)
    {
    satAz += 90.;
    }
  else satAz = satAz - 90.;

  return satAz;
}

SpotImageMetadataInterface::VariableLengthVectorType
SpotImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  int         nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "Spot 4") wavel.Fill(0.610);
    else if (sensorId == "Spot 5") wavel.Fill(0.475);
    else wavel.Fill(0.500); // assume SPOT 123
      //else itkExceptionMacro(<< "Invalid Spot Sensor ID");
    }
  else if (nbBands == 4)
    {
    wavel.SetSize(4);
    //FIXME is that supposed to correspond to the bands in the files?
    wavel[0] = 0.500;
    wavel[1] = 0.610;
    wavel[2] = 0.780;
    wavel[3] = 1.580;
    }
  else if (nbBands == 3)
    {
    wavel.SetSize(3);
    //FIXME is that supposed to correspond to the bands in the files?
    wavel[0] = 0.500;
    wavel[1] = 0.610;
    wavel[2] = 0.780;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

SpotImageMetadataInterface::VariableLengthVectorType
SpotImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType wavel(1);
  wavel.Fill(0.);

  int nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    if (sensorId == "Spot 4") wavel.Fill(0.680);
    else if (sensorId == "Spot 5") wavel.Fill(0.710);
    else wavel.Fill(0.730); // assume SPOT 123
      // else itkExceptionMacro(<< "Invalid Spot Sensor ID");
    }
  else if (nbBands == 4)
    {
    //FIXME is that supposed to correspond to the bands in the files?
    wavel.SetSize(4);
    wavel[0] = 0.590;
    wavel[1] = 0.680;
    wavel[2] = 0.890;
    wavel[3] = 1.750;
    }
  else if (nbBands == 3)
    {
    wavel.SetSize(3);
    wavel[0] = 0.590;
    wavel[1] = 0.680;
    wavel[2] = 0.890;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

unsigned int
SpotImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const
{
  otbMsgDevMacro(<< "SPOT detected: band 0 and 2 inverted");
  if (i == 0) return 2;
  if (i == 2) return 0;
  return i;
}

std::vector<unsigned int>
SpotImageMetadataInterface
::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 1;
  rgb[2] = 2;
  return rgb;
}

SpotImageMetadataInterface::WavelengthSpectralBandVectorType
SpotImageMetadataInterface
::GetSpectralSensitivity()  const
{
  //TODO tabulate spectral responses
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  int nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    if (sensorId == "Spot 4")
      {

      }
    else if (sensorId == "Spot 5")
      {

      }
    else
      {
      itkExceptionMacro(<< "Invalid Spot Sensor ID");
      }
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    //FIXME add other instrument relative spectral response (not only HRG)
    }
  else
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  return wavelengthSpectralBand;
}

} // end namespace otb
