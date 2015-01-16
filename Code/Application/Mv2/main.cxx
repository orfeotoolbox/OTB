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
#if !defined( _DEBUG ) && 0
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

  /*
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
	"Monteverdi2",
	"Monteverdi2 - Critical error!"
      ),
      QCoreApplication::translate(
	"Monteverdi2",
	"Error while creating cache repository.\n\n"
	"%1\n\n"
	"Application will exit!"
      )
      .arg( exc.what() )
    );

    return ERROR_CODE_CACHE_DIR;
    }
  */

  /*
  //
  // 4. Initialize database.
  try
    {
    mvd::CountType nb = application.OpenDatabase();

    if( nb>0 )
      {
      QMessageBox::StandardButton button =
        QMessageBox::warning(
          &mainWindow,
          QCoreApplication::translate(
            "Monteverdi2",
            "Monteverdi2 - Warning!"
          ),
          QCoreApplication::translate(
            "Monteverdi2",
            "There are %1 outdated dataset(s) in cache-directory.\n\n"
            "Please remove cache-directory '%2' and restart Monteverdi2\n\n"
            "Do you want to delete cache-directory '%2' before quitting Monteverdi2?"
          ).arg( nb ).arg( application.GetCacheDir().path() ),
          QMessageBox::Yes | QMessageBox::No,
          QMessageBox::Yes
        );

      if( button==QMessageBox::Yes )
        {
        if( application.GetCacheDir()==QDir::home() )
          {
          // throw std::runtime_error(
          //   mvd::ToStdString(
          //     QCoreApplication::translate(
          //       "Monteverdi2",
          //       "Tryed to remove home dir."
          //     )
          //   )
          // );

          QMessageBox::critical(
            &mainWindow,
            QCoreApplication::translate(
              "Monteverdi2",
              "Monteverdi2 - Critical error!"
            ),
            QCoreApplication::translate(
              "Monteverdi2",
              "Your Monteverdi2 cache-directory is set to your home directory '%1'. Deletion of cache-directory is aborted to avoid unrecoverable loss of all your account data.\n\nIt is generally a bad idea to set Monteverdi2 cache-directory to your home directory. Please choose another sub-directory.\n\nApplication will now exit."
            )
            .arg( application.GetCacheDir().path() ),
            QMessageBox::Ok
          );
          }
        else
          {
          itksys::SystemTools::RemoveADirectory(
            QFile::encodeName( application.GetCacheDir().path() ).constData()
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
        "Monteverdi2",
        "Monteverdi2 - Critical error!"
      ),
      QCoreApplication::translate(
        "Monteverdi2",
        "Failed to open Monteverdi2 database.\n\nApplication will now exit!\n\n%2\n\nPlease, remove your Monteverdi2 cache-directory."
      )
      .arg( exc.what() ),
      QMessageBox::Ok
    );

    return ERROR_CODE_DATABASE;
    }
  */

  //
  // 5. Show window.
#if defined( _DEBUG )
  // Usefull when developping/debugging to avoid overlapping other windows.
  mainWindow.show();

#else
  splash.finish( &mainWindow );

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

  /*
  application.CloseDatabase();
  */

  return result;
}


/*****************************************************************************/
/* FUNCTIONS IMPLEMENTATION                                                  */
/*****************************************************************************/
