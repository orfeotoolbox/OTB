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

double CosmoImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface &) const
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

std::vector<Orbit> CosmoImageMetadataInterface::getOrbits(const std::string & reference_UTC) const
{
    ////////////////// Add Orbit List ////////////////
  bool hasOrbit ;
  std::string nb_orbits =  m_MetadataSupplierInterface->GetMetadataValue("Number_of_State_Vectors", hasOrbit) ;
    // Get elements
    int stateVectorList_size = std::stoi(nb_orbits);
    std::string state_times = m_MetadataSupplierInterface->GetMetadataValue("State_Vectors_Times", hasOrbit);
    std::string ecef_satellite_pos = m_MetadataSupplierInterface->GetMetadataValue("ECEF_Satellite_Position", hasOrbit) ;
    std::string ecef_satellite_vel = m_MetadataSupplierInterface->GetMetadataValue("ECEF_Satellite_Velocity", hasOrbit);
    // Convert std::string to vector
    std::vector<std::string> vTimes;
    otb::Utils::ConvertStringToVector(state_times, vTimes, "State_Vectors_Times", " ");

    std::vector<std::string> vECEF_sat_pos;
    otb::Utils::ConvertStringToVector(ecef_satellite_pos, vECEF_sat_pos, "ECEF_Satellite_Position", " ");
    std::vector<std::string> vECEF_sat_vel;
    otb::Utils::ConvertStringToVector(ecef_satellite_vel, vECEF_sat_vel, "ECEF_Satellite_Velocity", " ");

  std::vector<Orbit> orbitVector;

  // Helper function to convert to a two digit string.
  auto formatTimeInt = [](int i) { return (i < 10 ? "0" + std::to_string(i)
                                                 : std::to_string(i) );};
  auto formatTimeDouble = [](double d) { return (d < 10 ? "0" + std::to_string(d)
                                                       : std::to_string(d) );};

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
    seconds += 0.5;

    std::string timeStr = reference_UTC + "T" + formatTimeInt(hour) + ":" + formatTimeInt(minutes) + ":" + formatTimeDouble(seconds);

    orbit.time = MetaData::ReadFormattedDate(timeStr);

    orbit.position[0] = std::stod(vECEF_sat_pos[i*3 + 0]) ;
    orbit.position[1] = std::stod(vECEF_sat_pos[i*3 + 1]) ;
    orbit.position[2] = std::stod(vECEF_sat_pos[i*3 + 2]) ;

    orbit.velocity[0] = std::stod(vECEF_sat_vel[i*3 + 0]) ;
    orbit.velocity[1] = std::stod(vECEF_sat_vel[i*3 + 1]) ;
    orbit.velocity[2] = std::stod(vECEF_sat_vel[i*3 + 2]) ;

    orbitVector.push_back(orbit);
  }
  return orbitVector;
}



std::vector<BurstRecord> CosmoImageMetadataInterface::CreateBurstRecord(const std::string & firstLineTimeStr, const std::string & lastLineTimeStr, const unsigned long endLine, const unsigned long endSample) const
{
  BurstRecord record;
  record.startLine = 0;
  record.startSample = 0;

  record.endLine = endLine;
  record.endSample = endSample;

  record.azimuthStartTime = MetaData::ReadFormattedDate(firstLineTimeStr, "%Y-%m-%dT%H:%M:%S%F");
  record.azimuthStopTime = MetaData::ReadFormattedDate(lastLineTimeStr, "%Y-%m-%dT%H:%M:%S%F");

  record.azimuthAnxTime = 0.;

  return {record};
}

bool not_in(std::vector<std::string> possible_values, std::string test_value)
{
  return std::none_of(possible_values.begin(), possible_values.end(), [test_value](std::string s){return s == test_value;});
}

void CosmoImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  // Check acquisition mode and product type
  Fetch(MDStr::Mode, imd, "Acquisition_Mode");
  if (not_in({"HIMAGE", "SPOTLIGHT", "ENHANCED SPOTLIGHT"}, imd[MDStr::Mode]))
  {
    otbGenericExceptionMacro(MissingMetadataException, "Not an expected acquisition mode (only HIMAGE and SPOTLIGHT expected)" << imd[MDStr::Mode] );
  }
  
  Fetch(MDStr::ProductType, imd, "Product_Type");
  if (not_in({"SCS_B", "SCS_U"}, imd[MDStr::ProductType]))
  {
    otbGenericExceptionMacro(MissingMetadataException, "Not an expected product type (only SCS_B and SCS_U expected) " << imd[MDStr::ProductType] );
  }

  imd.Add(MDStr::Mission, "CSK");
  imd.Add(MDStr::SensorID, "CSK");
  Fetch(MDStr::OrbitDirection, imd, "Orbit_Direction");
  bool hasOrbitNumber ;
  std::string orbitNumber =  m_MetadataSupplierInterface->GetMetadataValue("Orbit_Number", hasOrbitNumber) ;
  imd.Add(MDNum::OrbitNumber, std::stoi(orbitNumber));

  bool hasRadarFrequency ;
  std::string radarFrequency =  m_MetadataSupplierInterface->GetMetadataValue("Radar_Frequency", hasRadarFrequency) ;
  imd.Add(MDNum::RadarFrequency, std::stod(radarFrequency));

  Fetch(MDStr::Polarization, imd, "S01_Polarisation") ;
  imd.Add(MDStr::Swath, "S1");

  Fetch(MDNum::PRF, imd, "S01_PRF");

  //getTime
  auto subDsName = "HDF5:" + m_MetadataSupplierInterface->GetResourceFile() + "://S01/SBI";
  auto metadataBands = this->saveMetadataBands(subDsName) ;
  bool hasTimeUTC;
  int pos = m_MetadataSupplierInterface->GetMetadataValue("Reference_UTC", hasTimeUTC).find(" ");;
  std::string reference_UTC = m_MetadataSupplierInterface->GetMetadataValue("Reference_UTC", hasTimeUTC).substr(0, pos);

  double total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_First_Time"]);
  int hour = static_cast<int> (total_seconds/3600.0);
  int minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
  double seconds = total_seconds - hour*3600 - minutes*60;


  // Helper function to convert to a two digit string.
  auto formatTimeInt = [](int i) { return (i < 10 ? "0" + std::to_string(i)
                                                 : std::to_string(i) );};
  auto formatTimeDouble = [](double d) { return (d < 10 ? "0" + std::to_string(d)
                                                       : std::to_string(d) );};

  std::string first_line_time = reference_UTC + "T" + formatTimeInt(hour) + ":" + formatTimeInt(minutes) + ":" + formatTimeDouble(seconds); 

  total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_Last_Time"]);
  hour = static_cast<int> (total_seconds/3600.0);
  minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
  seconds = total_seconds - hour*3600 - minutes*60;


  std::string last_line_time = reference_UTC + "T" + formatTimeInt(hour) + ":" + formatTimeInt(minutes) + ":" + formatTimeDouble(seconds);
  MetaData::Time startTime = Utils::LexicalCast<MetaData::Time,std::string>(first_line_time, std::string("T"));
  MetaData::Time stoptTime = Utils::LexicalCast<MetaData::Time,std::string>(last_line_time, std::string("T"));

  imd.Add(MDTime::AcquisitionStartTime, startTime);
  imd.Add(MDTime::AcquisitionStopTime, stoptTime);

  // Retrieve the product dimension, as it is not stored in the metadata
  auto dataset = static_cast<GDALDataset*>(GDALOpen(subDsName.c_str(), GA_ReadOnly));

  int sizex = dataset->GetRasterXSize();
  int sizey = dataset->GetRasterYSize();
  GDALClose(dataset);

  //SAR Parameters
  SARParam sarParam;
  sarParam.orbits = this->getOrbits(reference_UTC);
  sarParam.burstRecords = CreateBurstRecord(first_line_time, last_line_time, sizex-1, sizey-1);

  auto rangeTimeInterval = std::stod(metadataBands[0]["S01_SBI_Column_Time_Interval"]);
  
  if (!rangeTimeInterval)
  {
    otbGenericExceptionMacro(MissingMetadataException, "Range time interval is null.");
  }
  sarParam.rangeSamplingRate = 1 / rangeTimeInterval;

  sarParam.azimuthTimeInterval = MetaData::DurationType(
      std::stod(metadataBands[0]["S01_SBI_Line_Time_Interval"])
      * 1e6);
  sarParam.nearRangeTime = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Range_First_Time"]);

  imd.Add(MDGeom::SAR, sarParam);

  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void CosmoImageMetadataInterface::ParseGeom(ImageMetadata &imd)
{
  // Check acquisition mode and product type
  Fetch(MDStr::Mode, imd, "support_data.acquisition_mode");
  if(not_in({"HIMAGE", "SPOTLIGHT", "ENHANCED SPOTLIGHT"}, imd[MDStr::Mode]))
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Not an expected acquisition mode (only HIMAGE and SPOTLIGHT expected)" << imd[MDStr::Mode] );

  }
  
  Fetch(MDStr::ProductType, imd, "support_data.product_type");
  if(not_in({"SCS_B", "SCS_U"}, imd[MDStr::ProductType]))
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Not an expected product type (only SCS_B and SCS_U expected) " << imd[MDStr::ProductType] );
  }

  imd.Add(MDStr::Mission, "CSK");
  imd.Add(MDStr::SensorID, "CSK");
  Fetch(MDStr::OrbitDirection, imd, "support_data.orbit_pass");
  Fetch(MDNum::OrbitNumber, imd, "support_data.abs_orbit");
  Fetch(MDNum::RadarFrequency, imd, "support_data.radar_frequency");
  Fetch(MDStr::Polarization, imd, "header.polarisation");
  imd.Add(MDStr::Swath, "S1");
  Fetch(MDNum::PRF, imd, "support_data.pulse_repetition_frequency");
  Fetch(MDTime::AcquisitionStartTime, imd, "header.first_line_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "header.last_line_time");

  //SAR Parameters
  SARParam sarParam;
  sarParam.orbits = this->GetOrbitsGeom();
  imd.Add(MDGeom::SAR, sarParam);
  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void CosmoImageMetadataInterface::Parse(ImageMetadata & imd)
{
  assert(m_MetadataSupplierInterface != nullptr && "In ImageMetadataInterface, the MetadataSupplier needs to be set before calling the Parse function.");
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "MISSION_ID") == "CSK")
    this->ParseGdal(imd);
  // Try to fetch the metadata from GEOM file
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "CSK")
    this->ParseGeom(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
             << "Not a CosmoSkyMed product");
}

} // end namespace otb
