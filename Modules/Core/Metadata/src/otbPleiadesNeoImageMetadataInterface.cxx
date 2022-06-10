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


#include "otbPleiadesNeoImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbGeometryMetadata.h"
#include "otbStringUtils.h"

#include "otbDimapMetadataHelper.h"

// useful constants
#include <otbMath.h>

#include "itksys/SystemTools.hxx"

#include "otbXMLMetadataSupplier.h"

namespace otb
{
using boost::lexical_cast;
using boost::bad_lexical_cast;

PleiadesNeoImageMetadataInterface::PleiadesNeoImageMetadataInterface()
{
}

void PleiadesNeoImageMetadataInterface::FetchSatAngles(
                    const std::vector<double> & incidenceAngles,
                    const std::vector<double> & alongTrackIncidenceAngles,
                    const std::vector<double> & acrossTrackIncidenceAngles,
                    const std::vector<double> & sceneOrientation,
                    ImageMetadata& imd)
{
  if(incidenceAngles.size() != 3 ||  sceneOrientation.size() != 3)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing satellite angles in Dimap")
  }

  // Convention use in input of atmospheric correction parameters computation is
  //"90 - satOrientation". PleiadesNeo does not seem to follow this convention so
  // inverse the formula here to be able to take the angle read in the metadata
  // as input for 6S. The second value is used (center value)
  imd.Add(MDNum::SatElevation, 90. - incidenceAngles[1]);

  if (alongTrackIncidenceAngles.size() != 3 ||
      acrossTrackIncidenceAngles.size() != 3)
  {
    // Use only orientation if across/along track incidence are not available
    imd.Add(MDNum::SatAzimuth, sceneOrientation[1]);
  }
  else
  {
    // Use center values
    auto cap = sceneOrientation[1];
    auto along = alongTrackIncidenceAngles[1];
    auto ortho = acrossTrackIncidenceAngles[1];
  
    auto satAzimuth =  (cap - std::atan2(std::tan(ortho * CONST_PI_180), 
                                        std::tan(along * CONST_PI_180)) 
                          * CONST_180_PI);

    imd.Add(MDNum::SatAzimuth, fmod(satAzimuth, 360));

  }
}

void PleiadesNeoImageMetadataInterface::FetchTabulatedPhysicalGain(ImageMetadata& imd)
{
  std::unordered_map<std::string, double> bandNameToPhysicalGain;
  // TODO check band order here.
  const auto &  sensorId = imd[MDStr::SensorID];
  if (sensorId == "PHRNEO" || sensorId == "PNEO3" || sensorId == "PNEO4" || sensorId == "PNEO5" || sensorId == "PNEO6")
  {
      bandNameToPhysicalGain = { {"P", 7.996},
          {"B5", 8.039}, {"B1", 6.600}, {"B2", 7.338}, {"B3", 8.132}, {"B6",9.955}, {"B4", 12.089}};
  }else
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Invalid metadata, bad sensor id");
  }

  for (auto & band: imd.Bands)
  {
    auto gain = bandNameToPhysicalGain.find(band[MDStr::BandName]);
    if (gain ==  bandNameToPhysicalGain.end())
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Cannot find the physical gain associated with " << band[MDStr::BandName]);
    }
    else
    {
      band.Add(MDNum::PhysicalGain, gain->second);
    }
  }
}


void PleiadesNeoImageMetadataInterface::FetchSolarIrradiance(const std::vector<double> & dimapSolarIrradiance, ImageMetadata& imd)
{
  std::unordered_map<std::string, double> defaultSolarIrradiance;

  const auto & sensorID = imd[MDStr::SensorID];

  otbGenericExceptionMacro(MissingMetadataException,<< "Invalid metadata, bad sensor id")

  // tolerance threshold
  double tolerance = 0.05;

  auto solarIrradianceIt = dimapSolarIrradiance.begin();
  for (auto & band : imd.Bands)
  {
    auto defaultValue = defaultSolarIrradiance.find(band[MDStr::BandName]);
    if (defaultValue != defaultSolarIrradiance.end() &&
         std::abs(*solarIrradianceIt - defaultValue->second) > (tolerance * defaultValue->second))
    {
      band.Add(MDNum::SolarIrradiance, defaultValue->second);
    }
    else
    {
      band.Add(MDNum::SolarIrradiance, *solarIrradianceIt);
    }
    solarIrradianceIt++;
  }

}

