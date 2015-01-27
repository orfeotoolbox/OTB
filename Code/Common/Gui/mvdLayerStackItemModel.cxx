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
#include "mvdLayerStackItemModel.h"


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
#include "Core/mvdAbstractLayerModel.h"
#include "Core/mvdStackedLayerModel.h"
#include "Core/mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::LayerStackItemModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{

QVariant
HEADERS[ LayerStackItemModel::COLUMN_COUNT ] =
{
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Index" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Name" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "I" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "J" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Red" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Green" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Blue" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "X" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Y" ) ),
  QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "EPSG" ) ),
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
LayerStackItemModel
::LayerStackItemModel( QObject* parent ) :
  QAbstractItemModel( parent ),
  m_StackedLayerModel( NULL )
{
  // QObject::connect(
  //   this,
  //   SIGNAL( modelAboutToBeReset() ),
  //   // to:
  //   this,
  //   SLOT( OnModelAboutToBeReset() )
  // );

  // QObject::connect(
  //   this,
  //   SIGNAL( modelReset() ),
  //   // to:
  //   this,
  //   SLOT( OnModelReset() )
  // );
}

/*****************************************************************************/
LayerStackItemModel
::~LayerStackItemModel()
{
}

/*****************************************************************************/
void
LayerStackItemModel
::SetStack( StackedLayerModel * model )
{
  // emit layoutAboutToBeChanged();

  if( m_StackedLayerModel!=NULL )
    {
    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( ContentAboutToBeReset() ),
      // to:
      this,
      SIGNAL( modelAboutToBeReset() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( ContentReset() ),
      // to:
      this,
      SIGNAL( modelReset() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerAdded( unsigned int ) ),
      // to: 
      this,
      SLOT( OnLayerAdded( unsigned int ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerDeleted( unsigned int ) ),
      // to: 
      this,
      SLOT( OnLayerDeleted( unsigned int ) )
    );
    }

  m_StackedLayerModel = model;

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( ContentAboutToBeReset() ),
    // to:
    this,
    SIGNAL( modelAboutToBeReset() )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( ContentReset() ),
    // to:
    this,
    SIGNAL( modelReset() )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( LayerAdded( unsigned int ) ),
    // to: 
    this,
    SLOT( OnLayerAdded( unsigned int ) )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( LayerDeleted( unsigned int ) ),
    // to: 
    this,
    SLOT( OnLayerDeleted( unsigned int ) )
  );

  // emit layoutChanged();
}

/*****************************************************************************/
/* QAbstractItemModel overloads                                              */
/*****************************************************************************/
int
LayerStackItemModel
::columnCount( const QModelIndex & parent ) const
{
  // qDebug() << this << "::columnCount(" << parent << ")";

  return COLUMN_COUNT;
}

/*****************************************************************************/
QVariant
LayerStackItemModel
::data( const QModelIndex & index, int role ) const
{
  // qDebug() << this << "::data(" << index << "," << role << ")";

  switch( role )
    {
    case Qt::DisplayRole:
      switch( index.column() )
        {
        case COLUMN_INDEX:
          // qDebug() << "index:" << index.row();
          return index.row();
          break;

        case COLUMN_NAME:
          assert( m_StackedLayerModel!=NULL );

          const AbstractLayerModel * layerModel =
            m_StackedLayerModel->At( index.row() );

          assert( layerModel!=NULL );

          if( layerModel->inherits(
                VectorImageModel::staticMetaObject.className() ) )
            {
            const VectorImageModel * vectorImageModel =
              qobject_cast< const VectorImageModel * >( layerModel );
            assert( vectorImageModel!=NULL );

            // qDebug() << "filename:" << vectorImageModel->GetFilename();

            return vectorImageModel->GetFilename();
            }
          else
            {
            qDebug() << "Unhandled AbstractLayerModel subclass.";
            }
          break;
        }
      break;

    default:
      break;
    }

  return QVariant();
}

/*****************************************************************************/
Qt::ItemFlags
LayerStackItemModel
::flags( const QModelIndex & index ) const
{
  // qDebug() << this << "::flags(" << index << ")";

  /*
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  */

  return QAbstractItemModel::flags( index );
}

/*****************************************************************************/
bool
LayerStackItemModel
::hasChildren( const QModelIndex & index ) const
{
  return !index.isValid();
}

