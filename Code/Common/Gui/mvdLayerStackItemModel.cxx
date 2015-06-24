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
#include "Core/mvdVisibleInterface.h"

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
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "I" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "J" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Red" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Green" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Blue" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "X" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Y" ) ),
  // QVariant( QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "EPSG" ) ),
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
::Connect( AbstractLayerModel * layer )
{
  assert( layer!=NULL );

  QObject::connect(
    layer,
    SIGNAL( VisibilityChanged( AbstractLayerModel *, bool ) ),
    // to:*
    this,
    SLOT( OnLayerVisibilityChanged( AbstractLayerModel *, bool ) )
  );
}

/*****************************************************************************/
void
LayerStackItemModel
::Disconnect( AbstractLayerModel * layer )
{
  assert( layer!=NULL );

  QObject::disconnect(
    layer,
    SIGNAL( VisibilityChanged( AbstractLayerModel *,  bool ) ),
    // to:*
    this,
    SLOT( OnLayerVisibilityChanged( AbstractLayerModel *, bool ) )
  );
}

/*****************************************************************************/
void
LayerStackItemModel
::SetStack( StackedLayerModel * model )
{
  // emit layoutAboutToBeChanged();

  if( m_StackedLayerModel!=NULL )
    {
    for( StackedLayerModel::ConstIterator it( m_StackedLayerModel->Begin() );
         it!=m_StackedLayerModel->End();
         ++it )
    Disconnect( it->second );

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
      SIGNAL( LayerAdded( size_t ) ),
      // to: 
      this,
      SLOT( OnLayerAdded( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerAboutToBeDeleted( size_t ) ),
      // to: 
      this,
      SLOT( OnLayerAboutToBeDeleted( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerDeleted( size_t ) ),
      // to: 
      this,
      SLOT( OnLayerDeleted( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( OrderAboutToBeChanged() ),
      // to:
      this,
      SIGNAL( layoutAboutToBeChanged() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( OrderChanged() ),
      // to:
      this,
      SIGNAL( layoutChanged() )
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
    SIGNAL( LayerAdded( size_t ) ),
    // to: 
    this,
    SLOT( OnLayerAdded( size_t ) )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( LayerAboutToBeDeleted( size_t ) ),
    // to: 
    this,
    SLOT( OnLayerAboutToBeDeleted( size_t ) )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( LayerDeleted( size_t ) ),
    // to: 
    this,
    SLOT( OnLayerDeleted( size_t ) )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( OrderAboutToBeChanged() ),
    // to:
    this,
    SIGNAL( layoutAboutToBeChanged() )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( OrderChanged() ),
    // to:
    this,
    SIGNAL( layoutChanged() )
  );

  for( StackedLayerModel::ConstIterator it( m_StackedLayerModel->Begin() );
       it!=m_StackedLayerModel->End();
       ++it )
    Connect( it->second );

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
    case Qt::CheckStateRole:
      if( index.column()!=COLUMN_NAME )
        return QVariant();
      else
        {
        assert( !index.parent().isValid() );
        assert( index.internalPointer()!=NULL );

        AbstractLayerModel * layer =
          static_cast< AbstractLayerModel * >( index.internalPointer() );

        assert( layer!=NULL );
        assert( layer==dynamic_cast< VisibleInterface * >( layer ) );

        VisibleInterface * interface =
          dynamic_cast< VisibleInterface * >( layer );

        assert( interface!=NULL );

        return
          interface->IsVisible()
          ? Qt::Checked
          : Qt::Unchecked;
        }
      break;

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
            static_cast< AbstractLayerModel * >( index.internalPointer() );
          // m_StackedLayerModel->At( index.row() );

          assert( layerModel!=NULL );

          if( layerModel->inherits(
                VectorImageModel::staticMetaObject.className() ) )
            {
            const VectorImageModel * vectorImageModel =
              qobject_cast< const VectorImageModel * >( layerModel );
            assert( vectorImageModel!=NULL );

            // qDebug() << "filename:" << vectorImageModel->GetFilename();

            return QFileInfo( vectorImageModel->GetFilename() ).fileName();
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
bool
LayerStackItemModel
::dropMimeData( const QMimeData * data,
                Qt::DropAction action,
                int row,
                int column,
                const QModelIndex & parent )
{
  // qDebug()
  //   << this
  //   << "::dropMimeData("
  //   << data << "," << action << "," << row << "," << column << "," << parent
  //   << ")";

  // qDebug() << "QMimeData::formats():" << data->formats();

  return QAbstractItemModel::dropMimeData( data, action, row, column, parent );
}

/*****************************************************************************/
Qt::ItemFlags
LayerStackItemModel
::flags( const QModelIndex & index ) const
{
  if( !index.isValid() )
    return QAbstractItemModel::flags( index );

  Qt::ItemFlags flags =
    QAbstractItemModel::flags( index )
    // | Qt::ItemIsDragEnabled
    // | Qt::ItemIsDropEnabled
    ;

  if( index.column()==COLUMN_NAME )
    flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;

  return flags;
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

  assert( row>=0 && column>=0 );

#if 1
  AbstractLayerModel * layer = m_StackedLayerModel->At( row );

  if( layer==NULL || parent.isValid() )
    return QModelIndex();
#endif

  return
    createIndex(
      row,
      column,
      parent.isValid()
      ? NULL
      : m_StackedLayerModel->At( row )
    );
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

  if( index.column()==COLUMN_NAME && role==Qt::CheckStateRole )
    {
    // qDebug() << index.row() << "check-state:" << value;

    assert( !index.parent().isValid() );
    assert( index.internalPointer()!=NULL );

    assert( index.internalPointer()!=NULL );

    AbstractLayerModel * layer =
      static_cast< AbstractLayerModel * >( index.internalPointer() );

    assert( layer!=NULL );
    assert( layer==dynamic_cast< VisibleInterface * >( layer ) );

    VisibleInterface * interface = dynamic_cast< VisibleInterface * >( layer );
    assert( interface!=NULL );

    switch( value.toInt() )
      {
      case Qt::Checked:
        interface->SetVisible( true );
        break;

      case Qt::Unchecked:
        interface->SetVisible( false );
        break;

      default:
        assert( false && "Unhandled Qt::CheckedState value." );
        break;
      }

    emit dataChanged( index, index );

    return true;
    }

  return false;
}

/*****************************************************************************/
Qt::DropActions
LayerStackItemModel
::supportedDropActions() const
{
  // qDebug() << this << "::supportedDropActions()";

  // return Qt::MoveAction;
  return QAbstractItemModel::supportedDropActions();
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
::OnLayerAboutToBeDeleted( size_t index )
{
  assert( m_StackedLayerModel!=NULL );

  Disconnect( m_StackedLayerModel->At( index ) );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerAdded( size_t index )
{
  if( !insertRow( index ) )
    {
    assert( false && "QAbstractItemModel::insertRow() failed!" );
    return;
    }

  assert( m_StackedLayerModel!=NULL );

  Connect( m_StackedLayerModel->At( index ) );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerDeleted( size_t index )
{
#ifdef _DEBUG
  bool isRowRemoved =
#endif

  removeRow( index );

#ifdef _DEBUG
  assert( isRowRemoved && "QAbstractItemModel::removeRow() failed!" );
#endif
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerVisibilityChanged( AbstractLayerModel * layer, bool isVisible )
{
  // qDebug() << this << "::OnLayerVisibilityChanged(" << layer << "," << isVisible << ")"; 

  assert( m_StackedLayerModel!=NULL );
  assert( m_StackedLayerModel->IndexOf( layer )!=StackedLayerModel::NIL_INDEX );

  QModelIndex index(
    createIndex(
      m_StackedLayerModel->IndexOf( layer ),
      COLUMN_NAME,
      layer
    )
  );

  emit dataChanged( index, index );
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
