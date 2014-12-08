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
#if ENABLE_TREE_WIDGET_TEST
#include "Gui/mvdDatabaseBrowserWidgetTest.h"
#else // ENABLE_TREE_WIDGET_TEST
#include "Gui/mvdDatabaseBrowserWidget.h"
#endif // ENABLE_TREE_WIDGET_TEST
#include "Gui/mvdDatasetPropertiesController.h"
#include "Gui/mvdDatasetPropertiesWidget.h"
#include "Gui/mvdFilenameDragAndDropEventFilter.h"
#include "Gui/mvdHistogramController.h"
#include "Gui/mvdHistogramWidget.h"
#include "Gui/mvdImageViewManipulator.h"
#include "Gui/mvdImageViewRenderer.h"
#include "Gui/mvdImageViewWidget.h"
#include "Gui/mvdPixelDescriptionWidget.h"
#include "Gui/mvdQuicklookViewManipulator.h"
#include "Gui/mvdQuicklookViewRenderer.h"
#include "Gui/mvdStatusBarWidget.h"
//
#include "mvdApplication.h"
#include "mvdPreferencesDialog.h"

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
  m_PixelDescriptionDock(NULL),
  m_HistogramDock( NULL ),
#ifdef OTB_WRAP_QT
  m_OtbApplicationsBrowserDock(NULL),
#endif
  m_ImageView( NULL ),
  m_QuicklookViewDock( NULL ),
  m_CentralTabWidget( NULL ),
  m_FilenameDragAndDropEventFilter( NULL )
{
  m_UI->setupUi( this );

  //
  // Event filters.
  m_FilenameDragAndDropEventFilter = new FilenameDragAndDropEventFilter( this );

  QObject::connect(
    m_FilenameDragAndDropEventFilter,
    SIGNAL( FilenameDropped( const QString& ) ),
    // to:
    this,
    SLOT( OnFilenameDropped( const QString& ) )
  );
}

/*****************************************************************************/
MainWindow
::~MainWindow()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
bool
MainWindow
::CheckGLCapabilities() const
{
  assert( m_ImageView!=NULL );
  assert( m_ImageView->GetRenderer()!=NULL );

  if( m_ImageView==NULL ||
      m_ImageView->GetRenderer()==NULL )
    return false;

  return m_ImageView->GetRenderer()->CheckGLCapabilities();
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

  InitializeStatusBarWidgets();
}

/*****************************************************************************/
void
MainWindow
::virtual_ConnectUI()
{
  ConnectViewMenu();

  //
  // CHAIN CONTROLLERS.
  // Forward model update signals of color-setup controller...
  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( RgbChannelIndexChanged( RgbwChannel, int ) ),
    // to: ...color-dynamics controller model update signal.
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SLOT( OnRgbChannelIndexChanged( RgbwChannel, int ) )
  );
  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( GrayChannelIndexChanged( int ) ),
    // to: ...color-dynamics controller model update signal.
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SLOT( OnGrayChannelIndexChanged( int ) )
  );
  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( GrayscaleActivated( bool ) ),
    // to: ...color-dynamics controller model update signal.
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SLOT( OnGrayscaleActivated( bool ) )
  );

  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( RgbChannelIndexChanged( RgbwChannel, int ) ),
    // to: ...color-dynamics controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnRgbChannelIndexChanged( RgbwChannel, int ) )
  );
  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( GrayChannelIndexChanged( int ) ),
    // to: ...color-dynamics controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnGrayChannelIndexChanged( int ) )
  );
  QObject::connect(
    m_ColorSetupDock->findChild< AbstractModelController* >(),
    SIGNAL( GrayscaleActivated( bool ) ),
    // to: ...color-dynamics controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnGrayscaleActivated( bool ) )
  );

  QObject::connect(
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SIGNAL( LowIntensityChanged( RgbwChannel, double, bool ) ),
    // to: ...histogram controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnLowIntensityChanged( RgbwChannel, double, bool ) )
  );
  QObject::connect(
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SIGNAL( HighIntensityChanged( RgbwChannel, double, bool ) ),
    // to: ...histogram controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnHighIntensityChanged( RgbwChannel, double, bool ) )
  );
  QObject::connect(
    m_ColorDynamicsDock->findChild< AbstractModelController* >(),
    SIGNAL( HistogramRefreshed() ),
    // to: ...histogram controller model update signal.
    m_HistogramDock->findChild< AbstractModelController* >(),
    SLOT( OnHistogramRefreshed() )
  );

  //
  // OTB application support.
