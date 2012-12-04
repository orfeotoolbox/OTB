/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved .
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdApplication.h"

//
// System includes (sorted by alphabetic order)
#include <iostream>

//
// Qt includes (sorted by alphabetic order)
#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"

//
// Class implementation.
namespace mvd
{
/*
  TRANSLATOR mvd::Application

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
Application
::Application( int& argc, char** argv ) :
  QApplication( argc, argv ),
  m_IsRunningFromBuildDir( false )
{
  InitializeCore();

  InitializeLocale();

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
::InitializeCore()
{
  //
  //
#if 1
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "utf8" ) );
#endif

  //
  // Setup application tags.
  QCoreApplication::setApplicationName(
    PROJECT_NAME
  );
  QCoreApplication::setApplicationVersion(
    Monteverdi2_VERSION_STRING
  );

  //
  // Setup organization tags.
  QCoreApplication::setOrganizationName(
    tr( "Centre National d'Etudes Spatiales (CNES)" )
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );
}

/*******************************************************************************/
void
Application
::InitializeDataDir()
{
}

/*******************************************************************************/
void
Application
::InitializeLocale()
{
  //
  // 1. default UI language is en-US (no translation).
  QLocale sys_lc( QLocale::system() );
  QLocale ui_lc(
#if 1
    QLocale::English, QLocale::UnitedStates
#else
    QLocale::C, QLocale::AnyCountry
#endif
  );

  if( sys_lc.language()==ui_lc.language()
      && sys_lc.country()==ui_lc.country()
#if QT_VERSION>=0x040800
      && sys_lc.script()==ui_lc.script()
#endif
  )
    {
    return;
    }

  //
  // 2. Choose i18n path between build dir and install dir.
  QDir i18n_dir;
  QDir bin_dir( QDir::cleanPath( QCoreApplication::applicationDirPath() ) );
  QDir build_i18n_dir( bin_dir );

  // If build dir is identified...
  if( build_i18n_dir.cd( "../i18n" ) &&
      build_i18n_dir.exists( "../" Monteverdi2_CONFIGURE_FILE ) )
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
Application
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

    qDebug() << message;

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
void
Application
::OnAboutToQuit()
{
  qDebug() << tr( ">DEBUG> mvd::Application::OnAboutToQuit()." );
}

} // end namespace 'mvd'
