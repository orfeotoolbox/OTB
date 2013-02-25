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
#include "mvdColorDynamicsController.h"
#include "mvdColorDynamicsWidget.h"
#include "mvdColorSetupController.h"
#include "mvdDatasetModel.h"
#include "mvdGLImageWidget.h"
#include "mvdImageModelRenderer.h"
#include "mvdImageViewManipulator.h"
#include "mvdQuicklookModel.h"
#include "mvdQuicklookViewManipulator.h"
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

  // instanciate the manipulator and the renderer relative to this widget
  ImageViewManipulator* imageViewManipulator = new ImageViewManipulator();
  ImageModelRenderer* imageModelRenderer = new ImageModelRenderer();

  // set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget(
    new GLImageWidget(
      imageViewManipulator,
      imageModelRenderer,
      this
    )
  );

  // instanciate the Ql manipulator/renderer here to be able to connect
  // its signals to the centralWidget manipulator slots
  m_QLModelRenderer   = new ImageModelRenderer();
  m_QLViewManipulator = new QuicklookViewManipulator();

  // Connect centralWidget manipulator to Ql renderer when viewportRegionChanged
  QObject::connect(
    imageViewManipulator, SIGNAL( ViewportRegionRepresentationChanged(const ImageRegionType&) ), 
    m_QLModelRenderer, SLOT( OnViewportRegionRepresentationChanged(const ImageRegionType&) )
    );

  // Connect ql mousePressEventpressed to centralWidget manipulator
  QObject::connect(
    m_QLViewManipulator, SIGNAL( ViewportRegionChanged(double, double) ), 
    imageViewManipulator, SLOT( OnViewportRegionChanged(double, double) )
  );

  // add the needed docks 
  InitializeDockWidgets();

  // Connect Quit action of main menu to QApplication's quit() slot.
  QObject::connect(
    m_UI->action_Quit, SIGNAL( activated() ),
    qApp, SLOT( quit() )
  );

  // Connect Appllication and MainWindow when selected model is about
  // to change.
  QObject::connect(
    qApp, SIGNAL( AboutToChangeSelectedModel( const AbstractModel* ) ),
    this, SLOT( OnAboutToChangeSelectedModel( const AbstractModel* ) )
  );

  // Connect Appllication and MainWindow when selected model has been
  // changed.
  QObject::connect(
    qApp, SIGNAL( SelectedModelChanged( AbstractModel* ) ),
    this, SLOT( OnSelectedModelChanged( AbstractModel* ) )
  );

  // Change to NULL model to force emitting GUI signals when GUI is
  // instanciated. So, GUI will be initialized and controller-widgets
  // disabled.
  Application::Instance()->SetModel( NULL );
}

