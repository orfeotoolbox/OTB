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

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplicationsToolBox.h"
#include "mvdApplicationsBrowser.h"
#include "mvdApplicationLauncher.h"

int mvdFillToolBoxWidgetTreeTest(int argc, char* argv[])
{ 
  //
  // instantiate the algorithms toolbox widget
  QApplication app(argc, argv);
  mvd::ApplicationsToolBox *  appToolBox = new mvd::ApplicationsToolBox();

  //
  // instantiate applications browser
  mvd::ApplicationsBrowser *  browser = new mvd::ApplicationsBrowser();

  //
  // connections
  QObject::connect(browser, 
                   SIGNAL( AvailableApplicationsTagsChanged(const ApplicationsTagContainer& ) ),
                   appToolBox,
                   SLOT( OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer& ) )
    );

  //
  // is search path set
  if (argc > 1)
    {
    browser->SetAutoLoadPath(argv[1]);
    }

  //
  // get the tags/algs map container in the widget
  browser->SearchAvailableApplicationsTags();

  //
  // instantiate Application launcher
  mvd::ApplicationLauncher * launcher = new mvd::ApplicationLauncher();
    
  //
  // connections
  QObject::connect(appToolBox, 
                   SIGNAL( ApplicationToLaunchSelected(const QString& ) ),
                   launcher,
                   SLOT( OnApplicationToLaunchSelected(const QString& ) )
    );

  // add quit
  // QObject::connect(appToolBox, 
  //                  SIGNAL( (const QString& ) ),
  //                  launcher,
  //                  SLOT( OnQuit() )
  //   );

  //
  // show the application for a while
  appToolBox->show();
  if ( argc <= 2 )
    {
    QTimer::singleShot(1000, &app, SLOT(quit()));
    }
  return app.exec();
}
