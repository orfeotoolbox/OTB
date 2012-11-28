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
  QApplication( argc, argv )
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
  QCoreApplication::setApplicationName(
    PROJECT_NAME
  );
  QCoreApplication::setApplicationVersion(
    Monteverdi2_VERSION_STRING
  );
  QCoreApplication::setOrganizationName(
    "Centre National d'Etudes Spatiales (CNES)"
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );
}

/*******************************************************************************/
void
Application::InitializeLocale()
{
  qDebug() << "UI languages: " << QLocale::system().uiLanguages();
  qDebug() << "System: " << QLocale::system();
  qDebug() << "en_US: " << QLocale( QLocale::English, QLocale::UnitedStates );

  //
  // default UI language is en-US (no translation).
  QLocale sys_lc( QLocale::system() );
  QLocale en_US_lc( QLocale::English, QLocale::UnitedStates );
  if( sys_lc.language()==en_US_lc.language() &&
      sys_lc.country()==en_US_lc.country() &&
      sys_lc.script()==en_US_lc.script() )
    {
    return;
    }

  //
  // Desired locale name.
  QString lc_name( QLocale::system().name() );

  //
  // Choose i18n path between build dir and install dir.
  QDir i18n_dir;

  QDir build_dir( QDir::cleanPath( QCoreApplication::applicationDirPath() ) );
  qDebug() << "Build dir: " << build_dir;

  QDir i18n_build_dir( QDir::cleanPath( build_dir.path() + "/../i18n" ) );
  qDebug() << "I18N build dir: " << i18n_build_dir;

  QDir install_dir( QDir::cleanPath( Monteverdi2_INSTALL_DATA_DIR ) );
  qDebug() << "Install dir: " << install_dir;

  QDir i18n_install_dir( QDir::cleanPath( install_dir.path() + "/../i18n" ) );
  qDebug() << "I18N install dir: " << i18n_install_dir;

  // If build dir is identified...
  if( i18n_build_dir.exists() &&
      i18n_build_dir.exists( "../ConfigureMonteverdi2.h" ) )
    {
    // ...use build dir as prioritary load path.
    i18n_dir = i18n_build_dir;

    // TODO: Use log system to trace message.
    qDebug()
      << tr( "Running from build directory '%1'" ).arg( build_dir.path() );

    }
  else
    {
    // ...use install dir if application is installed in the system.
    i18n_dir = i18n_install_dir;

    // TODO: Use log system to trace message.
    qDebug()
      << tr( "Running from install directory '%1'" ).arg( install_dir.path() );
    }

  //
  // Load QTranslator.
  QString lc_filename( i18n_dir.filePath( lc_name ) );
  qDebug() << "Filename: " << lc_name;
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
