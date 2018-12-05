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




#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbForwardFourierMellinTransformImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"

int otbFourierMellinImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputRealFilename = argv[2];
  const char * outputImaginaryFilename = argv[3];
  typedef double               InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<InputImageType>  WriterType;

  typedef itk::LinearInterpolateImageFunction<InputImageType, double> InterpolatorType;
  typedef otb::ForwardFourierMellinTransformImageFilter<InputPixelType, InterpolatorType,
      Dimension> FourierMellinTransformType;
  typedef FourierMellinTransformType::OutputImageType                    OutputImageType;
  typedef itk::ComplexToRealImageFilter<OutputImageType, InputImageType> RealFilterType;
  typedef itk::ComplexToImaginaryImageFilter<OutputImageType,
      InputImageType>                        ImaginaryFilterType;
  typedef itk::RescaleIntensityImageFilter<InputImageType, InputImageType> RescalerType;

  FourierMellinTransformType::Pointer FourierMellinTransform = FourierMellinTransformType::New();
  RealFilterType::Pointer             real = RealFilterType::New();
  ImaginaryFilterType::Pointer        imaginary = ImaginaryFilterType::New();
  RescalerType::Pointer               realRescaler = RescalerType::New();
  RescalerType::Pointer               imaginaryRescaler = RescalerType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer realWriter = WriterType::New();
  WriterType::Pointer imaginaryWriter = WriterType::New();

  reader->SetFileName(inputFilename);
  realWriter->SetFileName(outputRealFilename);
  imaginaryWriter->SetFileName(outputImaginaryFilename);

  OutputImageType::SizeType size;
  size[0] = 512;
  size[1] = 256;

  FourierMellinTransform->SetInput(reader->GetOutput());
  FourierMellinTransform->SetOutputSize(size);

  real->SetInput(FourierMellinTransform->GetOutput());
  realRescaler->SetInput(real->GetOutput());
  realRescaler->SetOutputMinimum(-1);
  realRescaler->SetOutputMaximum(1);

  realWriter->SetInput(realRescaler->GetOutput());
  realWriter->Update();

  imaginary->SetInput(FourierMellinTransform->GetOutput());
  imaginaryRescaler->SetInput(imaginary->GetOutput());
  imaginaryRescaler->SetOutputMinimum(-1);
  imaginaryRescaler->SetOutputMaximum(1);

  imaginaryWriter->SetInput(imaginaryRescaler->GetOutput());
  imaginaryWriter->Update();
  // Hugly hack for cleaning fftw threads
  #if defined( ITK_USE_FFTWF ) || defined( ITK_USE_FFTWD )
  fftw_cleanup_threads();
  #endif
  return EXIT_SUCCESS;
}
