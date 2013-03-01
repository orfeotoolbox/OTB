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

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"
#include "mvdDatasetModel.h"

//
// OTB includes (sorted by alphabetic order)

//
// MAIN
//
int
main( int argc, char* argv[] )
{
  mvd::Application application( argc, argv );

  //
  // Force numeric options of locale to "C"
  // See issue #635
  //
  // TODO: Move into I18nApplication.
  setlocale( LC_NUMERIC, "C" );

  // Check if the application have a settings file already available
  bool appHasSettingsFile = application.HasSettingsFile();
  bool appHasIncorrectCacheDir(false);
  if (appHasSettingsFile)
    {
    // Read cache dir from settings
    application.ReadCacheDirFromSettings();
    // Check the cache dir
    if (!application.CheckCacheDirIsCorrect() )
      {
      appHasIncorrectCacheDir = true;
      }
    }
  else // TODO MSD: should be removed
    {
    std::cout << "Application has no settings file";
    }

  mvd::MainWindow mainWindow;

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



#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();
#else
  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();
#endif

  // This code is here to propagate events from maximization to child
  // widgets, so that an image loaded from command-line will get the
  // appropriate widget size and occupy as much space as possible on screen.
  application.processEvents();

  // TODO: Move into mvd::Application.
  // Handle passing image filename from command-line
  if(argc>1)
    {
    mainWindow.OpenImage( QString(argv[1]) );
    }
  
  return application.exec();
}

//
// Main functions implementations.
//
