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

#include "otbStringUtils.h"
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

  std::list <std::vector<float> > tmpSpectralBandList;

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
  else if (nbBands == 4)
    {
    //FIXME add other instrument relative spectral response (not only HRG1)
    const float green[541] =
        {
          0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0022771144f,
0.0045542287f,0.021714223f,0.038874217f,0.088104725f,0.13733523f,0.21823871f,
0.29914218f,0.38027802f,0.46141386f,0.52832052f,0.59522718f,0.64936955f,
0.70351192f,0.74529693f,0.78708195f,0.82499008f,0.86289822f,0.891466f,
0.92003378f,0.94254762f,0.96506146f,0.9776788f,0.99029614f,0.99514807f,
1.0f,0.99612511f,0.99225022f,0.9804036f,0.96855699f,0.94715939f,
0.9257618f,0.90154813f,0.87733446f,0.84871261f,0.82009075f,0.78645969f,
0.75282863f,0.71875799f,0.68468734f,0.64818039f,0.61167343f,0.57338572f,
0.53509801f,0.49156491f,0.44803182f,0.39732491f,0.346618f,0.29724838f,
0.24787875f,0.20190886f,0.15593897f,0.12403284f,0.09212672f,0.070920769f,
0.049714818f,0.03798244f,0.026250061f,0.019816831f,0.013383602f,0.010152718f,
0.006921834f,0.0054235249f,0.0039252158f,0.0033455489f,0.002765882f,0.001382941f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f};

    const float red[541] =
        {
          0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0007673711f,
0.0015347422f,0.010419638f,0.019304534f,0.061542786f,0.10378104f,0.20079599f,
0.29781095f,0.41207816f,0.52634536f,0.62039262f,0.71443988f,0.78093055f,
0.84742121f,0.88691213f,0.92640305f,0.95094047f,0.9754779f,0.98546823f,
0.99545857f,0.99772928f,1.0f,0.99374775f,0.98749549f,0.97497667f,
0.96245784f,0.94318133f,0.92390482f,0.89924371f,0.8745826f,0.84464903f,
0.81471546f,0.76863444f,0.72255341f,0.65866954f,0.59478567f,0.5169305f,
0.43907532f,0.36620824f,0.29334116f,0.23579319f,0.17824522f,0.14140026f,
0.1045553f,0.082135956f,0.059716612f,0.047238187f,0.034759762f,0.027242624f,
0.019725486f,0.015547759f,0.011370033f,0.0090187963f,0.0066675595f,0.0050977127f,
0.0035278658f,0.0028316045f,0.0021353432f,0.0010676716f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f
        };
    const float nir[541] =
        {
          0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.00046485267f,
0.00092970533f,0.0022703511f,0.0036109969f,0.0072833599f,0.010955723f,0.020381246f,
0.029806769f,0.050425041f,0.071043314f,0.10698612f,0.14292892f,0.19323862f,
0.24354832f,0.30664343f,0.36973854f,0.43590247f,0.5020664f,0.56601537f,
0.62996434f,0.68195348f,0.73394262f,0.77728387f,0.82062512f,0.85237025f,
0.88411539f,0.91130117f,0.93848695f,0.95316338f,0.96783982f,0.9764626f,
0.98508538f,0.99215522f,0.99922506f,0.99961253f,1.0f,0.99705423f,
0.99410845f,0.98330518f,0.97250191f,0.95825152f,0.94400112f,0.93075194f,
0.91750276f,0.90228886f,0.88707495f,0.87152458f,0.8559742f,0.82966368f,
0.80335316f,0.7684215f,0.73348985f,0.68197206f,0.63045427f,0.56244955f,
0.49444484f,0.43235415f,0.37026347f,0.30941829f,0.24857311f,0.20286212f,
0.15715112f,0.13022816f,0.1033052f,0.084892217f,0.06647923f,0.053553967f,
0.040628703f,0.03389025f,0.027151797f,0.022906605f,0.018661412f,0.015551925f,
0.012442438f,0.010396472f,0.0083505073f,0.0070520644f,0.0057536215f,0.0049600816f,
0.0041665416f,0.0035720051f,0.0029774686f,0.0029639347f,0.0029504007f,0.0029368668f,
0.0029233328f,0.0029097989f,0.0028962649f,0.002882731f,0.0028691971f,0.0028556631f,
0.0028421292f,0.0028285952f,0.0028150613f,0.0028015273f,0.0027879934f,0.0027744594f,
0.0027609255f,0.0027473915f,0.0027338576f,0.0027203236f,0.0027067897f,0.0026932557f,
0.0026797218f,0.0026661878f,0.0026526539f,0.0026391199f,0.002625586f,0.002612052f,
0.0025985181f,0.0025849841f,0.0025714502f,0.0025579162f,0.0025443823f,0.0025308483f,
0.0025173144f,0.0025037804f,0.0024902465f,0.0024767125f,0.0024631786f,0.0024496447f,
0.0024361107f,0.0024225768f,0.0024090428f,0.0023955089f,0.0023819749f,0.002368441f,
0.002354907f,0.0023413731f,0.0023278391f,0.0023143052f,0.0023007712f,0.0022872373f,
0.0022737033f,0.0022601694f,0.0022466354f,0.0022331015f,0.0022195675f,0.0022060336f,
0.0021924996f,0.0021789657f,0.0021654317f,0.0021518978f,0.0021383638f,0.0021248299f,
0.0021112959f,0.002097762f,0.002084228f,0.0020706941f,0.0020571601f,0.0020436262f,
0.0020300923f,0.0020165583f,0.0020030244f,0.0019894904f,0.0019759565f,0.0019624225f,
0.0019488886f,0.0019353546f,0.0019218207f,0.0019082867f,0.0018947528f,0.0018812188f,
0.0018676849f,0.0018541509f,0.001840617f,0.001827083f,0.0018135491f,0.0018000151f,
0.0017864812f,0.0017729472f,0.0017594133f,0.0017458793f,0.0017323454f,0.0017188114f,
0.0017052775f,0.0016917435f,0.0016782096f,0.0016646756f,0.0016511417f,0.0016376078f,
0.0016240738f,0.0016105399f,0.0015970059f,0.001583472f,0.001569938f,0.0015564041f,
0.0015428701f,0.0015293362f,0.0015158022f,0.0015022683f,0.0014887343f,0.0014752004f,
0.0014616664f,0.0014481325f,0.0014345985f,0.0014210646f,0.0014075306f,0.0013939967f,
0.0013804627f,0.0013669288f,0.0013533948f,0.0013398609f,0.0013263269f,0.001312793f,
0.001299259f,0.0012857251f,0.0012721911f,0.0012586572f,0.0012451232f,0.0012315893f,
0.0012180554f,0.0012045214f,0.0011909875f,0.0011774535f,0.0011639196f,0.0011503856f,
0.0011368517f,0.0011233177f,0.0011097838f,0.0010962498f,0.0010827159f,0.0010691819f,
0.001055648f,0.001042114f,0.0010285801f,0.0010150461f,0.0010015122f,0.00098797823f,
0.00097444428f,0.00096091033f,0.00094737638f,0.00093384244f,0.00092030849f,0.00090677454f,
0.00089324059f,0.00087970664f,0.00086617269f,0.00085263875f,0.0008391048f,0.00082557085f,
0.0008120369f,0.00079850295f,0.000784969f,0.00077143506f,0.00075790111f,0.00074436716f,
0.00073083321f,0.00071729926f,0.00070376531f,0.00069023137f,0.00067669742f,0.00066316347f,
0.00064962952f,0.00063609557f,0.00062256162f,0.00060902768f,0.00059549373f,0.00058195978f,
0.00056842583f,0.00055489188f,0.00054135793f,0.00052782399f,0.00051429004f,0.00050075609f,
0.00048722214f,0.00047368819f,0.00046015424f,0.0004466203f,0.00043308635f,0.0004195524f,
0.00040601845f,0.0003924845f,0.00037895055f,0.00036541661f,0.00035188266f,0.00033834871f,
0.00032481476f,0.00031128081f,0.00029774686f,0.00028421292f,0.00027067897f,0.00025714502f,
0.00024361107f,0.00023007712f,0.00021654317f,0.00020300923f,0.00018947528f,0.00017594133f,
0.00016240738f,0.00014887343f,0.00013533948f,0.00012180554f,0.00010827159f,9.4737638e-05f,
8.120369e-05f,6.7669742e-05f,5.4135793e-05f,4.0601845e-05f,2.7067897e-05f,1.3533948e-05f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f
        };
    const float mir[541] =
        {
          0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
0.0f,0.0088105211f,0.017621042f,0.020528086f,0.02343513f,0.026392526f,
0.029349921f,0.033664215f,0.037978508f,0.042245031f,0.046511554f,0.055648902f,
0.064786249f,0.07395196f,0.08311767f,0.10386151f,0.12460535f,0.14594447f,
0.16728359f,0.20442288f,0.24156217f,0.27841272f,0.31526328f,0.35919335f,
0.40312341f,0.44621342f,0.48930344f,0.53207715f,0.57485086f,0.61905294f,
0.66325502f,0.7114532f,0.75965137f,0.8076253f,0.85559924f,0.89376042f,
0.93192161f,0.9659608f,1.0f,0.99696894f,0.99393787f,0.99224825f,
0.99055862f,0.96192825f,0.93329788f,0.90629723f,0.87929659f,0.87156435f,
0.8638321f,0.85303543f,0.84223875f,0.85502674f,0.86781473f,0.88056734f,
0.89331995f,0.89301593f,0.89271191f,0.86628263f,0.83985335f,0.80572933f,
0.77160531f,0.73099612f,0.69038694f,0.64866608f,0.60694523f,0.56782227f,
0.52869932f,0.48719362f,0.44568791f,0.40599882f,0.36630972f,0.32807542f,
0.28984112f,0.25834284f,0.22684456f,0.19656397f,0.16628339f,0.14643708f,
0.12659078f,0.106444f,0.086297227f,0.076081433f,0.065865639f,0.055683249f,
0.045500859f,0.040629869f,0.03575888f,0.031127116f,0.026495353f,0.024316707f,
0.022138061f,0.019934653f,0.017731245f,0.01619062f,0.014649995f,0.013104617f,
0.011559239f,0.01030835f,0.0090574603f,0.0078583703f,0.0066592804f,0.0059099939f,
0.0051607074f,0.0044159179f,0.0036711284f,0.0033605972f,0.0030500659f,0.0027368724f,
0.0024236788f,0.0021708901f,0.0019181013f,0.0016732152f,0.001428329f,0.001428329f,
0.001428329f
        };

    //Add multispectral bands to the temporary list
    const std::vector<float> vgreen (green, green + sizeof(green) / sizeof(float) );
    const std::vector<float> vred (red, red + sizeof(red) / sizeof(float) );
    const std::vector<float> vnir (nir, nir + sizeof(nir) / sizeof(float) );
    const std::vector<float> vmir (mir, mir + sizeof(mir) / sizeof(float) );

    tmpSpectralBandList.push_back(vgreen);
    tmpSpectralBandList.push_back(vred);
    tmpSpectralBandList.push_back(vnir);
    tmpSpectralBandList.push_back(vmir);

    unsigned int j = 0;
  for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
    {
    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.45);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.8);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.0025);
    ++j;
    }

    }
  else
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  return wavelengthSpectralBand;
}

} // end namespace otb
