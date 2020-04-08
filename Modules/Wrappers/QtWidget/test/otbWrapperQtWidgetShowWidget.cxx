/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbQtApplication.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperQtWidgetView.h"
#include "itksys/SystemTools.hxx"


using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;
using otb::Wrapper::QtApplication;
using otb::Wrapper::QtWidgetView;


struct static_finalizer
{
  ~static_finalizer()
  {
    ApplicationRegistry::CleanRegistry();
  }
};


namespace
{
static_finalizer finalizer;
}


int otbWrapperQtWidgetShowWidget(int argc, char* argv[])
{
  std::cout << "Usage: " << argv[0] << " [module_path]" << std::endl;

  // Register module-path list.
  std::for_each(
      // std::advance( std::begin( argv ) ),
      // std::end( argv ),
      argv + 1, argv + argc, [](auto module_path) { ApplicationRegistry::AddApplicationPath(module_path); });

  {
    // Create OTB-application.
    Application::Pointer otb_application = ApplicationRegistry::CreateApplication("TestApplication");

    if (!otb_application)
    {
      std::cerr << "Could not find application: 'TestApplication'" << std::endl;

      return EXIT_FAILURE;
    }

    // Qt application.
    QtApplication qt_app(argc, argv);

    qt_app.setQuitOnLastWindowClosed(true);

    // Qt main-window.
    QMainWindow main_window; // Stack instance acts as a
                             // scoped-pointer inside {}.

    // OTB-Application GUI.
    {
      QWidget* widget = new QWidget(&main_window);
      {
        QVBoxLayout* layout = new QVBoxLayout(widget);
        {
          // Create OTB-Application widget.
          //
          // SAT: QWidget should be created without parent when adding
          // into QLayout (because QLayout will take ownership of the
          // reference-counted pointer) but OTB API doesn't defined default nullptr
          // value such as in Qt.
          QtWidgetView* qwv = new QtWidgetView(otb_application, widget);

          // SAT: Should be automatically done in QtWidgetView().
          qwv->CreateGui();

          // Connect the view to main-window.
          QObject::connect(qwv, SIGNAL(QuitSignal()), &main_window, SLOT(close()));

          layout->addWidget(qwv);
        }
        widget->setLayout(layout);
      }
      main_window.setCentralWidget(widget);
    }

    // Show the main window
    main_window.show();

    QTimer::singleShot(1000, &qt_app, SLOT(quit()));

    // Start event processing loop
    if (qt_app.exec())
    {
      std::cerr << "Failed to execute Qt event-loop for application 'TestApplication'" << std::endl;

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
