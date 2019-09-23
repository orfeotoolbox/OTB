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

#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

typedef float      InputPixelType;
const unsigned int Dimension = 2;

typedef otb::Image<unsigned int, Dimension> LabelImageType;
typedef otb::Image<unsigned int, Dimension> MaskImageType;

typedef otb::VectorData<double, Dimension> VectorDataType;
typedef VectorDataType::Pointer                   VectorDataPointerType;
typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;
typedef VectorDataFileWriterType::Pointer         VectorDataFileWriterPointerType;

typedef otb::VectorImage<InputPixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;


typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter<ImageType, LabelImageType, MaskImageType, VectorDataType>
    ConnectedComponentSegmentationOBIAToVectorDataFilterType;

typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> MeanShiftFilterType;


int otbMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter(int itkNotUsed(argc), char* argv[])
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
  const char*  obiaexpression         = argv[8];
  unsigned int nbstreams              = atoi(argv[9]);

  // add meanshift options


  // Instantiating object
  MeanShiftFilterType::Pointer meanShiftFilter = MeanShiftFilterType::New();
  ReaderType::Pointer          reader          = ReaderType::New();

  reader->SetFileName(infname);

  meanShiftFilter->SetSpatialBandwidth(spatialBandwidth);
  meanShiftFilter->SetRangeBandwidth(rangeBandwidth);

  meanShiftFilter->SetThreshold(threshold);

  meanShiftFilter->SetInput(reader->GetOutput());
  meanShiftFilter->GetRangeOutput();

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected =
      ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(meanShiftFilter->GetRangeOutput());

  connected->GetFilter()->SetConnectedComponentExpression(segmentationexpression);
  connected->GetFilter()->SetMinimumObjectSize(minobjectsize);
  connected->GetFilter()->SetOBIAExpression(obiaexpression);

  connected->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(nbstreams);
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(outputFilename);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
