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

#include "otbIkonosImageMetadataInterface.h"

#include "otbStringUtils.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

namespace otb
{

IkonosImageMetadataInterface
::IkonosImageMetadataInterface()
{
}

bool
IkonosImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();
  if (sensorID.find("IKONOS-2") != std::string::npos) return true;
  else return false;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface::GetSolarIrradiance() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(1);

  if (!imageKeywordlist.HasKey("support_data.band_name"))
    {
    outputValuesVariableLengthVector[0] = -1;
    return outputValuesVariableLengthVector;
    }

  std::string keywordString = imageKeywordlist.GetMetadataByKey("support_data.band_name");

  // values from geoeye
  // TODO are these the correct values ????
  if (keywordString == "Pan")
    {
    outputValuesVariableLengthVector[0] = 1375.8;
    }
  else if (keywordString == "Blue")
    {
    outputValuesVariableLengthVector[0] = 1930.9;
    }
  else if (keywordString == "Green")
    {
    outputValuesVariableLengthVector[0] = 1854.8;
    }
  else if (keywordString == "Red")
    {
    outputValuesVariableLengthVector[0] = 1556.5;
    }
  else if (keywordString == "NIR")
    {
    outputValuesVariableLengthVector[0] = 1156.9;
    }

  return outputValuesVariableLengthVector;
}

int
IkonosImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.acquisition_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.acquisition_date");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  int value = atoi(outputValues[2].c_str());
  return value;
}

int
IkonosImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.acquisition_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.acquisition_date");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  int value = atoi(outputValues[1].c_str());
  return value;
}


int
IkonosImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.acquisition_date"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.acquisition_date");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");

  int value = atoi(outputValues[0].c_str());
  return value;
}


int
IkonosImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.acquisition_time"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.acquisition_time");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() < 2) itkExceptionMacro(<< "Invalid Hour");

  int value = atoi(outputValues[0].c_str());
  return value;
}

int
IkonosImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.acquisition_time"))
    {
    return -1;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.acquisition_time");
  std::vector<std::string> outputValues;

  boost::split(outputValues, valueString, boost::is_any_of(" T:-"));

  if (outputValues.size() < 2) itkExceptionMacro(<< "Invalid Minute");

  int value = atoi(outputValues[1].c_str());
  return value;
}

int
IkonosImageMetadataInterface::GetProductionDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-/"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Day");
  // MM/DD/YY
  int value = atoi(outputValues[1].c_str());
  return value;
}

int
IkonosImageMetadataInterface::GetProductionMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-/"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Month");
  // MM/DD/YY
  int value = atoi(outputValues[0].c_str());
  return value;
}

int
IkonosImageMetadataInterface::GetProductionYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
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
  boost::split(outputValues, valueString, boost::is_any_of(" T:-/"));

  if (outputValues.size() <= 2) itkExceptionMacro(<< "Invalid Year");
  // MM/DD/YY
  int year = atoi(outputValues[2].c_str());
  if (year == 99) year += 1900;
  else year += 2000;
  return year;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalBias() const
{
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(GetNumberOfBands());
  outputValuesVariableLengthVector.Fill(0.0);

  return outputValuesVariableLengthVector;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalGain() const
{
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  int  productionYear = this->GetProductionYear();
  int  productionMonth = this->GetProductionMonth();
  int  productionDay = this->GetProductionDay();
  bool isPost20010122 = false;
  if ((productionDay + 100 * productionMonth + 10000 * productionYear) >= 20010122)
    {
    isPost20010122 = true;
    }

  //Value computed from
  // http://www.geoeye.com/CorpSite/assets/docs/technical-papers/2009/IKONOS_Esun_Calculations.pdf
  // to get the equivalent of the SPOT alpha
  VariableLengthVectorType gain;
  gain.SetSize(5);
  if (isPost20010122)
    {
    gain[0] = 6.48830; //Pan
    gain[1] = 5.19064; //Blue
    gain[2] = 6.44122; //Green
    gain[3] = 6.24442; //Red
    gain[4] = 8.04222; //NIR
    }
  else
    {
    gain[0] = 6.48830; //Pan
    gain[1] = 4.51329; //Blue
    gain[2] = 5.75014; //Green
    gain[3] = 5.52720; //Red
    gain[4] = 7.11684; //NIR
    }

  std::vector<std::string> bandName = GetBandName();

  VariableLengthVectorType outputValuesVariableLengthVector;
  unsigned int             numBands = GetNumberOfBands();

  /** Tests if the number of bands in metadata are the same as bandName size*/
  if (numBands != bandName.size())
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  outputValuesVariableLengthVector.SetSize(numBands);
  for (unsigned int i = 0; i < numBands; ++i)
    {
    if (bandName[i].find("Pan") != std::string::npos) outputValuesVariableLengthVector[i] = gain[0];
    if (bandName[i].find("Blue") != std::string::npos) outputValuesVariableLengthVector[i] = gain[1];
    if (bandName[i].find("Green") != std::string::npos) outputValuesVariableLengthVector[i] = gain[2];
    if (bandName[i].find("Red") != std::string::npos) outputValuesVariableLengthVector[i] = gain[3];
    if (bandName[i].find("NIR") != std::string::npos) outputValuesVariableLengthVector[i] = gain[4];
    }

  return outputValuesVariableLengthVector;
}

double
IkonosImageMetadataInterface::GetSatElevation() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.nominal_collection_elevation_angle"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.nominal_collection_elevation_angle");
  double value = atof(valueString.c_str());
  return value;
}

