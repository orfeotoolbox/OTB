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
#include "otbRadarsat2ImageMetadataInterface.h"
#include "otbSARMetadata.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"

// useful constants
#include <otbMath.h>
#include "otbXMLMetadataSupplier.h"
#include <boost/filesystem.hpp> 

namespace otb
{

Radarsat2ImageMetadataInterface::Radarsat2ImageMetadataInterface()
{
}

bool Radarsat2ImageMetadataInterface::CanRead() const
{
  std::string sensorID = GetSensorID();

  if (sensorID.find("RADARSAT-2") != std::string::npos)
  {
    return true;
  }
  else
    return false;
}

bool Radarsat2ImageMetadataInterface::HasCalibrationLookupDataFlag() const
{
  itkExceptionMacro("HasCalibrationLookupDataFlag(mds) not yet implemented in Radarsat2ImageMetadataInterface"); // TODO
}

bool Radarsat2ImageMetadataInterface::CreateCalibrationLookupData(SARCalib&, const ImageMetadata&, const MetadataSupplierInterface&, const bool) const
{
//  std::string lut = "SigmaNought";

//  switch (type)
//  {
//  case SarCalibrationLookupData::BETA:
//  {
//    lut = "BetaNought";
//  }
//  break;

//  case SarCalibrationLookupData::GAMMA:
//  {
//    lut = "GammaNought";
//  }
//  break;

//  case SarCalibrationLookupData::DN:
//  {
//    lut = "DN";
//  }
//  break;

//  case SarCalibrationLookupData::SIGMA:
//  default:
//  {
//    lut = "SigmaNought";
//  }
//  break;
//  }

//  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
//  const std::string          key              = "referenceNoiseLevel[" + lut + "].gain";

//  Radarsat2CalibrationLookupData::GainListType glist;
//  int                                          offset = 0;

//  Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].gain"), glist, "referenceNoiseLevel[" + lut + "].gain");

//  Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("referenceNoiseLevel[" + lut + "].offset"), "referenceNoiseLevel[" + lut + "].offset");

//  Radarsat2CalibrationLookupData::Pointer sarLut;
//  sarLut = Radarsat2CalibrationLookupData::New();
//  sarLut->InitParameters(type, offset, glist);
//  return static_cast<Radarsat2ImageMetadataInterface::LookupDataPointerType> (sarLut);
}

void Radarsat2ImageMetadataInterface::ParseDateTime(const char* key, std::vector<int>& dateFields) const
{
  if (dateFields.size() < 1)
  {
    // parse from keyword list
    if (!this->CanRead())
    {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
    }

    const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
    {
      itkExceptionMacro(<< "no key named '" << key << "'");
    }

    std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    date_time_str.resize(date_time_str.size() - 1);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "-T:.");
  }
}



int Radarsat2ImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[0], "support_data.image_date(year)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetMonth() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[1], "support_data.image_date(month)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetDay() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[2], "support_data.image_date(day)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetHour() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 3)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[3], "support_data.image_date(hour)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetMinute() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 4)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[4], "support_data.image_date(minute)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[0], "support_data.image_date(year)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[1], "support_data.image_date(production month)");
  }
  return value;
}

int Radarsat2ImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[2], "support_data.image_date(production day)");
  }
  return value;
}


double Radarsat2ImageMetadataInterface::GetPRF() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetRSF() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double Radarsat2ImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

Radarsat2ImageMetadataInterface::UIntVectorType Radarsat2ImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

