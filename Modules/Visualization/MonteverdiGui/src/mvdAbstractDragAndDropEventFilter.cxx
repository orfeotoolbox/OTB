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

#include "mvdAbstractDragAndDropEventFilter.h"


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

namespace mvd
{

/*
  TRANSLATOR mvd::AbstractDragAndDropEventFilter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
AbstractDragAndDropEventFilter::AbstractDragAndDropEventFilter(QObject* p) : QObject(p)
{
}

/*****************************************************************************/
AbstractDragAndDropEventFilter::~AbstractDragAndDropEventFilter()
{
}

/*****************************************************************************/
bool AbstractDragAndDropEventFilter::eventFilter(QObject* object, QEvent* e)
{
  switch (e->type())
  {
  case QEvent::DragEnter:
    return DragEnterEvent(object, dynamic_cast<QDragEnterEvent*>(e));
    break;

  case QEvent::DragMove:
    return DragMoveEvent(object, dynamic_cast<QDragMoveEvent*>(e));
    break;

  case QEvent::DragLeave:
    return DragLeaveEvent(object, dynamic_cast<QDragLeaveEvent*>(e));
    break;

  case QEvent::Drop:
    return DropEvent(object, dynamic_cast<QDropEvent*>(e));
    break;

  default:
    break;
  }

  return QObject::eventFilter(object, e);
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
