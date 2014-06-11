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
#include "Gui/mvdTreeWidgetItem.h"


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
TreeWidgetItem
::TreeWidgetItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id,
                  const QStringList& columns,
                  TreeWidgetItem::ItemType type ) :
  QTreeWidgetItem( parent, QStringList( text ) << QString() << columns, type )
{
  SetId( id );

  switch( type )
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
