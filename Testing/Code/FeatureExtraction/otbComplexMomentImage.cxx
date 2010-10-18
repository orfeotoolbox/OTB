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
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbComplexMomentImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "itkResampleImageFilter.h"

int otbComplexMomentImage(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  unsigned int p((unsigned int) ::atoi(argv[2]));
  unsigned int q((unsigned int) ::atoi(argv[3]));
  const char * outputFilename  = argv[4];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>   ReaderType;

  typedef otb::ComplexMomentImageFunction<InputImageType> CMType;
  typedef CMType::ComplexType                             ComplexType;

  ReaderType::Pointer reader         = ReaderType::New();
  CMType::Pointer     function = CMType::New();

  reader->SetFileName(inputFilename);

  reader->Update();
  function->SetInputImage(reader->GetOutput());

  function->SetQmax(q);
  function->SetPmax(p);

  InputImageType::IndexType index;
  index[0] = 100;
  index[1] = 100;

  ComplexType Result;

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);
  
  function->SetNeighborhoodRadius(3);
  Result = function->EvaluateAtIndex(index);
  for (unsigned int k=0; k<=p; k++)
    {
    for (unsigned int l=0; l<=q; l++)
      {
      outputStream << "ComplexMoment c(" << k << l << ") : " << Result.at(k).at(l) << std::endl;
      }
    }
  outputStream.close();

  return EXIT_SUCCESS;
}


int otbComplexMomentImageScaleInvariant(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  unsigned int p((unsigned int) ::atoi(argv[2]));
  unsigned int q((unsigned int) ::atoi(argv[3]));
  const char * outputFilename  = argv[4];
  const char * outputImageName = argv[5];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>                          InputImageType;
  typedef otb::ImageFileReader<InputImageType>                            ReaderType;
  typedef otb::StreamingResampleImageFilter<InputImageType,
    InputImageType,
    double>                                                               StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<InputImageType, 
    double>                                                               InterpolatorType;
  //typedef itk::LinearInterpolateImageFunction<InputImageType, 
  //  double>                                                               InterpolatorType;
  typedef otb::ComplexMomentImageFunction<InputImageType>                 FunctionType;
  typedef FunctionType::ComplexType                                       ComplexType;
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
  function1->SetQmax(q);
  function1->SetPmax(p);
  InputImageType::IndexType index1;
  index1[0] = 100;
  index1[1] = 100;
  function1->SetNeighborhoodRadius(4);
  ComplexType Result1 = function1->EvaluateAtIndex(index1);

  function2->SetInputImage(resampler->GetOutput());
  function2->SetQmax(q);
  function2->SetPmax(p);
  InputImageType::IndexType index2;
  index2[0] = 200;
  index2[1] = 200;
  function2->SetNeighborhoodRadius(8);  
  ComplexType Result2 = function2->EvaluateAtIndex(index2);

  std::ofstream outputStream(outputFilename);
  outputStream << std::setprecision(10);

  for (unsigned int k=0; k<=p; k++)
    {
    for (unsigned int l=0; l<=q; l++)
      {
      double error;
      if ( (k+l) >= 2)
        {
        if (k == l)
          {
          error = 2*vcl_abs( Result1.at(k).at(l).real() - Result2.at(k).at(l).real())
            / vcl_abs(Result1.at(k).at(l).real() + Result2.at(k).at(l).real());
          }
        else
          {
          error = (vcl_abs( Result1.at(k).at(l).real() - Result2.at(k).at(l).real())
            / vcl_abs(Result1.at(k).at(l).real() + Result2.at(k).at(l).real())
            + vcl_abs( Result1.at(k).at(l).imag() - Result2.at(k).at(l).imag())
            / vcl_abs(Result1.at(k).at(l).imag() + Result2.at(k).at(l).imag()));
          }

        outputStream << "Error = " << error << std::endl
                     << "Original Image - Complex Moment c" << k << l 
                     << " : " << Result1.at(k).at(l) << std::endl
                     << "Scaled Image   - Complex Moment c" << k << l
                     << " : " << Result2.at(k).at(l) << std::endl
                     << std::endl;
        /*
        if (error > 1)
          {
          itkGenericExceptionMacro(  <<std::endl 
                                     << "Error = " << error
                                     << "  > 1     -> TEST FAILLED" << std::endl 
                                     << "Original Image - Complex Moment c" << k << l 
                                     << " : " << Result1.at(k).at(l) << std::endl 
                                     << " Scaled Image  - Complex Moment c" << k << l 
                                     << " : " << Result2.at(k).at(l) << std::endl );
          }
        */
        }
      }
    }
  outputStream.close();

  return EXIT_SUCCESS;
}
