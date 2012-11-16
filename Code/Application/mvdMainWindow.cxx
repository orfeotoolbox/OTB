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

  // Set the GLImageWidget as the centralWidget in MainWindow.
  setCentralWidget( new otb::QGLImageWidget( this ) );
  

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
  
  // image reader 
  typedef itk::RGBAPixel<unsigned char>   PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef ImageType::RegionType           RegionType;
  typedef otb::ImageFileReader<ImageType>   ReaderType;

  otb::QGLImageWidget* widget = GetGLImageWidget();

  ReaderType::Pointer reader( ReaderType::New() );

  reader->SetFileName( filename.toLatin1().data() );

  try
    {
    reader->UpdateOutputInformation();
    }
  catch( std::exception& exc )
    {
    QMessageBox::warning( this, tr("Exception!"), exc.what() );
    return;
    }

  ImageType::Pointer image( reader->GetOutput() );

  RegionType region( image->GetLargestPossibleRegion() );

  if( widget->width() < image->GetLargestPossibleRegion().GetSize( 0 ) )
    {
    region.SetSize( 0, widget->width() );
    }

  if( widget->height() < image->GetLargestPossibleRegion().GetSize( 1 ) )
    {
    region.SetSize( 1, widget->height() );
    }

  image->SetRequestedRegion( region );
  image->Update();

  widget->SetIsotropicZoom(1);
  widget->ReadBuffer(image, region);
  // main_window.resize( region.GetSize()[0], region.GetSize()[1] );
}

/*****************************************************************************/

} // end namespace 'mvd'
