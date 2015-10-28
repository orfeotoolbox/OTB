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
  ERROR_CODE_I18N = -1,
  ERROR_CODE_CACHE_DIR = -2,
  ERROR_CODE_DATABASE = -3,
  ERROR_CODE_GL_VERSION = -4,
  ERROR_CODE_USAGE = -5,
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
  // 0bis. Parse pre-initialization command-line arguments.
  QStringList args( qtApp.arguments() );
  {
  for( QStringList::iterator it( args.begin() );
       it!=args.end(); )
    if( it->compare( "-h" )==0 ||
	it->compare( "--help" )==0 )
      {
      std::cout
	<< mvd::ToStdString(
	  QCoreApplication::translate(
	    PROJECT_NAME,
	    "Usage: %1 [-h|--help] [-a|--applications] [<filename>...]\n"
	    "  -h, --help         display this help message.\n"
	    "  -a, --applications load OTB-applications from OTB_APPLICATIONS_PATH."
	  )
	  .arg( basename( argv[ 0 ] ) )
	)
	<< std::endl;

      return ERROR_CODE_USAGE;
      }
    else
      {
      ++ it;
      }
  }

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
	  PROJECT_NAME,
	  "Question!"
	),
	QCoreApplication::translate(
	  PROJECT_NAME,
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
  args.pop_front();
  {
  bool otbApplications = false;

  for( QStringList::iterator it( args.begin() );
       it!=args.end(); )
    if( it->compare( "-a" )==0 ||
	it->compare( "--applications" )==0 )
      {
      if( !otbApplications )
	{
#if USE_OTB_APPS
	mainWindow.SetupOTBApplications();
#else // USE_OTB_APPS
	qWarning() << "OTB-applications support is not included in this build.";
#endif // USE_OTB_APPS

	it = args.erase( it );
	}
      }
    else
      {
      ++ it;
      }
  }

  mainWindow.ImportImages( args );


  //
  // 6. Let's go: run the application and return exit code.
  int result = QCoreApplication::instance()->exec();

  /*
  application->CloseDatabase();
  */

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
