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
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0004358f,
          0.0008051f,
          0.0030464f,
          0.0060688f,
          0.0130170f,
          0.0240166f,
          0.0421673f,
          0.0718226f,
          0.1151660f,
          0.1738520f,
          0.2442960f,
          0.3204090f,
          0.3940400f,
          0.4552560f,
          0.5015220f,
          0.5324310f,
          0.5537040f,
          0.5683830f,
          0.5824280f,
          0.5967100f,
          0.6107110f,
          0.6232080f,
          0.6335930f,
          0.6421600f,
          0.6501020f,
          0.6570500f,
          0.6640260f,
          0.6707420f,
          0.6765340f,
          0.6808170f,
          0.6829760f,
          0.6834310f,
          0.6831800f,
          0.6831390f,
          0.6846760f,
          0.6892820f,
          0.6964400f,
          0.7073100f,
          0.7193370f,
          0.7323050f,
          0.7449550f,
          0.7554270f,
          0.7638770f,
          0.7715940f,
          0.7776340f,
          0.7826480f,
          0.7871060f,
          0.7913600f,
          0.7949010f,
          0.7974290f,
          0.7996850f,
          0.8007000f,
          0.8003370f,
          0.8002560f,
          0.8000160f,
          0.8012780f,
          0.8035920f,
          0.8077100f,
          0.8136680f,
          0.8209800f,
          0.8302930f,
          0.8390660f,
          0.8482460f,
          0.8564130f,
          0.8634600f,
          0.8689680f,
          0.8738000f,
          0.8767770f,
          0.8799960f,
          0.8825630f,
          0.8849870f,
          0.8894330f,
          0.8939110f,
          0.8994160f,
          0.9056300f,
          0.9123790f,
          0.9188800f,
          0.9244770f,
          0.9289540f,
          0.9338010f,
          0.9374110f,
          0.9414880f,
          0.9449190f,
          0.9480210f,
          0.9509810f,
          0.9532570f,
          0.9560420f,
          0.9581430f,
          0.9600910f,
          0.9609580f,
          0.9631350f,
          0.9649320f,
          0.9674470f,
          0.9728920f,
          0.9774240f,
          0.9827830f,
          0.9874630f,
          0.9926650f,
          0.9966400f,
          0.9993680f,
          0.9999140f,
          0.9942900f,
          0.9882640f,
          0.9810720f,
          0.9751580f,
          0.9699990f,
          0.9659940f,
          0.9632210f,
          0.9624050f,
          0.9621500f,
          0.9633410f,
          0.9654310f,
          0.9671030f,
          0.9672570f,
          0.9677370f,
          0.9653110f,
          0.9633140f,
          0.9579490f,
          0.9533250f,
          0.9486410f,
          0.9423590f,
          0.9358410f,
          0.9299340f,
          0.9243940f,
          0.9190090f,
          0.9141220f,
          0.9087800f,
          0.9039750f,
          0.8980690f,
          0.8927220f,
          0.8870500f,
          0.8828150f,
          0.8771870f,
          0.8734510f,
          0.8694140f,
          0.8664310f,
          0.8660560f,
          0.8588170f,
          0.8477750f,
          0.8331070f,
          0.8135860f,
          0.7865930f,
          0.7489860f,
          0.6981680f,
          0.6306270f,
          0.5506620f,
          0.4616840f,
          0.3695890f,
          0.2823490f,
          0.2074510f,
          0.1465300f,
          0.1010100f,
          0.0686868f,
          0.0471034f,
          0.0324221f,
          0.0246752f,
          0.0174143f,
          0.0126697f,
          0.0116629f,
          0.0086694f,
          0.0081772f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f,
          0.0000000f
        };
      //add panchromatic band to the temporary list
      const std::vector<float> vpan (pan, pan + sizeof(pan) / sizeof(float) );
      tmpSpectralBandList.push_back(vpan);
      }
    else if (sensorId == "PHR 1B")
      {
      const float pan[209] =
        {
          8.4949016432e-06f,1.27423524648e-05f,2.54847049296e-05f,4.10586912755e-05f,5.94643115024e-05f,7.64541147888e-05f,
          0.0001047705f,0.0001628184f,0.0004077553f,0.0007560434f,0.0027948226f,0.0055783471f,
          0.0119863062f,0.022134882f,0.0389179761f,0.0664188043f,0.1067016279f,0.1614272001f,
          0.2272018077f,0.2986552571f,0.367815083f,0.4258041132f,0.4697397445f,0.4991660838f,
          0.5193159905f,0.5332504608f,0.5467262065f,0.5603038909f,0.5736437182f,0.5855337488f,
          0.5956625032f,0.6041036038f,0.6120151889f,0.618774299f,0.625564557f,0.6319045853f,
          0.6377943837f,0.6421862479f,0.6443949223f,0.6448819633f,0.644884795f,0.6451141573f,
          0.6468357907f,0.6513182672f,0.6590911022f,0.6691405708f,0.6811806781f,0.6937021631f,
          0.7056091836f,0.7157690859f,0.7250568451f,0.7323511339f,0.7384193253f,0.7438758838f,
          0.7492531566f,0.7536450207f,0.7580793594f,0.7610893862f,0.7639125252f,0.7652348982f,
          0.766172169f,0.7665969141f,0.767494542f,0.7687546191f,0.7723026564f,0.7771079391f,
          0.7833828397f,0.7917191699f,0.8015449394f,0.8109516272f,0.8200326771f,0.8290684208f,
          0.8364646485f,0.8426517685f,0.8481876127f,0.8519168746f,0.8555045547f,0.8583361886f,
          0.8620173126f,0.8666838452f,0.8715797402f,0.8780188757f,0.8847071949f,0.8913473763f,
          0.898890849f,0.9045824331f,0.9105090428f,0.9154615705f,0.9207935371f,0.9253156564f,
          0.9289882855f,0.9328931087f,0.9369565033f,0.9400797954f,0.9441601799f,0.9469295178f,
          0.9503472999f,0.9521085762f,0.9542804394f,0.9574405428f,0.9607705442f,0.9663148834f,
          0.9720999114f,0.9788278735f,0.9845959117f,0.991221935f,0.9956874216f,0.9990882139f,
          1.0f,0.9963273709f,0.9914682871f,0.9869688209f,0.9817019819f,0.9771458829f,
          0.9745974125f,0.9729012638f,0.9739319785f,0.9741924888f,0.9768712145f,0.9798246086f,
          0.9827723395f,0.9845732586f,0.9850772894f,0.9837266001f,0.9825967782f,0.9790232562f,
          0.9745945808f,0.9697411603f,0.9648141174f,0.9598389367f,0.9533346736f,0.9483566613f,
          0.9445481137f,0.93968903f,0.9344561706f,0.9297160154f,0.9248342786f,0.9183356789f,
          0.9111942982f,0.9061001889f,0.9006181457f,0.8953456434f,0.8908093659f,0.8894954878f,
          0.8885412272f,0.8809779331f,0.8707840511f,0.8566683562f,0.8369346997f,0.8102408871f,
          0.7728519933f,0.718954674f,0.6528501811f,0.5710810895f,0.4795853355f,0.3846491464f,
          0.294773087f,0.2166449103f,0.1531319287f,0.1057054591f,0.0716799801f,0.0491432892f,
          0.0337757289f,0.0256986066f,0.0181762579f,0.0131968863f,0.0121562043f,0.0090201414f,
          0.0084156159f,0.0042799863f,0.0033130116f,0.0026688064f,0.0022115061f,0.0018292327f,
          0.0015290823f,0.0013634289f,0.0012147709f,0.0011015056f,0.0009939035f,0.0009188652f,
          0.0008523218f,0.0008225925f,0.0006965819f,0.0007220666f,0.0006682656f,0.0006470283f,
          0.0006144646f,0.000603138f,0.0006116329f,0.0005578319f,0.0005295155f,0.0005295155f,
          0.0005210206f,0.0005054466f,0.0004473982f,0.0004473953f,0.0004077553f,0.0004558931f,
          0.0004077553f,0.0004346558f,0.0003766073f,0.0003851022f,0.0003879338f,0.0003723599f,
          0.0003341328f,0.0003723599f,0.0003313012f,0.0003341328f,0.0003143114f
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
          0.0098681f,
          0.0293268f,
          0.0877320f,
          0.1287040f,
          0.1341240f,
          0.2457050f,
          0.4345520f,
          0.5133040f,
          0.4710970f,
          0.5125880f,
          0.6530370f,
          0.7707870f,
          0.7879420f,
          0.7648330f,
          0.7718380f,
          0.8013290f,
          0.8240790f,
          0.8352890f,
          0.8326150f,
          0.8249150f,
          0.8168160f,
          0.8163380f,
          0.8285420f,
          0.8623820f,
          0.9075060f,
          0.9379000f,
          0.9505710f,
          0.9572260f,
          0.9650570f,
          0.9632790f,
          0.9587260f,
          0.9567320f,
          0.9646760f,
          0.9804620f,
          0.9900240f,
          0.9838940f,
          0.9719110f,
          0.9715280f,
          0.9574890f,
          0.8770130f,
          0.7103910f,
          0.4943810f,
          0.3021990f,
          0.1722720f,
          0.0943537f,
          0.0543895f,
          0.0345732f,
          0.0261018f,
          0.0230010f,
          0.0223203f,
          0.0210136f,
          0.0173172f,
          0.0119112f,
          0.0072895f,
          0.0046311f,
          0.0033297f,
          0.0025865f,
          0.0020232f,
          0.0015030f,
          0.0010527f,
          0.0007044f,
          0.0005199f,
          0.0004117f,
          0.0004097f,
          0.0005317f,
          0.0009532f,
          0.0013521f,
          0.0014273f,
          0.0009182f,
          0.0003440f,
          0.0001323f,
          0.0000783f,
          0.0000626f,
          0.0000511f,
          0.0000538f,
          0.0000533f,
          0.0000454f,
          0.0000404f,
          0.0000315f,
          0.0000327f,
          0.0000262f,
          0.0000303f,
          0.0000206f,
          0.0000241f,
          0.0000241f,
          0.0000273f,
          0.0000258f,
          0.0000208f,
          0.0000341f,
          0.0000379f,
          0.0000393f,
          0.0000429f,
          0.0000281f,
          0.0000277f,
          0.0000187f,
          0.0000272f,
          0.0000245f,
          0.0000209f,
          0.0000137f,
          0.0000171f,
          0.0000257f,
          0.0000300f,
          0.0000330f,
          0.0000446f,
          0.0000397f,
          0.0000399f,
          0.0000384f,
          0.0000336f,
          0.0000307f,
          0.0000300f,
          0.0000242f,
          0.0000224f,
          0.0000210f,
          0.0000325f,
          0.0000690f,
          0.0002195f,
          0.0005063f,
          0.0008373f,
          0.0009464f,
          0.0007099f,
          0.0004910f,
          0.0004433f,
          0.0006064f,
          0.0012019f,
          0.0016241f,
          0.0016779f,
          0.0009733f,
          0.0003606f,
          0.0001659f,
          0.0000864f,
          0.0000564f,
          0.0000562f,
          0.0000590f,
          0.0000458f,
          0.0000382f,
          0.0000586f,
          0.0000685f,
          0.0000474f,
          0.0000872f,
          0.0000628f,
          0.0000948f,
          0.0001015f,
          0.0001564f,
          0.0002379f,
          0.0003493f,
          0.0005409f,
          0.0007229f,
          0.0007896f,
          0.0007188f,
          0.0005204f,
          0.0003939f,
          0.0003128f,
          0.0002699f,
          0.0002605f,
          0.0002378f,
          0.0002286f,
          0.0002406f,
          0.0002741f,
          0.0003203f,
          0.0003812f,
          0.0004904f,
          0.0006077f,
          0.0008210f,
          0.0011791f,
          0.0018150f,
          0.0030817f,
          0.0055589f,
          0.0103652f,
          0.0166309f,
          0.0211503f,
          0.0216246f,
          0.0176910f,
          0.0136927f,
          0.0107136f,
          0.0089555f,
          0.0079790f,
          0.0079189f,
          0.0080456f,
          0.0088920f,
          0.0102062f,
          0.0126157f,
          0.0162251f,
          0.0221306f,
          0.0308295f,
          0.0411980f,
          0.0498232f,
          0.0531265f,
          0.0484487f,
          0.0391122f,
          0.0291405f,
          0.0212633f,
          0.0162146f,
          0.0128925f,
          0.0108169f,
          0.0094115f,
          0.0084386f,
          0.0077249f,
          0.0074231f,
          0.0072603f,
          0.0073459f,
          0.0074214f,
          0.0076433f,
          0.0077788f,
          0.0078151f,
          0.0077003f,
          0.0072256f,
          0.0065903f,
          0.0057120f,
          0.0048136f
        };
      //B1 green band
      const float b1[209] =
        {
          0.0000144f,
          0.0000143f,
          0.0000259f,
          0.0000189f,
          0.0000132f,
          0.0000179f,
          0.0000224f,
          0.0000179f,
          0.0000124f,
          0.0000202f,
          0.0000276f,
          0.0000292f,
          0.0000420f,
          0.0000366f,
          0.0000261f,
          0.0000247f,
          0.0000445f,
          0.0000902f,
          0.0001144f,
          0.0000823f,
          0.0000778f,
          0.0001923f,
          0.0003401f,
          0.0004085f,
          0.0004936f,
          0.0007849f,
          0.0045979f,
          0.0085122f,
          0.0143014f,
          0.0243310f,
          0.0480572f,
          0.1097360f,
          0.2353890f,
          0.4328370f,
          0.6491340f,
          0.8095770f,
          0.8847680f,
          0.9066640f,
          0.9131150f,
          0.9186700f,
          0.9273270f,
          0.9405210f,
          0.9512930f,
          0.9587500f,
          0.9667360f,
          0.9709750f,
          0.9728630f,
          0.9769560f,
          0.9850710f,
          0.9892500f,
          0.9865960f,
          0.9743300f,
          0.9575190f,
          0.9435550f,
          0.9439310f,
          0.9571350f,
          0.9712530f,
          0.9761580f,
          0.9619590f,
          0.9244890f,
          0.8734580f,
          0.8349840f,
          0.8166740f,
          0.8015960f,
          0.7435910f,
          0.6160350f,
          0.4321320f,
          0.2544540f,
          0.1360870f,
          0.0769553f,
          0.0479321f,
          0.0342014f,
          0.0266703f,
          0.0212632f,
          0.0160541f,
          0.0106967f,
          0.0060543f,
          0.0030797f,
          0.0015416f,
          0.0008333f,
          0.0004706f,
          0.0002918f,
          0.0001917f,
          0.0001472f,
          0.0001063f,
          0.0000912f,
          0.0000589f,
          0.0000552f,
          0.0000752f,
          0.0000884f,
          0.0000985f,
          0.0001125f,
          0.0001368f,
          0.0001947f,
          0.0002284f,
          0.0002088f,
          0.0001498f,
          0.0000637f,
          0.0000307f,
          0.0000283f,
          0.0000311f,
          0.0000331f,
          0.0000215f,
          0.0000236f,
          0.0000205f,
          0.0000186f,
          0.0000233f,
          0.0000233f,
          0.0000198f,
          0.0000195f,
          0.0000161f,
          0.0000308f,
          0.0000464f,
          0.0000290f,
          0.0000264f,
          0.0000233f,
          0.0000395f,
          0.0001113f,
          0.0001903f,
          0.0002290f,
          0.0002229f,
          0.0001322f,
          0.0000548f,
          0.0000608f,
          0.0000414f,
          0.0000382f,
          0.0000381f,
          0.0000269f,
          0.0000233f,
          0.0000198f,
          0.0000208f,
          0.0000302f,
          0.0000419f,
          0.0000305f,
          0.0000340f,
          0.0000334f,
          0.0000362f,
          0.0000282f,
          0.0000337f,
          0.0000330f,
          0.0000424f,
          0.0000420f,
          0.0000470f,
          0.0000417f,
          0.0000233f,
          0.0000439f,
          0.0000503f,
          0.0000446f,
          0.0000428f,
          0.0000597f,
          0.0000671f,
          0.0001142f,
          0.0001780f,
          0.0003546f,
          0.0009610f,
          0.0041260f,
          0.0066679f,
          0.0078563f,
          0.0068645f,
          0.0029441f,
          0.0011320f,
          0.0007028f,
          0.0005471f,
          0.0004967f,
          0.0004929f,
          0.0005351f,
          0.0006223f,
          0.0007957f,
          0.0010708f,
          0.0016699f,
          0.0030334f,
          0.0054959f,
          0.0091390f,
          0.0125045f,
          0.0144212f,
          0.0141099f,
          0.0117418f,
          0.0089824f,
          0.0067916f,
          0.0056849f,
          0.0051998f,
          0.0053640f,
          0.0060350f,
          0.0067668f,
          0.0083174f,
          0.0106521f,
          0.0139110f,
          0.0183736f,
          0.0231289f,
          0.0272661f,
          0.0298126f,
          0.0300318f,
          0.0286507f,
          0.0266172f,
          0.0247529f,
          0.0236974f,
          0.0232734f,
          0.0236733f,
          0.0245808f,
          0.0257173f,
          0.0267721f,
          0.0267455f,
          0.0254447f,
          0.0227056f,
          0.0188513f,
          0.0147988f,
          0.0109864f,
          0.0079795f,
          0.0057516f
        };
      //B2 red band
      const float b2[209] =
        {
          0.0097386f,
          0.0035306f,
          0.0035374f,
          0.0114418f,
          0.0266686f,
          0.0373494f,
          0.0904431f,
          0.0907580f,
          0.0399312f,
          0.0208748f,
          0.0080694f,
          0.0027002f,
          0.0011241f,
          0.0006460f,
          0.0005029f,
          0.0006051f,
          0.0009979f,
          0.0019446f,
          0.0014554f,
          0.0006090f,
          0.0003230f,
          0.0002503f,
          0.0002538f,
          0.0003360f,
          0.0005377f,
          0.0007773f,
          0.0004895f,
          0.0002045f,
          0.0000875f,
          0.0000594f,
          0.0000217f,
          0.0000290f,
          0.0000297f,
          0.0000408f,
          0.0000456f,
          0.0000447f,
          0.0000322f,
          0.0000222f,
          0.0000147f,
          0.0000095f,
          0.0000072f,
          0.0000113f,
          0.0000313f,
          0.0000123f,
          0.0000122f,
          0.0000280f,
          0.0000180f,
          0.0000261f,
          0.0000138f,
          0.0000392f,
          0.0000517f,
          0.0000695f,
          0.0000797f,
          0.0000785f,
          0.0001004f,
          0.0001170f,
          0.0001483f,
          0.0001837f,
          0.0002110f,
          0.0002973f,
          0.0004162f,
          0.0006371f,
          0.0010012f,
          0.0032888f,
          0.0100109f,
          0.0181837f,
          0.0330510f,
          0.0624784f,
          0.1183670f,
          0.2218740f,
          0.3756820f,
          0.5574830f,
          0.7342220f,
          0.8636840f,
          0.9319920f,
          0.9527010f,
          0.9620090f,
          0.9527340f,
          0.9437220f,
          0.9456300f,
          0.9562330f,
          0.9693120f,
          0.9839640f,
          0.9949160f,
          0.9992700f,
          0.9993300f,
          0.9963430f,
          0.9944130f,
          0.9883050f,
          0.9857580f,
          0.9807560f,
          0.9683790f,
          0.9544700f,
          0.9371750f,
          0.9170350f,
          0.8922820f,
          0.8662710f,
          0.8442750f,
          0.8220420f,
          0.7888070f,
          0.7372920f,
          0.6625080f,
          0.5662120f,
          0.4493120f,
          0.3260000f,
          0.2194040f,
          0.1416500f,
          0.0925669f,
          0.0619437f,
          0.0456444f,
          0.0355683f,
          0.0310879f,
          0.0295168f,
          0.0233351f,
          0.0189628f,
          0.0158627f,
          0.0132266f,
          0.0107473f,
          0.0083969f,
          0.0063847f,
          0.0046601f,
          0.0033814f,
          0.0024167f,
          0.0017478f,
          0.0012949f,
          0.0009939f,
          0.0007442f,
          0.0006312f,
          0.0005142f,
          0.0004354f,
          0.0003549f,
          0.0003156f,
          0.0003079f,
          0.0002906f,
          0.0002867f,
          0.0002751f,
          0.0003048f,
          0.0003010f,
          0.0003342f,
          0.0004310f,
          0.0004955f,
          0.0005488f,
          0.0005838f,
          0.0006687f,
          0.0006968f,
          0.0006650f,
          0.0005866f,
          0.0004688f,
          0.0004086f,
          0.0003611f,
          0.0002404f,
          0.0002609f,
          0.0002476f,
          0.0002133f,
          0.0002098f,
          0.0001916f,
          0.0001642f,
          0.0001799f,
          0.0002180f,
          0.0002003f,
          0.0002030f,
          0.0002348f,
          0.0002735f,
          0.0002652f,
          0.0002944f,
          0.0004666f,
          0.0004882f,
          0.0006642f,
          0.0007798f,
          0.0010588f,
          0.0014008f,
          0.0019011f,
          0.0024917f,
          0.0034379f,
          0.0042182f,
          0.0053618f,
          0.0062814f,
          0.0068774f,
          0.0071141f,
          0.0070399f,
          0.0065876f,
          0.0067873f,
          0.0066877f,
          0.0068572f,
          0.0070486f,
          0.0073911f,
          0.0081201f,
          0.0087391f,
          0.0096581f,
          0.0106625f,
          0.0120129f,
          0.0137222f,
          0.0159817f,
          0.0180896f,
          0.0206562f,
          0.0236408f,
          0.0269627f,
          0.0310497f,
          0.0353146f,
          0.0398729f,
          0.0438795f,
          0.0462377f,
          0.0454916f,
          0.0408754f,
          0.0333175f,
          0.0251186f,
          0.0179089f,
          0.0125129f,
          0.0086117f
        };
      //B3 nir band
      const float b3[209] =
        {
          0.0024163f,
          0.0017305f,
          0.0020803f,
          0.0020499f,
          0.0012660f,
          0.0007361f,
          0.0006198f,
          0.0006344f,
          0.0007721f,
          0.0011837f,
          0.0020819f,
          0.0023991f,
          0.0013377f,
          0.0006328f,
          0.0003544f,
          0.0002890f,
          0.0002498f,
          0.0002541f,
          0.0003346f,
          0.0005048f,
          0.0008684f,
          0.0009871f,
          0.0006587f,
          0.0003833f,
          0.0002606f,
          0.0002356f,
          0.0002364f,
          0.0002791f,
          0.0003613f,
          0.0005575f,
          0.0007414f,
          0.0007413f,
          0.0005768f,
          0.0004230f,
          0.0003206f,
          0.0003044f,
          0.0003019f,
          0.0003201f,
          0.0003813f,
          0.0004630f,
          0.0005930f,
          0.0007080f,
          0.0008577f,
          0.0009017f,
          0.0008813f,
          0.0007801f,
          0.0006583f,
          0.0005863f,
          0.0005224f,
          0.0005506f,
          0.0006403f,
          0.0008293f,
          0.0013444f,
          0.0023942f,
          0.0027274f,
          0.0014330f,
          0.0006388f,
          0.0003596f,
          0.0002416f,
          0.0001718f,
          0.0001566f,
          0.0001642f,
          0.0001892f,
          0.0002351f,
          0.0003227f,
          0.0006734f,
          0.0014311f,
          0.0013325f,
          0.0005796f,
          0.0002424f,
          0.0001263f,
          0.0001022f,
          0.0000446f,
          0.0000652f,
          0.0000544f,
          0.0000573f,
          0.0000518f,
          0.0000504f,
          0.0000649f,
          0.0000723f,
          0.0000833f,
          0.0000739f,
          0.0000691f,
          0.0001382f,
          0.0001692f,
          0.0002240f,
          0.0002296f,
          0.0001553f,
          0.0001492f,
          0.0001121f,
          0.0001058f,
          0.0001068f,
          0.0001012f,
          0.0000864f,
          0.0000533f,
          0.0000354f,
          0.0000440f,
          0.0000371f,
          0.0000691f,
          0.0000769f,
          0.0000791f,
          0.0001333f,
          0.0001244f,
          0.0002048f,
          0.0002455f,
          0.0002721f,
          0.0003812f,
          0.0004568f,
          0.0006255f,
          0.0008185f,
          0.0009733f,
          0.0012281f,
          0.0013528f,
          0.0015758f,
          0.0017458f,
          0.0019104f,
          0.0020863f,
          0.0023053f,
          0.0025241f,
          0.0037234f,
          0.0044186f,
          0.0053574f,
          0.0066118f,
          0.0083509f,
          0.0107509f,
          0.0150393f,
          0.0212756f,
          0.0292566f,
          0.0414246f,
          0.0586633f,
          0.0834879f,
          0.1190380f,
          0.1671850f,
          0.2326370f,
          0.3124060f,
          0.4070470f,
          0.5091930f,
          0.6148270f,
          0.7140870f,
          0.8017550f,
          0.8714840f,
          0.9241260f,
          0.9587210f,
          0.9782990f,
          0.9882040f,
          0.9922940f,
          0.9902030f,
          0.9854020f,
          0.9777560f,
          0.9660200f,
          0.9532070f,
          0.9421250f,
          0.9303560f,
          0.9241490f,
          0.9212220f,
          0.9203820f,
          0.9217020f,
          0.9227420f,
          0.9230000f,
          0.9237670f,
          0.9243070f,
          0.9206520f,
          0.9154840f,
          0.9090910f,
          0.9003380f,
          0.8905620f,
          0.8776420f,
          0.8668600f,
          0.8537290f,
          0.8428590f,
          0.8305310f,
          0.8195740f,
          0.8069090f,
          0.7921080f,
          0.7791670f,
          0.7660510f,
          0.7521190f,
          0.7375270f,
          0.7217320f,
          0.7043220f,
          0.6853170f,
          0.6642500f,
          0.6413850f,
          0.6173030f,
          0.5919540f,
          0.5672310f,
          0.5430130f,
          0.5184560f,
          0.4957540f,
          0.4734340f,
          0.4528220f,
          0.4332270f,
          0.4131920f,
          0.3919120f,
          0.3659660f,
          0.3325420f,
          0.2917680f,
          0.2453910f,
          0.1962540f,
          0.1486850f,
          0.1068860f,
          0.0738260f,
          0.0491777f,
          0.0327991f,
          0.0215831f,
          0.0145386f,
          0.0103219f,
          0.0076144f,
          0.0061346f
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
        {0.0016025229f,0.0013381709f,0.0043437933f,0.0110724631f,0.0176270388f,0.0233671533f,
         0.057403285f,0.1345548468f,0.2237631463f,0.3085427458f,0.4615565735f,0.650820526f,
         0.755369094f,0.7477203647f,0.7168186424f,0.7185381794f,0.7566962056f,0.8101088802f,
         0.842166474f,0.8234370764f,0.775247228f,0.7527006008f,0.7802203291f,0.8199266521f,
         0.8516631705f,0.8602751259f,0.858684019f,0.8658261626f,0.8828217532f,0.9040412689f,
         0.9196954778f,0.9325955734f,0.9502333148f,0.9757980507f,0.9949769539f,1.0f,
         0.995062574f,0.9806284516f,0.9417497895f,0.8436220158f,0.6711424576f,0.4633403256f,
         0.2888651055f,0.1670783567f,0.0901800876f,0.0505194286f,0.0314882201f,0.0238137335f,
         0.0213108438f,0.0206298072f,0.0195599127f,0.0167943833f,0.0113582202f,0.0066519685f,
         0.004144013f,0.0030298814f,0.0024159139f,0.0019903178f,0.0015682749f,0.0011355438f,
         0.0012529075f,0.0008355166f,0.0005508227f,0.000419896f,0.0003617449f,0.000377798f,
         0.0005322716f,0.0011091514f,0.0019870999f,0.0012200722f,0.0003753015f,0.0001469798f,
         7.49175906503e-05f,5.2799e-05f,5.56530673402e-05f,5.74373902992e-05f,5.63665682035e-05f,3.78155457568e-05f,
         3.49615423035e-05f,2.06915250367e-05f,1.35565164034e-05f,1.99783095738e-05f,4.281e-06f,1.10591920316e-05f,
         1.21295146767e-05f,1.14159424633e-05f,9.27551122337e-06f,1.1773e-05f,1.92645233101e-05f,8.919e-06f,
         1.1416e-05f,1.71237353197e-05f,4.99450604335e-06f,8.56172495969e-06f,2.4259e-05f,3.85286898697e-05f,
         1.2843e-05f,2.35456711903e-05f,9.989e-06f,1.14162992137e-05f,2.06915250367e-05f,1.39132668351e-05f,
         5.708e-06f,2.854e-06f,8.562e-06f,1.24862651084e-05f,8.562e-06f,9.27565392354e-06f,
         6.42150777002e-06f,1.1773e-05f,5.708e-06f,1.35566591036e-05f,1.7124e-05f,7.13522268362e-06f,
         9.989e-06f,2.71133895572e-05f,6.35015768369e-05f,0.0002194051f,0.0007613054f,0.0011194829f,
         0.0007541704f,0.0004084771f,0.0003546099f,0.0004063402f,0.0006792528f,0.0016292935f,
         0.0024002169f,0.0010317222f,0.0003481884f,0.000165889f,8.2766100147e-05f,9.70361174137e-05f,
         4.63775561168e-05f,2.63994605933e-05f,3.21075388501e-05f,4.1026442342e-05f,1.5697e-05f,8.9192602423e-06f,
         4.70910569802e-05f,7.88411318978e-05f,2.21185267634e-05f,5.35134923013e-05f,8.2766100147e-05f,0.0001052421f,
         0.0001833697f,0.00026043f,0.000441657f,0.000710293f,0.000864763f,0.0007370464f,
         0.0005515362f,0.000394566f,0.0002811193f,0.0002340283f,0.0002247528f,0.0001922885f,
         0.0002197583f,0.0002340276f,0.0002454443f,0.0002454436f,0.0002782653f,0.000350685f,
         0.000431668f,0.0005326263f,0.0006885283f,0.0010277909f,0.0015625669f,0.0026085734f,
         0.0045749675f,0.0090618891f,0.0171625498f,0.0237927565f,0.021523467f,0.0159138519f,
         0.011956134f,0.0094824265f,0.007869201f,0.0072241962f,0.0072420338f,0.0075677469f,
         0.0083793541f,0.0098637927f,0.0122586583f,0.0162673916f,0.0226015668f,0.0320271273f,
         0.0444304123f,0.0546690783f,0.0564243618f,0.048004124f,0.0357991923f,0.0258336544f,
         0.0188870814f,0.0144386889f,0.0116792956f,0.0099112405f,0.008646917f,0.0078099805f,
         0.0072270502f,0.006995876f,0.0069230989f,0.0069141801f,0.0070208485f,0.0072527363f,
         0.0073726044f,0.0075049588f,0.007470354f,0.0070672332f,0.0063679952f
        };
      //B1 green band
      const float b1[209] =
        {3.2793605247e-06f,3.2793605247e-06f,4.919e-06f,6.5587210494e-06f,6.5587210494e-06f,2.86944045911e-06f,
         6.5587210494e-06f,5.73885632302e-06f,4.09920065587e-06f,2.86943226071e-06f,1.47571223611e-05f,2.29553597049e-05f,
         1.06579217053e-05f,1.27075220332e-05f,2.21356835417e-05f,1.8856323017e-05f,9.01824144292e-05f,0.0001151875f,
         0.0001123181f,0.0001893831f,0.000323017f,0.0004849346f,0.0006238983f,0.0008153327f,
         0.0015208034f,0.0041299447f,0.009953679f,0.0196421398f,0.0325476532f,0.0567927854f,
         0.1237909408f,0.2859094077f,0.5289756098f,0.7716253331f,0.8838040582f,0.9079565485f,
         0.9131461365f,0.913728223f,0.9224841156f,0.9367083419f,0.9497601968f,0.9544988727f,
         0.9585816766f,0.9642057799f,0.9705267473f,0.9722648084f,0.967517934f,0.9589096126f,
         0.9524492724f,0.9524656692f,0.9560483706f,0.95517934f,0.948989547f,0.9471449067f,
         0.9544496823f,0.9710596434f,0.9898175856f,1.0f,0.9953597049f,0.9698216848f,
         0.9253043656f,0.8633244517f,0.7948284485f,0.7238967001f,0.6453265013f,0.5438524288f,
         0.4170280795f,0.2766706292f,0.1575273622f,0.0856068867f,0.049225661f,0.0327237549f,
         0.0237934003f,0.0181966796f,0.0140618979f,0.0099658947f,0.0058454601f,0.0030383275f,
         0.0015355606f,0.0008386965f,0.0004878049f,0.0003115409f,0.0002074196f,0.0001381439f,
         9.26419348227e-05f,7.00964131994e-05f,6.39475302316e-05f,5.41093666735e-05f,4.0992e-05f,7.00968231195e-05f,
         4.75507276081e-05f,4.71408895265e-05f,6.23078499693e-05f,6.72277925804e-05f,0.0001483911f,0.0002512826f,
         0.0002992416f,0.0002303743f,0.0001270752f,6.72265628203e-05f,3.11539249846e-05f,3.15638450502e-05f,
         1.72166427547e-05f,6.5587210494e-06f,5.73888091822e-06f,1.76265628203e-05f,1.06579217053e-05f,1.72168067227e-05f,
         1.14777618364e-05f,2.86946505431e-06f,3.2793605247e-06f,2.86941586391e-06f,1.393728223e-05f,1.27072760812e-05f,
         1.72166427547e-05f,9.83840951015e-06f,7.37856118057e-06f,2.41853658537e-05f,3.2793605247e-05f,0.0001295372f,
         0.000277106f,0.0001893831f,0.0001237959f,2.4185e-05f,7.37856118057e-06f,7.37870875179e-06f,
         4.09920065587e-06f,1.0248e-05f,9.83808157409e-06f,3.2793605247e-06f,1.5577e-05f,2.29556056569e-05f,
         7.37856118057e-06f,1.02477556876e-05f,9.01824144292e-06f,6.96851813896e-06f,1.72166427547e-05f,2.29553597049e-05f,
         2.45952039352e-06f,2.1315e-05f,9.83808157409e-06f,1.1887763886e-05f,3.44332855093e-05f,9.01856937897e-06f,
         1.80364828858e-05f,1.72165607706e-05f,1.8856323017e-05f,1.8037e-05f,2.86944045911e-05f,2.86944865751e-05f,
         2.0496e-05f,4.34515269522e-05f,2.95142447223e-05f,5.28796884608e-05f,9.34617749539e-05f,0.0001336339f,
         0.000277106f,0.0007050527f,0.0031850789f,0.0141905308f,0.0083394138f,0.0022443452f,
         0.0009182209f,0.000571841f,0.0004369748f,0.0004029514f,0.0004181185f,0.000445174f,
         0.0005173191f,0.0006021734f,0.0007575323f,0.0011108916f,0.0019381021f,0.0036905349f,
         0.0063570404f,0.0102714491f,0.013107604f,0.0132605042f,0.0111153925f,0.0083664685f,
         0.0065644599f,0.0056847797f,0.0053797909f,0.0056228653f,0.0062004509f,0.007239172f,
         0.0089198606f,0.0115101455f,0.0149415864f,0.0192686206f,0.0234785817f,0.0264402542f,
         0.0270489854f,0.0255453986f,0.0233974175f,0.0213945481f,0.0199442509f,0.0192531256f,
         0.0190744005f,0.0196892806f,0.0206935848f,0.0220106579f,0.0232301701f,0.0237573273f,
         0.0229858578f,0.0206595614f,0.0172248412f,0.01329215f,0.0097815126f
        };
      //B2 red band
      const float b2[209] =
        {0.0004536879f,0.0008531472f,0.000520901f,0.0011945588f,0.0051417966f,0.0080033148f,
         0.0066930427f,0.0108594102f,0.0246908583f,0.0653593224f,0.1225423328f,0.0570092494f,
         0.0213752702f,0.0127972076f,0.006277544f,0.0023536016f,0.0009409824f,0.0005174639f,
         0.0003742544f,0.0003818922f,0.0005155545f,0.0008462731f,0.0010127781f,0.0006431042f,
         0.0003513408f,0.0002512851f,0.000223025f,0.000251667f,0.0003551597f,0.0004998969f,
         0.0005262475f,0.0003864772f,0.0002528126f,0.0001626868f,0.0001069298f,8.78352058781e-05f,
         7.02681647025e-05f,5.84297356542e-05f,5.49924767237e-05f,4.81185698901e-05f,3.66616511491e-05f,3.20791738908e-05f,
         1.90946099735e-05f,1.79493916457e-05f,3.74254355481e-05f,2.40590558097e-05f,3.43702979523e-05f,1.48934902656e-05f,
         6.87405959046e-06f,7.256e-06f,3.8189219947e-06f,2.29135319682e-06f,9.16541278728e-06f,2.94056993592e-05f,
         2.36773163671e-05f,2.90238071597e-05f,3.8953e-05f,4.5063e-05f,6.95043803035e-05f,8.13422747027e-05f,
         0.0001038747f,0.0001229693f,0.0001351898f,0.0001535207f,0.0001833083f,0.0002214967f,
         0.0007454536f,0.0012442124f,0.0021416515f,0.0038193268f,0.0068045552f,0.0123329031f,
         0.0221780076f,0.0413324983f,0.0780709861f,0.151934284f,0.277674582f,0.4510376011f,
         0.6291322645f,0.7625485958f,0.8329450763f,0.8579055504f,0.8658870974f,0.8692630244f,
         0.8752205427f,0.8857760431f,0.9005934605f,0.9174883714f,0.9348797421f,0.947810612f,
         0.9569531113f,0.9623301534f,0.9647666257f,0.9624294454f,0.9613066823f,0.9620246397f,
         0.9699145325f,0.9811574389f,0.9933932649f,1.0f,0.9809512171f,0.9522634751f,
         0.9131729895f,0.8694005056f,0.8252079403f,0.7830470415f,0.7361270021f,0.6734890435f,
         0.5877527172f,0.4804914189f,0.3630068664f,0.2523031919f,0.1626028245f,0.102221085f,
         0.0641273381f,0.0419155713f,0.0284639532f,0.0204548336f,0.0153696335f,0.0121174395f,
         0.0098810788f,0.0083168483f,0.0071016673f,0.0060949919f,0.0051723479f,0.0043142285f,
         0.0034950774f,0.0027713917f,0.0035890229f,0.0030310784f,0.0023173219f,0.001784208f,
         0.0013312762f,0.0010207978f,0.0007897531f,0.0006385253f,0.0005079166f,0.0004120624f,
         0.0003788371f,0.0003593606f,0.0002978759f,0.0002787813f,0.0002810727f,0.0002623607f,
         0.0002856554f,0.0002948208f,0.0002757262f,0.0003158256f,0.0003750181f,0.0004296295f,
         0.0005186096f,0.0005747478f,0.0006194291f,0.0006503624f,0.0006515081f,0.0006041535f,
         0.0005369404f,0.0004643817f,0.0003765457f,0.0003215532f,0.0002841278f,0.0002539583f,
         0.000226844f,0.0002226432f,0.0001672688f,0.0001978194f,0.0002207337f,0.000208895f,
         0.0002329542f,0.0002054572f,0.0001871272f,0.0002505213f,0.0002932932f,0.0002635079f,
         0.0002589229f,0.000449106f,0.0005361766f,0.000566728f,0.0008210682f,0.0009856638f,
         0.0013075989f,0.0017612792f,0.0024028657f,0.0032682258f,0.0043635003f,0.0056615366f,
         0.0068900991f,0.0078222979f,0.0083298327f,0.0084363806f,0.0081854774f,0.0081381228f,
         0.0080006416f,0.0077676873f,0.0077844906f,0.0079983502f,0.0081518709f,0.008505885f,
         0.008792686f,0.0093120594f,0.009752763f,0.0101358009f,0.0103874678f,0.0104061805f,
         0.0101713168f,0.009522482f,0.008608614f,0.0073373101f,0.0059842508f
        };
      //B3 nir band
      const float b3[209] =
        {0.0001962709f,0.0007612586f,0.0003823311f,0.0005128001f,0.0016870217f,0.0026649725f,
         0.0012252747f,0.0007391367f,0.0007408374f,0.0007538843f,0.0007760073f,0.0009904326f,
         0.0015486111f,0.0021799679f,0.0019876678f,0.001044887f,0.0004923789f,0.0002830611f,
         0.0002144231f,0.0001957036f,0.0002132886f,0.0002819266f,0.000417501f,0.0006069659f,
         0.0006410002f,0.000437355f,0.0002654762f,0.0001905983f,0.0001622354f,0.0001707443f,
         0.0001849257f,0.0002461883f,0.0003834656f,0.0005627165f,0.0006784391f,0.0006143403f,
         0.0004844373f,0.0003880048f,0.0003596408f,0.000353401f,0.0003970798f,0.0004623154f,
         0.0005513736f,0.0006410013f,0.0007306268f,0.0007572878f,0.0007669312f,0.0007192805f,
         0.0006364621f,0.0005706603f,0.0005286833f,0.0004878409f,0.0004764957f,0.0005519409f,
         0.0007181471f,0.0012241402f,0.0022656237f,0.0028788283f,0.00204099f,0.0008389819f,
         0.0003789275f,0.0002252021f,0.0001395452f,0.0001242292f,9.98371972976e-05f,9.70007998321e-05f,
         0.0001043753f,0.0001242292f,0.0001724461f,0.000286467f,0.0005275488f,0.0008208217f,
         0.0006602869f,0.0003221996f,0.000167908f,0.0001032404f,6.80708163393e-05f,5.72929370855e-05f,
         4.76495714375e-05f,3.23336377612e-05f,3.63044353809e-05f,4.14097466064e-05f,4.4246e-05f,3.91404924924e-05f,
         5.78601938884e-05f,6.35326484653e-05f,6.2398248311e-05f,9.01938316495e-05f,0.0001009717f,0.0001525921f,
         0.0001917328f,0.0001968381f,0.0001860602f,0.0001429487f,0.0001361416f,9.47317726208e-05f,
         8.96265748467e-05f,8.79249178896e-05f,7.9416e-05f,7.54452682274e-05f,7.60124115788e-05f,8.50885204241e-05f,
         9.41646292693e-05f,8.22522364099e-05f,8.1685e-05f,0.0001066443f,0.000121393f,0.0001350071f,
         0.0001690425f,0.0002036441f,0.0002507275f,0.0003187972f,0.0003970798f,0.000508261f,
         0.0006330586f,0.000795859f,0.0009688746f,0.0011538003f,0.0013523402f,0.0015542836f,
         0.0017562271f,0.0019888024f,0.002250875f,0.0025345034f,0.0029032203f,0.0052357916f,
         0.0064009258f,0.0080125023f,0.0101470897f,0.0131093047f,0.017134559f,0.0229045817f,
         0.030977894f,0.0426621362f,0.0591898438f,0.0835072354f,0.1178884433f,0.1663775548f,
         0.2311140356f,0.3159359454f,0.417216244f,0.528495011f,0.6409593447f,0.7472394448f,
         0.8385519068f,0.9088123344f,0.9593662607f,0.988113701f,1.0f,0.9992058405f,
         0.9896418908f,0.9676958596f,0.9514360106f,0.9374939729f,0.9254715322f,0.9152234708f,
         0.9087828371f,0.9026076795f,0.8966832495f,0.8924832801f,0.8854912728f,0.8776551873f,
         0.8676385667f,0.8568958573f,0.8474408209f,0.8360480353f,0.8236977202f,0.8130435029f,
         0.8016268925f,0.7921616455f,0.7827951012f,0.7769353384f,0.7714647138f,0.7631453255f,
         0.7543403654f,0.7452370283f,0.734920896f,0.7255123747f,0.714614237f,0.7030887133f,
         0.6919477897f,0.6816475407f,0.6705202312f,0.6595358705f,0.6475962493f,0.6291853625f,
         0.611093274f,0.5934323007f,0.5760594939f,0.5599936467f,0.5448751184f,0.5306142824f,
         0.5154832744f,0.4976759489f,0.4734098374f,0.4387697334f,0.3934220901f,0.3367690187f,
         0.2742732022f,0.2114336281f,0.1535541475f,0.1063407965f,0.0704419498f,0.0459120639f,
         0.0295347927f,0.0192753862f,0.0126804586f,0.0084623824f,0.0057690017f
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
