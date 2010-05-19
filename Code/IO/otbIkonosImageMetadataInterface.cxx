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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbIkonosImageMetadataInterface.h"
#include "itkMetaDataObject.h"
#include "base/ossimKeywordlist.h"

namespace otb
{

IkonosImageMetadataInterface
::IkonosImageMetadataInterface()
{
}

bool
IkonosImageMetadataInterface::CanRead(const MetaDataDictionaryType& dict) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("IKONOS-2") != std::string::npos) return true;
  else return false;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface::GetSolarIrradiance(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  ossimKeywordlist         kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  outputValuesVariableLengthVector.SetSize(1);
  // values from geoeye
  std::string key = "support_data.band_name";
  ossimString keywordString = kwl.find(key.c_str());
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
IkonosImageMetadataInterface::GetDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.acquisition_date";
  separatorList = "-";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  // YYYY/MM/DD
  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
IkonosImageMetadataInterface::GetMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.acquisition_date";
  separatorList = "-";
  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  // YYYY/MM/DD
  ossimString month = keywordStrings[1];

  return month.toInt();
}

int
IkonosImageMetadataInterface::GetHour(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.acquisition_time";
  separatorList = ":";
  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 1) itkExceptionMacro(<< "Invalid Hour");

  // HH:MM
  ossimString hour = keywordStrings[0];

  return hour.toInt();
}

int
IkonosImageMetadataInterface::GetMinute(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.acquisition_time";
  separatorList = ":";
  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 1) itkExceptionMacro(<< "Invalid Minute");

  // HH:MM
  ossimString minute = keywordStrings[1];

  return minute.toInt();
}

int
IkonosImageMetadataInterface::GetYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.acquisition_date";
  separatorList = "-";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  // YYYY/MM/DD
  ossimString year = keywordStrings[0];

  return year.toInt();
}

int
IkonosImageMetadataInterface::GetProductionDay(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.production_date";
  separatorList = "/";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Day");

  // MM/DD/YY
  ossimString day = keywordStrings[1];

  return day.toInt();
}

int
IkonosImageMetadataInterface::GetProductionMonth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.production_date";
  separatorList = "/";
  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro(<< "Invalid Month");

  // MM/DD/YY
  ossimString month = keywordStrings[0];

  return month.toInt();
}

int
IkonosImageMetadataInterface::GetProductionYear(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;

  key = "support_data.production_date";
  separatorList = "/";

  ossimString              keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if (keywordStrings.size() <= 2) itkExceptionMacro("Invalid Year");

  // MM/DD/YY
  int year = keywordStrings[2].toInt();

  if (year == 99) year += 1900;
  else year += 2000;

  return year;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalBias(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(GetNumberOfBands(dict));
  outputValuesVariableLengthVector.Fill(0.0);

  return outputValuesVariableLengthVector;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalGain(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  int  productionYear = this->GetProductionYear(dict);
  int  productionMonth = this->GetProductionMonth(dict);
  int  productionDay = this->GetProductionDay(dict);
  bool isPost20010122 = false;
  if ((productionYear > 2) || (productionYear < 99)) isPost20010122 = true;
  else
    {
    if (productionYear == 2)
      {
      if (productionMonth > 1) isPost20010122 = true;
      else
      if (productionDay >= 22) isPost20010122 = true;
      }
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

  std::vector<std::string> bandName = GetBandName(dict);

  VariableLengthVectorType outputValuesVariableLengthVector;
  unsigned int             numBands = GetNumberOfBands(dict);

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
IkonosImageMetadataInterface::GetSatElevation(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.nominal_collection_elevation_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

double
IkonosImageMetadataInterface::GetSatAzimuth(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
    {
    itkExceptionMacro(<< "Invalid Metadata, no Ikonos Image");
    }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
    }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.nominal_collection_azimuth_angle";
  ossimString keywordString = kwl.find(key.c_str());

  return keywordString.toDouble();
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetFirstWavelengths(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
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

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.number_bands";
  int         nbBands = ossimString(kwl.find(key.c_str())).toInt();

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
::GetLastWavelengths(const MetaDataDictionaryType& dict) const
{
  if (!this->CanRead(dict))
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

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "support_data.number_bands";
  int         nbBands = ossimString(kwl.find(key.c_str())).toInt();

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

} // end namespace otb
