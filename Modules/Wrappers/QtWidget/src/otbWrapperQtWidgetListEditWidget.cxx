/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbQtAdapters.h"
#include "otbWrapperQtWidgetListEditItemModel.h"
#include "otbWrapperTypes.h"

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
ListEditWidget::ListEditWidget(StringListInterface* sli, QWidget* p, Qt::WindowFlags flags) : QWidget(p, flags), m_UI(new otb::Wrapper::Ui::ListEditWidget())
{
  m_UI->setupUi(this);

  setAcceptDrops(true);

  assert(m_UI->treeView->selectionModel() == nullptr);

  //
  // Item-model.
  ListEditItemModel* model = new ListEditItemModel(sli, m_UI->treeView);

  m_UI->treeView->setModel(model);

  QObject::connect(model, &ListEditItemModel::dataChanged, this, &ListEditWidget::OnDataChanged);

  QObject::connect(model, &ListEditItemModel::modelReset, this, &ListEditWidget::OnModelReset);

  QObject::connect(model, &ListEditItemModel::rowsInserted, this, &ListEditWidget::OnRowsInserted);

  QObject::connect(model, &ListEditItemModel::rowsRemoved, this, &ListEditWidget::OnRowsRemoved);

  //
  // Selection-model.
  assert(m_UI->treeView->selectionModel() != nullptr);

  QObject::connect(m_UI->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ListEditWidget::OnSelectionChanged);

  //
  // Browse-button.
  assert(m_UI->browseButton != nullptr);

  m_UI->browseButton->setEnabled(model->IsBrowsable());
}

/*******************************************************************************/
ListEditWidget::~ListEditWidget()
{
  delete m_UI;
  m_UI = nullptr;
}

#if 0

/*****************************************************************************/
void
ListEditWidget
::SetBrowseEnabled( bool enabled )
{
  assert( m_UI!=nullptr );
  assert( m_UI->browseButton );

  m_UI->browseButton->setEnabled( enabled );
}

/*******************************************************************************/
bool
ListEditWidget
::IsBrowseEnabled() const
{
  assert( m_UI!=nullptr );
  assert( m_UI->browseButton );

  return m_UI->browseButton->isEnabled();
}

#endif

/*******************************************************************************/
const ListEditItemModel* ListEditWidget::GetItemModel() const
{
  return const_cast<ListEditWidget*>(this)->GetItemModel();
}

/*******************************************************************************/
ListEditItemModel* ListEditWidget::GetItemModel()
{
  assert(m_UI->treeView->model() == qobject_cast<ListEditItemModel*>(m_UI->treeView->model()));

  return qobject_cast<ListEditItemModel*>(m_UI->treeView->model());
}

/*******************************************************************************/
void ListEditWidget::Swap(int row1, int row2, SwapSelection s)
{
  assert(GetItemModel() != nullptr);

  assert(row1 >= 0);
  assert(row1 < GetItemModel()->rowCount());

  assert(row2 >= 0);
  assert(row2 < GetItemModel()->rowCount());


  ListEditItemModel* itemModel = GetItemModel();

  assert(itemModel != nullptr);


  itemModel->Swap(row1, row2);


  {
    int row = s == LEFT ? row1 : (s == RIGHT ? row2 : -1);

    if (row < 0)
      return;

    assert(m_UI != nullptr);
    assert(m_UI->treeView != nullptr);
    assert(m_UI->treeView->selectionModel() != nullptr);

    QItemSelectionModel* ism = m_UI->treeView->selectionModel();

    assert(ism != nullptr);

    ism->clear();

    ism->setCurrentIndex(itemModel->index(row, ListEditItemModel::COLUMN_NAME),
                         QItemSelectionModel::Clear | QItemSelectionModel::Select | QItemSelectionModel::Current | QItemSelectionModel::Rows);
  }
}

