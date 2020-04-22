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

#ifndef mvdFilenameDragAndDropEventFilter_h
#define mvdFilenameDragAndDropEventFilter_h

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
#include "mvdAbstractDragAndDropEventFilter.h"

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
 * \class FilenameDragAndDropEventFilter
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT FilenameDragAndDropEventFilter : public AbstractDragAndDropEventFilter
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Constructor. */
  FilenameDragAndDropEventFilter(QObject* p = NULL);

  /** \brief Destructor. */
  ~FilenameDragAndDropEventFilter() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /**
   */
  void FilenameDropped(const QString& filename);

  /**
   */
  void FilenamesDropped(const QStringList&);


  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragEnterEvent
   */
  bool DragEnterEvent(QObject* object, QDragEnterEvent* event) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragLeaveEvent
   */
  bool DragLeaveEvent(QObject* object, QDragLeaveEvent* event) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragMoveEvent
   */
  bool DragMoveEvent(QObject* object, QDragMoveEvent* event) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dropEvent
   */
  bool DropEvent(QObject* object, QDropEvent* event) override;

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

#endif // mvdFilenameDragAndDropEventFilter_h
