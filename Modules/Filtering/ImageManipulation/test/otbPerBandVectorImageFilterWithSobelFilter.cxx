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

#include "otbPerBandVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbPerBandVectorImageFilterWithSobelFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::Image<PixelType, Dimension>                         ImageType;
  typedef otb::VectorImage<PixelType, Dimension>                   VectorImageType;
  typedef itk::SobelEdgeDetectionImageFilter<ImageType, ImageType> SobelFilterType;

  typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, SobelFilterType> PerBandVectorImageFilterType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  // Instantiating object
  PerBandVectorImageFilterType::Pointer filter = PerBandVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