void PleiadesNeoImageMetadataInterface::FetchSpectralSensitivity(const std::string & sensorId, ImageMetadata& imd)
{
  otbGenericExceptionMacro(MissingMetadataException, "No Spectral sensitivity data for the PNEO sensor")
}

void PleiadesNeoImageMetadataInterface::Parse(ImageMetadata &imd)
{
  DimapMetadataHelper helper;

  // Satellite ID is either PHR 1A or PHR 1B
  // Product read by the TIFF/JP2 GDAL driver
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "IMAGERY/SATELLITEID").find("NEO") != std::string::npos)
  {
    // The driver stored the content of the Dimap XML file as metadatas in the IMD domain.
    helper.ParseDimapV3(*m_MetadataSupplierInterface, "IMD/");

    imd.Add(MDStr::GeometricLevel, helper.GetDimapData().ProcessingLevel);

    // fill RPC model
    if (imd[MDStr::GeometricLevel] == "SENSOR")
    {
      FetchRPC(imd, -0.5, -0.5);
    }
  }
  // Product read by the DIMAP GDAL driver
  else if (m_MetadataSupplierInterface->GetAs<std::string> ("","MISSION") == "PHRNEO")
  {
    // The DIMAP driver does not read the same metadata as the TIFF/JP2 one, and
    // some required metadata are missing. 
    // The XML Dimap file is read again and provided to the DimapMetadataHelper
    // using a XMLMetadataSupplier
    XMLMetadataSupplier xmlMds(m_MetadataSupplierInterface->GetResourceFile());

    helper.ParseDimapV3(xmlMds, "Dimap_Document.");

    imd.Add(MDStr::GeometricLevel, helper.GetDimapData().ProcessingLevel);

    // fill RPC model
    if (imd[MDStr::GeometricLevel] == "SENSOR")
    {
      FetchRPC(imd, -0.5, -0.5);
    }
  }
  // Geom case
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "support_data.sensorID").find("NEO") != std::string::npos)
  {
    helper.ParseGeom(*m_MetadataSupplierInterface);

    imd.Add(MDStr::GeometricLevel, helper.GetDimapData().ProcessingLevel);
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Sensor ID doesn't start with NEO")
  }

  const auto & dimapData = helper.GetDimapData();

  imd.Add(MDStr::SensorID, dimapData.mission + " " +dimapData.missionIndex);
  imd.Add(MDStr::Mission, "Pléiades NEO");

  imd.Add(MDStr::Instrument, dimapData.Instrument);
  imd.Add(MDStr::InstrumentIndex, dimapData.InstrumentIndex);

  if (dimapData.BandIDs.size() == imd.Bands.size())
  {
    const std::unordered_map<std::string, std::string> bandNameToEnhancedBandName =
      {{"P", "PAN"}, {"B5", "Deep Blue"}, {"B1", "Blue"}, {"B2", "Green"}, {"B3", "Red"}, {"B6", "Red edge"}, {"B4", "NIR"} };

    auto bandId = dimapData.BandIDs.begin();
    for (auto & band: imd.Bands)
    {
      band.Add(MDStr::BandName, *bandId);
      auto it = bandNameToEnhancedBandName.find(*bandId);
      if (it != bandNameToEnhancedBandName.end())
      {
        band.Add(MDStr::EnhancedBandName, it->second);
      }
      else
      {
        band.Add(MDStr::EnhancedBandName, "Unknown");
      }
      bandId++;
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException,
      << "The number of bands in image metadatas is incoherent with the DIMAP product")
  }

  //Sun elevation and azimuth should be taken from the center of the image , [0] is Top Center, [1] is Center, [2] is Bottom Center
  //This is the same for Viewing angle, it is taken from the center of the image (see FetchSatAngles)
  imd.Add(MDNum::SunAzimuth, dimapData.SunAzimuth[1]);
  imd.Add(MDNum::SunElevation, dimapData.SunElevation[1]);

  FetchSatAngles(dimapData.IncidenceAngle, dimapData.AlongTrackIncidenceAngle,
                 dimapData.AcrossTrackIncidenceAngle, dimapData.SceneOrientation,
                 imd);

  imd.Add(MDTime::ProductionDate,
    MetaData::ReadFormattedDate(dimapData.ProductionDate));
  imd.Add(MDTime::AcquisitionDate,
    MetaData::ReadFormattedDate(dimapData.AcquisitionDate));

  FetchSolarIrradiance(dimapData.SolarIrradiance, imd);

  //Store gain values from the dimap, if present
  if (dimapData.PhysicalGain.size() == imd.Bands.size())
  {
    auto gain = dimapData.PhysicalGain.begin();
    for (auto & band: imd.Bands)
    {
      band.Add(MDNum::PhysicalGain, *gain);
      gain++;
    }
  }
  else
  {
    //Store hard-coded values for gain
    FetchTabulatedPhysicalGain(imd);
    otbLogMacro(Info, << "Gain values from DIMAP could not be retrieved, reading hard-coded tables instead");
  }

  if (dimapData.PhysicalBias.size() == imd.Bands.size())
  {
    auto bias = dimapData.PhysicalBias.begin();
    for (auto & band: imd.Bands)
    {
      band.Add(MDNum::PhysicalBias, *bias);
      bias++;
    }
  }
  else
  {
    //Default Bias value
    for (auto & band: imd.Bands)
    {
      band.Add(MDNum::PhysicalBias, 0.0);
    }
    FetchSpectralSensitivity(imd[MDStr::SensorID], imd);
    otbLogMacro(Info, << "Bias values from DIMAP could not be retrieved, default values not available");
  }

  imd.Add(MetaData::PleiadesNeoUtils::IMAGE_ID_KEY, dimapData.ImageID);

  if (imd[MDStr::GeometricLevel] == "SENSOR")
  {
    /** These metadata are specific to PHR sensor products, and therefore as stored 
    as extra string keys in the metadata dictionary */
    imd.Add(MetaData::PleiadesNeoUtils::TIME_RANGE_START_KEY, dimapData.TimeRangeStart);
    imd.Add(MetaData::PleiadesNeoUtils::TIME_RANGE_END_KEY, dimapData.TimeRangeEnd);
    imd.Add(MetaData::PleiadesNeoUtils::LINE_PERIOD_KEY, dimapData.LinePeriod);
    imd.Add(MetaData::PleiadesNeoUtils::SWATH_FIRST_COL_KEY, dimapData.SwathFirstCol);
    imd.Add(MetaData::PleiadesNeoUtils::SWATH_LAST_COL_KEY, dimapData.SwathLastCol);
  }

  // Default display  
  // Panchromatic case
  if (imd.Bands.size() == 1)
  {
    imd.Add(MDNum::RedDisplayChannel, 0);
    imd.Add(MDNum::GreenDisplayChannel, 0);
    imd.Add(MDNum::BlueDisplayChannel, 0);
  }
  // Multispectral case
  else
  {
    imd.Add(MDNum::DeepBlueDisplayChannel, 0);
    imd.Add(MDNum::BlueDisplayChannel, 1);
    imd.Add(MDNum::GreenDisplayChannel, 2);
    imd.Add(MDNum::DeepBlueDisplayChannel, 3);
  }
}

