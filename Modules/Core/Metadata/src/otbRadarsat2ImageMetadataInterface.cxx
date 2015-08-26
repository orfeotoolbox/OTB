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

#include "otbSarImageMetadataInterface.h"
#include "otbRadarsat2ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

//useful constants
#include <otbMath.h>

namespace otb
{

Radarsat2ImageMetadataInterface
::Radarsat2ImageMetadataInterface()
{
}

bool
Radarsat2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  if (sensorID.find("RADARSAT-2") != std::string::npos)
    {
    return true;
    }
  else
    return false;
}

SarCalibrationLookupData*
Radarsat2ImageMetadataInterface::
GetCalibrationLookupData(std::string type)
  {
   std::string lut = "SigmaNought";

  if (type == "sigma")
    {
    lut = "SigmaNought";
    }
  else if(type == "beta")
    {
    lut = "BetaNought";
    }
  else if(type == "gamma")
    {
    lut = "GammaNought";
    }
  else if(type == "dn")
    {
    lut = "DN";
    }
  else
    {
    itkExceptionMacro( << "unknown calibration type");
    }

  const ImageKeywordlistType imageKeywordlist =  this->GetImageKeywordlist();
  const std::string key = "referenceNoiseLevel[" + lut + "].gain";
  Radarsat2CalibrationLookupData *lutData = new Radarsat2CalibrationLookupData(type);
  Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].gain"), lutData->list);
//  lutData->offset = boost::lexical_cast<int>(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].offset"));

  return lutData;
}

int
Radarsat2ImageMetadataInterface::GetDay() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, not a sentinel-1 Image");
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
    value = boost::lexical_cast<int> (outputValues[2]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Day");
    }

  return value;
}

int
Radarsat2ImageMetadataInterface::GetMonth() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, not a sentinel-1 Image");
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
    value = boost::lexical_cast<int> (outputValues[1]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Month");
    }
  return value;
}

int
Radarsat2ImageMetadataInterface::GetYear() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, not a Radarsat2 Image");
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
    value = boost::lexical_cast<int> (outputValues[0]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}

int
Radarsat2ImageMetadataInterface::GetHour() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, not a Radarsat2 Image");
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
    value = boost::lexical_cast<int> (outputValues[3]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Hour");
    }
  return value;
}

int
Radarsat2ImageMetadataInterface::GetMinute() const
{
  const MetaDataDictionaryType& dict = this->GetMetaDataDictionary();
  if (!this->CanRead())
    {
    itkExceptionMacro(<< "Invalid Metadata, not a Radarsat2 Image");
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
    value = boost::lexical_cast<int> (outputValues[4]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Minute");
    }
  return value;
}

double
Radarsat2ImageMetadataInterface::GetPRF() const
{
  return 0;
}

double
Radarsat2ImageMetadataInterface::GetRSF() const
{
  return 0;
}

double
Radarsat2ImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double
Radarsat2ImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

int
Radarsat2ImageMetadataInterface::GetProductionDay() const
{
  return 0;
}

int
Radarsat2ImageMetadataInterface::GetProductionMonth() const
{
  return 0;
}

int
Radarsat2ImageMetadataInterface::GetProductionYear() const
{
  return 0;
}

Radarsat2ImageMetadataInterface::UIntVectorType
Radarsat2ImageMetadataInterface::
GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

} // end namespace otb
