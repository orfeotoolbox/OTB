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

#include "otbWrapperQtWidgetListEditWidget.h"
#include "ui_otbWrapperQtWidgetListEditWidget.h"


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
#include "otbWrapperQtWidgetListEditItemModel.h"


namespace otb
{

namespace Wrapper
{

/*
  TRANSLATOR otn::Wrappers

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
ListEditWidget
::ListEditWidget( QWidget * p, Qt::WindowFlags flags ) :
  QWidget( p, flags ),
  m_UI( new otb::Wrapper::Ui::ListEditWidget() )
{
  m_UI->setupUi( this );

  {
  QItemSelectionModel * ism = m_UI->treeView->selectionModel();

  m_UI->treeView->setModel( new ListEditItemModel( m_UI->treeView ) );

  delete ism;
  ism = NULL;
  }
}

/*******************************************************************************/
ListEditWidget
::~ListEditWidget()
{
  delete m_UI;
  m_UI = nullptr;
}

/*******************************************************************************/
const ListEditItemModel *
ListEditWidget
::GetItemModel() const
{
  return const_cast< ListEditWidget * >( this )->GetItemModel();
}

/*******************************************************************************/
ListEditItemModel *
ListEditWidget
::GetItemModel()
{
  assert(
    m_UI->treeView->model()==
    qobject_cast< ListEditItemModel * >( m_UI->treeView->model() )
    );

  return qobject_cast< ListEditItemModel * >( m_UI->treeView->model() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ListEditWidget
::on_addButton_clicked()
{
  // qDebug() << this << "::on_addButton_clicked()";

  ListEditItemModel * itemModel = GetItemModel();

  assert( itemModel!=nullptr );

  if( !GetItemModel()->insertRow( itemModel->rowCount() ) )
    return;
}

/*******************************************************************************/
void
ListEditWidget
::on_removeButton_clicked()
{
  // qDebug() << this << "::on_removeButton_clicked()";

  assert( m_UI->treeView->selectionModel()!=nullptr );


  QModelIndexList indexes(
    m_UI->treeView->selectionModel()->selectedRows()
  );

  if( indexes.empty() )
    return;


  ListEditItemModel * itemModel = GetItemModel();

  assert( itemModel!=nullptr );


  for( const QModelIndex & i : indexes )
    {
    assert( i.isValid() );

    itemModel->removeRow( i.row() );
    }
}

/*******************************************************************************/
void
ListEditWidget
::on_upButton_clicked()
{
  // qDebug() << this << "::on_upButton_clicked()";

  assert( m_UI->treeView->selectionModel()!=nullptr );


  QModelIndexList indexes(
    m_UI->treeView->selectionModel()->selectedRows()
  );

  if( indexes.empty() )
    return;

  assert( indexes.size()==1 );

  if( indexes.front().row()<1 )
    return;

  assert( GetItemModel()!=nullptr );

  assert(
    dynamic_cast< ListEditItemModel * >( GetItemModel() )==
    GetItemModel() );

  dynamic_cast< ListEditItemModel * >( GetItemModel() )
    ->Swap(
      indexes.front().row(),
      indexes.front().row() - 1
    );
}

} // end namespace 'Wrapper'

} // end namespace 'otb'
