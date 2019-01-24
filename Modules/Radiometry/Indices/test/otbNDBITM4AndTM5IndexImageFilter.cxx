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

#include "itkBinaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBuiltUpIndicesFunctor.h"

int otbNDBITM4AndTM5IndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputTM4ImageType;
  typedef otb::Image<PixelType, Dimension> InputTM5ImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputTM4ImageType> TM4ReaderType;
  typedef otb::ImageFileReader<InputTM5ImageType> TM5ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::NDBI  <InputTM4ImageType::PixelType,
      InputTM5ImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef itk::BinaryFunctorImageFilter<InputTM4ImageType,
      InputTM5ImageType,
      OutputImageType,
      FunctorType> BinaryFunctorImageFilterType;

  // Instantiating object
  BinaryFunctorImageFilterType::Pointer filter = BinaryFunctorImageFilterType::New();
  TM4ReaderType::Pointer                readerTM4 = TM4ReaderType::New();
  TM5ReaderType::Pointer                readerTM5 = TM5ReaderType::New();
  WriterType::Pointer                   writer = WriterType::New();

  const char * inputFilenameTM4  = argv[1];
  const char * inputFilenameTM5  = argv[2];
  const char * outputFilename = argv[3];

  readerTM4->SetFileName(inputFilenameTM4);
  readerTM5->SetFileName(inputFilenameTM5);
  writer->SetFileName(outputFilename);
  filter->SetInput1(readerTM4->GetOutput());
  filter->SetInput2(readerTM5->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
