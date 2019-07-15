/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbSarImageMetadataInterface.h"
#include "otbSentinel1ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "ossim/ossimTimeUtilities.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimTimeUtilities.h"
#endif

//useful constants
#include <otbMath.h>
#include <iomanip>

namespace otb
{

Sentinel1ImageMetadataInterface
::Sentinel1ImageMetadataInterface()
{
}

bool
Sentinel1ImageMetadataInterface::CanRead() const
{
  const std::string sensorID = GetSensorID();

  return sensorID.find("SENTINEL-1") != std::string::npos;
}

void
Sentinel1ImageMetadataInterface
::CreateCalibrationLookupData(const short type)
{
  bool sigmaLut = false;
  bool betaLut = false;
  bool gammaLut = false;
  bool dnLut = false;

  switch (type)
    {
  case SarCalibrationLookupData::BETA:
      {
      otbMsgDevMacro("betaNought");
      betaLut = true;
      }
    break;

  case SarCalibrationLookupData::GAMMA:
      {
      otbMsgDevMacro("gamma");
      gammaLut = true;
      }
    break;

  case SarCalibrationLookupData::DN:
      {
      otbMsgDevMacro("dn");
      dnLut = true;
      }
    break;

  case SarCalibrationLookupData::SIGMA:
  default:
      otbMsgDevMacro("sigmaNought");
    sigmaLut = true;
    break;
    }

  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();

  // const double firstLineTime = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("calibration.startTime"), "calibration.startTime(double)");

  // const double lastLineTime = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("calibration.stopTime"), "calibration.stopTime(double)");
  using namespace ossimplugins::time;
  const ModifiedJulianDate firstLineTime = toModifiedJulianDate(imageKeywordlist.GetMetadataByKey("calibration.startTime"));
  const ModifiedJulianDate lastLineTime  = toModifiedJulianDate(imageKeywordlist.GetMetadataByKey("calibration.stopTime"));
  otbMsgDevMacro(<<"calibration.startTime: "<<std::setprecision(16) << firstLineTime);
  otbMsgDevMacro(<<"calibration.stopTime : "<<std::setprecision(16) << lastLineTime);

  //const std::string supportDataPrefix = "support_data."; //make && use GetBandPrefix(subSwath, polarisation)

  const int numOfLines = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("number_lines"), "number_lines(int)");
  otbMsgDevMacro(<<"numOfLines   : " << numOfLines);

  const int count = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("calibration.count"), "calibration.count");

  std::vector<Sentinel1CalibrationStruct> calibrationVectorList(count);
  double lastMJD = 0; 

  for(int i = 0; i < count; i++)
    {
    Sentinel1CalibrationStruct calibrationVector;

    std::stringstream prefix;
    prefix << "calibration.calibrationVector[" << i << "].";
    const std::string sPrefix = prefix.str();

    calibrationVector.line = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey(sPrefix + "line"), sPrefix + "line");

    // TODO: don't manipulate doubles, but ModifiedJulianDate for a better type
    // safety
    const std::string sAzimuth = imageKeywordlist.GetMetadataByKey(sPrefix + "azimuthTime");
    calibrationVector.timeMJD =  toModifiedJulianDate(sAzimuth).as_day_frac();
    calibrationVector.deltaMJD = calibrationVector.timeMJD - lastMJD;
    lastMJD = calibrationVector.timeMJD;

    otbMsgDevMacro(<<sPrefix<<"line   : " << calibrationVector.line <<" ;\t"<<sPrefix<<"timeMJD: "<<std::setprecision(16) << calibrationVector.timeMJD << " (" << sAzimuth << ")");

    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "pixel"), calibrationVector.pixels, sPrefix + "pixel");

    // prepare deltaPixels vector
    int prev_pixels = 0;
    calibrationVector.deltaPixels.resize(calibrationVector.pixels.size());
    for (std::size_t p=0, N=calibrationVector.pixels.size(); p!=N ; ++p)
    {
        calibrationVector.deltaPixels[p] = (calibrationVector.pixels[p] - prev_pixels);
        prev_pixels = calibrationVector.pixels[p];
    }

    if (sigmaLut)
      {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "sigmaNought"), calibrationVector.vect, sPrefix + "sigmaNought");
      }

    if (betaLut)
      {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "betaNought"), calibrationVector.vect, sPrefix + "betaNought");
      }

    if (gammaLut)
      {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "gamma"), calibrationVector.vect, sPrefix + "gamma");
      }

    if (dnLut)
      {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "dn"), calibrationVector.vect, sPrefix + "dn");
      }

    calibrationVectorList[i] = calibrationVector;
    }

  Sentinel1CalibrationLookupData::Pointer sarLut = Sentinel1CalibrationLookupData::New();
  sarLut->InitParameters(type, firstLineTime.as_day_frac(), lastLineTime.as_day_frac(), numOfLines, count, calibrationVectorList);
  this->SetCalibrationLookupData(sarLut);
}

void
Sentinel1ImageMetadataInterface
::ParseDateTime(const char* key, std::vector<int>& dateFields) const
{
  if(dateFields.size() < 1 )
    {
    //parse from keyword list
    if (!this->CanRead())
      {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
      }

    const ImageKeywordlistType imageKeywordlist  = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
      {
      itkExceptionMacro( << "no key named " << key );
      }

    const std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "T:-.");
    }
}

int
Sentinel1ImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 0 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[0], "support_data.image_date:year(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid year" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetMonth() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 1 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[1], "support_data.image_date:month(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid month" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetDay() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 2 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[2], "support_data.image_date:day(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid day" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetHour() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 3 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[3], "support_data.image_date:hour(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid hour" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetMinute() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 4 )
    {
    value = Utils::LexicalCast<int>( m_AcquisitionDateFields[4], "support_data.image_date:minute(int)");
    }
  else
    {
    itkExceptionMacro( << "Invalid minute" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 0 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[0], "support_data.date:year(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production year" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 1 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[1], "support_data.date:month(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production month" );
    }
  return value;
}

int
Sentinel1ImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if(m_ProductionDateFields.size() > 2 )
    {
    value = Utils::LexicalCast<int>( m_ProductionDateFields[2], "support_data.date:day(int)" );
    }
  else
    {
    itkExceptionMacro( << "Invalid production day" );
    }
  return value;
}

double
Sentinel1ImageMetadataInterface::GetPRF() const
{
  double value = 0;
  const ImageKeywordlistType imageKeywordlist  = this->GetImageKeywordlist();
  if (!imageKeywordlist.HasKey("support_data.pulse_repetition_frequency"))
    {
    return value;
    }

  value = Utils::LexicalCast<double>( imageKeywordlist.GetMetadataByKey("support_data.pulse_repetition_frequency"),
                                      "support_data.pulse_repetition_frequency(double)" );

  return value;
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

} // end namespace otb
