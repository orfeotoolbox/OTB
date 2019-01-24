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

#include "mvdTreeWidgetItemDragAndDropEventFilter.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTreeWidget.h"

namespace mvd
{

/*
  TRANSLATOR mvd::TreeWidgetItemDragAndDropEventFilter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* TREE_WIDGET_ITEM_MIME_TYPE = "application/x-qtreewidgetitemptrlist";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
TreeWidgetItemDragAndDropEventFilter
::TreeWidgetItemDragAndDropEventFilter( QObject* p  ) :
  AbstractDragAndDropEventFilter( p )
{
}

/*****************************************************************************/
TreeWidgetItemDragAndDropEventFilter
::~TreeWidgetItemDragAndDropEventFilter()
{
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DragEnterEvent( QObject *, QDragEnterEvent* e )
{
  //
  // Bypass event its MIME data does not contain not URL(s).
  if( !e->mimeData()->hasFormat( TREE_WIDGET_ITEM_MIME_TYPE ) )
    return false;

  //
  // Accept event if its MIME data contains some URL(s) and they are
  // all local filenames.
  e->acceptProposedAction();

  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DragLeaveEvent( QObject *, QDragLeaveEvent * )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DragMoveEvent( QObject *, QDragMoveEvent * )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DropEvent( QObject *, QDropEvent * e )
{
  assert( e!=NULL );
  assert( e->mimeData()!=NULL );


  //
  // Decode MIME data.

  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

  QTreeWidgetItemList items;

  int count = DecodeMimeData( items, e->mimeData() );

  if( count==0 )
    return false;

  //
  // Emit signals.

  for( QTreeWidgetItemList::const_iterator it = items.begin();
       it!=items.end();
       ++it )
    {
    emit ItemDropped( *it );
    }

  //
  // Eatup event.

  return true;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
