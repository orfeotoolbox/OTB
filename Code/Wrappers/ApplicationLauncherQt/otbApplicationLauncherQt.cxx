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
#include <QApplication>
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperQtWidgetView.h"
#include "itksys/SystemTools.hxx"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationFactory;
using otb::Wrapper::QtWidgetView;

int main(int argc, char* argv[])
{
  QApplication qtApp(argc, argv);

  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " module_name [module_path]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string moduleName = argv[1];

  // Get the module path list
  std::list<std::string> modulePathList;
  if (argc > 2)
    {
    std::copy(argv + 2, argv + argc, std::back_inserter(modulePathList));

    // Load the path in the environment
    std::string specificEnv("ITK_AUTOLOAD_PATH=");
    std::list<std::string>::const_iterator it = modulePathList.begin();
    while( it != modulePathList.end() )
      {
      std::string modulePath = *it;

      specificEnv += *it;
      ++it;
      if (it != modulePathList.end())
        specificEnv += ":";
      }

    // do NOT use putenv() directly, since the string memory must be managed carefully
    itksys::SystemTools::PutEnv(specificEnv.c_str());
    }

  // Reload factories to take into account new path
  itk::ObjectFactoryBase::ReHash();

  // Create module
  Application::Pointer app = ApplicationFactory::CreateApplication(moduleName);
  if (app.IsNull())
    {
    std::cerr << "Could not find application " << moduleName << std::endl;

    const char* modulePath = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
    std::cout << "Module search path : " << (modulePath ? modulePath : "") << std::endl;

    std::list<std::string> list = ApplicationFactory::GetAvailableApplications();

    std::cout << "Available applications : " << (list.empty() ? "None" : "") << std::endl;
    for (std::list<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
      {
      std::cout << "  " << *it << std::endl;
      }
    return EXIT_FAILURE;
    }

  // Create GUI based on module
  QtWidgetView* gui = new QtWidgetView(app);
  gui->CreateGui();
  gui->show();

  // Start event processing loop
  return qtApp.exec();
}
