/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QPixmap>
#include <QSplashScreen>

//
// System includes (sorted by alphabetic order)
#include <exception>

//
// ITK includes (sorted by alphabetic order)
#include "itksys/SystemTools.hxx"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "maplaApplication.h"
#include "maplaMainWindow.h"

enum ERROR_CODE
{
  ERROR_CODE_I18N = -1,
  ERROR_CODE_CACHE_DIR = -2,
  ERROR_CODE_DATABASE = -3,
  ERROR_CODE_GL_VERSION = -4,
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
#if !defined( _DEBUG )
  QPixmap pixmap(QLatin1String( ":/images/application_splash" ));
  QSplashScreen splash(pixmap);
  splash.show();
  qtApp.processEvents();//This is used to accept a click on the screen so that user can cancel the screen
#endif

  //
  // 1. Initialize application and sync settings.
  //
  // Coverity-14835
  // {
  mvd::Application * application = NULL;

  try
    {
    application = new mvd::Application( &qtApp );
    assert( application!=NULL );

    application->Initialize();
    }
  catch( std::exception & exc )
    {
    QMessageBox::StandardButton button =
      QMessageBox::question(
	NULL,
	QCoreApplication::translate(
	  "Mapla",
	  "Question!"
	),
	QCoreApplication::translate(
	  "Mapla",
	  "The following exception has been caught while initializing the software:\n\n"
	  "%1\n\n"
	  "The application may not function as expected. Do you want to continue?"
	)
	.arg( exc.what() ),
	QMessageBox::Yes | QMessageBox::No,
	QMessageBox::Yes
      );

    if( button==QMessageBox::No )
      return ERROR_CODE_I18N;
    }
  // }
  // Coverity-14835

  //
  // 2. Initialize main-window (UI).
  mvd::MainWindow mainWindow;
  mainWindow.Initialize();

  application->Foo();

  //
  // 3. Initialize cache directory.
  //
  // N/A

  //
  // 4. Initialize database.
  //
  // N.A.

  //
  // 5. Show window.
#if defined( _DEBUG )
#else // _DEBUG
  splash.finish(&mainWindow);
#endif // _DEBUG

  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();

  //
  // 6. Check OpenGL capabilities
  /*
  if( !mainWindow.CheckGLCapabilities() )
    return ERROR_CODE_GL_VERSION;
  */

  //
  // 7. Let's go: run the application and return exit code.
  int result = QCoreApplication::instance()->exec();

  // application->CloseDatabase();

  // Coverity-14835
  // {
  delete application;
  application = NULL;
  // }
  // Coverity-14835

  return result;
}


/*****************************************************************************/
/* FUNCTIONS IMPLEMENTATION                                                  */
/*****************************************************************************/
