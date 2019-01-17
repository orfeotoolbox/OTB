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

#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbMorphologicalPyramidMRToMSConverter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidMRToMSConverter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename1 = argv[2];
  const char *       outputFilename2 = argv[3];
  const char *       outputFilename3 = argv[4];
  const unsigned int numberOfLevels = atoi(argv[5]);
  const float        decimationRatio = atof(argv[6]);

  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef double OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningClosingFilterType;
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType, InputImageType, OpeningClosingFilterType>
  PyramidAnalysisFilterType;
  typedef otb::MorphologicalPyramid::MRToMSConverter<InputImageType, OutputImageType> MRToMSConverterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Analysis
  PyramidAnalysisFilterType::Pointer pyramidAnalysis = PyramidAnalysisFilterType::New();
  pyramidAnalysis->SetNumberOfLevels(numberOfLevels);
  pyramidAnalysis->SetDecimationRatio(decimationRatio);
  pyramidAnalysis->SetInput(reader->GetOutput());

  // From multi resolution to multi scale
  MRToMSConverterType::Pointer mrtoms = MRToMSConverterType::New();
  mrtoms->SetInput(pyramidAnalysis->GetOutput());
  mrtoms->SetSupFilter(pyramidAnalysis->GetSupFilter());
  mrtoms->SetSupDeci(pyramidAnalysis->GetSupDeci());
  mrtoms->SetInfFilter(pyramidAnalysis->GetInfFilter());
  mrtoms->SetInfDeci(pyramidAnalysis->GetInfDeci());
  mrtoms->Update();

  // Writing the output images
  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetFileName(outputFilename1);
  writer1->SetInput(mrtoms->GetSupFilterFullResolution()->Back());
  writer1->Update();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName(outputFilename2);
  writer2->SetInput(mrtoms->GetInfFilterFullResolution()->Back());
  writer2->Update();

  WriterType::Pointer writer3 = WriterType::New();
  writer3->SetFileName(outputFilename3);
  writer3->SetInput(mrtoms->GetOutput()->Back());
  writer3->Update();

  return EXIT_SUCCESS;
}
