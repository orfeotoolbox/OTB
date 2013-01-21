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

namespace Ui
{
class MainWindow;
}


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class MainWindow
 *
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
  /** Constructor */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /** Destructor */
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
  void AddWidgetToDock( QWidget* widget,
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


//
// Private attributes.
private:
  /**
   */
  Ui::MainWindow* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private attributes.
private slots:
  /** */
  void on_action_Open_activated();

  /** */
  void on_action_About_activated();

  /** */
  void OnAboutToChangeSelectedModel( const AbstractModel* );

  /** */
  void OnSelectedModelChanged( const AbstractModel* );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

} // end namespace 'mvd'

#endif // __MainWindow_h
