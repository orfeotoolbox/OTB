/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdI18nMainWindow_h
#define __mvdI18nMainWindow_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

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


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External class pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal class pre-declaration.
class AbstractModel;
class DatasetModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class I18nMainWindow
 *
 * \brief Main-window widget base for the i18n application.
 */
class Monteverdi2_EXPORT I18nMainWindow
  : public QMainWindow
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

//
// Public methods.
public:
  /** \brief Destructor. */
  virtual ~I18nMainWindow();

  /**
   */
  void Initialize();

  /**
   */
  void SetupCacheDir();

  /**
   * \brief This is a convenience static method that will return an
   * existing directory path selected by the user.
   *
   * This method stores the last accepted directory for next call.
   *
   * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getExistingDirectory
   *
   * \param parent Parent widget of that QFileDialog.
   * \param caption Title-bar caption of that QFileDialog.
   * \param dir Directory location where to pick path.
   * \param options of that QFileDialog.
   *
   * \return The selected path or an null QString if QFileDialog has
   * been rejected.
   */
  static inline
    QString
    GetExistingDirectory( QWidget* parent =0,
			  const QString& caption =QString(),
			  const QString& dir = QString(),
			  QFileDialog::Options options =QFileDialog::ShowDirsOnly
    );

  /**
   * \brief This is a convenience static method that will return an
   * existing filename selected by the user.
   *
   * This method stores the last accepted directory for next call.
   *
   * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getOpenFileName
   *
   * \param parent Parent widget of that QFileDialog.
   * \param caption Title-bar caption of that QFileDialog.
   * \param dir Directory location where to pick path.
   * \param filter Filename filter (e.g. wildcards).
   * \param selectedFilter Filter which has been selected by the user.
   * \param options Options of that QFileDialog.
   *
   * \return The selected filename or an null QString if QFileDialog has
   * been rejected.
   */
  static inline
    QString
    GetOpenFileName( QWidget* parent =0,
		     const QString& caption =QString(),
		     const QString& dir = QString(),
		     const QString& filter = QString(),
		     QString* selectedFilter =0,
		     QFileDialog::Options options =0 );

  /**
   * \brief This is a convenience static method that will return a
   * list of existing filenames selected by the user.
   *
   * This method stores the last accepted directory for next call.
   *
   * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getOpenFileNames
   *
   * \param parent Parent widget of that QFileDialog.
   * \param caption Title-bar caption of that QFileDialog.
   * \param dir Directory location where to pick path.
   * \param filter Filename filter (e.g. wildcards).
   * \param selectedFilter Filter which has been selected by the user.
   * \param options of that QFileDialog.
   *
   * \return The selected filename list or an empty QStringLIst if
   * QFileDialog has been rejected.
   */
  static inline
    QStringList
    GetOpenFileNames( QWidget* parent =0,
		      const QString& caption =QString(),
		      const QString& dir =QString(),
		      const QString& filter =QString(),
		      QString* selectedFilter =0,
		      QFileDialog::Options options =0 );

  /**
   * \brief This is a convenience static method that will return a
   * filename selected by the user. This file does not have to exist.
   *
   * This method stores the last accepted directory for next call.
   *
   * \see http://qt-project.org/doc/qt-4.8/qfiledialog.html#getSaveFileName
   *
   * \param parent Parent widget of that QFileDialog.
   * \param caption Title-bar caption of that QFileDialog.
   * \param dir Directory location where to pick path.
   * \param filter Filename filter (e.g. wildcards).
   * \param selectedFilter Filter which has been selected by the user.
   * \param options of that QFileDialog.
   *
   * \return The selected filename list or an empty QStringLIst if
   * QFileDialog has been rejected.
   */
  static inline
    QString
    GetSaveFileName( QWidget* parent =0,
		     const QString& caption =QString(),
		     const QString& dir =QString(),
		     const QString& filter =QString(),
		     QString* selectedFilter =0,
		     QFileDialog::Options options =0 );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** \brief Constructor. */
  I18nMainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /**
   */
  QDockWidget*
    AddWidgetToDock( QWidget* widget,
		     const QString& dockName,
		     const QString& dockTitle,
		     Qt::DockWidgetArea dockArea,
		     bool isFloating =false );

  /**
   */
  template< typename TWidget, typename TDockWidget >
    inline
    TDockWidget*
    AddDockWidget( const QString& dockName,
		   const QString& dockTitle,
		   Qt::DockWidgetArea dockArea,
		   bool isFloating =false );

  /**
   */
  template< typename TWidget, typename TController, typename TDockWidget >
    inline
    TDockWidget*
    AddDockWidget( const QString& dockName,
		   const QString& dockTitle,
		   Qt::DockWidgetArea dockArea,
		   bool isFloating =false );

  /**
   * \brief Assign model to the controller which is child of given
   * dock-widget.
   *
   * \param dock The dock-widget which is parent of the controller to
   * assign model to.
   *
   * \param model The model to assign to the controller.
   */
  inline void SetControllerModel( QDockWidget* dock, AbstractModel* model );

  /**
   */
  DatasetModel *
    ImportDataset( const QString& filename,
                   int width,
                   int height,
                   bool forceCreate );
  

  //
  // QMainWindow overrides.

  void closeEvent( QCloseEvent* event );

//
// Protected attributes.
protected:

  /*-[ PROTECTED SLOTS SECTION ]---------------------------------------------*/

//
// Protected slots. 
protected slots:

  /**
   */
  virtual void OnAboutToChangeModel( const AbstractModel* ) =0;

  /**
   */
  virtual void OnModelChanged( AbstractModel* ) =0;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private nested classes.
