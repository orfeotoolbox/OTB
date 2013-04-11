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
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdTypes.h"
//
#include "Gui/mvdI18nMainWindow.h"


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
// Core
class DatasetModel;
// Gui
class GLImageWidget;

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
  : public I18nMainWindow
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
  
  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:


//
// Protected attributes.
protected:

  /*-[ PROTECTED SLOTS SECTION ]---------------------------------------------*/

//
// Protected slots.
protected slots:

  /**
   */
  void OnAboutToChangeModel( const AbstractModel* );

  /**
   */
  void OnModelChanged( AbstractModel* );

  /**
   */
  void OnAboutToChangeSelectedDatasetModel( const DatasetModel* );

  /**
   */
  void OnSelectedDatasetModelChanged( DatasetModel* );

  /**
   */
  void OnApplicationToLaunchSelected( const QString & appName );

  /** */
  void OntabCloseRequested(int index);

  /** */
  void OntabCloseRequested();

  /** */
  void OnOTBApplicationOutputImageChanged( const QString & appName, const QString & outfname);

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:


//
// Private methods.
private:

  /**
   */
  GLImageWidget* CreateQuicklookWidget( QGLWidget* sharedGlWidget =NULL );

  /**
   */
  void InitializeDockWidgets();

  /**
   */
  void InitializeCentralWidget();

  /**
   */
  inline
    const GLImageWidget* GetQuicklookView() const;

  /**
   */
  inline
    GLImageWidget* GetQuicklookView();

  //
  // I18nMainWindow overrides.

  virtual void virtual_SetupUI();

  virtual void virtual_ConnectUI();

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MainWindow* m_UI;

  /**
   * \brief Color dynamicsk-view dock-widget.
   */
  QDockWidget* m_ColorDynamicsDock;

  /**
   * \brief Color setup dock-widget.
   */
  QDockWidget* m_ColorSetupDock;

  /**
   * \brief Database-browser dock-widget.
   */
  QDockWidget* m_DatabaseBrowserDock;

  /**
   * \brief Dataset-properties dock-widget.
   */
  QDockWidget* m_DatasetPropertiesDock;

#ifdef OTB_WRAP_QT
  /**
   *  \brief OTB-applications browser dock-widget.
   */
  QDockWidget* m_OtbApplicationsBrowserDock;
#endif

  /**
   * \brief Color dynamics dock-widget.
   */
  QDockWidget* m_QuicklookViewDock;

  /**
   */
  QTabWidget* m_CentralTabWidget;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private slots.
private slots:

#if 0

  /**
   * \brief Qt auto-connected slot which is called when 'Color-setup'
   * action is toggled.
   */
  void on_action_ColorSetup_toggled( bool );

  /**
   * \brief Qt auto-connected slot which is called when 'Database
   * browser' action is toggled.
   */
  void on_action_DatabaseBrowser_toggled();

  /**
   * \brief Qt auto-connected slot which is called when 'Dataset
   * properties' action is toggled.
   */
  void on_action_DatasetProperties_toggled();

  /**
   * \brief Qt auto-connected slot which is called when 'OTB
   * Applications browser' action is toggled.
   */
  void on_action_OtbApplicationsBrowser_toggled();

  /**
   * \brief Qt auto-connected slot which is called when 'Quicklook
   * view' action is toggled.
   */
  void on_action_QuicklookView_toggled();

  /**
   * \brief Qt auto-connected slot which is called when
   * 'Color-dynamics' action is toggled.
   */
  void on_action_ColorDynamics_toggled();

#endif

#if 0

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnColorDynamicsVisibilityChanged( bool visible );

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnColorSetupVisibilityChanged( bool visible );

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnDatabaseBrowserVisibilityChanged( bool visible );

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnDatasetPropertiesVisibilityChanged( bool visible );

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnOtbApplicationsBrowserVisibilityChanged( bool visible );

  /**
   * \brief Slot called when the visibility of related QDockWidget has
   * changed.
   *
   * \param visible Visibility state flag.
   */
  void OnQuicklookViewVisibilityChanged( bool visible );
#endif
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
#include "Gui/mvdGLImageWidget.h"

//
// Some constants.

namespace mvd
{

/*****************************************************************************/
inline
const GLImageWidget*
MainWindow
::GetQuicklookView() const
{
  return qobject_cast< const GLImageWidget* >( m_QuicklookViewDock->widget() );
}

/*****************************************************************************/
inline
GLImageWidget*
MainWindow
::GetQuicklookView()
{
  return qobject_cast< GLImageWidget* >( m_QuicklookViewDock->widget() );
}

} // end namespace 'mvd'

#endif // __MainWindow_h