#ifdef OTB_WRAP_QT
  //
  // Done here cause needed to be done once and only once.
  SetControllerModel(
    m_OtbApplicationsBrowserDock,
    Application::Instance()->GetOTBApplicationsModel()
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
    SIGNAL( ApplicationToLaunchSelected(const QString &, const QString &) ),
    this,
    SLOT( OnApplicationToLaunchSelected(const QString &, const QString &) )
  );

  // # Step 3 : connect close slots
#endif

  //
  // close tabs handling
  QObject::connect(
    m_CentralTabWidget,
    SIGNAL( tabCloseRequested( int ) ),
    this,
    SLOT( OnTabCloseRequested( int ) )
  );


  //
  // EVENT FILTERS.

  // Database browser
  DatabaseBrowserController* dbBrowserController =
    m_DatabaseBrowserDock->findChild< DatabaseBrowserController* >();
  assert( dbBrowserController!=NULL );

#if ENABLE_TREE_WIDGET_TEST
  assert(
    dbBrowserController->GetWidget()==
    dbBrowserController->GetWidget< DatabaseBrowserWidgetTest >()
  );

  DatabaseBrowserWidgetTest* dbBrowserWidget =
    dbBrowserController->GetWidget< DatabaseBrowserWidgetTest >();

#else // ENABLE_TREE_WIDGET_TEST
  assert(
    dbBrowserController->GetWidget()==
    dbBrowserController->GetWidget< DatabaseBrowserWidget >()
  );

  DatabaseBrowserWidget* dbBrowserWidget =
    dbBrowserController->GetWidget< DatabaseBrowserWidget >();
#endif // ENABLE_TREE_WIDGET_TEST

  dbBrowserWidget->InstallTreeEventFilter( m_FilenameDragAndDropEventFilter );

  //
  // Image views
  ConnectImageViews();
}

/*****************************************************************************/
void
MainWindow
::ConnectImageViews()
{
  assert( m_ImageView!=NULL );

  QObject::connect(
    this,
    SIGNAL( UserZoomIn() ),
    // to:
    m_ImageView,
    SLOT( ZoomIn() )
  );

  QObject::connect(
    this,
    SIGNAL( UserZoomOut() ),
    // to:
    m_ImageView,
    SLOT( ZoomOut() )
  );

  QObject::connect(
    this,
    SIGNAL( UserZoomExtent() ),
    m_ImageView,
    SLOT( ZoomToExtent() )
  );

  QObject::connect(
    this,
    SIGNAL( UserZoomFull() ),
    m_ImageView,
    SLOT( ZoomToFullResolution() )
  );

  //
  // Connect image-views for ROI-changed events.

  const AbstractImageViewManipulator* imageViewManipulator =
    m_ImageView->GetManipulator();

  assert( imageViewManipulator!=NULL );


  ImageViewWidget* quicklookView = GetQuicklookView();
  assert( quicklookView!=NULL );

  const AbstractImageViewManipulator* quicklookManipulator =
    quicklookView->GetManipulator();

  assert( quicklookManipulator!=NULL );


  QObject::connect(
    m_ImageView,
    SIGNAL(
      RoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    ),
    // to:
    quicklookManipulator,
    SLOT(
      OnRoiChanged(
        const PointType&, const SizeType&, const SpacingType&, const PointType&
      )
    )
  );

  QObject::connect(
    quicklookView,
    SIGNAL( CenterRoiRequested( const PointType& ) ),
    // to:
    imageViewManipulator,
    SLOT( CenterOn( const PointType& ) )
  );

  QObject::connect(
    quicklookManipulator,
    SIGNAL( RefreshViewRequested() ),
    // to:
    m_ImageView,
    SLOT( updateGL() )
  );
}

/*****************************************************************************/
void
MainWindow
::ConnectViewMenu()
{
  m_UI->menu_View->addAction( m_UI->m_ToolBar->toggleViewAction() );
  
  m_UI->menu_View->addSeparator();

  m_UI->menu_View->addAction( m_ColorSetupDock->toggleViewAction() );
  m_UI->menu_View->addAction( m_ColorDynamicsDock->toggleViewAction() );
  m_UI->menu_View->addAction( m_HistogramDock->toggleViewAction() );

  m_UI->menu_View->addSeparator();

  m_UI->menu_View->addSeparator();

  m_UI->menu_View->addAction( m_DatabaseBrowserDock->toggleViewAction() );
  m_UI->menu_View->addAction( m_DatasetPropertiesDock->toggleViewAction() );
  m_UI->menu_View->addAction( m_PixelDescriptionDock->toggleViewAction() );
#ifdef OTB_WRAP_QT
  m_UI->menu_View->addAction(
    m_OtbApplicationsBrowserDock->toggleViewAction() );
#endif
}

