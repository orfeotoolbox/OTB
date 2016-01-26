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

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbStringUtils.h"

//useful constants
#include <otbMath.h>

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
  if (sensorID.find("PHR") != std::string::npos)
    return true;
  else
    return false;
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

  // Check that values read from metadata is not too far from the standard realistic values
  // '999' are likely to be dummy values (see Mantis #601)
  // This values were provided by the French Space Agency
  double defaultRadianceMS[4];
  double defaultRadianceP;

  const std::string sensorId = this->GetSensorID();
  if (sensorId == "PHR 1A")
    {
    // MS, ordered as B0, B1, B2, B3
    defaultRadianceMS[0] = 1915.01;
    defaultRadianceMS[1] = 1830.57;
    defaultRadianceMS[2] = 1594.06;
    defaultRadianceMS[3] = 1060.01;

    defaultRadianceP = 1548.71;
    }
  else if (sensorId == "PHR 1B")
    {
    // MS, ordered as B0, B1, B2, B3
    defaultRadianceMS[0] = 1926.51688;
    defaultRadianceMS[1] = 1805.91412;
    defaultRadianceMS[2] = 1533.60973;
    defaultRadianceMS[3] = 1019.23037;

    defaultRadianceP = 1529.00384;
    }
  else
    {
    itkExceptionMacro(<< "Invalid sensor ID.");
    }

  // tolerance threshold
  double tolerance = 0.05;

  if (outputValues.size() == 1)
    {
    // Pan
    if (vcl_abs(outputValues[0] - defaultRadianceP) > (tolerance * defaultRadianceP))
      {
      outputValuesVariableLengthVector[0] = defaultRadianceP;
      }
    else
      {
      outputValuesVariableLengthVector[0] = outputValues[0];
      }
    }
  else
    {
    // MS
    for (unsigned int i = 0; i < outputValues.size(); ++i)
      {
      int wavelenghPos = this->BandIndexToWavelengthPosition(i);
      if (vcl_abs(outputValues[wavelenghPos] - defaultRadianceMS[wavelenghPos]) >
          (tolerance * defaultRadianceMS[wavelenghPos]))
        {
        outputValuesVariableLengthVector[i] = defaultRadianceMS[wavelenghPos];
        }
      else
        {
        outputValuesVariableLengthVector[i] = outputValues[wavelenghPos];
        }
      }
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

  // Use BandIndexToWavelengthPosition because values in keywordlist are sorted by wavelength
  for (unsigned int i = 0; i < outputValues.size(); ++i)
    {
    outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }


  return outputValuesVariableLengthVector;
}

