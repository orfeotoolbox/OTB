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


#include "otbWrapperQtWidgetMainWindow.h"
#include "mvdMaplaMainWindow.h"
#include "ui_mvdMaplaMainWindow.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifdef OTB_USE_QT
# include "mvdApplicationLauncher.h"
# include "mvdApplicationsToolBoxController.h"
# include "mvdOTBApplicationsModel.h"
# if 1
#   include "mvdQtWidgetView.h"
# else
#   include "otbWrappperQtWidgetView.h"
# endif
#endif
//
#include "mvdApplicationsToolBox.h"
//
#include "mvdMaplaApplication.h"
// #include "mvdPreferencesDialog.h"

namespace mvd
{

/*
  TRANSLATOR mvd::MaplaMainWindow

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
MaplaMainWindow
::MaplaMainWindow( QWidget * p, Qt::WindowFlags flags ) :
  I18nMainWindow( p, flags ),
  m_UI( new mvd::Ui::MaplaMainWindow() )
#ifdef OTB_USE_QT
  ,
  m_ApplicationsToolBoxController( NULL )
#endif
{
  m_UI->setupUi( this );
}

/*****************************************************************************/
MaplaMainWindow
::~MaplaMainWindow()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
MaplaMainWindow
::virtual_SetupUI()
{
  setObjectName( "Mapla" );
  setWindowTitle( PROJECT_NAME " Application Launcher" );

#ifdef OTB_USE_QT

  assert( m_ApplicationsToolBoxController==NULL );

  m_ApplicationsToolBoxController =
    new ApplicationsToolBoxController(
      new ApplicationsToolBox( this ),
      this
    );

  assert( MaplaApplication::Instance() );

  setCentralWidget( m_ApplicationsToolBoxController->GetWidget() );

#else // OTB_USE_QT

  setCentralWidget(
    new QLabel(
      tr( "Enable OTB_USE_QT preprocessor definition at compile time!" ),
      this
    )
  );

#endif // OTB_USE_QT

  if( !RestoreLayout( Monteverdi_UI_VERSION ) )
    {
    qWarning() << "Failed to restore window layout!";
    }
}

/*****************************************************************************/
void
MaplaMainWindow
::virtual_ConnectUI()
{
  //
  // OTB application support.
#ifdef OTB_USE_QT

  QObject::connect(
    m_ApplicationsToolBoxController->GetWidget(),
    SIGNAL( ApplicationToLaunchSelected( const QString &, const QString & ) ),
    // to:
    this,
    SLOT( OnApplicationToLaunchSelected(const QString &, const QString & ) )
  );

#endif
}

/*****************************************************************************/
void
MaplaMainWindow
::closeEvent( QCloseEvent* e )
{
  assert( MaplaApplication::Instance() );
  assert(
    MaplaApplication::Instance()->GetModel()==
    MaplaApplication::Instance()->GetModel< OTBApplicationsModel >()
  );

  SaveLayout( Monteverdi_UI_VERSION );

  I18nMainWindow::closeEvent( e );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
MaplaMainWindow
::on_action_Preferences_triggered()
{
#if 0
  PreferencesDialog prefDialog( this );

  prefDialog.exec();
#endif
}

/*****************************************************************************/
void
MaplaMainWindow
::OnAboutToChangeModel( const AbstractModel * )
{
  // qDebug() << this << "::OnAboutToChangeModel(" << model << ")";

  m_ApplicationsToolBoxController->SetModel( NULL );
}

/*****************************************************************************/
void
MaplaMainWindow
::OnModelChanged( AbstractModel* model )
{
  // qDebug() << this << "::OnModelChanged(" << model << ")";

  m_ApplicationsToolBoxController->SetModel( model );
}

/*****************************************************************************/
void
MaplaMainWindow
::OnApplicationToLaunchSelected( const QString & appName,
				 const QString & )
{
#ifdef OTB_USE_QT

  assert( MaplaApplication::ConstInstance()!=NULL );
  assert( MaplaApplication::ConstInstance()->GetModel()!=NULL );
  assert(
    MaplaApplication::ConstInstance()->GetModel()==
    MaplaApplication::ConstInstance()->GetModel< OTBApplicationsModel >()
  );
  assert(
    MaplaApplication::ConstInstance()
    ->GetModel< OTBApplicationsModel >()
    ->GetLauncher()!=NULL
  );

  otb::Wrapper::QtMainWindow* appWindow =
    MaplaApplication::ConstInstance()
    ->GetModel< OTBApplicationsModel >()
    ->GetLauncher()
    ->NewOtbApplicationWindow( appName, true, this );

  assert( appWindow!=NULL );

  appWindow->show();

#endif // OTB_USE_QT
}

/*****************************************************************************/
void
MaplaMainWindow
::OnExecutionDone( int status )
{
  if( status<0 )
    return;
}

} // end namespace 'mvd'
