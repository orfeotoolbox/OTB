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
#include "otbWaterIndicesFunctor.h"

int otbNDWIWaterIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<PixelType, Dimension> InputMIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileReader<InputMIRImageType> MIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::NDWI  <InputNIRImageType::PixelType,
      InputMIRImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef itk::BinaryFunctorImageFilter<InputNIRImageType,
      InputMIRImageType,
      OutputImageType,
      FunctorType> BinaryFunctorImageFilterType;

  // Instantiating object
  BinaryFunctorImageFilterType::Pointer filter = BinaryFunctorImageFilterType::New();
  NIRReaderType::Pointer                readerNIR = NIRReaderType::New();
  MIRReaderType::Pointer                readerMIR = MIRReaderType::New();
  WriterType::Pointer                   writer = WriterType::New();

  const char * inputFilenameNIR  = argv[1];
  const char * inputFilenameMIR  = argv[2];
  const char * outputFilename = argv[3];

  readerNIR->SetFileName(inputFilenameNIR);
  readerMIR->SetFileName(inputFilenameMIR);
  writer->SetFileName(outputFilename);
  filter->SetInput1(readerNIR->GetOutput());
  filter->SetInput2(readerMIR->GetOutput());

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