/*******************************************************************************/
QStringList ListEditWidget::browseFilenames(bool multi, const QString& filename)
{
  const ListEditItemModel* itemModel = GetItemModel();
  assert(itemModel != nullptr);

  QString filePath(QDir::current().filePath(filename));

  QString     selectedFilter;
  QStringList output;
  if (itemModel->IsInput())
  {
    if (multi)
    {
      output = otb::GetOpenFilenames(this, tr("Select input filename..."), filePath, itemModel->GetFilter(), &selectedFilter);
    }
    else
    {
      output.push_back(otb::GetOpenFilename(this, tr("Select input filename..."), filePath, itemModel->GetFilter(), &selectedFilter));
    }
  }
  else
  {
    output.push_back(otb::GetSaveFilename(this, tr("Select output filename..."), filePath, itemModel->GetFilter(), &selectedFilter));
  }
  return output;
}

/*******************************************************************************/
QString ListEditWidget::browseFilename(const QModelIndex& index)
{
  assert(index.isValid());
  assert(index.row() >= 0);
  assert(index.column() >= 0);

  //
  // Get item-model.
  const ListEditItemModel* itemModel = GetItemModel();
  assert(itemModel != nullptr);

  //
  // Pick-up filename.
  assert(itemModel->data(index, ListEditItemModel::USER_ROLE_DIRECTION).isValid());

  assert(itemModel->data(index, ListEditItemModel::USER_ROLE_DIRECTION) == Role_Input ||
         itemModel->data(index, ListEditItemModel::USER_ROLE_DIRECTION) == Role_Output);

  return (browseFilenames(false, itemModel->data(index).toString())).front();
}


/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void ListEditWidget::OnFilenameDropped(const QString& filename)
{
  ListEditItemModel* itemModel = GetItemModel();
  assert(itemModel != nullptr);

  if (filename.isEmpty())
    return;

  int row = itemModel->rowCount();
  assert(row >= 0);

  if (!itemModel->insertRow(row))
    return;

  itemModel->setData(itemModel->index(row, ListEditItemModel::COLUMN_NAME), filename);
}


void ListEditWidget::on_addButton_clicked()
{
  // qDebug() << this << "::on_addButton_clicked()";

  ListEditItemModel* itemModel = GetItemModel();
  assert(itemModel != nullptr);

  //
  // When not browsable
  if (!itemModel->IsBrowsable())
  {
    itemModel->insertRow(itemModel->rowCount());

    return;
  }

  //
  // When browsable.
  QStringList filenames(browseFilenames(true));

  if (filenames.isEmpty())
    return;

  int row = itemModel->rowCount();
  assert(row >= 0);

  for (int i = 0; i < filenames.size(); i++)
  {
    if (!itemModel->insertRow(row))
      return;

    itemModel->setData(itemModel->index(row, ListEditItemModel::COLUMN_NAME), filenames[i]);
    row++;
  }
}

/*******************************************************************************/
void ListEditWidget::on_removeButton_clicked()
{
  // qDebug() << this << "::on_removeButton_clicked()";

  assert(m_UI->treeView->selectionModel() != nullptr);


  QModelIndexList indexes(m_UI->treeView->selectionModel()->selectedRows());

  if (indexes.empty())
    return;


  ListEditItemModel* itemModel = GetItemModel();

  assert(itemModel != nullptr);


  for (const QModelIndex& i : indexes)
  {
    assert(i.isValid());

    itemModel->removeRow(i.row());
  }
}

