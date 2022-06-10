/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include "otbDimapMetadataHelper.h"
#include "itkPoint.h"
#include "itkPointSet.h"

namespace otb
{

void DimapMetadataHelper::ParseGeom(const MetadataSupplierInterface & mds)
{
  // Read a vector value from 
  auto readVector = [&mds](const std::string & path) 
  {
    auto strMetadata = mds.GetAs<std::string>(path);
    strMetadata.erase(std::remove(strMetadata.begin(), strMetadata.end(), '\"'), strMetadata.end());
    
    std::istringstream is( strMetadata );
    return std::vector<double>(( std::istream_iterator<double>( is ) ), 
                                ( std::istream_iterator<double>() ) );
  };

  m_Data.mission = mds.GetAs<std::string>("sensor");
  auto sensorName = mds.GetAs<std::string>("sensor");
  std::vector<std::string> parts;
  boost::split(parts, sensorName, boost::is_any_of(" "));
  m_Data.mission = parts[0];
  if (parts.size() > 1)
  {
    m_Data.missionIndex = parts[1];
  }
  
  m_Data.ImageID = mds.GetAs<std::string>("support_data.image_id");
  m_Data.ProcessingLevel = mds.GetAs<std::string>("", "support_data.processing_level");

  // Band name list might not be present in the dimap
  auto bandNameListStr = mds.GetAs<std::string>("", "support_data.band_name_list");
  if (!bandNameListStr.empty())
  {
    bandNameListStr.erase(std::remove(bandNameListStr.begin(), bandNameListStr.end(), '\"'), bandNameListStr.end());
    boost::trim(bandNameListStr);
    boost::split(m_Data.BandIDs, bandNameListStr, boost::is_any_of(" "));
  }

  m_Data.ProductionDate = mds.GetAs<std::string>("support_data.production_date");
  
  // Convert YYYY-MM-DD HH:mm:ss to YYYY-MM-DDTHH:mm:ss
  auto pos = m_Data.ProductionDate.find(" ",10);
  if (pos != std::string::npos)
  {
    m_Data.ProductionDate.replace(pos,1,"T");
  }

  m_Data.AcquisitionDate = mds.GetAs<std::string>("support_data.image_date");
  
  pos = m_Data.AcquisitionDate.find(" ",10);
  if (pos != std::string::npos)
  {
    m_Data.AcquisitionDate.replace(pos,1,"T");
  }
  
  m_Data.Instrument = mds.GetAs<std::string>("support_data.instrument");
  m_Data.InstrumentIndex = mds.GetAs<std::string>("support_data.instrument_index");
  
  m_Data.SunAzimuth = readVector("support_data.azimuth_angle");
  m_Data.SunElevation = readVector("support_data.elevation_angle");
  
  m_Data.IncidenceAngle = readVector("support_data.incident_angle");
  m_Data.SceneOrientation = readVector("support_data.scene_orientation");

  m_Data.StepCount = mds.GetAs<int>(0, "support_data.step_count");

  m_Data.PhysicalGain = readVector("support_data.physical_gain");
  m_Data.PhysicalBias = readVector("support_data.physical_bias");
  m_Data.SolarIrradiance = readVector("support_data.solar_irradiance");


  m_Data.softwareVersion = mds.GetAs<std::string>("", "support_data.software_version");
  m_Data.SatAzimuth = mds.GetAs<double>(0, "support_data.sat_azimuth_angle");
  auto acrossTrackViewingAngle = mds.GetAs<std::string>("", "viewing_angle_across_track");
  auto alongTrackViewingAngle = mds.GetAs<std::string>("", "viewing_angle_along_track");

  if (!acrossTrackViewingAngle.empty())
  {
    m_Data.AcrossTrackViewingAngle.push_back(std::stod("acrossTrackViewingAngle"));
  }
  
  if (!alongTrackViewingAngle.empty())
  {
    m_Data.AlongTrackViewingAngle.push_back(std::stod("alongTrackViewingAngle"));
  }

  if (mds.HasValue("support_data.along_track_incidence_angle"))
  {
    m_Data.AlongTrackIncidenceAngle = readVector("support_data.along_track_incidence_angle");
  }

  if (mds.HasValue("support_data.across_track_incidence_angle"))
  {
    m_Data.AcrossTrackIncidenceAngle = readVector("support_data.across_track_incidence_angle");
  }

  // These metadata are specific to PHR sensor products
  if (m_Data.mission == "PHR" && m_Data.ProcessingLevel == "SENSOR")
  {
    m_Data.TimeRangeStart = mds.GetAs<std::string>("support_data.time_range_start");
    m_Data.TimeRangeEnd = mds.GetAs<std::string>("support_data.time_range_end");
    m_Data.LinePeriod = mds.GetAs<std::string>("support_data.line_period");
    m_Data.SwathFirstCol = mds.GetAs<std::string>("support_data.swath_first_col");
    m_Data.SwathLastCol = mds.GetAs<std::string>("support_data.swath_last_col");
  }
}

void DimapMetadataHelper::ParseDimapV1(const MetadataSupplierInterface & mds, const std::string prefix)
{
  std::vector<double> defaultValue = {};
  std::vector<std::string> defaultValueStr = {};

  std::vector<std::string> missionVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Information"
                  ,"Scene_Source.MISSION", missionVec);
  m_Data.mission = missionVec[0];

