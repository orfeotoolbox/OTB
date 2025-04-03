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


#include "otbSarImageMetadataInterface.h"
#include "otbNisarImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbXMLMetadataSupplier.h"

#include "itksys/SystemTools.hxx"
#include "otbSpatialReference.h"
#include "otbStringUtilities.h"

// useful constants
#include <otbMath.h>
#include <iomanip>

namespace otb
{

double NisarImageMetadataInterface::GetCenterIncidenceAngle(const MetadataSupplierInterface &) const
{
  return m_MetadataSupplierInterface->GetAs<double>(0.0, "science_LSAR_GSLC_metadata_radarGrid_incidenceAngle__FillValue");
}

void NisarImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  auto imageFilePath = m_MetadataSupplierInterface->GetResourceFile();
  // If path is relative, find the full path to make sure we can retrieve the parent directory.
  if (!itksys::SystemTools::FileIsFullPath(imageFilePath))
  {
    imageFilePath = itksys::SystemTools::CollapseFullPath(imageFilePath);
  }
  auto const imageFileName = itksys::SystemTools::GetFilenameWithoutExtension(imageFilePath);
  auto splitImageFileNameIt = otb::split_on(imageFileName, '_').begin();

  imd.Add(MDStr::Mission, m_MetadataSupplierInterface->GetAs<std::string>("", "mission_name"));
  imd.Add(MDStr::SensorID, otb::to<std::string>(*(splitImageFileNameIt), "Reading SensorID"));
  ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt;
  auto productType = otb::to<std::string>(*(splitImageFileNameIt), "Reading ProductType");
  imd.Add(MDStr::ProductType, productType);
  ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt; ++splitImageFileNameIt;
  imd.Add(MDTime::AcquisitionDate, MetaData::ReadFormattedDate(otb::to<std::string>(*(splitImageFileNameIt), "Reading AcquisitionDate"), "%Y%m%dT%H%M%S"));
  ++splitImageFileNameIt;
  imd.Add(MDTime::ProductionDate, MetaData::ReadFormattedDate(otb::to<std::string>(*(splitImageFileNameIt), "Reading ProductionDate"), "%Y%m%dT%H%M%S"));

//  imd.Add(MDNum::OrbitNumber, m_MetadataSupplierInterface->GetAs<std::string>("", "science_LSAR_identification_absoluteOrbitNumber_units"));

//  prefix.str("science_LSAR_");
//  prefix << productType << "_";
//  const std::string sPrefix = prefix.str();
//  imd.Add(MDStr::Swath, itksys::SystemTools::UpperCase(imageFineName.substr(pos, pos2-pos)));
//  imd.Add(MDStr::Polarization, itksys::SystemTools::UpperCase(imageFineName.substr(pos, pos2-pos)));
//  imd.Add(MDStr::Mode, imd[MDStr::Swath]);
//  imd.Add(MDStr::BeamMode,
//    ManifestMS.GetFirstAs<std::string>(
//      "xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:instrument.safe:extension.s1sarl1:instrumentMode.s1sarl1:mode"
//    ));
//  imd.Add(MDStr::BeamSwath,
//    ManifestMS.GetFirstAs<std::string>(
//      "xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:platform.safe:instrument.safe:extension.s1sarl1:instrumentMode.s1sarl1:swath"
//    ));
//  imd.Add("FACILITY_IDENTIFIER", FIorganisation + " " + FIsoftware + " " + FIversion);
//  imd.Add(MDTime::AcquisitionStartTime, MetaData::ReadFormattedDate(AnnotationMS.GetAs<std::string>("product.adsHeader.startTime")));
//  imd.Add(MDTime::AcquisitionStopTime, MetaData::ReadFormattedDate(AnnotationMS.GetAs<std::string>("product.adsHeader.stopTime")));
//  imd.Add(MDNum::LineSpacing, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthPixelSpacing"));
//
//  imd.Add(MDStr::OrbitDirection, itksys::SystemTools::UpperCase(AnnotationMS.GetAs<std::string>("product.generalAnnotation.productInformation.pass")));
//  imd.Add(MDNum::OrbitNumber, AnnotationMS.GetAs<double>("product.adsHeader.absoluteOrbitNumber"));
//  imd.Add(MDNum::PixelSpacing, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.rangePixelSpacing"));
//  imd.Add(MDGeom::GCP, gcp);
//  imd.Add(MDNum::NumberOfLines, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfLines"));
//  imd.Add(MDNum::NumberOfColumns, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfSamples"));
//  imd.Add(MDNum::AverageSceneHeight, this->getBandTerrainHeight(AnnotationFilePath));
//  imd.Add(MDNum::RadarFrequency, AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.radarFrequency"));
//  imd.Add(MDNum::PRF, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthFrequency"));
//  imd.Add(MDNum::AbsoluteCalibrationConstant, CalibrationMS.GetAs<double>("calibration.calibrationInformation.absoluteCalibrationConstant"));
//  imd.Add(MDGeom::SAR, sarParam);
//  imd.Add(MDGeom::SARCalib, sarCalib);
  imd.Add(MDStr::Instrument, "LSAR");
}

void NisarImageMetadataInterface::ParseGeom(ImageMetadata &)
{
  // No need for a GEOM retrocompatibility
  throw std::logic_error("Function not implemented");
}

void NisarImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "mission_name") == "NISAR")
    this->ParseGdal(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
           << "Not a NISAR product");

  // Default display
  imd.Add(MDNum::RedDisplayChannel, 0);
  imd.Add(MDNum::GreenDisplayChannel, 0);
  imd.Add(MDNum::BlueDisplayChannel, 0);
}

} // end namespace otb
