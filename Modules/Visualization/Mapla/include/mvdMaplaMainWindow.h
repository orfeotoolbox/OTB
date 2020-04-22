/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef mvdMaplaMainWindow_h
#define mvdMaplaMainWindow_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"
//
#include "mvdI18nMainWindow.h"

#include "OTBMaplaExport.h"

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
// Gui
class ApplicationsToolBoxController;

namespace Ui
{
class MaplaMainWindow;
}

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class MaplaMainWindow
 *
 * \ingroup OTBMapla
 *
 * \brief The application main-widow widget.
 */
class OTBMapla_EXPORT MaplaMainWindow : public I18nMainWindow
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
  MaplaMainWindow(QWidget* Parent = 0, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~MaplaMainWindow() override;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // QMainWindow methods.

  void closeEvent(QCloseEvent* event) override;

  //
  // Protected attributes.
protected:
  /*-[ PROTECTED SLOTS SECTION ]---------------------------------------------*/

  //
  // Protected slots.
protected slots:

  /**
   */
  void OnAboutToChangeModel(const AbstractModel*) override;

  /**
   */
  void OnModelChanged(AbstractModel*) override;

  /**
   */
  void OnApplicationToLaunchSelected(const QString& name);

/**
 */
#if 0
  void OnOTBApplicationOutputImageChanged( const QString & name,
                                           const QString & filename);
#endif
  /**
   */
  void OnExecutionDone(int status);

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types.
private:
  //
  // Private methods.
private:
  /**
   */
  void InitializeDockWidgets();

  /**
   */
  void InitializeCentralWidget();

  //
  // I18nMainWindow methods.

  void virtual_SetupUI() override;

  void virtual_ConnectUI() override;

  //
  // Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MaplaMainWindow* m_UI;

#ifdef OTB_USE_QT
  /**
   *  \brief OTB-applications tool-box controller.
   */
  ApplicationsToolBoxController* m_ApplicationsToolBoxController;
#endif

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Private slots.
private slots:
  /**
   * \brief Qt auto-connected slot which is called when
   * 'File/Preferences' menu action is trigerred.
   */
  void on_action_Preferences_triggered();
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
#include "mvdImageViewWidget.h"

//
// Some constants.

namespace mvd
{

} // end namespace 'mvd'

#endif // maplaMainWindow_h
