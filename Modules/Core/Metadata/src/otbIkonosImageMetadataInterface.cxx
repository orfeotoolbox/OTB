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

#include <boost/algorithm/string.hpp>
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
       0.000777411,   0.000698442,   0.000619473,   0.000490086,   0.000360699,   0.000302118,
       0.000243538,   0.000339101,   0.000434664,   0.000674356,   0.000914047,   0.00165711,
       0.00240018,   0.00382524,   0.00525031,   0.00774772,   0.0102451,   0.0136149,
       0.0169847,   0.0216852,   0.0263858,   0.0322139,   0.0380419,   0.0451535,
       0.052265,   0.059372,   0.066479,   0.0749836,   0.0834881,   0.0922321,
       0.100976,   0.11238,   0.123783,   0.135806,   0.147828,   0.158671,
       0.169515,   0.181498,   0.193481,   0.199257,   0.205034,   0.224842,
       0.24465,   0.252955,   0.26126,   0.271512,   0.281764,   0.291251,
       0.300738,   0.30953,   0.318322,   0.321402,   0.324481,   0.329386,
       0.33429,   0.338595,   0.342899,   0.346889,   0.350879,   0.354954,
       0.359028,   0.366986,   0.374944,   0.383555,   0.392166,   0.407887,
       0.423609,   0.439638,   0.455668,   0.472286,   0.488904,   0.509193,
       0.529483,   0.549106,   0.56873,   0.592408,   0.616086,   0.639134,
       0.662182,   0.680563,   0.698943,   0.720886,   0.742829,   0.764357,
       0.785886,   0.805496,   0.825106,   0.843924,   0.862743,   0.882061,
       0.901379,   0.923168,   0.944958,   0.948979,   0.952999,   0.960789,
       0.968579,   0.973284,   0.977989,   0.972502,   0.967015,   0.980472,
       0.993929,   0.993271,   0.992614,   0.988076,   0.983538,   0.991769,
       1,   0.996457,   0.992914,   0.996456,   0.999998,   0.993659,
       0.98732,   0.980205,   0.973089,   0.957487,   0.941886,   0.939394,
       0.936901,   0.920086,   0.90327,   0.895178,   0.887086,   0.878162,
       0.869238,   0.876515,   0.883791,   0.883182,   0.882573,   0.883526,
       0.884479,   0.889877,   0.895275,   0.895902,   0.89653,   0.911267,
       0.926004,   0.924618,   0.923232,   0.91542,   0.907607,   0.904518,
       0.90143,   0.903022,   0.904613,   0.902261,   0.899908,   0.893658,
       0.887407,   0.890136,   0.892866,   0.889693,   0.88652,   0.892279,
       0.898039,   0.904587,   0.911135,   0.927581,   0.944026,   0.951458,
       0.95889,   0.947059,   0.935229,   0.946541,   0.957854,   0.965818,
       0.973781,   0.973879,   0.973978,   0.971217,   0.968457,   0.96373,
       0.959003,   0.95437,   0.949736,   0.932402,   0.915067,   0.906743,
       0.898418,   0.90573,   0.913042,   0.923885,   0.934728,   0.931538,
       0.928349,   0.934038,   0.939728,   0.923287,   0.906846,   0.914615,
       0.922384,   0.929317,   0.93625,   0.943838,   0.951427,   0.948289,
       0.945151,   0.949082,   0.953013,   0.938094,   0.923176,   0.921154,
       0.919132,   0.890009,   0.860885,   0.867236,   0.873588,   0.850292,
       0.826995,   0.817882,   0.808768,   0.800406,   0.792044,   0.777065,
       0.762087,   0.755304,   0.748521,   0.727815,   0.707108,   0.696978,
       0.686848,   0.68378,   0.680711,   0.675044,   0.669377,   0.641688,
       0.613999,   0.596144,   0.578289,   0.567037,   0.555785,   0.536112,
       0.516439,   0.498523,   0.480606,   0.475695,   0.470783,   0.452628,
       0.434473,   0.415006,   0.395538,   0.373581,   0.351624,   0.344258,
       0.336893,   0.323178,   0.309463,   0.296963,   0.284464,   0.277545,
       0.270627,   0.259838,   0.249049,   0.242783,   0.236516,   0.226031,
       0.215545,   0.205517,   0.195489,   0.187778,   0.180066,   0.172017,
       0.163968,   0.156649,   0.14933,   0.142851,   0.136372,   0.127527,
       0.118682,   0.111158,   0.103634,   0.0963653,   0
      };

      for (unsigned int i = 0; i<275; ++i)
        {
        b0[i] = tmp[i];
        }
      }
    else if (bandName[i].find("Blue") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000773147,   0.00111792,   0.00146269,   0.00116425,   0.000865817,   0.000798356,
       0.000730896,   0.000724069,   0.000717243,   0.000983961,   0.00125068,   0.00147711,
       0.00170354,   0.00181267,   0.0019218,   0.00235592,   0.00279004,   0.00398685,
       0.00518366,   0.00642842,   0.00767318,   0.00823188,   0.00879058,   0.0103296,
       0.0118686,   0.0146603,   0.017452,   0.0262281,   0.0350041,   0.0602011,
       0.085398,   0.139163,   0.192928,   0.261737,   0.330546,   0.375015,
       0.419484,   0.463835,   0.508186,   0.560708,   0.613231,   0.643718,
       0.674206,   0.697319,   0.720432,   0.740631,   0.760831,   0.760468,
       0.760104,   0.77422,   0.788336,   0.822457,   0.856578,   0.872968,
       0.889358,   0.916497,   0.943636,   0.971818,   1,   0.993393,
       0.986786,   0.956164,   0.925542,   0.843438,   0.761333,   0.651888,
       0.542443,   0.428187,   0.31393,   0.259011,   0.204092,   0.178009,
       0.151925,   0.118774,   0.0856216,   0.0643982,   0.0431748,   0.0370744,
       0.030974,   0.0288085,   0.0266429,   0.0243978,   0.0221527,   0.0221454,
       0.0221381,   0.0241802,   0.0262224,   0.027894,   0.0295655,   0.0282719,
       0.0269782,   0.0247994,   0.0226207,   0.0240241,   0.0254275,   0.0279644,
       0.0305013,   0.0289285,   0.0273557,   0.0243127,   0.0212697,   0.021495,
       0.0217203,   0.0256572,   0.0295941,   0.0319082,   0.0342224,   0.0306457,
       0.027069,   0.0232825,   0.019496,   0.0194953,   0.0194945,   0.019262,
       0.0190295,   0.0165004,   0.0139713,   0.0106605,   0.00734963,   0.00665076,
       0.00595189,   0.00555088,   0.00514986,   0.00651302,   0.00787617,   0.0101045,
       0.0123327,   0.0161468,   0.0199609,   0.0210092,   0.0220576,   0.021422,
       0.0207864,   0.0198958,   0.0190052,   0.0172422,   0.0154792,   0.012607,
       0.00973489,   0.00827171,   0.00680854,   0.00586579,   0.00492304,   0.00515291,
       0.00538278,   0.00845239,   0.011522,   0.0168842,   0.0222464,   0.0231172,
       0.023988,   0.0180264,   0.0120648,   0.0080432,   0.00402165,   0.00311771,
       0.00221378,   0.00238421,   0.00255463,   0.00240824,   0.00226184,   0.00286143,
       0.00346102,   0.00296337,   0.00246571,   0.00239388,   0.00232204,   0.00315372,
       0.0039854,   0.0037969,   0.00360839,   0.00315481,   0.00270124,   0.00254147,
       0.0023817,   0.00250869,   0.00263568,   0.00244201,   0.00224833,   0.00192361,
       0.00159888,   0.00157883,   0.00155879,   0.00177376,   0.00198873,   0.00199198,
       0.00199522,   0.00225955,   0.00252388,   0.00306228,   0.00360067,   0.00461183,
       0.00562299,   0.00610986,   0.00659673,   0.00707311,   0.00754949,   0.00775649,
       0.00796349,   0.00871715,   0.00947081,   0.00887752,   0.00828423,   0.00852913,
       0.00877403,   0.00901617,   0.00925831,   0.00995926,   0.0106602,   0.0115813,
       0.0125024,   0.0128068,   0.0131113,   0.0130847,   0.0130581,   0.0124604,
       0.0118627,   0.0111828,   0.0105029,   0.00998883,   0.00947476,   0.00913233,
       0.00878991,   0.00859062,   0.00839133,   0.00830986,   0.0082284,   0.00812489,
       0.00802138,   0.00818694,   0.0083525,   0.00823223,   0.00811195,   0.00805303,
       0.00799411,   0.00800437,   0.00801463,   0.00743377,   0.00685291,   0.00615093,
       0.00544896,   0.00512429,   0.00479963,   0.00463363,   0.00446762,   0.00428193,
       0.00409623,   0.00384906,   0.00360189,   0.00353085,   0.00345981,   0.00336041,
       0.00326101,   0.00300593,   0.00275086,   0.00278287,   0.00281488,   0.00259549,
       0.00237609,   0.00236111,   0.00234613,   0.00248306,   0.00261998,   0.00263357,
       0.00264716,   0.0026242,   0.00260124,   0.00210138,   0.00160152,   0.00177352,
       0.00194551,   0.00218138,   0.00241724,   0.00185843,   0
      };

      for (unsigned int i = 0; i<275; ++i)
        {
        b0[i] = tmp[i];
        }
      }
    else if (bandName[i].find("Green") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000643373,   0.000721567,   0.000799761,   0.000764593,   0.000729425,   0.000767756,
       0.000806087,   0.000691702,   0.000577317,   0.000709642,   0.000841967,   0.000741458,
       0.000640949,   0.000754853,   0.000868757,   0.000732192,   0.000595627,   0.000952731,
       0.00130983,   0.00125601,   0.00120219,   0.00194445,   0.00268671,   0.00183813,
       0.000989541,   0.00113105,   0.00127256,   0.00160875,   0.00194493,   0.00229841,
       0.00265189,   0.00301361,   0.00337532,   0.00370611,   0.00403689,   0.00486952,
       0.00570216,   0.00738653,   0.0090709,   0.010205,   0.011339,   0.0113175,
       0.011296,   0.0121669,   0.0130378,   0.0151163,   0.0171948,   0.0208267,
       0.0244586,   0.0305813,   0.036704,   0.0434839,   0.0502638,   0.0565094,
       0.0627551,   0.0860566,   0.109358,   0.151232,   0.193107,   0.242087,
       0.291067,   0.365317,   0.439568,   0.545145,   0.650723,   0.728637,
       0.806551,   0.839149,   0.871746,   0.88005,   0.888355,   0.889065,
       0.889775,   0.882352,   0.874928,   0.863518,   0.852108,   0.872214,
       0.89232,   0.944994,   0.997669,   0.981902,   0.966134,   0.936936,
       0.907737,   0.900477,   0.893218,   0.919539,   0.94586,   0.971546,
       0.997232,   0.978528,   0.959823,   0.918631,   0.877439,   0.813588,
       0.749737,   0.625854,   0.50197,   0.392566,   0.283162,   0.22152,
       0.159878,   0.133355,   0.106831,   0.0946597,   0.0824881,   0.0732495,
       0.0640108,   0.0556904,   0.0473701,   0.039668,   0.031966,   0.0273263,
       0.0226867,   0.0196283,   0.0165699,   0.0155271,   0.0144843,   0.0146602,
       0.0148362,   0.0155289,   0.0162216,   0.0174317,   0.0186418,   0.0192574,
       0.019873,   0.0193336,   0.0187943,   0.0169768,   0.0151594,   0.0138395,
       0.0125197,   0.0122022,   0.0118846,   0.0113754,   0.0108661,   0.0122059,
       0.0135457,   0.0150274,   0.0165092,   0.0174059,   0.0183026,   0.0170915,
       0.0158804,   0.0135402,   0.0112001,   0.00877389,   0.00634772,   0.00592819,
       0.00550866,   0.00444963,   0.0033906,   0.00363493,   0.00387926,   0.00369547,
       0.00351167,   0.00323999,   0.0029683,   0.00304107,   0.00311383,   0.00269225,
       0.00227067,   0.00285765,   0.00344463,   0.0035057,   0.00356676,   0.00444226,
       0.00531776,   0.00591164,   0.00650553,   0.00683137,   0.00715721,   0.00801677,
       0.00887633,   0.00920412,   0.0095319,   0.00876825,   0.00800459,   0.00751842,
       0.00703224,   0.00673578,   0.00643931,   0.00627542,   0.00611152,   0.00586377,
       0.00561602,   0.00543874,   0.00526147,   0.0057041,   0.00614673,   0.00676773,
       0.00738874,   0.00702707,   0.00666541,   0.0066904,   0.0067154,   0.00807246,
       0.00942952,   0.0111187,   0.0128079,   0.0135393,   0.0142706,   0.015097,
       0.0159234,   0.0165052,   0.017087,   0.0171597,   0.0172324,   0.0167555,
       0.0162786,   0.0157604,   0.0152423,   0.0143205,   0.0133987,   0.0126828,
       0.0119669,   0.0112674,   0.0105679,   0.0102702,   0.00997248,   0.00958224,
       0.009192,   0.00947248,   0.00975296,   0.00932626,   0.00889956,   0.00866396,
       0.00842837,   0.0081121,   0.00779583,   0.00765918,   0.00752253,   0.00724579,
       0.00696906,   0.0064155,   0.00586195,   0.00563761,   0.00541328,   0.00499098,
       0.00456868,   0.00425882,   0.00394895,   0.00377609,   0.00360322,   0.00343181,
       0.00326041,   0.00310311,   0.00294581,   0.00285307,   0.00276034,   0.00277242,
       0.0027845,   0.00250953,   0.00223456,   0.00235165,   0.00246875,   0.00235075,
       0.00223275,   0.00238435,   0.00253594,   0.00230124,   0.00206653,   0.00190024,
       0.00173395,   0.00191697,   0.00209999,   0.00206986,   0.00203973,   0.002129,
       0.00221828,   0.00179734,   0.00137641,   0.00148945,   0
      };

      for (unsigned int i = 0; i<275; ++i)
        {
        b0[i] = tmp[i];
        }
      }
    else if (bandName[i].find("Red") != std::string::npos)
      {
      const float tmp[275] =
      {
       0,   0.000251974,   0.000503948,   0.000447294,   0.000390639,   0.000493587,
       0.000596534,   0.000462484,   0.000328434,   0.000587581,   0.000846728,   0.000748537,
       0.000650345,   0.000516444,   0.000382543,   0.000939994,   0.00149745,   0.00175874,
       0.00202003,   0.00252033,   0.00302063,   0.00326826,   0.00351589,   0.00376182,
       0.00400775,   0.00406784,   0.00412793,   0.00595608,   0.00778423,   0.0080879,
       0.00839158,   0.00829197,   0.00819236,   0.00662469,   0.00505701,   0.00383289,
       0.00260876,   0.00345153,   0.00429429,   0.00487448,   0.00545468,   0.00545833,
       0.00546199,   0.00418794,   0.00291388,   0.00230581,   0.00169773,   0.00162001,
       0.0015423,   0.00259185,   0.00364139,   0.00396142,   0.00428144,   0.00339831,
       0.00251517,   0.00227348,   0.00203179,   0.00167973,   0.00132766,   0.00141316,
       0.00149866,   0.00150402,   0.00150938,   0.0018186,   0.00212782,   0.00224531,
       0.0023628,   0.0025393,   0.0027158,   0.00304882,   0.00338183,   0.00312524,
       0.00286866,   0.00286392,   0.00285919,   0.00345904,   0.0040589,   0.00503972,
       0.00602055,   0.00659323,   0.00716591,   0.00825183,   0.00933776,   0.00929585,
       0.00925395,   0.00899875,   0.00874355,   0.00912988,   0.0095162,   0.00993024,
       0.0103443,   0.0118352,   0.0133262,   0.0168796,   0.020433,   0.0244432,
       0.0284535,   0.0315921,   0.0347306,   0.0387702,   0.0428098,   0.0501643,
       0.0575189,   0.0714861,   0.0854534,   0.105156,   0.124858,   0.150634,
       0.17641,   0.217427,   0.258444,   0.338645,   0.418847,   0.528481,
       0.638116,   0.721314,   0.804512,   0.841436,   0.87836,   0.898975,
       0.91959,   0.936413,   0.953235,   0.954545,   0.955856,   0.942354,
       0.928852,   0.933873,   0.938894,   0.962762,   0.98663,   0.991712,
       0.996794,   0.952358,   0.907922,   0.838837,   0.769753,   0.68084,
       0.591927,   0.506722,   0.421517,   0.363416,   0.305314,   0.265769,
       0.226224,   0.192279,   0.158334,   0.131456,   0.104578,   0.0859652,
       0.067352,   0.0560433,   0.0447347,   0.0373336,   0.0299325,   0.0269409,
       0.0239493,   0.0229628,   0.0219762,   0.0201699,   0.0183636,   0.0171332,
       0.0159029,   0.0160653,   0.0162278,   0.0159488,   0.0156697,   0.0160819,
       0.0164941,   0.0168042,   0.0171144,   0.0167051,   0.0162959,   0.0141966,
       0.0120973,   0.0105832,   0.00906906,   0.00792008,   0.0067711,   0.00649551,
       0.00621991,   0.00588748,   0.00555504,   0.00486398,   0.00417292,   0.00419873,
       0.00422454,   0.00461445,   0.00500436,   0.00449582,   0.00398727,   0.00397629,
       0.00396531,   0.00332414,   0.00268297,   0.00284406,   0.00300515,   0.00277493,
       0.00254471,   0.00290075,   0.00325678,   0.00290726,   0.00255773,   0.00259229,
       0.00262684,   0.00269255,   0.00275825,   0.00271562,   0.00267299,   0.00275871,
       0.00284443,   0.00269768,   0.00255094,   0.00257098,   0.00259103,   0.00253264,
       0.00247425,   0.00242522,   0.00237619,   0.00235338,   0.00233057,   0.00261103,
       0.00289149,   0.0028024,   0.00271332,   0.00290319,   0.00309306,   0.00350249,
       0.00391192,   0.00395984,   0.00400776,   0.00401089,   0.00401403,   0.0037349,
       0.00345576,   0.00305136,   0.00264696,   0.00241812,   0.00218929,   0.00191393,
       0.00163858,   0.00153304,   0.00142751,   0.00137032,   0.00131312,   0.00134518,
       0.00137723,   0.0012429,   0.00110858,   0.00114136,   0.00117415,   0.00116687,
       0.00115959,   0.00108165,   0.00100371,   0.00131319,   0.00162267,   0.00130164,
       0.000980609,   0.00115412,   0.00132762,   0.0013807,   0.00143378,   0.00218704,
       0.00294031,   0.00234605,   0.00175179,   0.0020462,   0.0023406,   0.00180012,
       0.00125963,   0.00107872,   0.000897814,   0.000448907,   0
      };

      for (unsigned int i = 0; i<275; ++i)
        {
        b0[i] = tmp[i];
        }
      }
    else if (bandName[i].find("NIR") != std::string::npos)
      {
      const float tmp[275] =
      {
       0.000894394,   0.000713315,   0.000532235,   0.000603058,   0.000673881,   0.000760361,
       0.00084684,   0.000787802,   0.000728763,   0.000817305,   0.000905846,   0.000904774,
       0.000903701,   0.00160409,   0.00230447,   0.00221407,   0.00212367,   0.00314219,
       0.00416071,   0.00470014,   0.00523956,   0.00641267,   0.00758578,   0.0090575,
       0.0105292,   0.0156041,   0.0206791,   0.027643,   0.0346069,   0.0395911,
       0.0445753,   0.0408266,   0.0370779,   0.0276002,   0.0181225,   0.017135,
       0.0161476,   0.0168906,   0.0176337,   0.0172355,   0.0168373,   0.0168014,
       0.0167656,   0.0178122,   0.0188588,   0.0165949,   0.0143311,   0.0120729,
       0.0098147,   0.0125599,   0.0153051,   0.0187014,   0.0220978,   0.0244686,
       0.0268393,   0.0270579,   0.0272765,   0.0223541,   0.0174318,   0.0124528,
       0.00747381,   0.00633578,   0.00519774,   0.00546479,   0.00573183,   0.00496663,
       0.00420143,   0.00375248,   0.00330354,   0.00289353,   0.00248353,   0.00252329,
       0.00256304,   0.00431351,   0.00606397,   0.00864978,   0.0112356,   0.0122617,
       0.0132878,   0.0113116,   0.00933533,   0.00834238,   0.00734942,   0.00698854,
       0.00662766,   0.00662135,   0.00661504,   0.00802641,   0.00943779,   0.0113662,
       0.0132947,   0.0126595,   0.0120243,   0.00943568,   0.0068471,   0.00523536,
       0.00362362,   0.00341318,   0.00320274,   0.00318157,   0.00316041,   0.00303014,
       0.00289988,   0.00349364,   0.0040874,   0.00427183,   0.00445627,   0.00425418,
       0.0040521,   0.00432398,   0.00459586,   0.00500329,   0.00541072,   0.0052616,
       0.00511248,   0.00619044,   0.0072684,   0.007209,   0.0071496,   0.00780861,
       0.00846762,   0.0103595,   0.0122514,   0.014548,   0.0168445,   0.0180774,
       0.0193103,   0.0203458,   0.0213814,   0.021739,   0.0220966,   0.0227061,
       0.0233155,   0.0241796,   0.0250438,   0.0249493,   0.0248548,   0.0257403,
       0.0266259,   0.0274829,   0.0283399,   0.0315445,   0.034749,   0.0392491,
       0.0437491,   0.0488567,   0.0539644,   0.0651598,   0.0763552,   0.0905082,
       0.104661,   0.125801,   0.146941,   0.17194,   0.196939,   0.22805,
       0.25916,   0.283963,   0.308767,   0.332943,   0.357119,   0.400422,
       0.443726,   0.504385,   0.565045,   0.633202,   0.701359,   0.76896,
       0.83656,   0.889646,   0.942732,   0.971366,   1,   0.997999,
       0.995998,   0.963345,   0.930691,   0.881401,   0.832111,   0.811504,
       0.790896,   0.782153,   0.773411,   0.768032,   0.762653,   0.735848,
       0.709043,   0.707354,   0.705666,   0.697871,   0.690076,   0.672979,
       0.655882,   0.638978,   0.622073,   0.601538,   0.581002,   0.557459,
       0.533915,   0.524241,   0.514567,   0.501153,   0.487739,   0.452918,
       0.418097,   0.370059,   0.32202,   0.285355,   0.248689,   0.209974,
       0.171259,   0.142537,   0.113816,   0.092517,   0.0712178,   0.0646218,
       0.0580258,   0.050796,   0.0435662,   0.0353664,   0.0271667,   0.0232974,
       0.0194282,   0.0176517,   0.0158752,   0.0144977,   0.0131202,   0.0118952,
       0.0106703,   0.0101536,   0.00963684,   0.00921561,   0.00879438,   0.00794162,
       0.00708886,   0.00654941,   0.00600996,   0.00563429,   0.00525862,   0.00510421,
       0.00494981,   0.00454041,   0.00413101,   0.00410078,   0.00407056,   0.00396389,
       0.00385722,   0.00364911,   0.00344101,   0.00333006,   0.00321911,   0.00300415,
       0.00278918,   0.00317632,   0.00356346,   0.00312642,   0.00268937,   0.00260112,
       0.00251287,   0.00263123,   0.00274958,   0.00362147,   0.00449336,   0.00413978,
       0.0037862,   0.0028968,   0.0020074,   0.00190022,   0.00179303,   0.00227357,
       0.0027541,   0.00137705,   0,   0,   0
      };

      for (unsigned int i = 0; i<275; ++i)
        {
        b0[i] = tmp[i];
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
