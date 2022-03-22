/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#include "otbSpatialReference.h"

// useful constants
#include <otbMath.h>
#include <iomanip>
#include "otbSystem.h"
#include "otbXMLMetadataSupplier.h"
#include "gdal_pam.h"
#include <iostream>


namespace otb
{

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

  record.azimuthStartTime = MetaData::ReadFormattedDate(firstLineTimeStr, "%Y-%m-%dT%H:%M:%S");
  record.azimuthStopTime = MetaData::ReadFormattedDate(lastLineTimeStr, "%Y-%m-%dT%H:%M:%S");

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
  imd.Add(MDStr::Instrument, "SAR-2000");
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
  auto extension = itksys::SystemTools::GetFilenameExtension(m_MetadataSupplierInterface->GetResourceFile());
  auto subDsName = m_MetadataSupplierInterface->GetResourceFile();
  if (extension  == ".h5")
    {
      subDsName = "HDF5:" + m_MetadataSupplierInterface->GetResourceFile() + "://S01/SBI";
    }
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

  imd.Add(MDTime::AcquisitionStartTime, MetaData::ReadFormattedDate(first_line_time));
  imd.Add(MDTime::AcquisitionStopTime, MetaData::ReadFormattedDate(last_line_time));

  imd.Add(MDNum::LineSpacing, std::stod(metadataBands[0]["S01_SBI_Line_Spacing"]));
  imd.Add(MDNum::PixelSpacing, std::stod(metadataBands[0]["S01_SBI_Column_Spacing"]));

  // Retrieve the product dimension, as it is not stored in the metadata
  auto dataset = static_cast<GDALDataset*>(GDALOpen(subDsName.c_str(), GA_ReadOnly));

  int sizex = dataset->GetRasterXSize();
  int sizey = dataset->GetRasterYSize();

  imd.Add(MDNum::NumberOfLines, static_cast<double>(sizey));
  imd.Add(MDNum::NumberOfColumns, static_cast<double>(sizex));

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

  sarParam.azimuthTimeInterval = MetaData::Duration::Seconds(
      std::stod(metadataBands[0]["S01_SBI_Line_Time_Interval"]));
  sarParam.nearRangeTime = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Range_First_Time"]);
  
  auto lookSide = m_MetadataSupplierInterface->GetAs<std::string>("Look_Side");

  if (lookSide != "RIGHT" && lookSide != "LEFT")
  {
    otbGenericExceptionMacro(MissingMetadataException, "Not an expected look side (only RIGHT or LEFT expected)");
  }

  sarParam.rightLookingFlag = lookSide == "RIGHT";

  // Scene coordinate
  std::vector<std::string> vGeoCoor;
  otb::Utils::ConvertStringToVector(metadataBands[0]["S01_SBI_Top_Left_Geodetic_Coordinates"], vGeoCoor,
      "S01_SBI_Top_Left_Geodetic_Coordinates", " ");
  sarParam.ulSceneCoord.latitude = std::stod(vGeoCoor[0]);
  sarParam.ulSceneCoord.longitude = std::stod(vGeoCoor[1]);
  vGeoCoor.clear();
  otb::Utils::ConvertStringToVector(metadataBands[0]["S01_SBI_Top_Right_Geodetic_Coordinates"], vGeoCoor,
      "S01_SBI_Top_Right_Geodetic_Coordinates", " ");
  sarParam.urSceneCoord.latitude = std::stod(vGeoCoor[0]);
  sarParam.urSceneCoord.longitude = std::stod(vGeoCoor[1]);
  vGeoCoor.clear();
  otb::Utils::ConvertStringToVector(metadataBands[0]["S01_SBI_Bottom_Left_Geodetic_Coordinates"], vGeoCoor,
      "S01_SBI_Bottom_Left_Geodetic_Coordinates", " ");
  sarParam.llSceneCoord.latitude = std::stod(vGeoCoor[0]);
  sarParam.llSceneCoord.longitude = std::stod(vGeoCoor[1]);
  vGeoCoor.clear();
  otb::Utils::ConvertStringToVector(metadataBands[0]["S01_SBI_Bottom_Right_Geodetic_Coordinates"], vGeoCoor,
      "S01_SBI_Bottom_Right_Geodetic_Coordinates", " ");
  sarParam.lrSceneCoord.latitude = std::stod(vGeoCoor[0]);
  sarParam.lrSceneCoord.longitude = std::stod(vGeoCoor[1]);

