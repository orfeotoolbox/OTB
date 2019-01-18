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

#include "mvdTreeWidgetItem.h"


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

namespace mvd
{

/*
  TRANSLATOR mvd::TreeWidgetItem

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
int count = 0;
}

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/

// MANTIS-947: Workaround for QTreeWidget::itemAt() returning NULL for
// last item of view after DatabaseBrowserController::UpdateTree() has
// been called subsequent to insert new group.
//
// This workaround reverts the order of insertion of NODE items so
// that the last item is always at first index instead of last one.
//
// Side effect: This inverts the order for all QTreeWidget using
// mvd::TreeWidgetItem, not only those in DatabaseBrowser view.
#define BUG_WORKAROUND_MANTIS_947 1

TreeWidgetItem
::TreeWidgetItem( QTreeWidgetItem* p,
                  const QString& txt,
                  const QVariant& id,
                  const QStringList& columns,
                  TreeWidgetItem::ItemType itemType ) :
#if BUG_WORKAROUND_MANTIS_947
  QTreeWidgetItem( QStringList( txt ) << QString() << columns, itemType )
#else // BUG_WORKAROUND_MANTIS_947
  QTreeWidgetItem( p, QStringList( txt ) << QString() << columns, itemType )
#endif // BUG_WORKAROUND_MANTIS_947
{
  assert( p!=NULL );
  // parent->addChild( this );

  SetId( id );

  switch( itemType )
    {
    case TreeWidgetItem::ITEM_TYPE_NODE:
      setChildIndicatorPolicy(
        QTreeWidgetItem::ShowIndicator
      );
      setFlags(
        // Qt::ItemIsDragEnabled |
        Qt::ItemIsDropEnabled |
        Qt::ItemIsEditable |
        Qt::ItemIsEnabled
      );
#if BUG_WORKAROUND_MANTIS_947
      p->insertChild( 0, this );
#endif // BUG_WORKAROUND_MANTIS_947
    break;

    case TreeWidgetItem::ITEM_TYPE_LEAF:
      setChildIndicatorPolicy(
        QTreeWidgetItem::DontShowIndicator
      );
      setFlags(
        Qt::ItemIsDragEnabled |
        Qt::ItemIsEditable |
        Qt::ItemIsEnabled |
        Qt::ItemIsSelectable
      );
#if BUG_WORKAROUND_MANTIS_947
      p->addChild( this );
#endif // BUG_WORKAROUND_MANTIS_947
      break;

    default:
      assert( false );
      break;
    }

  /*
  qDebug()
    << "NEW Item:" << count << "\n"
    << "type:" << GetType() << "\n"
    << "id:" << GetId() << "\n"
    << "text:" << GetText() << "\n"
    << "hash:" << this->text( 2 ) << "\n"
    << "parent:" << this->parent() << "\n"
    << "text[ 0 ]:" << parent->text( 0 );
  */

  ++ count;

  qDebug() << "Item count:" << count;
}

/*******************************************************************************/
TreeWidgetItem
::~TreeWidgetItem()
{
  /*
  qDebug()
    << "DELETE Item:" << "\n"
    << "type:" << GetType() << "\n"
    << "id:" << GetId() << "\n"
    << "text:" << GetText() << "\n"
    << "hash:" << text( 2 ) << "\n"
    << "parent:" << parent() << "\n"
    << "text[ 0 ]:" << (parent()==NULL ? "" : parent()->text( 0 ));
  */

  -- count;

  qDebug() << "Item count:" << count;
}

/*******************************************************************************/
QTreeWidgetItem*
TreeWidgetItem
::clone() const
{
  qDebug() << this << "::clone()";

  return new TreeWidgetItem( *this );
}

} // end namespace 'mvd'
