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

#include "otbPleiadesImageMetadataInterface.h"

#include <boost/algorithm/string.hpp>
#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include <boost/lexical_cast.hpp>


namespace otb
{
using boost::lexical_cast;
using boost::bad_lexical_cast;

PleiadesImageMetadataInterface
::PleiadesImageMetadataInterface()
{
}

bool
PleiadesImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("PHR") != std::string::npos) return true;
  else return false;
}

std::string
PleiadesImageMetadataInterface::GetInstrument() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

std::string
PleiadesImageMetadataInterface::GetInstrumentIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (imageKeywordlist.HasKey("support_data.instrument_index"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.instrument_index");
    return valueString;
    }

  return ""; // Invalid value
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Solar Irradiance");
    }

  return outputValuesVariableLengthVector;
}

int
PleiadesImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[2]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Day");
    }

  return value;
}

int
PleiadesImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[1]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Month");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[0]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[3]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Hour");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-."));

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[4]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Minute");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[2]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Day");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[1]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Month");
    }
  return value;
}

int
PleiadesImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  int value;
  try
    {
    value = lexical_cast<int> (outputValues[0]);
    }
  catch (bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Bias");
    }

  return outputValuesVariableLengthVector;
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface
::GetPhysicalGain() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  if (outputValues.size() == 1)
    {
    //this is a PAN image
    outputValuesVariableLengthVector[0] = outputValues[0];
    }
  else if (outputValues.size() == 3)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    }
  else if (outputValues.size() == 4)
    {
    outputValuesVariableLengthVector[0] = outputValues[0];
    outputValuesVariableLengthVector[1] = outputValues[1];
    outputValuesVariableLengthVector[2] = outputValues[2];
    outputValuesVariableLengthVector[3] = outputValues[3];
    }
  else
    {
    itkExceptionMacro("Invalid Physical Gain");
    }

  return outputValuesVariableLengthVector;
}

double
PleiadesImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
  double value = 90 - atof(valueString.c_str()); // TODO MSD ????
  return value;
}

double
PleiadesImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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

  // TODO MSD ????
  if ((step - 48) < 0)
    {
    satAz += 90.;
    }
  else satAz = satAz - 90.;

  return satAz;
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
    wavel.Fill(-500.0); // TODO MSD Fill this value !!!
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    wavel.SetSize(4);
    wavel[0] = 0.440;
    wavel[1] = 0.500;
    wavel[2] = 0.610;
    wavel[3] = 0.770;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
    wavel.Fill(-500.0); // TODO MSD Fill this value !!!
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    //FIXME is that supposed to correspond to the bands in the files?
    wavel.SetSize(4);
    wavel[0] = 0.540;
    wavel[1] = 0.600;
    wavel[2] = 0.710;
    wavel[3] = 0.910;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

unsigned int
PleiadesImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const
{
  otbMsgDevMacro(<< "Pleiades detected: band 0 and 2 inverted");
  if (i == 0) return 2;
  if (i == 2) return 0;
  return i;
}

std::vector<unsigned int>
PleiadesImageMetadataInterface
::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 1;
  rgb[2] = 2;
  return rgb;
}

PleiadesImageMetadataInterface::WavelengthSpectralBandVectorType
PleiadesImageMetadataInterface
::GetSpectralSensitivity()  const
{
  //TODO tabulate spectral responses
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Pleiades Image");
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
    if (sensorId == "PHR 1A")
      {

      }
      else
      {
      itkExceptionMacro(<< "Invalid Pleiades Sensor ID");
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
