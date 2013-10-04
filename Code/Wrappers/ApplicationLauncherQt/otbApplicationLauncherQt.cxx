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
#include <QtGui>
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperQtWidgetView.h"
#include "otbWrapperQtWidgetSimpleProgressReport.h"
#include "otbQtApplication.h"
#include "itksys/SystemTools.hxx"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;
using otb::Wrapper::QtWidgetView;
//using otb::Wrapper::QtWidgetProgressReport;
using otb::Wrapper::QtWidgetSimpleProgressReport;
using otb::Wrapper::QtApplication;

int main(int argc, char* argv[])
{
  // Handle UTF-8 filenames
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

  QtApplication qtApp(argc, argv);

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
      ApplicationRegistry::AddApplicationPath( *(it) );
      ++it;
      }
    }

  // Create module
  Application::Pointer app = ApplicationRegistry::CreateApplication(moduleName);
  if (app.IsNull())

    {
    std::cerr << "Could not find application " << moduleName << std::endl;

    const char* modulePath = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
    std::cout << "Module search path : " << (modulePath ? modulePath : "") << std::endl;
    std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();

    std::cout << "Available applications : " << (list.empty() ? "None" : "") << std::endl;
    for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
      {
      std::cout << "  " << *it << std::endl;
      }
    return EXIT_FAILURE;
    }

  // MainWidget : that contains the view and any other widget
  // (progress, logs...)
  QMainWindow* mainWindow =  new QMainWindow();
  mainWindow->setWindowIcon(QIcon( ":/otb_small.png" ));
  mainWindow->setWindowTitle(QString(app->GetDocName()).append(" - ").append(OTB_VERSION_STRING));

  // Create GUI based on module
  QtWidgetView* gui = new QtWidgetView(app);
  gui->CreateGui();

  // Connect the View "Quit" signal, to the mainWindow close slot
  QObject::connect(gui, SIGNAL(QuitSignal()), mainWindow, SLOT(close()));

  QObject::connect(&qtApp, SIGNAL(UnhandledException(QString)), gui, SLOT(UnhandledException(QString)));


  // build the main window, central widget is the plugin view, other
  // are docked widget (progress, logs...)
  mainWindow->setCentralWidget(gui);

  // Show the main window
  mainWindow->show();

  // Start event processing loop
  return qtApp.exec();
}