  std::vector<std::string> missionIndexVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Information"
                  ,"Scene_Source.MISSION_INDEX", missionIndexVec);
  m_Data.missionIndex = missionIndexVec[0];

  ParseVector(mds, prefix + "Image_Interpretation.Spectral_Band_Info",
                     "BAND_DESCRIPTION", m_Data.BandIDs, defaultValueStr);

  ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.SUN_ELEVATION", m_Data.SunElevation);

  ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.SUN_AZIMUTH", m_Data.SunAzimuth);

  ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.INCIDENCE_ANGLE", m_Data.IncidenceAngle, defaultValue);

  // Try SATELLITE_INCIDENCE_ANGLE instead
  if (m_Data.IncidenceAngle.empty())
  {
    ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.SATELLITE_INCIDENCE_ANGLE", m_Data.IncidenceAngle);
  }

  ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.VIEWING_ANGLE_ALONG_TRACK", m_Data.AlongTrackViewingAngle, defaultValue);
  
  ParseVector(mds, prefix + "Dataset_Sources.Source_Information",
                     "Scene_Source.VIEWING_ANGLE_ACROSS_TRACK", m_Data.AcrossTrackViewingAngle, defaultValue);


  ParseVector(mds, prefix + "Data_Strip.Sensor_Calibration.Calibration.Band_Parameters",
                     "Gain_Section_List.Gain_Section.PHYSICAL_BIAS", m_Data.PhysicalBias, defaultValue);

  // Try Image_Interpretation.Spectral_Band_Info_i.PHYSICAL_BIAS instead
  if (m_Data.PhysicalBias.empty())
  {
    ParseVector(mds, prefix + "Image_Interpretation.Spectral_Band_Info",
                     "PHYSICAL_BIAS", m_Data.PhysicalBias);
  }

  ParseVector(mds, prefix + "Data_Strip.Sensor_Calibration.Calibration.Band_Parameters",
                     "Gain_Section_List.Gain_Section.PHYSICAL_GAIN", m_Data.PhysicalGain, defaultValue);
  
  // Try Image_Interpretation.Spectral_Band_Info_i.PHYSICAL_GAIN instead
  if (m_Data.PhysicalGain.empty())
  {
    ParseVector(mds, prefix + "Image_Interpretation.Spectral_Band_Info",
                     "PHYSICAL_GAIN", m_Data.PhysicalGain);
  }


  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Solar_Irradiance",
                     "VALUE" , m_Data.SolarIrradiance, defaultValue);

  ParseVector(mds, prefix + "Data_Strip.Sensor_Calibration.Solar_Irradiance.Band_Solar_Irradiance",
                     "SOLAR_IRRADIANCE_VALUE" , m_Data.SolarIrradiance, defaultValue);

  ParseVector(mds, prefix + "Dataset_Frame",
                     "SCENE_ORIENTATION" , m_Data.SceneOrientation);

  std::string path = prefix + "Production.JOB_ID";
  m_Data.ImageID =mds.GetAs<std::string>(path);
  
  path = prefix + "Production.DATASET_PRODUCTION_DATE";

  m_Data.ProductionDate = mds.GetAs<std::string>(path);
  
  // Convert YYYY-MM-DD HH:mm:ss to YYYY-MM-DDTHH:mm:ss
  auto pos = m_Data.ProductionDate.find(" ",10);
  if (pos != std::string::npos)
  {
    m_Data.ProductionDate.replace(pos,1,"T");
  }

  std::vector<std::string> imagingDateVec;

  m_Data.AcquisitionDate = mds.GetAs<std::string>("", prefix + "Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_TIME");

  // If SCENE_CENTER_TIME is not present, try /Dimap_Document/Data_Strip/Time_Stamp/REFERENCE_TIME instead (e.g. Formosat)
  if (m_Data.AcquisitionDate.empty())
  {
    m_Data.AcquisitionDate = mds.GetAs<std::string>("", prefix + "Data_Strip.Time_Stamp.REFERENCE_TIME");
    
    pos = m_Data.AcquisitionDate.find(" ",10);
    if (pos != std::string::npos)
    {
      m_Data.AcquisitionDate.replace(pos,1,"T");
    }
  }

  m_Data.Instrument = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.INSTRUMENT" );
  m_Data.InstrumentIndex = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.INSTRUMENT_INDEX" );

  m_Data.ProcessingLevel = mds.GetAs<std::string>
    (prefix + "Data_Processing.PROCESSING_LEVEL");

  // Metadata specific to spot 5
  if (m_Data.mission == "SPOT" && m_Data.missionIndex == "5")
  {
    m_Data.StepCount = mds.GetAs<int>
          (prefix + "Data_Strip.Sensor_Configuration.Mirror_Position.STEP_COUNT");
  }

  // Metadata speific to formosat
  if (m_Data.mission == "FORMOSAT" && m_Data.missionIndex == "2")
  {
    m_Data.softwareVersion = mds.GetAs<std::string>
          (prefix + "Production.Production_Facility.SOFTWARE_VERSION");
  
    m_Data.SatAzimuth = mds.GetAs<double>
          (prefix + "Dataset_Sources.Source_Information.Scene_Source.SATELLITE_AZIMUTH_ANGLE");
  }
}



