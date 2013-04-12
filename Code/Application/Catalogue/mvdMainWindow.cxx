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
#include "Core/mvdQuicklookModel.h"
#include "Core/mvdVectorImageModel.h"
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
#include "Gui/mvdImageViewManipulator.h"
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
  m_ImageView( NULL ),
  m_QuicklookViewDock( NULL ),
  m_CentralTabWidget( NULL )
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

  InitializeCentralWidget();

  InitializeDockWidgets();
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{
  // Connect quicklook-view dock-widget toggle view action to
  // menu-item action.
   QObject::connect(
    m_QuicklookViewDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_QuicklookView,
    SLOT( setChecked( bool ) )
  );
  // and vice-versa.
  QObject::connect(
    m_UI->action_QuicklookView,
    SIGNAL( toggled( bool ) ),
    // to:
    m_QuicklookViewDock,
    SLOT( setVisible( bool ) )
  );

  // Connect database-browser dock-widget toggle view action to
  // menu-item action.
   QObject::connect(
    m_DatabaseBrowserDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_DatabaseBrowser,
    SLOT( setChecked( bool ) )
  );
  // and vice-versa.
  QObject::connect(
    m_UI->action_DatabaseBrowser,
    SIGNAL( toggled( bool ) ),
    // to:
    m_DatabaseBrowserDock,
    SLOT( setVisible( bool ) )
  );

  // Connect database-properties dock-widget toggle view action to
  // menu-item action.
  assert( m_DatasetPropertiesDock!=NULL );
  QObject::connect(
    m_DatasetPropertiesDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_DatasetProperties,
    SLOT( setChecked( bool ) )
  );
  // and vice-versa.
  QObject::connect(
    m_UI->action_DatasetProperties,
    SIGNAL( toggled( bool ) ),
    // to:
    m_DatasetPropertiesDock,
    SLOT( setVisible( bool ) )
  );

  // Connect color-dynamics dock-widget toggle view action to
  // menu-item action.
  assert( m_ColorDynamicsDock!=NULL );
  QObject::connect(
    m_ColorDynamicsDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_ColorDynamics,
    SLOT( setChecked( bool ) )
    );
  // and vice-versa.
  QObject::connect(
    m_UI->action_ColorDynamics,
    SIGNAL( toggled( bool ) ),
    // to:
    m_ColorDynamicsDock,
    SLOT( setVisible( bool ) )
  );

  // Connect color-setup dock-widget toggle view-action to menu-item
  // action.
  assert( m_ColorSetupDock!=NULL );
  QObject::connect(
    m_ColorSetupDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_ColorSetup,
    SLOT( setChecked( bool ) )
    );
  // and vice-versa.
  QObject::connect(
    m_UI->action_ColorSetup,
    SIGNAL( toggled( bool ) ),
    // to:
    m_ColorSetupDock,
    SLOT( setVisible( bool ) )
  );

#ifdef OTB_WRAP_QT
  // Connect OTB-applications dock-widget toggle view action to
  // menu-item action.
  assert( m_OtbApplicationsBrowserDock!=NULL );
  QObject::connect(
    m_OtbApplicationsBrowserDock->toggleViewAction(),
    SIGNAL( toggled( bool ) ),
    // to:
    m_UI->action_OtbApplicationsBrowser,
    SLOT( setChecked( bool ) )
    );
  // and vice-versa.
  QObject::connect(
    m_UI->action_OtbApplicationsBrowser,
    SIGNAL( toggled( bool ) ),
    // to:
    m_OtbApplicationsBrowserDock,
    SLOT( setVisible( bool ) )
  );

  //
  // Done here cause needed to be done once and only once.
  SetControllerModel(
    m_OtbApplicationsBrowserDock, 
    CatalogueApplication::Instance()->GetOTBApplicationsModel()
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
      m_OtbApplicationsBrowserDock->findChild< ApplicationsToolBox* >() 
    );
  assert( appWidget!=NULL );
  
  // # Step 2 : setup connections
  QObject::connect(
    appWidget,
    SIGNAL( ApplicationToLaunchSelected(const QString &) ),
    this,
    SLOT( OnApplicationToLaunchSelected(const QString &) )
  );

  // # Step 3 : if OTB applications has any output, it may be 
  // ???

#endif

  //
  // close tabs handling
  QObject::connect(
    m_CentralTabWidget,
    SIGNAL( tabCloseRequested( int ) ),
    this,
    SLOT( OnTabCloseRequested( int ) )
  );

  // Access the quicklook-view.
  GLImageWidget* quicklookView = GetQuicklookView();
  assert( quicklookView!=NULL );

  //
  // Connect image-view manipulator to quicklook-view renderer to
  // handle viewportRegionChanged() signals.
  QObject::connect(
    m_ImageView->GetImageViewManipulator(),
    SIGNAL(
      ViewportRegionRepresentationChanged( const PointType&, const PointType& )
    ),
    // to:
    quicklookView->GetImageModelRenderer(),
    SLOT(
      OnViewportRegionRepresentationChanged( const PointType&, const PointType& )
    )
    );

  // Connect quicklook-view manipulator to image-view renderer to
  // handle mousePressEventpressed().
  QObject::connect(
    quicklookView->GetImageViewManipulator(),
    SIGNAL( ViewportRegionChanged( double, double ) ),
    // to:
    m_ImageView->GetImageViewManipulator(),
    SLOT( OnViewportRegionChanged( double, double ) )
  );
}

