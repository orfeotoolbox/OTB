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
#include "otbRadarsat2ImageMetadataInterface.h"
#include "otbSARMetadata.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbSpatialReference.h"

// useful constants
#include <otbMath.h>
#include "otbXMLMetadataSupplier.h"
#include <boost/filesystem.hpp> 
#include "itksys/SystemTools.hxx"

namespace otb
{

Radarsat2ImageMetadataInterface::Radarsat2ImageMetadataInterface()
{
}


bool Radarsat2ImageMetadataInterface::HasCalibrationLookupDataFlag(const MetadataSupplierInterface &mds) const
{
  return mds.GetAs<bool>(true, "support_data.calibration_lookup_flag");
}

bool Radarsat2ImageMetadataInterface::CreateCalibrationLookupData(SARCalib& sarCalib,
                                                                  const ImageMetadata& imd,
                                                                  const MetadataSupplierInterface& mds,
                                                                  const bool geom) const
{
  if(SarImageMetadataInterface::CreateCalibrationLookupData(sarCalib, imd, mds, geom))
    return true;

  int offset = 0;

  Radarsat2CalibrationLookupData::Pointer sigmaSarLut = Radarsat2CalibrationLookupData::New();
  Radarsat2CalibrationLookupData::Pointer betaSarLut = Radarsat2CalibrationLookupData::New();
  Radarsat2CalibrationLookupData::Pointer gammaSarLut = Radarsat2CalibrationLookupData::New();
  Radarsat2CalibrationLookupData::Pointer dnSarLut = Radarsat2CalibrationLookupData::New();

  if(geom)
  {
    std::string path = "referenceNoiseLevel[SigmaNought].gain";
    auto glist = mds.GetAsVector<float>(path);
    sigmaSarLut->InitParameters(SarCalibrationLookupData::SIGMA, offset, glist);

    path =  "referenceNoiseLevel[BetaNought].gain";
    glist = mds.GetAsVector<float>(path);
    betaSarLut->InitParameters(SarCalibrationLookupData::BETA, offset, glist);

    path = "referenceNoiseLevel[Gamma].gain";
    glist = mds.GetAsVector<float>(path);
    gammaSarLut->InitParameters(SarCalibrationLookupData::GAMMA, offset, glist);
  }
  else
  {
    std::string pathGains = "lut.gains";
    auto productPath = boost::filesystem::path(mds.GetResourceFile());  // TODO C++ 2017 use std::filesystem

    productPath = productPath.remove_filename() /= "lutSigma.xml";
    XMLMetadataSupplier sigmaLutSupplier(productPath.string());
    auto glist = sigmaLutSupplier.GetAsVector<float>(pathGains);
    sigmaSarLut->InitParameters(SarCalibrationLookupData::SIGMA, offset, glist);

    productPath = productPath.remove_filename() /= "lutBeta.xml";
    XMLMetadataSupplier betaLutSupplier(productPath.string());
    glist = betaLutSupplier.GetAsVector<float>(pathGains);
    betaSarLut->InitParameters(SarCalibrationLookupData::BETA, offset, glist);

    productPath = productPath.remove_filename() /= "lutGamma.xml";
    XMLMetadataSupplier gammaLutSupplier(productPath.string());
    glist = gammaLutSupplier.GetAsVector<float>(pathGains);
    gammaSarLut->InitParameters(SarCalibrationLookupData::GAMMA, offset, glist);
  }

  sarCalib.calibrationLookupData[SarCalibrationLookupData::SIGMA] = std::move(sigmaSarLut);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::BETA] = std::move(betaSarLut);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::GAMMA] = std::move(gammaSarLut);
  sarCalib.calibrationLookupData[SarCalibrationLookupData::DN] = std::move(dnSarLut);

  return true;
}

double Radarsat2ImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface &) const
{
  return 0;
}

