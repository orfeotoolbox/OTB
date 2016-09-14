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

#include "otbFormosatImageMetadataInterface.h"

#include "otbMacro.h"
#include "otbMath.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbStringUtils.h"

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

  if (imageKeywordlist.HasKey("support_data.instrument"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument");
    return valueString;
    }

  return "";
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

  if (imageKeywordlist.HasKey("support_data.instrument_index"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument_index");
    unsigned int value = atoi(valueString.c_str());
    return value;
    }

  return -1; // Invalid value
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
  else
    {
    itkExceptionMacro("Invalid Solar Irradiance");
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
    itkExceptionMacro("Invalid Physical Bias");
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

  if (!imageKeywordlist.HasKey("support_data.incident_angle"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.incident_angle");
  double value = 90 - atof(valueString.c_str());
  return value;
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

 if (!imageKeywordlist.HasKey("support_data.incident_angle"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.sat_azimuth_angle");
  double satAz = atof(valueString.c_str());

  // In some software version, a bug exists.
  // We have to check the version t correct the satellite azimuth angle contained in the metadata
  std::string softVersion = imageKeywordlist.GetMetadataByKey("support_data.software_version");

  if( (softVersion == "R2P_02.03.P1") || (softVersion == "R2P_02.02") || (softVersion == "R2P_02.03") )
    {
      if (!imageKeywordlist.HasKey("support_data.viewing_angle_across_track"))
        {
          return 0;
        }
      if (!imageKeywordlist.HasKey("support_data.viewing_angle_along_track"))
        {
          return 0;
        }
      if (!imageKeywordlist.HasKey("support_data.scene_orientation"))
        {
          return 0;
        }

      valueString = imageKeywordlist.GetMetadataByKey("support_data.viewing_angle_across_track");
      double viewingAngleAcrossTrack( atof(valueString.c_str()) );
      valueString = imageKeywordlist.GetMetadataByKey("support_data.viewing_angle_along_track");
      double viewingAngleAlongTrack( atof(valueString.c_str()) );
      valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");
      double sceneOrientation( atof(valueString.c_str()) );

      double alpha = vcl_atan( vcl_tan( viewingAngleAcrossTrack * CONST_PI_180 ) /  vcl_tan( viewingAngleAlongTrack * CONST_PI_180 ) ) * CONST_180_PI;

      if( viewingAngleAlongTrack < 0 )
        {
          if (alpha >0)
            {
              alpha = alpha - 180;
            }
          else
            {
              alpha = alpha + 180;
            }
        }

      alpha -= sceneOrientation;
      if (alpha >0)
        {
          satAz += 180;
        }
      else
        {
          satAz = 180 - satAz;
        }

    }

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

  int         nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    // FIXME that's definitely NOT correct in a formosat file!!!
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

  int nbBands = this->GetNumberOfBands();
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

  int nbBands = this->GetNumberOfBands();
  std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    const float b0[281] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.000219376f,   0.000219425f,   0.000231534f,   0.000284519f,
     0.000550079f,   0.001f,   0.003f,   0.006f,   0.012f,   0.0235f,
     0.035f,   0.043f,   0.0503893f,   0.0525f,   0.055f,   0.0565f,
     0.058f,   0.059f,   0.059f,   0.0608594f,   0.061f,   0.0615f,
     0.062f,   0.063f,   0.063f,   0.064f,   0.063f,   0.064f,
     0.065f,   0.066f,   0.066f,   0.066f,   0.066f,   0.0675f,
     0.069f,   0.0708104f,   0.0709615f,   0.0709001f,   0.071f,   0.0715f,
     0.073f,   0.074f,   0.074f,   0.073f,   0.073f,   0.0735f,
     0.075f,   0.0765f,   0.078f,   0.078f,   0.078f,   0.0785f,
     0.0800145f,   0.0815f,   0.083f,   0.083f,   0.083f,   0.083f,
     0.083f,   0.0835f,   0.085f,   0.0865f,   0.087f,   0.088f,
     0.087f,   0.087f,   0.088f,   0.0885f,   0.0907023f,   0.0915f,
     0.092f,   0.092f,   0.092f,   0.092f,   0.093f,   0.094f,
     0.095f,   0.096f,   0.096f,   0.096f,   0.095f,   0.095f,
     0.095f,   0.095f,   0.096f,   0.0975f,   0.098f,   0.099f,
     0.099f,   0.098f,   0.098f,   0.0965f,   0.096f,   0.0965f,
     0.098f,   0.0985f,   0.099f,   0.0995f,   0.099f,   0.0975f,
     0.097f,   0.0955f,   0.095f,   0.095f,   0.095f,   0.0965f,
     0.097f,   0.098f,   0.098f,   0.0975f,   0.097f,   0.096f,
     0.095f,   0.095f,   0.094f,   0.094f,   0.095f,   0.0955f,
     0.096f,   0.097f,   0.097f,   0.097f,   0.097f,   0.097f,
     0.097f,   0.096f,   0.097f,   0.097f,   0.097f,   0.098f,
     0.098f,   0.096f,   0.095f,   0.094f,   0.093f,   0.092f,
     0.091f,   0.0905658f,   0.089f,   0.089f,   0.089f,   0.088f,
     0.088f,   0.0875f,   0.087f,   0.086f,   0.085f,   0.084f,
     0.083f,   0.0825f,   0.082f,   0.081f,   0.081f,   0.0804051f,
     0.079f,   0.078f,   0.077f,   0.077f,   0.076f,   0.075f,
     0.074f,   0.0735f,   0.073f,   0.0725f,   0.072f,   0.071f,
     0.071f,   0.0704136f,   0.069f,   0.0685f,   0.068f,   0.067f,
     0.066f,   0.0645f,   0.063f,   0.062f,   0.0607892f,   0.059f,
     0.058f,   0.057f,   0.056f,   0.0545f,   0.052f,   0.0455f,
     0.038f,   0.0286994f,   0.0202138f,   0.0125f,   0.007f,   0.004f,
     0.002f,   0.001f,   0.000909856f,   0.000512159f,   0.000357051f,   0.00029112f,
     0.000215752f,   0.000187213f,   0.000171918f,   0.000169724f,   0.000166392f,   0.000163058f,
     0.000159726f,   0.000156393f,   0.000153061f,   0.000149728f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f
    };

    //add panchromatic band to the temporary list
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
    tmpSpectralBandList.push_back(vb0);
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    const float b1[281] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.000354959f,   0.000354603f,   0.000376745f,   0.000478212f,
     0.0009718f,   0.002f,   0.005f,   0.0124297f,   0.024f,   0.0415f,
     0.057f,   0.0685f,   0.074f,   0.074f,   0.076f,   0.0795695f,
     0.082f,   0.084f,   0.083f,   0.0825f,   0.086f,   0.0910953f,
     0.094f,   0.096f,   0.096f,   0.094f,   0.094f,   0.096f,
     0.099f,   0.0995f,   0.099f,   0.098f,   0.095f,   0.092f,
     0.095f,   0.0985f,   0.092f,   0.0695f,   0.037f,   0.025f,
     0.009f,   0.0025f,   0.001f,   0.000847053f,   0.000588401f,   0.00051966f,
     0.00050602f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0
    };

    const float b2[281] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.000324677f,   0.000326475f,   0.000331943f,   0.000386303f,
     0.000447683f,   0.001f,   0.003f,   0.0045f,   0.012f,   0.0205f,
     0.036f,   0.0615f,   0.075f,   0.081f,   0.082f,   0.0845f,
     0.087f,   0.0885f,   0.088f,   0.087f,   0.086f,   0.0875f,
     0.0908484f,   0.0925f,   0.094f,   0.094f,   0.093f,   0.0925f,
     0.093f,   0.0955f,   0.097f,   0.098f,   0.099f,   0.099f,
     0.099f,   0.099f,   0.099f,   0.099f,   0.095f,   0.0815f,
     0.057f,   0.032f,   0.018f,   0.0112534f,   0.005f,   0.0015f,
     0.000758484f,   0.000604297f,   0.000512471f,   0.000475316f,   0.000453283f,   0.00044559f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0
    };

    const float b3[281] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.000461752f,
     0.001f,   0.001f,   0.003f,   0.006f,   0.013f,   0.0335f,
     0.063f,   0.089f,   0.098f,   0.099f,   0.099f,   0.099f,
     0.099f,   0.0985f,   0.097f,   0.0945f,   0.092f,   0.0906796f,
     0.089f,   0.0907659f,   0.093f,   0.0965f,   0.1f,   0.097f,
     0.091f,   0.0865f,   0.086f,   0.0910438f,   0.094f,   0.092f,
     0.093f,   0.088f,   0.064f,   0.034f,   0.015f,   0.0075f,
     0.006f,   0.0045f,   0.003f,   0.001f,   0.000607601f,   0.000202927f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0
    };

    const float b4[281] =
    {
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.000481456f,   0.00071923f,   0.00098303f,   0.001f,   0.000947915f,
     0.00090899f,   0.000887367f,   0.000576847f,   0.000470858f,   0.00052246f,   0.000713811f,
     0.001f,   0.0015f,   0.003f,   0.005f,   0.008f,   0.0145f,
     0.022f,   0.035f,   0.049f,   0.0595f,   0.075f,   0.0905f,
     0.098f,   0.0995f,   0.097f,   0.093f,   0.091f,   0.0925f,
     0.094f,   0.096f,   0.096f,   0.0955f,   0.094f,   0.092f,
     0.0907811f,   0.089f,   0.088f,   0.088f,   0.088f,   0.088f,
     0.088f,   0.0875f,   0.087f,   0.086f,   0.085f,   0.084f,
     0.083f,   0.083f,   0.082f,   0.081f,   0.0806396f,   0.079f,
     0.078f,   0.077f,   0.076f,   0.075f,   0.074f,   0.073f,
     0.072f,   0.071f,   0.0700369f,   0.0685f,   0.067f,   0.0655f,
     0.064f,   0.063f,   0.063f,   0.062f,   0.059f,   0.054f,
     0.043f,   0.034f,   0.025f,   0.016f,   0.009f,   0.0055f,
     0.003f,   0.0015f,   0.001f,   0.000691333f,   0.000432126f,   0.000356974f,
     0.000265441f,   0.000219773f,   0.000195346f,   0.000192716f,   0.000188932f,   0.000185148f,
     0.000181364f,   0.00017758f,   0.000173796f,   0.000170011f,   0.000166227f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
     0.0f,   0.0f,   0.0f,   0.0f,   0
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
