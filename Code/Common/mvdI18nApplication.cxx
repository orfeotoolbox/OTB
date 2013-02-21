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

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// Class implementation.
namespace mvd
{
/*
  TRANSLATOR mvd::I18nApplication

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
I18nApplication
::I18nApplication( int& argc, char** argv ) :
  QApplication( argc, argv ),
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
  if( sys_lc.language()==QLocale::C ||
      ( sys_lc.language()==QLocale::English &&
	sys_lc.country()==QLocale::UnitedStates ) )
    {
    return;
    }

  //
  // 2. Choose i18n path between build dir and install dir.
  QDir i18n_dir;
  QDir bin_dir( QDir::cleanPath( QCoreApplication::applicationDirPath() ) );
  QDir build_i18n_dir( bin_dir );

  // If build dir is identified...
  if( build_i18n_dir.exists( "../i18n" )
         && build_i18n_dir.cd( "../i18n" )
         && build_i18n_dir.exists( "../" Monteverdi2_CONFIGURE_FILE )
       || build_i18n_dir.exists( "../../i18n" )
         && build_i18n_dir.cd( "../../i18n" )
         && build_i18n_dir.exists( "../" Monteverdi2_CONFIGURE_FILE ) )
    {
    m_IsRunningFromBuildDir = true;

    // ...use build dir as prioritary load path for translation.
    i18n_dir = build_i18n_dir;

    // TODO: Use log system to trace message.
    qDebug()
      << tr( "Running from build directory '%1'." ).arg( bin_dir.path() );
    }
  // Otherwise...
  else
    {
    m_IsRunningFromBuildDir = false;

    QDir install_i18n_dir( QDir::cleanPath( Monteverdi2_INSTALL_DATA_I18N_DIR ) );
    // ...if install data dir is identified
    if( install_i18n_dir.exists() )
      {
      // ...use install data dir as load path for translation.
      i18n_dir = install_i18n_dir;

      // TODO: Use log system to trace message.
      qDebug()
        << tr( "Running from install directory '%1'." ).arg( Monteverdi2_INSTALL_BIN_DIR );
      }
    // Otherwise
    else
      {
      QString message(
        tr( "Failed to access translation-files directory '%1'." )
        .arg( install_i18n_dir.path() )
      );

      // TODO: Use log system to trace error while loading locale translation file.

      qDebug() << message;

      // TODO: morph into better HMI design.
      QMessageBox::critical( NULL, tr( "Critical error!" ), message );

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
    QMessageBox::warning( NULL, tr( "Warning!" ), message );

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
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
