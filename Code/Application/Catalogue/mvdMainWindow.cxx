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
# include "ApplicationsWrapper/mvdApplicationsToolBoxController.h"
# include "ApplicationsWrapper/mvdOTBApplicationsModel.h"
# include "ApplicationsWrapper/mvdWrapperQtWidgetView.h"
#endif
//
#include "Core/mvdDatabaseModel.h"
#include "Core/mvdDatasetModel.h"
#include "Core/mvdI18nApplication.h"
//
#include "Gui/mvdApplicationsToolBox.h"
#include "Gui/mvdColorSetupController.h"
#include "Gui/mvdColorSetupWidget.h"
#include "Gui/mvdColorDynamicsController.h"
#include "Gui/mvdColorDynamicsWidget.h"
#include "Gui/mvdDatabaseBrowserController.h"
#include "Gui/mvdDatabaseBrowserWidget.h"
#include "Gui/mvdDatasetPropertiesController.h"
#include "Gui/mvdDatasetPropertiesWidget.h"
#include "Gui/mvdGLImageWidget.h"
#include "Gui/mvdImageModelRenderer.h"
#include "Gui/mvdQuicklookViewManipulator.h"
//
#include "mvdCatalogueApplication.h"

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
  m_UI( new mvd::Ui::MainWindow() ),
  m_ColorDynamicsDock( NULL ),
  m_ColorSetupDock( NULL ),
  m_DatabaseBrowserDock( NULL ),
  m_DatasetPropertiesDock(NULL),
#ifdef OTB_WRAP_QT
  m_OtbApplicationsBrowserDock(NULL),
#endif
  m_QuicklookViewDock( NULL )
{
  m_UI->setupUi( this );
}

/*****************************************************************************/
MainWindow
::~MainWindow()
{
}

