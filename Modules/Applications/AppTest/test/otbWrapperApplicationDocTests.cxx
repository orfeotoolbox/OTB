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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperApplicationRegistry.h"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;

typedef std::pair<std::string,std::string> DocElement;

bool CheckNonEmpty(const DocElement & elem)
{
  bool ret = true;
  if (elem.second == "")
    {
    std::cout << "  /!\\ Missing "<< elem.first << std::endl;
    ret = false;
    }
  return ret;
}

bool CheckMinimumSize(const DocElement & elem, unsigned int size)
{
  bool ret = true;
  if (elem.second.size() < size)
    {
    std::cout<<"  /!\\ " << elem.first <<" too small..."<<std::endl;
    ret = false;
    }
  return ret;
}

bool CheckNoNewline(const DocElement & elem)
{
  bool ret = true;
  if (elem.second.find('\n') != std::string::npos)
    {
    std::cout << "  /!\\ "<< elem.first << " should not contain newlines"<<std::endl;
    ret = false;
    }
  return ret;
}

int otbWrapperApplicationDocTest(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " [MODULEPATH]" << std::endl;
    return EXIT_FAILURE;
    }

  // Get the module path list
  std::list<std::string> modulePathList;
  if (argc > 1)
    {
    std::copy(argv + 1, argv + argc, std::back_inserter(modulePathList));

    // Load the path in the environment
    std::list<std::string>::const_iterator it = modulePathList.begin();
    while( it != modulePathList.end() )
      {
      ApplicationRegistry::AddApplicationPath( *(it) );
      ++it;
      }
    }

  bool isOK = true;
  
  // Get list of available applications
  std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();
  for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
    std::cout << "Testing documentation for application "<< (*it) << " ..." << std::endl;
    // Create module
    Application::Pointer app = ApplicationRegistry::CreateApplication(*it);
   
    // Check application availability
    if( app.IsNull() )
      {
      std::cout<<"Unable to load "<<argv[1]<<" application."<<std::endl;
      isOK = false;
      continue;
      }

    // Check doc element...
    DocElement name("Name",app->GetName());
    DocElement docName("Doc Name",app->GetDocName());
    DocElement description("Description",app->GetDescription());
    DocElement longDescription("Long description",app->GetDocLongDescription());
    DocElement authors("DocAuthors",app->GetDocAuthors());
    DocElement limitations("DocLimitations",app->GetDocLimitations());
    DocElement seeAlso("DocSeeAlso",app->GetDocSeeAlso());

    isOK = CheckNonEmpty(name) && isOK;
    isOK = CheckNoNewline(name) && isOK;

    isOK = CheckNonEmpty(docName) && isOK;
    isOK = CheckNoNewline(docName) && isOK;

    isOK = CheckNonEmpty(description) && isOK;

    isOK = CheckNonEmpty(longDescription) && isOK;
    isOK = CheckMinimumSize(longDescription,30) && isOK;

    isOK = CheckNonEmpty(authors) && isOK;

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
    }
  ApplicationRegistry::CleanRegistry();

  if( !isOK )
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}


