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

//
// System includes (sorted by alphabetic order)
#include <QtGui>

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
  m_ImageWidget =  new GLImageWidget(parent);
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

/*
** WARNING: Do not mix QObject allocation system with the ITK one!
** (reason: QObject instances are linked in a parent-children tree;
** deallocaingt an object (especially widgets) deallocates its
** children).
*/
  // Instanciate a QImageView
  //m_ImageView = ImageViewType::New();

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( m_ImageWidget );

// Connect Quit action of main menu to QApplication's quit() slot.
  QObject::connect(
    m_UI->action_Quit, SIGNAL( activated() ),
    qApp, SLOT( quit() )
  );
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
    "mvd::VectorImageModel('" + filename + "'"
  );

  try
    {
    m_VectorImageModel->loadFile( filename );

    dynamic_cast< Application* >( qApp )->SetModel( m_VectorImageModel );
    }
  catch( std::exception& exc )
    {
    delete m_VectorImageModel;
    m_VectorImageModel = NULL;

    QMessageBox::warning( this, tr("Exception!"), exc.what() );
    return;
    }

  // // typedef support for layers   
  // typedef otb::ImageLayer<VectorImageType, ImageType>    LayerType;
  // typedef LayerType::Pointer                        LayerPointerType;
  // typedef otb::ImageLayerGenerator<LayerType>            LayerGeneratorType;
  // typedef LayerGeneratorType::RenderingFunctionType RenderingFunctionType;

  // // Layer Generator
  // LayerGeneratorType::Pointer layerGenerator = LayerGeneratorType::New();

  // layerGenerator->SetImage(
  //   dynamic_cast< VectorImageModel* >
  //   ( dynamic_cast< Application* >
  //     ( qApp )->GetModel() )->GetOutput( 0 )
  // );

  // layerGenerator->GenerateQuicklookOff();
  // layerGenerator->GenerateLayer();
  
  // // Layer Rendering Model
  // RenderingModelType::Pointer imageModel = RenderingModelType::New();

  // // TODO : temporary cause we need an extractRegion to setup the
  // // ExtractROI filter.
  // RegionType  region;
  // region = imageModel->GetExtractRegion();
  // RegionType::SizeType size;
  // size[0] = static_cast<unsigned int>(this->width());
  // size[1] = static_cast<unsigned int>(this->height());
  // region.SetSize(size);  
  // imageModel->SetExtractRegion(region);
  
  // // Add the layer to the model
  // imageModel->AddLayer(layerGenerator->GetLayer());
  // imageModel->Update();

  // // Update the image view
  // //m_ImageView->SetModel(imageModel);
  // //m_ImageView->Update();
}

/*****************************************************************************/

} // end namespace 'mvd'