/*****************************************************************************/
void
MainWindow
::ConnectStatusBar( DatasetModel * model)
{
  assert( m_StatusBarWidget!=NULL );

  assert( m_ImageView!=NULL );

  QObject::connect(
    m_ImageView,
    SIGNAL( ScaleChanged( double, double ) ),
    // to:
    m_StatusBarWidget,
    SLOT( SetScale( double, double ) )
  );

  QObject::connect(
    m_StatusBarWidget,
    SIGNAL( ScaleChanged( double ) ),
    // to:
    m_ImageView->GetManipulator(),
    SLOT( ZoomTo( double ) )
  );

  QObject::connect(
    m_StatusBarWidget,
    SIGNAL( PixelIndexChanged( const IndexType& ) ),
    // to:
    m_ImageView,
    SLOT( CenterOn( const IndexType& ) )
  );

  // Access vector-image model.
  VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  if ( vectorImageModel )
    {
    //
    // send the physical point of the clicked point in screen
    // vectorImageModel is in charge of pixel information computation
    QObject::connect(
      m_ImageView,
      SIGNAL(
        PhysicalCursorPositionChanged( const PointType&,
                                       const DefaultImageType::PixelType& )
      ),
      // to:
      vectorImageModel,
      SLOT( OnPhysicalCursorPositionChanged( const PointType&,
                                             const DefaultImageType::PixelType& )
      )
    );

    QObject::connect(
      GetQuicklookView(),
      SIGNAL(
        PhysicalCursorPositionChanged( const PointType&,
                                       const DefaultImageType::PixelType& )
      ),
      // to:
      vectorImageModel,
      SLOT( OnPhysicalCursorPositionChanged( const PointType&,
                                             const DefaultImageType::PixelType& )
      )
    );

    //
    // Connect the status-bar widget to the vector-image corresponding
    // signal.
    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentIndexUpdated( const IndexType&, bool ) ),
      // to:
      m_StatusBarWidget,
      SLOT( SetPixelIndex( const IndexType&, bool ) )
    );

    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentRadioUpdated( const QString& ) ),
      // to:
      m_StatusBarWidget,
      SLOT( SetPixelRadiometryText( const QString& ) )
    );
    }
}

/*****************************************************************************/
void
MainWindow
::ConnectPixelDescriptionWidget( DatasetModel * model)
{
  // Access vector-image model.
  VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  // get the PixelDescription widget
  PixelDescriptionWidget * wpixelDescription = 
    qobject_cast<PixelDescriptionWidget*>(
      m_PixelDescriptionDock->findChild<PixelDescriptionWidget*>()
    );

  if ( vectorImageModel && wpixelDescription )
    {
    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentPhysicalUpdated(const QStringList& ) ),
      wpixelDescription,
      SLOT( OnCurrentPhysicalUpdated(const QStringList&) )
      );

    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentGeographicUpdated(const QStringList& ) ),
      wpixelDescription,
      SLOT( OnCurrentGeographicUpdated(const QStringList& ) )
      );

    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentPixelValueUpdated(const VectorImageType::PixelType&, 
                                       const QStringList& ) ),
      wpixelDescription,
      SLOT( OnCurrentPixelValueUpdated(const VectorImageType::PixelType&, 
                                       const QStringList& ) )
      );
    }
}

/*****************************************************************************/
void
MainWindow
::DisconnectPixelDescriptionWidget( const DatasetModel * model)
{
  // WIP

  // Access vector-image model.
  const VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  // get the PixelDescription widget
  PixelDescriptionWidget * wpixelDescription = 
    qobject_cast<PixelDescriptionWidget*>(
      m_PixelDescriptionDock->findChild<PixelDescriptionWidget*>()
    );

  if ( vectorImageModel && wpixelDescription )
    {
    QObject::disconnect(
      vectorImageModel,
      SIGNAL( CurrentPhysicalUpdated( const QStringList& ) ),
      // to:
      wpixelDescription,
      SLOT( OnCurrentPhysicalUpdated( const QStringList& ) )
    );

    QObject::disconnect(
      vectorImageModel,
      SIGNAL( CurrentGeographicUpdated( const QStringList& ) ),
      // to:
      wpixelDescription,
      SLOT( OnCurrentGeographicUpdated( const QStringList& ) )
    );

    QObject::connect(
      vectorImageModel,
      SIGNAL( CurrentPixelValueUpdated( const VectorImageType::PixelType&,
                                        const QStringList& ) ),
      // to:
      wpixelDescription,
      SLOT( OnCurrentPixelValueUpdated( const VectorImageType::PixelType&,
                                        const QStringList& ) )
    );
    }
}

