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

#include <iostream>
#include "otbVectorImage.h"
#include "otbImage.h"

#include "otbConnectedComponentMuParserFunctor.h"
#include <itkConnectedComponentFunctorImageFilter.h>


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbConnectedComponentMuParserFunctorTest(int argc, char* argv[])
{

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];
  const char* expression     = argv[3];
  const char* maskFilename;

  // maskFilename option is present
  if (argc == 5)
  {
    maskFilename = argv[4];
  }
  else
  {
    maskFilename = nullptr;
  }

  typedef float      InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned int, Dimension>         InputMaskImageType;
  typedef otb::ImageFileReader<InputVectorImageType> ReaderType;
  typedef otb::ImageFileReader<InputMaskImageType>   MaskReaderType;
  typedef otb::Image<unsigned int, Dimension> OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::Functor::ConnectedComponentMuParserFunctor<InputVectorImageType::PixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<InputVectorImageType, OutputImageType, FunctorType, InputMaskImageType> FilterType;


  MaskReaderType::Pointer maskReader;
  ReaderType::Pointer     reader = ReaderType::New();
  WriterType::Pointer     writer = WriterType::New();
  FilterType::Pointer     filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());

  if (nullptr != maskFilename)
  {
    maskReader = MaskReaderType::New();
    maskReader->SetFileName(maskFilename);
    maskReader->GenerateOutputInformation();

    filter->SetMaskImage(maskReader->GetOutput());
  }


  std::string stringExpression(expression);

  filter->GetFunctor().SetExpression(stringExpression);
  filter->Update();

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