double
IkonosImageMetadataInterface::GetSatAzimuth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }
  if (!imageKeywordlist.HasKey("support_data.nominal_collection_azimuth_angle"))
    {
    return 0;
    }

  std::string valueString = imageKeywordlist.GetMetadataByKey("support_data.nominal_collection_azimuth_angle");
  double value = atof(valueString.c_str());
  return value;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetFirstWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
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
    wavel.Fill(0.526);
    }
  else if (nbBands == 4)
    {
    wavel.SetSize(4);
    wavel[0] = 0.445;
    wavel[1] = 0.506;
    wavel[2] = 0.632;
    wavel[3] = 0.757;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetLastWavelengths() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
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
    wavel.Fill(0.929);
    }
  else if (nbBands == 4)
    {
    wavel.SetSize(4);
    wavel[0] = 0.516;
    wavel[1] = 0.595;
    wavel[2] = 0.698;
    wavel[3] = 0.853;
    }
  else itkExceptionMacro(<< "Invalid number of bands...");

  return wavel;
}

std::vector<unsigned int>
IkonosImageMetadataInterface::GetDefaultDisplay() const
{
  std::vector<unsigned int> rgb(3);
  rgb[0] = 2;
  rgb[1] = 1;
  rgb[2] = 0;
  return rgb;
}

std::vector<std::string>
IkonosImageMetadataInterface
::GetEnhancedBandNames() const
{
  // Already done for the current file
   return this->Superclass::GetBandName();
}

