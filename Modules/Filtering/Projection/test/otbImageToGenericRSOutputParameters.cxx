/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

typedef otb::VectorImage<double, 2>       ImageType;
typedef otb::ImageToGenericRSOutputParameters<ImageType>  FilterType;

int otbImageToGenericRSOutputParameters (int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef ImageType::SizeType              SizeType;
  typedef ImageType::SpacingType           SpacingType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  // ForceSize
  SizeType  size;
  size[0] = 400;
  size[1] = 399;

  SpacingType  spacing;
  spacing[0] = 0.000006;
  spacing[1] = -0.000006;

  // Filter  : Target SRS : WGS84
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOutputProjectionRef("EPSG:4326");  //WGS84
  filter->Compute();

  // Output file
  std::ofstream outfile(outfname);

  outfile<<"Output Parameters for SRID : 4326 (WGS84)"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;

  // Target SRS : 32631 UTM 31 N
  filter->SetOutputProjectionRef("EPSG:32631");  // UTM 31 N
  filter->Compute();

  outfile<<"Output Parameters for SRID : 32631 (UTM 31 N)"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;

  // Target SRS : lambertII
  std::string  lambertRef = otb::SpatialReference::FromDescription("EPSG:27572").ToWkt();

  filter->SetOutputProjectionRef(lambertRef);
  filter->Compute();

  outfile<<"Output Parameters for SRS : Lambert II Etendu"<<std::endl;
  outfile<<"Output Origin : "<<filter->GetOutputOrigin()<<std::endl;
  outfile<<"Output Spacing : "<<filter->GetOutputSpacing()<<std::endl;
  outfile<<"Output Size : "<<filter->GetOutputSize()<<std::endl;
  outfile<< std::endl;


  outfile.close();

  return EXIT_SUCCESS;
}