void ReadGeorefGCP(const XMLMetadataSupplier & xmlMS, ImageMetadata & imd)
{
  Projection::GCPParam gcp;
  std::stringstream ss;

  // Get the ellipsoid and semi-major, semi-minor axes
  if(xmlMS.HasValue("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.ellipsoidName"))
  {
    auto ellipsoidID = xmlMS.GetAs<std::string>("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.ellipsoidName");
    auto minor_axis = xmlMS.GetAs<double>(0, "product.imageAttributes.geographicInformation.referenceEllipsoidParameters.semiMinorAxis");
    auto major_axis = xmlMS.GetAs<double>(0, "product.imageAttributes.geographicInformation.referenceEllipsoidParameters.semiMajorAxis");
    if(ellipsoidID.empty() || minor_axis == 0 || major_axis == 0)
    {
      otbGenericExceptionMacro(MissingMetadataException, << "Cannot read GCP's spatial reference");
    }
    else if(ellipsoidID == "WGS84")
    {
      gcp.GCPProjection = SpatialReference::FromWGS84().ToWkt();
    }
    else
    {
      gcp.GCPProjection = SpatialReference::FromGeogCS("", "", ellipsoidID, major_axis,
                                                       major_axis/(major_axis - minor_axis)).ToWkt();
    }
  }

  auto GCPCount = xmlMS.GetNumberOf("product.imageAttributes.geographicInformation.geolocationGrid.imageTiePoint");

  for(unsigned int i = 1 ; i <= GCPCount ; ++i)
  {
    ss.str("");
    ss << "product.imageAttributes.geographicInformation.geolocationGrid.imageTiePoint_" << i << ".";
    const std::string id = std::to_string(i);
    gcp.GCPs.emplace_back(id,                                                        // id
                     "",                                                             // info
                     xmlMS.GetAs<double>(ss.str() + "imageCoordinate.pixel") + 0.5,  // col
                     xmlMS.GetAs<double>(ss.str() + "imageCoordinate.line") + 0.5,   // row
                     xmlMS.GetAs<double>(ss.str() + "geodeticCoordinate.longitude"), // px
                     xmlMS.GetAs<double>(ss.str() + "geodeticCoordinate.latitude"),  // py
                     xmlMS.GetAs<double>(ss.str() + "geodeticCoordinate.height"));   // pz
  }
  imd.Add(MDGeom::GCP, gcp);
}

