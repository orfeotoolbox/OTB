/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperApplicationHtmlDocGenerator.h"
#include "otbWrapperApplicationRegistry.h"

int otbWrapperApplicationHtmlDocGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::ApplicationHtmlDocGenerator DocGeneratorType;
  DocGeneratorType generator;

  return EXIT_SUCCESS;
}

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
