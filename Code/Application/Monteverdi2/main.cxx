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
#include "mvdApplication.h"
#include "mvdMainWindow.h"

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
	"Critical error!"
      ),
      QCoreApplication::translate(
	PROJECT_NAME,
	"Error while creating cache repository.\n\n"
	"%1\n\n"
	"Application will exit!"
      )
      .arg( exc.what() )
    );

    return ERROR_CODE_CACHE_DIR;
    }

  //
  // 4. Initialize database.
  try
    {
    mvd::CountType nb = application->OpenDatabase();

    if( nb>0 )
      {
      QMessageBox::StandardButton button =
        QMessageBox::warning(
          &mainWindow,
          QCoreApplication::translate(
            PROJECT_NAME,
            "Warning!"
          ),
          QCoreApplication::translate(
            PROJECT_NAME,
            "There are %1 outdated dataset(s) in cache-directory.\n\n"
            "Please remove cache-directory '%2' and restart Monteverdi\n\n"
            "Do you want to delete cache-directory '%2' before quitting Monteverdi?"
          ).arg( nb ).arg( application->GetCacheDir().path() ),
          QMessageBox::Yes | QMessageBox::No,
          QMessageBox::Yes
        );

      if( button==QMessageBox::Yes )
        {
        if( application->GetCacheDir()==QDir::home() )
          {
          // throw std::runtime_error(
          //   mvd::ToStdString(
          //     QCoreApplication::translate(
          //       "Monteverdi",
          //       "Tryed to remove home dir."
          //     )
          //   )
          // );

          QMessageBox::critical(
            &mainWindow,
            QCoreApplication::translate(
              PROJECT_NAME,
              "Critical error!"
            ),
            QCoreApplication::translate(
              PROJECT_NAME,
              "Your Monteverdi cache-directory is set to your home directory '%1'. Deletion of cache-directory is aborted to avoid unrecoverable loss of all your account data.\n\nIt is generally a bad idea to set Monteverdi cache-directory to your home directory. Please choose another sub-directory.\n\nApplication will now exit."
            )
            .arg( application->GetCacheDir().path() ),
            QMessageBox::Ok
          );
          }
        else
          {
          itksys::SystemTools::RemoveADirectory(
            QFile::encodeName( application->GetCacheDir().path() ).constData()
          );
          }
        }

      return ERROR_CODE_DATABASE;
      }
    }
  catch( std::exception& exc )
    {
    QMessageBox::critical(
      &mainWindow,
      QCoreApplication::translate(
        PROJECT_NAME,
        "Critical error!"
      ),
      QCoreApplication::translate(
        PROJECT_NAME,
        "Failed to open Monteverdi database.\n\nApplication will now exit!\n\n%2\n\nPlease, remove your Monteverdi cache-directory."
      )
      .arg( exc.what() ),
      QMessageBox::Ok
    );

    return ERROR_CODE_DATABASE;
    }

  //
  // 5. Show window.
#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();

#else
  splash.finish(&mainWindow);

  // TODO: Correctly manage main-window state via application settings.
  mainWindow.showMaximized();

#endif

  //
  // 6. Check OpenGL capabilities
  if( !mainWindow.CheckGLCapabilities() )
    return ERROR_CODE_GL_VERSION;

  //
  // 7. Let's go: run the application and return exit code.
  int result = QCoreApplication::instance()->exec();

  application->CloseDatabase();

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
