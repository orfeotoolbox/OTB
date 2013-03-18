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

#include "mvdI18nMainWindow.h"

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
#include "Core/mvdI18nApplication.h"
#include "Core/mvdDatasetModel.h"
//
#include "Gui/mvdAboutDialog.h"

namespace mvd
{
  
/*
  TRANSLATOR mvd::I18nMainWindow

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
I18nMainWindow
::I18nMainWindow( QWidget* parent, Qt::WindowFlags flags ) :
  QMainWindow( parent, flags )
{
}

/*****************************************************************************/
I18nMainWindow
::~I18nMainWindow()
{
}

/*****************************************************************************/
void
I18nMainWindow
::Initialize()
{
  /*
  // Default setup.
  setObjectName( "mvd::I18nMainWindow" );
  setWindowTitle( PROJECT_NAME );
  */

  virtual_SetupUI();

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

  virtual_ConnectUI();

  // Change to NULL model to force emitting GUI signals when GUI is
  // instanciated. So, GUI will be initialized and controller-widgets
  // disabled.
  I18nApplication::Instance()->SetModel( NULL );
}

/*****************************************************************************/
QDockWidget*
I18nMainWindow
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
I18nMainWindow
::closeEvent( QCloseEvent* event )
{
  assert( event!=NULL );

  qDebug() << "I18nMainWindow::closeEvent(" << event << ")";

  // Ensure there is an application instance.
  assert( I18nApplication::ConstInstance()!=NULL );
  // Ensure that there is no model or that the existing model is a
  // DatasetModel.
  assert( I18nApplication::ConstInstance()->GetModel()==
	  I18nApplication::ConstInstance()->GetModel< DatasetModel >() );
  // Get model.
  DatasetModel* model =
    I18nApplication::Instance()->GetModel< DatasetModel >();

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
I18nMainWindow
::on_action_Quit_triggered()
{
  close();
}

/*****************************************************************************/
void
I18nMainWindow
::on_action_About_activated()
{
  AboutDialog aboutDialog( this );

  aboutDialog.exec();
}

/*****************************************************************************/

} // end namespace 'mvd'
