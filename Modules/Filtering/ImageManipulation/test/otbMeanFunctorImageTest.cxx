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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkUnaryFunctorImageFilter.h"
#include "otbMeanFunctor.h"

int otbMeanFunctorImageTest(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const unsigned int    Dimension = 2;
  typedef unsigned char PixelType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef otb::Functor::MeanFunctor<VectorImageType::PixelType, ImageType::PixelType> MeanFunctorType;

  typedef itk::UnaryFunctorImageFilter<VectorImageType, ImageType, MeanFunctorType> MeanFilterType;
  MeanFilterType::Pointer filter = MeanFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
