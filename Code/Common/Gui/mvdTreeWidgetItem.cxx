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
#if BYPASS_MOUSE_EVENTS
        Qt::ItemIsDragEnabled |
#endif
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
    << "+Item:"
    << GetType()
    << GetId()
    << GetText()
    << this->text( 2 );
  */
}

/*******************************************************************************/
TreeWidgetItem
::~TreeWidgetItem()
{
}

/*******************************************************************************/
QTreeWidgetItem*
TreeWidgetItem
::clone()
{
  qDebug() << this << "::clone()";

  return new TreeWidgetItem( *this );
}

} // end namespace 'mvd'
