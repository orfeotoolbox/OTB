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

#include "otbWrapperApplicationRegistry.h"

int otbWrapperApplicationDocTest(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " module_name [MODULEPATH]" << std::endl;
    return EXIT_FAILURE;
    }
  // Load path
  for( int i=2; i<argc; i++)
    {
    otb::Wrapper::ApplicationRegistry::AddApplicationPath(argv[i]);
    }

  // Load application
  otb::Wrapper::Application::Pointer app = otb::Wrapper::ApplicationRegistry::CreateApplication(argv[1]);
  // Check application availability
  if( app.IsNull() )
    {
    std::cout<<"Unable to load "<<argv[1]<<" application."<<std::endl;
    return EXIT_FAILURE;
    }

  bool isOK = true;
  // Check doc element...
  if( std::string(app->GetName()) == "" )
    {
    std::cout<<"Missing Name."<<std::endl;
    isOK = false;
    }
  if( std::string(app->GetDescription()) == "" )
    {
    std::cout<<"Missing Description."<<std::endl;
    isOK = false;
    }
  if( std::string(app->GetDocLongDescription()) == "" )
    {
    std::cout<<"Missing DocLongDescription."<<std::endl;
    isOK = false;
    }
  else if( std::string(app->GetDocLongDescription()).size() < 30 )
    {
    std::cout<<"DocLongDescription too small..."<<std::endl;
    isOK = false;
    }
   if( std::string(app->GetDocAuthors()) == "" )
    {
    std::cout<<"Missing DocAuthors."<<std::endl;
    isOK = false;
    }
   if( std::string(app->GetDocLimitations()) == "" )
    {
    std::cout<<"Missing DocLimitations."<<std::endl;
    isOK = false;
    }
   if( std::string(app->GetDocSeeAlso()) == "" )
    {
    std::cout<<"Missing DocSeeAlso."<<std::endl;
    isOK = false;
    }
 if( app->GetDocTags().size() == 0 )
    {
    std::cout<<"Missing DocTags."<<std::endl;
    isOK = false;
    }

  // Check example data
  app->Init();
  otb::Wrapper::DocExampleStructure::Pointer doc = app->GetDocExample();
  if( doc->GetApplicationName() == "" )
    {
    std::cout<<"Error in doc example: no application name found."<<std::endl;
    isOK = false;
    }
  if( doc->GetParameterList().size() == 0 )
    {
    std::cout<<"Error in doc example: the list of parameter is empty."<<std::endl;
    isOK = false;
    }
  else
    {
    bool hasValue = false;
    unsigned int count = 0;
    while ( count<doc->GetParameterList().size() && !hasValue )
      {
      if( doc->GetParameterValue(count) != "" )
        {
        hasValue = true;
        }
      count++;
      }

    if( !hasValue )
      {
      std::cout<<"Error in doc example: no value for the example found !!!"<<std::endl;
      isOK = false;
      }
    }


 if( !isOK )
   return EXIT_FAILURE;

  return EXIT_SUCCESS;
}


