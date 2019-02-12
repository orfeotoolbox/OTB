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

#include "itkMacro.h"
#include "otbMorphologicalPyramidSegmenter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSegmenter(int itkNotUsed(argc), char * argv[])
{
  const char*        inputFilename = argv[1];
  const char*        originalFilename = argv[2];
  const char*        outputFilename1 = argv[3];
  const bool         segmentDark = atoi(argv[4]);
  const float        seedsQuantile = atof(argv[5]);
  const float        segmentationQuantile = atof(argv[6]);
  const unsigned int minObjectSize = atoi(argv[7]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef unsigned short OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::MorphologicalPyramid::Segmenter<InputImageType, OutputImageType>
  SegmenterType;

  // Input images reading
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(originalFilename);

  // Instantiation
  SegmenterType::Pointer segmenter = SegmenterType::New();
  segmenter->SetDetailsImage(reader->GetOutput());
  segmenter->SetOriginalImage(reader2->GetOutput());
  segmenter->SetSegmentDarkDetailsBool(segmentDark);
  segmenter->SetSeedsQuantile(seedsQuantile);
  segmenter->SetConnectedThresholdQuantile(segmentationQuantile);
  segmenter->SetMinimumObjectSize(minObjectSize);

  // File writing
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(segmenter->GetOutput());
  writer->SetFileName(outputFilename1);
  writer->Update();

  return EXIT_SUCCESS;
}
