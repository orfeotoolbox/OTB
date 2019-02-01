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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbShiftScaleImageAdaptor.h"

int otbShiftScaleImageAdaptor(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                                                        PixelType;
  typedef unsigned char                                                 OutputPixelType;
  typedef otb::Image<PixelType, 2>                                      InputImageType;
  typedef otb::Image<OutputPixelType, 2>                                OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                         WriterType;
  typedef otb::ShiftScaleImageAdaptor<InputImageType, PixelType>        FilterType;
  typedef itk::RescaleIntensityImageFilter<FilterType, OutputImageType> RescalerType;

  FilterType::Pointer   filter   = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();
  ReaderType::Pointer   reader   = ReaderType::New();
  WriterType::Pointer   writer   = WriterType::New();

  filter->SetScale(3.);
  filter->SetShift(10.);

  reader->SetFileName(inputFilename);

  filter->SetImage(reader->GetOutput());

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(filter);

  writer->SetFileName(outputFilename);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
