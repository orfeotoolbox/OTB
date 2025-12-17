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
  Fetch(MDStr::Mission, imd, "mission_name");
  Fetch(MDStr::SensorID, imd, "SensorID");
  Fetch(MDStr::Instrument, imd, "Instrument");
  Fetch(MDStr::ProductType, imd, "ProductType");
  Fetch(MDTime::AcquisitionStartTime, imd, "StartDateTime");
  Fetch(MDTime::AcquisitionStopTime, imd, "EndDateTime");
  Fetch(MDStr::Polarization, imd, "Polarization");
  Fetch(MDStr::BeamMode, imd, "BandwidthMode");
  Fetch(MDStr::OrbitDirection, imd, "OrbitDirection");
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