private:


//
// Private methods.
private:

  /**
   */
  virtual void virtual_SetupUI() =0;

  /**
   */
  virtual void virtual_ConnectUI() =0;

  /**
   */
  inline QMessageBox::StandardButton ConfirmSaveQuit( bool canBeCancelled );

//
// Private attributes.
private:

  /**
   * \brief Holds the last location accepted using QFileDialog.
   */
  static QDir m_LastAcceptedDir;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private slots.
private slots:

  /**
   * \brief Qt auto-connected slot which is called when File/Quit menu
   * action is activated.
   */
  void on_action_Quit_triggered();

  /**
   * \brief Qt auto-connected slot which is called when Help/About
   * menu action is activated.
   */
  virtual void on_action_About_activated();

};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdAbstractModelController.h"

namespace mvd
{

/*****************************************************************************/
inline
void
I18nMainWindow
::SetControllerModel( QDockWidget* dock, AbstractModel* model )
{
  assert( dock );

  AbstractModelController* controller =
    dock->findChild< AbstractModelController* >();

  assert( controller!=NULL );

  controller->SetModel( model );
}

/*****************************************************************************/
inline
QMessageBox::StandardButton
I18nMainWindow
::ConfirmSaveQuit( bool canBeCancelled )
{
  return QMessageBox::question(
    this,
    tr( PROJECT_NAME ),
    tr( "Dataset has been modified.\n"
	"Do you want to save settings before quitting?" ),
    QMessageBox::Save |
    QMessageBox::Discard |
    ( canBeCancelled ? QMessageBox::Cancel : QMessageBox::NoButton ),
    QMessageBox::Save
  );
}

/*****************************************************************************/
template< typename TWidget, typename TDockWidget >
inline
TDockWidget*
I18nMainWindow
::AddDockWidget( const QString& dockName,
		 const QString& dockTitle,
		 Qt::DockWidgetArea dockArea,
		 bool isFloating )
{
  TWidget* widget = new TWidget( this );

  TDockWidget* dockWidget =
    AddWidgetToDock(
      widget,
      dockName,
      dockTitle,
      dockArea,
      isFloating
    );

  return dockWidget;
}

/*****************************************************************************/
template< typename TWidget, typename TController, typename TDockWidget >
inline
TDockWidget*
I18nMainWindow
::AddDockWidget( const QString& dockName,
		 const QString& dockTitle,
		 Qt::DockWidgetArea dockArea,
		 bool isFloating )
{
  TWidget* widget = new TWidget( this );

  TDockWidget* dockWidget =
    AddWidgetToDock(
      widget,
      dockName,
      dockTitle,
      dockArea,
      isFloating
    );

  new TController(
    // wraps:
    widget,
    // as chid of:
    dockWidget
  );

  return dockWidget;
}

/*****************************************************************************/
inline
QString
I18nMainWindow
::GetExistingDirectory( QWidget* parent,
			const QString& caption,
			const QString& dir,
			QFileDialog::Options options )
{
  QString path(
    QFileDialog::getExistingDirectory(
      parent,
      caption,
      dir.isEmpty() ? I18nMainWindow::m_LastAcceptedDir.path() : dir,
      options
    )
  );

  if( !path.isNull() )
    I18nMainWindow::m_LastAcceptedDir.setPath( path );

  return path;
}

/*****************************************************************************/
inline
QString
I18nMainWindow
::GetOpenFileName( QWidget* parent,
		   const QString& caption,
		   const QString& dir,
		   const QString& filter,
		   QString* selectedFilter,
		   QFileDialog::Options options )
{
  QString filename(
    QFileDialog::getOpenFileName(
      parent,
      caption,
      dir.isEmpty() ? I18nMainWindow::m_LastAcceptedDir.path() : dir,
      filter,
      selectedFilter,
      options
    )
  );

  if( !filename.isNull() )
    I18nMainWindow::m_LastAcceptedDir.setPath( QFileInfo( filename ).path() );

  return filename;
}

/*****************************************************************************/
inline
QStringList
I18nMainWindow
::GetOpenFileNames( QWidget* parent,
		    const QString& caption,
		    const QString& dir,
		    const QString& filter,
		    QString* selectedFilter,
		    QFileDialog::Options options )
{
  QStringList filenames(
    QFileDialog::getOpenFileName(
      parent,
      caption,
      dir.isEmpty() ? I18nMainWindow::m_LastAcceptedDir.path() : dir,
      filter,
      selectedFilter,
      options
    )
  );

  if( !filenames.isEmpty() )
    I18nMainWindow::m_LastAcceptedDir.setPath(
      QFileInfo( filenames[ 0 ] ).path()
    );

  return filenames;
}

/*****************************************************************************/
inline
QString
I18nMainWindow
::GetSaveFileName( QWidget* parent,
		   const QString& caption,
		   const QString& dir,
		   const QString& filter,
		   QString* selectedFilter,
		   QFileDialog::Options options )
{
  QString filename(
    QFileDialog::getSaveFileName(
      parent,
      caption,
      dir.isEmpty() ? I18nMainWindow::m_LastAcceptedDir.path() : dir,
      filter,
      selectedFilter,
      options
    )
  );

  if( !filename.isNull() )
    I18nMainWindow::m_LastAcceptedDir.setPath( QFileInfo( filename ).path() );

  return filename;
}

} // end namespace 'mvd'

#endif // __I18nMainWindow_h