/*****************************************************************************/
void
MainWindow
::virtual_SetupUI()
{
  setObjectName( "mvd::MainWindow" );
  setWindowTitle( PROJECT_NAME );

  InitializeDockWidgets();

  InitializeCentralWidget();

#if 0
  m_CentralTabWidget->addTab(
    CreateQuicklookWidget(),
    m_QuicklookViewDock->windowTitle()
  );

  //
  // access to the quicklook tabBar to remove the close button
  QTabBar* tabBar = m_CentralTabWidget->findChild< QTabBar* >();

  tabBar->setTabButton(0, QTabBar::RightSide, 0);
  tabBar->setTabButton(0, QTabBar::LeftSide, 0);
#endif
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{

#ifdef OTB_WRAP_QT
  //
  // Done here cause needed to be done once
  SetControllerModel(
    m_OtbApplicationsBrowserDock, 
    I18nApplication::Instance< CatalogueApplication >()->GetOTBApplicationsModel()
  );

  //
  // need to get the ApplicationToolBox widget to setup connections.
  // a double click on the tree widget should trigger a signal connected
  // to this MainWindow slot. this slot will be in charge of getting the
  // widget of the application selected, and show it in the
  // MainWindow centralView.  
  
  // # Step 1 : get the ApplicationToolBoxWidget
  ApplicationsToolBox* appWidget = 
    qobject_cast< ApplicationsToolBox * >( 
      m_OtbApplicationsBrowserDock->findChild< ApplicationsToolBox* >( ) 
      );
  
  // # Step 2 : setup connections
  assert(appWidget != NULL);
  QObject::connect(appWidget,
                   SIGNAL( ApplicationToLaunchSelected(const QString &) ),
                   this,
                   SLOT( OnApplicationToLaunchSelected(const QString &) )
    );
  
  // # Step 3 : if OTB applications has any output, it may be 
  
#endif

  //
  // close tabs handling
  QObject::connect(m_CentralTabWidget,
                   SIGNAL(tabCloseRequested(int)),
                   this,
                   SLOT(OntabCloseRequested(int))
    );
}

/*****************************************************************************/
void
MainWindow
::InitializeDockWidgets()
{

  // Quicklook-view dock-widget (is left or right but is the top-most
  // widget of the dock-area. So, it is inserted as first
  // dock-widget.
  assert( m_QuicklookViewDock==NULL );
  m_QuicklookViewDock = AddWidgetToDock(
    CreateQuicklookWidget(),
    "QUICKLOOK_VIEW",
    tr( "Quicklook view" ),
#if 0
    Qt::LeftDockWidgetArea
#else
    Qt::RightDockWidgetArea
#endif
  );

  //
  // Left pane.

  // Database-browser.
  assert( m_DatabaseBrowserDock==NULL );
  m_DatabaseBrowserDock =
    AddDockWidget
    < DatabaseBrowserWidget, DatabaseBrowserController, QDockWidget >
    ( "DATABASE_BROWSER",
      tr( "Database browser" ),
      Qt::LeftDockWidgetArea
    );

  // Dataset-properties.
  assert( m_DatasetPropertiesDock==NULL );
  m_DatasetPropertiesDock =
    AddDockWidget
    < DatasetPropertiesWidget, DatasetPropertiesController, QDockWidget >
    ( "DATASET_PROPERTIES",
      tr( "Dataset properties" ),
      Qt::LeftDockWidgetArea
    );

  // Video color-setup.
  assert( m_ColorSetupDock==NULL );
  m_ColorSetupDock =
    AddDockWidget
    < ColorSetupWidget, ColorSetupController, QDockWidget >
    ( "COLOR_SETUP",
      tr( "Color setup" ),
      Qt::LeftDockWidgetArea
    );

  // Video color-dynamics.
  assert( m_ColorDynamicsDock==NULL );
  m_ColorDynamicsDock =
    AddDockWidget
    < ColorDynamicsWidget, ColorDynamicsController, QDockWidget >
    ( "COLOR_DYNAMICS",
      tr( "Color dynamics" ),
      Qt::LeftDockWidgetArea
    );

  //
  // Right pane.

#ifdef OTB_WRAP_QT

  assert( m_OtbApplicationsBrowserDock==NULL );
  m_OtbApplicationsBrowserDock =
    AddDockWidget
    < ApplicationsToolBox, ApplicationsToolBoxController, QDockWidget >
    ( "APPLICATIONS_BROWSER",
      tr( "OTB Applications browser" ),
      Qt::RightDockWidgetArea );

#endif
}

/*****************************************************************************/
void
MainWindow
::InitializeCentralWidget()
{
  //
  // need to setup the central widget as QTabWidget to be able to 
  // open several applications + the quicklook
  m_CentralTabWidget = new QTabWidget();
  m_CentralTabWidget->setTabsClosable(true);

  // 
  // add this tabWidget as central Widget
  setCentralWidget( m_CentralTabWidget );
}

/*****************************************************************************/
GLImageWidget*
MainWindow
::CreateQuicklookWidget( QGLWidget* sharedGlWidget )
{
  QuicklookViewManipulator* manipulator =
    new QuicklookViewManipulator( this );

  ImageModelRenderer* renderer =
    new ImageModelRenderer( this );

  GLImageWidget* quicklookView = new GLImageWidget(
    manipulator, // (will be reparented.)
    renderer, // (will be reparented.)
    this,
    sharedGlWidget
  );

  quicklookView->setMinimumSize( 100, 100 );

  return quicklookView;
}

/*****************************************************************************/
/* SLOTS                                                                     */

/*****************************************************************************/
void
MainWindow
::on_action_ColorSetup_toggled()
{
  m_ColorSetupDock->setVisible( m_UI->action_ColorSetup->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::on_action_ColorDynamics_toggled()
{
  m_ColorDynamicsDock->setVisible( m_UI->action_ColorDynamics->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::on_action_DatabaseBrowser_toggled()
{
  m_DatabaseBrowserDock->setVisible( m_UI->action_DatabaseBrowser->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::on_action_DatasetProperties_toggled()
{
  m_DatasetPropertiesDock->setVisible(
    m_UI->action_DatasetProperties->isChecked()
  );
}

/*****************************************************************************/
void
MainWindow
::on_action_OtbApplicationsBrowser_toggled()
{
  m_OtbApplicationsBrowserDock->setVisible(
    m_UI->action_OtbApplicationsBrowser->isChecked()
  );
}

/*****************************************************************************/
void
MainWindow
::on_action_QuicklookView_toggled()
{
  m_QuicklookViewDock->setVisible( m_UI->action_QuicklookView->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::OnColorDynamicsVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_ColorDynamics, visible );
}

/*****************************************************************************/
void
MainWindow
::OnColorSetupVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_ColorSetup, visible );
}

/*****************************************************************************/
void
MainWindow
::OnDatabaseBrowserVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_DatabaseBrowser, visible );
}

/*****************************************************************************/
void
MainWindow
::OnDatasetPropertiesVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_DatasetProperties, visible );
}

/*****************************************************************************/
void
MainWindow
::OnOtbApplicationsBrowserVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_OtbApplicationsBrowser, visible );
}

