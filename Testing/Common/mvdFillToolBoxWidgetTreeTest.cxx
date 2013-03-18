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
//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "ApplicationsWrapper/mvdApplicationsToolBox.h"
#include "ApplicationsWrapper/mvdApplicationsBrowser.h"
#include "ApplicationsWrapper/mvdApplicationLauncher.h"

int mvdFillToolBoxWidgetTreeTest(int argc, char* argv[])
{ 
  //
  // instanciate the algorithms toolbox widget
  QApplication app(argc, argv);
  mvd::ApplicationsToolBox *  appToolBox = new mvd::ApplicationsToolBox();

  //
  // instanciate applications browser
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
  // instanciate Application launcher
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
