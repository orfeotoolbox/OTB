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

#include "otbSpot6ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbStringUtils.h"

namespace otb
{
using boost::lexical_cast;
using boost::bad_lexical_cast;

Spot6ImageMetadataInterface
::Spot6ImageMetadataInterface()
{
}

bool
Spot6ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("SPOT 6") != std::string::npos || sensorID.find("SPOT 7") != std::string::npos)
    return true;
  else
    return false;
}

std::string
Spot6ImageMetadataInterface::GetInstrument() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetInstrumentIndex() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface::GetSolarIrradiance() const
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

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Irradiance");
    }

  return outputValuesVariableLengthVector;
}


int
Spot6ImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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


Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetPhysicalBias() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Bias");
    }

  return outputValuesVariableLengthVector;
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
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

  if (outputValues.size() > 0)
    {
           for (unsigned int i = 0; i < outputValues.size(); ++i)
              outputValuesVariableLengthVector[i] = outputValues[this->BandIndexToWavelengthPosition(i)];
    }
  else
    {
           itkExceptionMacro("Invalid Physical Bias");
    }

  return outputValuesVariableLengthVector;
}


double
Spot6ImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
Spot6ImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
    wavel.Fill(0.455); // todo
    }
  else if (nbBands > 1 && nbBands < 5) // todo
    {
    wavel.SetSize(4);
    wavel[0] = 0.454;
    wavel[1] = 0.527;
    wavel[2] = 0.624;
    wavel[3] = 0.756;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

Spot6ImageMetadataInterface::VariableLengthVectorType
Spot6ImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
  if (nbBands == 1) // todo
    {
    wavel.SetSize(1);
    wavel.Fill(0.744);
    }
  else if (nbBands > 1 && nbBands < 5) // todo
    {
    wavel.SetSize(4);
    wavel[0] = 0.519;
    wavel[1] = 0.587;
    wavel[2] = 0.694;
    wavel[3] = 0.880;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

// TODO MSD need to update this function
// Comment this part as relative response
// FIXME check if this is coherent with other sensor
unsigned int
Spot6ImageMetadataInterface
::BandIndexToWavelengthPosition(unsigned int i) const // todo
{
  int nbBands = this->GetNumberOfBands();
  //Panchromatic case
  if (nbBands == 1)
    {
    return 0;
    }
  else
    {
    otbMsgDevMacro(<< "Spot6 detected: first file component is red band and third component is blue one");
    if (i == 0) return 2;
    if (i == 2) return 0;
    }

  return i;
}

std::vector<std::string>
Spot6ImageMetadataInterface
::GetEnhancedBandNames() const // todo
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
Spot6ImageMetadataInterface
::GetDefaultDisplay() const // todo
{

  std::vector<unsigned int> rgb(3);
  rgb[0] = 0;
  rgb[1] = 1;
  rgb[2] = 2;
  return rgb;

}

Spot6ImageMetadataInterface::WavelengthSpectralBandVectorType
Spot6ImageMetadataInterface
::GetSpectralSensitivity()  const // todo
{
  //TODO tabulate spectral responses
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::list <std::vector<float> > tmpSpectralBandList;

  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Spot6 Image");
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
    if (sensorId == "SPOT 6" || sensorId == "SPOT 7") // todo: updated values for Spot7
      {
      const float pan[601] =
        {
0.00007493f,
0.00008750f,
0.00009848f,
0.00010843f,
0.00011646f,
0.00012259f,
0.00012552f,
0.00012814f,
0.00013531f,
0.00015014f,
0.00017236f,
0.00019564f,
0.00021235f,
0.00022348f,
0.00022676f,
0.00022299f,
0.00021213f,
0.00020083f,
0.00019926f,
0.00021359f,
0.00025083f,
0.00030983f,
0.00038670f,
0.00047229f,
0.00056950f,
0.00067560f,
0.00078669f,
0.00090069f,
0.00103421f,
0.00131540f,
0.00169029f,
0.00228198f,
0.00312648f,
0.00417071f,
0.00533582f,
0.00661006f,
0.00802817f,
0.00960467f,
0.01132980f,
0.01334611f,
0.01609231f,
0.02038299f,
0.02732543f,
0.03802347f,
0.05294767f,
0.07167096f,
0.09348728f,
0.11872659f,
0.14830802f,
0.18269957f,
0.22269040f,
0.26906796f,
0.32263521f,
0.38325366f,
0.44824011f,
0.51559417f,
0.58258589f,
0.64645021f,
0.70236661f,
0.74583583f,
0.77585237f,
0.79433792f,
0.80499388f,
0.81090952f,
0.81371638f,
0.81431418f,
0.81316313f,
0.81141755f,
0.81097193f,
0.81322836f,
0.81878063f,
0.82800769f,
0.83916423f,
0.85084085f,
0.86161004f,
0.87040692f,
0.87637562f,
0.87967534f,
0.88047879f,
0.88112137f,
0.88175192f,
0.88365397f,
0.88759508f,
0.89291809f,
0.89912245f,
0.90543957f,
0.91146456f,
0.91547923f,
0.91843965f,
0.91963891f,
0.91975983f,
0.91963913f,
0.91947516f,
0.92025788f,
0.92260193f,
0.92575482f,
0.93070662f,
0.93550391f,
0.94001389f,
0.94383039f,
0.94609629f,
0.94688618f,
0.94624211f,
0.94455744f,
0.94265988f,
0.94120799f,
0.94079078f,
0.94176327f,
0.94473408f,
0.94915584f,
0.95419339f,
0.95954236f,
0.96459688f,
0.96902841f,
0.97179112f,
0.97317857f,
0.97320070f,
0.97218736f,
0.97091309f,
0.97035732f,
0.96996616f,
0.97140258f,
0.97376942f,
0.97636229f,
0.97957895f,
0.98311659f,
0.98550114f,
0.98688700f,
0.98705275f,
0.98569258f,
0.98301588f,
0.97915984f,
0.97493690f,
0.97129138f,
0.96813872f,
0.96649466f,
0.96685400f,
0.96849224f,
0.97201807f,
0.97702694f,
0.98197923f,
0.98753229f,
0.99230951f,
0.99540351f,
0.99740084f,
0.99730413f,
0.99558763f,
0.99262103f,
0.98803927f,
0.98368879f,
0.97966358f,
0.97601679f,
0.97409076f,
0.97356455f,
0.97460306f,
0.97733165f,
0.98142794f,
0.98607299f,
0.99104944f,
0.99556304f,
0.99892911f,
1.00128281f,
1.00206838f,
1.00124192f,
0.99879621f,
0.99466280f,
0.99032401f,
0.98568114f,
0.98102690f,
0.97756954f,
0.97495949f,
0.97366152f,
0.97436348f,
0.97596613f,
0.97930722f,
0.98307748f,
0.98726114f,
0.99154671f,
0.99533904f,
0.99803299f,
0.99950550f,
1.00000000f,
0.99848218f,
0.99593416f,
0.99288307f,
0.98834389f,
0.98366197f,
0.97971238f,
0.97601117f,
0.97307044f,
0.97155305f,
0.97102663f,
0.97152475f,
0.97341779f,
0.97585074f,
0.97896590f,
0.98255796f,
0.98543794f,
0.98789503f,
0.98994151f,
0.99059899f,
0.99029895f,
0.98922305f,
0.98676544f,
0.98389430f,
0.98067297f,
0.97675508f,
0.97324400f,
0.97055473f,
0.96803100f,
0.96629732f,
0.96561899f,
0.96564173f,
0.96639591f,
0.96771777f,
0.96934976f,
0.97124886f,
0.97291816f,
0.97432735f,
0.97530655f,
0.97542227f,
0.97463623f,
0.97308990f,
0.97065297f,
0.96750332f,
0.96379792f,
0.95963369f,
0.95522586f,
0.95099476f,
0.94711647f,
0.94373897f,
0.94086191f,
0.93909609f,
0.93751312f,
0.93716161f,
0.93749275f,
0.93844552f,
0.93992284f,
0.94157292f,
0.94327292f,
0.94494878f,
0.94621266f,
0.94689150f,
0.94695971f,
0.94601526f,
0.94485162f,
0.94216490f,
0.93936780f,
0.93516770f,
0.93117984f,
0.92600825f,
0.92184558f,
0.91679022f,
0.91281220f,
0.90873714f,
0.90559995f,
0.90280144f,
0.90151379f,
0.90018969f,
0.90026306f,
0.90055224f,
0.90169504f,
0.90303974f,
0.90479165f,
0.90660298f,
0.90788333f,
0.90928294f,
0.90961910f,
0.91044105f,
0.90941307f,
0.90884628f,
0.90648792f,
0.90426184f,
0.90090015f,
0.89728707f,
0.89360311f,
0.88921759f,
0.88536863f,
0.88109823f,
0.87784434f,
0.87367129f,
0.87100044f,
0.86849439f,
0.86645265f,
0.86555715f,
0.86486409f,
0.86444275f,
0.86462554f,
0.86421244f,
0.86509991f,
0.86504142f,
0.86574075f,
0.86547448f,
0.86537728f,
0.86481040f,
0.86329140f,
0.86242059f,
0.85986818f,
0.85807444f,
0.85502168f,
0.85224705f,
0.84852762f,
0.84517422f,
0.84157685f,
0.83807810f,
0.83443274f,
0.83120263f,
0.82819085f,
0.82520041f,
0.82259510f,
0.82050631f,
0.81863236f,
0.81673157f,
0.81518962f,
0.81376687f,
0.81276571f,
0.81111987f,
0.81039972f,
0.80884166f,
0.80742716f,
0.80605476f,
0.80328088f,
0.80094601f,
0.79797843f,
0.79448406f,
0.79079478f,
0.78664955f,
0.78403080f,
0.77837404f,
0.77671056f,
0.77071265f,
0.76860809f,
0.76568459f,
0.76096821f,
0.75943556f,
0.75440145f,
0.74853854f,
0.73756232f,
0.72050194f,
0.69619742f,
0.66466129f,
0.62570237f,
0.58042003f,
0.52946690f,
0.47429123f,
0.41572304f,
0.35577781f,
0.29694274f,
0.24182875f,
0.19245049f,
0.15026362f,
0.11482153f,
0.08854978f,
0.06698482f,
0.05109390f,
0.03916000f,
0.03018623f,
0.02342899f,
0.01832614f,
0.01444326f,
0.01147024f,
0.00918513f,
0.00740254f,
0.00609562f,
0.00491959f,
0.00435297f,
0.00335678f,
0.00291823f,
0.00234224f,
0.00205113f,
0.00167084f,
0.00145104f,
0.00124552f,
0.00104170f,
0.00108470f,
0.00077931f,
0.00084497f,
0.00059188f,
0.00063835f,
0.00045747f,
0.00040248f,
0.00043271f,
0.00031792f,
0.00033871f,
0.00025452f,
0.00026931f,
0.00020572f,
0.00018559f,
0.00019577f,
0.00015320f,
0.00016086f,
0.00012704f,
0.00011590f,
0.00012165f,
0.00009829f,
0.00010178f,
0.00008372f,
0.00007722f,
0.00008098f,
0.00007448f,
0.00006262f,
0.00006513f,
0.00005514f,
0.00005663f,
0.00004857f,
0.00004996f,
0.00004369f,
0.00004402f,
0.00003879f,
0.00003626f,
0.00003796f,
0.00003303f,
0.00003435f,
0.00003053f,
0.00003118f,
0.00002826f,
0.00002628f,
0.00002754f,
0.00002484f,
0.00002559f,
0.00002406f,
0.00002348f,
0.00002237f,
0.00002233f,
0.00002269f,
0.00002170f,
0.00001996f,
0.00002062f,
0.00001880f,
0.00002003f,
0.00001893f,
0.00001935f,
0.00001823f,
0.00001794f,
0.00001879f,
0.00001726f,
0.00001797f,
0.00001774f,
0.00001520f,
0.00001784f,
0.00001500f,
0.00001790f,
0.00001464f,
0.00001317f,
0.00001633f,
0.00001127f,
0.00001471f,
0.00001314f,
0.00001114f,
0.00000968f,
0.00001262f,
0.00001126f,
0.00002112f,
0.00001135f,
0.00001674f,
0.00001146f,
0.00001572f,
0.00002157f,
0.00001166f,
0.00002111f,
0.00001206f,
0.00002362f,
0.00002815f,
0.00001740f,
0.00001816f,
0.00002519f,
0.00001627f,
0.00002405f,
0.00002027f,
0.00002469f,
0.00002124f,
0.00001758f,
0.00002971f,
0.00002051f,
0.00002437f,
0.00002119f,
0.00001926f,
0.00001850f,
0.00001774f,
0.00002056f,
0.00002202f,
0.00001602f,
0.00001436f,
0.00001710f,
0.00001369f,
0.00001827f,
0.00001218f,
0.00001567f,
0.00001596f,
0.00001348f,
0.00001857f,
0.00001662f,
0.00001805f,
0.00002361f,
0.00001927f,
0.00003019f,
0.00003036f,
0.00003602f,
0.00003469f,
0.00004530f,
0.00004918f,
0.00005892f,
0.00005708f,
0.00006488f,
0.00006319f,
0.00007284f,
0.00006971f,
0.00007822f,
0.00007143f,
0.00007223f,
0.00007024f,
0.00007097f,
0.00007120f,
0.00006619f,
0.00006294f,
0.00005778f,
0.00005333f,
0.00005405f,
0.00004915f,
0.00004779f,
0.00004379f,
0.00004087f,
0.00003311f,
0.00003608f,
0.00003592f,
0.00002213f,
0.00002159f,
0.00002350f,
0.00001705f,
0.00002055f,
0.00001764f,
0.00001453f,
0.00001636f,
0.00001197f,
0.00001123f,
0.00001127f,
0.00001042f,
0.00001021f,
0.00001099f,
0.00001171f,
0.00001030f,
0.00001236f,
0.00001192f,
0.00000823f,
0.00000820f,
0.00000893f,
0.00001115f,
0.00001001f,
0.00000802f,
0.00000976f,
0.00000966f,
0.00000957f,
0.00000726f,
0.00000967f,
0.00001001f,
0.00000803f,
0.00000726f,
0.00000739f,
0.00000845f,
0.00000698f,
0.00000568f,
0.00000472f,
0.00000650f,
0.00000706f,
0.00000546f,
0.00000433f,
0.00000607f,
0.00000408f,
0.00000788f,
0.00000464f,
0.00000958f,
0.00000689f,
0.00000976f,
0.00000672f,
0.00000574f,
0.00000980f,
0.00000503f,
0.00000796f,
0.00000325f,
0.00000636f,
0.00000334f,
0.00000562f,
0.00000413f,
0.00000400f,
0.00000610f,
0.00000240f,
0.00000486f,
0.00000262f,
0.00000333f,
0.00000152f,
0.00000236f,
0.00000152f,
0.00000228f,
0.00000070f,
0.00000185f,
0.00000162f,
0.00000093f,
0.00000188f,
0.00000116f,
0.00000041f,
0.00000126f
        };
      //add panchromatic band to the temporary list
      const std::vector<float> vpan (pan, pan + sizeof(pan) / sizeof(float) );
      tmpSpectralBandList.push_back(vpan);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Spot6 Sensor ID");
      }
    }
  else if (nbBands > 1 && nbBands < 5)
    {
    if (sensorId == "SPOT 6" || sensorId == "SPOT 7") // todo: updated values for Spot7
      {
      //band B0 (blue band)
      const float b0[601] =
        {
0.00006627f,
0.00006114f,
0.00005964f,
0.00006755f,
0.00007354f,
0.00007335f,
0.00007303f,
0.00006988f,
0.00006669f,
0.00006467f,
0.00008891f,
0.00010731f,
0.00006420f,
0.00008781f,
0.00008978f,
0.00008292f,
0.00008193f,
0.00005499f,
0.00008203f,
0.00011262f,
0.00009092f,
0.00010575f,
0.00013926f,
0.00016613f,
0.00019666f,
0.00019375f,
0.00021841f,
0.00033008f,
0.00059283f,
0.00096710f,
0.00120426f,
0.00143132f,
0.00190776f,
0.00246548f,
0.00314995f,
0.00393858f,
0.00502501f,
0.00693758f,
0.00995331f,
0.01395395f,
0.01816332f,
0.02248784f,
0.02806060f,
0.03582005f,
0.04774668f,
0.06795621f,
0.09842750f,
0.13108997f,
0.16221496f,
0.20486488f,
0.26488470f,
0.33784410f,
0.41558626f,
0.48567541f,
0.54882896f,
0.61466150f,
0.68475833f,
0.75228611f,
0.80736646f,
0.84246227f,
0.85754473f,
0.85841741f,
0.85407547f,
0.85129313f,
0.85144931f,
0.85321674f,
0.85405128f,
0.85387063f,
0.85555834f,
0.86252514f,
0.87558536f,
0.89251130f,
0.90900546f,
0.92154349f,
0.92918777f,
0.93306769f,
0.93515923f,
0.93666027f,
0.93739349f,
0.93661481f,
0.93379608f,
0.92929650f,
0.92442581f,
0.92058032f,
0.91895381f,
0.92090421f,
0.92631122f,
0.93455782f,
0.94463030f,
0.95473194f,
0.96364502f,
0.97043568f,
0.97436437f,
0.97495098f,
0.97200508f,
0.96636029f,
0.95962307f,
0.95371958f,
0.95061152f,
0.95192705f,
0.95818895f,
0.96856416f,
0.98070274f,
0.99142860f,
0.99788650f,
1.00000000f,
0.99910819f,
0.99808156f,
0.99650360f,
0.99314712f,
0.98404465f,
0.96550845f,
0.93867401f,
0.90804427f,
0.87480177f,
0.83387151f,
0.77606904f,
0.69859059f,
0.60871934f,
0.51759429f,
0.43125686f,
0.34977809f,
0.27392459f,
0.20473488f,
0.14738691f,
0.10459452f,
0.07555179f,
0.05654570f,
0.04380857f,
0.03477386f,
0.02793581f,
0.02225302f,
0.01751704f,
0.01359022f,
0.01046026f,
0.00814691f,
0.00649078f,
0.00530930f,
0.00446096f,
0.00383097f,
0.00333164f,
0.00292664f,
0.00257637f,
0.00225949f,
0.00198202f,
0.00172287f,
0.00150439f,
0.00130359f,
0.00113903f,
0.00099076f,
0.00086925f,
0.00076142f,
0.00067853f,
0.00060841f,
0.00055953f,
0.00051880f,
0.00049699f,
0.00047445f,
0.00045863f,
0.00044229f,
0.00042470f,
0.00040647f,
0.00038780f,
0.00035963f,
0.00033330f,
0.00029660f,
0.00027179f,
0.00024451f,
0.00022875f,
0.00021524f,
0.00021255f,
0.00021004f,
0.00021079f,
0.00021138f,
0.00021521f,
0.00021336f,
0.00021459f,
0.00020993f,
0.00020747f,
0.00019888f,
0.00019065f,
0.00018249f,
0.00017164f,
0.00016473f,
0.00015826f,
0.00015437f,
0.00015378f,
0.00015002f,
0.00015089f,
0.00015050f,
0.00015245f,
0.00015136f,
0.00015264f,
0.00015147f,
0.00015009f,
0.00014792f,
0.00014493f,
0.00014083f,
0.00013665f,
0.00013056f,
0.00012700f,
0.00012212f,
0.00011620f,
0.00011384f,
0.00010918f,
0.00010782f,
0.00010377f,
0.00010231f,
0.00010126f,
0.00010021f,
0.00009925f,
0.00009923f,
0.00009906f,
0.00009850f,
0.00009892f,
0.00009742f,
0.00009802f,
0.00009809f,
0.00009662f,
0.00009692f,
0.00009575f,
0.00009546f,
0.00009575f,
0.00009469f,
0.00009485f,
0.00009453f,
0.00009384f,
0.00009463f,
0.00009451f,
0.00009458f,
0.00009520f,
0.00009478f,
0.00009588f,
0.00009579f,
0.00009545f,
0.00009560f,
0.00009549f,
0.00009549f,
0.00009551f,
0.00009535f,
0.00009579f,
0.00009570f,
0.00009544f,
0.00009552f,
0.00009622f,
0.00009645f,
0.00009648f,
0.00009799f,
0.00009797f,
0.00009864f,
0.00009916f,
0.00010183f,
0.00010153f,
0.00010358f,
0.00010461f,
0.00010712f,
0.00010718f,
0.00011085f,
0.00011321f,
0.00011173f,
0.00011398f,
0.00011982f,
0.00011831f,
0.00012135f,
0.00012902f,
0.00012720f,
0.00013658f,
0.00013538f,
0.00014397f,
0.00014356f,
0.00015119f,
0.00015202f,
0.00015693f,
0.00015686f,
0.00015130f,
0.00015184f,
0.00013690f,
0.00013092f,
0.00013335f,
0.00011361f,
0.00010715f,
0.00010823f,
0.00009932f,
0.00009856f,
0.00009765f,
0.00009914f,
0.00010008f,
0.00010777f,
0.00010686f,
0.00011218f,
0.00012769f,
0.00012456f,
0.00014074f,
0.00014680f,
0.00014400f,
0.00015360f,
0.00015552f,
0.00015166f,
0.00015221f,
0.00014625f,
0.00014561f,
0.00013613f,
0.00013172f,
0.00013242f,
0.00012141f,
0.00012300f,
0.00011836f,
0.00010821f,
0.00010974f,
0.00010309f,
0.00010302f,
0.00010127f,
0.00009629f,
0.00009854f,
0.00009280f,
0.00009475f,
0.00009054f,
0.00009196f,
0.00008822f,
0.00008866f,
0.00008744f,
0.00008550f,
0.00008673f,
0.00008483f,
0.00008628f,
0.00008449f,
0.00009254f,
0.00008532f,
0.00009594f,
0.00008917f,
0.00009084f,
0.00010183f,
0.00009128f,
0.00010000f,
0.00008800f,
0.00009430f,
0.00008055f,
0.00008475f,
0.00007796f,
0.00007850f,
0.00007713f,
0.00007770f,
0.00007623f,
0.00007664f,
0.00007474f,
0.00007405f,
0.00007519f,
0.00007318f,
0.00007349f,
0.00007204f,
0.00007375f,
0.00007369f,
0.00007149f,
0.00007148f,
0.00007312f,
0.00007167f,
0.00007255f,
0.00007120f,
0.00007186f,
0.00007087f,
0.00007145f,
0.00007024f,
0.00007096f,
0.00007041f,
0.00007088f,
0.00006995f,
0.00007149f,
0.00006924f,
0.00007103f,
0.00006949f,
0.00007093f,
0.00006951f,
0.00007194f,
0.00007193f,
0.00006859f,
0.00006781f,
0.00007056f,
0.00006741f,
0.00006870f,
0.00006628f,
0.00006687f,
0.00006601f,
0.00006595f,
0.00006670f,
0.00006450f,
0.00006595f,
0.00006433f,
0.00006569f,
0.00006431f,
0.00006573f,
0.00006406f,
0.00006507f,
0.00006298f,
0.00006428f,
0.00006201f,
0.00006357f,
0.00006101f,
0.00006096f,
0.00006309f,
0.00006103f,
0.00006314f,
0.00006382f,
0.00006164f,
0.00006245f,
0.00006061f,
0.00005973f,
0.00006261f,
0.00005982f,
0.00006197f,
0.00005956f,
0.00006099f,
0.00005881f,
0.00005972f,
0.00006005f,
0.00005691f,
0.00005647f,
0.00005823f,
0.00005603f,
0.00005759f,
0.00005605f,
0.00005558f,
0.00005646f,
0.00005480f,
0.00005543f,
0.00005551f,
0.00005357f,
0.00005295f,
0.00005549f,
0.00005286f,
0.00005495f,
0.00005278f,
0.00005414f,
0.00005475f,
0.00005169f,
0.00005307f,
0.00005117f,
0.00005068f,
0.00005036f,
0.00005270f,
0.00004991f,
0.00005177f,
0.00005900f,
0.00004889f,
0.00006116f,
0.00005285f,
0.00006427f,
0.00005589f,
0.00006943f,
0.00006195f,
0.00007219f,
0.00007275f,
0.00006694f,
0.00007232f,
0.00006712f,
0.00007044f,
0.00006607f,
0.00007040f,
0.00007120f,
0.00006047f,
0.00006863f,
0.00006547f,
0.00005919f,
0.00006852f,
0.00005996f,
0.00006589f,
0.00006938f,
0.00006013f,
0.00007365f,
0.00006702f,
0.00007595f,
0.00008089f,
0.00006987f,
0.00008784f,
0.00007751f,
0.00009504f,
0.00010978f,
0.00008984f,
0.00012777f,
0.00010534f,
0.00014784f,
0.00012341f,
0.00017455f,
0.00018601f,
0.00015342f,
0.00020484f,
0.00017984f,
0.00018931f,
0.00022557f,
0.00022780f,
0.00021393f,
0.00022939f,
0.00023016f,
0.00021920f,
0.00021508f,
0.00022046f,
0.00020159f,
0.00021016f,
0.00018988f,
0.00019842f,
0.00017117f,
0.00018205f,
0.00015240f,
0.00014443f,
0.00015946f,
0.00014629f,
0.00012207f,
0.00013127f,
0.00010545f,
0.00011422f,
0.00009446f,
0.00009843f,
0.00008316f,
0.00008914f,
0.00008435f,
0.00006511f,
0.00007269f,
0.00006160f,
0.00006279f,
0.00005814f,
0.00005882f,
0.00005406f,
0.00005351f,
0.00005183f,
0.00005084f,
0.00004786f,
0.00004877f,
0.00004859f,
0.00004583f,
0.00004279f,
0.00004263f,
0.00004130f,
0.00004093f,
0.00004059f,
0.00004012f,
0.00004037f,
0.00004396f,
0.00004304f,
0.00004054f,
0.00004200f,
0.00004073f,
0.00004199f,
0.00004077f,
0.00004125f,
0.00003820f,
0.00003723f,
0.00003610f,
0.00003503f,
0.00003232f,
0.00003214f,
0.00003263f,
0.00003065f,
0.00003127f,
0.00002955f,
0.00002956f,
0.00002840f,
0.00002896f,
0.00002780f,
0.00002805f,
0.00002680f,
0.00002585f,
0.00002704f,
0.00002540f,
0.00002576f,
0.00002438f,
0.00002409f,
0.00002110f,
0.00002288f,
0.00001930f,
0.00002147f,
0.00001830f,
0.00001907f,
0.00001917f,
0.00001634f,
0.00001778f,
0.00001705f,
0.00001506f,
0.00001555f,
0.00001544f,
0.00001376f,
0.00001432f,
0.00001256f,
0.00001321f,
0.00001143f,
0.00001207f,
0.00001040f,
0.00001079f,
0.00001077f,
0.00000938f,
0.00000992f,
0.00000926f,
0.00000875f,
0.00000915f,
0.00000791f,
0.00000858f,
0.00000757f,
0.00000885f,
0.00000740f,
0.00000859f,
0.00000871f,
0.00000732f,
0.00000711f
        };
      //B1 green band
      const float b1[601] =
        {
0.00001010f,
0.00001389f,
0.00001902f,
0.00001356f,
0.00001327f,
0.00002873f,
0.00003122f,
0.00002184f,
0.00002860f,
0.00002274f,
0.00006723f,
0.00008343f,
0.00001226f,
0.00006171f,
0.00005563f,
0.00004426f,
0.00004539f,
0.00000677f,
0.00002035f,
0.00001522f,
0.00000528f,
0.00001865f,
0.00001834f,
0.00002028f,
0.00001608f,
0.00000017f,
0.00000057f,
0.00001313f,
0.00001658f,
0.00000224f,
0.00001983f,
0.00001885f,
0.00001859f,
0.00001975f,
0.00000023f,
0.00000139f,
0.00001634f,
0.00001789f,
0.00000616f,
0.00002267f,
0.00001903f,
0.00001956f,
0.00001844f,
0.00000342f,
0.00001448f,
0.00001340f,
0.00000774f,
0.00001880f,
0.00001632f,
0.00001912f,
0.00001915f,
0.00000950f,
0.00000865f,
0.00000927f,
0.00001089f,
0.00001236f,
0.00001349f,
0.00001273f,
0.00001062f,
0.00000958f,
0.00000938f,
0.00000973f,
0.00000998f,
0.00001014f,
0.00001042f,
0.00001060f,
0.00001115f,
0.00001174f,
0.00001254f,
0.00001338f,
0.00001400f,
0.00001431f,
0.00001481f,
0.00001528f,
0.00001646f,
0.00001812f,
0.00001991f,
0.00002232f,
0.00002548f,
0.00002855f,
0.00003235f,
0.00003705f,
0.00004200f,
0.00004766f,
0.00005414f,
0.00006070f,
0.00006821f,
0.00007648f,
0.00008464f,
0.00009410f,
0.00010524f,
0.00011803f,
0.00013402f,
0.00015319f,
0.00017614f,
0.00020243f,
0.00023094f,
0.00026260f,
0.00029673f,
0.00033499f,
0.00038048f,
0.00043594f,
0.00050952f,
0.00061362f,
0.00076395f,
0.00097908f,
0.00126923f,
0.00163382f,
0.00207590f,
0.00261314f,
0.00327921f,
0.00413303f,
0.00526369f,
0.00684627f,
0.00915649f,
0.01267648f,
0.01800685f,
0.02585007f,
0.03667779f,
0.05118064f,
0.07035894f,
0.09617429f,
0.13094754f,
0.17727190f,
0.23641290f,
0.30751157f,
0.38790047f,
0.47436827f,
0.56468213f,
0.65643377f,
0.74674718f,
0.82841738f,
0.89713046f,
0.94858047f,
0.98476388f,
1.00572477f,
1.01799762f,
1.02241761f,
1.02351216f,
1.02270186f,
1.02115905f,
1.01965328f,
1.01731773f,
1.01540955f,
1.01129946f,
1.00955329f,
1.00573235f,
1.00407549f,
1.00164097f,
1.00130077f,
0.99962912f,
1.00130006f,
1.00140898f,
1.00384853f,
1.00599723f,
1.00910590f,
1.01051975f,
1.01099594f,
1.00890543f,
1.00483865f,
0.99901144f,
0.99220799f,
0.98468953f,
0.97823873f,
0.97314859f,
0.97022501f,
0.96868569f,
0.97004107f,
0.97166154f,
0.97626613f,
0.98067188f,
0.98683373f,
0.99219054f,
0.99666299f,
1.00000000f,
0.99912543f,
0.99696320f,
0.99036225f,
0.98384547f,
0.97519907f,
0.96391617f,
0.94573906f,
0.91096960f,
0.85904551f,
0.79290657f,
0.72361833f,
0.65257621f,
0.57623116f,
0.48870213f,
0.39405229f,
0.29840580f,
0.21347365f,
0.14739732f,
0.10143634f,
0.07106678f,
0.05101665f,
0.03753145f,
0.02803887f,
0.02107987f,
0.01582745f,
0.01178705f,
0.00868732f,
0.00634606f,
0.00461992f,
0.00337907f,
0.00250694f,
0.00189987f,
0.00147499f,
0.00117574f,
0.00096470f,
0.00080813f,
0.00069267f,
0.00060187f,
0.00053101f,
0.00046977f,
0.00041864f,
0.00037515f,
0.00033197f,
0.00029595f,
0.00026098f,
0.00022974f,
0.00020299f,
0.00017878f,
0.00015870f,
0.00014470f,
0.00012912f,
0.00012281f,
0.00011148f,
0.00010580f,
0.00010508f,
0.00009942f,
0.00009982f,
0.00009664f,
0.00009667f,
0.00009589f,
0.00009628f,
0.00009600f,
0.00009459f,
0.00009484f,
0.00009162f,
0.00009246f,
0.00008883f,
0.00008616f,
0.00008688f,
0.00008318f,
0.00008295f,
0.00008153f,
0.00008262f,
0.00008265f,
0.00008855f,
0.00009224f,
0.00008974f,
0.00010120f,
0.00010537f,
0.00010299f,
0.00011271f,
0.00011213f,
0.00011810f,
0.00011639f,
0.00011824f,
0.00011152f,
0.00011302f,
0.00010361f,
0.00010441f,
0.00009482f,
0.00009519f,
0.00008675f,
0.00008735f,
0.00008078f,
0.00008067f,
0.00007509f,
0.00007486f,
0.00006895f,
0.00006944f,
0.00006375f,
0.00006358f,
0.00005878f,
0.00005896f,
0.00005523f,
0.00005501f,
0.00005343f,
0.00005281f,
0.00005140f,
0.00005170f,
0.00005134f,
0.00005138f,
0.00005107f,
0.00005085f,
0.00005096f,
0.00005018f,
0.00005046f,
0.00004904f,
0.00004833f,
0.00004769f,
0.00004781f,
0.00004694f,
0.00004600f,
0.00004584f,
0.00004562f,
0.00004584f,
0.00004525f,
0.00004562f,
0.00004612f,
0.00004747f,
0.00004854f,
0.00004734f,
0.00004932f,
0.00004894f,
0.00005047f,
0.00004972f,
0.00005030f,
0.00005129f,
0.00004999f,
0.00005048f,
0.00004864f,
0.00004901f,
0.00004923f,
0.00004719f,
0.00004950f,
0.00004776f,
0.00005348f,
0.00005702f,
0.00005214f,
0.00006320f,
0.00005556f,
0.00007144f,
0.00005903f,
0.00005649f,
0.00007807f,
0.00004800f,
0.00004042f,
0.00005618f,
0.00002571f,
0.00003251f,
0.00002175f,
0.00000592f,
0.00001077f,
0.00000286f,
0.00000380f,
0.00000323f,
0.00000149f,
0.00000247f,
0.00000112f,
0.00000145f,
0.00000111f,
0.00000139f,
0.00000105f,
0.00000141f,
0.00000097f,
0.00000085f,
0.00000123f,
0.00000158f,
0.00000089f,
0.00000137f,
0.00000086f,
0.00000158f,
0.00000202f,
0.00000091f,
0.00000174f,
0.00000102f,
0.00000200f,
0.00000128f,
0.00000479f,
0.00000149f,
0.00000719f,
0.00000417f,
0.00000502f,
0.00000963f,
0.00000561f,
0.00000984f,
0.00000949f,
0.00000318f,
0.00000643f,
0.00000153f,
0.00000184f,
0.00000116f,
0.00000173f,
0.00000086f,
0.00000185f,
0.00000089f,
0.00000193f,
0.00000096f,
0.00000196f,
0.00000116f,
0.00000200f,
0.00000115f,
0.00000195f,
0.00000131f,
0.00000197f,
0.00000125f,
0.00000187f,
0.00000099f,
0.00000070f,
0.00000049f,
0.00000119f,
0.00000051f,
0.00000135f,
0.00000086f,
0.00000153f,
0.00000104f,
0.00000186f,
0.00000156f,
0.00000205f,
0.00000293f,
0.00000170f,
0.00000296f,
0.00000182f,
0.00000266f,
0.00000171f,
0.00000236f,
0.00000240f,
0.00000093f,
0.00000174f,
0.00000183f,
0.00000096f,
0.00000144f,
0.00000177f,
0.00000076f,
0.00000198f,
0.00000079f,
0.00000160f,
0.00000058f,
0.00000154f,
0.00000070f,
0.00000161f,
0.00000076f,
0.00000068f,
0.00000245f,
0.00000079f,
0.00000299f,
0.00000128f,
0.00000285f,
0.00000154f,
0.00000296f,
0.00000178f,
0.00000333f,
0.00000130f,
0.00000347f,
0.00000220f,
0.00000159f,
0.00000449f,
0.00000188f,
0.00000635f,
0.00000903f,
0.00000344f,
0.00001051f,
0.00000601f,
0.00000991f,
0.00001299f,
0.00000632f,
0.00001514f,
0.00000941f,
0.00001426f,
0.00000749f,
0.00001524f,
0.00000887f,
0.00000743f,
0.00001404f,
0.00000558f,
0.00001092f,
0.00001135f,
0.00000460f,
0.00001024f,
0.00000547f,
0.00001036f,
0.00000503f,
0.00001140f,
0.00000557f,
0.00001124f,
0.00000612f,
0.00001120f,
0.00000642f,
0.00001093f,
0.00000638f,
0.00001191f,
0.00000531f,
0.00000907f,
0.00000487f,
0.00000794f,
0.00000998f,
0.00000316f,
0.00000322f,
0.00001260f,
0.00000365f,
0.00000446f,
0.00001209f,
0.00000567f,
0.00001173f,
0.00000705f,
0.00001006f,
0.00000466f,
0.00000325f,
0.00000921f,
0.00000755f,
0.00000493f,
0.00000922f,
0.00000673f,
0.00001643f,
0.00001135f,
0.00001160f,
0.00002335f,
0.00001573f,
0.00002364f,
0.00001931f,
0.00002613f,
0.00002318f,
0.00001698f,
0.00001954f,
0.00001127f,
0.00001395f,
0.00001618f,
0.00000862f,
0.00001383f,
0.00000962f,
0.00000696f,
0.00000696f,
0.00000561f,
0.00000807f,
0.00000905f,
0.00000632f,
0.00000850f,
0.00000820f,
0.00001222f,
0.00000921f,
0.00001027f,
0.00001204f,
0.00001323f,
0.00001373f,
0.00001579f,
0.00001521f,
0.00002156f,
0.00001894f,
0.00002375f,
0.00002243f,
0.00002409f,
0.00002019f,
0.00002182f,
0.00001452f,
0.00001755f,
0.00001475f,
0.00000805f,
0.00001071f,
0.00000537f,
0.00000682f,
0.00000663f,
0.00000526f,
0.00000418f,
0.00000477f,
0.00000426f,
0.00000405f,
0.00000367f,
0.00000399f,
0.00000340f,
0.00000339f,
0.00000169f,
0.00000178f,
0.00000242f,
0.00000171f,
0.00000172f,
0.00000233f,
0.00000182f,
0.00000230f,
0.00000204f,
0.00000245f,
0.00000243f,
0.00000136f,
0.00000259f,
0.00000105f,
0.00000239f,
0.00000090f,
0.00000213f,
0.00000104f,
0.00000226f,
0.00000088f,
0.00000229f,
0.00000065f,
0.00000246f,
0.00000063f,
0.00000238f,
0.00000072f,
0.00000210f,
0.00000091f,
0.00000122f,
0.00000184f,
0.00000177f,
0.00000066f,
0.00000153f,
0.00000065f,
0.00000147f,
0.00000054f,
0.00000145f,
0.00000023f,
0.00000134f,
0.00000029f,
0.00000141f,
0.00000045f,
0.00000127f,
0.00000146f,
0.00000040f,
0.00000142f,
0.00000038f
        };
      //B2 red band
      const float b2[601] =
        {
0.00001168f,
0.00001419f,
0.00001348f,
0.00000996f,
0.00001009f,
0.00001366f,
0.00003289f,
0.00001748f,
0.00002207f,
0.00002377f,
0.00002383f,
0.00002470f,
0.00001811f,
0.00001353f,
0.00001155f,
0.00001303f,
0.00001374f,
0.00001126f,
0.00000712f,
0.00000464f,
0.00000438f,
0.00000381f,
0.00000312f,
0.00000581f,
0.00000319f,
0.00000473f,
0.00000585f,
0.00000644f,
0.00000967f,
0.00000918f,
0.00000742f,
0.00001090f,
0.00001649f,
0.00002016f,
0.00001631f,
0.00001673f,
0.00001578f,
0.00001766f,
0.00002209f,
0.00003114f,
0.00003091f,
0.00001848f,
0.00003451f,
0.00003492f,
0.00004332f,
0.00004770f,
0.00004658f,
0.00004547f,
0.00004239f,
0.00005155f,
0.00005834f,
0.00006365f,
0.00005824f,
0.00004581f,
0.00003799f,
0.00003584f,
0.00003689f,
0.00003817f,
0.00003951f,
0.00004017f,
0.00003987f,
0.00003754f,
0.00003651f,
0.00003781f,
0.00003645f,
0.00003408f,
0.00003160f,
0.00003251f,
0.00003509f,
0.00003935f,
0.00004494f,
0.00005115f,
0.00005707f,
0.00006145f,
0.00006038f,
0.00005716f,
0.00005354f,
0.00004978f,
0.00004759f,
0.00004735f,
0.00005985f,
0.00006501f,
0.00005803f,
0.00006474f,
0.00006650f,
0.00006908f,
0.00006585f,
0.00005102f,
0.00003616f,
0.00002648f,
0.00002034f,
0.00001645f,
0.00001363f,
0.00001142f,
0.00000950f,
0.00000755f,
0.00000648f,
0.00000560f,
0.00000476f,
0.00000466f,
0.00000444f,
0.00000389f,
0.00000408f,
0.00000399f,
0.00000392f,
0.00000436f,
0.00000413f,
0.00000397f,
0.00000424f,
0.00000400f,
0.00000389f,
0.00000385f,
0.00000390f,
0.00000379f,
0.00000357f,
0.00000383f,
0.00000386f,
0.00000378f,
0.00000439f,
0.00000377f,
0.00000416f,
0.00000494f,
0.00000455f,
0.00000553f,
0.00000605f,
0.00000613f,
0.00000672f,
0.00000696f,
0.00000744f,
0.00000760f,
0.00000766f,
0.00000770f,
0.00000725f,
0.00000682f,
0.00000716f,
0.00000758f,
0.00000759f,
0.00000722f,
0.00000805f,
0.00000826f,
0.00000816f,
0.00000835f,
0.00000798f,
0.00000798f,
0.00000796f,
0.00000817f,
0.00000856f,
0.00000897f,
0.00000920f,
0.00000935f,
0.00000980f,
0.00001003f,
0.00000998f,
0.00001038f,
0.00001044f,
0.00001035f,
0.00001090f,
0.00001143f,
0.00001155f,
0.00001240f,
0.00001322f,
0.00001374f,
0.00001466f,
0.00001507f,
0.00001561f,
0.00001799f,
0.00001806f,
0.00001990f,
0.00002207f,
0.00002400f,
0.00002768f,
0.00003153f,
0.00003519f,
0.00003855f,
0.00004048f,
0.00004339f,
0.00004685f,
0.00004712f,
0.00004885f,
0.00005031f,
0.00005058f,
0.00005380f,
0.00006109f,
0.00006875f,
0.00008360f,
0.00010367f,
0.00013385f,
0.00017220f,
0.00021349f,
0.00025613f,
0.00029745f,
0.00033957f,
0.00038077f,
0.00042014f,
0.00045664f,
0.00049082f,
0.00052462f,
0.00057202f,
0.00064046f,
0.00073788f,
0.00088389f,
0.00109195f,
0.00138377f,
0.00179072f,
0.00234897f,
0.00310035f,
0.00409611f,
0.00539886f,
0.00764596f,
0.00982485f,
0.01250677f,
0.01576553f,
0.01971871f,
0.02460136f,
0.03076162f,
0.03870973f,
0.04932564f,
0.06405112f,
0.08527458f,
0.11683854f,
0.16336997f,
0.22811553f,
0.30778033f,
0.39105868f,
0.46939880f,
0.54633346f,
0.62987089f,
0.71765372f,
0.80227884f,
0.87400338f,
0.92669327f,
0.95728386f,
0.97246242f,
0.98093913f,
0.98815314f,
0.99512479f,
1.00000000f,
1.00071278f,
0.99509151f,
0.98322813f,
0.96618038f,
0.94726377f,
0.92911907f,
0.91427200f,
0.90407257f,
0.89868685f,
0.89729260f,
0.90048309f,
0.90619876f,
0.91486402f,
0.92548194f,
0.93716217f,
0.95109280f,
0.96341044f,
0.97756310f,
0.98873468f,
1.00002059f,
1.00784176f,
1.01361246f,
1.01518655f,
1.01365106f,
1.00889905f,
1.00094470f,
0.99098939f,
0.97907661f,
0.96736515f,
0.95491923f,
0.94403019f,
0.93408742f,
0.92588661f,
0.91979557f,
0.91551236f,
0.91345541f,
0.91406966f,
0.91586446f,
0.92112352f,
0.92637606f,
0.93343259f,
0.93977000f,
0.94511856f,
0.94655018f,
0.94482431f,
0.93695460f,
0.92470622f,
0.90515848f,
0.87651239f,
0.84722733f,
0.81626058f,
0.77926628f,
0.74095072f,
0.69853161f,
0.65530863f,
0.60957078f,
0.56408506f,
0.51760530f,
0.47150814f,
0.42482252f,
0.37706746f,
0.32909071f,
0.28093192f,
0.23513938f,
0.19289082f,
0.15584587f,
0.12438828f,
0.09857760f,
0.07759031f,
0.06074302f,
0.04708451f,
0.03618668f,
0.02749464f,
0.02064713f,
0.01533676f,
0.01131374f,
0.00832392f,
0.00613477f,
0.00454626f,
0.00340798f,
0.00258536f,
0.00199356f,
0.00156305f,
0.00125005f,
0.00101327f,
0.00085049f,
0.00071368f,
0.00062520f,
0.00054959f,
0.00049985f,
0.00047162f,
0.00043891f,
0.00042848f,
0.00040347f,
0.00038227f,
0.00035692f,
0.00033769f,
0.00030789f,
0.00029814f,
0.00024879f,
0.00021170f,
0.00018471f,
0.00013393f,
0.00012509f,
0.00007240f,
0.00006419f,
0.00003891f,
0.00003294f,
0.00002353f,
0.00002711f,
0.00001690f,
0.00001474f,
0.00001598f,
0.00001292f,
0.00001366f,
0.00001329f,
0.00001411f,
0.00002354f,
0.00009563f,
0.00012882f,
0.00011528f,
0.00001687f,
0.00015343f,
0.00012777f,
0.00015354f,
0.00024298f,
0.00015520f,
0.00024274f,
0.00012696f,
0.00014910f,
0.00000979f,
0.00000629f,
0.00000738f,
0.00000483f,
0.00000489f,
0.00000536f,
0.00000458f,
0.00000528f,
0.00000438f,
0.00000475f,
0.00000446f,
0.00000488f,
0.00000456f,
0.00000547f,
0.00000448f,
0.00000478f,
0.00000435f,
0.00000470f,
0.00000425f,
0.00000458f,
0.00000435f,
0.00000380f,
0.00000373f,
0.00000298f,
0.00000390f,
0.00000278f,
0.00000290f,
0.00000277f,
0.00000307f,
0.00000247f,
0.00000231f,
0.00000301f,
0.00000293f,
0.00000319f,
0.00000351f,
0.00000323f,
0.00000339f,
0.00000318f,
0.00000318f,
0.00000396f,
0.00000313f,
0.00000364f,
0.00000310f,
0.00000419f,
0.00000349f,
0.00000396f,
0.00000341f,
0.00000365f,
0.00000362f,
0.00000290f,
0.00000345f,
0.00000289f,
0.00000288f,
0.00000429f,
0.00000323f,
0.00000329f,
0.00000329f,
0.00000455f,
0.00000497f,
0.00000353f,
0.00000556f,
0.00000383f,
0.00000507f,
0.00000482f,
0.00000484f,
0.00000546f,
0.00000374f,
0.00000601f,
0.00000422f,
0.00000512f,
0.00000509f,
0.00000286f,
0.00000637f,
0.00000539f,
0.00000579f,
0.00000462f,
0.00000647f,
0.00000604f,
0.00000432f,
0.00000662f,
0.00000521f,
0.00000680f,
0.00001180f,
0.00000638f,
0.00000805f,
0.00000389f,
0.00001200f,
0.00001134f,
0.00000562f,
0.00001181f,
0.00000868f,
0.00001541f,
0.00001086f,
0.00001222f,
0.00000823f,
0.00000960f,
0.00000871f,
0.00001051f,
0.00001235f,
0.00000864f,
0.00001357f,
0.00001158f,
0.00001522f,
0.00001091f,
0.00001237f,
0.00000928f,
0.00000973f,
0.00000865f,
0.00001333f,
0.00000847f,
0.00001329f,
0.00000740f,
0.00000974f,
0.00001229f,
0.00001128f,
0.00000704f,
0.00001244f,
0.00000915f,
0.00001448f,
0.00001164f,
0.00000861f,
0.00001441f,
0.00000700f,
0.00001461f,
0.00001401f,
0.00001007f,
0.00001695f,
0.00002042f,
0.00002108f,
0.00003064f,
0.00003171f,
0.00004339f,
0.00004171f,
0.00005692f,
0.00007019f,
0.00007121f,
0.00007122f,
0.00008504f,
0.00007450f,
0.00008010f,
0.00008936f,
0.00008826f,
0.00008831f,
0.00009721f,
0.00009019f,
0.00008741f,
0.00008467f,
0.00007373f,
0.00006748f,
0.00006984f,
0.00005307f,
0.00004844f,
0.00005417f,
0.00004482f,
0.00004483f,
0.00003814f,
0.00003524f,
0.00002726f,
0.00002455f,
0.00002566f,
0.00002043f,
0.00001759f,
0.00001473f,
0.00001512f,
0.00001132f,
0.00000862f,
0.00001262f,
0.00000857f,
0.00000918f,
0.00000689f,
0.00000671f,
0.00000434f,
0.00000648f,
0.00000513f,
0.00000748f,
0.00000691f,
0.00000789f,
0.00000754f,
0.00000715f,
0.00000806f,
0.00000701f,
0.00000449f,
0.00000640f,
0.00000595f,
0.00000877f,
0.00000724f,
0.00000837f,
0.00000733f,
0.00000815f,
0.00000621f,
0.00000755f,
0.00000444f,
0.00000580f,
0.00000524f,
0.00000343f,
0.00000356f,
0.00000340f,
0.00000361f,
0.00000245f,
0.00000146f,
0.00000232f,
0.00000165f,
0.00000168f,
0.00000491f,
0.00000168f,
0.00000411f,
0.00000122f,
0.00000367f,
0.00000244f,
0.00000179f,
0.00000395f,
0.00000177f,
0.00000108f,
0.00000330f,
0.00000166f,
0.00000258f,
0.00000204f,
0.00000054f,
0.00000259f,
0.00000133f,
0.00000205f,
0.00000101f,
0.00000210f,
0.00000084f,
0.00000090f,
0.00000176f,
0.00000079f,
0.00000165f,
0.00000071f,
0.00000173f,
0.00000070f,
0.00000043f,
0.00000056f,
0.00000159f
        };
      //B3 nir band
      const float b3[601] =
        {
0.00000184f,
0.00000202f,
0.00000371f,
0.00000470f,
0.00000487f,
0.00000512f,
0.00000521f,
0.00000490f,
0.00000308f,
0.00000160f,
0.00000126f,
0.00000131f,
0.00000119f,
0.00000105f,
0.00000082f,
0.00000078f,
0.00000076f,
0.00000059f,
0.00000058f,
0.00000052f,
0.00000064f,
0.00000070f,
0.00000068f,
0.00000068f,
0.00000079f,
0.00000093f,
0.00000124f,
0.00000092f,
0.00000124f,
0.00000117f,
0.00000125f,
0.00000129f,
0.00000121f,
0.00000153f,
0.00000149f,
0.00000148f,
0.00000139f,
0.00000166f,
0.00000211f,
0.00000193f,
0.00000241f,
0.00000211f,
0.00000228f,
0.00000294f,
0.00000406f,
0.00000507f,
0.00001753f,
0.00062612f,
0.00099041f,
0.00038281f,
0.00084800f,
0.00090343f,
0.00098820f,
0.00100027f,
0.00030612f,
0.00003473f,
0.00002650f,
0.00002396f,
0.00002429f,
0.00002735f,
0.00003712f,
0.00005612f,
0.00007768f,
0.00008914f,
0.00009611f,
0.00010759f,
0.00011246f,
0.00011011f,
0.00009290f,
0.00006854f,
0.00005014f,
0.00003890f,
0.00003142f,
0.00002673f,
0.00002266f,
0.00002061f,
0.00002008f,
0.00002029f,
0.00002207f,
0.00002564f,
0.00003063f,
0.00003885f,
0.00004812f,
0.00005996f,
0.00007077f,
0.00007965f,
0.00008609f,
0.00008841f,
0.00008691f,
0.00008090f,
0.00007026f,
0.00005867f,
0.00004706f,
0.00003684f,
0.00002888f,
0.00002252f,
0.00001864f,
0.00001631f,
0.00001512f,
0.00001579f,
0.00001740f,
0.00002076f,
0.00002500f,
0.00002970f,
0.00003324f,
0.00003568f,
0.00003719f,
0.00003637f,
0.00003522f,
0.00003204f,
0.00002806f,
0.00002554f,
0.00002401f,
0.00002509f,
0.00002676f,
0.00003097f,
0.00003703f,
0.00004613f,
0.00006025f,
0.00007876f,
0.00010325f,
0.00012473f,
0.00014349f,
0.00015902f,
0.00017128f,
0.00017677f,
0.00017814f,
0.00017111f,
0.00016498f,
0.00016271f,
0.00016590f,
0.00017244f,
0.00018189f,
0.00018854f,
0.00019224f,
0.00019405f,
0.00019354f,
0.00018898f,
0.00018087f,
0.00017218f,
0.00016359f,
0.00015899f,
0.00015586f,
0.00015606f,
0.00015819f,
0.00016068f,
0.00016530f,
0.00016816f,
0.00017049f,
0.00017164f,
0.00017169f,
0.00017074f,
0.00017054f,
0.00016913f,
0.00016744f,
0.00016766f,
0.00016951f,
0.00017426f,
0.00018167f,
0.00019232f,
0.00020428f,
0.00021790f,
0.00023244f,
0.00024402f,
0.00025416f,
0.00026158f,
0.00026375f,
0.00026297f,
0.00026655f,
0.00026988f,
0.00027504f,
0.00028070f,
0.00028807f,
0.00030046f,
0.00031707f,
0.00033920f,
0.00036610f,
0.00039570f,
0.00043575f,
0.00048703f,
0.00053825f,
0.00057909f,
0.00060314f,
0.00061674f,
0.00061491f,
0.00059039f,
0.00054258f,
0.00047833f,
0.00039787f,
0.00031987f,
0.00025214f,
0.00019590f,
0.00015376f,
0.00012305f,
0.00010304f,
0.00008730f,
0.00007548f,
0.00006686f,
0.00006139f,
0.00005448f,
0.00005006f,
0.00004759f,
0.00004287f,
0.00003904f,
0.00003667f,
0.00003331f,
0.00003138f,
0.00002877f,
0.00002657f,
0.00002417f,
0.00002328f,
0.00002129f,
0.00002036f,
0.00001896f,
0.00001779f,
0.00001738f,
0.00001579f,
0.00001598f,
0.00001498f,
0.00001430f,
0.00001381f,
0.00001310f,
0.00001315f,
0.00001249f,
0.00001236f,
0.00001250f,
0.00001178f,
0.00001194f,
0.00001165f,
0.00001136f,
0.00001177f,
0.00001162f,
0.00001149f,
0.00001162f,
0.00001162f,
0.00001183f,
0.00001163f,
0.00001211f,
0.00001182f,
0.00001142f,
0.00001146f,
0.00001129f,
0.00001064f,
0.00001038f,
0.00001123f,
0.00001061f,
0.00001039f,
0.00001007f,
0.00001068f,
0.00001053f,
0.00001131f,
0.00001150f,
0.00001210f,
0.00001199f,
0.00001228f,
0.00001244f,
0.00001267f,
0.00001226f,
0.00001236f,
0.00001242f,
0.00001236f,
0.00001227f,
0.00001222f,
0.00001218f,
0.00001214f,
0.00001206f,
0.00001202f,
0.00001242f,
0.00001231f,
0.00001283f,
0.00001325f,
0.00001395f,
0.00001410f,
0.00001399f,
0.00001534f,
0.00001580f,
0.00001690f,
0.00001748f,
0.00001885f,
0.00001898f,
0.00001988f,
0.00002137f,
0.00002320f,
0.00002504f,
0.00002641f,
0.00002837f,
0.00003079f,
0.00003251f,
0.00003533f,
0.00003740f,
0.00004081f,
0.00004361f,
0.00004639f,
0.00004986f,
0.00005346f,
0.00005805f,
0.00006226f,
0.00006694f,
0.00007188f,
0.00007800f,
0.00008485f,
0.00009055f,
0.00010008f,
0.00010713f,
0.00011933f,
0.00012975f,
0.00014324f,
0.00015785f,
0.00017539f,
0.00019507f,
0.00021689f,
0.00024318f,
0.00027135f,
0.00030518f,
0.00034281f,
0.00039712f,
0.00044651f,
0.00050188f,
0.00056707f,
0.00063984f,
0.00072378f,
0.00081879f,
0.00092782f,
0.00105511f,
0.00120160f,
0.00137088f,
0.00156792f,
0.00180276f,
0.00207920f,
0.00240799f,
0.00280298f,
0.00327494f,
0.00384821f,
0.00454832f,
0.00540493f,
0.00645469f,
0.00774941f,
0.00936399f,
0.01137832f,
0.01388118f,
0.01700597f,
0.02092752f,
0.02586928f,
0.03206911f,
0.03985523f,
0.04956652f,
0.06173849f,
0.07680296f,
0.09550239f,
0.11837088f,
0.14615821f,
0.19110224f,
0.23178705f,
0.27801636f,
0.33031673f,
0.38771033f,
0.44914649f,
0.51370254f,
0.57939644f,
0.64484781f,
0.70835304f,
0.76788190f,
0.82252523f,
0.87063314f,
0.91086251f,
0.94360264f,
0.96734308f,
0.98447500f,
0.99402844f,
0.99904759f,
1.00000000f,
0.99829749f,
0.99534944f,
0.99113813f,
0.98712837f,
0.98344438f,
0.97955733f,
0.97683333f,
0.97381307f,
0.97171326f,
0.96937935f,
0.96682308f,
0.96494693f,
0.96155704f,
0.95914861f,
0.95524656f,
0.95141098f,
0.94725240f,
0.94238136f,
0.93805030f,
0.93299243f,
0.92787669f,
0.92261726f,
0.91765026f,
0.91344782f,
0.90866986f,
0.90481152f,
0.90031693f,
0.89720732f,
0.89342505f,
0.89040645f,
0.88745245f,
0.88446834f,
0.88140255f,
0.87825555f,
0.87487903f,
0.87077850f,
0.86665463f,
0.86193864f,
0.85704385f,
0.85203260f,
0.84608531f,
0.84067874f,
0.83464870f,
0.82847182f,
0.82237664f,
0.81646034f,
0.81067178f,
0.80547624f,
0.79996761f,
0.79538371f,
0.79048464f,
0.78593335f,
0.78246221f,
0.77854086f,
0.77505353f,
0.77183963f,
0.76834628f,
0.76559075f,
0.76238953f,
0.75915219f,
0.75631414f,
0.75288758f,
0.74984835f,
0.74648701f,
0.74311251f,
0.73936992f,
0.73559705f,
0.73163277f,
0.72765295f,
0.72334537f,
0.71892768f,
0.71424776f,
0.70932732f,
0.70417024f,
0.69865547f,
0.69314542f,
0.68706957f,
0.68120878f,
0.67553032f,
0.66989091f,
0.66440763f,
0.65922988f,
0.65415969f,
0.64965635f,
0.64537477f,
0.64153052f,
0.63820324f,
0.63506916f,
0.63264959f,
0.62992135f,
0.62726732f,
0.62445640f,
0.62156999f,
0.61847181f,
0.61501909f,
0.61133939f,
0.60744762f,
0.60314205f,
0.59883705f,
0.59425662f,
0.58928886f,
0.58381508f,
0.57719033f,
0.56857851f,
0.55699311f,
0.54163099f,
0.52202898f,
0.49847084f,
0.47191641f,
0.44270534f,
0.41155122f,
0.37843868f,
0.34304355f,
0.30590964f,
0.26730226f,
0.22805403f,
0.18974714f,
0.15369596f,
0.12145605f,
0.09413162f,
0.07189929f,
0.05457376f,
0.04144546f,
0.03158417f,
0.02425525f,
0.01879617f,
0.01470317f,
0.01163795f,
0.00930858f,
0.00744330f,
0.00607348f,
0.00501379f,
0.00413379f,
0.00347084f,
0.00293573f,
0.00250919f,
0.00215786f,
0.00186591f,
0.00163284f,
0.00142997f,
0.00127599f,
0.00112918f,
0.00102171f,
0.00091069f,
0.00083541f,
0.00075489f,
0.00069931f,
0.00063879f,
0.00059080f,
0.00055023f,
0.00050660f,
0.00047777f,
0.00044789f,
0.00041317f,
0.00039493f,
0.00035947f,
0.00033744f,
0.00031989f,
0.00028719f,
0.00027707f,
0.00025430f,
0.00023132f,
0.00022543f,
0.00019537f,
0.00018380f,
0.00016713f,
0.00014542f,
0.00013876f,
0.00011616f,
0.00011019f,
0.00009086f,
0.00009183f,
0.00006774f,
0.00006181f,
0.00006360f,
0.00004633f,
0.00005224f,
0.00003735f,
0.00003902f,
0.00002393f,
0.00003180f,
0.00001716f,
0.00001762f,
0.00002033f,
0.00001262f,
0.00001259f,
0.00001118f,
0.00001421f,
0.00000830f,
0.00001294f,
0.00000699f,
0.00000782f,
0.00000718f,
0.00000642f,
0.00000548f,
0.00000427f,
0.00000417f,
0.00000440f,
0.00000377f,
0.00000253f,
0.00000456f,
0.00000202f,
0.00000373f,
0.00000325f,
0.00000341f,
0.00000232f,
0.00000301f,
0.00000117f,
0.00000319f,
0.00000169f,
0.00000286f,
0.00000184f,
0.00000260f,
0.00000212f,
0.00000293f,
0.00000175f,
0.00000229f,
0.00000126f,
0.00000232f,
0.00000099f,
0.00000250f,
0.00000135f,
0.00000090f,
0.00000289f,
0.00000070f,
0.00000210f,
0.00000118f,
0.00000072f,
0.00000182f,
0.00000111f
        };
      //Add multispectral bands to the temporary list
      const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );
      const std::vector<float> vb1 (b1, b1 + sizeof(b1) / sizeof(float) );
      const std::vector<float> vb2 (b2, b2 + sizeof(b2) / sizeof(float) );
      const std::vector<float> vb3 (b3, b3 + sizeof(b3) / sizeof(float) );
      //For Spot6 MS image the order of band is: B2 B1 B0 B3
      //(BandIndexToWavelength method could be used here)
      tmpSpectralBandList.push_back(vb2);
      tmpSpectralBandList.push_back(vb1);
      tmpSpectralBandList.push_back(vb0);
      tmpSpectralBandList.push_back(vb3);
      }
    else
      {
      itkExceptionMacro(<< "Invalid Spot6 Sensor ID");
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
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(0.4);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(1.0);
    wavelengthSpectralBand->GetNthElement(j)->SetUserStep(0.001);
    ++j;
    }
  return wavelengthSpectralBand;
}

} // end namespace otb
