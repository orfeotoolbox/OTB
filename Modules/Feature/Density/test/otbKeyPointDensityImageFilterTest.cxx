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


#include <stdio.h>

#include "otbKeyPointDensityImageFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbKeyPointDensityImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char*        infname  = argv[1];
  const char*        outfname = argv[2];
  const unsigned int scales   = atoi(argv[3]);
  const unsigned int octaves  = atoi(argv[4]);
  const unsigned int radius   = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef float      PixelType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>      ReaderType;
  typedef otb::ImageFileWriter<ImageType>      WriterType;
  typedef itk::VariableLengthVector<PixelType> RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> DetectorType;

  typedef otb::KeyPointDensityImageFilter<ImageType, ImageType, DetectorType> FilterType;

  /**Instantiation of an object*/
  FilterType::Pointer   filter   = FilterType::New();
  DetectorType::Pointer detector = DetectorType::New();
  ReaderType::Pointer   reader   = ReaderType::New();

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  filter->SetInput(reader->GetOutput());
  detector = filter->GetDetector();
  detector->SetOctavesNumber(octaves);
  detector->SetScalesNumber(scales);

  filter->SetDetector(detector);
  filter->SetNeighborhoodRadius(radius);

  /** Writing the densty Image*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
