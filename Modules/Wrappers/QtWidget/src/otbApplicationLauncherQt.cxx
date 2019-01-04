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

#include <QtWidgets>
#include "otbConfigurationManager.h"
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
  /*/////////////////////////////// Warning //////////////////////
  we need to replace those lines with a function of Qt5
  // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  // QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  //////////////////////////////////////////////////////////////////*/
  QtApplication qtApp(argc, argv);

  otb::ConfigurationManager::InitOpenMPThreads();

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
    std::string modulePath = ApplicationRegistry::GetApplicationPath();
    std::cout << "Module search path : " << modulePath << std::endl;
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
  QtWidgetView* gui = new QtWidgetView(app, mainWindow);
  gui->CreateGui();

  // Connect the View "Quit" signal, to the mainWindow close slot
  QObject::connect(gui, &QtWidgetView::QuitSignal, mainWindow, &QMainWindow::close);

  QObject::connect(&qtApp, &QtApplication::UnhandledException, gui, &QtWidgetView::UnhandledException);


  // build the main window, central widget is the plugin view, other
  // are docked widget (progress, logs...)
  mainWindow->setCentralWidget(gui);

  // Show the main window
  mainWindow->show();

  // Start event processing loop
  int ret = qtApp.exec();

  // Clean resources
  delete mainWindow;
  app = nullptr;
  ApplicationRegistry::CleanRegistry();

  return ret;
}