namespace MetaData
{
namespace PleiadesNeoUtils
{
  bool HasSensorModelCharacteristics(const ImageMetadata & imd)
  {
    return imd.Has(IMAGE_ID_KEY)
        && imd.Has(TIME_RANGE_START_KEY)
        && imd.Has(TIME_RANGE_END_KEY)
        && imd.Has(LINE_PERIOD_KEY)
        && imd.Has(SWATH_FIRST_COL_KEY)
        && imd.Has(SWATH_LAST_COL_KEY);
  }

  SensorModelCharacteristics GetSensorModelCharacteristics(const ImageMetadata & imd)
  {
    SensorModelCharacteristics output;

    output.imageID = imd[IMAGE_ID_KEY];
    output.timeRangeStart = ReadFormattedDate(imd[TIME_RANGE_START_KEY]);
    output.timeRangeEnd = ReadFormattedDate(imd[TIME_RANGE_END_KEY]);
    output.linePeriod = std::stod(imd[LINE_PERIOD_KEY]);
    output.swathFirstCol = std::stoi(imd[SWATH_FIRST_COL_KEY]);
    output.swathLastCol = std::stoi(imd[SWATH_LAST_COL_KEY]);

    return output;
  }

} // end namespace PleiadeNeoUtils
} // end namespace MetaData
} // end namespace otb
