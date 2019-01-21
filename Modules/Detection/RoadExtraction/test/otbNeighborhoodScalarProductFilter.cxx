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

#include "otbNeighborhoodScalarProductFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"

int otbNeighborhoodScalarProductFilter(int itkNotUsed(argc), char * argv[])
{

  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * diroutfname = argv[3];
  const double sigma = atof(argv[4]);

  const unsigned int Dimension = 2;
  typedef double                                                                      PixelType;
  typedef itk::CovariantVector<PixelType, Dimension>                                  VectorPixelType;
  typedef otb::Image<VectorPixelType, Dimension>                                      VectorImageType;
  typedef otb::Image<PixelType, Dimension>                                            ImageType;
  typedef otb::NeighborhoodScalarProductFilter<VectorImageType, ImageType, ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType>                                             ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                             WriterType;
  typedef itk::GradientRecursiveGaussianImageFilter<ImageType, VectorImageType>       GradientFilterType;

  // Instantiating object
  FilterType::Pointer         filter = FilterType::New();
  ReaderType::Pointer         reader = ReaderType::New();
  WriterType::Pointer         writer = WriterType::New();
  GradientFilterType::Pointer gradient = GradientFilterType::New();

  reader->SetFileName(infname);
  reader->Update();

  gradient->SetInput(reader->GetOutput());
  gradient->SetSigma(sigma);
  filter->SetInput(gradient->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
  writer = WriterType::New();
  writer->SetFileName(diroutfname);
  writer->SetInput(filter->GetOutputDirection());
  writer->Update();
  return EXIT_SUCCESS;
}
