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

#ifndef __mvdMainWindow_h
#define __mvdMainWindow_h

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
#include "mvdTypes.h"


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

namespace Ui
{
class MainWindow;
}


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class MainWindow
 *
 * \brief The application main-widow widget.
 */
class Monteverdi2_EXPORT MainWindow
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
  /** \brief Constructor. */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~MainWindow();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:
  void LargestPossibleRegionChanged(const ImageRegionType& largestRegion);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /** */
  void Initialize();

  /** */
  void InitializeDockWidgets();

  /** */
  QDockWidget*
    AddWidgetToDock( QWidget* widget,
		     const QString& dockName,
		     const QString& dockTitle,
		     Qt::DockWidgetArea dockArea );

  /** */
  inline const QDockWidget*  GetColorSetupDock() const;

  /** */
  inline QDockWidget* GetColorSetupDock();

  /** */
  inline const QDockWidget* GetColorDynamicsDock() const;

  /** */
  inline QDockWidget* GetColorDynamicsDock();

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


//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MainWindow* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private slots.
private slots:
  /**
   * \brief Qt auto-connected slot which is called when File/Load menu
   * action is activated.
   */
  void on_action_Open_activated();

  /**
   * \brief Qt auto-connected slot which is called when Help/About
   * menu action is activated.
   */
  void on_action_About_activated();

  /** */
  void OnAboutToChangeSelectedModel( const AbstractModel* );

  /** */
  void OnSelectedModelChanged( AbstractModel* );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModelController.h"

//
// Some constants.
#define VIDEO_COLOR_DYNAMICS_DOCK "videoColorDynamicsDock"
#define VIDEO_COLOR_SETUP_DOCK "videoColorSetupDock"

namespace mvd
{

/*****************************************************************************/
inline
const QDockWidget*
MainWindow
::GetColorSetupDock() const
{
  return findChild< QDockWidget* >( VIDEO_COLOR_SETUP_DOCK );
}

/*****************************************************************************/
inline
QDockWidget*
MainWindow
::GetColorSetupDock()
{
  return findChild< QDockWidget* >( VIDEO_COLOR_SETUP_DOCK );
}

/*****************************************************************************/
inline
const QDockWidget*
MainWindow
::GetColorDynamicsDock() const
{
  return findChild< QDockWidget* >( VIDEO_COLOR_DYNAMICS_DOCK );
}

/*****************************************************************************/
inline
QDockWidget*
MainWindow
::GetColorDynamicsDock()
{
  return findChild< QDockWidget* >( VIDEO_COLOR_DYNAMICS_DOCK );
}

/*****************************************************************************/
inline
void
MainWindow
::SetControllerModel( QDockWidget* dock, AbstractModel* model )
{
  assert( dock );

  AbstractModelController* controller =
    dock->findChild< AbstractModelController* >();

  assert( controller!=NULL );

  controller->SetModel( model );
}

} // end namespace 'mvd'

#endif // __MainWindow_h