/*****************************************************************************/
void
MainWindow
::OnQuicklookViewVisibilityChanged( bool visible )
{
  VisibilityChanged( m_UI->action_QuicklookView, visible );
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeModel( const AbstractModel* model )
{
  qDebug() << this << "::OnAboutToChangeModel(" << model << ")";

  SetControllerModel( m_DatabaseBrowserDock, NULL );

  assert( I18nApplication::Instance()!=NULL );

  DatabaseModel* databaseModel =
    I18nApplication::Instance()->GetModel< DatabaseModel >();
  // Check that NULL==NULL or (DatabaseModel*)==(AbstractModel*)
  assert( databaseModel==I18nApplication::Instance()->GetModel() );

  if( databaseModel==NULL )
    return;

  // Disonnect database-model from main-window when selected
  // dataset-model is about to change.
  QObject::disconnect(
    databaseModel,
    SIGNAL( AboutToChangeSelectedDatasetModel( const DatasetModel* ) ),
    // to:
    this,
    SLOT( OnAboutToChangeSelectedDatasetModel( const DatasetModel* ) )
  );

  // Disconnect database-model to main-window when selected
  // dataset-model has been changed.
  QObject::disconnect(
    databaseModel,
    SIGNAL( SelectedDatasetModelChanged( DatasetModel* ) ),
    // to:
    this,
    SLOT( OnSelectedDatasetModelChanged( DatasetModel* ) )
  );
}

/*****************************************************************************/
void
MainWindow
::OnModelChanged( AbstractModel* model )
{
  qDebug() << this << "::OnModelChanged(" << model << ")";

  SetControllerModel( m_DatabaseBrowserDock, model );

  DatabaseModel* databaseModel = qobject_cast< DatabaseModel* >( model );
  assert( databaseModel==model );

  if( databaseModel==NULL )
    return;

  // Connect database-model to main-window when selected dataset-model
  // is about to change.
  QObject::connect(
    databaseModel,
    SIGNAL( AboutToChangeSelectedDatasetModel( const DatasetModel* ) ),
    // to:
    this,
    SLOT( OnAboutToChangeSelectedDatasetModel( const DatasetModel* ) )
  );

  // Connect database-model to main-window when selected dataset-model
  // has been changed.
  QObject::connect(
    databaseModel,
    SIGNAL( SelectedDatasetModelChanged( DatasetModel* ) ),
    // to:
    this,
    SLOT( OnSelectedDatasetModelChanged( DatasetModel* ) )
  );
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeSelectedDatasetModel( const DatasetModel* model )
{
  qDebug() << this << "::OnAboutToChangeSelectedDatasetModel(" << model << ")";

  GLImageWidget* quicklookView = GetQuicklookView();

  assert( quicklookView!=NULL );
  quicklookView->SetImageModel( NULL );

  // unset the dataset model in DatasetProperties controller
  SetControllerModel( m_DatasetPropertiesDock, NULL );
}

/*****************************************************************************/
void
MainWindow
::OnSelectedDatasetModelChanged( DatasetModel* model )
{
  qDebug() << this << "::OnSelectedDatasetModelChanged(" << model << ")";

  GLImageWidget* quicklookView = GetQuicklookView();

  assert( quicklookView!=NULL );
  quicklookView->SetImageModel( model->GetSelectedImageModel() );

  // set the dataset model in DatasetProperties controller
  SetControllerModel( m_DatasetPropertiesDock, model );
}

/*****************************************************************************/
void
MainWindow
::OnApplicationToLaunchSelected(const QString & appName)
{

#ifdef OTB_WRAP_QT
  // need to get the controller to request the application widget
  ApplicationsToolBoxController * controller = 
    m_OtbApplicationsBrowserDock->findChild<ApplicationsToolBoxController *>();

  //
  // add the application in a tab 
  // TODO : check if this application is already opened ???
  m_CentralTabWidget->addTab(controller->GetSelectedApplicationWidget(appName), 
                              appName);

  // no checking needed here, if index is not available nothing is
  // done. Focus on the newly added tab
  m_CentralTabWidget->setCurrentIndex( m_CentralTabWidget->count() - 1 );

  // set tab icon
  m_CentralTabWidget->setTabIcon( m_CentralTabWidget->currentIndex(), 
                                   QIcon(":/images/process_icon")  );

  //
  // connections. not using m_CentralTabWidget->currentWidget() leads
  // to a wrong connection!!!!
  QObject::connect(
    qobject_cast<Wrapper::QtWidgetView *>( m_CentralTabWidget->currentWidget() ),
    SIGNAL( OTBApplicationOutputImageChanged( const QString &, const QString &) ),
    this,
    SLOT( OnOTBApplicationOutputImageChanged( const QString &, const QString &) )
    );

  //
  // on quit widget signal, close its tab
  QObject::connect(
    qobject_cast<Wrapper::QtWidgetView *>( m_CentralTabWidget->currentWidget() ),
    SIGNAL( QuitSignal() ),
    this,
    SLOT( OntabCloseRequested() )
    );

#endif

}

/*****************************************************************************/
void
MainWindow
::OntabCloseRequested()
{
  // get current tab index
  int currentIndex = m_CentralTabWidget->currentIndex();
  
  // close tab and delete its widget
  OntabCloseRequested( currentIndex );  
}


/*****************************************************************************/
void
MainWindow
::OntabCloseRequested(int index)
{
  //
  // Quicklook tab is not removable
  if (index > 0 )
    {
    //
    //
    QWidget * widgetToDelete = m_CentralTabWidget->widget(index);
    
    //
    // remove the tab
    m_CentralTabWidget->removeTab(index);

    //
    // delete the selected widget
    if (widgetToDelete)
      {
      delete widgetToDelete;
      }
    }
}

/*****************************************************************************/
void
MainWindow
::OnOTBApplicationOutputImageChanged( const QString & appName , const QString & outfname)
{
  //
  // If this slot is called, it means that an application has finished
  // its process and has an output image parameter. The 'outfname' in
  // parameters is the output filename. This slot may launch or not,
  // depending on the app settings, the import of the 'outfname' to the
  // catalog database. 
  
  // TODO : implement the import following the catalog settings
  
}

} // end namespace 'mvd'
