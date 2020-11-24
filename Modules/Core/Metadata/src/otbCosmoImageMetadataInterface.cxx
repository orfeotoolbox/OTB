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
#include <iostream>


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



std::vector<std::map<std::string, std::string> > CosmoImageMetadataInterface::saveMetadataBands(std::string file)
{
  // Create GDALImageIO to retrieve all metadata (from .h5 input file)
  std::map<std::string, std::string> metadataDataSet;
  std::vector<std::map<std::string, std::string> > metadataBands;
  GDALDataset * dataset = static_cast<GDALDataset*>(GDALOpen(file.c_str(), GA_ReadOnly));
  // Metadata for dataset
  char** papszMetadata = dataset->GetMetadata(nullptr);
  for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
    {
    std::string key, value;
    if (otb::System::ParseHdfSubsetName(papszMetadata[cpt], key, value))
      {
      metadataDataSet[key] = value;
      }
    }

  int nbRasterCount = dataset->GetRasterCount();
  // Metadata for each Band
  for (int iBand = 0; iBand < nbRasterCount; iBand++)
    {
    std::map<std::string, std::string> mapCurrentBand;
    GDALRasterBand * Band = dataset->GetRasterBand(iBand + 1);

    papszMetadata = Band->GetMetadata(nullptr);
    for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
      {
      std::string key, value;
      if (otb::System::ParseHdfSubsetName(papszMetadata[cpt], key, value))
        {
        mapCurrentBand[key] = value;
        }
      }
    metadataBands.push_back(mapCurrentBand);
    }

  GDALClose(static_cast<GDALDatasetH>(dataset));
  return metadataBands;

}




std::vector<Orbit> CosmoImageMetadataInterface::getOrbits(const MetadataSupplierInterface & mds, const std::string & reference_UTC)
{
    ////////////////// Add Orbit List ////////////////
  bool hasOrbit ;
  std::string nb_orbits =  mds.GetMetadataValue("Number_of_State_Vectors", hasOrbit) ;
    // Get elements
    int stateVectorList_size = std::stoi(nb_orbits);
    std::string state_times = mds.GetMetadataValue("State_Vectors_Times", hasOrbit);
    std::string ecef_satellite_pos = mds.GetMetadataValue("ECEF_Satellite_Position", hasOrbit) ;
    std::string ecef_satellite_vel = mds.GetMetadataValue("ECEF_Satellite_Velocity", hasOrbit);
    // Convert std::string to vector
    std::vector<std::string> vTimes;
    otb::Utils::ConvertStringToVector(state_times, vTimes, "State_Vectors_Times", " ");

    std::vector<std::string> vECEF_sat_pos;
    otb::Utils::ConvertStringToVector(ecef_satellite_pos, vECEF_sat_pos, "ECEF_Satellite_Position", " ");
    std::vector<std::string> vECEF_sat_vel;
    otb::Utils::ConvertStringToVector(ecef_satellite_vel, vECEF_sat_vel, "ECEF_Satellite_Velocity", " ");

  std::vector<Orbit> orbitVector;

  std::ostringstream oss;
  for (int i = 0; i != stateVectorList_size ; ++i)
  {
    oss.str("");
    oss << stateVectorList_size;
    Orbit orbit;

    double total_seconds = std::stod(vTimes[i]);
    int hour = static_cast<int> (total_seconds/3600.0);
    int minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
    double seconds = total_seconds - hour*3600 - minutes*60;
    std::string timestr = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    MetaData::Time time = Utils::LexicalCast<MetaData::Time,std::string>(timestr, std::string("T"));

    orbit.time = time ;

    orbit.posX = std::stod(vECEF_sat_pos[i*3 + 0]) ;
    orbit.posY = std::stod(vECEF_sat_pos[i*3 + 1]) ;
    orbit.posZ = std::stod(vECEF_sat_pos[i*3 + 2]) ;

    orbit.velX = std::stod(vECEF_sat_vel[i*3 + 0]) ;
    orbit.velY = std::stod(vECEF_sat_vel[i*3 + 1]) ;
    orbit.velZ = std::stod(vECEF_sat_vel[i*3 + 2]) ;


    orbitVector.push_back(orbit);
  }
  return orbitVector ;
}