/*****************************************************************************/
void
MainWindow
::InitializeDockWidgets()
{
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

#ifdef OTB_WRAP_QT
  // OTB-applications browser.
  assert( m_OtbApplicationsBrowserDock==NULL );
  m_OtbApplicationsBrowserDock =
    AddDockWidget
    < ApplicationsToolBox, ApplicationsToolBoxController, QDockWidget >
    ( "APPLICATIONS_BROWSER",
      tr( "OTB Applications browser" ),
      Qt::LeftDockWidgetArea );

  tabifyDockWidget( m_DatasetPropertiesDock, m_OtbApplicationsBrowserDock );
#endif

  //
  // Right pane.

  // Quicklook-view dock-widget
  assert( m_QuicklookViewDock==NULL );
  assert( m_ImageView!=NULL );
  m_QuicklookViewDock = AddWidgetToDock(
    CreateQuicklookWidget( m_ImageView),
    "QUICKLOOK_VIEW",
    tr( "Quicklook view" ),
    Qt::RightDockWidgetArea
  );

  // Color-setup.
  assert( m_ColorSetupDock==NULL );
  m_ColorSetupDock =
    AddDockWidget
    < ColorSetupWidget, ColorSetupController, QDockWidget >
    ( "COLOR_SETUP",
      tr( "Color setup" ),
      Qt::RightDockWidgetArea
    );

  // m_ColorSetupDock->setVisible( false );

  // Color-dynamics.
  assert( m_ColorDynamicsDock==NULL );
  m_ColorDynamicsDock =
    AddDockWidget
    < ColorDynamicsWidget, ColorDynamicsController, QDockWidget >
    ( "COLOR_DYNAMICS",
      tr( "Color dynamics" ),
      Qt::RightDockWidgetArea
    );

  // m_ColorDynamicsDock->setVisible( false );

  // Tabify dock-widgets.
  tabifyDockWidget( m_ColorSetupDock, m_ColorDynamicsDock );
}

