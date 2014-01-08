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
#include "Core/mvdDataStream.h"

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

class StaticInitializer
{
public:
  StaticInitializer() :
    m_QTreeWidgetItemPtrMetaTypeId( -1 )
  {
    Initialize();
  }

  ~StaticInitializer()
  {
    Finalize();
  }

private:
  inline
  void
  Initialize()
  {
    //
    // Call qRegisterMetaType<>() to make type available in
    // non-template signatures and serialization.
    StaticInitializer::m_QTreeWidgetItemPtrMetaTypeId =
      qRegisterMetaType< QTreeWidgetItem* >( "QTreeWidgetItem *" );

    //
    // Register serialization operators for custom meta-types.
    qRegisterMetaTypeStreamOperators< QTreeWidgetItem* >();
  }

  inline
  void
  Finalize()
  {
  }

  int m_QTreeWidgetItemPtrMetaTypeId;
};

namespace
{
static const StaticInitializer STATIC_INITIALIZER;
}

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
        QTreeWidgetItem::DontShowIndicatorWhenChildless
      );
      setFlags(
        Qt::ItemIsEnabled |
#if BYPASS_MOUSE_EVENTS
        // Qt::ItemIsEditable |
        Qt::ItemIsDragEnabled |
#endif
        Qt::ItemIsDropEnabled
      );
    break;

    case TreeWidgetItem::ITEM_TYPE_LEAF:
      setChildIndicatorPolicy(
        QTreeWidgetItem::DontShowIndicator
      );
      setFlags(
        Qt::ItemIsEnabled |
        Qt::ItemIsSelectable |
        // Qt::ItemIsEditable |
        Qt::ItemIsDragEnabled
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

/*****************************************************************************/
/* GLOBAL FUNCTIONS IMPLEMENTATION SECTION                                   */

#if TREE_WIDGET_ITEM_USE_STREAM_OPERATORS

/*****************************************************************************/
QDataStream&
operator << ( QDataStream& out, QTreeWidgetItem const * item )
{
  /*
  qDebug() <<
    "QDataStream& operator << ( QDataStream&, QTreeWidgetItem const * & );";
  */

#if DATA_STREAM_USE_TEMPLATE_OPERATORS
  return operator << < QTreeWidgetItem >( out, item );

#else // DATA_STREAM_USE_TEMPLATE_OPERATORS
  DATA_STREAM_OUT( out, QTreeWidgetItem, item );

  return out;

#endif // DATA_STREAM_USE_TEMPLATE_OPERATORS
}

/*****************************************************************************/
QDataStream&
operator >>( QDataStream& in, QTreeWidgetItem * & item )
{
  /*
  qDebug() <<
    "QDataStream& operator >> ( QDataStream&, QTreeWidgetItem * & );";
  */

#if DATA_STREAM_USE_TEMPLATE_OPERATORS
  return operator >> < QTreeWidgetItem >( in, item );

#else // DATA_STREAM_USE_TEMPLATE_OPERATORS
  DATA_STREAM_IN( in, QTreeWidgetItem, item );

  return in;

#endif // DATA_STREAM_USE_TEMPLATE_OPERATORS
}

#endif // TREE_WIDGET_ITEM_USE_STREAM_OPERATORS
