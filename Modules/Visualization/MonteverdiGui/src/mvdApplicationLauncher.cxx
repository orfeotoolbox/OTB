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
::ApplicationLauncher( QObject* p ) :
  QObject( p )
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
			   QWidget * p,
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
	tr( "Failed to instantiate OTB-application '%1'." )
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
    new Wrapper::QtWidgetView( otbApp, p, flags );

  gui->CreateGui();

  return gui;
}
/*******************************************************************************/
QWidget * 
ApplicationLauncher
::NewOtbApplicationWindow( const QString & appName,
			   bool isStandalone,
			   QWidget * p,
			   Qt::WindowFlags flags  ) const
{
#if 0
  Wrapper::QtWidgetView * appWidget =
    ApplicationLauncher::NewOtbApplicationWidget( appName, isStandalone );

  assert( appWidget!=NULL );
  assert( appWidget->GetModel()->GetApplication() );

  QMainWindow * mainWindow = new QMainWindow( p, flags );

  mainWindow->setWindowTitle(
    QString( "%1 (OTB-" OTB_VERSION_STRING ")" )
    .arg( appWidget->GetModel()->GetApplication()->GetDocName() )
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
      p,
      flags | Qt::Window
    );

  assert( appWidget!=NULL );
  assert( appWidget->GetModel()->GetApplication() );

  appWidget->setWindowTitle(
    QString( "%1 (OTB-" OTB_VERSION_STRING ")" )
    .arg( appWidget->GetModel()->GetApplication()->GetDocName() )
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