IkonosImageMetadataInterface::WavelengthSpectralBandVectorType
IkonosImageMetadataInterface
::GetSpectralSensitivity()  const
{
  //TODO tabulate spectral responses
  WavelengthSpectralBandVectorType wavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();

  std::vector<std::string> bandName = GetBandName();
  unsigned int             numBands = GetNumberOfBands();

  /** Tests if the number of bands in metadata are the same as bandName size*/
  if (numBands != bandName.size())
    {
    itkExceptionMacro(<< "Invalid number of bands...");
    }

  for (unsigned int i = 0; i < numBands; ++i)
    {
    float b0[275];
    if (bandName[i].find("Pan") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000777411f,   0.000698442f,   0.000619473f,   0.000490086f,   0.000360699f,   0.000302118f,
       0.000243538f,   0.000339101f,   0.000434664f,   0.000674356f,   0.000914047f,   0.00165711f,
       0.00240018f,   0.00382524f,   0.00525031f,   0.00774772f,   0.0102451f,   0.0136149f,
       0.0169847f,   0.0216852f,   0.0263858f,   0.0322139f,   0.0380419f,   0.0451535f,
       0.052265f,   0.059372f,   0.066479f,   0.0749836f,   0.0834881f,   0.0922321f,
       0.100976f,   0.11238f,   0.123783f,   0.135806f,   0.147828f,   0.158671f,
       0.169515f,   0.181498f,   0.193481f,   0.199257f,   0.205034f,   0.224842f,
       0.24465f,   0.252955f,   0.26126f,   0.271512f,   0.281764f,   0.291251f,
       0.300738f,   0.30953f,   0.318322f,   0.321402f,   0.324481f,   0.329386f,
       0.33429f,   0.338595f,   0.342899f,   0.346889f,   0.350879f,   0.354954f,
       0.359028f,   0.366986f,   0.374944f,   0.383555f,   0.392166f,   0.407887f,
       0.423609f,   0.439638f,   0.455668f,   0.472286f,   0.488904f,   0.509193f,
       0.529483f,   0.549106f,   0.56873f,   0.592408f,   0.616086f,   0.639134f,
       0.662182f,   0.680563f,   0.698943f,   0.720886f,   0.742829f,   0.764357f,
       0.785886f,   0.805496f,   0.825106f,   0.843924f,   0.862743f,   0.882061f,
       0.901379f,   0.923168f,   0.944958f,   0.948979f,   0.952999f,   0.960789f,
       0.968579f,   0.973284f,   0.977989f,   0.972502f,   0.967015f,   0.980472f,
       0.993929f,   0.993271f,   0.992614f,   0.988076f,   0.983538f,   0.991769f,
       1.0f,   0.996457f,   0.992914f,   0.996456f,   0.999998f,   0.993659f,
       0.98732f,   0.980205f,   0.973089f,   0.957487f,   0.941886f,   0.939394f,
       0.936901f,   0.920086f,   0.90327f,   0.895178f,   0.887086f,   0.878162f,
       0.869238f,   0.876515f,   0.883791f,   0.883182f,   0.882573f,   0.883526f,
       0.884479f,   0.889877f,   0.895275f,   0.895902f,   0.89653f,   0.911267f,
       0.926004f,   0.924618f,   0.923232f,   0.91542f,   0.907607f,   0.904518f,
       0.90143f,   0.903022f,   0.904613f,   0.902261f,   0.899908f,   0.893658f,
       0.887407f,   0.890136f,   0.892866f,   0.889693f,   0.88652f,   0.892279f,
       0.898039f,   0.904587f,   0.911135f,   0.927581f,   0.944026f,   0.951458f,
       0.95889f,   0.947059f,   0.935229f,   0.946541f,   0.957854f,   0.965818f,
       0.973781f,   0.973879f,   0.973978f,   0.971217f,   0.968457f,   0.96373f,
       0.959003f,   0.95437f,   0.949736f,   0.932402f,   0.915067f,   0.906743f,
       0.898418f,   0.90573f,   0.913042f,   0.923885f,   0.934728f,   0.931538f,
       0.928349f,   0.934038f,   0.939728f,   0.923287f,   0.906846f,   0.914615f,
       0.922384f,   0.929317f,   0.93625f,   0.943838f,   0.951427f,   0.948289f,
       0.945151f,   0.949082f,   0.953013f,   0.938094f,   0.923176f,   0.921154f,
       0.919132f,   0.890009f,   0.860885f,   0.867236f,   0.873588f,   0.850292f,
       0.826995f,   0.817882f,   0.808768f,   0.800406f,   0.792044f,   0.777065f,
       0.762087f,   0.755304f,   0.748521f,   0.727815f,   0.707108f,   0.696978f,
       0.686848f,   0.68378f,   0.680711f,   0.675044f,   0.669377f,   0.641688f,
       0.613999f,   0.596144f,   0.578289f,   0.567037f,   0.555785f,   0.536112f,
       0.516439f,   0.498523f,   0.480606f,   0.475695f,   0.470783f,   0.452628f,
       0.434473f,   0.415006f,   0.395538f,   0.373581f,   0.351624f,   0.344258f,
       0.336893f,   0.323178f,   0.309463f,   0.296963f,   0.284464f,   0.277545f,
       0.270627f,   0.259838f,   0.249049f,   0.242783f,   0.236516f,   0.226031f,
       0.215545f,   0.205517f,   0.195489f,   0.187778f,   0.180066f,   0.172017f,
       0.163968f,   0.156649f,   0.14933f,   0.142851f,   0.136372f,   0.127527f,
       0.118682f,   0.111158f,   0.103634f,   0.0963653f,   0.0f
      };

      for (unsigned int j = 0; j < 275; ++j)
        {
        b0[j] = tmp[j];
        }
      }
    else if (bandName[i].find("Blue") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000773147f,   0.00111792f,   0.00146269f,   0.00116425f,   0.000865817f,   0.000798356f,
       0.000730896f,   0.000724069f,   0.000717243f,   0.000983961f,   0.00125068f,   0.00147711f,
       0.00170354f,   0.00181267f,   0.0019218f,   0.00235592f,   0.00279004f,   0.00398685f,
       0.00518366f,   0.00642842f,   0.00767318f,   0.00823188f,   0.00879058f,   0.0103296f,
       0.0118686f,   0.0146603f,   0.017452f,   0.0262281f,   0.0350041f,   0.0602011f,
       0.085398f,   0.139163f,   0.192928f,   0.261737f,   0.330546f,   0.375015f,
       0.419484f,   0.463835f,   0.508186f,   0.560708f,   0.613231f,   0.643718f,
       0.674206f,   0.697319f,   0.720432f,   0.740631f,   0.760831f,   0.760468f,
       0.760104f,   0.77422f,   0.788336f,   0.822457f,   0.856578f,   0.872968f,
       0.889358f,   0.916497f,   0.943636f,   0.971818f,   1.0f,   0.993393f,
       0.986786f,   0.956164f,   0.925542f,   0.843438f,   0.761333f,   0.651888f,
       0.542443f,   0.428187f,   0.31393f,   0.259011f,   0.204092f,   0.178009f,
       0.151925f,   0.118774f,   0.0856216f,   0.0643982f,   0.0431748f,   0.0370744f,
       0.030974f,   0.0288085f,   0.0266429f,   0.0243978f,   0.0221527f,   0.0221454f,
       0.0221381f,   0.0241802f,   0.0262224f,   0.027894f,   0.0295655f,   0.0282719f,
       0.0269782f,   0.0247994f,   0.0226207f,   0.0240241f,   0.0254275f,   0.0279644f,
       0.0305013f,   0.0289285f,   0.0273557f,   0.0243127f,   0.0212697f,   0.021495f,
       0.0217203f,   0.0256572f,   0.0295941f,   0.0319082f,   0.0342224f,   0.0306457f,
       0.027069f,   0.0232825f,   0.019496f,   0.0194953f,   0.0194945f,   0.019262f,
       0.0190295f,   0.0165004f,   0.0139713f,   0.0106605f,   0.00734963f,   0.00665076f,
       0.00595189f,   0.00555088f,   0.00514986f,   0.00651302f,   0.00787617f,   0.0101045f,
       0.0123327f,   0.0161468f,   0.0199609f,   0.0210092f,   0.0220576f,   0.021422f,
       0.0207864f,   0.0198958f,   0.0190052f,   0.0172422f,   0.0154792f,   0.012607f,
       0.00973489f,   0.00827171f,   0.00680854f,   0.00586579f,   0.00492304f,   0.00515291f,
       0.00538278f,   0.00845239f,   0.011522f,   0.0168842f,   0.0222464f,   0.0231172f,
       0.023988f,   0.0180264f,   0.0120648f,   0.0080432f,   0.00402165f,   0.00311771f,
       0.00221378f,   0.00238421f,   0.00255463f,   0.00240824f,   0.00226184f,   0.00286143f,
       0.00346102f,   0.00296337f,   0.00246571f,   0.00239388f,   0.00232204f,   0.00315372f,
       0.0039854f,   0.0037969f,   0.00360839f,   0.00315481f,   0.00270124f,   0.00254147f,
       0.0023817f,   0.00250869f,   0.00263568f,   0.00244201f,   0.00224833f,   0.00192361f,
       0.00159888f,   0.00157883f,   0.00155879f,   0.00177376f,   0.00198873f,   0.00199198f,
       0.00199522f,   0.00225955f,   0.00252388f,   0.00306228f,   0.00360067f,   0.00461183f,
       0.00562299f,   0.00610986f,   0.00659673f,   0.00707311f,   0.00754949f,   0.00775649f,
       0.00796349f,   0.00871715f,   0.00947081f,   0.00887752f,   0.00828423f,   0.00852913f,
       0.00877403f,   0.00901617f,   0.00925831f,   0.00995926f,   0.0106602f,   0.0115813f,
       0.0125024f,   0.0128068f,   0.0131113f,   0.0130847f,   0.0130581f,   0.0124604f,
       0.0118627f,   0.0111828f,   0.0105029f,   0.00998883f,   0.00947476f,   0.00913233f,
       0.00878991f,   0.00859062f,   0.00839133f,   0.00830986f,   0.0082284f,   0.00812489f,
       0.00802138f,   0.00818694f,   0.0083525f,   0.00823223f,   0.00811195f,   0.00805303f,
       0.00799411f,   0.00800437f,   0.00801463f,   0.00743377f,   0.00685291f,   0.00615093f,
       0.00544896f,   0.00512429f,   0.00479963f,   0.00463363f,   0.00446762f,   0.00428193f,
       0.00409623f,   0.00384906f,   0.00360189f,   0.00353085f,   0.00345981f,   0.00336041f,
       0.00326101f,   0.00300593f,   0.00275086f,   0.00278287f,   0.00281488f,   0.00259549f,
       0.00237609f,   0.00236111f,   0.00234613f,   0.00248306f,   0.00261998f,   0.00263357f,
       0.00264716f,   0.0026242f,   0.00260124f,   0.00210138f,   0.00160152f,   0.00177352f,
       0.00194551f,   0.00218138f,   0.00241724f,   0.00185843f,   0.0f
      };

      for (unsigned int j = 0; j < 275; ++j)
        {
        b0[j] = tmp[j];
        }
      }
    else if (bandName[i].find("Green") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000643373f,   0.000721567f,   0.000799761f,   0.000764593f,   0.000729425f,   0.000767756f,
       0.000806087f,   0.000691702f,   0.000577317f,   0.000709642f,   0.000841967f,   0.000741458f,
       0.000640949f,   0.000754853f,   0.000868757f,   0.000732192f,   0.000595627f,   0.000952731f,
       0.00130983f,   0.00125601f,   0.00120219f,   0.00194445f,   0.00268671f,   0.00183813f,
       0.000989541f,   0.00113105f,   0.00127256f,   0.00160875f,   0.00194493f,   0.00229841f,
       0.00265189f,   0.00301361f,   0.00337532f,   0.00370611f,   0.00403689f,   0.00486952f,
       0.00570216f,   0.00738653f,   0.0090709f,   0.010205f,   0.011339f,   0.0113175f,
       0.011296f,   0.0121669f,   0.0130378f,   0.0151163f,   0.0171948f,   0.0208267f,
       0.0244586f,   0.0305813f,   0.036704f,   0.0434839f,   0.0502638f,   0.0565094f,
       0.0627551f,   0.0860566f,   0.109358f,   0.151232f,   0.193107f,   0.242087f,
       0.291067f,   0.365317f,   0.439568f,   0.545145f,   0.650723f,   0.728637f,
       0.806551f,   0.839149f,   0.871746f,   0.88005f,   0.888355f,   0.889065f,
       0.889775f,   0.882352f,   0.874928f,   0.863518f,   0.852108f,   0.872214f,
       0.89232f,   0.944994f,   0.997669f,   0.981902f,   0.966134f,   0.936936f,
       0.907737f,   0.900477f,   0.893218f,   0.919539f,   0.94586f,   0.971546f,
       0.997232f,   0.978528f,   0.959823f,   0.918631f,   0.877439f,   0.813588f,
       0.749737f,   0.625854f,   0.50197f,   0.392566f,   0.283162f,   0.22152f,
       0.159878f,   0.133355f,   0.106831f,   0.0946597f,   0.0824881f,   0.0732495f,
       0.0640108f,   0.0556904f,   0.0473701f,   0.039668f,   0.031966f,   0.0273263f,
       0.0226867f,   0.0196283f,   0.0165699f,   0.0155271f,   0.0144843f,   0.0146602f,
       0.0148362f,   0.0155289f,   0.0162216f,   0.0174317f,   0.0186418f,   0.0192574f,
       0.019873f,   0.0193336f,   0.0187943f,   0.0169768f,   0.0151594f,   0.0138395f,
       0.0125197f,   0.0122022f,   0.0118846f,   0.0113754f,   0.0108661f,   0.0122059f,
       0.0135457f,   0.0150274f,   0.0165092f,   0.0174059f,   0.0183026f,   0.0170915f,
       0.0158804f,   0.0135402f,   0.0112001f,   0.00877389f,   0.00634772f,   0.00592819f,
       0.00550866f,   0.00444963f,   0.0033906f,   0.00363493f,   0.00387926f,   0.00369547f,
       0.00351167f,   0.00323999f,   0.0029683f,   0.00304107f,   0.00311383f,   0.00269225f,
       0.00227067f,   0.00285765f,   0.00344463f,   0.0035057f,   0.00356676f,   0.00444226f,
       0.00531776f,   0.00591164f,   0.00650553f,   0.00683137f,   0.00715721f,   0.00801677f,
       0.00887633f,   0.00920412f,   0.0095319f,   0.00876825f,   0.00800459f,   0.00751842f,
       0.00703224f,   0.00673578f,   0.00643931f,   0.00627542f,   0.00611152f,   0.00586377f,
       0.00561602f,   0.00543874f,   0.00526147f,   0.0057041f,   0.00614673f,   0.00676773f,
       0.00738874f,   0.00702707f,   0.00666541f,   0.0066904f,   0.0067154f,   0.00807246f,
       0.00942952f,   0.0111187f,   0.0128079f,   0.0135393f,   0.0142706f,   0.015097f,
       0.0159234f,   0.0165052f,   0.017087f,   0.0171597f,   0.0172324f,   0.0167555f,
       0.0162786f,   0.0157604f,   0.0152423f,   0.0143205f,   0.0133987f,   0.0126828f,
       0.0119669f,   0.0112674f,   0.0105679f,   0.0102702f,   0.00997248f,   0.00958224f,
       0.009192f,   0.00947248f,   0.00975296f,   0.00932626f,   0.00889956f,   0.00866396f,
       0.00842837f,   0.0081121f,   0.00779583f,   0.00765918f,   0.00752253f,   0.00724579f,
       0.00696906f,   0.0064155f,   0.00586195f,   0.00563761f,   0.00541328f,   0.00499098f,
       0.00456868f,   0.00425882f,   0.00394895f,   0.00377609f,   0.00360322f,   0.00343181f,
       0.00326041f,   0.00310311f,   0.00294581f,   0.00285307f,   0.00276034f,   0.00277242f,
       0.0027845f,   0.00250953f,   0.00223456f,   0.00235165f,   0.00246875f,   0.00235075f,
       0.00223275f,   0.00238435f,   0.00253594f,   0.00230124f,   0.00206653f,   0.00190024f,
       0.00173395f,   0.00191697f,   0.00209999f,   0.00206986f,   0.00203973f,   0.002129f,
       0.00221828f,   0.00179734f,   0.00137641f,   0.00148945f,   0.0f
      };

      for (unsigned int j = 0; j < 275; ++j)
        {
        b0[j] = tmp[j];
        }
      }
    else if (bandName[i].find("Red") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.0f,   0.000251974f,   0.000503948f,   0.000447294f,   0.000390639f,   0.000493587f,
       0.000596534f,   0.000462484f,   0.000328434f,   0.000587581f,   0.000846728f,   0.000748537f,
       0.000650345f,   0.000516444f,   0.000382543f,   0.000939994f,   0.00149745f,   0.00175874f,
       0.00202003f,   0.00252033f,   0.00302063f,   0.00326826f,   0.00351589f,   0.00376182f,
       0.00400775f,   0.00406784f,   0.00412793f,   0.00595608f,   0.00778423f,   0.0080879f,
       0.00839158f,   0.00829197f,   0.00819236f,   0.00662469f,   0.00505701f,   0.00383289f,
       0.00260876f,   0.00345153f,   0.00429429f,   0.00487448f,   0.00545468f,   0.00545833f,
       0.00546199f,   0.00418794f,   0.00291388f,   0.00230581f,   0.00169773f,   0.00162001f,
       0.0015423f,   0.00259185f,   0.00364139f,   0.00396142f,   0.00428144f,   0.00339831f,
       0.00251517f,   0.00227348f,   0.00203179f,   0.00167973f,   0.00132766f,   0.00141316f,
       0.00149866f,   0.00150402f,   0.00150938f,   0.0018186f,   0.00212782f,   0.00224531f,
       0.0023628f,   0.0025393f,   0.0027158f,   0.00304882f,   0.00338183f,   0.00312524f,
       0.00286866f,   0.00286392f,   0.00285919f,   0.00345904f,   0.0040589f,   0.00503972f,
       0.00602055f,   0.00659323f,   0.00716591f,   0.00825183f,   0.00933776f,   0.00929585f,
       0.00925395f,   0.00899875f,   0.00874355f,   0.00912988f,   0.0095162f,   0.00993024f,
       0.0103443f,   0.0118352f,   0.0133262f,   0.0168796f,   0.020433f,   0.0244432f,
       0.0284535f,   0.0315921f,   0.0347306f,   0.0387702f,   0.0428098f,   0.0501643f,
       0.0575189f,   0.0714861f,   0.0854534f,   0.105156f,   0.124858f,   0.150634f,
       0.17641f,   0.217427f,   0.258444f,   0.338645f,   0.418847f,   0.528481f,
       0.638116f,   0.721314f,   0.804512f,   0.841436f,   0.87836f,   0.898975f,
       0.91959f,   0.936413f,   0.953235f,   0.954545f,   0.955856f,   0.942354f,
       0.928852f,   0.933873f,   0.938894f,   0.962762f,   0.98663f,   0.991712f,
       0.996794f,   0.952358f,   0.907922f,   0.838837f,   0.769753f,   0.68084f,
       0.591927f,   0.506722f,   0.421517f,   0.363416f,   0.305314f,   0.265769f,
       0.226224f,   0.192279f,   0.158334f,   0.131456f,   0.104578f,   0.0859652f,
       0.067352f,   0.0560433f,   0.0447347f,   0.0373336f,   0.0299325f,   0.0269409f,
       0.0239493f,   0.0229628f,   0.0219762f,   0.0201699f,   0.0183636f,   0.0171332f,
       0.0159029f,   0.0160653f,   0.0162278f,   0.0159488f,   0.0156697f,   0.0160819f,
       0.0164941f,   0.0168042f,   0.0171144f,   0.0167051f,   0.0162959f,   0.0141966f,
       0.0120973f,   0.0105832f,   0.00906906f,   0.00792008f,   0.0067711f,   0.00649551f,
       0.00621991f,   0.00588748f,   0.00555504f,   0.00486398f,   0.00417292f,   0.00419873f,
       0.00422454f,   0.00461445f,   0.00500436f,   0.00449582f,   0.00398727f,   0.00397629f,
       0.00396531f,   0.00332414f,   0.00268297f,   0.00284406f,   0.00300515f,   0.00277493f,
       0.00254471f,   0.00290075f,   0.00325678f,   0.00290726f,   0.00255773f,   0.00259229f,
       0.00262684f,   0.00269255f,   0.00275825f,   0.00271562f,   0.00267299f,   0.00275871f,
       0.00284443f,   0.00269768f,   0.00255094f,   0.00257098f,   0.00259103f,   0.00253264f,
       0.00247425f,   0.00242522f,   0.00237619f,   0.00235338f,   0.00233057f,   0.00261103f,
       0.00289149f,   0.0028024f,   0.00271332f,   0.00290319f,   0.00309306f,   0.00350249f,
       0.00391192f,   0.00395984f,   0.00400776f,   0.00401089f,   0.00401403f,   0.0037349f,
       0.00345576f,   0.00305136f,   0.00264696f,   0.00241812f,   0.00218929f,   0.00191393f,
       0.00163858f,   0.00153304f,   0.00142751f,   0.00137032f,   0.00131312f,   0.00134518f,
       0.00137723f,   0.0012429f,   0.00110858f,   0.00114136f,   0.00117415f,   0.00116687f,
       0.00115959f,   0.00108165f,   0.00100371f,   0.00131319f,   0.00162267f,   0.00130164f,
       0.000980609f,   0.00115412f,   0.00132762f,   0.0013807f,   0.00143378f,   0.00218704f,
       0.00294031f,   0.00234605f,   0.00175179f,   0.0020462f,   0.0023406f,   0.00180012f,
       0.00125963f,   0.00107872f,   0.000897814f,   0.000448907f,   0.0f
      };

      for (unsigned int j = 0; j < 275; ++j)
        {
        b0[j] = tmp[j];
        }
      }
    else if (bandName[i].find("NIR") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000894394f,   0.000713315f,   0.000532235f,   0.000603058f,   0.000673881f,   0.000760361f,
       0.00084684f,   0.000787802f,   0.000728763f,   0.000817305f,   0.000905846f,   0.000904774f,
       0.000903701f,   0.00160409f,   0.00230447f,   0.00221407f,   0.00212367f,   0.00314219f,
       0.00416071f,   0.00470014f,   0.00523956f,   0.00641267f,   0.00758578f,   0.0090575f,
       0.0105292f,   0.0156041f,   0.0206791f,   0.027643f,   0.0346069f,   0.0395911f,
       0.0445753f,   0.0408266f,   0.0370779f,   0.0276002f,   0.0181225f,   0.017135f,
       0.0161476f,   0.0168906f,   0.0176337f,   0.0172355f,   0.0168373f,   0.0168014f,
       0.0167656f,   0.0178122f,   0.0188588f,   0.0165949f,   0.0143311f,   0.0120729f,
       0.0098147f,   0.0125599f,   0.0153051f,   0.0187014f,   0.0220978f,   0.0244686f,
       0.0268393f,   0.0270579f,   0.0272765f,   0.0223541f,   0.0174318f,   0.0124528f,
       0.00747381f,   0.00633578f,   0.00519774f,   0.00546479f,   0.00573183f,   0.00496663f,
       0.00420143f,   0.00375248f,   0.00330354f,   0.00289353f,   0.00248353f,   0.00252329f,
       0.00256304f,   0.00431351f,   0.00606397f,   0.00864978f,   0.0112356f,   0.0122617f,
       0.0132878f,   0.0113116f,   0.00933533f,   0.00834238f,   0.00734942f,   0.00698854f,
       0.00662766f,   0.00662135f,   0.00661504f,   0.00802641f,   0.00943779f,   0.0113662f,
       0.0132947f,   0.0126595f,   0.0120243f,   0.00943568f,   0.0068471f,   0.00523536f,
       0.00362362f,   0.00341318f,   0.00320274f,   0.00318157f,   0.00316041f,   0.00303014f,
       0.00289988f,   0.00349364f,   0.0040874f,   0.00427183f,   0.00445627f,   0.00425418f,
       0.0040521f,   0.00432398f,   0.00459586f,   0.00500329f,   0.00541072f,   0.0052616f,
       0.00511248f,   0.00619044f,   0.0072684f,   0.007209f,   0.0071496f,   0.00780861f,
       0.00846762f,   0.0103595f,   0.0122514f,   0.014548f,   0.0168445f,   0.0180774f,
       0.0193103f,   0.0203458f,   0.0213814f,   0.021739f,   0.0220966f,   0.0227061f,
       0.0233155f,   0.0241796f,   0.0250438f,   0.0249493f,   0.0248548f,   0.0257403f,
       0.0266259f,   0.0274829f,   0.0283399f,   0.0315445f,   0.034749f,   0.0392491f,
       0.0437491f,   0.0488567f,   0.0539644f,   0.0651598f,   0.0763552f,   0.0905082f,
       0.104661f,   0.125801f,   0.146941f,   0.17194f,   0.196939f,   0.22805f,
       0.25916f,   0.283963f,   0.308767f,   0.332943f,   0.357119f,   0.400422f,
       0.443726f,   0.504385f,   0.565045f,   0.633202f,   0.701359f,   0.76896f,
       0.83656f,   0.889646f,   0.942732f,   0.971366f,   1.0f,   0.997999f,
       0.995998f,   0.963345f,   0.930691f,   0.881401f,   0.832111f,   0.811504f,
       0.790896f,   0.782153f,   0.773411f,   0.768032f,   0.762653f,   0.735848f,
       0.709043f,   0.707354f,   0.705666f,   0.697871f,   0.690076f,   0.672979f,
       0.655882f,   0.638978f,   0.622073f,   0.601538f,   0.581002f,   0.557459f,
       0.533915f,   0.524241f,   0.514567f,   0.501153f,   0.487739f,   0.452918f,
       0.418097f,   0.370059f,   0.32202f,   0.285355f,   0.248689f,   0.209974f,
       0.171259f,   0.142537f,   0.113816f,   0.092517f,   0.0712178f,   0.0646218f,
       0.0580258f,   0.050796f,   0.0435662f,   0.0353664f,   0.0271667f,   0.0232974f,
       0.0194282f,   0.0176517f,   0.0158752f,   0.0144977f,   0.0131202f,   0.0118952f,
       0.0106703f,   0.0101536f,   0.00963684f,   0.00921561f,   0.00879438f,   0.00794162f,
       0.00708886f,   0.00654941f,   0.00600996f,   0.00563429f,   0.00525862f,   0.00510421f,
       0.00494981f,   0.00454041f,   0.00413101f,   0.00410078f,   0.00407056f,   0.00396389f,
       0.00385722f,   0.00364911f,   0.00344101f,   0.00333006f,   0.00321911f,   0.00300415f,
       0.00278918f,   0.00317632f,   0.00356346f,   0.00312642f,   0.00268937f,   0.00260112f,
       0.00251287f,   0.00263123f,   0.00274958f,   0.00362147f,   0.00449336f,   0.00413978f,
       0.0037862f,   0.0028968f,   0.0020074f,   0.00190022f,   0.00179303f,   0.00227357f,
       0.0027541f,   0.00137705f,   0.0f,   0.0f,   0.0f
      };

      for (unsigned int j = 0; j < 275; ++j)
        {
        b0[j] = tmp[j];
        }
      }
    else
      {
      itkExceptionMacro(<< "Invalid band name...");
      }

    //create wavelength band vector
    const std::vector<float> vb0 (b0, b0 + sizeof(b0) / sizeof(float) );

    wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
    wavelengthSpectralBand->GetNthElement(i)->SetFilterFunctionValues(vb0);
    wavelengthSpectralBand->GetNthElement(i)->SetMinSpectralValue(0.350);
    wavelengthSpectralBand->GetNthElement(i)->SetMaxSpectralValue(1.035);
    wavelengthSpectralBand->GetNthElement(i)->SetUserStep(0.0025);
    }


  return wavelengthSpectralBand;
}

} // end namespace otb
