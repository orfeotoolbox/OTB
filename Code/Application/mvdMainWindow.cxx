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
#include "mvdAboutDialog.h"
#include "mvdApplication.h"
#include "mvdColorDynamicsWidget.h"
#include "mvdColorSetupWidget.h"
#include "mvdDatasetModel.h"
#include "mvdGLImageWidget.h"
#include "mvdVectorImageModel.h"


namespace mvd
{
/*
  TRANSLATOR mvd::MainWindow

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
MainWindow
::MainWindow( QWidget* parent, Qt::WindowFlags flags ) :
  QMainWindow( parent, flags ), 
  m_UI( new mvd::Ui::MainWindow() )
{
  m_UI->setupUi( this );

  Initialize();
}

/*****************************************************************************/
MainWindow
::~MainWindow()
{
}

/*****************************************************************************/
void
MainWindow
::Initialize()
{
  setObjectName( "mvd::MainWindow" );
  setWindowTitle( PROJECT_NAME );

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( new GLImageWidget( this ) );

  InitializeDockWidgets();

  // Connect Quit action of main menu to QApplication's quit() slot.
  QObject::connect(
    m_UI->action_Quit, SIGNAL( activated() ),
    qApp, SLOT( quit() )
  );

  // Connect the setLargestPossibleregion
  QObject::connect(
    this, SIGNAL( LargestPossibleRegionChanged(const ImageRegionType&) ),
    centralWidget(), SLOT( OnLargestPossibleRegionChanged(const ImageRegionType&)) );

  // Connect Appllication and MainWindow when selected model is about
  // to change.
  QObject::connect(
    qApp, SIGNAL( AboutToChangeSelectedModel( const AbstractModel* ) ),
    this, SLOT( OnAboutToChangeSelectedModel( const AbstractModel* ) )
  );

  // Connect Appllication and MainWindow when selected model has been
  // changed.
  QObject::connect(
    qApp, SIGNAL( SelectedModelChanged( const AbstractModel* ) ),
    this, SLOT( OnSelectedModelChanged( const AbstractModel* ) )
  );
}

/*****************************************************************************/
void
MainWindow
::InitializeDockWidgets()
{
  AddWidgetToDock( 
    new ColorSetupWidget( this ),
    VIDEO_COLOR_SETUP_DOCK,
    tr( "Video color setup" ),
    Qt::LeftDockWidgetArea
  );

  AddWidgetToDock( 
    new ColorDynamicsWidget( this ),
    VIDEO_COLOR_DYNAMICS_DOCK,
    tr( "Video color dynamics" ),
    Qt::LeftDockWidgetArea
  );

#if 0

  QToolBox* toolBox = new QToolBox( this );

  toolBox->setObjectName( "mvd::VideoColorToolBox" );

  toolBox->addItem( new ColorSetupWidget( toolBox ), tr( "Video color setup" ) );
  toolBox->addItem( new ColorDynamicsWidget( toolBox ), tr( "Video color dynamics" ) );

  AddWidgetToDock( 
    toolBox,
    "videoColorSettingsDock",
    tr( "Video color dynamics" ),
    Qt::LeftDockWidgetArea
  );
#endif
}

/*****************************************************************************/
void
MainWindow
::AddWidgetToDock( QWidget* widget,
		   const QString& dockName,
		   const QString& dockTitle,
		   Qt::DockWidgetArea dockArea )
{
  // New dock.
  QDockWidget* dockWidget = new QDockWidget( dockTitle, this );

  // You can use findChild( dockName ) to get dock-widget.
  dockWidget->setObjectName( dockName );
  dockWidget->setWidget( widget );

  // Features.
  dockWidget->setFloating( false );
  dockWidget->setFeatures(
    QDockWidget::DockWidgetMovable |
    QDockWidget::DockWidgetFloatable
  );

  // Add dock.
  addDockWidget( dockArea, dockWidget );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
MainWindow
::on_action_Open_activated()
{
  QString filename(
    QFileDialog::getOpenFileName( this, tr( "Open file..." ) )
  );

  if( filename.isNull() )
    {
    return;
    }
 
  // TODO: Replace with complex model (list of DatasetModel) when
  // implemented.
  DatasetModel* model = new DatasetModel();

  model->setObjectName( "mvd::DatasetModel('" + filename + "'" );

  try
    {
    model->ImportImage( filename );
    }
  catch( std::exception& exc )
    {
    delete model;
    model = NULL;

    QMessageBox::warning( this, tr("Exception!"), exc.what() );
    return;
    }

  Application::Instance()->SetModel( model );
}

/*****************************************************************************/
void
MainWindow
::on_action_About_activated()
{
  AboutDialog aboutDialog( this );

  aboutDialog.exec();
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeSelectedModel( const AbstractModel* )
{
  const Application* app = Application::ConstInstance();
  assert( app!=NULL );

  const DatasetModel* datasetModel = 
    qobject_cast< const DatasetModel* >( app->GetModel() );

  // It is Ok there is no previously selected model (e.g. at
  // application startup.
  if( datasetModel==NULL )
    {
    return;
    }

  assert( datasetModel->HasSelectedImageModel() );

  const VectorImageModel* vectorImageModel =
    qobject_cast< const VectorImageModel* >(
      datasetModel->GetSelectedImageModel()
    );

  assert( vectorImageModel!=NULL );

  //
  // MAIN VIEW.

  // Disconnect previously selected model from view.
  QObject::disconnect(
    vectorImageModel,
    SIGNAL( SettingsUpdated() ),
    // to:
    centralWidget(),
    SLOT( updateGL() )
  );

  //
  // COLOR SETUP.

  // Disconnect previously selected model from UI controller.
  QObject::disconnect(
    GetColorSetupDock()->widget(),
    SIGNAL( CurrentIndexChanged( RgbaChannel, int ) ),
    // from:
    vectorImageModel,
    SLOT( OnCurrentIndexChanged( RgbaChannel, int ) )
  );

  //
  // COLOR DYNAMICS.

  QWidget* colorDynamicsWidget = GetColorDynamicsDock()->widget();
  assert( colorDynamicsWidget!=NULL );

  // Disconnect previously selected model to UI controller.
  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowQuantileChanged( RgbaChannel, double ) ),
    // from:
    vectorImageModel,
    SLOT( OnLowQuantileChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbaChannel, double ) ),
    // from:
    vectorImageModel,
    SLOT( OnHighQuantileChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbaChannel, double ) ),
    // from:
    vectorImageModel,
    SLOT( OnLowIntensityChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbaChannel, double ) ),
    // from:
    vectorImageModel,
    SLOT( OnHighIntensityChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbaChannel ) ),
    // from:
    vectorImageModel,
    SLOT( OnResetQuantileClicked( RgbaChannel ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbaChannel ) ),
    // from:
    vectorImageModel,
    SLOT( OnResetIntensityClicked( RgbaChannel  ) )
  );
}

