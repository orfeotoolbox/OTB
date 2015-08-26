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
#include "otbSentinel1ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

//useful constants
#include <otbMath.h>

namespace otb
{

Sentinel1ImageMetadataInterface
::Sentinel1ImageMetadataInterface()
{
}

bool
Sentinel1ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  if (sensorID.find("SENTINEL-1") != std::string::npos)
    {
    return true;
    }
  else
    return false;
}

SarCalibrationLookupData*
Sentinel1ImageMetadataInterface
::GetCalibrationLookupData(const std::string type)
  {
    bool sigmaLut = false;
    bool betaLut = false;
    bool gammaLut = false;
    bool dnLut = false;

    if( type == "beta")
      betaLut = true;
    else if( type == "gamma")
      gammaLut = true;
    else if( type == "dn")
      dnLut = true;
    else
      sigmaLut = true;

  const ImageKeywordlistType imageKeywordlist =  this->GetImageKeywordlist();

  const double firstLineTime = boost::lexical_cast<double>(imageKeywordlist.GetMetadataByKey("calibration.startTime"));

  const double lastLineTime = boost::lexical_cast<double>(imageKeywordlist.GetMetadataByKey("calibration.stopTime"));

  const std::string bandPrefix = "Band[0]."; //make && use GetBandPrefix(subSwath, polarisation)

  const int numOfLines = boost::lexical_cast<int>(imageKeywordlist.GetMetadataByKey(bandPrefix + "number_lines"));

  const int count = boost::lexical_cast<int>(imageKeywordlist.GetMetadataByKey("calibration.count"));

  std::vector<Sentinel1CalibrationStruct> calibrationVectorList(count);

  for(int i = 0; i < count; i++)
    {
    Sentinel1CalibrationStruct calibrationVector;

    std::stringstream prefix;
    prefix << "calibration.calibrationVector[" << i << "].";

    calibrationVector.line = boost::lexical_cast<int>(imageKeywordlist.GetMetadataByKey(prefix.str() + "line"));

    calibrationVector.timeMJD =  boost::lexical_cast<double>(imageKeywordlist.GetMetadataByKey(prefix.str() + "azimuthTime"));

    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(prefix.str() + "pixel"), calibrationVector.pixels);

    if (sigmaLut) {
    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(prefix.str() + "sigmaNought"), calibrationVector.vect);
      }

    if (betaLut) {
    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(prefix.str() + "betaNought"), calibrationVector.vect);
    }

    if (gammaLut) {
    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(prefix.str() + "gamma"), calibrationVector.vect);
    }

    if (dnLut) {
    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(prefix.str() + "dn"), calibrationVector.vect);
    }

    calibrationVectorList[i] = calibrationVector;

    }

  return (new Sentinel1CalibrationLookupData(type, firstLineTime, lastLineTime, numOfLines, count, calibrationVectorList));

  }

int
Sentinel1ImageMetadataInterface::GetDay() const
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
Sentinel1ImageMetadataInterface::GetMonth() const
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
Sentinel1ImageMetadataInterface::GetYear() const
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
    value = boost::lexical_cast<int> (outputValues[0]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Year");
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetHour() const
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
    value = boost::lexical_cast<int> (outputValues[3]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Hour");
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetMinute() const
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
    value = boost::lexical_cast<int> (outputValues[4]);
    }
  catch (boost::bad_lexical_cast &)
    {
    itkExceptionMacro(<< "Invalid Minute");
    }
  return value;
}

double
Sentinel1ImageMetadataInterface::GetPRF() const
{
  return 0;
}

double
Sentinel1ImageMetadataInterface::GetRSF() const
{
  return 0;
}

double
Sentinel1ImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double
Sentinel1ImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

int
Sentinel1ImageMetadataInterface::GetProductionDay() const
{
  return 0;
}

int
Sentinel1ImageMetadataInterface::GetProductionMonth() const
{
  return 0;
}

int
Sentinel1ImageMetadataInterface::GetProductionYear() const
{
  return 0;
}

Sentinel1ImageMetadataInterface::UIntVectorType
Sentinel1ImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

} // end namespace otb
