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

#ifndef otbMetadataDataHelper_h
#define otbMetadataDataHelper_h

#include "OTBMetadataExport.h"
#include "otbMetadataSupplierInterface.h"


namespace otb
{

struct DimapData
{
  std::string mission;
  std::string missionIndex;

  std::string SensorID;
  std::string ImageID;
  std::string ProductionDate;
  std::string AcquisitionDate;
  std::string Instrument;
  std::string InstrumentIndex;
  std::string ProcessingLevel;
  std::string SpectralProcessing;

  std::vector<std::string> BandIDs;

  std::vector<double>  SunAzimuth;
  std::vector<double>  SunElevation;
  std::vector<double>  IncidenceAngle;
  std::vector<double>  ViewingAngle;
  std::vector<double>  AzimuthAngle;
  std::vector<double>  SceneOrientation;

  std::vector<double>  PhysicalBias;
  std::vector<double>  PhysicalGain;
  
  std::vector<double>  SolarIrradiance;

  // Optional 
  std::vector<double> AlongTrackIncidenceAngle;
  std::vector<double> AcrossTrackIncidenceAngle;
  std::vector<double> AlongTrackViewingAngle;
  std::vector<double> AcrossTrackViewingAngle;
   
  double sunElevation;
  double sunAzimuth;

};



/** \class MetadataSupplierInterface
 *
 * \brief Base class to access metadata information in files/images
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT DimapMetadataHelper
{
public:
  enum DIMAPVersion
  {
    UNKNOWN,
    V1,
    V2
  };
  
  DimapMetadataHelper() : m_DimapLevel(DIMAPVersion::UNKNOWN)
  {
  }
  
  
  const DimapData & GetDimapData() const
  {
    return m_Data;
  }

  void ParseDimapV1(const MetadataSupplierInterface & mds, const std::string prefix)
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

    ParseVector(mds, prefix + "Dataset_Frame",
                       "SCENE_ORIENTATION" , m_Data.SceneOrientation);

    std::string path = prefix + "Production.JOB_ID";
    m_Data.ImageID =mds.GetAs<std::string>(path);
    
    path = prefix + "Production.DATASET_PRODUCTION_DATE";

    m_Data.ProductionDate = mds.GetAs<std::string>(path);

    std::vector<std::string> imagingDateVec;

    auto imagingDate = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.IMAGING_DATE" );
    auto imagingTime = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.IMAGING_TIME" );
    m_Data.AcquisitionDate = imagingDate + "T" + imagingTime;

    m_Data.Instrument = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.INSTRUMENT" );
    m_Data.InstrumentIndex = GetSingleValueFromList<std::string>(mds, prefix + "Dataset_Sources.Source_Information", "Scene_Source.INSTRUMENT_INDEX" );

    m_Data.ProcessingLevel = mds.GetAs<std::string>
      (prefix + "Data_Processing.PROCESSING_LEVEL");
  }


  void Parse(const MetadataSupplierInterface & mds)
  {
    std::vector<std::string> missionVec;
    ParseVector(mds, "IMD/Dataset_Sources.Source_Identification"
                    ,"Strip_Source.MISSION", missionVec);
    m_Data.mission = missionVec[0];

    std::vector<std::string> missionIndexVec;
    ParseVector(mds, "IMD/Dataset_Sources.Source_Identification"
                    ,"Strip_Source.MISSION_INDEX", missionIndexVec);
    m_Data.missionIndex = missionIndexVec[0];

    ParseVector(mds, "IMD/Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                       "BAND_ID", m_Data.BandIDs);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Solar_Incidences.SUN_ELEVATION", m_Data.SunElevation);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Solar_Incidences.SUN_AZIMUTH", m_Data.SunAzimuth);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.INCIDENCE_ANGLE", m_Data.IncidenceAngle);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.INCIDENCE_ANGLE_ALONG_TRACK", m_Data.AlongTrackIncidenceAngle);
    
    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.INCIDENCE_ANGLE_ACROSS_TRACK", m_Data.AcrossTrackIncidenceAngle);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.VIEWING_ANGLE", m_Data.ViewingAngle);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.AZIMUTH_ANGLE", m_Data.AzimuthAngle);

    ParseVector(mds, "IMD/Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                       "BIAS", m_Data.PhysicalBias);

    ParseVector(mds, "IMD/Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Radiance",
                       "GAIN", m_Data.PhysicalGain);

    ParseVector(mds, "IMD/Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.Band_Solar_Irradiance",
                       "VALUE" , m_Data.SolarIrradiance);

    ParseVector(mds, "IMD/Geometric_Data.Use_Area.Located_Geometric_Values",
                       "Acquisition_Angles.AZIMUTH_ANGLE" , m_Data.SceneOrientation);

    std::string path = "IMD/Product_Information.Delivery_Identification.JOB_ID";
    m_Data.ImageID =mds.GetAs<std::string>(path);
    
    path = "IMD/Product_Information.Delivery_Identification.PRODUCTION_DATE";

    m_Data.ProductionDate = mds.GetAs<std::string>(path);


    auto imagingDate = GetSingleValueFromList<std::string>(mds, "IMD/Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_DATE" );
    auto imagingTime = GetSingleValueFromList<std::string>(mds, "IMD/Dataset_Sources.Source_Identification", "Strip_Source.IMAGING_TIME" );
    m_Data.AcquisitionDate = imagingDate + "T" + imagingTime;

    m_Data.Instrument = GetSingleValueFromList<std::string>(mds, "IMD/Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT" );
    m_Data.InstrumentIndex = GetSingleValueFromList<std::string>(mds, "IMD/Dataset_Sources.Source_Identification", "Strip_Source.INSTRUMENT_INDEX" );

    m_Data.ProcessingLevel = mds.GetAs<std::string>
      ("IMD/Processing_Information.Product_Settings.PROCESSING_LEVEL");
    m_Data.SpectralProcessing = mds.GetAs<std::string>
      ("IMD/Processing_Information.Product_Settings.SPECTRAL_PROCESSING");
  }
  
protected:

private:

  template <class T>
  void ParseVector(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name,
                    std::vector<T> & dest) 
  {
    dest.clear();

    std::vector<std::string> mdStr;

    bool hasValue = false;
    auto value = mds.GetMetadataValue(prefix + "." + name, hasValue);

    if (hasValue)
    {
      mdStr.push_back(value);
    }
    else
    {
      hasValue = true;
      unsigned int i = 1;
      while(hasValue)
      {
        value = mds.GetMetadataValue(prefix + "_" + std::to_string(i) + "." + name, hasValue);
        if (hasValue)
          mdStr.push_back(value);
        i++;
      }
    }

    if (mdStr.empty())
    {
      otbGenericExceptionMacro(MissingMetadataException,
                          <<"Missing metadata '"<< prefix + name <<"'")
    }

    for (const auto & elem : mdStr)
    {
      try
      {
        dest.push_back(boost::lexical_cast<T>(elem));
      }
      catch (boost::bad_lexical_cast&)
      {
        otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<prefix + name<<"', got: "<<elem)
      }
    }

    //dest.push_back( );
  }
  
  // Non throwing version
  //TODO factorize
  template <class T>
  void ParseVector(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name,
                    std::vector<T> & dest,
                    std::vector<T> & defaultValue) 
  {
    dest.clear();

    std::vector<std::string> mdStr;

    bool hasValue = false;
    auto value = mds.GetMetadataValue(prefix + "." + name, hasValue);

    if (hasValue)
    {
      mdStr.push_back(value);
    }
    else
    {
      hasValue = true;
      unsigned int i = 1;
      while(hasValue)
      {
        value = mds.GetMetadataValue(prefix + "_" + std::to_string(i) + "." + name, hasValue);
        if (hasValue)
          mdStr.push_back(value);
        i++;
      }
    }

    if (mdStr.empty())
    {
      dest = defaultValue;
    }

    for (const auto & elem : mdStr)
    {
      try
      {
        dest.push_back(boost::lexical_cast<T>(elem));
      }
      catch (boost::bad_lexical_cast&)
      {
        dest = defaultValue;
      }
    }

    //dest.push_back( );
  }
  


  template <class T>
  T GetSingleValueFromList(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name)
  {
    std::vector<T> vector;
    ParseVector(mds, prefix, name, vector);
    return vector[0];

  }



  DIMAPVersion m_DimapLevel;

  DimapData m_Data;
};


} // end namespace otb

#endif