/*****************************************************************************/
QVariant
LayerStackItemModel
::headerData( int section,
              Qt::Orientation orientation,
              int role ) const
{
  // qDebug()
  //   << this << "::headerData("
  //   << section << "," << orientation << "," << role
  //   << ")";

  assert( orientation==Qt::Horizontal );

  switch( role )
    {
    case Qt::DisplayRole:
      assert( section>=0 && section<COLUMN_COUNT );
      return HEADERS[ section ];
      break;

    default:
      break;
    }

  /*
  switch( orientation )
    {
    case Qt::Horizontal:
      switch( role )
        {
        case Qt::DisplayRole:
          assert( section>=0 && section<COLUMN_COUNT );
          return HEADERS[ section ];
          break;
        default:
          break;
        }
      break;

    case Qt::Vertical:
      break;

    default:
      assert( false && "Unhandled Qt::Orientation value." );
      break;
    }
  */

  return QVariant();
}

/*****************************************************************************/
QModelIndex
LayerStackItemModel
::index( int row,
         int column,
         const QModelIndex & parent ) const
{
  // qDebug()
  //   << this << "::index(" << row << "," << column << "," << parent << ")";

  assert( m_StackedLayerModel!=NULL );

  // qDebug()
  //   << "index:" << row << "," << column << "," << m_StackedLayerModel->At( row );

  return createIndex( row, column, m_StackedLayerModel->At( row ) );
}

/*****************************************************************************/
bool
LayerStackItemModel
::insertRows( int row, int count, const QModelIndex & parent )
{
  assert( !parent.isValid() );
  assert( count==1 );

  if( parent.isValid() || count!=1 )
    return false;

  beginInsertRows( parent, row, row + count - 1 );
  {
  // StackedLayerModel has already been grown.

  // TODO: Allocate additionnal row data here.
  }
  endInsertRows();

  // emit dataChanged( index( row, 0 ), index( last, columnCount() - 1 ) );

  return true;
}

/*****************************************************************************/
QModelIndex
LayerStackItemModel
::parent( const QModelIndex & index ) const
{
  // qDebug() << this << "::parent(" << index << ")";

  return QModelIndex();
}

/*****************************************************************************/
bool
LayerStackItemModel
::removeRows( int row, int count, const QModelIndex & parent )
{
  assert( !parent.isValid() );
  assert( count==1 );

  if( parent.isValid() || count!=1 )
    return false;

  beginRemoveRows( parent, row, row + count - 1 );
  {
  // StackedLayerModel has already been shortened.

  // TODO: Release additionnal row data here.
  }
  endRemoveRows();

  return true;
}

/*****************************************************************************/
int
LayerStackItemModel
::rowCount( const QModelIndex & parent ) const
{
  // qDebug() << this << "::rowCount(" << parent << ")";

  // qDebug() << "row-count:" <<
  //   ( ( m_StackedLayerModel==NULL || parent.isValid() )
  //     ? 0
  //     : m_StackedLayerModel->GetCount()
  //   );

  return
    ( m_StackedLayerModel==NULL || parent.isValid() )
    ? 0
    : m_StackedLayerModel->GetCount();
}

/*****************************************************************************/
bool
LayerStackItemModel
::setData( const QModelIndex & index,
           const QVariant & value,
           int role )
{
  // qDebug()
  //   << this << "::setData(" << index << "," << value << "," << role << ")";

  return false;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
// void
// LayerStackItemModel
// ::OnContentAboutToBeChanged()
// {
//   qDebug() << this << "::OnAboutContentChanged()";

//   emit layoutAboutToBeChanged();
// }

/*****************************************************************************/
// void
// LayerStackItemModel
// ::OnContentChanged()
// {
//   qDebug() << this << "::OnContentChanged()";

//   // assert( m_StackedLayerModel!=NULL );

//   // beginResetModel();
//   // {
//   // }
//   // endResetModel();

//   // if( m_StackedLayerModel->GetCount()>0 )
//   //   emit dataChanged(
//   //     index( 0, 0 ),
//   //     index( m_StackedLayerModel->GetCount() - 1, COLUMN_COUNT -1 )
//   //   );

//   emit layoutChanged();
// }

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerAdded( unsigned int index )
{
  insertRow( index );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerDeleted( unsigned int index )
{
  removeRow( index );
}

/*****************************************************************************/
// void
// LayerStackItemModel
// ::OnModelAboutToBeReset()
// {
//   qDebug() << this << "::OnModelAboutToBeReset()";
// }

/*****************************************************************************/
// void
// LayerStackItemModel
// ::OnModelReset()
// {
//   qDebug() << this << "::OnModelReset()";
// }

/*****************************************************************************/

} // end namespace 'mvd'
