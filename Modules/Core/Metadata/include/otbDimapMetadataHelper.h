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
  
  DimapMetadataHelper(const MetadataSupplierInterface * mds) : m_mds(mds), m_DimapLevel(DIMAPVersion::UNKNOWN)
  {
    std::cout << "DimapMetadataHelper constructor" << std::endl;
  }
  
  void ParseRadiometry(ImageMetadata & imd) const
  {
    std::cout << "DimapMetadataHelper::ParseRadiometry" << std::endl;
    
    // Sun elevation
    std::string path = "IMD/Geometric_Data.Use_Area.Located_Geometric_Values_1.Solar_Incidences.SUN_ELEVATION";
    imd.Add(MDNum::SunElevation, m_mds->GetAs<double>(path.c_str()));
    
    // Sun azimuth
    path = "IMD/Geometric_Data.Use_Area.Located_Geometric_Values_1.Solar_Incidences.SUN_AZIMUTH";
    imd.Add(MDNum::SunAzimuth, m_mds->GetAs<double>(path.c_str()));
    
    // Sat Elevation 
    path = "IMD/Geometric_Data.Use_Area.Located_Geometric_Values_1.Solar_Incidences.SUN_AZIMUTH";
    imd.Add(MDNum::SunAzimuth, m_mds->GetAs<double>(path.c_str()));
    
    // Sat Azimuth 
    path = "IMD/Geometric_Data.Use_Area.Located_Geometric_Values_1.Solar_Incidences.SUN_AZIMUTH";
    imd.Add(MDNum::SunAzimuth, m_mds->GetAs<double>(path.c_str()));
    
    // Band specific metadata
    path = "IMD/Raster_Data.Raster_Dimensions.NBANDS";
    auto nbBands = m_mds->GetAs<int>(path.c_str());
    std::cout << "Number of bands :" << nbBands << std::endl;
    
    const std::string bandPrefix = "IMD/Radiometric_Data.Radiometric_Calibration.Instrument_Calibration.Band_Measurement_List.";
    
    for (int i = 0; i < nbBands; i++)
    {
      // Physical bias
      path = bandPrefix + "Band_Radiance_" + std::to_string(i+1) +".BIAS"; 
      imd.Bands[i].Add(MDNum::PhysicalBias, m_mds->GetAs<double>(path.c_str()));
      
      // Physical gain
      path = bandPrefix + "Band_Radiance_" + std::to_string(i+1) +".GAIN"; 
      imd.Bands[i].Add(MDNum::PhysicalGain, m_mds->GetAs<double>(path.c_str()));
      
      // Solar irradiance
      path = bandPrefix + "Band_Solar_Irradiance_" + std::to_string(i+1) +".VALUE";
      imd.Bands[i].Add(MDNum::SolarIrradiance, m_mds->GetAs<double>(path.c_str()));
    }
    
    std::cout << imd << std::endl;
    
    // First wawelength
    
    // Last wawelength
    
    
  }
  
  
  
protected:

private:

  const MetadataSupplierInterface * m_mds;

  DIMAPVersion m_DimapLevel;


};

// TODO : for complex types ...

} // end namespace otb

#endif

