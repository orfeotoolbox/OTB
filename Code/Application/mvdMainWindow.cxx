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

//
// Qt includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
//#include "DockWidget.h"

namespace mvd
{

MainWindow
::MainWindow( QWidget* parent, Qt::WindowFlags flags ) :
  QMainWindow( parent, flags ),
  m_UI( new Ui::MainWindow() )
{
  m_UI->setupUi( this );

  
  /*
  QDockWidget* dock_widget = new DockWidget( tr( "Dock Widget" ), this );

  dock_widget->setAllowedAreas(
    Qt::LeftDockWidgetArea |
    Qt::RightDockWidgetArea
    );

  dockWidget->setWidget( dockWidgetContents );
  addDockWidget( Qt::LeftDockWidgetArea, dock_widget );
  */

  // Set the GLImageWidget as the centralWidget in MainWindow
  m_QGLWidget = new otb::QGLImageWidget();
  this->setCentralWidget(m_QGLWidget);
  
  QObject::connect(
    m_UI->action_Quit, SIGNAL( activated() ),
    qApp, SLOT( quit() )
    );
}

MainWindow
::~MainWindow()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

void
MainWindow
::on_action_Open_activated()
{
  qDebug( ">DEBUG< mvd::MainWindow::on_action_Open_activated()." );
}

} // end namespace 'mvd'
