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
  m_StackedLayerModel = model;

  // TODO: emit refresh data signal.
}

/*****************************************************************************/
/* QAbstractItemModel overloads                                              */
/*****************************************************************************/
int
LayerStackItemModel
::columnCount( const QModelIndex & parent ) const
{
  return COLUMN_COUNT;
}

/*****************************************************************************/
QVariant
LayerStackItemModel
::data( const QModelIndex & index, int role ) const
{
  switch( role )
    {
    case Qt::DisplayRole:
      switch( index.column() )
        {
        case COLUMN_INDEX:
          return QVariant( index.row() );
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

            return QVariant( vectorImageModel->GetFilename() );
            }
          else
            {
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
  return QAbstractItemModel::flags( index );
}

/*****************************************************************************/
QVariant
LayerStackItemModel
::headerData( int section,
              Qt::Orientation orientation,
              int role ) const
{
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
  return QModelIndex();
}

/*****************************************************************************/
QModelIndex
LayerStackItemModel
::parent( const QModelIndex & index ) const
{
  return QModelIndex();
}

/*****************************************************************************/
int
LayerStackItemModel
::rowCount( const QModelIndex & parent ) const
{
  return 0;
}

/*****************************************************************************/
bool
LayerStackItemModel
::setData( const QModelIndex & index,
           const QVariant & value,
           int role )
{
  return false;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
