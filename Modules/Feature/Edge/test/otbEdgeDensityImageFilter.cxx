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
#include "otbEdgeDensityImageFilter.h"
#include "otbBinaryImageDensityFunction.h"
#include "itkCannyEdgeDetectionImageFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbEdgeDensityImageFilter(int itkNotUsed(argc), char* argv[])
{

  const char*        infname  = argv[1];
  const char*        outfname = argv[2];
  const unsigned int radius   = atoi(argv[3]);

  /** Variables for the canny detector*/
  const double upperThreshold = atof(argv[4]);
  const double lowerThreshold = atof(argv[5]);
  const double variance       = atof(argv[6]);
  const double maximumError   = atof(argv[7]);
  /*--*/

  const unsigned int Dimension = 2;
  typedef float      PixelType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::BinaryImageDensityFunction<ImageType> CountFunctionType;
  typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType> CannyDetectorType;

  typedef otb::EdgeDensityImageFilter<ImageType, ImageType, CannyDetectorType, CountFunctionType> EdgeDensityFilterType;

  /**Instantiation of an object*/
  EdgeDensityFilterType::Pointer filter      = EdgeDensityFilterType::New();
  ReaderType::Pointer            reader      = ReaderType::New();
  CannyDetectorType::Pointer     CannyFilter = CannyDetectorType::New();

  /** Set The input*/
  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());

  /** Update the Canny Filter Information*/
  CannyFilter->SetUpperThreshold(static_cast<ImageType::PixelType>(upperThreshold)); /** 30*/
  CannyFilter->SetLowerThreshold(static_cast<ImageType::PixelType>(lowerThreshold)); /** 10*/
  CannyFilter->SetVariance(variance);                                                // 1.
  CannyFilter->SetMaximumError(maximumError);                                        /// 0.01f

  filter->SetDetector(CannyFilter);
  filter->SetNeighborhoodRadius(radius);

  /** Write the output*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
