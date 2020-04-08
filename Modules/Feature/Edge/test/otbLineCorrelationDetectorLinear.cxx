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
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineCorrelationDetectorImageFilter.h"


int otbLineCorrelationDetectorLinear(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename   = argv[1];
  const char* outputFilename1 = argv[2];
  const char* outputFilename2 = argv[3];

  // Parameters of the line to detect
  unsigned int WidthLine((unsigned int)::atoi(argv[4]));
  unsigned int LengthLine((unsigned int)::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int    Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;
  typedef otb::LineCorrelationDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType, InterpolatorType> FilterType;

  FilterType::Pointer FilterLineCorrelation = FilterType::New();

  FilterLineCorrelation->SetWidthLine(WidthLine);
  FilterLineCorrelation->SetLengthLine(LengthLine);

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();

  reader->SetFileName(inputFilename);
  writer1->SetFileName(outputFilename1);
  writer2->SetFileName(outputFilename2);

  FilterLineCorrelation->SetInput(reader->GetOutput());
  writer1->SetInput(FilterLineCorrelation->GetOutput());
  writer2->SetInput(FilterLineCorrelation->GetOutputDirection());

  writer1->Update();
  writer2->Update();

  return EXIT_SUCCESS;
}
