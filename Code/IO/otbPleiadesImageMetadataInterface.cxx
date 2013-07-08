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

  // Use BandIndexToWavelengthPosition because values in keywordlist are sorted by wavelength
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

  // MSD: for the moment take only topCenter value
  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.incident_angle");
  double value = atof(valueString.c_str());
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

  // MSD: for the moment take only topCenter value
  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.scene_orientation");
  double satAz = atof(valueString.c_str());

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
    otbMsgDevMacro(<< "Pleiades detected: band 0 and 2 inverted");
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

  int nbBands = this->GetNumberOfBands();

  std::string key = "support_data.band_name_list";
  std::vector<unsigned int> rgb(3);

  // TODO MSD remove this limitation when we get a real pleiades image
  // Band order in PHR products seems to be always the same : RGB => keep the flag off
  bool realProduct = false;
  if (realProduct)
    {
    if (imageKeywordlist.HasKey(key) && (nbBands > 1))
      {
      std::string keywordStringBandNameList = imageKeywordlist.GetMetadataByKey(key);
      std::vector<std::string> bandNameList;
      boost::trim(keywordStringBandNameList);
      boost::split(bandNameList, keywordStringBandNameList, boost::is_any_of(" "));

      for (int i = 0; i < nbBands && i < 3; i++)
        {
        size_t found;
        found = bandNameList[i].find_first_not_of("B");
        rgb[i] = lexical_cast<int> (bandNameList[i].at(found));
        }
      }
    else
      {
      // Default values
      rgb[0] = 2;
      rgb[1] = 1;
      rgb[2] = 0;
      }
    }
  else
    {
    // Default values for simulation product
    rgb[0] = 0;
    rgb[1] = 1;
    rgb[2] = 2;
    }

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
    if (sensorId.find("PHR") != std::string::npos)
      //if (sensorId == "PHR 1A")
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
    else
      {
      itkExceptionMacro(<< "Invalid Pleiades Sensor ID");
      }
    }
  else if (nbBands > 1 && nbBands < 5)
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
