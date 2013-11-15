/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkMacro.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbHuMomentsImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"
#include "itkResampleImageFilter.h"

#include "itkAffineTransform.h"

int otbHuMomentsImageFunctionNew(int argc, char * argv[])
{
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::HuMomentsImageFunction<InputImageType>             FunctionType;

  // Instantiating object
  FunctionType::Pointer function       = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}

int otbHuMomentsImageFunction(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>             InputImageType;
  typedef otb::ImageFileReader<InputImageType>               ReaderType;
  typedef otb::HuMomentsImageFunction<InputImageType>        FunctionType;
  typedef FunctionType::OutputType                           OutputType;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function = FunctionType::New();

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
  outputStream << std::setprecision(10) << "Hu Image moments: [10]" << std::endl;

  for (unsigned int j = 1; j < 8; ++j)
    {
    outputStream << "Hu(" << j << ") = " << Result[j-1] << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}

int otbHuMomentsImageFunctionScaleInvariant(int argc, char * argv[])
{
  const char * inputFilename   = argv[1];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType,
    InputImageType,
    double>                                                               StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType,
    double>                                                               InterpolatorType;
  typedef otb::HuMomentsImageFunction<InputImageType>                     FunctionType;
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
  index1[0] = 100;
  index1[1] = 100;
  function1->SetNeighborhoodRadius(2);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 200;
  index2[1] = 200;
  function2->SetNeighborhoodRadius(4);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error = 0.0;

  for (unsigned int j = 1; j < 8; ++j)
    {
    error += vcl_pow(vcl_abs( Result1[j-1] - Result2[j-1]), 2);

    std::cout << "Original -H" << j
              << " : " << Result1[j-1]
              << "  /  Scaled - H" << j
              << " : " << Result2[j-1] << std::endl;
    }

  error = vcl_sqrt(error)/7.0;
  std::cout << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-3)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-3     -> TEST FAILLED" << std::endl );
    }

  return EXIT_SUCCESS;
}

int otbHuMomentsImageFunctionRotationInvariant(int argc, char * argv[])
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
  typedef otb::HuMomentsImageFunction<InputImageType>                     FunctionType;
  typedef FunctionType::OutputType                                        OutputType;
  typedef itk::AffineTransform< double, Dimension >  TransformType;


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

  const InputImageType::SpacingType & spacing = reader->GetOutput()->GetSpacing();
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

  const double degreesToRadians = vcl_atan(1.0) / 45.0;
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

  for (unsigned int j = 1; j < 8; ++j)
    {
    error += vcl_pow(vcl_abs( Result1[j-1] - Result2[j-1]), 2);

    std::cout << "Original -H" << j
              << " : " << Result1[j-1]
              << "  /  Rotated - H" << j
              << " : " << Result2[j-1] << std::endl;
    }

  error = vcl_sqrt(error)/7.0;
  std::cout << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-3)
      {
      itkGenericExceptionMacro( << "Error = " << error
                                << "  > 1E-3     -> TEST FAILLED" << std::endl );
      }

  return EXIT_SUCCESS;
}
