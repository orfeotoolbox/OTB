/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbImage.h"
#include "otbBinaryImageMinimalBoundingRegionCalculator.h"
#include "otbImageFileReader.h"
#include "otbImageList.h"

int otbBinaryImageMinimalBoundingRegionCalculator(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;

  int    nbImages  = atoi(argv[1]);
  char * outfile   = argv[2];

  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::BinaryImageMinimalBoundingRegionCalculator<ImageType>
  BoundingRegionCalculatorType;
  typedef BoundingRegionCalculatorType::RegionType RegionType;
  typedef otb::ImageList<ImageType>                ImageListType;
  typedef ImageListType::Iterator                  IteratorType;
  // reference image list
  ImageListType::Pointer images = ImageListType::New();

  // Reading input images
  std::ofstream out;
  out.open(outfile, std::ios::out);
  out << "Test results from otbBinaryImageBoundingRegionCalculator test." << std::endl;
  for (int i = 1; i <= nbImages; ++i)
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[2 + i]);
    reader->Update();
    images->PushBack(reader->GetOutput());
    }

  // Declaration
  BoundingRegionCalculatorType::Pointer brct;
  // Computing bounding region for each image
  for (IteratorType it = images->Begin(); it != images->End(); ++it)
    {
    brct = BoundingRegionCalculatorType::New();
    brct->SetInput(it.Get());
    brct->Update();
    RegionType region = brct->GetRegion();
    out << region.GetIndex() << "\t" << region.GetSize() << std::endl;
    }
  out << std::endl << "Testing the pad option" << std::endl << std::endl;
  for (IteratorType it = images->Begin(); it != images->End(); ++it)
    {
    brct = BoundingRegionCalculatorType::New();
    brct->SetPad(1);
    brct->SetInput(it.Get());
    brct->Update();
    RegionType region = brct->GetRegion();
    out << region.GetIndex() << "\t" << region.GetSize() << std::endl;
    }

  out.close();

  return EXIT_SUCCESS;
}
