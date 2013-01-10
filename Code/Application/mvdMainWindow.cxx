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
MainWindow::InitializeDockWidgets()
{
  // New dock.
  QDockWidget* videoColorSetupDock =
    new QDockWidget( tr( "Video color setup" ), this );

  // Setup dock.
  // You can use findChild( "videoColorSetupDock" ) to get dock-widget.
  videoColorSetupDock->setObjectName( VIDEO_COLOR_SETUP_DOCK );
  videoColorSetupDock->setWidget(
    new ColorSetupWidget( videoColorSetupDock )
  );
  videoColorSetupDock->setFloating( true );
  videoColorSetupDock->setFeatures(
    QDockWidget::DockWidgetMovable |
    QDockWidget::DockWidgetFloatable
  );

  // Update OpenGL view when color-setup has changed.
  QObject::connect(
    videoColorSetupDock->widget(),
    SIGNAL( currentIndexChanged( Channel, int  ) ),
    // to:
    centralWidget(),
    SLOT( updateGL()  )
  );

  // Add dock.
  addDockWidget( Qt::LeftDockWidgetArea, videoColorSetupDock );
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

    dynamic_cast< Application* >( qApp )->SetModel( model );

    // set the largest possible region of the image
    // TODO:  rename signal name when handling DataSets collections
    // TODO: move signal into mvdApplication and link it to DockWidget and ImageView.
    emit largestPossibleRegionChanged(model->GetOutput(0)->GetLargestPossibleRegion());
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
  Application* app = dynamic_cast< Application* >( qApp );

  const VectorImageModel* vectorImageModel =
    dynamic_cast< const VectorImageModel* >( app->GetModel() );

  QWidget* widget = GetVideoColorSetupDock()->widget();

  if( vectorImageModel==NULL ||
      widget==NULL )
    {
    return;
    }

  // Disconnect previously selected model from UI controller.
  QObject::disconnect(
    GetVideoColorSetupDock()->widget(),
    SIGNAL( currentIndexChanged( ColorSetupWidget::Channel, int ) ),
    // to:
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
    qobject_cast< ColorSetupWidget*  >( GetVideoColorSetupDock()->widget() );

  const VectorImageModel* vectorImageModel =
    dynamic_cast< const VectorImageModel* >( model );

  colorSetupWidget->SetComponents( vectorImageModel->GetBandNames() );

  // Connect newly selected model to UI controller.
  QObject::connect(
    GetVideoColorSetupDock()->widget(),
    SIGNAL( currentIndexChanged( ColorSetupWidget::Channel, int ) ),
    // to:
    vectorImageModel,
    SLOT( onCurrentIndexChanged( ColorSetupWidget::Channel, int ) )
  );

  for( int i=0; i<ColorSetupWidget::CHANNEL_COUNT; ++i )
    {
    colorSetupWidget->SetCurrentIndex(
      ColorSetupWidget::Channel( i ),
      vectorImageModel->GetSettings().m_RGBChannels[ i ]
    );
    }
}

/*****************************************************************************/

} // end namespace 'mvd'
