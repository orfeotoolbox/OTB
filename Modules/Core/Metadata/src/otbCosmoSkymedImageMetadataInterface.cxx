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
#include "otbCosmoSkymedImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

//useful constants
#include <otbMath.h>

namespace otb
{

CosmoSkymedImageMetadataInterface
::CosmoSkymedImageMetadataInterface()
{
}

bool
CosmoSkymedImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  if (sensorID.find("CSK") != std::string::npos)
    {
    return true;
    }
  else
    return false;
}

void
CosmoSkymedImageMetadataInterface
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
CosmoSkymedImageMetadataInterface::GetYear() const
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
CosmoSkymedImageMetadataInterface::GetMonth() const
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
CosmoSkymedImageMetadataInterface::GetDay() const
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
CosmoSkymedImageMetadataInterface::GetHour() const
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
CosmoSkymedImageMetadataInterface::GetMinute() const
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
CosmoSkymedImageMetadataInterface::GetProductionYear() const
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
CosmoSkymedImageMetadataInterface::GetProductionMonth() const
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
CosmoSkymedImageMetadataInterface::GetProductionDay() const
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
CosmoSkymedImageMetadataInterface::GetPRF() const
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

CosmoSkymedImageMetadataInterface::UIntVectorType
CosmoSkymedImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

double
CosmoSkymedImageMetadataInterface::GetRSF() const
{
  return 0;
}

double
CosmoSkymedImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double
CosmoSkymedImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

} // end namespace otb