/*****************************************************************************/
void
MainWindow
::DisconnectStatusBar( const DatasetModel * model )
{
  assert( m_StatusBarWidget!=NULL );

  assert( m_ImageView!=NULL );

  QObject::disconnect(
    m_ImageView,
    SIGNAL( ScaleChanged( double, double ) ),
    // to:
    m_StatusBarWidget,
    SLOT( SetScale( double, double ) )
  );

  QObject::disconnect(
    m_StatusBarWidget,
    SIGNAL( ScaleChanged( double ) ),
    // to:
    m_ImageView->GetManipulator(),
    SLOT( ZoomTo( double ) )
  );

  QObject::disconnect(
    m_StatusBarWidget,
    SIGNAL( PixelIndexChanged( const IndexType& ) ),
    // to:
    m_ImageView,
    SLOT( CenterOn( const IndexType& ) )
  );

  // Access vector-image model.
  const VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  if( vectorImageModel )
    {
    //
    // send the physical point of the clicked point in screen
    // vectorImageModel is in charge of pixel information computation
    QObject::disconnect(
      m_ImageView,
      SIGNAL(
        PhysicalCursorPositionChanged( const PointType&,
                                       const DefaultImageType::PixelType& )
      ),
      // to:
      vectorImageModel,
      SLOT( OnPhysicalCursorPositionChanged( const PointType&,
                                             const DefaultImageType::PixelType& )
      )
    );

    QObject::disconnect(
      GetQuicklookView(),
      SIGNAL(
        PhysicalCursorPositionChanged( const PointType&,
                                       const DefaultImageType::PixelType& )
      ),
      // to:
      vectorImageModel,
      SLOT( OnPhysicalCursorPositionChanged( const PointType&,
                                             const DefaultImageType::PixelType& )
      )
    );

    //
    // disconnect the statusBar widget to the vectorImage corresponding
    // signal
    QObject::disconnect(
      vectorImageModel,
      SIGNAL( CurrentIndexUpdated( const IndexType&, bool ) ),
      m_StatusBarWidget,
      SLOT( SetPixelIndex( const IndexType&, bool ) )
      );

    QObject::disconnect(
      vectorImageModel,
      SIGNAL( CurrentRadioUpdated(const QString& ) ),
      m_StatusBarWidget,
      SLOT( SetPixelRadiometryText(const QString &) )
      );
    }
}

