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

#define USE_SPLASH_SCREEN 0

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
// #include "itksys/SystemTools.hxx"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"

enum ERROR_CODE
{
  ERROR_CODE_CACHE_DIR = -1,
  ERROR_CODE_DATABASE = -2,
  ERROR_CODE_GL_VERSION = -3,
};

/*****************************************************************************/
/* FUNCTIONS DECLARATION                                                     */


/*****************************************************************************/
/* MAIN                                                                      */

int
main( int argc, char* argv[] )
{
  QApplication qtApp( argc, argv );

  //
  // 0. Splash-screen.
#if !defined( _DEBUG ) && USE_SPLASH_SCREEN
  QPixmap pixmap(QLatin1String( ":/images/application_splash" ));
  QSplashScreen splash(pixmap);
  splash.show();
  qtApp.processEvents();//This is used to accept a click on the screen so that user can cancel the screen
#endif

  //
  // 1. Initialize application and sync settings.
  mvd::Application application( &qtApp );
  application.Initialize();

  //
  // 2. Initialize main-window (UI).
  mvd::MainWindow mainWindow;
  mainWindow.Initialize();

  //
  // 3. Show window.
#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();

#else // _DEBUG

#if USE_SPLASH_SCREEN
  splash.finish( &mainWindow );
#endif // USE_SPLASH_SCREEN

  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();

#endif // _DEBUG

  //
  // 4. Check OpenGL capabilities
  if( !mainWindow.CheckGLCapabilities() )
    return ERROR_CODE_GL_VERSION;

  //
  // 5. Parse command-line filenames.
  QStringList filenames( qtApp.arguments() );

  filenames.pop_front();

  mainWindow.ImportImages( filenames );
 

  //
  // 6. Let's go: run the application and return exit code.
  int result = QCoreApplication::instance()->exec();

  /*
  application.CloseDatabase();
  */

  return result;
}


/*****************************************************************************/
/* FUNCTIONS IMPLEMENTATION                                                  */
/*****************************************************************************/
