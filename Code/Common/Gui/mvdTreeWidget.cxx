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
#include "mvdTreeWidget.h"


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
#include "Core/mvdAlgorithm.h"
#include "Core/mvdDataStream.h"
#include "Gui/mvdTreeWidgetItem.h"

namespace mvd
{

/*
  TRANSLATOR mvd::TreeWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */


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
TreeWidget
::TreeWidget( QWidget* parent  ):
  QTreeWidget( parent )
{
  setDefaultDropAction( Qt::MoveAction );

  setDragEnabled( true );

  setAcceptDrops( true );
}

/*******************************************************************************/
TreeWidget
::~TreeWidget()
{
}

/*******************************************************************************/
QStringList
TreeWidget
::mimeTypes() const
{
  // qDebug() << this << "::mimeTypes()";

  QStringList mimeTypes( QTreeWidget::mimeTypes() );

  mimeTypes << "application/x-qtreewidgetitemptrlist";

  return mimeTypes;
}

/*******************************************************************************/
QMimeData*
TreeWidget
::mimeData( const QList< QTreeWidgetItem* > items ) const
{
  // qDebug() << this << "::mimeData(" << items << ")";

  QMimeData* mimeData = QTreeWidget::mimeData( items );

  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

  QByteArray byteArray;
  QDataStream stream( &byteArray, QIODevice::WriteOnly );

  for( QTreeWidgetItemList::const_iterator it( items.begin() );
       it!=items.end();
       ++it )
    {
    /*
    qDebug()
      << "QTreeWidgetItem::parent()==" << ( *it )->parent();
    qDebug()
      << "Pointer:" << static_cast< void* >( *it );
    qDebug()
      << "Variant:" << QVariant::fromValue< QTreeWidgetItem* >( *it );
    */

    // http://www.qtfr.org/viewtopic.php?id=9630
    // stream << *it;
    stream << QVariant::fromValue< QTreeWidgetItem* >( *it );
    }

  mimeData->setData( "application/x-qtreewidgetitemptrlist", byteArray );

  /*
  qDebug() << mimeData->formats();

  for( QTreeWidgetItemList::const_iterator it( items.begin() );
       it!=items.end();
       ++it )
    {
    QTreeWidgetItem* item = *it;

    qDebug()
      << item->type() << item->text( 0 ) << item->text( 1 ) << item->text( 2 );
    }
  */

  return mimeData;
}

/*******************************************************************************/

/*******************************************************************************/
void 
TreeWidget
::dropEvent( QDropEvent* event )
{
  // qDebug() << this << "::dropEvent(" << event << ")";

  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

  QTreeWidgetItemList items;

  if( event->mimeData()->hasFormat( "application/x-qtreewidgetitemptrlist" ) )
    {
    QByteArray byteArray(
      event->mimeData()->data( "application/x-qtreewidgetitemptrlist" )
    );

    QDataStream stream( &byteArray, QIODevice::ReadOnly );

    int count = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    QTreeWidgetItem* item = NULL;

    while( !stream.atEnd() )
      {
      QVariant variant;

      stream >> variant;

      // qDebug() << "Variant:" << variant;

      // http://www.qtfr.org/viewtopic.php?id=9630

      item = variant.value< QTreeWidgetItem* >();
      assert( item!=NULL );

      items.push_back( item );

      // qDebug()
      //   << "Item (variant):"
      //   << varItem
      //   << varItem->text( 0 )
      //   << varItem->text( 1 )
      //   << varItem->text( 2 )
      //   << varItem->parent();

      ++ count;
      }

    // qDebug() << count2 << "items.";
    }

  QTreeWidget::dropEvent( event );

  for( QTreeWidgetItemList::const_iterator it = items.begin();
       it!=items.end();
       ++it )
    {
      switch( defaultDropAction() )
        {
        case Qt::MoveAction:
          emit ItemMoved( *it );
          break;

        default:
          break;
        }
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

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