/*****************************************************************************/
void
MainWindow
::InitializeDockWidgets()
{
#define ENABLE_QTOOLBAR_TEST 0
#if ENABLE_QTOOLBAR_TEST
  // XP: QToolBar test.
  QDockWidget* dock =
    AddWidgetToDock(
      m_UI->m_ToolBar,
      "TOOL_BAR",
      tr( "Tool bar" ),
      Qt::TopDockWidgetArea
    );

  m_UI->menu_View->addAction( dock->toggleViewAction() );
#endif

  //
  // Left pane.

  // Database-browser.
  assert( m_DatabaseBrowserDock==NULL );
#if ENABLE_TREE_WIDGET_TEST
  m_DatabaseBrowserDock =
    AddDockWidget
    < DatabaseBrowserWidgetTest, DatabaseBrowserController, QDockWidget >
    ( "DATABASE_BROWSER",
      tr( "Database browser (test)" ),
      Qt::LeftDockWidgetArea
    );
#else // ENABLE_TREE_WIDGET_TEST
  m_DatabaseBrowserDock =
    AddDockWidget
    < DatabaseBrowserWidget, DatabaseBrowserController, QDockWidget >
    ( "DATABASE_BROWSER",
      tr( "Database browser" ),
      Qt::LeftDockWidgetArea
    );
#endif // ENABLE_TREE_WIDGET_TEST

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
    CreateQuicklookViewWidget( m_ImageView ),
    "QUICKLOOK_VIEW",
    tr( "Quicklook view" ),
    Qt::RightDockWidgetArea
  );

  assert( m_HistogramDock==NULL );
  m_HistogramDock =
    AddDockWidget
    < HistogramWidget, HistogramController, QDockWidget >
    ( "HISTOGRAM",
      tr( "Histogram" ),
      Qt::RightDockWidgetArea,
      false
    );

  // Pixel Description (no controller needed here / direct update of
  // the pixel description through signals from VectorImageModel)
  assert( m_PixelDescriptionDock==NULL );
  m_PixelDescriptionDock =
    AddDockWidget
    < PixelDescriptionWidget, QDockWidget >
    ( "CURRENT_PIXEL_DESCRIPTION",
      tr( "Pixel Description" ),
      Qt::RightDockWidgetArea
    );

  tabifyDockWidget( m_PixelDescriptionDock, m_HistogramDock );

  // Color-setup.
  assert( m_ColorSetupDock==NULL );
  m_ColorSetupDock =
    AddDockWidget
    < ColorSetupWidget, ColorSetupController, QDockWidget >
    ( "COLOR_SETUP",
      tr( "Color setup" ),
      Qt::RightDockWidgetArea
    );

  // Color-dynamics.
  assert( m_ColorDynamicsDock==NULL );
  m_ColorDynamicsDock =
    AddDockWidget
    < ColorDynamicsWidget, ColorDynamicsController, QDockWidget >
    ( "COLOR_DYNAMICS",
      tr( "Color dynamics" ),
      Qt::RightDockWidgetArea
    );

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

  //
  // access to the quicklook tabBar to remove the close button
  QTabBar* tabBar = m_CentralTabWidget->findChild< QTabBar* >();

  // Initialize image-view.
  assert( m_ImageView==NULL );
  m_ImageView = CreateImageViewWidget();

  // Add first tab: image-view.
  int index = m_CentralTabWidget->addTab(
    m_ImageView,
    tr( "Image view" )
  );

  tabBar->setTabButton( index, QTabBar::RightSide, 0);
  tabBar->setTabButton( index, QTabBar::LeftSide, 0);
}

/*****************************************************************************/
void
MainWindow
::InitializeStatusBarWidgets()
{
  m_StatusBarWidget = new StatusBarWidget(statusBar());
  statusBar()->addPermanentWidget(m_StatusBarWidget, 1);
}

/*****************************************************************************/
ImageViewWidget*
MainWindow
::CreateImageViewWidget( QGLWidget* sharedGlWidget )
{
  ImageViewRenderer* renderer =
    new ImageViewRenderer( this );

  ImageViewManipulator* manipulator =
    new ImageViewManipulator(
#if USE_VIEW_SETTINGS_SIDE_EFFECT
      renderer->GetViewSettings(),
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
      this
    );

  ImageViewWidget* imageView = new ImageViewWidget(
    manipulator, // (will be reparented.)
    renderer, // (will be reparented.)
    this,
    sharedGlWidget
  );

  imageView->setMinimumWidth( 256 );

  return imageView;
}

/*****************************************************************************/
ImageViewWidget*
MainWindow
::CreateQuicklookViewWidget( QGLWidget* sharedGlWidget )
{
  QuicklookViewRenderer* renderer =
    new QuicklookViewRenderer( this );

  QuicklookViewManipulator* manipulator =
    new QuicklookViewManipulator(
#if USE_VIEW_SETTINGS_SIDE_EFFECT
      renderer->GetViewSettings(),
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT
      this
    );

  ImageViewWidget* quicklookView = new ImageViewWidget(
    manipulator, // (will be reparented.)
    renderer, // (will be reparented.)
    this,
    sharedGlWidget
  );

  quicklookView->setMinimumSize(  64,  64 );
  quicklookView->setMaximumSize( 512, 512 );
  quicklookView->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );

  return quicklookView;
}

/*****************************************************************************/
void
MainWindow
::ImportImage( const QString& filename, bool forceCreate )
{
  DatasetModel* datasetModel =
    ImportDataset(
      filename,
#if 0 // USE_OLD_IMAGE_VIEW
      // Seen but left here intentionaly.
      m_ImageView1->width(),
      m_ImageView1->height(),
#else
      -1, -1,
#endif // USE_OLD_IMAGE_VIEW
      forceCreate
    );

  if( datasetModel==NULL )
    return;

  assert( Application::Instance() );
  assert( Application::Instance()->GetModel()!=NULL );
  assert(
    Application::Instance()->GetModel()==
    Application::Instance()->GetModel< DatabaseModel >()
  );

  DatabaseModel* databaseModel =
    Application::Instance()->GetModel< DatabaseModel >();
  assert( databaseModel!=NULL );

  DatasetHash hash( 
    databaseModel->RegisterDatasetModel(
      datasetModel
    )
  );

  qDebug() << "hash:" << hash;

  databaseModel->SelectDatasetModel( hash );
}

