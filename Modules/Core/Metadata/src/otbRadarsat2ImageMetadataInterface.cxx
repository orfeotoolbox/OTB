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


void
Radarsat2ImageMetadataInterface::
CreateCalibrationLookupData(const short type)
  {
   std::string lut = "SigmaNought";

   switch (type)
     {
     case SarCalibrationLookupData::BETA:
     {
     lut = "BetaNought";
     }
     break;

     case SarCalibrationLookupData::GAMMA:
     {
     lut = "GammaNought";
     }
     break;

     case SarCalibrationLookupData::DN:
     {
     lut = "DN";
     }
     break;

     case SarCalibrationLookupData::SIGMA:
     default:
     {
     lut = "SigmaNought";
     }
     break;
     }

  const ImageKeywordlistType imageKeywordlist =  this->GetImageKeywordlist();
  const std::string key = "referenceNoiseLevel[" + lut + "].gain";

  Radarsat2CalibrationLookupData::GainListType glist;
  int offset = 0;

  Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].gain"), glist, "referenceNoiseLevel[" + lut + "].gain");

  Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].offset"), "referenceNoiseLevel[" + lut + "].offset");

  Radarsat2CalibrationLookupData::Pointer sarLut;
  sarLut = Radarsat2CalibrationLookupData::New();
  sarLut->InitParameters(type, offset, glist);
  this->SetCalibrationLookupData(sarLut);

}

void
Radarsat2ImageMetadataInterface
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
Radarsat2ImageMetadataInterface::GetYear() const
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
Radarsat2ImageMetadataInterface::GetMonth() const
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
Radarsat2ImageMetadataInterface::GetDay() const
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
Radarsat2ImageMetadataInterface::GetHour() const
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
Radarsat2ImageMetadataInterface::GetMinute() const
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
Radarsat2ImageMetadataInterface::GetProductionYear() const
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
Radarsat2ImageMetadataInterface::GetProductionMonth() const
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
Radarsat2ImageMetadataInterface::GetProductionDay() const
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
