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

#include "otbCommandLineArgumentParser.h"
#include "otbOSMDataToVectorDataGenerator.h"
#include "otbVectorDataFileWriter.h"


typedef otb::OSMDataToVectorDataGenerator                     FilterType;
typedef otb::VectorDataFileWriter<FilterType::VectorDataType> VectorDataFileWriterType;


int otbOSMToVectorDataGeneratorTest (int itkNotUsed(argc), char * argv[])
{
  FilterType::Pointer filter = FilterType::New();
  filter->SetFileName(argv[1]);
  filter->SetUseUrl(false);
  filter->SetNorth(43.62811);
  filter->SetSouth(43.60185);
  filter->SetWest(1.54911);
  filter->SetEast(1.59323);
  filter->Update();

  // Write the IndexTile
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetInput(filter->GetVectorDataByName("building"));
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbOSMToVectorDataGeneratorByName (int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddOption("--OSMFile","The osm file containig the points to extract",
                    "-osm", 1, true);

  parser->AddOption("--OutputVectorData","The output vectordata",
                    "-vd", 1, true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
    {
    parser->ParseCommandLine(argc, argv, parseResult);
    }
  catch ( itk::ExceptionObject & )
    {
    return EXIT_FAILURE;
    }

  FilterType::Pointer filter = FilterType::New();
  filter->SetFileName(parseResult->GetParameterString("--OSMFile"));
  filter->SetUseUrl(false);

  // Set the extent of the request
  filter->SetNorth(43.62811);
  filter->SetSouth(43.60185);
  filter->SetWest(1.54911);
  filter->SetEast(1.59323);

  filter->Update();

  const FilterType::VectorDataType*  v1 = filter->GetVectorDataByName("waterway");
  std::cout <<"Size of the vectordata v1 : "<< v1->Size() << std::endl;

  const FilterType::VectorDataType*  v2 = filter->GetVectorDataByName("highway");
  std::cout <<"Size of the vectordata v2 : "<< v2->Size() << std::endl;


  const FilterType::VectorDataType*  v3 = filter->GetVectorDataByName("building");
  std::cout <<"Size of the vectordata v3 : "<< v3->Size() << std::endl;

  const FilterType::VectorDataType*  v4 = filter->GetVectorDataByName("highway","motorway_link");
  std::cout <<"Size of the vectordata v4 : "<< v4->Size() << std::endl;

  // Write the VectorData
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(parseResult->GetParameterString("--OutputVectorData"));
  writer->SetInput(v4);
  writer->Update();


  return EXIT_SUCCESS;
}