/*****************************************************************************/
void
MainWindow
::InitializeCentralWidget()
{
  // Create central tab-widget for multi-view support.
  assert( m_CentralTabWidget==NULL );
  m_CentralTabWidget = new QTabWidget( this );

  // Customize it.
  m_CentralTabWidget->setTabsClosable( true );

  // Set-it up as central widget.
  setCentralWidget( m_CentralTabWidget );

  // Initialize image-view.
  assert( m_ImageView==NULL );
  m_ImageView = CreateImageWidget();

  // Add first tab: image-view.
  m_CentralTabWidget->addTab(
    m_ImageView,
    tr( "Image view" )
  );

  //
  // access to the quicklook tabBar to remove the close button
  QTabBar* tabBar = m_CentralTabWidget->findChild< QTabBar* >();

  tabBar->setTabButton(0, QTabBar::RightSide, 0);
  tabBar->setTabButton(0, QTabBar::LeftSide, 0);
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
GLImageWidget*
MainWindow
::CreateImageWidget( QGLWidget* sharedGlWidget )
{
  ImageViewManipulator* manipulator =
    new ImageViewManipulator( this );

  ImageModelRenderer* renderer =
    new ImageModelRenderer( this );

  GLImageWidget* imageView = new GLImageWidget(
    manipulator, // (will be reparented.)
    renderer, // (will be reparented.)
    this,
    sharedGlWidget
  );

  return imageView;
}

/*****************************************************************************/
/* SLOTS                                                                     */

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeModel( const AbstractModel* model )
{
  qDebug() << this << "::OnAboutToChangeModel(" << model << ")";

  SetControllerModel( m_DatabaseBrowserDock, NULL );

  assert( CatalogueApplication::Instance()!=NULL );

  DatabaseModel* databaseModel =
    CatalogueApplication::Instance()->GetModel< DatabaseModel >();
  // Check that NULL==NULL or (DatabaseModel*)==(AbstractModel*)
  assert( databaseModel==CatalogueApplication::Instance()->GetModel() );

  // Exit, if there were no previously set database model.
  if( databaseModel==NULL )
    return;

  // Force to disconnect previously selected dataset-model before
  // database-model is connected.
  //
  // N.B.: This will cause UI controllers to disable widgets.
  OnAboutToChangeSelectedDatasetModel( NULL );

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

  // Force to onnect selected dataset-model after database-model is
  // connected.
  //
  // N.B.: This will cause UI controllers to disable widgets.
  OnSelectedDatasetModelChanged( databaseModel->GetSelectedDatasetModel() );
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeSelectedDatasetModel( const DatasetModel* model )
{
  qDebug() << this << "::OnAboutToChangeSelectedDatasetModel(" << model << ")";

  // Unsert dataset-model from dataset-properties controller.
  SetControllerModel( m_DatasetPropertiesDock, NULL );

  // Update image-view.
  m_ImageView->SetImageModel( NULL );

  // Access quicklook-view.
  GLImageWidget* quicklookView = GetQuicklookView();

  // Update quicklook-view.
  assert( quicklookView!=NULL );
  quicklookView->SetImageModel( NULL );

  // Unset image-model to color-dynamics controller.
  SetControllerModel( m_ColorDynamicsDock, NULL );

  // Unset image-model to color-setup controller.
  SetControllerModel( m_ColorSetupDock, NULL ); 
}

/*****************************************************************************/
void
MainWindow
::OnSelectedDatasetModelChanged( DatasetModel* model )
{
  qDebug() << this << "::OnSelectedDatasetModelChanged(" << model << ")";

  // Assign dataset-model to dataset-properties controller.
  SetControllerModel( m_DatasetPropertiesDock, model );

  // Access vector-image model.
  VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  // Check type.
  assert( vectorImageModel==model->GetSelectedImageModel() );

  // Update image-view.
  assert( m_ImageView!=NULL );
  m_ImageView->SetImageModel( vectorImageModel );

  // Access quicklook-view.
  GLImageWidget* quicklookView = GetQuicklookView();

  // Update quicklook-view.
  assert( quicklookView!=NULL );
  quicklookView->SetImageModel(
    vectorImageModel==NULL
    ? NULL
    : vectorImageModel->GetQuicklookModel()
  );

  // Assign image-model to color-dynamics controller.
  SetControllerModel( m_ColorDynamicsDock, vectorImageModel );

  // Assign image-model to color-setup controller.
  SetControllerModel( m_ColorSetupDock, vectorImageModel );
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
    SLOT( OnTabCloseRequested() )
    );

#endif

}

/*****************************************************************************/
void
MainWindow
::OnTabCloseRequested()
{
  // get current tab index
  int currentIndex = m_CentralTabWidget->currentIndex();
  
  // close tab and delete its widget
  OnTabCloseRequested( currentIndex );  
}


/*****************************************************************************/
void
MainWindow
::OnTabCloseRequested(int index)
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