/*****************************************************************************/
void
MainWindow
::InitializeDockWidgets()
{
  //
  // EXPERIMENTAL QUICKLOOK Widget.
  assert( qobject_cast< GLImageWidget* >( centralWidget() )!=NULL );
  GLImageWidget* qlWidget = new GLImageWidget(
    m_QLViewManipulator,
    m_QLModelRenderer,
    this,
    qobject_cast< GLImageWidget* >( centralWidget() )
  );
  // TODO: Set better minimum size for quicklook GL widget.
  qlWidget->setMinimumSize(100,100);

  AddWidgetToDock( 
    qlWidget,
    QUICKLOOK_DOCK,
    tr( "Quicklook" ),
    Qt::LeftDockWidgetArea
    );

  //
  // COLOR SETUP.
  ColorSetupWidget* colorSetupWgt = new ColorSetupWidget( this );

  ColorSetupController* colorSetupCtrl = new ColorSetupController(
    // wraps:
    colorSetupWgt,
    // as child of:
    AddWidgetToDock(
      colorSetupWgt,
      VIDEO_COLOR_SETUP_DOCK,
      tr( "Video color setup" ),
      Qt::LeftDockWidgetArea
    )
  );

  //
  // COLOR DYNAMICS.
  ColorDynamicsWidget* colorDynWgt = new ColorDynamicsWidget( this );

  // Controller is childed to dock.
  ColorDynamicsController* colorDynamicsCtrl = new ColorDynamicsController(
    // wraps:
    colorDynWgt,
    // as child of:
    AddWidgetToDock(
      colorDynWgt,
      VIDEO_COLOR_DYNAMICS_DOCK,
      tr( "Video color dynamics" ),
      Qt::LeftDockWidgetArea
    )
  );

  //
  // CHAIN CONTROLLERS.
  // Forward model update signals of color-setup controller...
  QObject::connect(
    colorSetupCtrl,
    SIGNAL( RgbChannelIndexChanged( RgbaChannel, int ) ),
    // to: ...color-dynamics controller model update signal.
    colorDynamicsCtrl,
    SLOT( OnRgbChannelIndexChanged( RgbaChannel, int ) )
  );

  //
  // EXPERIMENTAL TOOLBOX.

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
QDockWidget*
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

  return dockWidget;
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
 
  try
    {
    // TODO: Replace with complex model (list of DatasetModel) when implemented.
    DatasetModel* model = Application::LoadDatasetModel(
      filename,
      // TODO: Remove width and height from dataset model loading.
      centralWidget()->width(), centralWidget()->height()
    );

    Application::Instance()->SetModel( model );
    }
  catch( std::exception& exc )
    {
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
::OnAboutToChangeSelectedModel( const AbstractModel* )
{
  //
  // COLOR SETUP.
  SetControllerModel( GetColorSetupDock(), NULL );

  //
  // COLOR DYNAMICS.
  SetControllerModel( GetColorDynamicsDock(), NULL );

  // De-assign models to view after controllers (LIFO disconnect).
  qobject_cast< GLImageWidget *>( centralWidget() )->SetImageModel( NULL );
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->SetImageModel(
    NULL
  );

  //
  //
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

  // TODO : where to do this
  QObject::disconnect(
    // vectorImageModel->GetQuicklookModel(),
    // TODO: Remove temporary hack by better design.
    vectorImageModel,
    SIGNAL( SettingsUpdated() ),
    // to:
    GetQuicklookDock()->widget(),
    SLOT( updateGL()  )
  );
}

/*****************************************************************************/
void
MainWindow
::OnSelectedModelChanged( AbstractModel* model )
{
  if( model==NULL )
    return;

  DatasetModel* datasetModel = qobject_cast< DatasetModel* >( model );

  assert( datasetModel!=NULL );
  assert( datasetModel->HasSelectedImageModel() );

  VectorImageModel* vectorImageModel =
    qobject_cast< VectorImageModel* >(
      datasetModel->GetSelectedImageModel()
    );

  assert( vectorImageModel!=NULL );


  //
  // COLOR SETUP.
  SetControllerModel( GetColorSetupDock(), vectorImageModel );

  //
  // COLOR DYNAMICS.
  SetControllerModel( GetColorDynamicsDock(), vectorImageModel );

  //
  // MAIN VIEW.

  // Assign newly selected model to view.
  qobject_cast< GLImageWidget *>( centralWidget() )->SetImageModel(
    vectorImageModel
  );

  // Connect newly selected model to view (after all other widgets are
  // connected to prevent signals/slots to produce multiple view
  // refreshes).
  QObject::connect(
    vectorImageModel,
    SIGNAL( SettingsUpdated() ),
    // to:
    centralWidget(),
    SLOT( updateGL()  )
  );

  //
  // QUICKLOOK VIEW.

  // Assign newly selected model to view.
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->SetImageModel(
    vectorImageModel->GetQuicklookModel()
  );

  // Connect newly selected model to view (after all other widgets are
  // connected to prevent signals/slots to produce multiple view
  // refreshes).
  // TODO : find where to do this
  QObject::connect(
    // vectorImageModel->GetQuicklookModel(),
    // TODO: Remove temporary hack by better design.
    vectorImageModel,
    SIGNAL( SettingsUpdated() ),
    // to:
    GetQuicklookDock()->widget(),
    SLOT( updateGL()  )
  );

  //
  // connect the vectorimage model spacing change (when zooming)
  QObject::connect(
    vectorImageModel,
    SIGNAL( SpacingChanged(const SpacingType&) ),
    // to:
    centralWidget(),
    SLOT( OnSpacingChanged(const SpacingType&)  )
  );

  // TODO : OLU uncomment those connects when ql and central view 
  //        are well connected
  // // TODO : where to do this
  // QObject::connect(
  //   // vectorImageModel->GetQuicklookModel(),
  //   // TODO: Remove temporary hack by better design.
  //   centralWidget(),
  //   SIGNAL( movingMouse() ),
  //   // to:
  //   GetQuicklookDock()->widget(),
  //   SLOT( updateGL()  )
  // );
  
  // // TODO : where to do this
  // QObject::connect(
  //   // vectorImageModel->GetQuicklookModel(),
  //   // TODO: Remove temporary hack by better design.
  //   centralWidget(),
  //   SIGNAL( releasingMouse() ),
  //   // to:
  //   GetQuicklookDock()->widget(),
  //   SLOT( updateGL()  )
  // );

  /*
  // Connect newly selected model to UI controller.
  QObject::connect(
    GetColorSetupDock()->widget(),
    SIGNAL( CurrentIndexChanged( RgbaChannel, int ) ),
    // to:
    vectorImageModel->GetQuicklookModel(),
    SLOT( OnCurrentIndexChanged( RgbaChannel, int ) )
  );
  */
}

/*****************************************************************************/

} // end namespace 'mvd'
