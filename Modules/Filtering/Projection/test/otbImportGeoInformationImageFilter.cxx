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

#include "otbImportGeoInformationImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImportGeoInformationImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const unsigned int   Dimension = 2;
  typedef unsigned int PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::ImportGeoInformationImageFilter<ImageType, ImageType> ImportGeoInformationImageFilterType;

  // Instantiating objects
  ReaderType::Pointer                          reader = ReaderType::New();
  WriterType::Pointer                          writer = WriterType::New();
  ImportGeoInformationImageFilterType::Pointer import = ImportGeoInformationImageFilterType::New();

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetIndex(index);
  region.SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());
  ImageType::Pointer black = ImageType::New();
  black->SetRegions(region);
  black->Allocate();
  black->FillBuffer(128);
  std::cout << "black: " << black->GetLargestPossibleRegion() << std::endl;

  black->SetOrigin(reader->GetOutput()->GetOrigin());
  black->SetSignedSpacing(reader->GetOutput()->GetSignedSpacing());

  import->SetInput(black);
  import->SetSource(reader->GetOutput());
  //  import->Update();
  //       std::cout<<"import output: "<<import->GetOutput()<<std::endl;

  writer->SetFileName(outfname);
  writer->SetInput(import->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
