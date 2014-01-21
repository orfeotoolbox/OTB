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
#include "mvdDatasetDragAndDropEventFilter.h"


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
#include "Core/mvdVectorImageModel.h"
#include "Gui/mvdTreeWidgetItem.h"

namespace mvd
{

/*
  TRANSLATOR mvd::DatasetDragAndDropEventFilter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
DatasetDragAndDropEventFilter
::DatasetDragAndDropEventFilter( DatabaseModel* model, QObject* parent  ) :
  TreeWidgetItemDragAndDropEventFilter( parent ),
  m_DatabaseModel( model )
{
  QObject::connect(
    this,
    SIGNAL( ItemDropped( QTreeWidgetItem* ) ),
    // to:
    this,
    SLOT( OnItemDropped( QTreeWidgetItem* ) )
  );
}

/*****************************************************************************/
DatasetDragAndDropEventFilter
::~DatasetDragAndDropEventFilter()
{
}

/*****************************************************************************/
void
DatasetDragAndDropEventFilter
::EmitDatasetDropped( DatasetModel* dataset )
{
  assert( dataset!=NULL );

  emit DatasetDropped( dataset );

  assert(
    dataset->GetSelectedImageModel()==
    dataset->GetSelectedImageModel< VectorImageModel  >()
  );

  VectorImageModel* image =
    dataset->GetSelectedImageModel< VectorImageModel  >();

  if( image!=NULL )
    emit ImageFilenameDropped( image->GetFilename() );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
DatasetDragAndDropEventFilter
::OnItemDropped( QTreeWidgetItem* item )
{
  qDebug() << this << "::OnItemDropped(" << item << ")";

  assert( item!=NULL );

  assert( m_DatabaseModel!=NULL );

  assert( dynamic_cast< TreeWidgetItem* >( item )!=NULL );
  TreeWidgetItem* twi = dynamic_cast< TreeWidgetItem* >( item );

  switch( twi->GetType() )
    {
    case TreeWidgetItem::ITEM_TYPE_NODE:
      break;

    case TreeWidgetItem::ITEM_TYPE_LEAF:
      EmitDatasetDropped(
        m_DatabaseModel->FindDatasetModel( twi->GetHash() )
      );
      break;

    default:
      break;
    }
}

} // end namespace 'mvd'
