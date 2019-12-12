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
#include "otbConfigurationManager.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdMaplaApplication.h"
#include "mvdMaplaMainWindow.h"

enum ERROR_CODE
{
  ERROR_CODE_I18N       = -1,
  ERROR_CODE_CACHE_DIR  = -2,
  ERROR_CODE_DATABASE   = -3,
  ERROR_CODE_GL_VERSION = -4,
};

/*****************************************************************************/
/* FUNCTIONS DECLARATION                                                     */


/*****************************************************************************/
/* MAIN                                                                      */

int main(int argc, char* argv[])
{
  QApplication qtApp(argc, argv);

  otb::ConfigurationManager::InitOpenMPThreads();

//
// 0. Splash-screen.
#if !defined(OTB_DEBUG)
  QPixmap       pixmap(QLatin1String(":/images/application_splash"));
  QSplashScreen splash(pixmap);
  splash.show();
  qtApp.processEvents(); // This is used to accept a click on the screen so that user can cancel the screen
#endif

  //
  // 1. Initialize application and sync settings.
  //
  // Coverity-14835
  // {
  mvd::MaplaApplication* application = NULL;

  try
  {
    application = new mvd::MaplaApplication(&qtApp);
    assert(application != NULL);

    application->Initialize();
  }
  catch (std::exception& exc)
  {
    QMessageBox::StandardButton button =
        QMessageBox::question(NULL, QCoreApplication::translate("Mapla", "Question!"),
                              QCoreApplication::translate("Mapla",
                                                          "The following exception has been caught while initializing the software:\n\n"
                                                          "%1\n\n"
                                                          "The application may not function as expected. Do you want to continue?")
                                  .arg(exc.what()),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (button == QMessageBox::No)
      return ERROR_CODE_I18N;
  }
  // }
  // Coverity-14835

  //
  // 2. Initialize main-window (UI).
  mvd::MaplaMainWindow mainWindow;
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
#if defined(OTB_DEBUG)
#else  // OTB_DEBUG
  splash.finish(&mainWindow);
#endif // OTB_DEBUG

  // Useful when developping/debugging to avoid overlapping other windows.
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
