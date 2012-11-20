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

#include "otbImageLayer.h"
#include "otbImageLayerGenerator.h"
//
// System includes (sorted by alphabetic order)
#include <exception>

//
// Qt includes (sorted by alphabetic order)
#include <QFileDialog>
#include <QMessageBox>

//
// ITK includes (sorted by alphabetic order)
#include "itkRGBAPixel.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbImage.h"
#include "otbImageFileReader.h"

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{

/*****************************************************************************/
MainWindow
::MainWindow( QWidget* parent, Qt::WindowFlags flags ) :
  QMainWindow( parent, flags ),
  m_UI( new Ui::MainWindow() )
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
  /*
  QDockWidget* dock_widget = new DockWidget( tr( "Dock Widget" ), this );
  
  dock_widget->setAllowedAreas(
    Qt::LeftDockWidgetArea |
    Qt::RightDockWidgetArea
  );

  dockWidget->setWidget( dockWidgetContents );
  addDockWidget( Qt::LeftDockWidgetArea, dock_widget );
  */

  // Instanciate a QImageView
  m_ImageView = ImageViewType::New();

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( m_ImageView->GetFullWidget() );
  

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

  //
  // TODO: Move piece of code below to ImageModel.
  ReaderType::Pointer reader( ReaderType::New() );
  reader->SetFileName( filename.toLatin1().data() );
  reader->UpdateOutputInformation();

  // typedef support for layers   
  typedef ImageLayer<VectorImageType, ImageType>    LayerType;
  typedef LayerType::Pointer                        LayerPointerType;
  typedef ImageLayerGenerator<LayerType>            LayerGeneratorType;
  typedef LayerGeneratorType::RenderingFunctionType RenderingFunctionType;
  
  // Layer Generator
  LayerGeneratorType::Pointer layerGenerator = LayerGeneratorType::New();
  layerGenerator->SetImage(reader->GetOutput());
  layerGenerator->GenerateQuicklookOff();
  layerGenerator->GenerateLayer();
  
  // Layer Rendering Model
  RenderingModelType::Pointer imageModel = RenderingModelType::New();

  // TODO : temporary cause we need an extractRegion to setup the
  // ExtractROI filter.
  RegionType  region;
  region = imageModel->GetExtractRegion();
  RegionType::SizeType size;
  size[0] = static_cast<unsigned int>(this->width());
  size[1] = static_cast<unsigned int>(this->height());
  region.SetSize(size);  
  imageModel->SetExtractRegion(region);
  
  // Add the layer to the model
  imageModel->AddLayer(layerGenerator->GetLayer());
  imageModel->Update();

  // Update the image view
  m_ImageView->SetModel(imageModel);
  m_ImageView->Update();
}

/*****************************************************************************/

} // end namespace 'mvd'