PleiadesImageMetadataInterface::VariableLengthVectorType
PleiadesImageMetadataInterface
::GetPhysicalGain() const
{
  //We use here tabulate in flight values for physical gain of PHR. Those values evolve
  //with time and are much more accurate. Values provided by CNES calibration
  //team. Diference between metadata values and in flight gain can lead to
  //difference of 10%.

  //Values:
  // Ak values for PHR1A are given in the following table :

  // Dates                                       PA NTDI=13       B0           B1           B2           B3

  // From 17/12/2011 to 01/08/2012               11.75     9.52       9.62       10.55     15.73
  // From 01/08/2012 to 01/03/2013               11.73     9.45       9.48       10.51     15.71
  // From 01/03/2013                             11.70     9.38       9.34       10.46     15.69

  // For PHR1B in the following table :

  // Dates                       PA NTDI=13       B0           B1           B2           B3
  // From 01/12/2012             12.04     10.46     10.47     11.32     17.21

  std::vector<double>      outputValues;
  const std::string sensorId = this->GetSensorID();
  const int         nbBands = this->GetNumberOfBands();

  if (sensorId == "PHR 1A")
    {
    //PHR 1A
    if ( (this->GetYear() < 2012) || (this->GetYear() == 2012 && this->GetMonth() <8 ) )
      {
      if (nbBands == 1)
        {
        outputValues.push_back(11.75);
        }
      else
        {
        outputValues.push_back(9.52);
        outputValues.push_back(9.62);
        outputValues.push_back(10.55);
        outputValues.push_back(15.73);
        }
      }
    else if ( (this->GetYear() == 2012 && this->GetMonth() >=8) || (this->GetYear() == 2013 && this->GetMonth() <3) )
      {
      if (nbBands == 1)
        {
        outputValues.push_back(11.73);
        }
      else
        {
        outputValues.push_back(9.45);
        outputValues.push_back(9.48);
        outputValues.push_back(10.51);
        outputValues.push_back(15.71);
        }
      }
    else if ( (this->GetYear() == 2013 && this->GetMonth() >= 3) || this->GetYear() > 2013 )
      {
      if (nbBands == 1)
        {
        outputValues.push_back(11.7);
        }
      else
        {
        outputValues.push_back(9.38);
        outputValues.push_back(9.34);
        outputValues.push_back(10.46);
        outputValues.push_back(15.69);
        }
      }
    else
      {
      itkExceptionMacro(<< "Invalid metadata, wrong acquisition date");
      }
    }
  else if (sensorId == "PHR 1B")
    {
    //PHR 1B
    if ( this->GetYear() >= 2012 )
      {
      if (nbBands == 1)
        {
        outputValues.push_back(12.04);
        }
      else
        {
        outputValues.push_back(10.46);
        outputValues.push_back(10.47);
        outputValues.push_back(11.32);
        outputValues.push_back(17.21);
        }
      }
    else
      {
      itkExceptionMacro(<< "Invalid metadata, wrong acquisition date");
      }
    }
  else
    {
    itkExceptionMacro(<< "Invalid metadata, bad sensor id");
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(outputValues.size());
  outputValuesVariableLengthVector.Fill(0);

  // Use BandIndexToWavelengthPosition because values are tabulated and sorted by wavelength
  for (unsigned int i = 0; i < outputValues.size(); ++i)
    {
    outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }

  otbMsgDevMacro( << "physical gain " << outputValuesVariableLengthVector);
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

  std::istringstream is(valueString);
  std::vector<double> vecValues = std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>());

  //Take the second value (Center value)
  double value = vecValues[1];

  //Convention use in input of atmospheric correction parameters computation is
  //"90 - satOrientation". Pleiades does not seem to follow this convention so
  //inverse the formula here to be able to take the angle read in the metadata
  //as input for 6S
  value = 90. - value;

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

  if (!imageKeywordlist.HasKey("support_data.scene_orientation"))
    {
    return 0;
    }
  else if (!imageKeywordlist.HasKey("support_data.along_track_incidence_angle") || !imageKeywordlist.HasKey("support_data.across_track_incidence_angle"))
    {
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");

    std::istringstream is(valueString);
    std::vector<double> vecCap = std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>());

    //Take the second value (Center value)
    double cap = vecCap[1];

    //return only orientation if across/along track incidence are not available
    return cap;
    }
  else
    {
    //Got orientation and incidences angle which allow computing satellite
    // azimuthal angle

    // MSD: for the moment take only topCenter value
    std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");

    std::istringstream is(valueString);
    std::vector<double> vecCap = std::vector<double>(std::istream_iterator<double>(is), std::istream_iterator<double>());

    //Take the second value (Center value)
    double cap = vecCap[1];

    valueString = imageKeywordlist.GetMetadataByKey("support_data.along_track_incidence_angle");
    std::istringstream isAlong(valueString);
    std::vector<double> vecAlong = std::vector<double>(std::istream_iterator<double>(isAlong), std::istream_iterator<double>());

    //Take the second value (Center value)
    double along = vecAlong[1];

    valueString = imageKeywordlist.GetMetadataByKey("support_data.across_track_incidence_angle");
    std::istringstream isAcross(valueString);
    std::vector<double> vecAcross = std::vector<double>(std::istream_iterator<double>(isAcross), std::istream_iterator<double>());

    //Take the second value (Center value)
    double ortho = vecAcross[1];

    //Compute Satellite azimuthal angle using the azimuthal angle and the along
    //and across track incidence angle

    double satAz = (cap - vcl_atan2(vcl_tan(ortho * CONST_PI_180),vcl_tan(along * CONST_PI_180)) * CONST_180_PI);

    satAz = fmod(satAz,360);

    return satAz;
    }
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
    wavel.Fill(0.430);
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    wavel.SetSize(4);
    wavel[0] = 0.430;
    wavel[1] = 0.430;
    wavel[2] = 0.430;
    wavel[3] = 0.430;
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

  // Panchromatic case
  if (nbBands == 1)
    {
    wavel.SetSize(1);
    wavel.Fill(0.95);
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    wavel.SetSize(4);
    wavel[0] = 0.95;
    wavel[1] = 0.95;
    wavel[2] = 0.95;
    wavel[3] = 0.95;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

// TODO MSD need to update this function
// Comment this part as relative response
// FIXME check if this is coherent with other sensor
unsigned int
PleiadesImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const
{
  int nbBands = this->GetNumberOfBands();
  //Panchromatic case
  if (nbBands == 1)
    {
    return 0;
    }
  else
    {
    otbMsgDevMacro(<< "Pleiades detected: first file component is red band and third component is blue one");
    if (i == 0) return 2;
    if (i == 2) return 0;
    }

  return i;
}

std::vector<std::string>
PleiadesImageMetadataInterface
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
      if ( !(*it).compare("B0") )
        {
        enhBandNames.push_back("Blue");
        }
      else if ( !(*it).compare("B1") )
        {
        enhBandNames.push_back("Green");
        }
      else if ( !(*it).compare("B2") )
        {
        enhBandNames.push_back("Red");
        }
      else if ( !(*it).compare("B3") )
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

std::vector<unsigned int>
PleiadesImageMetadataInterface
::GetDefaultDisplay() const
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

  std::list <std::vector<float> > tmpSpectralBandList;

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

  const int nbBands = this->GetNumberOfBands();
  const std::string sensorId = this->GetSensorID();

  // Panchromatic case
  if (nbBands == 1)
    {
    //if (sensorId.find("PHR") != std::string::npos)
    if (sensorId == "PHR 1A")
      {
      const float pan[209] =
        {
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0004358,
          0.0008051,
          0.0030464,
          0.0060688,
          0.0130170,
          0.0240166,
          0.0421673,
          0.0718226,
          0.1151660,
          0.1738520,
          0.2442960,
          0.3204090,
          0.3940400,
          0.4552560,
          0.5015220,
          0.5324310,
          0.5537040,
          0.5683830,
          0.5824280,
          0.5967100,
          0.6107110,
          0.6232080,
          0.6335930,
          0.6421600,
          0.6501020,
          0.6570500,
          0.6640260,
          0.6707420,
          0.6765340,
          0.6808170,
          0.6829760,
          0.6834310,
          0.6831800,
          0.6831390,
          0.6846760,
          0.6892820,
          0.6964400,
          0.7073100,
          0.7193370,
          0.7323050,
          0.7449550,
          0.7554270,
          0.7638770,
          0.7715940,
          0.7776340,
          0.7826480,
          0.7871060,
          0.7913600,
          0.7949010,
          0.7974290,
          0.7996850,
          0.8007000,
          0.8003370,
          0.8002560,
          0.8000160,
          0.8012780,
          0.8035920,
          0.8077100,
          0.8136680,
          0.8209800,
          0.8302930,
          0.8390660,
          0.8482460,
          0.8564130,
          0.8634600,
          0.8689680,
          0.8738000,
          0.8767770,
          0.8799960,
          0.8825630,
          0.8849870,
          0.8894330,
          0.8939110,
          0.8994160,
          0.9056300,
          0.9123790,
          0.9188800,
          0.9244770,
          0.9289540,
          0.9338010,
          0.9374110,
          0.9414880,
          0.9449190,
          0.9480210,
          0.9509810,
          0.9532570,
          0.9560420,
          0.9581430,
          0.9600910,
          0.9609580,
          0.9631350,
          0.9649320,
          0.9674470,
          0.9728920,
          0.9774240,
          0.9827830,
          0.9874630,
          0.9926650,
          0.9966400,
          0.9993680,
          0.9999140,
          0.9942900,
          0.9882640,
          0.9810720,
          0.9751580,
          0.9699990,
          0.9659940,
          0.9632210,
          0.9624050,
          0.9621500,
          0.9633410,
          0.9654310,
          0.9671030,
          0.9672570,
          0.9677370,
          0.9653110,
          0.9633140,
          0.9579490,
          0.9533250,
          0.9486410,
          0.9423590,
          0.9358410,
          0.9299340,
          0.9243940,
          0.9190090,
          0.9141220,
          0.9087800,
          0.9039750,
          0.8980690,
          0.8927220,
          0.8870500,
          0.8828150,
          0.8771870,
          0.8734510,
          0.8694140,
          0.8664310,
          0.8660560,
          0.8588170,
          0.8477750,
          0.8331070,
          0.8135860,
          0.7865930,
          0.7489860,
          0.6981680,
          0.6306270,
          0.5506620,
          0.4616840,
          0.3695890,
          0.2823490,
          0.2074510,
          0.1465300,
          0.1010100,
          0.0686868,
          0.0471034,
          0.0324221,
          0.0246752,
          0.0174143,
          0.0126697,
          0.0116629,
          0.0086694,
          0.0081772,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000,
          0.0000000
        };
      //add panchromatic band to the temporary list
      const std::vector<float> vpan (pan, pan + sizeof(pan) / sizeof(float) );
      tmpSpectralBandList.push_back(vpan);
      }
    else if (sensorId == "PHR 1B")
      {
      const float pan[209] =
        {
          8.4949016432e-06,1.27423524648e-05,2.54847049296e-05,4.10586912755e-05,5.94643115024e-05,7.64541147888e-05,
          0.0001047705,0.0001628184,0.0004077553,0.0007560434,0.0027948226,0.0055783471,
          0.0119863062,0.022134882,0.0389179761,0.0664188043,0.1067016279,0.1614272001,
          0.2272018077,0.2986552571,0.367815083,0.4258041132,0.4697397445,0.4991660838,
          0.5193159905,0.5332504608,0.5467262065,0.5603038909,0.5736437182,0.5855337488,
          0.5956625032,0.6041036038,0.6120151889,0.618774299,0.625564557,0.6319045853,
          0.6377943837,0.6421862479,0.6443949223,0.6448819633,0.644884795,0.6451141573,
          0.6468357907,0.6513182672,0.6590911022,0.6691405708,0.6811806781,0.6937021631,
          0.7056091836,0.7157690859,0.7250568451,0.7323511339,0.7384193253,0.7438758838,
          0.7492531566,0.7536450207,0.7580793594,0.7610893862,0.7639125252,0.7652348982,
          0.766172169,0.7665969141,0.767494542,0.7687546191,0.7723026564,0.7771079391,
          0.7833828397,0.7917191699,0.8015449394,0.8109516272,0.8200326771,0.8290684208,
          0.8364646485,0.8426517685,0.8481876127,0.8519168746,0.8555045547,0.8583361886,
          0.8620173126,0.8666838452,0.8715797402,0.8780188757,0.8847071949,0.8913473763,
          0.898890849,0.9045824331,0.9105090428,0.9154615705,0.9207935371,0.9253156564,
          0.9289882855,0.9328931087,0.9369565033,0.9400797954,0.9441601799,0.9469295178,
          0.9503472999,0.9521085762,0.9542804394,0.9574405428,0.9607705442,0.9663148834,
          0.9720999114,0.9788278735,0.9845959117,0.991221935,0.9956874216,0.9990882139,
          1.0,0.9963273709,0.9914682871,0.9869688209,0.9817019819,0.9771458829,
          0.9745974125,0.9729012638,0.9739319785,0.9741924888,0.9768712145,0.9798246086,
          0.9827723395,0.9845732586,0.9850772894,0.9837266001,0.9825967782,0.9790232562,
          0.9745945808,0.9697411603,0.9648141174,0.9598389367,0.9533346736,0.9483566613,
          0.9445481137,0.93968903,0.9344561706,0.9297160154,0.9248342786,0.9183356789,
          0.9111942982,0.9061001889,0.9006181457,0.8953456434,0.8908093659,0.8894954878,
          0.8885412272,0.8809779331,0.8707840511,0.8566683562,0.8369346997,0.8102408871,
          0.7728519933,0.718954674,0.6528501811,0.5710810895,0.4795853355,0.3846491464,
          0.294773087,0.2166449103,0.1531319287,0.1057054591,0.0716799801,0.0491432892,
          0.0337757289,0.0256986066,0.0181762579,0.0131968863,0.0121562043,0.0090201414,
          0.0084156159,0.0042799863,0.0033130116,0.0026688064,0.0022115061,0.0018292327,
          0.0015290823,0.0013634289,0.0012147709,0.0011015056,0.0009939035,0.0009188652,
          0.0008523218,0.0008225925,0.0006965819,0.0007220666,0.0006682656,0.0006470283,
          0.0006144646,0.000603138,0.0006116329,0.0005578319,0.0005295155,0.0005295155,
          0.0005210206,0.0005054466,0.0004473982,0.0004473953,0.0004077553,0.0004558931,
          0.0004077553,0.0004346558,0.0003766073,0.0003851022,0.0003879338,0.0003723599,
          0.0003341328,0.0003723599,0.0003313012,0.0003341328,0.0003143114
        };
      //add panchromatic band to the temporary list
      const std::vector<float> vpan (pan, pan + sizeof(pan) / sizeof(float) );
      tmpSpectralBandList.push_back(vpan);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Pleiades Sensor ID");
      }
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    if (sensorId == "PHR 1A")
      {
      //band B0 (blue band)
      const float b0[209] =
        {
          0.0098681,
          0.0293268,
          0.0877320,
          0.1287040,
          0.1341240,
          0.2457050,
          0.4345520,
          0.5133040,
          0.4710970,
          0.5125880,
          0.6530370,
          0.7707870,
          0.7879420,
          0.7648330,
          0.7718380,
          0.8013290,
          0.8240790,
          0.8352890,
          0.8326150,
          0.8249150,
          0.8168160,
          0.8163380,
          0.8285420,
          0.8623820,
          0.9075060,
          0.9379000,
          0.9505710,
          0.9572260,
          0.9650570,
          0.9632790,
          0.9587260,
          0.9567320,
          0.9646760,
          0.9804620,
          0.9900240,
          0.9838940,
          0.9719110,
          0.9715280,
          0.9574890,
          0.8770130,
          0.7103910,
          0.4943810,
          0.3021990,
          0.1722720,
          0.0943537,
          0.0543895,
          0.0345732,
          0.0261018,
          0.0230010,
          0.0223203,
          0.0210136,
          0.0173172,
          0.0119112,
          0.0072895,
          0.0046311,
          0.0033297,
          0.0025865,
          0.0020232,
          0.0015030,
          0.0010527,
          0.0007044,
          0.0005199,
          0.0004117,
          0.0004097,
          0.0005317,
          0.0009532,
          0.0013521,
          0.0014273,
          0.0009182,
          0.0003440,
          0.0001323,
          0.0000783,
          0.0000626,
          0.0000511,
          0.0000538,
          0.0000533,
          0.0000454,
          0.0000404,
          0.0000315,
          0.0000327,
          0.0000262,
          0.0000303,
          0.0000206,
          0.0000241,
          0.0000241,
          0.0000273,
          0.0000258,
          0.0000208,
          0.0000341,
          0.0000379,
          0.0000393,
          0.0000429,
          0.0000281,
          0.0000277,
          0.0000187,
          0.0000272,
          0.0000245,
          0.0000209,
          0.0000137,
          0.0000171,
          0.0000257,
          0.0000300,
          0.0000330,
          0.0000446,
          0.0000397,
          0.0000399,
          0.0000384,
          0.0000336,
          0.0000307,
          0.0000300,
          0.0000242,
          0.0000224,
          0.0000210,
          0.0000325,
          0.0000690,
          0.0002195,
          0.0005063,
          0.0008373,
          0.0009464,
          0.0007099,
          0.0004910,
          0.0004433,
          0.0006064,
          0.0012019,
          0.0016241,
          0.0016779,
          0.0009733,
          0.0003606,
          0.0001659,
          0.0000864,
          0.0000564,
          0.0000562,
          0.0000590,
          0.0000458,
          0.0000382,
          0.0000586,
          0.0000685,
          0.0000474,
          0.0000872,
          0.0000628,
          0.0000948,
          0.0001015,
          0.0001564,
          0.0002379,
          0.0003493,
          0.0005409,
          0.0007229,
          0.0007896,
          0.0007188,
          0.0005204,
          0.0003939,
          0.0003128,
          0.0002699,
          0.0002605,
          0.0002378,
          0.0002286,
          0.0002406,
          0.0002741,
          0.0003203,
          0.0003812,
          0.0004904,
          0.0006077,
          0.0008210,
          0.0011791,
          0.0018150,
          0.0030817,
          0.0055589,
          0.0103652,
          0.0166309,
          0.0211503,
          0.0216246,
          0.0176910,
          0.0136927,
          0.0107136,
          0.0089555,
          0.0079790,
          0.0079189,
          0.0080456,
          0.0088920,
          0.0102062,
          0.0126157,
          0.0162251,
          0.0221306,
          0.0308295,
          0.0411980,
          0.0498232,
          0.0531265,
          0.0484487,
          0.0391122,
          0.0291405,
          0.0212633,
          0.0162146,
          0.0128925,
          0.0108169,
          0.0094115,
          0.0084386,
          0.0077249,
          0.0074231,
          0.0072603,
          0.0073459,
          0.0074214,
          0.0076433,
          0.0077788,
          0.0078151,
          0.0077003,
          0.0072256,
          0.0065903,
          0.0057120,
          0.0048136
        };
      //B1 green band
      const float b1[209] =
        {
          0.0000144,
          0.0000143,
          0.0000259,
          0.0000189,
          0.0000132,
          0.0000179,
          0.0000224,
          0.0000179,
          0.0000124,
          0.0000202,
          0.0000276,
          0.0000292,
          0.0000420,
          0.0000366,
          0.0000261,
          0.0000247,
          0.0000445,
          0.0000902,
          0.0001144,
          0.0000823,
          0.0000778,
          0.0001923,
          0.0003401,
          0.0004085,
          0.0004936,
          0.0007849,
          0.0045979,
          0.0085122,
          0.0143014,
          0.0243310,
          0.0480572,
          0.1097360,
          0.2353890,
          0.4328370,
          0.6491340,
          0.8095770,
          0.8847680,
          0.9066640,
          0.9131150,
          0.9186700,
          0.9273270,
          0.9405210,
          0.9512930,
          0.9587500,
          0.9667360,
          0.9709750,
          0.9728630,
          0.9769560,
          0.9850710,
          0.9892500,
          0.9865960,
          0.9743300,
          0.9575190,
          0.9435550,
          0.9439310,
          0.9571350,
          0.9712530,
          0.9761580,
          0.9619590,
          0.9244890,
          0.8734580,
          0.8349840,
          0.8166740,
          0.8015960,
          0.7435910,
          0.6160350,
          0.4321320,
          0.2544540,
          0.1360870,
          0.0769553,
          0.0479321,
          0.0342014,
          0.0266703,
          0.0212632,
          0.0160541,
          0.0106967,
          0.0060543,
          0.0030797,
          0.0015416,
          0.0008333,
          0.0004706,
          0.0002918,
          0.0001917,
          0.0001472,
          0.0001063,
          0.0000912,
          0.0000589,
          0.0000552,
          0.0000752,
          0.0000884,
          0.0000985,
          0.0001125,
          0.0001368,
          0.0001947,
          0.0002284,
          0.0002088,
          0.0001498,
          0.0000637,
          0.0000307,
          0.0000283,
          0.0000311,
          0.0000331,
          0.0000215,
          0.0000236,
          0.0000205,
          0.0000186,
          0.0000233,
          0.0000233,
          0.0000198,
          0.0000195,
          0.0000161,
          0.0000308,
          0.0000464,
          0.0000290,
          0.0000264,
          0.0000233,
          0.0000395,
          0.0001113,
          0.0001903,
          0.0002290,
          0.0002229,
          0.0001322,
          0.0000548,
          0.0000608,
          0.0000414,
          0.0000382,
          0.0000381,
          0.0000269,
          0.0000233,
          0.0000198,
          0.0000208,
          0.0000302,
          0.0000419,
          0.0000305,
          0.0000340,
          0.0000334,
          0.0000362,
          0.0000282,
          0.0000337,
          0.0000330,
          0.0000424,
          0.0000420,
          0.0000470,
          0.0000417,
          0.0000233,
          0.0000439,
          0.0000503,
          0.0000446,
          0.0000428,
          0.0000597,
          0.0000671,
          0.0001142,
          0.0001780,
          0.0003546,
          0.0009610,
          0.0041260,
          0.0066679,
          0.0078563,
          0.0068645,
          0.0029441,
          0.0011320,
          0.0007028,
          0.0005471,
          0.0004967,
          0.0004929,
          0.0005351,
          0.0006223,
          0.0007957,
          0.0010708,
          0.0016699,
          0.0030334,
          0.0054959,
          0.0091390,
          0.0125045,
          0.0144212,
          0.0141099,
          0.0117418,
          0.0089824,
          0.0067916,
          0.0056849,
          0.0051998,
          0.0053640,
          0.0060350,
          0.0067668,
          0.0083174,
          0.0106521,
          0.0139110,
          0.0183736,
          0.0231289,
          0.0272661,
          0.0298126,
          0.0300318,
          0.0286507,
          0.0266172,
          0.0247529,
          0.0236974,
          0.0232734,
          0.0236733,
          0.0245808,
          0.0257173,
          0.0267721,
          0.0267455,
          0.0254447,
          0.0227056,
          0.0188513,
          0.0147988,
          0.0109864,
          0.0079795,
          0.0057516
        };
      //B2 red band
      const float b2[209] =
        {
          0.0097386,
          0.0035306,
          0.0035374,
          0.0114418,
          0.0266686,
          0.0373494,
          0.0904431,
          0.0907580,
          0.0399312,
          0.0208748,
          0.0080694,
          0.0027002,
          0.0011241,
          0.0006460,
          0.0005029,
          0.0006051,
          0.0009979,
          0.0019446,
          0.0014554,
          0.0006090,
          0.0003230,
          0.0002503,
          0.0002538,
          0.0003360,
          0.0005377,
          0.0007773,
          0.0004895,
          0.0002045,
          0.0000875,
          0.0000594,
          0.0000217,
          0.0000290,
          0.0000297,
          0.0000408,
          0.0000456,
          0.0000447,
          0.0000322,
          0.0000222,
          0.0000147,
          0.0000095,
          0.0000072,
          0.0000113,
          0.0000313,
          0.0000123,
          0.0000122,
          0.0000280,
          0.0000180,
          0.0000261,
          0.0000138,
          0.0000392,
          0.0000517,
          0.0000695,
          0.0000797,
          0.0000785,
          0.0001004,
          0.0001170,
          0.0001483,
          0.0001837,
          0.0002110,
          0.0002973,
          0.0004162,
          0.0006371,
          0.0010012,
          0.0032888,
          0.0100109,
          0.0181837,
          0.0330510,
          0.0624784,
          0.1183670,
          0.2218740,
          0.3756820,
          0.5574830,
          0.7342220,
          0.8636840,
          0.9319920,
          0.9527010,
          0.9620090,
          0.9527340,
          0.9437220,
          0.9456300,
          0.9562330,
          0.9693120,
          0.9839640,
          0.9949160,
          0.9992700,
          0.9993300,
          0.9963430,
          0.9944130,
          0.9883050,
          0.9857580,
          0.9807560,
          0.9683790,
          0.9544700,
          0.9371750,
          0.9170350,
          0.8922820,
          0.8662710,
          0.8442750,
          0.8220420,
          0.7888070,
          0.7372920,
          0.6625080,
          0.5662120,
          0.4493120,
          0.3260000,
          0.2194040,
          0.1416500,
          0.0925669,
          0.0619437,
          0.0456444,
          0.0355683,
          0.0310879,
          0.0295168,
          0.0233351,
          0.0189628,
          0.0158627,
          0.0132266,
          0.0107473,
          0.0083969,
          0.0063847,
          0.0046601,
          0.0033814,
          0.0024167,
          0.0017478,
          0.0012949,
          0.0009939,
          0.0007442,
          0.0006312,
          0.0005142,
          0.0004354,
          0.0003549,
          0.0003156,
          0.0003079,
          0.0002906,
          0.0002867,
          0.0002751,
          0.0003048,
          0.0003010,
          0.0003342,
          0.0004310,
          0.0004955,
          0.0005488,
          0.0005838,
          0.0006687,
          0.0006968,
          0.0006650,
          0.0005866,
          0.0004688,
          0.0004086,
          0.0003611,
          0.0002404,
          0.0002609,
          0.0002476,
          0.0002133,
          0.0002098,
          0.0001916,
          0.0001642,
          0.0001799,
          0.0002180,
          0.0002003,
          0.0002030,
          0.0002348,
          0.0002735,
          0.0002652,
          0.0002944,
          0.0004666,
          0.0004882,
          0.0006642,
          0.0007798,
          0.0010588,
          0.0014008,
          0.0019011,
          0.0024917,
          0.0034379,
          0.0042182,
          0.0053618,
          0.0062814,
          0.0068774,
          0.0071141,
          0.0070399,
          0.0065876,
          0.0067873,
          0.0066877,
          0.0068572,
          0.0070486,
          0.0073911,
          0.0081201,
          0.0087391,
          0.0096581,
          0.0106625,
          0.0120129,
          0.0137222,
          0.0159817,
          0.0180896,
          0.0206562,
          0.0236408,
          0.0269627,
          0.0310497,
          0.0353146,
          0.0398729,
          0.0438795,
          0.0462377,
          0.0454916,
          0.0408754,
          0.0333175,
          0.0251186,
          0.0179089,
          0.0125129,
          0.0086117
        };
      //B3 nir band
      const float b3[209] =
        {
          0.0024163,
          0.0017305,
          0.0020803,
          0.0020499,
          0.0012660,
          0.0007361,
          0.0006198,
          0.0006344,
          0.0007721,
          0.0011837,
          0.0020819,
          0.0023991,
          0.0013377,
          0.0006328,
          0.0003544,
          0.0002890,
          0.0002498,
          0.0002541,
          0.0003346,
          0.0005048,
          0.0008684,
          0.0009871,
          0.0006587,
          0.0003833,
          0.0002606,
          0.0002356,
          0.0002364,
          0.0002791,
          0.0003613,
          0.0005575,
          0.0007414,
          0.0007413,
          0.0005768,
          0.0004230,
          0.0003206,
          0.0003044,
          0.0003019,
          0.0003201,
          0.0003813,
          0.0004630,
          0.0005930,
          0.0007080,
          0.0008577,
          0.0009017,
          0.0008813,
          0.0007801,
          0.0006583,
          0.0005863,
          0.0005224,
          0.0005506,
          0.0006403,
          0.0008293,
          0.0013444,
          0.0023942,
          0.0027274,
          0.0014330,
          0.0006388,
          0.0003596,
          0.0002416,
          0.0001718,
          0.0001566,
          0.0001642,
          0.0001892,
          0.0002351,
          0.0003227,
          0.0006734,
          0.0014311,
          0.0013325,
          0.0005796,
          0.0002424,
          0.0001263,
          0.0001022,
          0.0000446,
          0.0000652,
          0.0000544,
          0.0000573,
          0.0000518,
          0.0000504,
          0.0000649,
          0.0000723,
          0.0000833,
          0.0000739,
          0.0000691,
          0.0001382,
          0.0001692,
          0.0002240,
          0.0002296,
          0.0001553,
          0.0001492,
          0.0001121,
          0.0001058,
          0.0001068,
          0.0001012,
          0.0000864,
          0.0000533,
          0.0000354,
          0.0000440,
          0.0000371,
          0.0000691,
          0.0000769,
          0.0000791,
          0.0001333,
          0.0001244,
          0.0002048,
          0.0002455,
          0.0002721,
          0.0003812,
          0.0004568,
          0.0006255,
          0.0008185,
          0.0009733,
          0.0012281,
          0.0013528,
          0.0015758,
          0.0017458,
          0.0019104,
          0.0020863,
          0.0023053,
          0.0025241,
          0.0037234,
          0.0044186,
          0.0053574,
          0.0066118,
          0.0083509,
          0.0107509,
          0.0150393,
          0.0212756,
          0.0292566,
          0.0414246,
          0.0586633,
          0.0834879,
          0.1190380,
          0.1671850,
          0.2326370,
          0.3124060,
          0.4070470,
          0.5091930,
          0.6148270,
          0.7140870,
          0.8017550,
          0.8714840,
          0.9241260,
          0.9587210,
          0.9782990,
          0.9882040,
          0.9922940,
          0.9902030,
          0.9854020,
          0.9777560,
          0.9660200,
          0.9532070,
          0.9421250,
          0.9303560,
          0.9241490,
          0.9212220,
          0.9203820,
          0.9217020,
          0.9227420,
          0.9230000,
          0.9237670,
          0.9243070,
          0.9206520,
          0.9154840,
          0.9090910,
          0.9003380,
          0.8905620,
          0.8776420,
          0.8668600,
          0.8537290,
          0.8428590,
          0.8305310,
          0.8195740,
          0.8069090,
          0.7921080,
          0.7791670,
          0.7660510,
          0.7521190,
          0.7375270,
          0.7217320,
          0.7043220,
          0.6853170,
          0.6642500,
          0.6413850,
          0.6173030,
          0.5919540,
          0.5672310,
          0.5430130,
          0.5184560,
          0.4957540,
          0.4734340,
          0.4528220,
          0.4332270,
          0.4131920,
          0.3919120,
          0.3659660,
          0.3325420,
          0.2917680,
          0.2453910,
          0.1962540,
          0.1486850,
          0.1068860,
          0.0738260,
          0.0491777,
          0.0327991,
          0.0215831,
          0.0145386,
          0.0103219,
          0.0076144,
          0.0061346
        };
      //Add multispectral bands to the temporary list
      const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
      const std::vector<float> vb1 (b1, b1 + sizeof(b1) / sizeof(float) );
      const std::vector<float> vb2 (b2, b2 + sizeof(b2) / sizeof(float) );
      const std::vector<float> vb3 (b3, b3 + sizeof(b3) / sizeof(float) );
      //For Pleiades MS image the order of band in 1A product is: B2 B1 B0 B3
      //(BandIndexToWavelength method could be used here)
      tmpSpectralBandList.push_back(vb2);
      tmpSpectralBandList.push_back(vb1);
      tmpSpectralBandList.push_back(vb0);
      tmpSpectralBandList.push_back(vb3);
      }
    else if (sensorId == "PHR 1B")
      {
      //B0 blue band
      const float b0[209] =
        {0.0016025229,0.0013381709,0.0043437933,0.0110724631,0.0176270388,0.0233671533,
         0.057403285,0.1345548468,0.2237631463,0.3085427458,0.4615565735,0.650820526,
         0.755369094,0.7477203647,0.7168186424,0.7185381794,0.7566962056,0.8101088802,
         0.842166474,0.8234370764,0.775247228,0.7527006008,0.7802203291,0.8199266521,
         0.8516631705,0.8602751259,0.858684019,0.8658261626,0.8828217532,0.9040412689,
         0.9196954778,0.9325955734,0.9502333148,0.9757980507,0.9949769539,1.0,
         0.995062574,0.9806284516,0.9417497895,0.8436220158,0.6711424576,0.4633403256,
         0.2888651055,0.1670783567,0.0901800876,0.0505194286,0.0314882201,0.0238137335,
         0.0213108438,0.0206298072,0.0195599127,0.0167943833,0.0113582202,0.0066519685,
         0.004144013,0.0030298814,0.0024159139,0.0019903178,0.0015682749,0.0011355438,
         0.0012529075,0.0008355166,0.0005508227,0.000419896,0.0003617449,0.000377798,
         0.0005322716,0.0011091514,0.0019870999,0.0012200722,0.0003753015,0.0001469798,
         7.49175906503e-05,5.2799e-05,5.56530673402e-05,5.74373902992e-05,5.63665682035e-05,3.78155457568e-05,
         3.49615423035e-05,2.06915250367e-05,1.35565164034e-05,1.99783095738e-05,4.281e-06,1.10591920316e-05,
         1.21295146767e-05,1.14159424633e-05,9.27551122337e-06,1.1773e-05,1.92645233101e-05,8.919e-06,
         1.1416e-05,1.71237353197e-05,4.99450604335e-06,8.56172495969e-06,2.4259e-05,3.85286898697e-05,
         1.2843e-05,2.35456711903e-05,9.989e-06,1.14162992137e-05,2.06915250367e-05,1.39132668351e-05,
         5.708e-06,2.854e-06,8.562e-06,1.24862651084e-05,8.562e-06,9.27565392354e-06,
         6.42150777002e-06,1.1773e-05,5.708e-06,1.35566591036e-05,1.7124e-05,7.13522268362e-06,
         9.989e-06,2.71133895572e-05,6.35015768369e-05,0.0002194051,0.0007613054,0.0011194829,
         0.0007541704,0.0004084771,0.0003546099,0.0004063402,0.0006792528,0.0016292935,
         0.0024002169,0.0010317222,0.0003481884,0.000165889,8.2766100147e-05,9.70361174137e-05,
         4.63775561168e-05,2.63994605933e-05,3.21075388501e-05,4.1026442342e-05,1.5697e-05,8.9192602423e-06,
         4.70910569802e-05,7.88411318978e-05,2.21185267634e-05,5.35134923013e-05,8.2766100147e-05,0.0001052421,
         0.0001833697,0.00026043,0.000441657,0.000710293,0.000864763,0.0007370464,
         0.0005515362,0.000394566,0.0002811193,0.0002340283,0.0002247528,0.0001922885,
         0.0002197583,0.0002340276,0.0002454443,0.0002454436,0.0002782653,0.000350685,
         0.000431668,0.0005326263,0.0006885283,0.0010277909,0.0015625669,0.0026085734,
         0.0045749675,0.0090618891,0.0171625498,0.0237927565,0.021523467,0.0159138519,
         0.011956134,0.0094824265,0.007869201,0.0072241962,0.0072420338,0.0075677469,
         0.0083793541,0.0098637927,0.0122586583,0.0162673916,0.0226015668,0.0320271273,
         0.0444304123,0.0546690783,0.0564243618,0.048004124,0.0357991923,0.0258336544,
         0.0188870814,0.0144386889,0.0116792956,0.0099112405,0.008646917,0.0078099805,
         0.0072270502,0.006995876,0.0069230989,0.0069141801,0.0070208485,0.0072527363,
         0.0073726044,0.0075049588,0.007470354,0.0070672332,0.0063679952
        };
      //B1 green band
      const float b1[209] =
        {3.2793605247e-06,3.2793605247e-06,4.919e-06,6.5587210494e-06,6.5587210494e-06,2.86944045911e-06,
         6.5587210494e-06,5.73885632302e-06,4.09920065587e-06,2.86943226071e-06,1.47571223611e-05,2.29553597049e-05,
         1.06579217053e-05,1.27075220332e-05,2.21356835417e-05,1.8856323017e-05,9.01824144292e-05,0.0001151875,
         0.0001123181,0.0001893831,0.000323017,0.0004849346,0.0006238983,0.0008153327,
         0.0015208034,0.0041299447,0.009953679,0.0196421398,0.0325476532,0.0567927854,
         0.1237909408,0.2859094077,0.5289756098,0.7716253331,0.8838040582,0.9079565485,
         0.9131461365,0.913728223,0.9224841156,0.9367083419,0.9497601968,0.9544988727,
         0.9585816766,0.9642057799,0.9705267473,0.9722648084,0.967517934,0.9589096126,
         0.9524492724,0.9524656692,0.9560483706,0.95517934,0.948989547,0.9471449067,
         0.9544496823,0.9710596434,0.9898175856,1.0,0.9953597049,0.9698216848,
         0.9253043656,0.8633244517,0.7948284485,0.7238967001,0.6453265013,0.5438524288,
         0.4170280795,0.2766706292,0.1575273622,0.0856068867,0.049225661,0.0327237549,
         0.0237934003,0.0181966796,0.0140618979,0.0099658947,0.0058454601,0.0030383275,
         0.0015355606,0.0008386965,0.0004878049,0.0003115409,0.0002074196,0.0001381439,
         9.26419348227e-05,7.00964131994e-05,6.39475302316e-05,5.41093666735e-05,4.0992e-05,7.00968231195e-05,
         4.75507276081e-05,4.71408895265e-05,6.23078499693e-05,6.72277925804e-05,0.0001483911,0.0002512826,
         0.0002992416,0.0002303743,0.0001270752,6.72265628203e-05,3.11539249846e-05,3.15638450502e-05,
         1.72166427547e-05,6.5587210494e-06,5.73888091822e-06,1.76265628203e-05,1.06579217053e-05,1.72168067227e-05,
         1.14777618364e-05,2.86946505431e-06,3.2793605247e-06,2.86941586391e-06,1.393728223e-05,1.27072760812e-05,
         1.72166427547e-05,9.83840951015e-06,7.37856118057e-06,2.41853658537e-05,3.2793605247e-05,0.0001295372,
         0.000277106,0.0001893831,0.0001237959,2.4185e-05,7.37856118057e-06,7.37870875179e-06,
         4.09920065587e-06,1.0248e-05,9.83808157409e-06,3.2793605247e-06,1.5577e-05,2.29556056569e-05,
         7.37856118057e-06,1.02477556876e-05,9.01824144292e-06,6.96851813896e-06,1.72166427547e-05,2.29553597049e-05,
         2.45952039352e-06,2.1315e-05,9.83808157409e-06,1.1887763886e-05,3.44332855093e-05,9.01856937897e-06,
         1.80364828858e-05,1.72165607706e-05,1.8856323017e-05,1.8037e-05,2.86944045911e-05,2.86944865751e-05,
         2.0496e-05,4.34515269522e-05,2.95142447223e-05,5.28796884608e-05,9.34617749539e-05,0.0001336339,
         0.000277106,0.0007050527,0.0031850789,0.0141905308,0.0083394138,0.0022443452,
         0.0009182209,0.000571841,0.0004369748,0.0004029514,0.0004181185,0.000445174,
         0.0005173191,0.0006021734,0.0007575323,0.0011108916,0.0019381021,0.0036905349,
         0.0063570404,0.0102714491,0.013107604,0.0132605042,0.0111153925,0.0083664685,
         0.0065644599,0.0056847797,0.0053797909,0.0056228653,0.0062004509,0.007239172,
         0.0089198606,0.0115101455,0.0149415864,0.0192686206,0.0234785817,0.0264402542,
         0.0270489854,0.0255453986,0.0233974175,0.0213945481,0.0199442509,0.0192531256,
         0.0190744005,0.0196892806,0.0206935848,0.0220106579,0.0232301701,0.0237573273,
         0.0229858578,0.0206595614,0.0172248412,0.01329215,0.0097815126
        };
      //B2 red band
      const float b2[209] =
        {0.0004536879,0.0008531472,0.000520901,0.0011945588,0.0051417966,0.0080033148,
         0.0066930427,0.0108594102,0.0246908583,0.0653593224,0.1225423328,0.0570092494,
         0.0213752702,0.0127972076,0.006277544,0.0023536016,0.0009409824,0.0005174639,
         0.0003742544,0.0003818922,0.0005155545,0.0008462731,0.0010127781,0.0006431042,
         0.0003513408,0.0002512851,0.000223025,0.000251667,0.0003551597,0.0004998969,
         0.0005262475,0.0003864772,0.0002528126,0.0001626868,0.0001069298,8.78352058781e-05,
         7.02681647025e-05,5.84297356542e-05,5.49924767237e-05,4.81185698901e-05,3.66616511491e-05,3.20791738908e-05,
         1.90946099735e-05,1.79493916457e-05,3.74254355481e-05,2.40590558097e-05,3.43702979523e-05,1.48934902656e-05,
         6.87405959046e-06,7.256e-06,3.8189219947e-06,2.29135319682e-06,9.16541278728e-06,2.94056993592e-05,
         2.36773163671e-05,2.90238071597e-05,3.8953e-05,4.5063e-05,6.95043803035e-05,8.13422747027e-05,
         0.0001038747,0.0001229693,0.0001351898,0.0001535207,0.0001833083,0.0002214967,
         0.0007454536,0.0012442124,0.0021416515,0.0038193268,0.0068045552,0.0123329031,
         0.0221780076,0.0413324983,0.0780709861,0.151934284,0.277674582,0.4510376011,
         0.6291322645,0.7625485958,0.8329450763,0.8579055504,0.8658870974,0.8692630244,
         0.8752205427,0.8857760431,0.9005934605,0.9174883714,0.9348797421,0.947810612,
         0.9569531113,0.9623301534,0.9647666257,0.9624294454,0.9613066823,0.9620246397,
         0.9699145325,0.9811574389,0.9933932649,1.0,0.9809512171,0.9522634751,
         0.9131729895,0.8694005056,0.8252079403,0.7830470415,0.7361270021,0.6734890435,
         0.5877527172,0.4804914189,0.3630068664,0.2523031919,0.1626028245,0.102221085,
         0.0641273381,0.0419155713,0.0284639532,0.0204548336,0.0153696335,0.0121174395,
         0.0098810788,0.0083168483,0.0071016673,0.0060949919,0.0051723479,0.0043142285,
         0.0034950774,0.0027713917,0.0035890229,0.0030310784,0.0023173219,0.001784208,
         0.0013312762,0.0010207978,0.0007897531,0.0006385253,0.0005079166,0.0004120624,
         0.0003788371,0.0003593606,0.0002978759,0.0002787813,0.0002810727,0.0002623607,
         0.0002856554,0.0002948208,0.0002757262,0.0003158256,0.0003750181,0.0004296295,
         0.0005186096,0.0005747478,0.0006194291,0.0006503624,0.0006515081,0.0006041535,
         0.0005369404,0.0004643817,0.0003765457,0.0003215532,0.0002841278,0.0002539583,
         0.000226844,0.0002226432,0.0001672688,0.0001978194,0.0002207337,0.000208895,
         0.0002329542,0.0002054572,0.0001871272,0.0002505213,0.0002932932,0.0002635079,
         0.0002589229,0.000449106,0.0005361766,0.000566728,0.0008210682,0.0009856638,
         0.0013075989,0.0017612792,0.0024028657,0.0032682258,0.0043635003,0.0056615366,
         0.0068900991,0.0078222979,0.0083298327,0.0084363806,0.0081854774,0.0081381228,
         0.0080006416,0.0077676873,0.0077844906,0.0079983502,0.0081518709,0.008505885,
         0.008792686,0.0093120594,0.009752763,0.0101358009,0.0103874678,0.0104061805,
         0.0101713168,0.009522482,0.008608614,0.0073373101,0.0059842508
        };
      //B3 nir band
      const float b3[209] =
        {0.0001962709,0.0007612586,0.0003823311,0.0005128001,0.0016870217,0.0026649725,
         0.0012252747,0.0007391367,0.0007408374,0.0007538843,0.0007760073,0.0009904326,
         0.0015486111,0.0021799679,0.0019876678,0.001044887,0.0004923789,0.0002830611,
         0.0002144231,0.0001957036,0.0002132886,0.0002819266,0.000417501,0.0006069659,
         0.0006410002,0.000437355,0.0002654762,0.0001905983,0.0001622354,0.0001707443,
         0.0001849257,0.0002461883,0.0003834656,0.0005627165,0.0006784391,0.0006143403,
         0.0004844373,0.0003880048,0.0003596408,0.000353401,0.0003970798,0.0004623154,
         0.0005513736,0.0006410013,0.0007306268,0.0007572878,0.0007669312,0.0007192805,
         0.0006364621,0.0005706603,0.0005286833,0.0004878409,0.0004764957,0.0005519409,
         0.0007181471,0.0012241402,0.0022656237,0.0028788283,0.00204099,0.0008389819,
         0.0003789275,0.0002252021,0.0001395452,0.0001242292,9.98371972976e-05,9.70007998321e-05,
         0.0001043753,0.0001242292,0.0001724461,0.000286467,0.0005275488,0.0008208217,
         0.0006602869,0.0003221996,0.000167908,0.0001032404,6.80708163393e-05,5.72929370855e-05,
         4.76495714375e-05,3.23336377612e-05,3.63044353809e-05,4.14097466064e-05,4.4246e-05,3.91404924924e-05,
         5.78601938884e-05,6.35326484653e-05,6.2398248311e-05,9.01938316495e-05,0.0001009717,0.0001525921,
         0.0001917328,0.0001968381,0.0001860602,0.0001429487,0.0001361416,9.47317726208e-05,
         8.96265748467e-05,8.79249178896e-05,7.9416e-05,7.54452682274e-05,7.60124115788e-05,8.50885204241e-05,
         9.41646292693e-05,8.22522364099e-05,8.1685e-05,0.0001066443,0.000121393,0.0001350071,
         0.0001690425,0.0002036441,0.0002507275,0.0003187972,0.0003970798,0.000508261,
         0.0006330586,0.000795859,0.0009688746,0.0011538003,0.0013523402,0.0015542836,
         0.0017562271,0.0019888024,0.002250875,0.0025345034,0.0029032203,0.0052357916,
         0.0064009258,0.0080125023,0.0101470897,0.0131093047,0.017134559,0.0229045817,
         0.030977894,0.0426621362,0.0591898438,0.0835072354,0.1178884433,0.1663775548,
         0.2311140356,0.3159359454,0.417216244,0.528495011,0.6409593447,0.7472394448,
         0.8385519068,0.9088123344,0.9593662607,0.988113701,1.0,0.9992058405,
         0.9896418908,0.9676958596,0.9514360106,0.9374939729,0.9254715322,0.9152234708,
         0.9087828371,0.9026076795,0.8966832495,0.8924832801,0.8854912728,0.8776551873,
         0.8676385667,0.8568958573,0.8474408209,0.8360480353,0.8236977202,0.8130435029,
         0.8016268925,0.7921616455,0.7827951012,0.7769353384,0.7714647138,0.7631453255,
         0.7543403654,0.7452370283,0.734920896,0.7255123747,0.714614237,0.7030887133,
         0.6919477897,0.6816475407,0.6705202312,0.6595358705,0.6475962493,0.6291853625,
         0.611093274,0.5934323007,0.5760594939,0.5599936467,0.5448751184,0.5306142824,
         0.5154832744,0.4976759489,0.4734098374,0.4387697334,0.3934220901,0.3367690187,
         0.2742732022,0.2114336281,0.1535541475,0.1063407965,0.0704419498,0.0459120639,
         0.0295347927,0.0192753862,0.0126804586,0.0084623824,0.0057690017
        };

      //Add multispectral bands to the temporary list
      const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
      const std::vector<float> vb1 (b1, b1 + sizeof(b1) / sizeof(float) );
      const std::vector<float> vb2 (b2, b2 + sizeof(b2) / sizeof(float) );
      const std::vector<float> vb3 (b3, b3 + sizeof(b3) / sizeof(float) );
      //For Pleiades MS image the order of band in 1A product is: B2 B1 B0 B3
      //(BandIndexToWavelength method could be used here)
      tmpSpectralBandList.push_back(vb2);
      tmpSpectralBandList.push_back(vb1);
      tmpSpectralBandList.push_back(vb0);
      tmpSpectralBandList.push_back(vb3);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Pleiades Sensor ID");
      }
    }
  else
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  unsigned int j = 0;
  for (std::list <std::vector<float> >::const_iterator it = tmpSpectralBandList.begin(); it != tmpSpectralBandList.end(); ++it)
    {
    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(*it);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.430);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(0.950);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.0025);
    ++j;
    }
  return wavelengthSpectralBand;
}

} // end namespace otb
