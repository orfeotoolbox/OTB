/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
#include "otbCosmoImageMetadataInterface.h"

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

// useful constants
#include <otbMath.h>
#include <iomanip>
#include "otbSystem.h"
#include "otbXMLMetadataSupplier.h"
#include "gdal_pam.h"


namespace otb
{


bool CosmoImageMetadataInterface::CanRead() const
{
  const std::string sensorID = GetSensorID();

  return sensorID.find("CSK") != std::string::npos;
}

void CosmoImageMetadataInterface::ParseDateTime(std::string key, std::vector<int>& dateFields) const
{
  if (dateFields.empty())
  {
    // parse from keyword list
    if (!this->CanRead())
    {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
    }

    const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
    {
      itkExceptionMacro(<< "no key named " << key);
    }

    const std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "T:-.");
  }
}

int CosmoImageMetadataInterface::GetYear() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
  {
    ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  }

  if (m_AcquisitionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[0], "support_data.image_date:year(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid year");
  }
  return value;
}

int CosmoImageMetadataInterface::GetMonth() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
  {
    ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  }

  if (m_AcquisitionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[1], "support_data.image_date:month(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid month");
  }
  return value;
}

int CosmoImageMetadataInterface::GetDay() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
  {
    ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  }

  if (m_AcquisitionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[2], "support_data.image_date:day(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid day");
  }
  return value;
}

int CosmoImageMetadataInterface::GetHour() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
  {
    ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  }

  if (m_AcquisitionDateFields.size() > 3)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[3], "support_data.image_date:hour(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid hour");
  }
  return value;
}

int CosmoImageMetadataInterface::GetMinute() const
{
  int value = 0;
  if (m_AcquisitionDateFields.empty())
  {
    ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  }

  if (m_AcquisitionDateFields.size() > 4)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[4], "support_data.image_date:minute(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid minute");
  }
  return value;
}

int CosmoImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[0], "support_data.date:year(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production year");
  }
  return value;
}

int CosmoImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[1], "support_data.date:month(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production month");
  }
  return value;
}

int CosmoImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[2], "support_data.date:day(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production day");
  }
  return value;
}

double CosmoImageMetadataInterface::GetPRF() const
{
  double                     value            = 0;
  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
  if (!imageKeywordlist.HasKey("support_data.pulse_repetition_frequency"))
  {
    return value;
  }

  value = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("support_data.pulse_repetition_frequency"),
                                     "support_data.pulse_repetition_frequency(double)");

  return value;
}


CosmoImageMetadataInterface::UIntVectorType CosmoImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

double CosmoImageMetadataInterface::GetRSF() const
{
  return 0;
}

double CosmoImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double CosmoImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}



void CosmoImageMetadataInterface::Parse(const MetadataSupplierInterface *mds)
{

  assert(mds);
  assert(mds->GetNbBands() == this->m_Imd.Bands.size());

  // Check SubDatasets (For COSMO, we need //S01/SBI dataset)
  auto subDsName = "HDF5:" + mds->GetResourceFile() + "://S01/SBI";
  auto ds = (GDALDataset *) GDALOpen(subDsName.c_str(), GA_ReadOnly );
  if (ds)
  {
    std::cout << "Found S01/SBI ! : " << std::endl;
    // Do stuff
    GDALClose(ds);
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,
      << "Cannot find S01/SBI subdataset")
  }

  // Metadata read by GDAL

  Fetch(MDStr::Mission, *mds, "MISSION_ID");
  Fetch(MDStr::Mode, *mds, "Acquisition_Mode");
  Fetch(MDStr::ProductType, *mds, "Product_Type");

  // Check Mission Id, acquisition mode and product type
  if(! (m_Imd[MDStr::Mission] == "CSK" ))
    {
    itkExceptionMacro(<< "Not a valid missionId" << m_Imd[MDStr::Mission] );
    }
  if((m_Imd[MDStr::Mode] != "HIMAGE") &&
        (m_Imd[MDStr::Mode] != "SPOTLIGHT") &&
          (m_Imd[MDStr::Mode] != "ENHANCED SPOTLIGHT"))
    {
    itkExceptionMacro(<< "Not an expected acquisition mode (only HIMAGE and SPOTLIGHT expected)" << m_Imd[MDStr::Mode] );
    }

  if( (m_Imd[MDStr::ProductType] != "SCS_B") && m_Imd[MDStr::ProductType] != "SCS_U")
    {
    itkExceptionMacro(<< "Not an expected product type (only SCS_B and SCS_U expected) " << m_Imd[MDStr::ProductType] );
    }



  m_Imd.Add(MDStr::SensorID, "CSK");
  Fetch(MDStr::Instrument, *mds, "Satellite_ID");

  Fetch(MDStr::OrbitDirection, *mds, "Orbit_Direction");
  bool hasOrbitNumber ;
  std::string orbitNumber =  mds->GetMetadataValue("Orbit_Number", hasOrbitNumber) ;
  m_Imd.Add(MDNum::OrbitNumber, std::stoi(orbitNumber));

  bool hasRadarFrequency ;
  std::string radarFrequency =  mds->GetMetadataValue("Radar_Frequency", hasRadarFrequency) ;
  m_Imd.Add(MDNum::RadarFrequency, std::stod(radarFrequency));

  Fetch(MDStr::Polarization, *mds, "S01_Polarisation") ;
  m_Imd.Add(MDStr::Swath, "S1");


}



} // end namespace otb
