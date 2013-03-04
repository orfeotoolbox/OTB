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
#include "mvdAboutDialog.h"
#include "mvdApplication.h"
#include "mvdColorDynamicsController.h"
#include "mvdColorDynamicsWidget.h"
#include "mvdColorSetupController.h"
#include "mvdDatasetModel.h"
#include "mvdGLImageWidget.h"
#include "mvdImageModelRenderer.h"
#include "mvdImageViewManipulator.h"
#include "mvdPreferencesDialog.h"
#include "mvdQuicklookModel.h"
#include "mvdQuicklookViewManipulator.h"
#include "mvdSystemError.h"
#include "mvdVectorImageModel.h"


namespace mvd
{

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}
  
void ImageLoader::OpenImage()
{
  // This method is executed in a separate thread
  /*
  class Sleeper : public QThread
  {
    public:
    static void sleep(unsigned long secs) {
      QThread::sleep(secs);
      }
  };

  Sleeper::sleep( 5 );
  */
  
  try
    {
    DatasetModel* model = Application::LoadDatasetModel(
      filename, width, height );

    // We can only push to another thread,
    // so thread affinity must be set here,
    // and not in the slot that receives the object
    model->moveToThread(Application::Instance()->thread());
    
    emit ModelLoaded(model);
    }
  catch( std::exception& exc )
    {
    emit Error( exc.what() );
    return;
    }
  emit Finished();
}

  
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
  m_DatasetCreationProgressDialog( 0 )
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
  QString defaultCacheDirStr (QDir::homePath());
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
    msgBox.setWindowTitle("Welcome new Montevedi2 user !");
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
      cacheDirStr = defaultCacheDirStr;
      }
    else
      { // User select something
      cacheDirStr = datasetDir;
      }
    }
  else
    if (ret == QMessageBox::No)
      { // User push default button => set the value to the default place
      cacheDirStr = defaultCacheDirStr;
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
  
  // instanciate the Ql manipulator/renderer 
  m_QLModelRenderer   = new ImageModelRenderer();
  m_QLViewManipulator = new QuicklookViewManipulator();

  // Connect centralWidget manipulator to Ql renderer when viewportRegionChanged
  QObject::connect(
    m_ImageViewManipulator, SIGNAL( ViewportRegionRepresentationChanged(const PointType&, const PointType&) ), 
    m_QLModelRenderer, SLOT( OnViewportRegionRepresentationChanged(const PointType&, const PointType&) )
    );

  // Connect ql mousePressEventpressed to centralWidget manipulator
  QObject::connect(
    m_QLViewManipulator, SIGNAL( ViewportRegionChanged(double, double) ), 
    m_ImageViewManipulator, SLOT( OnViewportRegionChanged(double, double) )
  );

  // add the needed docks 
  InitializeDockWidgets();

  // add needed widget to the status bar
  InitializeStatusBarWidgets();

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

  // Connect Application and MainWindow when selected model has been
  // changed.
  QObject::connect(
    qApp, SIGNAL( SelectedModelChanged( AbstractModel* ) ),
    this, SLOT( OnSelectedModelChanged( AbstractModel* ) )
  );
  
  // 
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
  // inspired from Qgis
  //QFont myFont( "Arial", 9 );

  // label
  m_CurrentPixelIndexLabel = new QLabel( QString(), statusBar() );
  //m_CurrentPixelIndexLabel->setFont( myFont );
  m_CurrentPixelIndexLabel->setMinimumWidth( 10 );
  m_CurrentPixelIndexLabel->setMaximumHeight( 20 );
  m_CurrentPixelIndexLabel->setMargin( 3 );
  m_CurrentPixelIndexLabel->setAlignment( Qt::AlignCenter );
  m_CurrentPixelIndexLabel->setFrameStyle( QFrame::NoFrame );
  m_CurrentPixelIndexLabel->setText( tr( "Index:" ) );
  m_CurrentPixelIndexLabel->setToolTip( tr( "Current map coordinate" ) );
  statusBar()->addPermanentWidget( m_CurrentPixelIndexLabel, 0 );
  
  // Add a QLabel to the status bar to show pixel index
  m_CurrentPixelIndex = new QLineEdit(QString(), statusBar());
  //m_CurrentPixelIndex->setFont( myFont );
  m_CurrentPixelIndex->setMinimumWidth( 10 );
  m_CurrentPixelIndex->setContentsMargins( 0, 0, 0, 0 );
  m_CurrentPixelIndex->setAlignment( Qt::AlignCenter );
  statusBar()->addPermanentWidget( m_CurrentPixelIndex,      0 );

  
  
  // Add a QLabel to the status bar to show pixel index
  m_CurrentPixelPhysical = new QLabel(statusBar());
  //m_CurrentPixelPhysical->setFont( myFont );
  m_CurrentPixelPhysical->setAlignment(Qt::AlignCenter);
  statusBar()->addPermanentWidget( m_CurrentPixelPhysical,   0 );

  // Add a QLabel to the status bar to show pixel index
  m_CurrentPixelGeographic = new QLabel(statusBar());
  //m_CurrentPixelGeographic->setFont( myFont );
  m_CurrentPixelGeographic->setAlignment(Qt::AlignCenter);
  statusBar()->addPermanentWidget( m_CurrentPixelGeographic, 0 );

  // Add a QLabel to the status bar to show pixel index
  m_CurrentPixelRadio = new QLabel(statusBar());
  m_CurrentPixelRadio->setAlignment(Qt::AlignCenter);
  statusBar()->addPermanentWidget( m_CurrentPixelRadio,      1 );
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
    QDockWidget::DockWidgetFloatable |
    QDockWidget::DockWidgetClosable
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

  emit OpenImageRequest( filename );
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
  m_UI->action_Quicklook->setChecked( visible );
}