/*****************************************************************************/
void
MainWindow
::closeEvent( QCloseEvent* event )
{
  assert( Application::Instance() );
  assert(
    Application::Instance()->GetModel()==
    Application::Instance()->GetModel< DatabaseModel >()
  );

  DatabaseModel* databaseModel =
    Application::Instance()->GetModel< DatabaseModel >();

  DatasetModel* datasetModel = databaseModel->GetSelectedDatasetModel();

  if( datasetModel!=NULL )
    datasetModel->Save();

  I18nMainWindow::closeEvent( event );
}

/*****************************************************************************/
/* SLOTS                                                                     */

/*****************************************************************************/
void
MainWindow
::on_action_ImportImage_triggered()
{
  //
  // Select filename.

  QString filename(
    I18nMainWindow::GetOpenFileName( this, tr( "Open file..." ) )
  );

  if( filename.isNull() )
    return;

  //
  // Import the image
  ImportImage( filename, true );
}

/*****************************************************************************/
void
MainWindow
::on_action_Preferences_triggered()
{
  PreferencesDialog prefDialog( this );

  prefDialog.exec();
}

/*****************************************************************************/
void
MainWindow
::on_action_ZoomIn_triggered()
{
  emit UserZoomIn();
}

/*****************************************************************************/
void
MainWindow
::on_action_ZoomOut_triggered()
{
  emit UserZoomOut();
}

/*****************************************************************************/
void
MainWindow
::on_action_ZoomExtent_triggered()
{
  emit UserZoomExtent();
}

