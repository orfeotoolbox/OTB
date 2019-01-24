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

#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbOpeningClosingMorphologicalFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  const unsigned int Radius = atoi(argv[3]);

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, OutputImageType,
      StructuringElementType> OpeningClosingFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Creating Kernel
  StructuringElementType structElt;
  structElt.SetRadius(Radius);
  structElt.CreateStructuringElement();

  // Instantiating the opening closing filter
  OpeningClosingFilterType::Pointer openingClosing = OpeningClosingFilterType::New();
  openingClosing->SetInput(reader->GetOutput());
  openingClosing->SetKernel(structElt);

  // Write the result to file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(openingClosing->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
