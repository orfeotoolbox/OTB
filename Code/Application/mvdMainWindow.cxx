/*=========================================================================

  Program:   qTutor
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 
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
  ui( new Ui::MainWindow() )
{
  ui->setupUi( this );

/*
  QDockWidget* dock_widget = new DockWidget( tr( "Dock Widget" ), this );

  dock_widget->setAllowedAreas(
    Qt::LeftDockWidgetArea |
    Qt::RightDockWidgetArea
    );

  // dockWidget->setWidget( dockWidgetContents );

  addDockWidget( Qt::LeftDockWidgetArea, dock_widget );
*/

  QObject::connect(
    ui->action_Quit, SIGNAL( activated() ),
    qApp, SLOT( quit() )
    );
}

MainWindow
::~MainWindow()
{
}

//
// SLOTS
//

void
MainWindow
::on_action_Quit_activated()
{
  //qApp->quit();
}

} // end namespace 'mvd'