void DimapMetadataHelper::ParseDimapV2(const MetadataSupplierInterface & mds, const std::string & prefix)
{
  std::vector<std::string> missionVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Identification"
                  ,"Strip_Source.MISSION", missionVec);
  m_Data.mission = missionVec[0];

  std::vector<std::string> missionIndexVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Identification"
                  ,"Strip_Source.MISSION_INDEX", missionIndexVec);
  m_Data.missionIndex = missionIndexVec[0];

  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "BAND_ID", m_Data.BandIDs);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Solar_Incidences.SUN_ELEVATION", m_Data.SunElevation);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Solar_Incidences.SUN_AZIMUTH", m_Data.SunAzimuth);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE", m_Data.IncidenceAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE_ALONG_TRACK", m_Data.AlongTrackIncidenceAngle);
  
  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE_ACROSS_TRACK", m_Data.AcrossTrackIncidenceAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.VIEWING_ANGLE", m_Data.ViewingAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.AZIMUTH_ANGLE", m_Data.AzimuthAngle);

  std::vector<double> gainbiasUnavail={};
  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "BIAS", m_Data.PhysicalBias,gainbiasUnavail);

  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "GAIN", m_Data.PhysicalGain,gainbiasUnavail);
  
  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Solar_Irradiance",
                     "VALUE" , m_Data.SolarIrradiance);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.AZIMUTH_ANGLE" , m_Data.SceneOrientation);

  std::string path = prefix + "Product_Information.Delivery_Identification.JOB_ID";
  m_Data.ImageID =mds.GetAs<std::string>(path);
  
  path = prefix + "Product_Information.Delivery_Identification.PRODUCTION_DATE";

  m_Data.ProductionDate = mds.GetAs<std::string>(path);

  auto imagingDate = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_DATE" );
  auto imagingTime = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_TIME" );
  m_Data.AcquisitionDate = imagingDate + "T" + imagingTime;

  m_Data.Instrument = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT" );
  m_Data.InstrumentIndex = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT_INDEX" );

  m_Data.ProcessingLevel = mds.GetAs<std::string>
    (prefix + "Processing_Information.Product_Settings.PROCESSING_LEVEL");
  m_Data.SpectralProcessing = mds.GetAs<std::string>
    (prefix + "Processing_Information.Product_Settings.SPECTRAL_PROCESSING");

  // These metadata are specific to PHR sensor products
  if (m_Data.mission == "PHR" && m_Data.ProcessingLevel == "SENSOR")
  {
    m_Data.TimeRangeStart = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Time.Time_Range.START");
    m_Data.TimeRangeEnd = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Time.Time_Range.END");
    m_Data.LinePeriod = mds.GetAs<std::string>(prefix +"Geometric_Data.Refined_Model.Time.Time_Stamp.LINE_PERIOD");
    m_Data.SwathFirstCol = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Geometric_Calibration.Instrument_Calibration.Swath_Range.FIRST_COL");
    m_Data.SwathLastCol = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Geometric_Calibration.Instrument_Calibration.Swath_Range.LAST_COL");
  }
}

