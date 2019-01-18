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

#include "otbImage.h"
#include "otbEdgeDetectorImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkSobelEdgeDetectionImageFilter.h"

int otbEdgeDetectorImageFilter(int itkNotUsed(argc), char* argv[])
{
  typedef double                          PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef itk::SobelEdgeDetectionImageFilter<ImageType, ImageType>               SobelDetectionType;
  typedef otb::EdgeDetectorImageFilter<ImageType, ImageType, SobelDetectionType> DetectorType;

  /**Instantiation of an object*/
  ReaderType::Pointer         reader      = ReaderType::New();
  WriterType::Pointer         writer      = WriterType::New();
  DetectorType::Pointer       detector    = DetectorType::New();
  SobelDetectionType::Pointer sobelFilter = SobelDetectionType::New();

  reader->SetFileName(argv[1]);
  detector->SetInput(reader->GetOutput());

  detector->SetLowerThreshold(static_cast<PixelType>(atof(argv[3])));
  detector->SetUpperThreshold(static_cast<PixelType>(atof(argv[4])));

  writer->SetFileName(argv[2]);
  writer->SetInput(detector->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