/*****************************************************************************/
void
MainWindow
::OnSelectedModelChanged( const AbstractModel* model )
{
  const DatasetModel* datasetModel =
    qobject_cast< const DatasetModel* >( model );

  assert( datasetModel!=NULL );
  assert( datasetModel->HasSelectedImageModel() );

  const VectorImageModel* vectorImageModel =
    qobject_cast< const VectorImageModel* >(
      datasetModel->GetSelectedImageModel()
    );

  assert( vectorImageModel!=NULL );

  //
  // MAIN VIEW.

  // Connect newly selected model to view.
  QObject::connect(
    vectorImageModel,
    SIGNAL( SettingsUpdated() ),
    // to:
    centralWidget(),
    SLOT( updateGL()  )
  );

  //
  // COLOR SETUP.

  ColorSetupWidget* colorSetupWidget =
    qobject_cast< ColorSetupWidget*  >( GetColorSetupDock()->widget() );
  assert( colorSetupWidget!=NULL );

  // Connect newly selected model to UI controller.
  QObject::connect(
    colorSetupWidget,
    SIGNAL( CurrentIndexChanged( RgbaChannel, int ) ),
    // to:
    vectorImageModel,
    SLOT( OnCurrentIndexChanged( RgbaChannel, int ) )
  );

  // Setup color-setup controller.
  colorSetupWidget->blockSignals( true );
  {
  colorSetupWidget->SetComponents( vectorImageModel->GetBandNames() );

  for( int i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    colorSetupWidget->SetCurrentIndex(
      static_cast< RgbaChannel >( i ),
      vectorImageModel->GetSettings().RgbChannel( i )
    );
    }
  }
  colorSetupWidget->blockSignals( false );

  //
  // COLOR DYNAMICS.

  ColorDynamicsWidget* colorDynamicsWidget =
    qobject_cast< ColorDynamicsWidget*  >( GetColorDynamicsDock()->widget() );
  assert( colorDynamicsWidget!=NULL );

  // Connect newly selected model to UI controller.
  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LowQuantileChanged( RgbaChannel, double ) ),
    // to:
    vectorImageModel,
    SLOT( OnLowQuantileChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbaChannel, double ) ),
    // to:
    vectorImageModel,
    SLOT( OnHighQuantileChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbaChannel, double ) ),
    // to:
    vectorImageModel,
    SLOT( OnLowIntensityChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbaChannel, double ) ),
    // to:
    vectorImageModel,
    SLOT( OnHighIntensityChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbaChannel ) ),
    // to:
    vectorImageModel,
    SLOT( OnResetQuantileClicked( RgbaChannel ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbaChannel ) ),
    // to:
    vectorImageModel,
    SLOT( OnResetIntensityClicked( RgbaChannel  ) )
  );

  // Setup color-dynamics controller.
  colorDynamicsWidget->blockSignals( true );
  colorDynamicsWidget->blockSignals( false );

  //
  // REFRESH DISPLAY.

  // set the largest possible region of the image
  // TODO:  rename signal name when handling DataSets collections
  // TODO: move signal into mvdApplication and link it to DockWidget
  // and ImageView.
  emit LargestPossibleRegionChanged(
    vectorImageModel->ToImageBase()->GetLargestPossibleRegion()
  );
}

/*****************************************************************************/

} // end namespace 'mvd'
