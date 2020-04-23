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


#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbGeodesicMorphologyIterativeDecompositionImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char*        inputFilename        = argv[1];
  const char*        outputFilenamePrefix = argv[2];
  const char*        outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels       = atoi(argv[4]);
  const unsigned int step                 = atoi(argv[5]);
  const unsigned int initValue            = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef double     InputPixelType;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension>                                     StructuringElementType;
  typedef otb::GeodesicMorphologyIterativeDecompositionImageFilter<InputImageType, StructuringElementType> DecompositionImageFilterType;
  typedef DecompositionImageFilterType::OutputImageListType::Iterator ImageListIterator;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  DecompositionImageFilterType::Pointer decomposition = DecompositionImageFilterType::New();
  decomposition->SetNumberOfIterations(numberOfLevels);
  decomposition->SetStep(step);
  decomposition->SetInitialValue(initValue);
  decomposition->SetInput(reader->GetOutput());
  decomposition->Update();

  // Retrieving iterators on the results images
  ImageListIterator itAnalyse    = decomposition->GetOutput()->Begin();
  ImageListIterator itConvexMap  = decomposition->GetConvexOutput()->Begin();
  ImageListIterator itConcaveMap = decomposition->GetConcaveOutput()->Begin();

  WriterType::Pointer writer;

  int                i = 1;
  std::ostringstream oss;
  // Writing the results images
  while ((itAnalyse != decomposition->GetOutput()->End()) && (itConvexMap != decomposition->GetConvexOutput()->End()) &&
         (itConcaveMap != decomposition->GetConcaveOutput()->End()))
  {
    oss << outputFilenamePrefix << "_leveling_" << i << "." << outputFilenameSuffix;
    writer = WriterType::New();
    writer->SetInput(itAnalyse.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_convMap_" << i << "." << outputFilenameSuffix;
    writer = WriterType::New();
    writer->SetInput(itConvexMap.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_concMap_" << i << "." << outputFilenameSuffix;
    writer = WriterType::New();
    writer->SetInput(itConcaveMap.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    ++itAnalyse;
    ++itConvexMap;
    ++itConcaveMap;
    ++i;
  }
  return EXIT_SUCCESS;
}
