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
#include "Core/mvdI18nApplication.h"
//
#include "Gui/mvdDatabaseBrowserController.h"
#include "Gui/mvdDatabaseBrowserWidget.h"
#include "Gui/mvdGLImageWidget.h"
#include "Gui/mvdImageModelRenderer.h"
#include "Gui/mvdQuicklookViewManipulator.h"

#include "Gui/mvdApplicationsToolBox.h"
#include "Gui/mvdApplicationsToolBoxController.h"

#include "Core/mvdI18nApplication.h"
#include "mvdCatalogueApplication.h"
#include "Core/mvdOTBApplicationsModel.h"

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
  m_QuicklookView( NULL ),
  m_ApplicationsBrowserDock(NULL)
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

  assert( m_QuicklookView==NULL );
  m_QuicklookView = CreateQuicklookWidget();

  setCentralWidget( m_QuicklookView );
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{
  //
  // Done here cause needed to be done once
  SetControllerModel(m_ApplicationsBrowserDock, 
                     I18nApplication::Instance< CatalogueApplication >()->GetOTBApplicationsModel());
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


  assert (m_ApplicationsBrowserDock == NULL);
  m_ApplicationsBrowserDock =
    AddDockWidget
    < ApplicationsToolBox, ApplicationsToolBoxController, QDockWidget >
    ( "APPLICATIONS_BROWSER", tr( "Applications browser" ), Qt::RightDockWidgetArea );
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
}

/*****************************************************************************/
void
MainWindow
::OnSelectedDatasetModelChanged( DatasetModel* model )
{
  qDebug() << this << "::OnSelectedDatasetModelChanged(" << model << ")";
}

} // end namespace 'mvd'
