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


// useful constants
#include <otbMath.h>

#include "itksys/SystemTools.hxx"

#include "otbXMLMetadataSupplier.h"

namespace otb
{
using boost::bad_lexical_cast;
using boost::lexical_cast;

void PleiadesNeoImageMetadataInterface::FetchSatAngles(const std::vector<double>& incidenceAngles, const std::vector<double>& alongTrackIncidenceAngles,
                                                       const std::vector<double>& acrossTrackIncidenceAngles, const std::vector<double>& sceneOrientation,
                                                       ImageMetadata& imd)
{
  if (incidenceAngles.size() <= 3 || sceneOrientation.size() <= 3)
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Missing satellite angles in Dimap")
  }

  // Convention use in input of atmospheric correction parameters computation is
  //"90 - satOrientation". PleiadesNeo does not seem to follow this convention so
  // inverse the formula here to be able to take the angle read in the metadata
  // as input for 6S. The second value is used (center value)
  imd.Add(MDNum::SatElevation, 90. - incidenceAngles[1]);

  if (alongTrackIncidenceAngles.size() != 3 || acrossTrackIncidenceAngles.size() != 3)
  {
    // Use only orientation if across/along track incidence are not available
    imd.Add(MDNum::SatAzimuth, sceneOrientation[1]);
  }
  else
  {
    // Use center values
    auto cap   = sceneOrientation[1];
    auto along = alongTrackIncidenceAngles[1];
    auto ortho = acrossTrackIncidenceAngles[1];

    auto satAzimuth = (cap - std::atan2(std::tan(ortho * CONST_PI_180), std::tan(along * CONST_PI_180)) * CONST_180_PI);

    imd.Add(MDNum::SatAzimuth, fmod(satAzimuth, 360));
  }
}

void PleiadesNeoImageMetadataInterface::FetchSpectralSensitivity(const std::string& sensorId, ImageMetadata& imd, DimapMetadataHelper& helper)
{
  otb::MetaData::LUT1D spectralSensitivity;
  spectralSensitivity.Axis[0].Origin  = 0.0;
  spectralSensitivity.Axis[0].Spacing = 1.0;
  spectralSensitivity.Axis[0].Size    = 255;
  std::string ProductFilePath         = itksys::SystemTools::GetParentDirectory(m_MetadataSupplierInterface->GetResourceFile()) + "/";
  if(helper.GetDimapData().LUTFileNames.size()>0){
    for (std::string const& lutPath : helper.GetDimapData().LUTFileNames)
    {
      if (itksys::SystemTools::FileExists(ProductFilePath + lutPath))
      {
        XMLMetadataSupplier xmlLut(ProductFilePath + lutPath);
        helper.ParseLUT(xmlLut);
      }
    }
  }
  else// TODO add default LUT according to the type of image RGB or NED
  {
    helper.createDefaultLUTs();
  }

  if (sensorId.find("NEO") != std::string::npos)
  {
    for (auto& band : imd.Bands)
    {
      auto SpectralSensitivityIt = (helper.GetDimapData().LUTs).find(band[MDStr::BandName]);
      if (SpectralSensitivityIt != (helper.GetDimapData().LUTs).end())
      {
        spectralSensitivity.Array = SpectralSensitivityIt->second;
        band.Add(MDL1D::SpectralSensitivity, spectralSensitivity);
      }
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, "Invalid PNEO Sensor ID")
  }
}