/*******************************************************************************/
void ListEditWidget::on_removeAllButton_clicked()
{
  // qDebug() << this << "::on_removeAllButton_clicked()";

  ListEditItemModel* model = GetItemModel();
  assert(model);

  if (model->rowCount() < 1)
    return;

  assert(qApp);
  assert(!qApp->applicationName().isEmpty());

  if (QMessageBox::question(this, qApp->applicationName(), tr("Are you sure you want to delete all (%1) item(s)?").arg(model->rowCount()),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    return;

  model->removeRows(0, model->rowCount());
}

/*******************************************************************************/
void ListEditWidget::on_upButton_clicked()
{
  // qDebug() << this << "::on_upButton_clicked()";

  assert(m_UI != nullptr);
  assert(m_UI->treeView != nullptr);
  assert(m_UI->treeView->selectionModel() != nullptr);


  QModelIndexList indexes(m_UI->treeView->selectionModel()->selectedRows());

  if (indexes.empty())
    return;

  assert(indexes.size() == 1);


  const QModelIndex& front = indexes.front();

  if (front.row() < 1)
    return;


  Swap(front.row(), front.row() - 1, RIGHT);
}

/*******************************************************************************/
void ListEditWidget::on_downButton_clicked()
{
  // qDebug() << this << "::on_downButton_clicked()";

  assert(m_UI != nullptr);
  assert(m_UI->treeView != nullptr);
  assert(m_UI->treeView->selectionModel() != nullptr);


  QModelIndexList indexes(m_UI->treeView->selectionModel()->selectedRows());

  if (indexes.empty())
    return;

  assert(indexes.size() == 1);


  const QModelIndex& front = indexes.front();

  if (front.row() >= GetItemModel()->rowCount() - 1)
    return;


  Swap(front.row(), front.row() + 1, RIGHT);
}

/*******************************************************************************/
void ListEditWidget::on_browseButton_clicked()
{
  // qDebug() << this << "::on_browseButton_clicked()";

  assert(m_UI != nullptr);
  assert(m_UI->treeView != nullptr);
  assert(m_UI->treeView->selectionModel() != nullptr);


  //
  // Pick-up first item of selection.
  QModelIndexList indexes(m_UI->treeView->selectionModel()->selectedRows());

  if (indexes.isEmpty())
    return;

  assert(indexes.size() == 1);

  const QModelIndex& front = indexes.front();

  //
  // Get item-model.
  ListEditItemModel* itemModel = GetItemModel();
  assert(itemModel != nullptr);

  //
  // Pick-up filename.
  QString selectedFilter;

  QString filename(browseFilename(front));

  if (filename.isEmpty())
    return;

  //
  // Foo.
  itemModel->setData(front, filename);
}

/*******************************************************************************/
void ListEditWidget::OnSelectionChanged(const QItemSelection& /* selected */, const QItemSelection& /* deselected */)
{
  // qDebug()
  //   << this
  //   << "::onSelectionChanged(" << selected << "," << deselected << ")";

  // Experimental code.
  // assert( selected.indexes().size()>=0 && selected.indexes().size()<=1 );

  // assert( m_UI->upButton );
  // assert( m_UI->downButton );
  // assert( m_UI->browseButton );
  // assert( m_UI->removeButton );

  // if( selected.empty() )
  //   {
  //   m_UI->browseButton->setEnabled( false );
  //   m_UI->removeButton->setEnabled( false );

  //   m_UI->upButton->setEnabled( false );
  //   m_UI->downButton->setEnabled( false );

  //   return;
  //   }

  // assert( GetItemModel() );

  // m_UI->browseButton->setEnabled( GetItemModel()->IsBrowsable() );
  // m_UI->removeButton->setEnabled( true );

  // const QModelIndex & index = selected.indexes().front();

  // m_UI->upButton->setEnabled( index.isValid() && index.row()>0 );

  // m_UI->downButton->setEnabled(
  //   index.isValid() &&
  //   index.sibling( index.row() + 1, index.column() ).isValid()
  // );
}

/*****************************************************************************/
void ListEditWidget::OnDataChanged(const QModelIndex&, const QModelIndex&)
{
  // qDebug() << this << "::OnDataChanged()";

  assert(GetItemModel() != nullptr);

  // Notify the ParameterList that the parameter has been modified (UserValue)
  // ParameterList is not available in this class, transfer the signal to WidgetParameterList
  Q_EMIT ValueChanged();

  // Then, trigger the update the application parameters
  Q_EMIT Updated();
}

/*****************************************************************************/
void ListEditWidget::OnModelReset()
{
  // qDebug() << this << "::OnModelReset()";

  Q_EMIT Updated();
}

/*****************************************************************************/
void ListEditWidget::OnRowsInserted(const QModelIndex&, int, int)
{
  // qDebug() << this << "::OnRowsInserted()";

  Q_EMIT Updated();
}

/*****************************************************************************/
void ListEditWidget::OnRowsRemoved(const QModelIndex&, int, int)
{
  // qDebug() << this << "::OnRowsRemoved()";

  Q_EMIT Updated();
}

} // end namespace 'Wrapper'

} // end namespace 'otb'
