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
#include "Gui/mvdTreeWidget.h"

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
::TreeWidgetItemDragAndDropEventFilter( QObject* parent  ) :
  AbstractDragAndDropEventFilter( parent )
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
::DragEnterEvent( QObject* object, QDragEnterEvent* event )
{
  //
  // Bypass event its MIME data does not contain not URL(s).
  if( !event->mimeData()->hasFormat( TREE_WIDGET_ITEM_MIME_TYPE ) )
    return false;

  //
  // Accept event if its MIME data contains some URL(s) and they are
  // all local filenames.
  event->acceptProposedAction();

  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DragLeaveEvent( QObject* object, QDragLeaveEvent* event )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DragMoveEvent( QObject* object, QDragMoveEvent* event )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
TreeWidgetItemDragAndDropEventFilter
::DropEvent( QObject* object, QDropEvent* event )
{
  assert( event!=NULL );
  assert( event->mimeData()!=NULL );


  //
  // Decode MIME data.

  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

  QTreeWidgetItemList items;

  int count = DecodeMimeData( items, event->mimeData() );

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
