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


#include <iostream>
#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbExtendedFilenameToReaderOptions.h"
#include "otbSarImageMetadataInterfaceFactory.h"
#include "otbGeomMetadataSupplier.h"
#include "otbSARMetadata.h"

using RealType = double;
using ImageMetadataInterfaceType = otb::SarImageMetadataInterface;
using ImageMetadataInterfaceFactoryType = otb::SarImageMetadataInterfaceFactory;
using LookupDataType = otb::SarCalibrationLookupData;
using InputImageType = otb::Image<double, 2>;
using ImageReaderType = otb::ImageFileReader<InputImageType>;
using MetadataSupplierInterfaceType = otb::MetadataSupplierInterface;
using ImageMetadataType = otb::ImageMetadata;

std::unique_ptr<MetadataSupplierInterfaceType> GetIMI(const char* TheFileName)
{
  otb::ExtendedFilenameToReaderOptions::Pointer FilenameHelper = otb::ExtendedFilenameToReaderOptions::New();
  FilenameHelper->SetExtendedFileName(TheFileName);
  std::string DerivatedFileName = ImageReaderType::GetDerivedDatasetSourceFileName(TheFileName);
  std::string extension         = itksys::SystemTools::GetFilenameLastExtension(DerivatedFileName);
  std::string attachedGeom      = DerivatedFileName.substr(0, DerivatedFileName.size() - extension.size()) + std::string(".geom");
  std::string imageFileName     = DerivatedFileName.substr(0, DerivatedFileName.find('?'));
  // Case 1: external geom supplied through extended filename
  if (!FilenameHelper->GetSkipGeom() && FilenameHelper->ExtGEOMFileNameIsSet())
  {
    return std::make_unique<otb::GeomMetadataSupplier>(FilenameHelper->GetExtGEOMFileName(), imageFileName);
  }
  // Case 2: attached geom (if present)
  else if (!FilenameHelper->GetSkipGeom() && itksys::SystemTools::FileExists(attachedGeom))
  {
    return std::make_unique<otb::GeomMetadataSupplier>(attachedGeom, imageFileName);
  }
  // Case 3: tags in file
  else
  {
    auto supplierPointer = dynamic_cast<MetadataSupplierInterfaceType*>(
          otb::ImageIOFactory::CreateImageIO(TheFileName, otb::ImageIOFactory::ReadMode).GetPointer());
    return std::unique_ptr<MetadataSupplierInterfaceType>(supplierPointer);
  }
}

int otbSarCalibrationLookupDataTest(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: otbSarCalibationLookupDataTest /path/to/input/file /path/to/output/file  !" << std::endl;
    return EXIT_FAILURE;
  }
  ImageReaderType::Pointer reader = ImageReaderType::New();
  const char*   inFileName = argv[1];
  reader->SetFileName(inFileName);
  reader->UpdateOutputInformation();
  auto imd = reader->GetOutput()->GetImageMetadata();

  const char*   outFileName = argv[2];
  std::ofstream outfile;
  outfile.open(outFileName);

  auto supplier = GetIMI(inFileName);
  ImageMetadataInterfaceType::Pointer imageMetadataInterface = ImageMetadataInterfaceFactoryType::CreateIMI(imd, *supplier);

  if (!imageMetadataInterface.IsNotNull())
  {
    std::cerr << "cannot create a otb::SarImageMetadataInterface for input image." << std::endl;
    return EXIT_FAILURE;
  }

<<<<<<< HEAD
  outfile << imd[otb::MDStr::Instrument] << std::endl;

//TODO adapt this part
/*
  const std::string sensorId = imageMetadataInterface->GetSensorID();

  outfile << sensorId << std::endl;

  LookupDataType::Pointer lookupDataObj = imageMetadataInterface->GetCalibrationLookupData(0);

  if (!lookupDataObj.IsNotNull())
  {
    std::cerr << "lookupDataObj is Null" << std::endl;
    return EXIT_FAILURE;
  }

  RealType lutVal = static_cast<RealType>(lookupDataObj->GetValue(10,19));

  outfile << imageMetadataInterface->HasCalibrationLookupDataFlag() << std::endl;
  outfile << lutVal << std::endl;
*/

  outfile.close();
  return EXIT_SUCCESS;
}
