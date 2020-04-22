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

#include "itkMacro.h"

#include <iostream>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


#include "otbMaskMuParserFilter.h"

int otbMaskMuParserFilterTest(int argc, char* argv[])
{

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " infname outframe MuParserexpression" << std::endl;
    return EXIT_FAILURE;
  }

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];
  const char* expression     = argv[3];


  typedef float      InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::ImageFileReader<InputVectorImageType> ReaderType;
  typedef otb::Image<unsigned int, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::MaskMuParserFilter<InputVectorImageType, OutputImageType> FilterType;


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);


  filter->SetInput(reader->GetOutput());

  std::string stringExpression(expression);
  filter->SetExpression(stringExpression);
  filter->UpdateOutputInformation();


  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