void CosmoImageMetadataInterface::Parse(const MetadataSupplierInterface & mds)
{
  assert(mds.GetNbBands() == this->m_Imd.Bands.size());

  // Check Mission Id, acquisition mode and product type
  Fetch(MDStr::Mission, mds, "MISSION_ID");

  if (m_Imd[MDStr::Mission] != "CSK" )
  {
    otbGenericExceptionMacro(MissingMetadataException,
      << "Not a CosmoSkyMed product");
  }

  Fetch(MDStr::Mode, mds, "Acquisition_Mode");
  if((m_Imd[MDStr::Mode] != "HIMAGE") &&
        (m_Imd[MDStr::Mode] != "SPOTLIGHT") &&
          (m_Imd[MDStr::Mode] != "ENHANCED SPOTLIGHT"))
  {
    itkWarningMacro(<< "Not an expected acquisition mode (only HIMAGE and SPOTLIGHT expected)" << m_Imd[MDStr::Mode] );
  }
  
  Fetch(MDStr::ProductType, mds, "Product_Type");
  if( (m_Imd[MDStr::ProductType] != "SCS_B") && m_Imd[MDStr::ProductType] != "SCS_U")
  {
    itkWarningMacro(<< "Not an expected product type (only SCS_B and SCS_U expected) " << m_Imd[MDStr::ProductType] );
  }

  m_Imd.Add(MDStr::SensorID, "CSK");
  Fetch(MDStr::Instrument, mds, "Satellite_ID");

  Fetch(MDStr::OrbitDirection, mds, "Orbit_Direction");
  bool hasOrbitNumber ;
  std::string orbitNumber =  mds.GetMetadataValue("Orbit_Number", hasOrbitNumber) ;
  m_Imd.Add(MDNum::OrbitNumber, std::stoi(orbitNumber));

  bool hasRadarFrequency ;
  std::string radarFrequency =  mds.GetMetadataValue("Radar_Frequency", hasRadarFrequency) ;
  m_Imd.Add(MDNum::RadarFrequency, std::stod(radarFrequency));

  Fetch(MDStr::Polarization, mds, "S01_Polarisation") ;
  m_Imd.Add(MDStr::Swath, "S1");

  bool hasPRF;
  std::string PRFNumber =  mds.GetMetadataValue("S01_PRF", hasPRF);
  m_Imd.Add(MDNum::PRF, std::stoi(PRFNumber));


  //getTime
  auto subDsName = "HDF5:" + mds.GetResourceFile() + "://S01/SBI";
  auto metadataBands = this->saveMetadataBands(subDsName) ;
  bool hasTimeUTC;
  int pos = mds.GetMetadataValue("Reference_UTC", hasTimeUTC).find(" ");;
  std::string reference_UTC = mds.GetMetadataValue("Reference_UTC", hasTimeUTC).substr(0, pos);

  double total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_First_Time"]);
  int hour = static_cast<int> (total_seconds/3600.0);
  int minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
  double seconds = total_seconds - hour*3600 - minutes*60;
  std::string first_line_time = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds); 

  total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_Last_Time"]);
  hour = static_cast<int> (total_seconds/3600.0);
  minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
  seconds = total_seconds - hour*3600 - minutes*60;
  std::string last_line_time = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
  MetaData::Time startTime = Utils::LexicalCast<MetaData::Time,std::string>(first_line_time, std::string("T"));
  MetaData::Time stoptTime = Utils::LexicalCast<MetaData::Time,std::string>(last_line_time, std::string("T"));

  m_Imd.Add(MDTime::AcquisitionStartTime, startTime);
  m_Imd.Add(MDTime::AcquisitionStopTime, stoptTime);

  //SAR Parameters
  SARParam sarParam;
  sarParam.orbits = this->getOrbits(mds, reference_UTC);

  m_Imd.Bands[0].Add(MDGeom::SAR, sarParam);

}








} // end namespace otb
