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




#include <iostream>
#include "itkUnaryFunctorImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileReaderONERAComplex(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputRealFilename = argv[2];
  const char * outputImagFilename = argv[3];

  typedef std::complex<float> InputPixelType;
  typedef float               OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer Reader = ReaderType::New();

  Reader->SetFileName(inputFilename);

  typedef otb::ExtractROI<InputPixelType,
      InputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetStartX(10);
  extractROIFilter->SetStartY(10);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);
  extractROIFilter->SetInput(Reader->GetOutput());

  typedef itk::ComplexToRealImageFilter<
      InputImageType, OutputImageType> RealFilterType;
  typedef itk::ComplexToImaginaryImageFilter<
      InputImageType, OutputImageType> ImagFilterType;

  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(extractROIFilter->GetOutput());
  ImagFilterType::Pointer imagFilter = ImagFilterType::New();
  imagFilter->SetInput(extractROIFilter->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputRealFilename);
  writer->SetInput(realFilter->GetOutput());
  writer->Update();
  writer = WriterType::New();
  writer->SetFileName(outputImagFilename);
  writer->SetInput(imagFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
