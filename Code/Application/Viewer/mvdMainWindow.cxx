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
#include "otbSystem.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdImageLoader.h"
#include "mvdPreferencesDialog.h"
//
#include "Core/mvdDatasetModel.h"
#include "Core/mvdQuicklookModel.h"
#include "Core/mvdSystemError.h"
#include "Core/mvdVectorImageModel.h"
//
#include "Gui/mvdAboutDialog.h"
#include "Gui/mvdColorDynamicsController.h"
#include "Gui/mvdColorDynamicsWidget.h"
#include "Gui/mvdColorSetupController.h"
#include "Gui/mvdGLImageWidget.h"
#include "Gui/mvdImageModelRenderer.h"
#include "Gui/mvdImageViewManipulator.h"
#include "Gui/mvdMainWindowTitleLoader.h"
#include "Gui/mvdQuicklookViewManipulator.h"
#include "Gui/mvdStatusBarWidget.h"

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
  m_UI( new mvd::Ui::MainWindow() ),
  m_DatasetCreationProgressDialog( new DatasetCreationProgressDialog(this) )
{
  m_UI->setupUi( this );
  Initialize();
}

/*****************************************************************************/
MainWindow
::~MainWindow()
{
}

void
MainWindow
::OpenImage( QString filename )
{
  emit OpenImageRequest( filename );
}

/*****************************************************************************/
QString
MainWindow
::SelectCacheDir(bool incorrectCacheDir)
{
  QString defaultCacheDirLocStr (QDir::toNativeSeparators(QDir::homePath()));
  QString defaultCacheDirStr (defaultCacheDirLocStr);
  defaultCacheDirStr.append(QDir::separator());
  defaultCacheDirStr.append( I18nApplication::DEFAULT_CACHE_DIR_NAME );
  
  int ret;

  if (incorrectCacheDir)
    {
    QMessageBox::warning( this,
        tr("Warning"),
        tr("This repository seems incorrect to store the cache directory."
           "\nPlease choose another one.") );
    ret = QMessageBox::Yes;
    }
  else
    {
    QMessageBox msgBox;
    msgBox.setWindowTitle( tr("Welcome new Montevedi2 user !" ) );
    msgBox.setText(tr("Monteverdi 2 maintains a repository "
      "to store additional data related to each image "
      "(display settings, histogram, quicklook...).\n\n"
      "Warning:\nYou need to have write access to this directory.\n"
      "Be aware that this directory can grow and consume a significant disk space."));
    msgBox.setInformativeText(tr("Do you want to specify a custom location ?\n"
        "(No will use the default location: %1)").arg(defaultCacheDirStr));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    ret = msgBox.exec();
    }

  QString cacheDirStr;
  if (ret == QMessageBox::Yes)
    {
    // TODO MSD: share the code with the choice of the settings into the preference menu

    QString datasetDir = QFileDialog::getExistingDirectory(this,
         tr("Select the repository to store the cache repository for Monteverdi2"),
         defaultCacheDirStr);
    if (datasetDir.isEmpty())
      { // User push default button => set the value to the default place
      cacheDirStr = defaultCacheDirLocStr;
      }
    else
      { // User select something
      cacheDirStr = datasetDir;
      }
    }
  else
    if (ret == QMessageBox::No)
      { // User push default button => set the value to the default place
      cacheDirStr = defaultCacheDirLocStr;
      }

  return cacheDirStr;
}

