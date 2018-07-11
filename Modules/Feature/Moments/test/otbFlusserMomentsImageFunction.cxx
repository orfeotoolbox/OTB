/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include <iomanip>
#include <fstream>
#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"


int otbFlusserMomentsImageFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>        FunctionType;

  // Instantiating object
  FunctionType::Pointer function       = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}

int otbFlusserMomentsImageFunction(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];


  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>        FunctionType;
  typedef FunctionType::OutputType                                OutputType;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function       = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);
  OutputType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Flusser Image moments: [10]" << std::endl;

  for (unsigned int j = 1; j < 12; ++j)
    {
    outputStream << "Flusser(" << j << ") = " << Result[j-1] << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}

int otbFlusserMomentsImageFunctionScaleInvariant(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType,
    InputImageType,
    double>                                                               StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType,
    double>                                                               InterpolatorType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>                FunctionType;
  typedef FunctionType::OutputType                                        OutputType;

  ReaderType::Pointer                         reader = ReaderType::New();
  StreamingResampleImageFilterType::Pointer   resampler = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();
  FunctionType::Pointer                       function1 = FunctionType::New();
  FunctionType::Pointer                       function2 = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());
  interpolator->SetRadius(2);
  interpolator->SetAlpha(-0.5);

  resampler->SetInput(reader->GetOutput());
  resampler->SetInterpolator(interpolator);
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 1024;
  size[1] = 1024;
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(0.5);
  resampler->Update();

  function1->SetInputImage(reader->GetOutput());
  InputImageType::IndexType index1;
  index1[0] = 256;
  index1[1] = 256;
  function1->SetNeighborhoodRadius(3);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 512;
  index2[1] = 512;
  function2->SetNeighborhoodRadius(6);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error = 0.0;

  for (unsigned int j = 1; j < 12; ++j)
    {
    error += std::pow(std::abs( Result1[j-1] - Result2[j-1]), 2);

    std::cout << "Original -F" << j
              << " : " << Result1[j-1]
              << "  /  Scaled - F" << j
              << " : " << Result2[j-1] << std::endl;
    }

  error = std::sqrt(error)/11.0;
  std::cout << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-3)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-3     -> TEST FAILLED" << std::endl );
    }

  return EXIT_SUCCESS;
}

int otbFlusserMomentsImageFunctionRotationInvariant(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const double angleInDegrees = atoi(argv[2]);


  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef itk::ResampleImageFilter<
    InputImageType, InputImageType >                                      FilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType,
    double>                                                               InterpolatorType;
  typedef otb::FlusserMomentsImageFunction<InputImageType>                FunctionType;
  typedef FunctionType::OutputType                                        OutputType;
  typedef itk::AffineTransform< double, Dimension >                       TransformType;


  ReaderType::Pointer                         reader = ReaderType::New();
  FilterType::Pointer                         filter = FilterType::New();
  TransformType::Pointer                      transform = TransformType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();
  FunctionType::Pointer                       function1 = FunctionType::New();
  FunctionType::Pointer                       function2 = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());
  interpolator->SetRadius(2);
  interpolator->SetAlpha(-0.5);

  filter->SetInterpolator(interpolator);
  filter->SetDefaultPixelValue( 100 );

  const InputImageType::SpacingType & spacing = reader->GetOutput()->GetSignedSpacing();
  const InputImageType::PointType & origin  = reader->GetOutput()->GetOrigin();
  InputImageType::SizeType size =
      reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  filter->SetOutputOrigin( origin );
  filter->SetOutputSpacing( spacing );
  filter->SetOutputDirection( reader->GetOutput()->GetDirection() );
  filter->SetSize( size );

  filter->SetInput(reader->GetOutput());

  TransformType::OutputVectorType translation1;
  const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
  const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;
  translation1[0] =   -imageCenterX;
  translation1[1] =   -imageCenterY;
  transform->Translate( translation1 );

  const double degreesToRadians = std::atan(1.0) / 45.0;
  const double angle = angleInDegrees * degreesToRadians;
  transform->Rotate2D( -angle, false );

  TransformType::OutputVectorType translation2;
  translation2[0] =   imageCenterX;
  translation2[1] =   imageCenterY;
  transform->Translate( translation2, false );

  filter->SetTransform( transform );
  filter->Update();

  function1->SetInputImage(reader->GetOutput());
  InputImageType::IndexType index1;
  index1[0] = 256;
  index1[1] = 256;
  function1->SetNeighborhoodRadius(4);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(filter->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 256;
  index2[1] = 256;
  function2->SetNeighborhoodRadius(4);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error = 0.0;

  for (unsigned int j = 1; j < 12; ++j)
    {
    error += std::pow(std::abs( Result1[j-1] - Result2[j-1]), 2);

    std::cout << "Original -F" << j
              << " : " << Result1[j-1]
              << "  /  Rotated - F" << j
              << " : " << Result2[j-1] << std::endl;
    }

  error = std::sqrt(error)/11.0;
  std::cout << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-3)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-3     -> TEST FAILLED" << std::endl );
    }

  return EXIT_SUCCESS;
}
