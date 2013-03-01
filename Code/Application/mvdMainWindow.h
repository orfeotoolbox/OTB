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
class ImageViewManipulator;
class QuicklookViewManipulator;
class ImageModelRenderer;

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
  void UserCoordinatesEditingFinished(const QString&);

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
  void InitializeStatusBarWidgets();

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

  /** */
  inline const QDockWidget* GetQuicklookDock() const;

  /** */
  inline  QDockWidget* GetQuicklookDock();

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

  /** Write settings into a settings file*/
  void WriteDsPathSettings(QString dsPath);

  /** Read settings from a settings file*/
  void InitializeDsPathSettings();

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MainWindow* m_UI;

  /**
   * \brief centralView manipulator.
   */
  ImageViewManipulator*   m_ImageViewManipulator;
  
  /**
   * \brief centralView manipulator.
   */
  ImageModelRenderer*     m_ImageModelRenderer;

  /**
   * \brief ql manipulator.
   */
  QuicklookViewManipulator* m_QLViewManipulator;
  
  /**
   * \brief ql manipulator.
   */
  ImageModelRenderer*       m_QLModelRenderer;

  QLabel *                   m_CurrentPixelIndexLabel;
  QLineEdit *                m_CurrentPixelIndex;
  QLabel *                   m_CurrentPixelPhysical;
  QLabel *                   m_CurrentPixelGeographic;
  QLabel *                   m_CurrentPixelPlaceName;
  QLabel *                   m_CurrentPixelRadio;
  QLabel *                   m_CurrentPixelScale;

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
   * \brief Qt auto-connected slots which are called when View/Quicklook,
   * View/Color Setup, View/Color Dynamics check boxes are toggled.
   */
  void on_action_Quicklook_toggled();
  void on_action_ColorSetup_toggled();
  void on_action_ColorDynamics_toggled();

  /**
   * \brief Qt auto-connected slot which is called when Help/About
   * menu action is activated.
   */
  void on_action_About_activated();

  /**
   * \brief Qt auto-connected slot which is called when File/Preferences
   * menu action is activated.
   */
  void on_action_Preferences_activated();

  /** */
  void OnQuicklookVisibilityChanged ( bool );
  void OnColorSetupVisibilityChanged ( bool );
  void OnColorDynamicsVisibilityChanged ( bool );

  /** */
  void OnAboutToChangeSelectedModel( const AbstractModel* );

  /** */
  void OnSelectedModelChanged( AbstractModel* );

  /** */
  void OnUserCoordinatesEditingFinished();

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
#define QUICKLOOK_DOCK "quicklookDock"

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
const QDockWidget*
MainWindow
::GetQuicklookDock() const
{
  return findChild< QDockWidget* >( QUICKLOOK_DOCK  );
}

/*****************************************************************************/
inline
QDockWidget*
MainWindow
::GetQuicklookDock()
{
  return findChild< QDockWidget* >( QUICKLOOK_DOCK  );
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
