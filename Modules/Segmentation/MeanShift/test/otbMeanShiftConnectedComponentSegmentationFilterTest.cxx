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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftConnectedComponentSegmentationFilter.h"

typedef float      InputPixelType;
const unsigned int Dimension = 2;
typedef otb::Image<unsigned int, Dimension> LabelImageType;
typedef otb::Image<unsigned int, Dimension> MaskImageType;


typedef otb::VectorImage<InputPixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType>      ReaderType;
typedef otb::ImageFileWriter<LabelImageType> WriterType;


typedef otb::MeanShiftConnectedComponentSegmentationFilter<ImageType, MaskImageType, LabelImageType> MeanShiftConnectedComponentsegmentationFilterType;

typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> MeanShiftFilterType;


int otbMeanShiftConnectedComponentSegmentationFilter(int itkNotUsed(argc), char* argv[])
{

  /* mean shift parameters */

  const char* infname        = argv[1];
  const char* outputFilename = argv[2];

  /* mean shift parameters */
  const double spatialBandwidth = atof(argv[3]);
  const double rangeBandwidth   = atof(argv[4]);
  const double threshold        = atof(argv[5]);

  /* conencted component parameters */

  const char*  segmentationexpression = argv[6];
  unsigned int minobjectsize          = atoi(argv[7]);

  // add meanshift options


  // Instantiating object
  MeanShiftFilterType::Pointer                               meanShiftFilter    = MeanShiftFilterType::New();
  MeanShiftConnectedComponentsegmentationFilterType::Pointer segmentationFilter = MeanShiftConnectedComponentsegmentationFilterType::New();
  ReaderType::Pointer                                        reader             = ReaderType::New();
  WriterType::Pointer                                        writer             = WriterType::New();

  reader->SetFileName(infname);

  meanShiftFilter->SetSpatialBandwidth(spatialBandwidth);

  meanShiftFilter->SetRangeBandwidth(rangeBandwidth);
  meanShiftFilter->SetThreshold(threshold);

  meanShiftFilter->SetInput(reader->GetOutput());
  meanShiftFilter->GetRangeOutput();

  // set the mean shift filter
  segmentationFilter->SetMeanShiftFilter(meanShiftFilter);


  segmentationFilter->SetInput(meanShiftFilter->GetRangeOutput());

  segmentationFilter->SetConnectedComponentExpression(segmentationexpression);
  segmentationFilter->SetMinimumObjectSize(minobjectsize);

  // segmentationFilter->Update();

  writer->SetInput(segmentationFilter->GetOutput());
  writer->SetFileName(outputFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
