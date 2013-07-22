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
#include <QPixmap>
#include <QSplashScreen>
//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"

/*****************************************************************************/
/* FUNCTIONS DECLARATION                                                     */


/*****************************************************************************/
/* MAIN                                                                      */

int
main( int argc, char* argv[] )
{
  QApplication qtApp( argc, argv );

  // 0. Splash-screen.
#if !defined( _DEBUG )
  QPixmap pixmap(QLatin1String( ":/images/application_splash" ));
  QSplashScreen splash(pixmap);
  splash.show();
  qtApp.processEvents();//This is used to accept a click on the screen so that user can cancel the screen
#endif

  // 1. Initialize application and sync settings.
  mvd::Application application( &qtApp );
  application.Initialize();

  // 2. Initialize main-window (UI).
  mvd::MainWindow mainWindow;
  mainWindow.Initialize();

  // 3. Initialize cache directory.
  try
    {
    mainWindow.SetupCacheDir();
    }
  catch( std::exception& exc )
    {
    QMessageBox::critical(
      &mainWindow,
      QCoreApplication::translate(
	PROJECT_NAME,
	PROJECT_NAME " - Critical error!"
      ),
      QCoreApplication::translate(
	PROJECT_NAME,
	"Error when creating repository cache-directory:\n"
	"%1\n"
	"Application will exit!"
      )
      .arg( exc.what() )
    );

    return -1;
    }

  // 4. Initialize database.
  mvd::CountType nb = application.OpenDatabase();
  if( nb > 0 )
    {
#if defined( _DEBUG )
    QMessageBox::StandardButton button =
#endif
      QMessageBox::warning(
  &mainWindow,
  QCoreApplication::translate(
    PROJECT_NAME,
    PROJECT_NAME " " Monteverdi2_VERSION_STRING " - Warning! "
  ),
  QCoreApplication::translate(
    PROJECT_NAME,
    "There are %1 outdated dataset(s) in cache-directory.\n\n"
    "Please remove cache-directory '%2' and restart "
    PROJECT_NAME "."
#if defined( _DEBUG )
    "\n\nDo you want to quit now ?" 
#endif
  ).arg( nb ).arg( application.GetCacheDir().path() ),
#if defined( _DEBUG )
  QMessageBox::Yes | QMessageBox::No,
  QMessageBox::Yes
#else
  QMessageBox::Ok
#endif
      );

#if defined( _DEBUG )
    if( button==QMessageBox::Yes )
      {
      return -2;
      }
#else
    return -2;
#endif
    }

  // 5. Show window.
#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();

#else
  splash.finish(&mainWindow);

  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();
  
#endif

  // 6. Let's go: run the application and return exit code.
  return QCoreApplication::instance()->exec();
}


/*****************************************************************************/
/* FUNCTIONS IMPLEMENTATION                                                  */
