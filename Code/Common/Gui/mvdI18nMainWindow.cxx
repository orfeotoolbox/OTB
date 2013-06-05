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
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

QDir
I18nMainWindow::m_LastAcceptedDir( QDir::home().path() );

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

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
		   Qt::DockWidgetArea dockArea,
		   bool isFloating )
{
  // New dock.
  QDockWidget* dockWidget = new QDockWidget( dockTitle, this );

  // You can use findChild( dockName ) to get dock-widget.
  dockWidget->setObjectName( dockName );
  dockWidget->setWidget( widget );

  // Features.
  dockWidget->setFloating( isFloating );
  dockWidget->setFeatures(
    QDockWidget::DockWidgetMovable |
    QDockWidget::DockWidgetFloatable |
    QDockWidget::DockWidgetClosable
  );

  // Add dock.
  addDockWidget( dockArea, dockWidget );

#if 0
  // Install event-handler to catch closeEvents() if QDockWidget is
  // closable.
  //
  // N.B.: Test is done here in case QDockWidget::features() is given
  // as argument.
  if( dockWidget->features() & QDockWidget::DockWidgetClosable )
    {
    dockWidget->installEventFilter( this );
    }
#endif

    //
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

  // Get model.
  AbstractModel* model = I18nApplication::Instance()->GetModel();

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
void
I18nMainWindow
::SetupCacheDir()
{
  if( I18nApplication::ConstInstance()->IsCacheDirValid() )
    return;

  QFileInfo fileInfo( QDir::home(), I18nApplication::DEFAULT_CACHE_DIR_NAME ); 
  QDir dir( fileInfo.dir() );

  QMessageBox messageBox;

  messageBox.setWindowTitle( tr( "Welcome new " PROJECT_NAME " user!" ) );

  messageBox.setText(
    tr(
      PROJECT_NAME
      " maintains a repository where cached data related to images is stored "
      "(such as, for example, color-settings, histogram, quicklook etc.)."
      "\n\n"
      "Default cache-directory location is: '%1'."
      "\n\n"
      "Warning:\n"
      "- Your account needs to have write access to this directory;\n"
      "- Please be aware that this directory can consume significant disk "
      "space."
    )
    .arg( dir.path() )
  );

  messageBox.setInformativeText(
    tr( "Do you want to specify a custom repository location?" )
  );

  messageBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
  messageBox.setDefaultButton( QMessageBox::No );

  if( messageBox.exec()==QMessageBox::Yes )
    {
    QString path(
      QFileDialog::getExistingDirectory(
	this,
	tr(
	  "Please, select directory where the "
	  PROJECT_NAME
	  " cache repository will be stored."
	),
	dir.path()
    )
    );

    if( !path.isEmpty() )
      dir.setPath( path );
    }

  I18nApplication::Instance()->MakeCacheDir( dir.path() );
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
