/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCatalogueApplication.h"
#include "mvdMainWindow.h"

/*****************************************************************************/
/* FUNCTIONS DECLARATION                                                     */


/*****************************************************************************/
/* MAIN                                                                      */

int
main( int argc, char* argv[] )
{
  // 1. Initialize application and sync settings.
  mvd::CatalogueApplication application( argc, argv );
  application.Initialize();

  // 2. Initialize main-window (UI).
  mvd::MainWindow mainWindow;
  mainWindow.Initialize();

  // 3. Initialize cache directory.
#if 0
// TODO: 1) MVD2-viewer: Factorize cache-dir search between Viewer & Catalogue.
  if (!appHasSettingsFile || appHasIncorrectCacheDir)
    {
    // Loop until the directory will be correct
    while (true)
      {
      // Select a new location for the cache director
      try
        {
        // Create the cache directory
        application.MakeCacheDir(mainWindow.SelectCacheDir(appHasIncorrectCacheDir));
        break;
        }
      catch (...)
        {
        appHasIncorrectCacheDir = true;
        }
      }
    // Save the cache directory into the settings file
    application.WriteCacheDirIntoSettings();
    }
#endif

  // 4. Initialize database.
  application.OpenDatabase();

  // 5. Show window.
#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();
#else
  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();
#endif


  // 6. Let's go: run the application and return exit code.
  return application.exec();
}


/*****************************************************************************/
/* FUNCTIONS IMPLEMENTATION                                                  */
