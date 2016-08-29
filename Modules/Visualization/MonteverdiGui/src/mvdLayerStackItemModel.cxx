/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
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
#include "mvdAbstractLayerModel.h"
#include "mvdStackedLayerModel.h"
#include "mvdVectorImageModel.h"
#include "mvdVisibleInterface.h"

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

const char * const
HEADERS[ LayerStackItemModel::COLUMN_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Proj" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Res" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Name" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Effect" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "I" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "J" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Red" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Green" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Blue" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "X" ),
  QT_TRANSLATE_NOOP( "mvd::LayerStackItemModel", "Y" ),
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
const AbstractLayerModel *
LayerStackItemModel
::GetLayer( const QModelIndex & index )
{
  assert( index.isValid() );
  assert( !index.parent().isValid() );

  assert(
    index.internalPointer()==
    static_cast< const AbstractLayerModel * >( index.internalPointer() )
  );

  return
    static_cast< const AbstractLayerModel * >( index.internalPointer() );
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
LayerStackItemModel
::LayerStackItemModel( QObject* p ) :
  QAbstractItemModel( p ),
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

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    QObject::connect(
      layer,
      SIGNAL( SettingsUpdated( AbstractImageModel * ) ),
      // to:
      this,
      SLOT( OnImageSettingsUpdated( AbstractImageModel * ) )
    );
    }
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
    // from:
    this,
    SLOT( OnLayerVisibilityChanged( AbstractLayerModel *, bool ) )
  );

  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
    {
    QObject::disconnect(
      layer,
      SIGNAL( SettingsUpdated( AbstractImageModel * ) ),
      // from:
      this,
      SLOT( OnImageSettingsUpdated( AbstractImageModel * ) )
    );
    }
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
      // from:
      this,
      SIGNAL( modelAboutToBeReset() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( ContentReset() ),
      // from:
      this,
      SIGNAL( modelReset() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerAdded( size_t ) ),
      // from: 
      this,
      SLOT( OnLayerAdded( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerAboutToBeDeleted( size_t ) ),
      // from: 
      this,
      SLOT( OnLayerAboutToBeDeleted( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( LayerDeleted( size_t ) ),
      // from: 
      this,
      SLOT( OnLayerDeleted( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( OrderAboutToBeChanged() ),
      // from:
      this,
      SIGNAL( layoutAboutToBeChanged() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( OrderChanged() ),
      // from:
      this,
      SIGNAL( layoutChanged() )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( ReferenceChanged( size_t ) ),
      // from:
      this,
      SLOT( OnReferenceChanged( size_t ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( PixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & ) ),
      // from:
      this,
      SLOT( OnPixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & ) )
    );

    QObject::disconnect(
      m_StackedLayerModel,
      SIGNAL( ResolutionsChanged( const PixelInfo::Vector & ) ),
      // from:
      this,
      SLOT( ResolutionsChanged( const PixelInfo::Vector & ) )
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

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( ReferenceChanged( size_t ) ),
    // to:
    this,
    SLOT( OnReferenceChanged( size_t ) )
  );

  QObject::connect(
    m_StackedLayerModel,
    SIGNAL( PixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & ) ),
    // to:
    this,
    SLOT( OnPixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & ) )
  );

    QObject::connect(
      m_StackedLayerModel,
      SIGNAL( ResolutionsChanged( const PixelInfo::Vector & ) ),
      // to:
      this,
      SLOT( OnResolutionsChanged( const PixelInfo::Vector & ) )
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
::columnCount( const QModelIndex & ) const
{
  // qDebug() << this << "::columnCount(" << parent << ")";

  return COLUMN_COUNT;
}

/*****************************************************************************/
QVariant
LayerStackItemModel
::data( const QModelIndex & idx, int role ) const
{
  // qDebug() << this << "::data(" << idx << "," << role << ")";

  // Get layer.
  assert( m_StackedLayerModel!=NULL );

  assert( idx.isValid() );
  assert( !idx.parent().isValid() );
  assert( idx.internalPointer()!=NULL );

  const AbstractLayerModel * layer =
    static_cast< const AbstractLayerModel * >( idx.internalPointer() );

  assert( layer!=NULL );

  // Return data given role.
  switch( role )
    {
    case Qt::CheckStateRole:
      if( idx.column()!=COLUMN_NAME )
        return QVariant();
      else
        {
        assert( layer==dynamic_cast< const VisibleInterface * >( layer ) );

        const VisibleInterface * interface =
          dynamic_cast< const VisibleInterface * >( layer );

        assert( interface!=NULL );

        return
          interface->IsVisible()
          ? Qt::Checked
          : Qt::Unchecked;
        }
      break;

    case Qt::DisplayRole:
      switch( idx.column() )
        {
        case COLUMN_PROJ:
	  return FromStdString( layer->GetAuthorityCode( true ) );
	  break;

	case COLUMN_RESOLUTION:
	{
	const PixelInfo::Vector & pixels = m_StackedLayerModel->PixelInfos();

	assert( idx.row()>=0 );

	if( idx.row()>=0 &&
	    static_cast< size_t >( idx.row() )<pixels.size() &&
	    pixels[ idx.row() ].m_HasResolution )
	  return
	    static_cast< qlonglong >( pixels[ idx.row() ].m_Resolution );
	else
	  return QVariant();
	}
	break;

        case COLUMN_NAME:
          if( layer->inherits(
                VectorImageModel::staticMetaObject.className() ) )
            {
            const VectorImageModel * vectorImageModel =
              qobject_cast< const VectorImageModel * >( layer );
            assert( vectorImageModel!=NULL );

            // qDebug() << "filename:" << vectorImageModel->GetFilename();

            return QFileInfo( vectorImageModel->GetFilename() ).fileName();
            }
          else
            {
            qDebug() << "Unhandled AbstractLayerModel subclass.";
            }
          break;

	case COLUMN_EFFECT:
          if( layer->inherits(
                VectorImageModel::staticMetaObject.className() ) )
            {
            const VectorImageModel * vectorImageModel =
              qobject_cast< const VectorImageModel * >( layer );
            assert( vectorImageModel!=NULL );

	    return vectorImageModel->GetSettings().GetEffectName();
            }
          else
            {
            qDebug() << "Unhandled AbstractLayerModel subclass.";
            }
	  break;

	case COLUMN_I:
	case COLUMN_J:
	{
	const PixelInfo::Vector & pixels = m_StackedLayerModel->PixelInfos();

	assert( idx.row()>=0 );

	if( idx.row()>=0 &&
	    static_cast< size_t >( idx.row() )<pixels.size() &&
	    pixels[ idx.row() ].m_HasIndex )
	  return
	    static_cast< qlonglong >( pixels[ idx.row() ].m_Index[ idx.column() - COLUMN_I ] );
	else
	  return QVariant();
	}
	break;

	case COLUMN_R:
	case COLUMN_G:
	case COLUMN_B:
	{
	const PixelInfo::Vector & pixels = m_StackedLayerModel->PixelInfos();

	assert( idx.row()>=0 );

	if( idx.row()>=0 &&
	    static_cast< size_t >( idx.row() )<pixels.size() &&
	    pixels[ idx.row() ].m_HasPixel )
	  return
	    pixels[ idx.row() ].m_Pixel[ idx.column() - COLUMN_R ];
	}
	break;

	case COLUMN_X:
	case COLUMN_Y:
	{
	const PixelInfo::Vector & pixels = m_StackedLayerModel->PixelInfos();

	assert( idx.row()>=0 );

	if( idx.row()>=0 &&
	    static_cast< size_t >( idx.row() )<pixels.size() &&
	    pixels[ idx.row() ].m_HasPoint )
	  return
	    pixels[ idx.row() ].m_Point[ idx.column() - COLUMN_X ];
	}
	  break;

	default:
	  break;
        }
      break;
    
    case Qt::FontRole:
      assert( idx.row()>=0 );

      if( static_cast< StackedLayerModel::SizeType >( idx.row() )==
	  m_StackedLayerModel->GetReferenceIndex() )
	{
	QFont font;

	font.setBold( true );

	return font;
	}
      break;

    case Qt::ToolTipRole:
      switch( idx.column() )
	{
	case COLUMN_NAME:
	  if( layer->inherits( VectorImageModel::staticMetaObject.className() ) )
	    {
            const VectorImageModel * vectorImageModel =
              qobject_cast< const VectorImageModel * >( layer );
            assert( vectorImageModel!=NULL );

            // qDebug() << "filename:" << vectorImageModel->GetFilename();

            return vectorImageModel->GetFilename();
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
::dropMimeData( const QMimeData * dat,
                Qt::DropAction action,
                int row,
                int column,
                const QModelIndex & p )
{
  // qDebug()
  //   << this
  //   << "::dropMimeData("
  //   << dat << "," << action << "," << row << "," << column << "," << parent
  //   << ")";

  // qDebug() << "QMimeData::formats():" << dat->formats();

  return QAbstractItemModel::dropMimeData( dat, action, row, column, p );
}

/*****************************************************************************/
Qt::ItemFlags
LayerStackItemModel
::flags( const QModelIndex & idx ) const
{
  if( !idx.isValid() )
    return QAbstractItemModel::flags( idx );

  Qt::ItemFlags iflags =
    QAbstractItemModel::flags( idx )
    // | Qt::ItemIsDragEnabled
    // | Qt::ItemIsDropEnabled
    ;

  if( idx.column()==COLUMN_NAME )
    iflags |=
        Qt::ItemIsUserCheckable
      | Qt::ItemIsEditable
      | Qt::ItemIsDragEnabled;

  return iflags;
}

/*****************************************************************************/
bool
LayerStackItemModel
::hasChildren( const QModelIndex & idx ) const
{
  return !idx.isValid();
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
      return tr( HEADERS[ section ] );
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
          return tr( HEADERS[ section ] );
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
         const QModelIndex & p ) const
{
  // qDebug()
  //   << this << "::index(" << row << "," << column << "," << parent << ")";

  assert( m_StackedLayerModel!=NULL );

  // qDebug()
  //   << "index:" << row << "," << column << "," << m_StackedLayerModel->At( row );

  assert( row>=0 && column>=0 );

#if 1
  AbstractLayerModel * layer = m_StackedLayerModel->At( row );

  if( layer==NULL || p.isValid() )
    return QModelIndex();
#endif

  return
    createIndex(
      row,
      column,
      p.isValid()
      ? NULL
      : m_StackedLayerModel->At( row )
    );
}

/*****************************************************************************/
bool
LayerStackItemModel
::insertRows( int row, int count, const QModelIndex & p )
{
  assert( !p.isValid() );
  assert( count==1 );

  if( p.isValid() || count!=1 )
    return false;

  beginInsertRows( p, row, row + count - 1 );
  {
  // StackedLayerModel has already been grown.

  // TODO: Allocate additionnal row data here.
  }
  endInsertRows();

  // emit dataChanged( index( row, 0 ), index( last, columnCount() - 1 ) );

  return true;
}

/*****************************************************************************/
QMimeData *
LayerStackItemModel
::mimeData( const QModelIndexList & indexes ) const
{
  QMimeData * mData = QAbstractItemModel::mimeData( indexes );
  assert( mData!=NULL );

  typedef QList< QUrl > UrlList;

  UrlList urls;

  foreach( const QModelIndex & idx, indexes )
    if( idx.isValid() )
      {
      assert( idx.internalPointer()!=NULL );

      AbstractLayerModel * layer =
	static_cast< AbstractLayerModel * >( idx.internalPointer() );

      FilenameInterface * interface =
	dynamic_cast< FilenameInterface * >( layer );

      urls << QUrl::fromLocalFile( interface->GetFilename() );
      }

  mData->setUrls( urls );

  // qDebug() << this << "mime-data:" << mData;

  return mData;
}

/*****************************************************************************/
QStringList
LayerStackItemModel
::mimeTypes() const
{
  QStringList mTypes( QAbstractItemModel::mimeTypes() );

  mTypes << "text/uri-list";

  // qDebug() << this << "mime-types:" << mTypes;

  return mTypes;
}

/*****************************************************************************/
QModelIndex
LayerStackItemModel
::parent( const QModelIndex & ) const
{
  // qDebug() << this << "::parent(" << index << ")";

  return QModelIndex();
}

/*****************************************************************************/
bool
LayerStackItemModel
::removeRows( int row, int count, const QModelIndex & p )
{
  assert( !p.isValid() );
  assert( count==1 );

  if( p.isValid() || count!=1 )
    return false;

  beginRemoveRows( p, row, row + count - 1 );
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
::rowCount( const QModelIndex & p ) const
{
  // qDebug() << this << "::rowCount(" << p << ")";

  // qDebug() << "row-count:" <<
  //   ( ( m_StackedLayerModel==NULL || p.isValid() )
  //     ? 0
  //     : m_StackedLayerModel->GetCount()
  //   );

  return
    ( m_StackedLayerModel==NULL || p.isValid() )
    ? 0
    : m_StackedLayerModel->GetCount();
}

/*****************************************************************************/
bool
LayerStackItemModel
::setData( const QModelIndex & idx,
           const QVariant & value,
           int role )
{
  // qDebug()
  //   << this << "::setData(" << idx << "," << value << "," << role << ")";

  if( idx.column()==COLUMN_NAME && role==Qt::CheckStateRole )
    {
    // qDebug() << idx.row() << "check-state:" << value;

    assert( !idx.parent().isValid() );
    assert( idx.internalPointer()!=NULL );

    assert( idx.internalPointer()!=NULL );

    AbstractLayerModel * layer =
      static_cast< AbstractLayerModel * >( idx.internalPointer() );

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

    emit dataChanged( idx, idx );

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
::OnImageSettingsUpdated( AbstractImageModel * image )
{
  // qDebug() << this << "::OnImageSettingsUpdated(" << image << ")";

  assert( m_StackedLayerModel!=NULL );

  StackedLayerModel::SizeType row = m_StackedLayerModel->IndexOf( image );

  assert( row!=StackedLayerModel::NIL_INDEX );

  emit dataChanged(
    createIndex( row, LayerStackItemModel::COLUMN_EFFECT, image ),
    createIndex( row, LayerStackItemModel::COLUMN_EFFECT, image )
  );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerAboutToBeDeleted( size_t idx )
{
  assert( m_StackedLayerModel!=NULL );

  Disconnect( m_StackedLayerModel->At( idx ) );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerAdded( size_t idx )
{
  if( !insertRow( idx ) )
    {
    assert( false && "QAbstractItemModel::insertRow() failed!" );
    return;
    }

  assert( m_StackedLayerModel!=NULL );

  Connect( m_StackedLayerModel->At( idx ) );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerDeleted( size_t idx )
{
#ifdef OTB_DEBUG
  bool isRowRemoved =
#endif

  removeRow( idx );

#ifdef OTB_DEBUG
  assert( isRowRemoved && "QAbstractItemModel::removeRow() failed!" );
#endif
}

/*****************************************************************************/
void
LayerStackItemModel
::OnLayerVisibilityChanged( AbstractLayerModel * layer, bool )
{
  // qDebug() << this << "::OnLayerVisibilityChanged(" << layer << "," << isVisible << ")"; 

  assert( m_StackedLayerModel!=NULL );
  assert( m_StackedLayerModel->IndexOf( layer )!=StackedLayerModel::NIL_INDEX );

  QModelIndex idx(
    createIndex(
      m_StackedLayerModel->IndexOf( layer ),
      COLUMN_NAME,
      layer
    )
  );

  emit dataChanged( idx, idx );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnPixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & pixels )
{
  if( pixels.empty() )
    return;

  emit dataChanged(
    index( 0, COLUMN_I ),
    index( pixels.size() - 1, COLUMN_COUNT - 1 )
  );
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
void
LayerStackItemModel
::OnReferenceChanged( size_t idx )
{
  // qDebug() << this << "::OnReferenceChanged(" << idx << ")";

  assert( m_StackedLayerModel!=NULL );

  AbstractLayerModel * layer = m_StackedLayerModel->GetReference();

  assert( LayerStackItemModel::COLUMN_COUNT>0 );

  emit dataChanged(
    createIndex( idx, 0, layer ),
    createIndex( idx, LayerStackItemModel::COLUMN_COUNT - 1, layer )
  );
}

/*****************************************************************************/
void
LayerStackItemModel
::OnResolutionsChanged( const PixelInfo::Vector & pixels )
{
  if( pixels.empty() )
    return;

  emit dataChanged(
    index( 0, COLUMN_RESOLUTION ),
    index( pixels.size() - 1, COLUMN_RESOLUTION )
  );
}

/*****************************************************************************/

} // end namespace 'mvd'
