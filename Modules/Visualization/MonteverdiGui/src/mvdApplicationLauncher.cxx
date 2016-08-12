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
#include "mvdApplicationLauncher.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperApplication.h"
 
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdI18nCoreApplication.h"
//#include "mvdTextStream.h"
#include "mvdQtWidgetView.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ApplicationLauncher

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationLauncher
::ApplicationLauncher( QObject* parent ) :
  QObject( parent )
{
}

/*******************************************************************************/
ApplicationLauncher
::~ApplicationLauncher()
{
}

/*******************************************************************************/
Wrapper::QtWidgetView *
ApplicationLauncher
::NewOtbApplicationWidget( const QString & appName,
			   bool isStandalone,
			   QWidget * parent,
			   Qt::WindowFlags flags ) const
{
  // Create module
  otb::Wrapper::Application::Pointer otbApp(
    otb::Wrapper::ApplicationRegistry::CreateApplication(
      ToStdString( appName )
    )
  );

  if( otbApp.IsNull() )
    {
    throw std::runtime_error(
      ToStdString(
	tr( "Failed to instanciate OTB-application '%1'." )
	.arg( appName )
      )
    );
    }

  if( !isStandalone )
    {
    // Search for elev parameters
    typedef std::vector< std::string > ParametersKeys;
    const ParametersKeys parameters( otbApp->GetParametersKeys() );

    // Little flag structure with bool operator to optimize look
    // scanning parameter keys.
    struct Flags
    {
      Flags() :
        m_HasDem( false ),
        m_HasGeoid( false )
      {
      }

      inline operator bool () const
      {
        return m_HasDem && m_HasGeoid;
      }

      bool m_HasDem : 1;
      bool m_HasGeoid : 1;
    };

    Flags found;

    for( ParametersKeys::const_iterator it( parameters.begin() );
         it!=parameters.end() && !found;
         ++it )
      {
      std::size_t lastDot = it->find_last_of('.');

      assert( I18nCoreApplication::ConstInstance()!=NULL );
      const I18nCoreApplication* i18nApp = I18nCoreApplication::ConstInstance();

      if( lastDot != std::string::npos )
        {
        std::string lastKey(
          it->substr( lastDot + 1, it->size() - lastDot - 1 )
        );

        if( lastKey=="dem" )
          {
          found.m_HasDem = true;

          if( i18nApp->HasSettingsKey(
                I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE ) &&
              i18nApp->RetrieveSettingsKey(
                I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE ).toBool() )
            {
            otbApp->EnableParameter( *it );
            otbApp->SetParameterString(
              *it,
              ToStdString(
                i18nApp->RetrieveSettingsKey(
                  I18nCoreApplication::SETTINGS_KEY_SRTM_DIR
                )
                .toString()
              )
            );
            }
          }
        else if( lastKey=="geoid" )
          {
          found.m_HasGeoid = true;

          if( i18nApp->HasSettingsKey(
                I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE ) &&
              i18nApp->RetrieveSettingsKey(
                I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE )
              .toBool() )
            {
            otbApp->EnableParameter( *it );

            otbApp->SetParameterString(
              *it,
              ToStdString(
                i18nApp->RetrieveSettingsKey(
                  I18nCoreApplication::SETTINGS_KEY_GEOID_PATH
                )
                .toString()
              )
            );
            }
          }
        }
      }
    }

  // Create GUI based on module
  Wrapper::QtWidgetView * gui =
    new Wrapper::QtWidgetView( otbApp, parent, flags );

  gui->CreateGui();

  return gui;
}
/*******************************************************************************/
QWidget * 
ApplicationLauncher
::NewOtbApplicationWindow( const QString & appName,
			   bool isStandalone,
			   QWidget * parent,
			   Qt::WindowFlags flags  ) const
{
#if 0
  Wrapper::QtWidgetView * appWidget =
    ApplicationLauncher::NewOtbApplicationWidget( appName, isStandalone );

  assert( appWidget!=NULL );
  assert( appWidget->GetApplication() );

  QMainWindow * mainWindow = new QMainWindow( parent, flags );

  mainWindow->setWindowTitle(
    QString( "%1 (OTB-" OTB_VERSION_STRING ")" )
    .arg( appWidget->GetApplication()->GetDocName() )
  );

  mainWindow->setWindowIcon( QIcon( ":/otb_small.png" ) );
 
  mainWindow->setCentralWidget( appWidget );

  // Connect OTB-app widget quit signal, to the mainWindow close slot.
  QObject::connect(
    appWidget,
    SIGNAL( QuitSignal() ),
    // to:
    mainWindow,
    SLOT( close() )
  );

  return mainWindow;

#else
  Wrapper::QtWidgetView * appWidget =
    ApplicationLauncher::NewOtbApplicationWidget(
      appName,
      isStandalone,
      parent,
      flags | Qt::Window
    );

  assert( appWidget!=NULL );
  assert( appWidget->GetApplication() );

  appWidget->setWindowTitle(
    QString( "%1 (OTB-" OTB_VERSION_STRING ")" )
    .arg( appWidget->GetApplication()->GetDocName() )
  );

  appWidget->setWindowIcon( QIcon( ":/icons/process" ) );
 
  QObject::connect(
    appWidget,
    SIGNAL( QuitSignal() ),
    // to:
    appWidget,
    SLOT( close() )
  );

  return appWidget;
#endif
}

/*******************************************************************************/
/* SLOTS                                                                       */

} // end namespace 'mvd'
