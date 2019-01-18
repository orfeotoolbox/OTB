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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperDocExampleStructure.h"
#include "itksys/SystemTools.hxx"
#include <fstream>



int otbWrapperDocExampleStructureTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::DocExampleStructure DocExampleStructureType;
  DocExampleStructureType::Pointer docStruct = DocExampleStructureType::New();

  docStruct->SetApplicationName("TestApplication");

  docStruct->SetExampleComment("Exmaple1", 0);

  docStruct->AddParameter( "key1", "val1_1" );
  docStruct->AddParameter( "key2", "val1_2" );

  docStruct->AddExample("Example2");
  docStruct->AddParameter( "key2", "val2_2", 1 );
  docStruct->AddParameter( "key3", "name3" );
  docStruct->AddParameter( "key3", "val2_3", 1 );

  std::ofstream ofs( argv[1] );
  if (!ofs.is_open())
    {
    fprintf(stderr, "Error, can't open file");
    return EXIT_FAILURE;
    }
  ofs << docStruct->GenerateCLExample();
  ofs.close();

  return EXIT_SUCCESS;
}