void PleiadesNeoImageMetadataInterface::Parse(ImageMetadata& imd)
{
  DimapMetadataHelper helper;

  // Satellite ID is either PHR 1A or PHR 1B
  // Product read by the TIFF/JP2 GDAL driver
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "IMAGERY/SATELLITEID").find("PNEO") != std::string::npos)
  {
    // The driver stored the content of the Dimap XML file as metadatas in the IMD domain.
    helper.ParseDimapV3(*m_MetadataSupplierInterface, "IMD/");

    imd.Add(MDStr::GeometricLevel, helper.GetDimapData().ProcessingLevel);

    // fill RPC model
    if (imd[MDStr::GeometricLevel] == "SENSOR")
    {
      FetchRPC(imd);
    }
  }
  // Product read by the DIMAP GDAL driver
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "MISSION") == "PNEO")
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
      FetchRPC(imd);
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
    otbGenericExceptionMacro(MissingMetadataException, << "Sensor ID doesn't start with NEO")
  }

  const auto& dimapData = helper.GetDimapData();

  imd.Add(MDStr::SensorID, dimapData.mission + " " + dimapData.missionIndex);
  imd.Add(MDStr::Mission, "PNEO");

  imd.Add(MDStr::Instrument, dimapData.Instrument);
  imd.Add(MDStr::InstrumentIndex, dimapData.InstrumentIndex);

  // gdal computes wrong number of bands with some PNEO products (3 instead of 6 for Primary Full Bundle MS for example)
  // next line is a temporary fix for this issue
  imd.Bands.resize(dimapData.BandIDs.size());

  if (dimapData.BandIDs.size() == imd.Bands.size())
  {
    const std::unordered_map<std::string, std::string> bandNameToEnhancedBandName = {{"P", "P"},  {"DB", "B5"}, {"B", "B1"},  {"G", "B2"},
                                                                                     {"R", "B3"}, {"RE", "B6"}, {"NIR", "B4"}};

    auto bandId = dimapData.BandIDs.begin();
    for (auto& band : imd.Bands)
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
    otbGenericExceptionMacro(MissingMetadataException, << "The number of bands in image metadatas is incoherent with the DIMAP product")
  }

  // Sun elevation and azimuth should be taken from the center of the image , [0] is Top Center, [1] is Center, [2] is Bottom Center
  // This is the same for Viewing angle, it is taken from the center of the image (see FetchSatAngles)
  imd.Add(MDNum::SunAzimuth, dimapData.SunAzimuth[1]);
  imd.Add(MDNum::SunElevation, dimapData.SunElevation[1]);

  FetchSatAngles(dimapData.IncidenceAngle, dimapData.AlongTrackIncidenceAngle, dimapData.AcrossTrackIncidenceAngle, dimapData.SceneOrientation, imd);

  imd.Add(MDTime::ProductionDate, MetaData::ReadFormattedDate(dimapData.ProductionDate));
  imd.Add(MDTime::AcquisitionDate, MetaData::ReadFormattedDate(dimapData.AcquisitionDate));

  // Store gain values from the dimap, if present
  if (dimapData.PhysicalGain.size() == imd.Bands.size() && dimapData.SolarIrradiance.size() == imd.Bands.size())
  {
    auto solarIrradianceIt = dimapData.SolarIrradiance.begin();
    auto gain = dimapData.PhysicalGain.begin();
    for (auto& band : imd.Bands)
    {
      band.Add(MDNum::PhysicalGain, *gain);
      band.Add(MDNum::SolarIrradiance,*solarIrradianceIt);
      ++solarIrradianceIt;
      ++gain;
    }
  }
  else
  {
    otbGenericExceptionMacro(MissingMetadataException, << "Gain values from DIMAP could not be retrieved")
  }

  if (dimapData.PhysicalBias.size() == imd.Bands.size())
  {
    auto bias = dimapData.PhysicalBias.begin();
    for (auto& band : imd.Bands)
    {
      band.Add(MDNum::PhysicalBias, *bias);
      bias++;
    }
  }
  else
  {
    // Default Bias value
    for (auto& band : imd.Bands)
    {
      band.Add(MDNum::PhysicalBias, 0.0);
    }
    otbGenericExceptionMacro(MissingMetadataException, << "Bias values from DIMAP could not be retrieved");
  }

  imd.Add(MetaData::PleiadesNeoUtils::IMAGE_ID_KEY, dimapData.ImageID);

  if (imd[MDStr::GeometricLevel] == "SENSOR")
  {
    /** These metadata are specific to PHR sensor products, and therefore as stored
    as extra string keys in the metadata dictionary */
    imd.Add(MetaData::PleiadesNeoUtils::TIME_RANGE_START_KEY, dimapData.TimeRangeStart);
    imd.Add(MetaData::PleiadesNeoUtils::TIME_RANGE_END_KEY, dimapData.TimeRangeEnd);
    imd.Add(MetaData::PleiadesNeoUtils::LINE_PERIOD_KEY, dimapData.LinePeriod);
   // imd.Add(MetaData::PleiadesNeoUtils::SWATH_FIRST_COL_KEY, dimapData.SwathFirstCol);
   // imd.Add(MetaData::PleiadesNeoUtils::SWATH_LAST_COL_KEY, dimapData.SwathLastCol);
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
    imd.Add(MDNum::RedDisplayChannel, 3);
  }

  // add LUTs
  FetchSpectralSensitivity(imd[MDStr::SensorID], imd, helper);
}

namespace MetaData
{
namespace PleiadesNeoUtils
{
bool HasSensorModelCharacteristics(const ImageMetadata& imd)
{
  return imd.Has(IMAGE_ID_KEY) && imd.Has(TIME_RANGE_START_KEY) && imd.Has(TIME_RANGE_END_KEY) && imd.Has(LINE_PERIOD_KEY) && imd.Has(SWATH_FIRST_COL_KEY) &&
         imd.Has(SWATH_LAST_COL_KEY);
}

SensorModelCharacteristics GetSensorModelCharacteristics(const ImageMetadata& imd)
{
  SensorModelCharacteristics output;

  output.imageID        = imd[IMAGE_ID_KEY];
  output.timeRangeStart = ReadFormattedDate(imd[TIME_RANGE_START_KEY]);
  output.timeRangeEnd   = ReadFormattedDate(imd[TIME_RANGE_END_KEY]);
  output.linePeriod     = std::stod(imd[LINE_PERIOD_KEY]);
  output.swathFirstCol  = std::stoi(imd[SWATH_FIRST_COL_KEY]);
  output.swathLastCol   = std::stoi(imd[SWATH_LAST_COL_KEY]);

  return output;
}

} // end namespace PleiadesNeoUtils
} // end namespace MetaData
} // end namespace otb
