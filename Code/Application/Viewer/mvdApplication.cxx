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
#include "mvdApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractModel.h"
#include "Core/mvdDatasetModel.h"
#include "Core/mvdVectorImageModel.h"

//
// Class implementation.
namespace mvd
{
/*
  TRANSLATOR mvd::Application

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* Application::DATASET_EXT = ".ds";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
Application
::Application( QApplication* qtApp ) :
  I18nApplication( qtApp )
{
  QObject::connect(
    this, SIGNAL( aboutToQuit() ),
    this, SLOT( OnAboutToQuit() )
    );
}

/*******************************************************************************/
Application
::~Application()
{
}

/*******************************************************************************/
void
Application
::virtual_InitializeCore()
{
  I18nApplication::virtual_InitializeCore();

  setObjectName( "ViewerApplication" );

  InitializeCore(
    PROJECT_NAME, Monteverdi2_VERSION_STRING,
    "OrfeoToolBox", "orfeo-toolbox.org"
  );
}

/*******************************************************************************/
bool
Application
::HasSettingsFile()
{
  // The settings file should contain the cacheDir key to be valid
  QSettings settings;
  settings.sync();
  return ( (settings.status()==QSettings::NoError) && settings.contains("cacheDir"));
}

/*******************************************************************************/
void
Application
::ReadCacheDirFromSettings()
{
  QSettings settings;
  QString cacheDirSetting = settings.value("cacheDir").toString();
  GetCacheDir().setPath(cacheDirSetting);
}

/*******************************************************************************/
void Application::WriteCacheDirIntoSettings()
{
  QSettings settings;
  settings.setValue("cacheDir", GetCacheDir().path());
}

/*******************************************************************************/
bool
Application
::CheckCacheDirIsCorrect()
{
  QDir dir = GetCacheDir();

  if (!TestDirExistenceAndWriteAcess(dir))
    {
    return false;
    }
  else
    {
    // Check if this directory has the good name
    if (GetCacheDir().dirName().compare(QString(I18nApplication::DEFAULT_CACHE_DIR_NAME)))
      {
      return false;
      }
    }

  return true;
}

/*******************************************************************************/
bool
Application
::TestDirExistenceAndWriteAcess( QDir dir)
{
  // Check if this directory exists
  if (!dir.exists())
    {
    return false;
    }

  // Check if we can write in this directory
  if (dir.mkdir("testWriteAccess"))
    { // ok this repository is correct
    dir.rmdir("testWriteAccess");
    }
  else
    { // ko this repository is not correct
    return false;
    }

  return true;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
Application
::OnAboutToQuit()
{
  qDebug() << "mvd::Application::OnAboutToQuit().";
}

} // end namespace 'mvd'
