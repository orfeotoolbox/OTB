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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "otbComplexToVectorImageCastFilter.h"


int otbImageFileReaderRADComplexFloatExtract(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float> InputPixelType;
  typedef std::complex<float> OutputPixelType;

  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputCplxImageType;
  typedef otb::VectorImage<float, Dimension>       OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ComplexToVectorImageCastFilter<OutputCplxImageType, OutputImageType>          CasterType;


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  CasterType::Pointer caster = CasterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::ExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetInput(reader->GetOutput());

  extractROIFilter->SetStartX(10);
  extractROIFilter->SetStartY(10);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);

  caster->SetInput(extractROIFilter->GetOutput());

  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
