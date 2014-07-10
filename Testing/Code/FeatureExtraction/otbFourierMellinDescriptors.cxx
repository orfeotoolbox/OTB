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
#include <iomanip>

#include "itkMacro.h"

#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"


int otbFourierMellinDescriptorsNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double     InputPixelType;

  typedef otb::Image<InputPixelType,  Dimension>                     ImageType;
  typedef otb::FourierMellinDescriptorsImageFunction<ImageType>      FunctionType;

  // Instantiating object
  FunctionType::Pointer function = FunctionType::New();

  std::cout << function << std::endl;

  return EXIT_SUCCESS;
}

int otbFourierMellinDescriptors(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  unsigned int p((unsigned int) ::atoi(argv[2]));
  unsigned int q((unsigned int) ::atoi(argv[3]));
  const char * outputFilename  = argv[4];

  typedef double                  PixelType;
  const   unsigned int            Dimension = 2;

  typedef otb::Image<PixelType, Dimension>                       ImageType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::FourierMellinDescriptorsImageFunction<ImageType>  FunctionType;
  typedef FunctionType::OutputType                               OutputType;

  ReaderType::Pointer         reader = ReaderType::New();
  FunctionType::Pointer     function = FunctionType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  function->SetInputImage(reader->GetOutput());

  function->SetQmax(q);
  function->SetPmax(p);

  ImageType::IndexType index;
  index[0] = 256;
  index[1] = 256;
  function->SetNeighborhoodRadius(3);

  OutputType Result = function->EvaluateAtIndex(index);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);

  for (unsigned int k=0; k<=p; ++k)
    {
    for (unsigned int l=0; l<=q; ++l)
      {
      outputStream << "FMDescriptor |M(" << k << l << ")| : " << Result.at(k).at(l) << std::endl;
      }
    }
  outputStream.close();

  return EXIT_SUCCESS;
}

int otbFourierMellinDescriptorsScaleInvariant(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  unsigned int p((unsigned int) ::atoi(argv[2]));
  unsigned int q((unsigned int) ::atoi(argv[3]));

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType,
    InputImageType,
    double>                                                               StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType,
    double>                                                               InterpolatorType;
  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>      FunctionType;
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
  function1->SetQmax(q);
  function1->SetPmax(p);
  InputImageType::IndexType index1;
  index1[0] = 148;
  index1[1] = 248;
  function1->SetNeighborhoodRadius(3);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  function2->SetQmax(q);
  function2->SetPmax(p);
  InputImageType::IndexType index2;
  index2[0] = 296;
  index2[1] = 496;
  function2->SetNeighborhoodRadius(6);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error = 0.0;

  for (unsigned int k=0; k<=p; ++k)
    {
    for (unsigned int l=0; l<=q; ++l)
      {
      error += vcl_pow(vcl_abs( Result1.at(k).at(l) - Result2.at(k).at(l) ), 2);

      std::cout << "Original - D" << k << l
                << " : " << Result1.at(k).at(l)
                << "  /  Scaled - D" << k << l
                << " : " << Result2.at(k).at(l) << std::endl;
      }
    }

  error = vcl_sqrt(error)/(q+p);
  std::cout << "Error : " << error << std::endl
            << std::endl;

  if (error > 1E-2)
    {
    itkGenericExceptionMacro( << "Error = " << error
                              << "  > 1E-2     -> TEST FAILLED" << std::endl );
    }

  return EXIT_SUCCESS;
}


int otbFourierMellinDescriptorsRotationInvariant(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  unsigned int p((unsigned int) ::atoi(argv[2]));
  unsigned int q((unsigned int) ::atoi(argv[3]));
  const double angleInDegrees = atoi(argv[4]);

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef itk::ResampleImageFilter<
    InputImageType, InputImageType >                                      FilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType,
    double>                                                               InterpolatorType;
  typedef otb::FourierMellinDescriptorsImageFunction<InputImageType>      FunctionType;
  typedef FunctionType::OutputType                                        OutputType;
  typedef itk::AffineTransform< InputPixelType, Dimension >               TransformType;

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
  function1->SetQmax(q);
  function1->SetPmax(p);
  OutputType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(filter->GetOutput());
  InputImageType::IndexType index2;
  index2[0] = 256;
  index2[1] = 256;
  function2->SetNeighborhoodRadius(4);
  function2->SetQmax(q);
  function2->SetPmax(p);
  OutputType Result2 = function2->EvaluateAtIndex(index2);

  double error = 0.0;

  for (unsigned int k=0; k<=p; ++k)
    {
    for (unsigned int l=0; l<=q; ++l)
      {
      error += vcl_pow(vcl_abs( Result1.at(k).at(l) - Result2.at(k).at(l) ), 2);

      std::cout << "Original - D" << k << l
                << " : " << Result1.at(k).at(l)
                << "  /  Rotated - D" << k << l
                << " : " << Result2.at(k).at(l) << std::endl;
      }
    }

  error = vcl_sqrt(error)/(q+p);
  std::cout << "Error : " << error << std::endl
            << std::endl;

 if (error > 1E-3)
   {
  itkGenericExceptionMacro( << "Error = " << error
                            << "  > 1E-3     -> TEST FAILLED" << std::endl );
   }

  return EXIT_SUCCESS;
}