void DimapMetadataHelper::ParseSpot5Model(const MetadataSupplierInterface & mds, Spot5Param& spot5Param, const std::string & prefix){

  using Point3DType = itk::Point<double, 3>;
  
  std::vector<double> yaw_vector;
  std::vector<double> pitch_vector;
  std::vector<double> roll_vector;
  std::vector<std::string> time_vector;
  std::string resRoi;
  bool hasValue;

  /* RefLineTime and RefLineTime */
  // acquisitionDate convert to date since 2002 (spot5 launch)
  spot5Param.RefLineTime = GetTime(mds.GetMetadataValue(prefix + "Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_TIME", hasValue));
  spot5Param.RefLineTimeLine = stoi(mds.GetMetadataValue(prefix + "Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_LINE", hasValue)) - 1.0;
  spot5Param.LineSamplingPeriod = std::stod(mds.GetMetadataValue(prefix + "Data_Strip.Sensor_Configuration.Time_Stamp.LINE_PERIOD", hasValue));

  /* ImageSize */
  // If SWIR /2 size?
  spot5Param.ImageSize[0] = stoi(mds.GetMetadataValue(prefix + "Raster_Dimensions.NCOLS", hasValue));
  spot5Param.ImageSize[1] = stoi(mds.GetMetadataValue(prefix + "Raster_Dimensions.NROWS", hasValue));

  /* SubImageOffset */
  resRoi = mds.GetMetadataValue(prefix + "Data_Processing.Regions_Of_Interest.Region_Of_Interest.COL_MIN", hasValue);
  if (hasValue) {
    spot5Param.SubImageOffset[0] = std::stod(resRoi) - 1.0;
  }
  else {
    spot5Param.SubImageOffset[0] = 0.0;
  }
  resRoi = mds.GetMetadataValue(prefix + "Data_Processing.Regions_Of_Interest.Region_Of_Interest.ROW_MIN", hasValue);  
  if (hasValue) {
    spot5Param.SubImageOffset[1] = std::stod(resRoi) - 1.0;
  }
  else {
    spot5Param.SubImageOffset[1] = 0.0;
  }

  /* Satellite Attitudes */

  // Cas Corrected Attitude or Raw Attitude
  std::string expr_attitude = prefix + "Data_Strip.Satellite_Attitudes.Corrected_Attitudes.Corrected_Attitude.Angles";

  ParseVector(mds, expr_attitude,"YAW", yaw_vector);
  if(yaw_vector.size() == 0)
  {
    // take raw attitude instead of corrected
    expr_attitude = prefix + "Data_Strip.Satellite_Attitudes.Raw_Attitudes.Aocs_Attitude.Angles_List.Angles";
    ParseVector(mds, expr_attitude,"YAW", yaw_vector);
  }
  ParseVector(mds, expr_attitude,"PITCH", pitch_vector);
  ParseVector(mds, expr_attitude,"ROLL", roll_vector);     
  ParseVector(mds, expr_attitude,"TIME", time_vector);  

  for (size_t i=0; i < yaw_vector.size(); i++){
    Point3DType point3d;
    point3d[0] = pitch_vector[i];
    point3d[1] = roll_vector[i];
    point3d[2] = yaw_vector[i];
    spot5Param.AttitudesSamples.push_back(std::move(point3d));
    double sampletime = GetTime(time_vector[i]);
    spot5Param.AttitudesSamplesTimes.push_back(sampletime);
  }

  if (yaw_vector.empty()||pitch_vector.empty()||roll_vector.empty()||time_vector.empty())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Missing attitudes information");
  }

  /* Look Angles */

  // Use look angles from Green band
  // /!\ Warning check condition with SWIR band not clear in OSSIM!
  bool bandFound = false;
  std::string expr;

  hasValue = false;

  if (m_Data.BandIDs.size()>1){
    size_t i = 1;
    while (i < m_Data.BandIDs.size() && !bandFound) {
      expr = prefix + "Data_Strip.Sensor_Configuration.Instrument_Look_Angles_List.Instrument_Look_Angles_"+std::to_string(i)+".BAND_INDEX";
      mds.GetMetadataValue(expr, hasValue) == "2" ? bandFound=true : i++;
    }
    expr = "Dimap_Document.Data_Strip.Sensor_Configuration.Instrument_Look_Angles_List.Instrument_Look_Angles_"+std::to_string(i)+".Look_Angles_List.Look_Angles";           
  }
  else{
    expr = "Dimap_Document.Data_Strip.Sensor_Configuration.Instrument_Look_Angles_List.Instrument_Look_Angles.Look_Angles_List.Look_Angles";           
  }
  
  ParseVector(mds, expr, "PSI_X", spot5Param.PixelLookAngleX);  
  ParseVector(mds, expr, "PSI_Y", spot5Param.PixelLookAngleY); 

  std::vector<double> pos_x, pos_y, pos_z, vel_x, vel_y, vel_z;

  expr = prefix + "Data_Strip.Ephemeris.Points.Point";

  ParseVector(mds, expr,"Location.X", pos_x);
  ParseVector(mds, expr,"Location.Y", pos_y);
  ParseVector(mds, expr,"Location.Z", pos_z);   
  ParseVector(mds, expr,"Velocity.X", vel_x);
  ParseVector(mds, expr,"Velocity.Y", vel_y);                     
  ParseVector(mds, expr,"Velocity.Z", vel_z);
  ParseVector(mds, expr,"TIME", time_vector);

  for (size_t i = 0; i < pos_x.size(); i++) {
    Point3DType position;
    Point3DType velocity;
    position[0] = pos_x[i];
    position[1] = pos_y[i];
    position[2] = pos_z[i];
    velocity[0] = vel_x[i];
    velocity[1] = vel_y[i];
    velocity[2] = vel_z[i];
    double sampletime = GetTime(time_vector[i]);

    spot5Param.EcefPosSamples.push_back(std::move(position));
    spot5Param.EcefVelSamples.push_back(std::move(velocity));
    spot5Param.EcefTimeSamples.push_back(sampletime);
  }

}

} // end namespace otb
void DimapMetadataHelper::ParseDimapV3(const MetadataSupplierInterface & mds, const std::string & prefix)
{
  std::vector<std::string> missionVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Identification"
                  ,"Strip_Source.MISSION", missionVec);
  m_Data.mission = missionVec[0];

  std::vector<std::string> missionIndexVec;
  ParseVector(mds, prefix + "Dataset_Sources.Source_Identification"
                  ,"Strip_Source.MISSION_INDEX", missionIndexVec);
  m_Data.missionIndex = missionIndexVec[0];

  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "BAND_ID", m_Data.BandIDs);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Solar_Incidences.SUN_ELEVATION", m_Data.SunElevation);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Solar_Incidences.SUN_AZIMUTH", m_Data.SunAzimuth);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE", m_Data.IncidenceAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE_ALONG_TRACK", m_Data.AlongTrackIncidenceAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.INCIDENCE_ANGLE_ACROSS_TRACK", m_Data.AcrossTrackIncidenceAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.VIEWING_ANGLE", m_Data.ViewingAngle);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.AZIMUTH_ANGLE", m_Data.AzimuthAngle);

  std::vector<double> gainbiasUnavail={};
  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "BIAS", m_Data.PhysicalBias,gainbiasUnavail);

  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                     "GAIN", m_Data.PhysicalGain,gainbiasUnavail);

  ParseVector(mds, prefix + "Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Solar_Irradiance",
                     "VALUE" , m_Data.SolarIrradiance);

  ParseVector(mds, prefix + "Geometric_Data.Use_Area.Located_Geometric_Values",
                     "Acquisition_Angles.AZIMUTH_ANGLE" , m_Data.SceneOrientation);

  std::string path = prefix + "Product_Information.Delivery_Identification.JOB_ID";
  m_Data.ImageID =mds.GetAs<std::string>(path);

  path = prefix + "Product_Information.Delivery_Identification.PRODUCTION_DATE";

  m_Data.ProductionDate = mds.GetAs<std::string>(path);

  auto imagingDate = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_DATE" );
  auto imagingTime = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_TIME" );
  m_Data.AcquisitionDate = imagingDate + "T" + imagingTime;

  m_Data.Instrument = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT" );
  m_Data.InstrumentIndex = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT_INDEX" );

  m_Data.ProcessingLevel = mds.GetAs<std::string>
    (prefix + "Processing_Information.Product_Settings.PROCESSING_LEVEL");
  m_Data.SpectralProcessing = mds.GetAs<std::string>
    (prefix + "Processing_Information.Product_Settings.SPECTRAL_PROCESSING");

  // These metadata are specific to PHR sensor products
  if (m_Data.mission == "PHRNEO" && m_Data.ProcessingLevel == "SENSOR")
  {
    m_Data.TimeRangeStart = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Time.Time_Range.START");
    m_Data.TimeRangeEnd = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Time.Time_Range.END");
    m_Data.LinePeriod = mds.GetAs<std::string>(prefix +"Geometric_Data.Refined_Model.Time.Time_Stamp.LINE_PERIOD");
    m_Data.SwathFirstCol = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Geometric_Calibration.Instrument_Calibration.Swath_Range.FIRST_COL");
    m_Data.SwathLastCol = mds.GetAs<std::string>(prefix + "Geometric_Data.Refined_Model.Geometric_Calibration.Instrument_Calibration.Swath_Range.LAST_COL");
  }
}

} // end namespace otb
