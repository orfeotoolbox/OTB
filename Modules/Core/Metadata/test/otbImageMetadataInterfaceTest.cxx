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
#include "otbSarImageMetadataInterface.h"

int otbImageMetadataInterfaceTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  std::string inputFilename  = argv[1];
  std::string outputFilename = argv[2];

  typedef otb::Image<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  auto imd = reader->GetImageIO()->GetImageMetadata();
  auto mds = dynamic_cast<otb::MetadataSupplierInterface*>(reader->GetImageIO());
  if (!mds)
  {
    std::cout << "Input reader does not use GDALImageIO" << std::endl;
    return EXIT_FAILURE;
  }

  otb::ImageMetadataInterfaceBase::Pointer imi = otb::ImageMetadataInterfaceFactory::CreateIMI(imd, *mds);
  std::ofstream file;
  file.open(outputFilename);
  otb::testtools::PrintMetadata(imd, file);
  if (imd.Has(otb::MDGeom::GCP))
    file << "\nGCP\n" << boost::any_cast<otb::Projection::GCPParam>(imd[otb::MDGeom::GCP]).ToJSON(true) << '\n';
  if (imd.Has(otb::MDGeom::SAR))
  {
    otb::MetaData::Keywordlist kwl;
    boost::any_cast<otb::SARParam>(imd[otb::MDGeom::SAR]).ToKeywordlist(kwl, "");
    otb::SARParam sarParam;
    sarParam.FromKeywordlist(kwl, "");
  }
  if (imd.Has(otb::MDGeom::SARCalib))
  {
    otb::MetaData::Keywordlist kwl;
    boost::any_cast<otb::SARCalib>(imd[otb::MDGeom::SARCalib]).ToKeywordlist(kwl, "");
    otb::SARCalib SARCalib;
    SARCalib.FromKeywordlist(kwl, "");
  }
  file.close();

  if (dynamic_cast<otb::OpticalImageMetadataInterface*>(imi.GetPointer()))
  {
    if (!otb::HasOpticalSensorMetadata(imd))
    {
      std::cout << "Input image does not contain all required optical image metadata" << std::endl;
      return EXIT_FAILURE;
    }
  }
  else if (dynamic_cast<otb::SarImageMetadataInterface*>(imi.GetPointer()))
  {
    if (!otb::HasSARSensorMetadata(imd))
    {
      std::cout << "Input image does not contain all required sar image metadata" << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    std::cout << "Unknown interface" ;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
