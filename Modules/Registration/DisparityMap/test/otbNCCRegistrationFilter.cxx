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

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

#include "otbNCCRegistrationFilter.h"
#include "itkRecursiveGaussianImageFilter.h"

int otbNCCRegistrationFilter(int argc, char* argv[])
{

  if (argc != 7)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedFileName movingFileName fieldOutName";
    std::cerr << "explorationSize bluringSigma nbIterations ";

    return EXIT_FAILURE;
  }

  const unsigned int ImageDimension = 2;

  typedef double PixelType;
  typedef itk::Vector<double, ImageDimension>               DisplacementPixelType;
  typedef otb::Image<PixelType, ImageDimension>             MovingImageType;
  typedef otb::Image<PixelType, ImageDimension>             FixedImageType;
  typedef otb::Image<DisplacementPixelType, ImageDimension> DisplacementFieldType;

  typedef otb::ImageFileReader<FixedImageType> FixedReaderType;
  FixedReaderType::Pointer                     fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);

  typedef otb::ImageFileReader<MovingImageType> MovingReaderType;
  MovingReaderType::Pointer                     mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);

  typedef itk::RecursiveGaussianImageFilter<FixedImageType, FixedImageType> FixedBlurType;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(atof(argv[5]));

  typedef itk::RecursiveGaussianImageFilter<MovingImageType, MovingImageType> MovingBlurType;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(atof(argv[5]));

  typedef otb::NCCRegistrationFilter<FixedImageType, MovingImageType, DisplacementFieldType> RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage(mBlur->GetOutput());
  registrator->SetFixedImage(fBlur->GetOutput());

  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType radius;

  radius[0] = atoi(argv[4]);
  radius[1] = atoi(argv[4]);

  registrator->SetNCCRadius(radius);

  registrator->SetNumberOfIterations(atoi(argv[6]));

  typedef otb::ImageFileWriter<DisplacementFieldType> DFWriterType;
  DFWriterType::Pointer                               dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(registrator->GetOutput());
  dfWriter->Update();

  return EXIT_SUCCESS;
}