  // TODO: compute a GCP at the center of scene using the inverse sar model like it is done in ossim plugins
  // This require to move IMIs to another higher level module that depends on OTBTransform (which depends 
  // on OTBMetadata) so that SarSensorModel can be used. (this cannot be done while IMIs still depend on Ossim)

  // Add the top left corner as GCP for now
  std::istringstream output(metadataBands[0]["S01_SBI_Top_Left_Geodetic_Coordinates"]);

  GCP gcp;

  output >> gcp.m_GCPY; // lat
  output >> gcp.m_GCPX; // lon
  output >> gcp.m_GCPZ; // height
  gcp.m_GCPRow = 0;
  gcp.m_GCPCol = 0;
  gcp.m_Id = "1";
  
  Projection::GCPParam gcpParam;
  gcpParam.GCPProjection = SpatialReference::FromWGS84().ToWkt();
  gcpParam.GCPs.push_back(gcp);
  imd.Add(MDGeom::GCP, gcpParam);

  // Left corner => azimuthTime == first_line_time and slantRangeTime == near_range_time
  GCPTime time;
  time.azimuthTime = MetaData::ReadFormattedDate(first_line_time);
  time.slantRangeTime = sarParam.nearRangeTime;
  sarParam.gcpTimes["0"] = time;

  imd.Add(MDGeom::SAR, sarParam);

  SARCalib sarCalib;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStartTime;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStopTime;
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
  imd.Add(MDStr::Instrument, "SAR-2000");
  Fetch(MDStr::OrbitDirection, imd, "support_data.orbit_pass");
  Fetch(MDNum::OrbitNumber, imd, "support_data.abs_orbit");
  Fetch(MDNum::RadarFrequency, imd, "support_data.radar_frequency");
  Fetch(MDStr::Polarization, imd, "header.polarisation");
  imd.Add(MDStr::Swath, "S1");
  Fetch(MDNum::PRF, imd, "support_data.pulse_repetition_frequency");
  Fetch(MDTime::AcquisitionStartTime, imd, "header.first_line_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "header.last_line_time");
  Fetch(MDTime::AcquisitionStartTime, imd, "support_data.first_line_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "support_data.last_line_time");
  Fetch(MDNum::LineSpacing, imd, "support_data.azimuth_spacing");
  Fetch(MDNum::PixelSpacing, imd, "support_data.range_spacing");
  Fetch(MDNum::NumberOfLines, imd, "number_lines");
  Fetch(MDNum::NumberOfColumns, imd, "number_samples");

  //SAR Parameters
  SARParam sarParam;
  sarParam.orbits = this->GetOrbitsGeom();
  sarParam.azimuthFmRates = this->GetAzimuthFmRateGeom();
  sarParam.dopplerCentroids = this->GetDopplerCentroidGeom();

  // support_data.look_side for Cosmo geoms (all versions) and TerraSAR-X geoms (OTB 7.4)
  const std::string supportDataPrefix = "support_data.";
  auto lookSide = m_MetadataSupplierInterface->GetAs<std::string>("",
                                "support_data.look_side");


  sarParam.rightLookingFlag = lookSide == "RIGHT";

  sarParam.rangeSamplingRate = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "range_sampling_rate");

  sarParam.nearRangeTime = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "slant_range_to_first_pixel");

  sarParam.rangeResolution = m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "range_spacing");

  sarParam.azimuthTimeInterval = MetaData::Duration::Seconds(m_MetadataSupplierInterface->GetAs<double>(
                                supportDataPrefix + "line_time_interval") );


  sarParam.burstRecords = CreateBurstRecord(m_MetadataSupplierInterface->GetAs<std::string>(
					     supportDataPrefix + "first_line_time"),
					    m_MetadataSupplierInterface->GetAs<std::string>(
					     supportDataPrefix + "last_line_time"),
					    static_cast<int>(imd[MDNum::NumberOfColumns])-1,
					    static_cast<int>(imd[MDNum::NumberOfLines])-1);

  imd.Add(MDGeom::SAR, sarParam);

  Projection::GCPParam gcp;
  imd.Add(MDGeom::GCP, gcp);

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


  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 0);
  imd.Add(MDNum::BlueDisplayChannel, 0);
}

} // end namespace otb
