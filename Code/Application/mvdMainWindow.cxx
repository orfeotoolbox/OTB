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
#include "itkRGBAPixel.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbImageLayer.h"
#include "otbImageLayerGenerator.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
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

  /*
  QDockWidget* dock_widget = new DockWidget( tr( "Dock Widget" ), this );
  
  dock_widget->setAllowedAreas(
    Qt::LeftDockWidgetArea |
    Qt::RightDockWidgetArea
  );

  dockWidget->setWidget( dockWidgetContents );
  addDockWidget( Qt::LeftDockWidgetArea, dock_widget );
  */

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( new GLImageWidget( this ) );

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
  m_VectorImageModel = new VectorImageModel();

  m_VectorImageModel->setObjectName(
    "mvd::VectorImageModel('" + filename + "')"
  );

  // load file
  try
    {
    m_VectorImageModel->loadFile( filename );

    // set the largest possible region of the image
    // TODO:  rename signal name when handling DataSets collections
    emit largestPossibleRegionChanged(m_VectorImageModel->GetOutput(0)->GetLargestPossibleRegion());

    dynamic_cast< Application* >( qApp )->SetModel( m_VectorImageModel );
    }
  catch( std::exception& exc )
    {
    delete m_VectorImageModel;
    m_VectorImageModel = NULL;

    QMessageBox::warning( this, tr("Exception!"), exc.what() );
    return;
    }
}

/*****************************************************************************/

} // end namespace 'mvd'