/*****************************************************************************/
void
MainWindow
::Initialize()
{
  setObjectName( "mvd::MainWindow" );
  setWindowTitle( PROJECT_NAME );

  // instanciate the manipulator and the renderer relative to this widget
  m_ImageViewManipulator = new ImageViewManipulator();
  m_ImageModelRenderer   = new ImageModelRenderer();

  // set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget(
    new GLImageWidget(
      m_ImageViewManipulator,
      m_ImageModelRenderer,
      this
    )
  );
  
  // Instanciate the Ql manipulator/renderer.
  m_QLModelRenderer   = new ImageModelRenderer();
  m_QLViewManipulator = new QuicklookViewManipulator();

  // Connect centralWidget manipulator to Ql renderer when
  // viewportRegionChanged.
  QObject::connect(
    m_ImageViewManipulator,
    SIGNAL( ViewportRegionRepresentationChanged( const PointType&,
						 const PointType& ) ),
    // to:
    m_QLModelRenderer,
    SLOT( OnViewportRegionRepresentationChanged( const PointType&,
						 const PointType& ) )
    );

  // Connect ql mousePressEventpressed to centralWidget manipulator.
  QObject::connect(
    m_QLViewManipulator, SIGNAL( ViewportRegionChanged( double, double ) ), 
    m_ImageViewManipulator, SLOT( OnViewportRegionChanged( double, double ) )
  );

  // Add the needed docks.
  InitializeDockWidgets();

  // Add needed widget to the status bar.
  InitializeStatusBarWidgets();

  // Connect Appllication and MainWindow when selected model is about
  // to change.
  QObject::connect(
    qApp, SIGNAL( AboutToChangeModel( const AbstractModel* ) ),
    this, SLOT( OnAboutToChangeModel( const AbstractModel* ) )
  );

  // Connect Application and MainWindow when selected model has been
  // changed.
  QObject::connect(
    qApp, SIGNAL( ModelChanged( AbstractModel* ) ),
    this, SLOT( OnModelChanged( AbstractModel* ) )
  );
  
  // Show the progress dialog when a new image is loaded
  QObject::connect(
    this, SIGNAL( OpenImageRequest(QString) ), 
    this, SLOT( OnShowProgressDialog(QString) )
    );
  // Trigger the actual dataset creation when a new image is loaded
  QObject::connect(
    this, SIGNAL( OpenImageRequest(QString) ), 
    this, SLOT( OnOpenImageRequest(QString) )
    );

  // Change to NULL model to force emitting GUI signals when GUI is
  // instanciated. So, GUI will be initialized and controller-widgets
  // disabled.
  Application::Instance()->SetModel( NULL );
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

  QDockWidget* qlDockWgt = AddWidgetToDock(
    qlWidget,
    QUICKLOOK_DOCK,
    tr( "Quicklook" ),
    Qt::LeftDockWidgetArea
    );

  // Quicklook DockWidget's visibilityChanged signal is connected
  QObject::connect(
    qlDockWgt, SIGNAL( visibilityChanged( bool ) ),
    // to:
    this, SLOT( OnQuicklookVisibilityChanged( bool ) )
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

  // Color Setup DockWidget's visibilityChanged signal is connected
  QObject::connect(
    colorSetupCtrl->parent(), SIGNAL( visibilityChanged( bool ) ),
    // to:
    this, SLOT( OnColorSetupVisibilityChanged( bool ) )
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

  // Color Dynamics DockWidget's visibilityChanged signal is connected
  QObject::connect(
    colorDynamicsCtrl->parent(), SIGNAL( visibilityChanged( bool ) ),
    // to:
    this, SLOT( OnColorDynamicsVisibilityChanged( bool ) )
    );

  //
  // CHAIN CONTROLLERS.
  // Forward model update signals of color-setup controller...
  QObject::connect(
    colorSetupCtrl,
    SIGNAL( RgbChannelIndexChanged( RgbwChannel, int ) ),
    // to: ...color-dynamics controller model update signal.
    colorDynamicsCtrl,
    SLOT( OnRgbChannelIndexChanged( RgbwChannel, int ) )
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
    QDockWidget::DockWidgetFloatable |
    QDockWidget::DockWidgetClosable
  );

  // Add dock.
  addDockWidget( dockArea, dockWidget );

  return dockWidget;
}

/*****************************************************************************/
void
MainWindow::closeEvent( QCloseEvent* event )
{
  assert( event!=NULL );

  // Ensure there is an application instance.
  assert( Application::ConstInstance()!=NULL );
  // Ensure that there is no model or that the existing model is a
  // DatasetModel.
  assert( Application::ConstInstance()->GetModel()==
	  Application::ConstInstance()->GetModel< DatasetModel >() );
  // Get model.
  DatasetModel* model =
    Application::Instance()->GetModel< DatasetModel >();

  if( model==NULL || !model->IsModified() )
    return;

  QMessageBox::StandardButton clickedButton = ConfirmSaveQuit( true );

  switch( clickedButton )
    {
    case QMessageBox::Save:
      model->Save();
      break;

    case QMessageBox::Discard:
      break;

    case QMessageBox::Cancel:
      // Ignore event: do not close/quit.
      event->ignore();
      break;

    default:
      // should never be reached.
      assert( false );
      break;
    }
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

  emit OpenImageRequest( filename );
}

/*****************************************************************************/
void
MainWindow
::on_action_Quit_triggered()
{
  close();
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
::on_action_Preferences_activated()
{
  PreferencesDialog prefDialog( this );

  prefDialog.exec();
}

/*****************************************************************************/
void
MainWindow
::on_action_Quicklook_toggled()
{
  GetQuicklookDock()->setVisible( m_UI->action_Quicklook->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::on_action_ColorSetup_toggled()
{
  GetColorSetupDock()->setVisible( m_UI->action_ColorSetup->isChecked() );
}

/*****************************************************************************/
void
MainWindow
::on_action_ColorDynamics_toggled()
{
  GetColorDynamicsDock()->setVisible( m_UI->action_ColorDynamics->isChecked() );
}

/*****************************************************************************/
// The Quicklook DockWidget's visibilityChanged signal is linked to the
// View/Quicklook show/hide checkbox
void
MainWindow
::OnQuicklookVisibilityChanged ( bool visible )
{
  m_UI->action_Quicklook->blockSignals( true );
  {
    m_UI->action_Quicklook->setChecked( visible );
  }
  m_UI->action_Quicklook->blockSignals( false );
}

/*****************************************************************************/
// The Color Setup DockWidget's visibilityChanged signal is linked to the
// View/Color Setup show/hide checkbox
void
MainWindow
::OnColorSetupVisibilityChanged ( bool visible )
{
  m_UI->action_ColorSetup->blockSignals( true );
  {
    m_UI->action_ColorSetup->setChecked( visible );
  }
  m_UI->action_ColorSetup->blockSignals( false );
}

/*****************************************************************************/
// The Color Dynamics DockWidget's visibilityChanged signal is linked to the
// View/Color Dynamics show/hide checkbox
void
MainWindow
::OnColorDynamicsVisibilityChanged ( bool visible )
{
  m_UI->action_ColorDynamics->blockSignals( true );
  {
    m_UI->action_ColorDynamics->setChecked( visible );
  }
  m_UI->action_ColorDynamics->blockSignals( false );
}

/*****************************************************************************/
void
MainWindow
::OnAboutToChangeModel( const AbstractModel* )
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
  const Application* app = I18nApplication::ConstInstance< Application >();
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

  //
  // disconnect the vectorimage model spacing change (when zooming)
  QObject::disconnect(
    vectorImageModel,
    SIGNAL( SpacingChanged(const SpacingType&) ),
    // to:
    centralWidget(),
    SLOT( OnSpacingChanged(const SpacingType&)  )
  );

  // disconnect signal used to update the ql widget
  QObject::disconnect(
    centralWidget(),
    SIGNAL( CentralWidgetUpdated() ),
    // to:
    GetQuicklookDock()->widget(),
    SLOT( updateGL()  )
  );

  // 
  // send the physical point of the clicked point in screen 
  // vectorImageModel is in charge of pixel information computation
    QObject::disconnect(
    m_QLViewManipulator, SIGNAL( PhysicalCursorPositionChanged(double, double) ), 
    vectorImageModel, SLOT( OnPhysicalCursorPositionChanged(double, double) )
    );
  
  QObject::disconnect(
    m_ImageViewManipulator, SIGNAL( PhysicalCursorPositionChanged(double, double) ), 
    vectorImageModel, SLOT( OnPhysicalCursorPositionChanged(double, double) )
    );
  //
  // disconnect the statusBar widget to the vectorImage corresponding
  // signal 
  QObject::disconnect(
    vectorImageModel, 
    SIGNAL( CurrentIndexUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentPixelIndexWidget(),
    SLOT( setText(const QString &) )
  );


  QObject::disconnect(
    vectorImageModel, 
    SIGNAL( CurrentRadioUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentPixelRadioWidget(),
    SLOT( setText(const QString &) )
  );

  QObject::disconnect(
    m_ImageViewManipulator, 
    SIGNAL( CurrentScaleUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentScaleWidget(),
    SLOT( setText(const QString&) )
  );

  // index widget edition
  QObject::disconnect(m_StatusBarWidget->GetCurrentPixelIndexWidget(),
                   SIGNAL( editingFinished() ),
                   this,
                   SLOT( OnUserCoordinatesEditingFinished() )
    );
  
  QObject::disconnect(this,
                   SIGNAL( UserCoordinatesEditingFinished(const QString&) ),
                   vectorImageModel,
                   SLOT( OnUserCoordinatesEditingFinished(const QString&) )
    );

  QObject::disconnect(vectorImageModel,
                   SIGNAL( ViewportRegionChanged(double, double) ),
                   m_ImageViewManipulator,
                   SLOT( OnViewportRegionChanged(double, double) )
    );

  // m_TitleLoader is a QPointer. When destroyed, it is set to 0
  if (m_TitleLoader)
    {
    QObject::disconnect(m_TitleLoader, 
                        SIGNAL(TitleLoaded(const QString &)), 
                        this, 
                        SLOT(setWindowTitle(const QString & ))
      );
    }
}

/*****************************************************************************/
void
MainWindow
::OnModelChanged( AbstractModel* model )
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
  // Title  -----------------------------------------------------------------
  QThread* thread = new QThread;
  m_TitleLoader = new MainWindowTitleLoader(vectorImageModel);
  m_TitleLoader->moveToThread(thread);
  
  // At thread startup, trigger the processing function
  QObject::connect(thread, SIGNAL(started()), m_TitleLoader, SLOT(LoadTitle()));

  // On Successfull title composition, update with the window title
  QObject::connect(m_TitleLoader, 
                   SIGNAL(TitleLoaded(const QString &)), 
                   this, 
                   SLOT(setWindowTitle(const QString & ))
    );

  // Cleanup
  //  - quit the thread's event loop, exit cleanly
  QObject::connect(m_TitleLoader, SIGNAL(Finished()), thread, SLOT(quit()));
  //  - cleanup heap
  QObject::connect(m_TitleLoader, SIGNAL(Finished()), m_TitleLoader, SLOT(deleteLater()));
  QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

  // GO
  thread->start();

  //
  // End title setup ------------------------------------------------------------

  //
  // COLOR SETUP.
  SetControllerModel( GetColorSetupDock(), vectorImageModel );

  //
  // COLOR DYNAMICS.
  SetControllerModel( GetColorDynamicsDock(), vectorImageModel );

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

  // signal used to update the ql widget
  QObject::connect(
    centralWidget(),
    SIGNAL( CentralWidgetUpdated() ),
    // to:
    GetQuicklookDock()->widget(),
    SLOT( updateGL()  )
  );

  // 
  // send the physical point of the clicked point in screen 
  // vectorImageModel is in charge of pixel information computation
    QObject::connect(
    m_QLViewManipulator, SIGNAL( PhysicalCursorPositionChanged(double, double) ), 
    vectorImageModel, SLOT( OnPhysicalCursorPositionChanged(double, double) )
    );
  
  QObject::connect(
    m_ImageViewManipulator, SIGNAL( PhysicalCursorPositionChanged(double, double) ), 
    vectorImageModel, SLOT( OnPhysicalCursorPositionChanged(double, double) )
    );

  //
  // connect the statusBar widget to the vectorImage corresponding
  // signal 
  QObject::connect(
    vectorImageModel, 
    SIGNAL( CurrentIndexUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentPixelIndexWidget(),
    SLOT( setText(const QString &) )
  );

  QObject::connect(
    vectorImageModel, 
    SIGNAL( CurrentRadioUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentPixelRadioWidget(),
    SLOT( setText(const QString &) )
  );

  QObject::connect(
    m_ImageViewManipulator, 
    SIGNAL( CurrentScaleUpdated(const QString& ) ),
    m_StatusBarWidget->GetCurrentScaleWidget(),
    SLOT( setText(const QString&) )
  );

  // index widget in status bar edited
  QObject::connect(m_StatusBarWidget->GetCurrentPixelIndexWidget(),
                   SIGNAL( editingFinished() ),
                   this,
                   SLOT( OnUserCoordinatesEditingFinished() )
    );
  
  QObject::connect(this,
                   SIGNAL( UserCoordinatesEditingFinished(const QString&) ),
                   vectorImageModel,
                   SLOT( OnUserCoordinatesEditingFinished(const QString&) )
    );

  QObject::connect(vectorImageModel,
                   SIGNAL( ViewportRegionChanged(double, double) ),
                   m_ImageViewManipulator,
                   SLOT( OnViewportRegionChanged(double, double) )
    );

  // scale widget in status bar edited
  QObject::connect(m_StatusBarWidget->GetCurrentScaleWidget(),
                   SIGNAL( editingFinished() ),
                   this,
                   SLOT( OnUserScaleEditingFinished() )
    );
  
  QObject::connect(this,
                   SIGNAL( UserScaleEditingFinished(const QString&) ),
                   m_ImageViewManipulator,
                   SLOT( OnUserScaleEditingFinished(const QString&) )
    );

  //
  // MAIN VIEW.

  // Assign newly selected model to view.
  qobject_cast< GLImageWidget *>( centralWidget() )->SetImageModel(
    vectorImageModel
  );

  //
  // QUICKLOOK VIEW.
  
  // Assign newly selected model to view.
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->SetImageModel(
    vectorImageModel->GetQuicklookModel()
  );
}

/*****************************************************************************/
void
MainWindow
::OnUserCoordinatesEditingFinished()
{
  // get the text and send it to the vector image model to be
  // processed 
  QString coord = m_StatusBarWidget->GetCurrentPixelIndexWidget()->text();
  emit UserCoordinatesEditingFinished(coord);

  // update the Quicklook
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->update();
}

/*****************************************************************************/
void
MainWindow
::OnUserScaleEditingFinished()
{
  // get the text and send it to the view manipulator to be
  // processed 
  QString scale = m_StatusBarWidget->GetCurrentScaleWidget()->text();
  emit UserScaleEditingFinished(scale);

  // update the Quicklook
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->update();
}

/*****************************************************************************/
void
MainWindow
::OnOpenImageRequest( QString filename )
{
  // Inspired by :
  // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation
  
  // TODO : move this code to core ?

  //
  // get the absolute filepath from the input filename everytime
  QFileInfo  finfo(filename);  
 
  QThread* thread = new QThread;
  ImageLoader* loader = new ImageLoader(finfo.absoluteFilePath(), centralWidget()->width(), centralWidget()->height());
  loader->moveToThread(thread);
  
  // At thread startup, trigger the processing function
  connect(thread, SIGNAL(started()), loader, SLOT(OpenImage()));
  
  // On successfull image loading, notify ourself, passing the created AbstractModel instance
  connect(loader, SIGNAL(ModelLoaded(AbstractModel*)), this, SLOT(OnModelLoaded(AbstractModel*)));
  // On error, notify ourself
  connect(loader, SIGNAL(Error(QString)), this, SLOT(OnOpenImageError(QString)));
  
  // Cleanup
  //  - quit the thread's event loop, exit cleanly
  connect(loader, SIGNAL(Finished()), thread, SLOT(quit()));
  //  - hide the progress dialog
  connect(loader, SIGNAL(Finished()), this, SLOT(OnHideProgressDialog()));
  //  - cleanup heap
  connect(loader, SIGNAL(Finished()), loader, SLOT(deleteLater()));
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  
  // Everything is set up. Let's go !
  thread->start();
}

/*****************************************************************************/
void
MainWindow
::OnOpenImageError( QString message )
{
  QMessageBox::warning( this, tr("Error"), message );
}

/*****************************************************************************/
void
MainWindow
::OnModelLoaded( AbstractModel* model )
{
  try
    {
    Application::Instance()->SetModel( model );
    }
  catch( std::exception& exc )
    {
    OnOpenImageError( exc.what() );
    return;
    }
}


/*****************************************************************************/
void
MainWindow
::OnShowProgressDialog( QString filename )
{
  QString targetFile;
  QString targetPath;
  Application::DatasetPathName( targetPath, targetFile, filename );

  m_DatasetCreationProgressDialog->SetImage( filename );
  m_DatasetCreationProgressDialog->SetDataset( targetPath + QDir::separator() + targetFile  );  

  m_DatasetCreationProgressDialog->adjustSize();
  m_DatasetCreationProgressDialog->show();
}

/*****************************************************************************/
void
MainWindow
::OnHideProgressDialog( )
{
  m_DatasetCreationProgressDialog->hide();
}

/*****************************************************************************/

} // end namespace 'mvd'
