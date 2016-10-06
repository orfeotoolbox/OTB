/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef mvdI18nMainWindow_h
#define mvdI18nMainWindow_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

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
#include "OTBMonteverdiGUIExport.h"
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
class AbstractModelController;
class AbstractWorker;
class DatasetModel;
class VectorImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class I18nMainWindow
 *
 * \ingroup OTBMonteverdiGUI
*
 * \brief Main-window widget base for the i18n application.
 */
class OTBMonteverdiGUI_EXPORT I18nMainWindow
  : public QMainWindow
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  enum DockLayout
  {
    DOCK_LAYOUT_NONE = 0,
    DOCK_LAYOUT_FLOATING = 1,
    DOCK_LAYOUT_SCROLLABLE = 2,
  };

  typedef QFlags< DockLayout > DockLayoutFlags;

//
// Public methods.
public:
  /** \brief Destructor. */
  virtual ~I18nMainWindow();

  /**
   */
  void Initialize();

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
		     DockLayoutFlags flags = DOCK_LAYOUT_NONE );

  /**
   */
  template< typename TWidget, typename TDockWidget >
    inline
    TDockWidget*
    AddDockWidget( const QString& dockName,
		   const QString& dockTitle,
		   Qt::DockWidgetArea dockArea,
		   DockLayoutFlags flags = DOCK_LAYOUT_NONE );

  /**
   */
  template< typename TWidget, typename TController, typename TDockWidget >
    inline
    TDockWidget*
    AddDockWidget( const QString& dockName,
		   const QString& dockTitle,
		   Qt::DockWidgetArea dockArea,
		   DockLayoutFlags flags = DOCK_LAYOUT_NONE );
  /**
   */
  const AbstractModelController *
    GetController( const QDockWidget * ) const;
  /**
   */
  AbstractModelController *
    GetController( const QDockWidget * );

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
  VectorImageModel *
    ImportImage( const QString & filename,
                 int width,
                 int height );

  /**
   */
  bool BuildGDALOverviews( const QStringList & filenames );

  /**
   */
  void SaveLayout( int version ) const;

  /**
   */
  bool RestoreLayout( int version );

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
  QObject * Import( AbstractWorker * );

  /**
   */
  virtual void virtual_SetupUI() =0;

  /**
   */
  virtual void virtual_ConnectUI() =0;

  /**
   */
  virtual void virtual_InitializeUI();

  /**
   */

//
// Private attributes.
private:

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
  virtual void on_action_About_triggered();

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
#include "mvdAbstractModelController.h"

namespace mvd
{

/*****************************************************************************/
inline
const AbstractModelController *
I18nMainWindow
::GetController( const QDockWidget * dock ) const
{
  assert( dock!=NULL );

  return dock->findChild< const AbstractModelController * >();
}

/*****************************************************************************/
inline
AbstractModelController *
I18nMainWindow
::GetController( const QDockWidget * dock )
{
  assert( dock!=NULL );

  return dock->findChild< AbstractModelController * >();
}

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
template< typename TWidget, typename TDockWidget >
inline
TDockWidget*
I18nMainWindow
::AddDockWidget( const QString & dockName,
		 const QString & dockTitle,
		 Qt::DockWidgetArea dockArea,
		 DockLayoutFlags flags )
{
  TWidget * widget = new TWidget( this );

  TDockWidget * dockWidget =
    AddWidgetToDock(
      widget,
      dockName,
      dockTitle,
      dockArea,
      flags
    );

  return dockWidget;
}

/*****************************************************************************/
template< typename TWidget, typename TController, typename TDockWidget >
inline
TDockWidget*
I18nMainWindow
::AddDockWidget( const QString & dockName,
		 const QString & dockTitle,
		 Qt::DockWidgetArea dockArea,
		 DockLayoutFlags flags )
{
  TWidget * widget = new TWidget( this );

  QWidget * pannel = widget;

  if( flags.testFlag( DOCK_LAYOUT_SCROLLABLE ) )
    {
    QScrollArea * scrollArea = new QScrollArea( this );

    scrollArea->setWidgetResizable( true );

    scrollArea->setWidget( widget );

    pannel = scrollArea;
    }


  TDockWidget * dockWidget =
    AddWidgetToDock(
      pannel,
      dockName,
      dockTitle,
      dockArea,
      flags
    );

  new TController(
    // wraps:
    widget,
    // as chid of:
    dockWidget
  );

  return dockWidget;
}

} // end namespace 'mvd'

#endif // I18nMainWindow_h
