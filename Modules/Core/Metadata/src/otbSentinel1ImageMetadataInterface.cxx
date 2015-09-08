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
::GetCalibrationLookupData(const short type)
  {
    bool sigmaLut = false;
    bool betaLut = false;
    bool gammaLut = false;
    bool dnLut = false;

    switch (type)
      {
      case SarCalibrationLookupData::SIGMA:
      {
      sigmaLut = true;
      }
      break;

      case SarCalibrationLookupData::BETA:
      {
      betaLut = true;
      }
      break;

      case SarCalibrationLookupData::GAMMA:
      {
      gammaLut = true;
      }
      break;

      case SarCalibrationLookupData::DN:
      {
      dnLut = true;
      }
      break;

      default:
        sigmaLut = true;
        break;
      }

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
    Utils::ConvertStringToVector(date_time_str, dateFields, " T:-.");
    }

}

int
Sentinel1ImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if(m_AcquisitionDateFields.size() > 0 )
    {
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[0] );
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
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[1] );
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
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[2] );
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
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[3] );
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
    value = boost::lexical_cast<int>( m_AcquisitionDateFields[4] );
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
    value = boost::lexical_cast<int>( m_ProductionDateFields[0] );
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
    value = boost::lexical_cast<int>( m_ProductionDateFields[1] );
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
    value = boost::lexical_cast<int>( m_ProductionDateFields[2] );
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

  value = boost::lexical_cast<double> ( imageKeywordlist.GetMetadataByKey("support_data.pulse_repetition_frequency") );

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
