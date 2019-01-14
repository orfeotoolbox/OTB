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


#include <QtWidgets>
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperQtWidgetView.h"
#include "otbWrapperQtWidgetProgressReport.h"
#include "itksys/SystemTools.hxx"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;
using otb::Wrapper::QtWidgetView;
using otb::Wrapper::QtWidgetProgressReport;


int otbWrapperQtWidgetShowWidget(int argc, char* argv[])
{
  QApplication qtApp(argc, argv);

  std::cout << "Usage : " << argv[0] << " [module_path]" << std::endl;

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

  bool result = true;

  // Get list of available applications
  std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();
  for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
    std::cout << "Testing QtWidget for application "<< (*it) << " ..." << std::endl;
    // Create module
    Application::Pointer app = ApplicationRegistry::CreateApplication(*it);
    if (app.IsNull())
      {
      std::cerr << "Could not find application " << (*it) << std::endl;
      result = false;
      continue;
      }
  
    // MainWidget : that contains the view and any other widget
    // (progress, logs...)
    QMainWindow* mainWindow =  new QMainWindow();
  
    // Create GUI based on module
    QtWidgetView* gui = new QtWidgetView(app, mainWindow);
    gui->CreateGui();
  
    // Connect the View "Quit" signal, to the mainWindow close slot
    QObject::connect(gui, &QtWidgetView::QuitSignal, mainWindow, &QMainWindow::close);
  
    // Create a progressReport object
    QtWidgetProgressReport* progressReport =  new QtWidgetProgressReport(gui->GetModel(), mainWindow);
    progressReport->SetApplication(app);
  
    // Create a dock widget containing the progress widget
    QDockWidget* qdock = new QDockWidget("Progress Reporting ...", mainWindow);
    qdock->setWidget(progressReport);
  
    // build the main window, central widget is the plugin view, other
    // are docked widget (progress, logs...)
    mainWindow->setCentralWidget(gui);
    mainWindow->addDockWidget(Qt::BottomDockWidgetArea, qdock);
  
    // Show the main window
    mainWindow->show();
  
    QTimer::singleShot(1000, &qtApp, SLOT(quit()));
  
    // Start event processing loop
    if (qtApp.exec())
      {
      std::cerr << "Failed to show widget for application " << (*it) << std::endl;
      result = false;
      }
    
    // clean main window
    if (mainWindow) delete mainWindow;
    }

  ApplicationRegistry::CleanRegistry();
  
  if (result)
    {
    return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}
