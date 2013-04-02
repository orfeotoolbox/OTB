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
#include "Core/mvdDatabaseModel.h"
#include "Core/mvdDatasetModel.h"
#include "Core/mvdI18nApplication.h"
//
#include "Gui/mvdDatabaseBrowserController.h"
#include "Gui/mvdDatabaseBrowserWidget.h"
#include "Gui/mvdGLImageWidget.h"
#include "Gui/mvdImageModelRenderer.h"
#include "Gui/mvdQuicklookViewManipulator.h"
#include "Gui/mvdApplicationsToolBox.h"

#include "Core/mvdI18nApplication.h"
#include "mvdCatalogueApplication.h"

#ifdef OTB_WRAP_QT
#include "ApplicationsWrapper/mvdApplicationsToolBoxController.h"
#include "ApplicationsWrapper/mvdOTBApplicationsModel.h"
#include "ApplicationsWrapper/mvdWrapperQtWidgetView.h"
#endif

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
  m_DatabaseBrowserDock( NULL ),
  m_ApplicationsBrowserDock(NULL),
  m_QuicklookView( NULL )
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

  assert( m_QuicklookView==NULL );
  m_QuicklookView = CreateQuicklookWidget();

  m_CentralWidgetTabs->addTab(m_QuicklookView, "Quicklook");
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{

#ifdef OTB_WRAP_QT
  //
  // Done here cause needed to be done once
  SetControllerModel(m_ApplicationsBrowserDock, 
                     I18nApplication::Instance< CatalogueApplication >()->GetOTBApplicationsModel());

  //
  // need to get the ApplicationToolBox widget to setup connections.
  // a double click on the tree widget should trigger a signal connected
  // to this MainWindow slot. this slot will be in charge of getting the
  // widget of the application selected, and show it in the
  // MainWindow centralView.  
  
  // # Step 1 : get the ApplicationToolBoxWidget
  ApplicationsToolBox* appWidget = 
    qobject_cast< ApplicationsToolBox * >( 
      m_ApplicationsBrowserDock->findChild< ApplicationsToolBox* >( ) 
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
  QObject::connect(m_CentralWidgetTabs,
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
  assert( m_DatabaseBrowserDock==NULL );
  m_DatabaseBrowserDock =
    AddDockWidget
    < DatabaseBrowserWidget, DatabaseBrowserController, QDockWidget >
    ( "DATABASE_BROWSER", tr( "Database browser" ), Qt::LeftDockWidgetArea );


#ifdef OTB_WRAP_QT

  assert (m_ApplicationsBrowserDock == NULL);
  m_ApplicationsBrowserDock =
    AddDockWidget
    < ApplicationsToolBox, ApplicationsToolBoxController, QDockWidget >
    ( "APPLICATIONS_BROWSER", tr( "Applications browser" ), Qt::RightDockWidgetArea );

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
  m_CentralWidgetTabs = new QTabWidget();
  m_CentralWidgetTabs->setTabsClosable(true);

  // 
  // add this tabWidget as central Widget
  setCentralWidget( m_CentralWidgetTabs );
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

  return quicklookView;
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

  assert( m_QuicklookView!=NULL );
  m_QuicklookView->SetImageModel( NULL );
}

/*****************************************************************************/
void
MainWindow
::OnSelectedDatasetModelChanged( DatasetModel* model )
{
  qDebug() << this << "::OnSelectedDatasetModelChanged(" << model << ")";

  assert( m_QuicklookView!=NULL );
  m_QuicklookView->SetImageModel( model->GetSelectedImageModel() );
}

/*****************************************************************************/
void
MainWindow
::OnApplicationToLaunchSelected(const QString & appName)
{

#ifdef OTB_WRAP_QT
  // need to get the controller to request the application widget
  ApplicationsToolBoxController * controller = 
    m_ApplicationsBrowserDock->findChild<ApplicationsToolBoxController *>();

  //
  // add the application in a tab 
  // TODO : check if this application is already opened ???
  m_CentralWidgetTabs->addTab(controller->GetSelectedApplicationWidget(appName), 
                              appName);

  // no checking needed here, if index is not available nothing is
  // done. Focus on the newly added tab
  m_CentralWidgetTabs->setCurrentIndex( m_CentralWidgetTabs->count() - 1 );

  // set tab icon
  m_CentralWidgetTabs->setTabIcon( m_CentralWidgetTabs->currentIndex(), 
                                   QIcon(":/images/process_icon")  );

  //
  // connections. not using m_CentralWidgetTabs->currentWidget() leads
  // to a wrong connection!!!!
  QObject::connect(
    qobject_cast<Wrapper::QtWidgetView *>( m_CentralWidgetTabs->currentWidget() ),
    SIGNAL( OTBApplicationOutputImageChanged( const QString &, const QString &) ),
    this,
    SLOT( OnOTBApplicationOutputImageChanged( const QString &, const QString &) )
    );

  //
  // on quit widget signal, close its tab
  QObject::connect(
    qobject_cast<Wrapper::QtWidgetView *>( m_CentralWidgetTabs->currentWidget() ),
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
  int currentIndex = m_CentralWidgetTabs->currentIndex();
  
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
    QWidget * widgetToDelete = m_CentralWidgetTabs->widget(index);
    
    //
    // remove the tab
    m_CentralWidgetTabs->removeTab(index);

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
