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


#include "otbImageToGenericRSOutputParameters.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include <iostream>
#include <fstream>

typedef otb::VectorImage<double, 2> ImageType;
typedef otb::ImageToGenericRSOutputParameters<ImageType> FilterType;

int otbImageToGenericRSOutputParameters(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  typedef otb::ImageFileReader<ImageType> ReaderType;

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  // Filter  : Target SRS : WGS84
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOutputProjectionRef("EPSG:4326"); // WGS84
  filter->Compute();

  // Output file
  std::ofstream outfile(outfname);

  outfile << "Output Parameters for SRID : 4326 (WGS84)\n";
  outfile << "Output Origin : " << filter->GetOutputOrigin() << "\n";
  outfile << "Output Spacing : " << filter->GetOutputSpacing() << "\n";
  outfile << "Output Size : " << filter->GetOutputSize() << "\n";
  outfile << std::endl;

  // Target SRS : 32631 UTM 31 N
  filter->SetOutputProjectionRef("EPSG:32631"); // UTM 31 N
  filter->Compute();

  outfile << "Output Parameters for SRID : 32631 (UTM 31 N)\n";
  outfile << "Output Origin : " << filter->GetOutputOrigin() << "\n";
  outfile << "Output Spacing : " << filter->GetOutputSpacing() << "\n";
  outfile << "Output Size : " << filter->GetOutputSize() << "\n";
  outfile << std::endl;

  // Target SRS : lambertII
  std::string lambertRef = otb::SpatialReference::FromDescription("EPSG:27572").ToWkt();

  filter->SetOutputProjectionRef(lambertRef);
  filter->Compute();

  outfile << "Output Parameters for SRS : Lambert II Etendu" << "\n";
  outfile << "Output Origin : " << filter->GetOutputOrigin() << "\n";
  outfile << "Output Spacing : " << filter->GetOutputSpacing() << "\n";
  outfile << "Output Size : " << filter->GetOutputSize() << "\n";
  outfile << std::endl;


  outfile.close();

  return EXIT_SUCCESS;
}
