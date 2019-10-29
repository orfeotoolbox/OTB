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


#include <math.h>
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStopwatch.h"
#include "otbConvolutionImageFilter.h"
#include "otbOverlapSaveConvolutionImageFilter.h"
#include "otbGaborFilterGenerator.h"
#include "itkConstantBoundaryCondition.h"

int otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter(int argc, char* argv[])
{
  if (argc != 12)
  {
    std::cerr << "Usage: " << argv[0] << " infname outfname1 outfname2 xradius yradius a b theta u0 v0 phi" << std::endl;
    return EXIT_FAILURE;
  }

  const char*        infname   = argv[1];
  const char*        outfname1 = argv[2];
  const char*        outfname2 = argv[3];
  const unsigned int xradius   = atoi(argv[4]);
  const unsigned int yradius   = atoi(argv[5]);
  const double       a         = atof(argv[6]);
  const double       b         = atof(argv[7]);
  const double       theta     = atof(argv[8]);
  const double       u0        = atof(argv[9]);
  const double       v0        = atof(argv[10]);
  const double       phi       = atof(argv[11]);

  typedef double                                   PrecisionType;
  typedef otb::GaborFilterGenerator<PrecisionType> GaborGeneratorType;
  typedef GaborGeneratorType::RadiusType           RadiusType;
  typedef GaborGeneratorType::ArrayType            ArrayType;

  typedef otb::Image<PrecisionType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  typedef otb::OverlapSaveConvolutionImageFilter<ImageType, ImageType> OSConvolutionFilterType;
  // Setting the same boundary conditions than the one used in the overlap save convolution filter
  typedef itk::ConstantBoundaryCondition<ImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<ImageType, ImageType, BoundaryConditionType> ConvolutionFilterType;

  // Gabor filter generation
  GaborGeneratorType::Pointer gabor = GaborGeneratorType::New();
  RadiusType                  radius;
  radius[0] = xradius;
  radius[1] = yradius;
  gabor->SetRadius(radius);
  gabor->SetA(a);
  gabor->SetB(b);
  gabor->SetTheta(theta);
  gabor->SetU0(u0);
  gabor->SetV0(v0);
  gabor->SetPhi(phi);
  ArrayType filter = gabor->GetFilter();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ConvolutionFilterType::Pointer convolution = ConvolutionFilterType::New();
  convolution->SetRadius(radius);
  convolution->SetFilter(filter);
  convolution->SetInput(reader->GetOutput());

  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetInput(convolution->GetOutput());
  writer1->SetFileName(outfname1);

  otb::Stopwatch chrono = otb::Stopwatch::StartNew();
  writer1->Update();

  std::cout << "Classical convolution algorithm took " << chrono.GetElapsedMilliseconds() << " ms." << std::endl;

  OSConvolutionFilterType::Pointer osconvolution = OSConvolutionFilterType::New();
  osconvolution->SetRadius(radius);
  osconvolution->SetFilter(filter);
  osconvolution->SetInput(reader->GetOutput());

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(osconvolution->GetOutput());
  writer2->SetFileName(outfname2);

  chrono.Restart();
  writer2->Update();

  std::cout << "Overlap-save convolution algorithm took " << chrono.GetElapsedMilliseconds() << " ms." << std::endl;

  return EXIT_SUCCESS;
}
