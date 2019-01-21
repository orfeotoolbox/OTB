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

#include "otbWrapperApplicationHtmlDocGenerator.h"
#include "otbWrapperApplicationRegistry.h"


int otbWrapperApplicationHtmlDocGeneratorTest1(int argc, char * argv[])
{
  if (argc != 5)
    {
    std::cerr << "Usage : " << argv[0] << " module_name module_path outputfilename show_parameter_key" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Wrapper::ApplicationHtmlDocGenerator DocGeneratorType;

  // Add module path list
  otb::Wrapper::ApplicationRegistry::AddApplicationPath( argv[2] );

 // Create module
  otb::Wrapper::Application::Pointer app = otb::Wrapper::ApplicationRegistry::CreateApplication(argv[1]);

  if (app.IsNull())
    {
    std::cout << "Could not find application " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  app->Init();
  // Create the documentation output file
  DocGeneratorType::GenerateDoc( app, std::string(argv[3]), static_cast<bool>(atoi(argv[4])) );

  return EXIT_SUCCESS;
}