void Radarsat2ImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  // Product file
  std::string ProductFilePath =
      itksys::SystemTools::GetParentDirectory(m_MetadataSupplierInterface->GetResourceFile(""))
      + "/product.xml";
  if (!itksys::SystemTools::FileExists(ProductFilePath))
  {
    otbGenericExceptionMacro(MissingMetadataException,
           << "Cannot find the Radarsat2 product.xml file");
  }
  XMLMetadataSupplier ProductMS(ProductFilePath);

  // Polarization
  std::string ImageFilePath = itksys::SystemTools::GetFilenameName(m_MetadataSupplierInterface->GetResourceFile(""));
  imd.Add(MDStr::Polarization, ImageFilePath.substr(8, 2));

  imd.Add(MDTime::AcquisitionStartTime, MetaData::ReadFormattedDate(ProductMS.GetAs<std::string>("product.sourceAttributes.rawDataStartTime")));
  imd.Add(MDStr::BeamMode, ProductMS.GetAs<std::string>("product.sourceAttributes.beamModeMnemonic"));
  imd.Add("FACILITY_IDENTIFIER", ProductMS.GetAs<std::string>("product.sourceAttributes.inputDatasetFacilityId"));
  imd.Add(MDNum::LineSpacing, ProductMS.GetAs<double>("product.imageAttributes.rasterAttributes.sampledLineSpacing"));
  imd.Add(MDNum::PixelSpacing, ProductMS.GetAs<double>("product.imageAttributes.rasterAttributes.sampledPixelSpacing"));
  imd.Add(MDStr::OrbitDirection, ProductMS.GetAs<std::string>("product.sourceAttributes.orbitAndAttitude.orbitInformation.passDirection"));
  imd.Add(MDStr::ProductType, ProductMS.GetAs<std::string>("product.imageGenerationParameters.generalProcessingInformation.productType"));
  imd.Add(MDStr::Instrument, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));
  imd.Add(MDStr::SensorID, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));
  imd.Add(MDStr::Mission, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));
  imd.Add(MDNum::NumberOfLines, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfLines"));
  imd.Add(MDNum::NumberOfColumns, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfSamplesPerLine"));
  imd.Add(MDTime::ProductionDate, MetaData::ReadFormattedDate(ProductMS.GetFirstAs<std::string>("product.imageGenerationParameters.generalProcessingInformation.processingTime")));
  imd.Add(MDNum::AverageSceneHeight, ProductMS.GetAs<double>("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.geodeticTerrainHeight"));
//  imd.Add(MDNum::RadarFrequency, this->GetRadarFrequency()); // not parsed
//  imd.Add(MDNum::PRF, 0.); // not parsed
//  imd.Add(MDNum::RSF, 0.); // not parsed
  imd.Add(MDNum::CenterIncidenceAngle, this->GetCenterIncidenceAngle(ProductMS));
  imd.Add(MDNum::CalScale, 1.0);

  SARParam sarParam;
  // Fetch the GCP
  ReadGeorefGCP(ProductMS, imd);
  imd.Add(MDGeom::SAR, sarParam);

  SARCalib sarCalib;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStartTime;
  std::istringstream("1970-01-01T00:00:00.000000") >> sarCalib.calibrationStopTime;
  LoadRadiometricCalibrationData(sarCalib, ProductMS, imd);
  CreateCalibrationLookupData(sarCalib, imd, ProductMS, false);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Radarsat2ImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  Fetch(MDTime::AcquisitionStartTime, imd, "support_data.image_date");
  Fetch(MDNum::LineSpacing, imd, "meters_per_pixel_y");
  Fetch(MDNum::PixelSpacing, imd, "meters_per_pixel_x");
  Fetch(MDStr::Instrument, imd, "sensor");
  imd.Add(MDStr::SensorID, "RADARSAT-2");
  imd.Add(MDNum::CalScale, 1.0);

  // Product file
  auto ProductFilePath = boost::filesystem::path(m_MetadataSupplierInterface->GetResourceFile("geom"));  // TODO C++ 2017 use std::filesystem

  if (!ProductFilePath.empty())
  {
    ProductFilePath = ProductFilePath.remove_filename() /= "product.xml";
    if(boost::filesystem::exists(ProductFilePath))
    {
      XMLMetadataSupplier ProductMS((ProductFilePath.remove_filename() /= "product.xml").string());
      imd.Add(MDStr::Mission, ProductMS.GetAs<std::string>("product.sourceAttributes.satellite"));
      imd.Add(MDNum::NumberOfLines, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfLines"));
      imd.Add(MDNum::NumberOfColumns, ProductMS.GetAs<int>("product.imageAttributes.rasterAttributes.numberOfSamplesPerLine"));
      imd.Add(MDTime::ProductionDate,
          MetaData::ReadFormattedDate(ProductMS.GetFirstAs<std::string>("product.imageGenerationParameters.generalProcessingInformation.processingTime")));
      imd.Add(MDNum::AverageSceneHeight,
          ProductMS.GetAs<double>("product.imageAttributes.geographicInformation.referenceEllipsoidParameters.geodeticTerrainHeight"));
//    imd.Add(MDNum::RadarFrequency, 0.); // not parsed
//    imd.Add(MDNum::PRF, 0.); // not parsed
//    imd.Add(MDNum::RSF, 0.); // not parsed
      imd.Add(MDNum::CenterIncidenceAngle, this->GetCenterIncidenceAngle(ProductMS));
      imd.Add(MDStr::BeamMode, ProductMS.GetAs<std::string>("product.sourceAttributes.beamModeMnemonic"));
      imd.Add("FACILITY_IDENTIFIER", ProductMS.GetAs<std::string>("product.sourceAttributes.inputDatasetFacilityId"));
      imd.Add(MDStr::OrbitDirection, ProductMS.GetAs<std::string>("product.sourceAttributes.orbitAndAttitude.orbitInformation.passDirection"));
      imd.Add(MDStr::ProductType, ProductMS.GetAs<std::string>("product.imageGenerationParameters.generalProcessingInformation.productType"));

      ReadGeorefGCP(ProductMS, imd);
    }
  }

  // Polarization
  auto polarization = m_MetadataSupplierInterface->GetResourceFile("image");
  if(!polarization.empty())
  {
    polarization = itksys::SystemTools::GetFilenameName(polarization);
    polarization = polarization.substr(polarization.rfind("_") + 1, 2);
    imd.Add(MDStr::Polarization, polarization);
  }

  // SAR Model
  SARParam sarParam;
  imd.Add(MDGeom::SAR, sarParam);
  SARCalib sarCalib;
  LoadRadiometricCalibrationData(sarCalib, *m_MetadataSupplierInterface, imd);
  CreateCalibrationLookupData(sarCalib, imd, *m_MetadataSupplierInterface, true);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Radarsat2ImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GEOM file
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "RADARSAT-2")
    this->ParseGeom(imd);
  // Try to fetch the metadata from GDAL Metadata Supplier
  else
    this->ParseGdal(imd);

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 0);
  imd.Add(MDNum::BlueDisplayChannel, 0);
}

} // end namespace otb