/*****************************************************************************/
// The Color Setup DockWidget's visibilityChanged signal is linked to the
// View/Color Setup show/hide checkbox
void
MainWindow
::OnColorSetupVisibilityChanged ( bool visible )
{
  m_UI->action_ColorSetup->setChecked( visible );
}

/*****************************************************************************/
// The Color Dynamics DockWidget's visibilityChanged signal is linked to the
// View/Color Dynamics show/hide checkbox
void
MainWindow
::OnColorDynamicsVisibilityChanged ( bool visible )
{
  m_UI->action_ColorDynamics->setChecked( visible );
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
    m_CurrentPixelIndex,
    SLOT( setText(const QString &) )
  );

  QObject::disconnect(
    vectorImageModel, 
    SIGNAL( CurrentPhysicalUpdated(const QString& ) ),
    m_CurrentPixelPhysical,
    SLOT( setText(const QString &) )
  );

  QObject::disconnect(
    vectorImageModel, 
    SIGNAL( CurrentGeographicUpdated(const QString& ) ),
    m_CurrentPixelGeographic,
    SLOT( setText(const QString &) )
  );

  QObject::disconnect(
    vectorImageModel, 
    SIGNAL( CurrentRadioUpdated(const QString& ) ),
    m_CurrentPixelRadio,
    SLOT( setText(const QString &) )
  );

  // index widget edition
  QObject::disconnect(m_CurrentPixelIndex,
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

  std::ostringstream oss;
  oss<<PROJECT_NAME<<" - "<<otb::System::GetShortFileName(ToStdString(vectorImageModel->GetFilename()));
  oss<<" ("<<vectorImageModel->GetNbComponents()<<tr(" bands, ").toLatin1().constData();
  oss<<vectorImageModel->GetNativeLargestRegion().GetSize()[0];
  oss<<"x"<<vectorImageModel->GetNativeLargestRegion().GetSize()[1]<<tr(" pixels)").toLatin1().constData();
  
  // add the placename to the title if any
  std::string placename = vectorImageModel->GetCenterPixelPlaceName();
  if (!placename.empty())
    {
    oss <<" - " <<ToStdString( tr("Location") )<< " : " << placename;
    }
  
  setWindowTitle( FromStdString(oss.str()) );

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
    m_CurrentPixelIndex,
    SLOT( setText(const QString &) )
  );

  QObject::connect(
    vectorImageModel, 
    SIGNAL( CurrentPhysicalUpdated(const QString& ) ),
    m_CurrentPixelPhysical,
    SLOT( setText(const QString &) )
  );

  QObject::connect(
    vectorImageModel, 
    SIGNAL( CurrentGeographicUpdated(const QString& ) ),
    m_CurrentPixelGeographic,
    SLOT( setText(const QString &) )
  );

  QObject::connect(
    vectorImageModel, 
    SIGNAL( CurrentRadioUpdated(const QString& ) ),
    m_CurrentPixelRadio,
    SLOT( setText(const QString &) )
  );

  // index widget edition
  QObject::connect(m_CurrentPixelIndex,
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
}

/*****************************************************************************/
void
MainWindow
::OnUserCoordinatesEditingFinished()
{
  // get the text and send it to the vector image model to be
  // processed 
  QString coord = m_CurrentPixelIndex->text();
  emit UserCoordinatesEditingFinished(coord);

  // update the Quicklook
  qobject_cast< GLImageWidget * >( GetQuicklookDock()->widget() )->update();
}

void
MainWindow
::OnOpenImageRequest( QString filename )
{
  delete m_DatasetCreationProgressDialog;
  m_DatasetCreationProgressDialog = new DatasetCreationProgressDialog(this);
  
  m_DatasetCreationProgressDialog->SetImage( filename );
  
  QString targetFile;
  QString targetPath;
  Application::DatasetPathName( targetPath, targetFile, filename );
  
  m_DatasetCreationProgressDialog->SetDataset( targetPath + QDir::separator() + filename  );  
  m_DatasetCreationProgressDialog->show();
  
  // Inspired by :
  // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation
  
  QThread* thread = new QThread;
  ImageLoader* loader = new ImageLoader();
  loader->filename = filename;
  loader->width = centralWidget()->width();
  loader->height = centralWidget()->height();
  loader->moveToThread(thread);
  
  connect(loader, SIGNAL(Error(QString)), this, SLOT(OnError(QString)));
  connect(thread, SIGNAL(started()), loader, SLOT(OpenImage()));
  connect(loader, SIGNAL(ModelLoaded(AbstractModel*)), this, SLOT(OnModelLoaded(AbstractModel*)));
  connect(loader, SIGNAL(Finished()), thread, SLOT(quit()));
  connect(loader, SIGNAL(Finished()), loader, SLOT(deleteLater()));
  connect(thread, SIGNAL(Finished()), thread, SLOT(deleteLater()));
  thread->start();
}


void
MainWindow
::OnError( QString message )
{
  QMessageBox::warning( this, tr("Erreur"), message );
}

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
    OnError( exc.what() );
    return;
    }
  
  m_DatasetCreationProgressDialog->close();
  delete m_DatasetCreationProgressDialog;
  m_DatasetCreationProgressDialog = 0;
}

/*****************************************************************************/

} // end namespace 'mvd'