void Radarsat2ImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  // Metadata read by GDAL
  Fetch(MDTime::AcquisitionStartTime, imd, "ACQUISITION_START_TIME");
  // Fetch(MDTime::AcquisitionStopTime, imd, "PROCESSING_TIME");
  Fetch(MDStr::BeamMode, imd, "BEAM_MODE");
  Fetch("FACILITY_IDENTIFIER", imd, "FACILITY_IDENTIFIER");
  Fetch(MDNum::LineSpacing, imd, "LINE_SPACING");
  Fetch(MDNum::PixelSpacing, imd, "PIXEL_SPACING");
  // Fetch(MDStr::Mode, imd, "MODE");
  Fetch(MDStr::OrbitDirection, imd, "ORBIT_DIRECTION");
  // Fetch(MDNum::OrbitNumber, imd, "ORBIT_NUMBER");
  Fetch(MDStr::ProductType, imd, "PRODUCT_TYPE");
  Fetch(MDStr::Instrument, imd, "SATELLITE_IDENTIFIER");
  Fetch(MDStr::SensorID, imd, "SENSOR_IDENTIFIER");
  
  // Product file
  std::string ProductFilePath = m_MetadataSupplierInterface->GetResourceFile("product.xml");
  if (!ProductFilePath.empty())
  {
    XMLMetadataSupplier ProductMS(ProductFilePath);
    imd.Add(MDStr::Mission, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));

    imd.Add(MDNum::NumberOfLines, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfLines"));
    imd.Add(MDNum::NumberOfColumns, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfSamplesPerLine"));
    imd.Add(MDTime::ProductionDate,
        ProductMS.GetFirstAs<MetaData::Time>("product.imageGenerationParameters.generalProcessingInformation.processingTime"));
    // imd.Add(MDNum::RadarFrequency, ProductMS.GetAs<double>("product.sourceAttributes.radarParameters.radarCenterFrequency"));
    imd.Add(MDNum::AverageSceneHeight, ProductMS.GetAs<double>("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.geodeticTerrainHeight"));


    imd.Add(MDNum::RadarFrequency, this->GetRadarFrequency());
    imd.Add(MDNum::PRF, this->GetPRF());
    imd.Add(MDNum::RSF, this->GetRSF());
    imd.Add(MDNum::CenterIncidenceAngle, this->GetCenterIncidenceAngle());


    assert(m_MetadataSupplierInterface->GetNbBands() == imd.Bands.size());

    SARParam sarParam;
    for (int bandId = 0 ; bandId < m_MetadataSupplierInterface->GetNbBands() ; ++bandId)
    {
      Fetch(MDStr::Polarization, imd, "POLARIMETRIC_INTERP", bandId);
      imd.Bands[bandId].Add(MDGeom::SAR, sarParam);
    }
    SARCalib sarCalib;
    LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
    imd.Add(MDGeom::SARCalib, sarCalib);
  }
}

void Radarsat2ImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  // Metadata read by GDAL
  Fetch(MDTime::AcquisitionStartTime, imd, "support_data.image_date");
  Fetch(MDNum::LineSpacing, imd, "meters_per_pixel_y");
  Fetch(MDNum::PixelSpacing, imd, "meters_per_pixel_x");
  Fetch(MDStr::Instrument, imd, "sensor");
  imd.Add(MDStr::SensorID, "SAR");

  // Product file
  auto ProductFilePath = boost::filesystem::path(m_MetadataSupplierInterface->GetResourceFile());
  if (!ProductFilePath.empty())
  {
    XMLMetadataSupplier ProductMS((ProductFilePath.remove_filename() /= "product.xml").string());
    imd.Add(MDStr::Mission, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));
    imd.Add(MDNum::NumberOfLines, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfLines"));
    imd.Add(MDNum::NumberOfColumns, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfSamplesPerLine"));
    imd.Add(MDTime::ProductionDate,
	      ProductMS.GetFirstAs<MetaData::Time>("product.imageGenerationParameters.generalProcessingInformation.processingTime"));
    imd.Add(MDNum::AverageSceneHeight,
	      ProductMS.GetAs<double>("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.geodeticTerrainHeight"));
    imd.Add(MDNum::RadarFrequency, this->GetRadarFrequency());
    imd.Add(MDNum::PRF, this->GetPRF());
    imd.Add(MDNum::RSF, this->GetRSF());
    imd.Add(MDNum::CenterIncidenceAngle, this->GetCenterIncidenceAngle());
    imd.Add(MDStr::BeamMode, ProductMS.GetAs<std::string>("product.sourceAttributes.beamModeMnemonic"));
    imd.Add("FACILITY_IDENTIFIER", ProductMS.GetAs<std::string>("product.sourceAttributes.inputDatasetFacilityId"));
    imd.Add(MDStr::OrbitDirection, ProductMS.GetAs<std::string>("product.sourceAttributes.orbitAndAttitude.orbitInformation.passDirection"));
    imd.Add(MDStr::ProductType, ProductMS.GetAs<std::string>("product.imageGenerationParameters.generalProcessingInformation.productType"));

    auto polarizations = ProductMS.GetAsVector<std::string>("product.sourceAttributes.radarParameters.polarizations");
    assert(polarizations.size() == imd.Bands.size());
    SARParam sarParam;
    for (int bandId = 0 ; bandId < polarizations.size() ; ++bandId)
    {
      imd.Bands[bandId].Add(MDStr::Polarization, polarizations[bandId]);
      imd.Bands[bandId].Add(MDGeom::SAR, sarParam);
    }
  }  
  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Radarsat2ImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "SATELLITE_IDENTIFIER") == "RADARSAT-2")
    this->ParseGdal(imd);
  // Try to fetch the metadata from GEOM file
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "RADARSAT-2")
    this->ParseGeom(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
			     << "Not a Sentinel1 product");
}

} // end namespace otb
