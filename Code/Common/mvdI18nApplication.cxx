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
#include "mvdI18nApplication.h"


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
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"

namespace mvd
{
/*
  TRANSLATOR mvd::I18nApplication

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* I18nApplication::CACHE_DIR_NAME = "mvd2";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
bool
I18nApplication
::MakeDirTree( const QString& path, const QString& tree )
{
  QDir pathDir( path );

  /*
  qDebug() << path;
  qDebug() << pathDir.currentPath();
  qDebug() << pathDir.path();
  */

  if( !pathDir.exists() )
    throw SystemError( ToStdString( QString( "('%1')" ).arg( path ) ) );

  QDir treeDir( pathDir.filePath( tree ) );
  if( treeDir.exists() )
    return false;

  if( !pathDir.mkpath( tree ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( pathDir.filePath( tree ) )
      )
    );

  return true;
}

/*******************************************************************************/
I18nApplication
::I18nApplication( int& argc, char** argv ) :
  QApplication( argc, argv ),
  m_CacheDir(),
  m_IsRunningFromBuildDir( false )
{
  InitializeLocale();
}

/*******************************************************************************/
I18nApplication
::~I18nApplication()
{
}

/*******************************************************************************/
void
I18nApplication
::InitializeLocale()
{
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );

  //
  // 1. default UI language is english (no translation).
  QLocale sys_lc( QLocale::system() );
  if( sys_lc.language() == QLocale::C ||
      ( sys_lc.language() == QLocale::English &&
        sys_lc.country() == QLocale::UnitedStates ) )
    {
    return;
    }

  //
  // 2. Choose i18n path between build dir and install dir.

  // Begin from the executable path
  QDir bin_dir( QDir::cleanPath(QCoreApplication::applicationDirPath()) );
  qDebug() << tr( "Executable dir : %1" ).arg( bin_dir.path() );
  
  // Go up in the directory hierarchy until we have a candidate install prefix
  bool prefixFound = false;
  QDir prefix( bin_dir );
  while ( prefix.cdUp() )
    {
    if ( QDir(prefix).cd(Monteverdi2_INSTALL_BIN_DIR) )
      {
      prefixFound = true;
      break;
      }
    }
  
  if (prefixFound)
    {
    qDebug() << tr( "Candidate install prefix found : %1" ).arg( prefix.path() );
    }
  else
    {
    QString message( tr( "Unable to locate translation files" ) );
    qDebug() << message;
    QMessageBox::critical( NULL, tr( "Critical error" ), message );
    return;
    }
  
  QDir i18n_dir;

  // At this point the candidate install prefix can also be the build dir root
  if ( prefix.exists( Monteverdi2_CONFIGURE_FILE )
       && prefix.exists("i18n") )
    {
    m_IsRunningFromBuildDir = true;
    
    // Report found build dir root
    qDebug() << tr( "Running from build directory '%1'." ).arg( prefix.path() );
    
    // Go into the i18n dir (it exists we just checked for it)
    i18n_dir = prefix;
    i18n_dir.cd("i18n");
    
    qDebug() << tr( "Using translation dir '%1'." ).arg( i18n_dir.path() );
    }
  else
    {
    m_IsRunningFromBuildDir = false;
    
    // Report found install prefix
    qDebug() << tr( "Running from install directory '%1'." ).arg( prefix.path() );

    // Go into the i18n dir configured at cmake-time
    i18n_dir = prefix;
    
    if (i18n_dir.cd(Monteverdi2_INSTALL_DATA_I18N_DIR))
      {
      qDebug() << tr( "Using translation dir '%1'." ).arg( i18n_dir.path() );
      }
    else
      {
      QString message( tr( "Failed to access translation-files directory '%1'" )
                       .arg(QDir::cleanPath(prefix.path()
                             + QDir::separator()
                             + Monteverdi2_INSTALL_DATA_I18N_DIR)) );
      qDebug() << message;
      QMessageBox::critical( NULL, tr( "Critical error" ), message );
      return;
      }
    }
  
  //
  // 3.1 Stack Qt translator.
  LoadAndInstallTranslator(
    "qt_" + sys_lc.name(),
    QLibraryInfo::location( QLibraryInfo::TranslationsPath  )
  );

  //
  // 3.2 Stack Monteverdi2 translator as prioritary over Qt translator.
  LoadAndInstallTranslator( sys_lc.name(), i18n_dir.path() );

  // TODO: Record locale translation filename(s) used in UI component (e.g.
  // AboutDialog, Settings dialog, Information dialog etc.)
}

/*******************************************************************************/
bool
I18nApplication
::LoadAndInstallTranslator(const QString& filename,
                           const QString& directory,
                           const QString& searchDelimiters,
                           const QString& suffix )
{
  QString filename_ext(
    filename +
    ( suffix.isNull()
      ? ".qm"
      : suffix )
  );

  // (a) Do need to new QTranslator() here!
  QTranslator* lc_translator = new QTranslator( this );

  if( !lc_translator->load( filename, directory, searchDelimiters, suffix ) )
    {
    QString message(
      tr( "Failed to load '%1' translation file from '%2'." )
      .arg( filename_ext )
      .arg( directory )
    );

    // TODO: Use log system to trace error while loading locale translation file.

    qWarning() << message;

    // TODO: morph into better HMI design.
#if !defined( _DEBUG )
    QMessageBox::warning( NULL, tr( "Warning!" ), message );
#endif

    return false;
    }

  // (a) ...because QTranslator needs to be alive during the whole
  // lifespan of the application.
  QCoreApplication::installTranslator( lc_translator );

  QString message(
    tr( "Successfully loaded '%1' translation file from '%2'." )
    .arg( filename_ext )
    .arg( directory )
  );

  // TODO: Log locale translation filename used.

  qDebug() << message;

  return true;
}

/*******************************************************************************/
bool
I18nApplication
::MakeCacheDir(QString cacheDirStr)
{
  QDir homeDir (cacheDirStr);

  if (!homeDir.exists())
    SystemError(ToStdString( QString( "('%1')" ).arg( homeDir.path() ) ));

  bool isNew = I18nApplication::MakeDirTree(
    homeDir.path(), I18nApplication::CACHE_DIR_NAME );

  QDir cacheDir( homeDir );

  if( !cacheDir.cd( I18nApplication::CACHE_DIR_NAME ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( homeDir.filePath( I18nApplication::CACHE_DIR_NAME ) )
      )
    );

  m_CacheDir = cacheDir;
  qDebug() << tr("Cache directory created at %1").arg(m_CacheDir.path());

  return isNew;
}

/*******************************************************************************/
bool
I18nApplication
::CheckCacheDirIsCorrect()
{
  QDir dir = m_CacheDir;

  if (!TestDirExistenceAndWriteAcess(dir))
    {
    return false;
    }
  else
    {
    // Check if this directory has the good name
    if (m_CacheDir.dirName().compare(QString(I18nApplication::CACHE_DIR_NAME)))
      {
      return false;
      }
    }

  return true;
}
/*******************************************************************************/
bool
I18nApplication
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

} // end namespace 'mvd'
