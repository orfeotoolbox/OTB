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
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"

//
// Class pre-declaration.

//
// Class implementation.
namespace mvd
{

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

/*
bool
Application::Initialize()
{
  InitializeLocale();

  return true;
}
*/

/*******************************************************************************/
void
Application::InitializeCore()
{
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
    "Centre National d'Etudes Spatiales (CNES)"
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );
}

/*******************************************************************************/
void
Application::InitializeDataDir()
{
}

/*******************************************************************************/
void
Application::InitializeLocale()
{
  //
  // 1. default UI language is en-US (no translation).
  QLocale sys_lc( QLocale::system() );
  QLocale en_US_lc( QLocale::English, QLocale::UnitedStates );
  if( sys_lc.language()==en_US_lc.language() &&
      sys_lc.country()==en_US_lc.country() &&
      sys_lc.script()==en_US_lc.script() )
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
      << tr( "Running from build directory '%1'" ).arg( bin_dir.path() );
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
	<< tr( "Running from install directory '%1'" ).arg( Monteverdi2_INSTALL_BIN_DIR );
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
      QMessageBox::critical( NULL, "Critical...", message );

      return;
      }
    }

  //
  // 3. Desired locale name.
  QString lc_name( QLocale::system().name() );

  //
  // 4. Load QTranslator.
  QString lc_filename( i18n_dir.filePath( lc_name ) );
  // (a) No need to new QTranslator() here nor delete in destructor...
  QTranslator lc_translator;
  if( !lc_translator.load( lc_name, i18n_dir.path() ) )
    {
    QString message(
      tr( "Failed to load '%1' translation file in '%2'." )
      .arg( lc_name )
      .arg( i18n_dir.path() )
    );

    // TODO: Use log system to trace error while loading locale translation file.

    qDebug() << message;

    // TODO: morph into better HMI design.
    QMessageBox::warning( NULL, "Warning", message );

    return;
    }

  // (a) ...because of Qt private implementation and shallow-copies mecanisms.
  installTranslator( &lc_translator );

  QString message(
    tr( "Successfully loaded '%1' translation file in '%2'." )
    .arg( lc_name )
    .arg( i18n_dir.path() )
  );

  // TODO: Log locale translation filename used.

  qDebug() << message;

  // TODO: Record locale translation filename used in UI component (e.g.
  // AboutDialog, Settings dialog, Information dialog etc.)
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
