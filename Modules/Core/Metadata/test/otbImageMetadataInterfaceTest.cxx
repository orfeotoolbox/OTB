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


#include "itkMacro.h"
#include <boost/any.hpp>
#include <fstream>
#include <iostream>
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbGDALImageIO.h"
#include "otbMetaDataKey.h"
#include "otbTestTools.h"
#include "otbOpticalImageMetadataInterface.h"

int otbImageMetadataInterfaceTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef otb::Image<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  auto imd = reader->GetImageIO()->GetImageMetadata();
  auto mds = dynamic_cast<otb::MetadataSupplierInterface*>(reader->GetImageIO());
  otb::ImageMetadataInterfaceBase::Pointer imi = otb::ImageMetadataInterfaceFactory::CreateIMI(imd, mds);

  const otb::ImageMetadata& imd2 = imi->GetImageMetadata();
  std::ofstream file;
  file.open(outputFilename);
  otb::testtools::PrintMetadata(imd2, file);
  if (imd2.Has(otb::MDGeom::GCP))
    file << boost::any_cast<otb::Projection::GCPParam>(imd2[otb::MDGeom::GCP]).ToJSON(true);
  file.close();

  if (dynamic_cast<otb::OpticalImageMetadataInterface*>(imi.GetPointer()))
  {
    if (!otb::HasOpticalSensorMetadata(imi->GetImageMetadata()))
    {
      std::cout << "Input image does not contains all required optical image metadata" << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