/*****************************************************************************/
void
MainWindow
::on_action_ZoomFull_triggered()
{
  emit UserZoomFull();
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeModel( const AbstractModel* model )
{
  // qDebug() << this << "::OnAboutToChangeModel(" << model << ")";

  SetControllerModel( m_DatabaseBrowserDock, NULL );

  assert( Application::Instance()!=NULL );

  DatabaseModel* databaseModel =
    Application::Instance()->GetModel< DatabaseModel >();
  // Check that NULL==NULL or (DatabaseModel*)==(AbstractModel*)
  assert( databaseModel==Application::Instance()->GetModel() );

  // Force to disconnect previously selected dataset-model before
  // database-model is disconnected.
  //
  // If there were no previously set database-model, this will cause
  // GUI views to be disabled.
  //
  // N.B.: This will cause UI controllers to disable widgets.
  OnAboutToChangeSelectedDatasetModel( NULL );

  // Exit, if there were no previously set database model.
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
  // qDebug() << this << "::OnModelChanged(" << model << ")";

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

  // Force to connect selected dataset-model after database-model is
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
  // qDebug() << this << "::OnAboutToChangeSelectedDatasetModel(" << model << ")";

  //
  // CONTROLLERS.
  //

  //
  // Unset model from controllers.
  //
  // N.B.: This step must be done *before* disconnecting signals &
  // slots between model(s) and view(s).
  //
  // See also, ::OnSelectedDatasetModel() changed.

  // Unset dataset-model from dataset-properties controller.
  SetControllerModel( m_DatasetPropertiesDock, NULL );

  // Unset image-model from color-dynamics controller.
  SetControllerModel( m_ColorDynamicsDock, NULL );

  // Unset image-model from color-setup controller.
  SetControllerModel( m_ColorSetupDock, NULL );

  // Unset histogram-model from histogram controller.
  SetControllerModel( m_HistogramDock, NULL );

  //
  // VIEWS.
  //

  // Update image-view.
  m_ImageView->SetImageList(
    ImageViewWidget::VectorImageModelList(),
    ImageViewWidget::ZOOM_TYPE_FULL
  );

  // Update quicklook-view.
  assert( GetQuicklookView()!=NULL );
  GetQuicklookView()->SetImageList(
    ImageViewWidget::VectorImageModelList(),
    ImageViewWidget::ZOOM_TYPE_FULL
  );

  //
  // MODEL(s).
  //

  // Access database-model.
  DatabaseModel* databaseModel =
    Application::Instance()->GetModel< DatabaseModel >();


  // Do not continue if application does not have a databaseModel set.
  if( databaseModel==NULL )
    return;

  // Access previously selected dataset-model.
  DatasetModel* datasetModel =
    databaseModel->GetSelectedDatasetModel();

  // If there were no previously selected dataset-model, return.
  if( datasetModel==NULL )
    return;

  // Save dataset-model if modified.
  /*
  if( datasetModel->IsModified() &&
      QMessageBox::question(
	this,
	tr( PROJECT_NAME ),
	tr( "Dataset has been modified.\n"
	    "Do you want to save settings?" ),
	QMessageBox::Yes |
	QMessageBox::No,
	QMessageBox::Yes )==QMessageBox::Yes )
  */
  datasetModel->Save();

  // Access previously selected vector-image model.
  const VectorImageModel* vectorImageModel =
    datasetModel->GetSelectedImageModel< VectorImageModel >();

  // Check type.
  assert( vectorImageModel==datasetModel->GetSelectedImageModel() );

  // Connect newly selected model to view (after all other widgets are
  // connected to prevent signals/slots to produce multiple view
  // refreshes).
  if( vectorImageModel!=NULL )  // could be null when no dataset selected
    {
    // Disconnect previously selected image-model from view.
    QObject::disconnect(
      vectorImageModel,
      SIGNAL( SettingsUpdated() ),
      // from:
      m_ImageView,
      SLOT( updateGL()  )
      );

    // Disconnect previously selected quicklook-model from view.
    // TODO: Remove quicklook temporary hack by better design.
    QObject::disconnect(
      vectorImageModel,
      SIGNAL( SettingsUpdated() ),
      // from:
      m_QuicklookViewDock->widget(),
      SLOT( updateGL()  )
      );
    }

  QObject::disconnect(
    m_ImageView,
    SIGNAL( RoiChanged( const PointType&, double, double ) ),
    // to:
    datasetModel,
    SLOT( OnRoiChanged( const PointType&, double, double ) )
  );

  // Disconnect the signals from the previous dataset model
  DisconnectStatusBar( datasetModel );

  // Disconnect the 
  DisconnectPixelDescriptionWidget( datasetModel );
}

/*****************************************************************************/
void
MainWindow
::OnSelectedDatasetModelChanged( DatasetModel* model )
{
  // qDebug() << this << "::OnSelectedDatasetModelChanged(" << model << ")";

  // Access vector-image model.
  VectorImageModel* vectorImageModel =
    model->GetSelectedImageModel< VectorImageModel >();

  // Check type.
  assert( vectorImageModel==model->GetSelectedImageModel() );

  //
  // VIEWS.
  //

  // connect the status bar
  // must be done before the SetImageModel (to be able to receive
  // the signal with zoom level)
  ConnectStatusBar( model );

  ConnectPixelDescriptionWidget( model );

  if( model!=NULL )
    {
    QObject::connect(
      m_ImageView,
      SIGNAL( RoiChanged( const PointType&, double, double ) ),
      // to:
      model,
      SLOT( OnRoiChanged( const PointType&, double, double ) )
    );
    }

  if( vectorImageModel!=NULL )
    {
    //
    // Image-models.
    ImageViewWidget::VectorImageModelList images;

    images << vectorImageModel;

    // m_ImageView->SetImageList( images, ImageViewWidget::ZOOM_TYPE_FULL );
    m_ImageView->SetImageList(
      images,
      model->GetLastPhysicalCenter(),
      model->GetLastIsotropicZoom()
    );

    //
    // Quicklooks.
    ImageViewWidget* quicklookView = GetQuicklookView();
    assert( quicklookView!=NULL );

#if 0
    // Use full image for quicklook-view.
    quicklookView->SetImageList( images, ImageViewWidget::ZOOM_TYPE_EXTENT );
#else
    // Use quicklook-image for quicklook-view.
    ImageViewWidget::VectorImageModelList quicklooks;

    assert( vectorImageModel->GetQuicklookModel()!=NULL );
    quicklooks << vectorImageModel->GetQuicklookModel();

    // qDebug() << quicklooks;

    quicklookView->SetImageList( quicklooks, ImageViewWidget::ZOOM_TYPE_EXTENT );
#endif
    }

  //
  // MODEL(s).
  //

  // connected to prevent signals/slots to produce multiple view
  // refreshes).
  if( vectorImageModel!=NULL )  // could be null when no dataset selected
    {
    //
    // SAT: Using m_TabWidget->index( 0 ) or m_ImageView is equivalent
    // since Qt may use signal & slot names to connect (see MOC .cxx
    // files). Thus, using m_ImageView saves one indirection call.
    QObject::connect(
      vectorImageModel,
      SIGNAL( SettingsUpdated() ),
      // to:
      m_ImageView,
      SLOT( updateGL()  )
      );

    QObject::connect(
      vectorImageModel,
      SIGNAL( SettingsUpdated() ),
      // to:
      m_QuicklookViewDock->widget(),
      SLOT( updateGL()  )
      );


    //
    // trigger the placename computation
    // used by m_DatasetPropertiesDock
    // Done here to find placename if not computed, everytime this
    // dataset is loaded
    model->LoadImagePlacename();
    }

  //
  // CONTROLLERS.
  //

  //
  // Connect image-model controllers.
  //
  // N.B.: This step *must* be done after signals and slots between
  // model(s) and view(s) have been connected (because when model is
  // assigned to controller, widgets/view are reset and emit
  // refreshing signals).
  //
  // See also: OnAboutToChangeDatasetModel().

  // Assign dataset-model to dataset-properties controller.
  SetControllerModel( m_DatasetPropertiesDock, model );

  // Assign image-model to color-dynamics controller.
  SetControllerModel( m_ColorDynamicsDock, vectorImageModel );

  // Assign image-model to color-setup controller.
  SetControllerModel( m_ColorSetupDock, vectorImageModel );

  // Assign histogram-model to histogram controller.
  SetControllerModel( m_HistogramDock, vectorImageModel );

  //
  // TOOLBAR.
  //
  m_UI->action_ZoomIn->setEnabled( vectorImageModel!=NULL );
  m_UI->action_ZoomOut->setEnabled( vectorImageModel!=NULL );
  m_UI->action_ZoomExtent->setEnabled( vectorImageModel!=NULL );
  m_UI->action_ZoomFull->setEnabled( vectorImageModel!=NULL );
}

/*****************************************************************************/
void
MainWindow
::OnApplicationToLaunchSelected( const QString& appName,
				 const QString& docName)
{
#ifdef OTB_WRAP_QT

  assert( Application::ConstInstance()!=NULL );
  assert( Application::ConstInstance()->GetOTBApplicationsModel()!=NULL );
  assert(
    Application::ConstInstance()->GetOTBApplicationsModel()->GetLauncher()!=NULL
  );

  Wrapper::QtWidgetView* appWidget =
    Application::ConstInstance()
    ->GetOTBApplicationsModel()
    ->GetLauncher()
    ->NewOtbApplicationWidget( appName );

  assert( appWidget!=NULL );

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
::OnTabCloseRequested( int index )
{
  assert( index >= 1 );

  QWidget* appWidget = m_CentralTabWidget->widget( index );
  assert( appWidget!=NULL );

#ifdef OTB_WRAP_QT

  assert( appWidget==qobject_cast< Wrapper::QtWidgetView* >( appWidget ) );
  Wrapper::QtWidgetView* appWidgetView =
    qobject_cast< Wrapper::QtWidgetView* >( appWidget );
  assert( appWidgetView!=NULL );

  if( !appWidgetView->IsClosable() )
    {
    QMessageBox::warning(
      this,
      tr( "Monteverdi2 - Warning!" ),
      tr( "Tab cannot be closed while OTB application is running." )
    );
   
    return;
    }

#endif

  m_CentralTabWidget->removeTab( index );

  delete appWidget;
  appWidget = NULL;
}

/*****************************************************************************/
void
MainWindow
::OnOTBApplicationOutputImageChanged( const QString& appName,
				      const QString & outfname )
{
  //
  // If this slot is called, it means that an application has finished
  // its process and has an output image parameter. The 'outfname' in
  // parameters is the output filename. This slot may launch or not,
  // depending on the app settings, the import of the 'outfname' to the
  // catalog database.

  // import the result image into the database
  ImportImage( outfname, true );
}

/*****************************************************************************/
void
MainWindow
::OnExecutionDone( int status )
{
  if( status<0 )
    return;

  assert( m_DatabaseBrowserDock!=NULL );
  assert(
    m_DatabaseBrowserDock->findChild< const DatabaseBrowserController* >()!=NULL
  );

  m_DatabaseBrowserDock
    ->findChild< DatabaseBrowserController* >()
    ->CheckDatasetsConsistency();
}

/*****************************************************************************/
void
MainWindow
::OnFilenameDropped( const QString& filename )
{
  ImportImage( filename, true );
}

} // end namespace 'mvd'
