/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbCommandLineArgumentParser.h"

// Images
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

// Concatener
#include "otbConcatenateVectorDataFilter.h"


typedef otb::VectorData<>                                 VectorDataType;
typedef otb::VectorDataFileReader<VectorDataType>         ReaderType;
typedef otb::VectorDataFileWriter<VectorDataType>         WriterType;
typedef otb::ConcatenateVectorDataFilter<VectorDataType>  ConcatenateFilterType;

int otbConcatenateVectorDataFilter (int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddOptionNParams("--InputVectorDatas","Input VectorDatas to concatenate ", "-in", true);
  parser->AddOption("--OutputVectorData","Output concatenated VectorData","-out", true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();
  parser->ParseCommandLine(argc, argv, parseResult);

  // Get number of input vectorDatas
  unsigned int nbInputs = parseResult->GetNumberOfParameters("--InputVectorDatas");

  // Instantiate a concatenate filter
    // Concatenate the vector datas
  ConcatenateFilterType::Pointer concatenate = ConcatenateFilterType::New();

  for (unsigned int idx = 0; idx < nbInputs; ++idx)
    {
    // Reader object
    ReaderType::Pointer reader = ReaderType::New();
    //std::cout << << std::endl;
    reader->SetFileName(parseResult->GetParameterString("--InputVectorDatas", idx ));
    reader->Update();

    concatenate->AddInput(reader->GetOutput());
    }

  // Write the output
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(parseResult->GetParameterString("--OutputVectorData"));
  writer->SetInput(concatenate->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
