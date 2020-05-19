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

#include "otbSOMbasedImageFilter.h"
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbSOMbasedImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* somfname = argv[2];
  const char* outfname = argv[3];

  const unsigned int Dimension = 2;

  typedef double                                              ComponentType;
  typedef itk::VariableLengthVector<ComponentType>            PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType> DistanceType;
  typedef otb::VectorImage<ComponentType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>   ReaderType;
  typedef otb::SOMMap<ImageType::PixelType> SOMMapType;
  typedef otb::SOMbasedImageFilter<ImageType, ImageType, DistanceType, SOMMapType> SOMbasedImageFilterType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileReader<SOMMapType> SOMReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;

  // Instantiating object
  SOMbasedImageFilterType::Pointer filter = SOMbasedImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(somfname);
  somreader->Update();

  filter->SetMap(somreader->GetOutput());
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
