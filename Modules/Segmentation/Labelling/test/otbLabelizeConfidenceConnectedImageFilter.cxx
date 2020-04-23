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


#include <iostream>

#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbLabelizeConfidenceConnectedImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLabelizeConfidenceConnectedImageFilter(int itkNotUsed(argc), char* argv[])
{
  // Arguments
  char* inputImageName  = argv[1];
  char* outputImageName = argv[2];

  typedef unsigned char                InputPixelType;
  typedef unsigned char                OutputPixelType;
  typedef itk::RGBPixel<unsigned char> ColorPixelType;
  const unsigned int                   Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<ColorPixelType, Dimension>  RGBImageType;

  InputPixelType lowerThreshold((InputPixelType)::atoi(argv[3]));
  InputPixelType upperThreshold((InputPixelType)::atoi(argv[4]));

  double       multiplier         = (double)(atof(argv[5]));
  unsigned int numberOfIterations = (unsigned int)(atoi(argv[6]));
  unsigned int initialRadius      = (unsigned int)(atoi(argv[7]));

  // Reader
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer                          reader = ReaderType::New();
  reader->SetFileName(inputImageName);

  // Writer
  typedef otb::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer                        writer = WriterType::New();
  writer->SetFileName(outputImageName);

  // Labelize filter
  typedef otb::LabelizeConfidenceConnectedImageFilter<InputImageType, OutputImageType> LabelizeFilterType;
  LabelizeFilterType::Pointer filter = LabelizeFilterType::New();

  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);
  filter->SetMultiplier(multiplier);
  filter->SetNumberOfIterations(numberOfIterations);
  filter->SetInitialNeighborhoodRadius(initialRadius);

  // Rescale intensity
  typedef itk::RescaleIntensityImageFilter<OutputImageType, OutputImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  // Label to RGB image
  typedef itk::Functor::ScalarToRGBPixelFunctor<OutputPixelType> FunctorType;
  typedef itk::UnaryFunctorImageFilter<OutputImageType, RGBImageType, FunctorType> ColorLabelFilterType;
  ColorLabelFilterType::Pointer labelToRGB = ColorLabelFilterType::New();

  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  labelToRGB->SetInput(rescaler->GetOutput());
  writer->SetInput(labelToRGB->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
