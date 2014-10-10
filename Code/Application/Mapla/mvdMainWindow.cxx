/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdMainWindow.h"
#include "ui_mvdMainWindow.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifdef OTB_WRAP_QT
# include "ApplicationsWrapper/mvdApplicationLauncher.h"
# include "ApplicationsWrapper/mvdApplicationsToolBoxController.h"
# include "ApplicationsWrapper/mvdOTBApplicationsModel.h"
# if 1
#   include "ApplicationsWrapper/mvdWrapperQtWidgetView.h"
# else
#   include "otbWrappperQtWidgetView.h"
# endif
#endif
//
#include "Gui/mvdApplicationsToolBox.h"
//
#include "mvdApplication.h"
// #include "mvdPreferencesDialog.h"

namespace mvd
{

/*
  TRANSLATOR mvd::MainWindow

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
MainWindow
::MainWindow( QWidget* parent, Qt::WindowFlags flags ) :
  I18nMainWindow( parent, flags ),
  m_UI( new mvd::Ui::MainWindow() )
#ifdef OTB_WRAP_QT
  ,
  m_ApplicationsToolBoxController( NULL )
#endif
{
  m_UI->setupUi( this );
}

/*****************************************************************************/
MainWindow
::~MainWindow()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
MainWindow
::virtual_SetupUI()
{
  setObjectName( "mvd::MainWindow" );
  setWindowTitle( PROJECT_NAME );

#ifdef OTB_WRAP_QT

  assert( m_ApplicationsToolBoxController==NULL );

  m_ApplicationsToolBoxController =
    new ApplicationsToolBoxController(
      new ApplicationsToolBox( this ),
      this
    );

  assert( Application::Instance() );

  setCentralWidget( m_ApplicationsToolBoxController->GetWidget() );

#else // OTB_WRAP_QT

  setCentralWidget(
    new QLabel(
      tr( "Enable OTB_WRAP_QT preprocessor definition at compile time!" ),
      this
    )
  );

#endif // OTB_WRAP_QT
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{
  //
  // OTB application support.
#ifdef OTB_WRAP_QT

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
MainWindow
::closeEvent( QCloseEvent* event )
{
  assert( Application::Instance() );
  assert(
    Application::Instance()->GetModel()==
    Application::Instance()->GetModel< OTBApplicationsModel >()
  );

  I18nMainWindow::closeEvent( event );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
MainWindow
::on_action_Preferences_activated()
{
#if 0
  PreferencesDialog prefDialog( this );

  prefDialog.exec();
#endif
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeModel( const AbstractModel* model )
{
  qDebug() << this << "::OnAboutToChangeModel(" << model << ")";

  m_ApplicationsToolBoxController->SetModel( NULL );
}

/*****************************************************************************/
void
MainWindow
::OnModelChanged( AbstractModel* model )
{
  qDebug() << this << "::OnModelChanged(" << model << ")";

  m_ApplicationsToolBoxController->SetModel( model );
}

/*****************************************************************************/
void
MainWindow
::OnApplicationToLaunchSelected( const QString& appName,
				 const QString& docName)
{
#ifdef OTB_WRAP_QT

  assert( Application::ConstInstance()!=NULL );
  assert( Application::ConstInstance()->GetModel()!=NULL );
  assert(
    Application::ConstInstance()->GetModel()==
    Application::ConstInstance()->GetModel< OTBApplicationsModel >()
  );
  assert(
    Application::ConstInstance()
    ->GetModel< OTBApplicationsModel >()
    ->GetLauncher()!=NULL
  );

  Wrapper::QtWidgetView* appWidget =
    Application::ConstInstance()
    ->GetModel< OTBApplicationsModel >()
    ->GetLauncher()
    ->NewOtbApplicationWidget( appName );

  assert( appWidget!=NULL );

  QMainWindow * mainWindow = new QMainWindow( /* this */ );

  mainWindow->setCentralWidget( appWidget );

  mainWindow->show();

  /*
  //
  // add the application in a tab
  // TODO : check if this application is already opened ???
  int tabIndex = m_CentralTabWidget->addTab(
    appWidget, QIcon( ":/icons/process" ), docName );

  // no checking needed here, if index is not available nothing is
  // done. Focus on the newly added tab
  m_CentralTabWidget->setCurrentIndex( tabIndex );

  //
  // connections. not using m_CentralTabWidget->currentWidget() leads
  // to a wrong connection!!!!
  QObject::connect(
    m_CentralTabWidget->currentWidget(),
    SIGNAL( OTBApplicationOutputImageChanged( const QString&,
					      const QString& ) ),
    // to:
    this,
    SLOT( OnOTBApplicationOutputImageChanged( const QString&,
					      const QString& ) )
    );

  QObject::connect(
    appWidget,
    SIGNAL( ExecutionDone( int ) ),
    // to:
    this,
    SLOT( OnExecutionDone( int ) )
  );

  //
  // on quit widget signal, close its tab
  QObject::connect(
    m_CentralTabWidget->currentWidget(),
    SIGNAL( QuitSignal() ),
    // to:
    this,
    SLOT( OnTabCloseRequested() )
    );
  */

#endif // OTB_WRAP_QT
}

/*****************************************************************************/
void
MainWindow
::OnExecutionDone( int status )
{
  if( status<0 )
    return;
}

} // end namespace 'mvd'
