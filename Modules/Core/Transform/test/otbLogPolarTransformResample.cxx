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

#include "otbLogPolarTransform.h"
#include "otbImage.h"
#include "itkResampleImageFilter.h"
#include "itkPoint.h"
#include "otbMacro.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLogPolarTransformResample(int itkNotUsed(argc), char* argv[])
{
  char * inputFileName = argv[1];
  char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                        PrecisionType;
  typedef double                                                        PixelType;
  typedef otb::Image<PixelType, Dimension>                              ImageType;
  typedef otb::LogPolarTransform<PrecisionType>                         LogPolarTransformType;
  typedef itk::LinearInterpolateImageFunction<ImageType, PrecisionType> InterpolatorType;
  typedef otb::ImageFileReader<ImageType>                               ReaderType;
  typedef otb::ImageFileWriter<ImageType>                               WriterType;
  typedef itk::ResampleImageFilter<ImageType, ImageType, PrecisionType> ResampleFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->UpdateOutputInformation();

  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  interpolator->SetInputImage(reader->GetOutput());

  std::cout << interpolator << std::endl;
  LogPolarTransformType::Pointer transform = LogPolarTransformType::New();

  LogPolarTransformType::ParametersType params(4);
  // Center the transform
  params[0] = 0.5 * static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]);
  params[1] = 0.5 * static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]);
  params[2] = 360. / 1024;
  params[3] =
    std::log(std::sqrt(std::pow(static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]), 2)
                     + std::pow(static_cast<double>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[
                                                     1]), 2)) / 2) / 512;
  transform->SetParameters(params);

  ImageType::SizeType size;
  size[0] = 1024;
  size[1] = 512;

  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  resampler->SetInput(reader->GetOutput());
  resampler->SetTransform(transform);
  resampler->SetInterpolator(interpolator);
  resampler->SetDefaultPixelValue(0);
  resampler->SetSize(size);

  ImageType::PointType origin;
  origin[0] = 0.5;
  origin[1] = 0.5;
  resampler->SetOutputOrigin(origin);

  ImageType::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = 1.0;
  resampler->SetOutputSpacing(spacing);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(resampler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
