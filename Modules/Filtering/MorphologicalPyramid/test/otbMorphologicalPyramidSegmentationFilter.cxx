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
#include "otbMorphologicalPyramidSegmentationFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSegmentationFilter(int itkNotUsed(argc), char * argv[])
{
  const char*        inputFilename = argv[1];
  const char*        outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels = atoi(argv[4]);
  const double       decimationRatio = atof(argv[5]);
  const float        seedsQuantile = atof(argv[6]);
  const float        segmentationQuantile = atof(argv[7]);
  const unsigned int minObjectSize = atoi(argv[8]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef unsigned short OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningClosingFilterType;
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType, InputImageType, OpeningClosingFilterType>
  PyramidFilterType;
  typedef otb::MorphologicalPyramidSegmentationFilter<InputImageType, OutputImageType>
  SegmentationFilterType;
  typedef SegmentationFilterType::OutputImageListIteratorType OutputListIteratorType;

  // Input images reading
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  //Analyse
  PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
  pyramid->SetNumberOfLevels(numberOfLevels);
  pyramid->SetDecimationRatio(decimationRatio);
  pyramid->SetInput(reader->GetOutput());

  // Segmentation
  SegmentationFilterType::Pointer segmentation = SegmentationFilterType::New();
  segmentation->SetReferenceImage(reader->GetOutput());
  segmentation->SetBrighterDetails(pyramid->GetSupFilter());
  segmentation->SetDarkerDetails(pyramid->GetInfFilter());
  segmentation->SetSeedsQuantile(seedsQuantile);
  segmentation->SetConnectedThresholdQuantile(segmentationQuantile);
  segmentation->SetMinimumObjectSize(minObjectSize);
  segmentation->Update();

  // Output writing
  OutputListIteratorType it = segmentation->GetOutput()->Begin();
  WriterType::Pointer    writer;
  int                    index = 1;
  std::stringstream      oss;
  while (it != segmentation->GetOutput()->End())
    {
    oss << outputFilenamePrefix << index << "." << outputFilenameSuffix;
    writer = WriterType::New();
    writer->SetInput(it.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    std::cout << oss.str() << " file written." << std::endl;
    oss.str("");
    ++index;
    ++it;
    }

  return EXIT_SUCCESS;
}
