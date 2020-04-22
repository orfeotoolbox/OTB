/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdAbstractDragAndDropEventFilter_h
#define mvdAbstractDragAndDropEventFilter_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

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
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractDragAndDropEventFilter
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT AbstractDragAndDropEventFilter : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Destructor. */
  ~AbstractDragAndDropEventFilter() override;

  //
  // QObject overloads.

  /**
   * \see http://qt-project.org/doc/qt-4.8/qobject.html#eventFilter
   */
  bool eventFilter(QObject* watched, QEvent* event) override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** \brief Constructor. */
  AbstractDragAndDropEventFilter(QObject* p = NULL);

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragEnterEvent
   */
  virtual bool DragEnterEvent(QObject* object, QDragEnterEvent* event) = 0;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragLeaveEvent
   */
  virtual bool DragLeaveEvent(QObject* object, QDragLeaveEvent* event) = 0;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragMoveEvent
   */
  virtual bool DragMoveEvent(QObject* object, QDragMoveEvent* event) = 0;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dropEvent
   */
  virtual bool DropEvent(QObject* object, QDropEvent* event) = 0;

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  //
  // Private attributes.
private:
  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdAbstractDragAndDropEventFilter_h
