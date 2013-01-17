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
    this, SIGNAL(largestPossibleRegionChanged(const ImageRegionType&)),
    centralWidget(), SLOT( onLargestPossibleRegionChanged(const ImageRegionType&)) );

  // Connect Appllication and MainWindow when selected model is about
  // to change.
  QObject::connect(
    qApp, SIGNAL( aboutToChangeSelectedModel( const AbstractModel* ) ),
    this, SLOT( onAboutToChangeSelectedModel( const AbstractModel* ) )
  );

  // Connect Appllication and MainWindow when selected model has been
  // changed.
  QObject::connect(
    qApp, SIGNAL( selectedModelChanged( const AbstractModel* ) ),
    this, SLOT( onSelectedModelChanged( const AbstractModel* ) )
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

  /*
  qDebug() << dockWidget;
  qDebug() << widget;
  qDebug() << widget->parent();
  qDebug();
  */

  // Features.
  dockWidget->setFloating( true );
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
 
  // TODO: Replace with complex model (list of DatasetModel) when implemented.
  VectorImageModel* model = new VectorImageModel();

  model->setObjectName(
    "mvd::VectorImageModel('" + filename + "')"
  );

  // load file
  try
    {
    model->loadFile( filename );

    qobject_cast< Application* >( qApp )->SetModel( model );
    }
  catch( std::exception& exc )
    {
    delete model;
    model = NULL;

    QMessageBox::warning( this, tr("Exception!"), exc.what() );
    return;
    }
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
::onAboutToChangeSelectedModel( const AbstractModel* )
{
  Application* app = qobject_cast< Application* >( qApp );

  const VectorImageModel* vectorImageModel =
    qobject_cast< const VectorImageModel* >( app->GetModel() );

  if( vectorImageModel==NULL )
    {
    return;
    }

  // Disconnect previously selected model from view.
  QObject::disconnect(
    vectorImageModel,
    SIGNAL( settingsUpdated() ),
    // to:
    centralWidget(),
    SLOT( updateGL() )
  );


  QWidget* widget = GetColorSetupDock()->widget();

  if( widget==NULL )
    {
    return;
    }

  // Disconnect previously selected model from UI controller.
  QObject::disconnect(
    GetColorSetupDock()->widget(),
    SIGNAL( currentIndexChanged( ColorSetupWidget::Channel, int ) ),
    // from:
    vectorImageModel,
    SLOT( onCurrentIndexChanged( ColorSetupWidget::Channel, int ) )
  );
}

/*****************************************************************************/
void
MainWindow
::onSelectedModelChanged( const AbstractModel* model )
{
  ColorSetupWidget* colorSetupWidget =
    qobject_cast< ColorSetupWidget*  >( GetColorSetupDock()->widget() );

  const VectorImageModel* vectorImageModel =
    qobject_cast< const VectorImageModel* >( model );

  // Connect newly selected model to UI controller.
  QObject::connect(
    colorSetupWidget,
    SIGNAL( currentIndexChanged( ColorSetupWidget::Channel, int ) ),
    // to:
    vectorImageModel,
    SLOT( onCurrentIndexChanged( ColorSetupWidget::Channel, int ) )
  );

  // Connect newly selected model to view.
  QObject::connect(
    vectorImageModel,
    SIGNAL( settingsUpdated() ),
    // to:
    centralWidget(),
    SLOT( updateGL()  )
  );

  // Setup color-setup controller.
  colorSetupWidget->blockSignals( true );
  {
  // qDebug() << vectorImageModel->GetBandNames();

  colorSetupWidget->SetComponents( vectorImageModel->GetBandNames() );

  for( int i=0; i<ColorSetupWidget::CHANNEL_COUNT; ++i )
    {
    colorSetupWidget->SetCurrentIndex(
      ColorSetupWidget::Channel( i ),
      vectorImageModel->GetSettings().RgbChannel( i )
    );
    }
  }
  colorSetupWidget->blockSignals( false );

  // set the largest possible region of the image
  // TODO:  rename signal name when handling DataSets collections
  // TODO: move signal into mvdApplication and link it to DockWidget
  // and ImageView.
  emit largestPossibleRegionChanged(
    vectorImageModel->GetOutput( 0 )->GetLargestPossibleRegion()
  );
}

/*****************************************************************************/

} // end namespace 'mvd'
