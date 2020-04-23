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

#include "otbMeanShiftSegmentationFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbMeanShiftSegmentationFilter(int argc, char* argv[])
{
  if (argc != 8 && argc != 9)
  {
    std::cerr << "Usage: " << argv[0]
              << " inputFileName outputLabelFileName outputClusteredFileName spatialBandwidth rangeBandwidth maxIter threshold minregionsize" << std::endl;
    return EXIT_FAILURE;
  }

  const char*        inputFileName           = argv[1];
  const char*        outputLabelFileName     = argv[2];
  const char*        outputClusteredFileName = argv[3];
  const double       spatialBandwidth        = atof(argv[4]);
  const double       rangeBandwidth          = atof(argv[5]);
  const unsigned int maxIter                 = atoi(argv[6]);
  const double       threshold               = atof(argv[7]);

  double minregionsize = 0;
  if (argc == 9)
    minregionsize = atof(argv[8]);

  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::Image<unsigned int, Dimension>    LabelImageType;
  typedef otb::ImageFileReader<ImageType>      ReaderType;
  typedef otb::ImageFileWriter<ImageType>      ClusteredWriterType;
  typedef otb::ImageFileWriter<LabelImageType> LabelWriterType;

  typedef otb::MeanShiftSegmentationFilter<ImageType, LabelImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFileName);

  // Set filter parameters
  filter->SetSpatialBandwidth(spatialBandwidth);
  filter->SetRangeBandwidth(rangeBandwidth);
  filter->SetMaxIterationNumber(maxIter);
  filter->SetThreshold(threshold);
  filter->SetMinRegionSize(minregionsize);
  filter->SetInput(reader->GetOutput());

  // filter->SetNumberOfThreads(1);
  ClusteredWriterType::Pointer writerClustered = ClusteredWriterType::New();
  LabelWriterType::Pointer     writerLabel     = LabelWriterType::New();

  writerClustered->SetFileName(outputClusteredFileName);
  writerLabel->SetFileName(outputLabelFileName);

  writerClustered->SetInput(filter->GetClusteredOutput());
  writerLabel->SetInput(filter->GetLabelOutput());

  writerClustered->Update();
  writerLabel->Update();

  return EXIT_SUCCESS;
}
