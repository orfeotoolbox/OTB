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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkExceptionObject.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbFlusserImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "itkResampleImageFilter.h"


int otbFlusserImage(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];


  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                    ReaderType;
  typedef otb::FlusserImageFunction<InputImageType>               FunctionType;
  typedef FunctionType::RealType                                  RealType;

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function       = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();
  function->SetInputImage(reader->GetOutput());

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  function->SetNeighborhoodRadius(3);  
  RealType Result;
  Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10) << "Flusser Image moments: [10]" << std::endl;

  for (unsigned int j = 1; j < 12; j++)
    {
    outputStream << "Flusser(" << j << ") = " << Result[j-1] << std::endl;
    }

  outputStream.close();

  return EXIT_SUCCESS;
}

int otbFlusserImageScaleInvariant(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];
  const char * outputImageName = argv[3];


  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType,
    InputImageType,
    double>                                                               StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType, 
    double>                                                               InterpolatorType;
  typedef otb::FlusserImageFunction<InputImageType>                       FunctionType;
  typedef FunctionType::RealType                                          RealType;
  typedef otb::StreamingImageFileWriter<InputImageType>                   WriterType;
  
  ReaderType::Pointer                         reader = ReaderType::New();
  StreamingResampleImageFilterType::Pointer   resampler = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();
  FunctionType::Pointer                       function1 = FunctionType::New();
  FunctionType::Pointer                       function2 = FunctionType::New();
  WriterType::Pointer                         writer = WriterType::New();
  
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
  writer->SetInput(resampler->GetOutput());
  writer->SetFileName(outputImageName);
  writer->Update();

  function1->SetInputImage(reader->GetOutput());
  InputImageType::IndexType index1;
  index1[0] = 100;
  index1[1] = 100;
  function1->SetNeighborhoodRadius(2);  
  RealType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 200;
  index2[1] = 200;
  function2->SetNeighborhoodRadius(4);  
  RealType Result2 = function2->EvaluateAtIndex(index2);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);
  
  for (unsigned int j = 1; j < 12; j++)
    {
    double error = 2*vcl_abs( Result1[j-1] - Result2[j-1]) / vcl_abs(Result1[j-1] + Result2[j-1]);

    outputStream << "Error = " << error << std::endl
                 << "Original Image - Flusser Moment #" << j << " : " << Result1[j-1] << std::endl
                 << "Scaled Image   - Flusser Moment #" << j << " : " << Result2[j-1] << std::endl
                 << std::endl;
    
    if (error > 1)
      {
      itkGenericExceptionMacro(  <<std::endl 
                                 << "Error = " << error 
                                 << "  > 1     -> TEST FAILLED" << std::endl 
                                 << "Original Image - Flusser Moment #" << j << " : " << Result1[j-1] << std::endl
                                 << "Scaled Image   - Flusser Moment #" << j << " : " << Result2[j-1] << std::endl
                                 << std::endl;)
        }
    }
  outputStream.close();
  
  return EXIT_SUCCESS;
}

int otbFlusserImageRotationInvariant(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];
  const char * outputImageName = argv[3];
  const double angleInDegrees = atoi(argv[4]);


  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef itk::ResampleImageFilter<
    InputImageType, InputImageType >                                      FilterType;
  typedef itk::LinearInterpolateImageFunction<InputImageType, double>     InterpolatorType;
  typedef otb::FlusserImageFunction<InputImageType>                       FunctionType;
  typedef FunctionType::RealType                                          RealType;
  typedef otb::StreamingImageFileWriter<InputImageType>                   WriterType;
  typedef itk::AffineTransform< double, Dimension >  TransformType;
 

  ReaderType::Pointer                         reader = ReaderType::New();
  FilterType::Pointer                         filter = FilterType::New();
  TransformType::Pointer                      transform = TransformType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();
  FunctionType::Pointer                       function1 = FunctionType::New();
  FunctionType::Pointer                       function2 = FunctionType::New();
  WriterType::Pointer                         writer = WriterType::New();
  
  reader->SetFileName(inputFilename);
  reader->Update();

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

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputImageName);
  writer->Update();

  function1->SetInputImage(reader->GetOutput());
  InputImageType::IndexType index1;
  index1[0] = 256;
  index1[1] = 256;
  function1->SetNeighborhoodRadius(4);  
  RealType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(filter->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 256;
  index2[1] = 256;
  function2->SetNeighborhoodRadius(4);  
  RealType Result2 = function2->EvaluateAtIndex(index2);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);

  for (unsigned int j = 1; j < 12; j++)
    {
    double error = vcl_abs( Result1[j-1] - Result2[j-1]);
    
    outputStream << "Error = " << error << std::endl
                 << "Original Image - Flusser Moment #" << j << " : " << Result1[j-1] << std::endl
                 << "Rotated Image  - Flusser Moment #" << j << " : " << Result2[j-1] << std::endl
                 << "Rotation angle : " << angleInDegrees << std::endl;
    
    if (error > 1E-9)
      {
      itkGenericExceptionMacro(  <<std::endl 
                                 << "Error = " << error
                                 << "  > 1E-9     -> TEST FAILLED" << std::endl 
                                 << "Original Image - Flusser Moment #" << j << " : " << Result1[j-1] << std::endl 
                                 << " Rotated Image - Flusser Moment #" << j << " : " << Result2[j-1] << std::endl
                                 << "Rotation angle : " << angleInDegrees << std::endl );
      }
    }
  outputStream.close();
  
  return EXIT_SUCCESS;
}
