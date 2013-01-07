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
}

/*****************************************************************************/
void
MainWindow::InitializeDockWidgets()
{
  QDockWidget* videoColorSetupDockWidget =
    new QDockWidget( tr( "Video color setup" ), this );

  // You can use findChild( "videoColorSetupDockWidget" ) to get dock-widget.
  videoColorSetupDockWidget->setObjectName( "videoColorSetupDockWidget" );
  videoColorSetupDockWidget->setWidget(
    new ColorSetupWidget( videoColorSetupDockWidget )
  );
  videoColorSetupDockWidget->setFloating( true );
  videoColorSetupDockWidget->setFeatures(
    QDockWidget::DockWidgetMovable |
    QDockWidget::DockWidgetFloatable
  );

  addDockWidget( Qt::LeftDockWidgetArea, videoColorSetupDockWidget );

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( new GLImageWidget( this ) );
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

} // end namespace 'mvd'
