/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperQtWidgetListEditItemModel.h"


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

#include "otbWrapperStringListInterface.h"


namespace otb
{

namespace Wrapper
{

/*
  TRANSLATOR otb::Wapper::ListEditItemModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{

const char * const
HEADERS[ ListEditItemModel::COLUMN_COUNT ] =
{
  QT_TRANSLATE_NOOP( "otb::Wrapper::ListEditItemModel", "Name" ),
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ListEditItemModel
::ListEditItemModel( QObject * p ) :
  QAbstractItemModel( p ),
  m_StringList( nullptr )
{
}

/*******************************************************************************/
ListEditItemModel
::~ListEditItemModel()
{
}

/*******************************************************************************/
void
ListEditItemModel
::SetStringList( StringListInterface * stringList )
{
  m_StringList = stringList;
}

/*****************************************************************************/
/* QAbstractItemModel overloads                                              */
/*****************************************************************************/
int
ListEditItemModel
::columnCount( const QModelIndex & ) const
{
  // qDebug() << this << "::columnCount(" << parent << ")";

  return COLUMN_COUNT;
}

/*****************************************************************************/
QVariant
ListEditItemModel
::data( const QModelIndex & idx, int role ) const
{
  // qDebug() << this << "::data(" << idx << "," << role << ")";

  // Get layer.
  assert( m_StringList!=NULL );

  assert( idx.isValid() );
  assert( !idx.parent().isValid() );
  assert( idx.internalPointer()!=NULL );

  const StringListInterface * stringList =
    static_cast< const StringListInterface * >( idx.internalPointer() );

  assert( stringList!=nullptr );

  // Return data given role.
  switch( role )
    {
    case Qt::CheckStateRole:
      if( idx.column()!=COLUMN_NAME )
        return QVariant();
      else
	{
	assert( idx.row() >= 0 );
	return stringList->IsActive( idx.row() );
	}
      break;

    case Qt::DisplayRole:
      switch( idx.column() )
        {
        case COLUMN_NAME:
	  assert( idx.row() >= 0 );
	  return
	    QFile::decodeName(
	      stringList->GetNthFileName( idx.row() ).c_str()
	    );
          break;

	default:
	  break;
        }
      break;

    case Qt::FontRole:
      break;

    case Qt::ToolTipRole:
      switch( idx.column() )
	{
	case COLUMN_NAME:
	  assert( idx.row() >= 0 );
	  return QString::fromStdString( stringList->GetToolTip( idx.row() ) );
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
ListEditItemModel
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
      // | Qt::ItemIsDragEnabled
      ;

  return iflags;
}

/*****************************************************************************/
bool
ListEditItemModel
::hasChildren( const QModelIndex & idx ) const
{
  return !idx.isValid();
}

/*****************************************************************************/
QVariant
ListEditItemModel
::headerData( int section,
              Qt::Orientation /**orientation*/,
              int role ) const
{
  // qDebug()
  //   << this << "::headerData("
  //   << section << "," << orientation << "," << role
  //   << ")";

  // assert( orientation==Qt::Horizontal );

  switch( role )
    {
    case Qt::DisplayRole:
      assert( section>=0 && section<COLUMN_COUNT );
      return tr( HEADERS[ section ] );
      break;

    default:
      break;
    }

  return QVariant();
}

/*****************************************************************************/
QModelIndex
ListEditItemModel
::index( int row,
         int column,
         const QModelIndex & p ) const
{
  // qDebug()
  //   << this << "::index(" << row << "," << column << "," << parent << ")";

  if( m_StringList == nullptr )
    return QModelIndex();

  // qDebug()
  //   << "index:" << row << "," << column << "," << m_StringList->At( row );

  assert( row>=0 && column>=0 );

#if 0
  AbstractLayerModel * layer = m_StringList->At( row );

  if( layer==NULL || p.isValid() )
    return QModelIndex();
#endif

  return
    createIndex(
      row,
      column,
      p.isValid()
      ? NULL
      : m_StringList
    );
}

/*****************************************************************************/
bool
ListEditItemModel
::insertRow( int row, const QModelIndex & parent )
{
  return insertRows( row, 1, parent );
}

/*****************************************************************************/
bool
ListEditItemModel
::insertRows( int row, int count, const QModelIndex & parent )
{
  qDebug() << this << "::insertRows(" << row << "," << count << "," << parent << ")";

  assert( m_StringList!=nullptr );

  beginInsertRows( parent, row, count );
  {
    for( int r=row; r<row+count; ++r )
      m_StringList->Insert( "", r );
  }
  endInsertRows();

  return true;
}

/*****************************************************************************/
QModelIndex
ListEditItemModel
::parent( const QModelIndex & ) const
{
  // qDebug() << this << "::parent(" << index << ")";

  return QModelIndex();
}

/*****************************************************************************/
int
ListEditItemModel
::rowCount( const QModelIndex & p ) const
{
  // qDebug() << this << "::rowCount(" << p << ")";

  // qDebug() << "row-count:" <<
  //   ( ( m_StringList==NULL || p.isValid() )
  //     ? 0
  //     : m_StringList->GetCount()
  //   );

  return
    ( m_StringList==nullptr || p.isValid() )
    ? 0
    : m_StringList->Size();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'Wrapper'.

} // end namespace 'otb'
