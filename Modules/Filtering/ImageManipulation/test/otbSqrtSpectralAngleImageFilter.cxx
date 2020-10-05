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

#include "otbSqrtSpectralAngleFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbSqrtSpectralAngleImageFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension> InputImageType;
  typedef otb::Image<double, Dimension>       OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::Functor::SqrtSpectralAngleFunctor<InputImageType::PixelType, OutputImageType::PixelType> FunctorType;

  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, FunctorType> SqrtSpectralAngleImageFilterType;

  // Instantiating objects
  SqrtSpectralAngleImageFilterType::Pointer filter = SqrtSpectralAngleImageFilterType::New();
  ReaderType::Pointer                       reader = ReaderType::New();
  WriterType::Pointer                       writer = WriterType::New();

  const char*  inputFilename  = argv[1];
  const char*  outputFilename = argv[2];
  unsigned int blueValue(::atoi(argv[3]));
  unsigned int greenValue(::atoi(argv[4]));
  unsigned int redValue(::atoi(argv[5]));
  unsigned int nirValue(::atoi(argv[6]));

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  InputImageType::PixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel[0] = blueValue;
  referencePixel[1] = greenValue;
  referencePixel[2] = redValue;
  referencePixel[3] = nirValue;

  filter->GetFunctor().SetReferencePixel(referencePixel);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
