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

#include "mvdLayerStackWidget.h"
#include "ui_mvdLayerStackWidget.h"


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
#include "mvdGui.h"
#include "mvdLayerStackItemModel.h"

namespace mvd
{

/*
  TRANSLATOR mvd::LayerStackWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
LayerStackWidget::LayerStackWidget(QWidget* p, Qt::WindowFlags flags) : QWidget(p, flags), m_UI(new mvd::Ui::LayerStackWidget())
{
  m_UI->setupUi(this);

  m_UI->reloadButton->setVisible(false);
  m_UI->treeView->setDragEnabled(true);

  {
    QItemSelectionModel* ism = m_UI->treeView->selectionModel();

    m_UI->treeView->setModel(new LayerStackItemModel(m_UI->treeView));

    delete ism;
    ism = NULL;
  }

  InstallEventFilter(this);

  // Width of the columns in the layer stack:
  // Header section sizes are user adjustable but are not saved after a restart
  // So we set it to a guess value here
  // The unit is pixel, Qt's default is 100
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_PROJ, 75);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_RESOLUTION, 40);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_NAME, 200);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_EFFECT, 90);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_I, 60);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_J, 60);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_R, 90);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_G, 90);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_B, 90);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_X, 90);
  m_UI->treeView->header()->resizeSection(LayerStackItemModel::COLUMN_Y, 90);

  QObject::connect(m_UI->treeView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
                   // to:
                   this, SLOT(OnCurrentRowChanged(const QModelIndex&, const QModelIndex&)));

  QObject::connect(m_UI->treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   // to:
                   this, SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));

  QObject::connect(m_UI->topButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(TopButtonClicked()));

  QObject::connect(m_UI->bottomButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(BottomButtonClicked()));

  QObject::connect(m_UI->upButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(UpButtonClicked()));

  QObject::connect(m_UI->downButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(DownButtonClicked()));

  QObject::connect(m_UI->deleteButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(DeleteLayerRequested()));

  QObject::connect(m_UI->deleteAllButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(DeleteAllLayersRequested()));

  QObject::connect(m_UI->projectionButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(ProjectionButtonClicked()));

  QObject::connect(m_UI->applyButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(ApplyButtonClicked()));

  QObject::connect(m_UI->resetEffectsButton, SIGNAL(clicked()),
                   // to:
                   this, SIGNAL(ResetEffectsButtonClicked()));

  /////// Bugfix for layer deletion
  QObject::connect(this->GetItemModel(), SIGNAL(LayerDeletingModel(unsigned int)),
                   // to:
                   this, SIGNAL(LayerDeletingWidget(unsigned int)));
  ///////////////
}

/*******************************************************************************/
LayerStackWidget::~LayerStackWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
bool LayerStackWidget::eventFilter(QObject* object, QEvent* e)
{
  assert(object == m_UI->treeView);
  assert(e != NULL);

  if (object != m_UI->treeView)
    return false;

  switch (e->type())
  {
  //
  // KEY RELEASE
  case QEvent::KeyRelease:
  {
    QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(e);
    assert(keyEvent != NULL);

    switch (keyEvent->key())
    {
    case Qt::Key_C:
      if (keyEvent->modifiers() == Qt::ControlModifier && m_UI->treeView->currentIndex().isValid())
      {
        emit CopyLayerRequested(LayerStackItemModel::GetLayer(m_UI->treeView->currentIndex()));

        return true;
      }
      break;
    //
    case Qt::Key_Delete:
      if (keyEvent->modifiers() == Qt::NoModifier)
      {
        emit DeleteLayerRequested();

        return true;
      }
      else if (keyEvent->modifiers() == Qt::ShiftModifier)
      {
        emit DeleteAllLayersRequested();

        return true;
      }
      break;
    }
  }
  break;
  //
  // MOUSE-WHEEL
  case QEvent::Wheel:
  {
    QWheelEvent* wEvent = dynamic_cast<QWheelEvent*>(e);
    assert(wEvent != NULL);

    if (wEvent->modifiers() == Qt::ControlModifier)
    {
      emit RotateLayersRequested(wEvent->delta() / (MOUSE_WHEEL_STEP_FACTOR * MOUSE_WHEEL_STEP_DEGREES));

      return true;
    }
  }
  break;
  //
  // other.
  default:
    break;
  }

  return false;
}

/*******************************************************************************/
const LayerStackItemModel* LayerStackWidget::GetItemModel() const
{
  return const_cast<LayerStackWidget*>(this)->GetItemModel();
}

/*******************************************************************************/
LayerStackItemModel* LayerStackWidget::GetItemModel()
{
  assert(m_UI->treeView->model() == qobject_cast<LayerStackItemModel*>(m_UI->treeView->model()));

  return qobject_cast<LayerStackItemModel*>(m_UI->treeView->model());
}

/*******************************************************************************/
void LayerStackWidget::InstallEventFilter(QObject* filter)
{
  assert(m_UI != NULL);
  assert(m_UI->treeView != NULL);

  m_UI->treeView->installEventFilter(filter);
}

/*******************************************************************************/
// void
// LayerStackWidget
// ::SetModel( LayerStackItemModel * itemModel )
// {
//   // See http://qt-project.org/doc/qt-4.8/qabstractitemview.html#setModel .
//   QItemSelectionModel * itemSelectionModel = m_UI->treeView->selectionModel();

//   m_UI->treeView->setModel( itemModel );

//   itemModel->setParent( m_UI->treeView );

//   delete itemSelectionModel;
//   itemSelectionModel = NULL;
// }

/*******************************************************************************/
void LayerStackWidget::SetApplyEnabled(bool enabled)
{
  assert(m_UI->applyButton != NULL);

  m_UI->applyButton->setEnabled(enabled);
}

/*******************************************************************************/
void LayerStackWidget::SetCurrent(int row)
{
  assert(m_UI->treeView->selectionModel() != NULL);

  // if( m_UI->treeView->selectionModel()->currentIndex().row()==row )
  //   return;

  if (row < 0)
    m_UI->treeView->selectionModel()->clearSelection();

  else
    m_UI->treeView->selectionModel()->select(m_UI->treeView->model()->index(row, 1), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  /*
  m_UI->treeView->selectionModel()->setCurrentIndex(
    m_UI->treeView->model()->index( row, 1 ),
    QItemSelectionModel::ClearAndSelect |
    // QItemSelectionModel::Current |
    QItemSelectionModel::Rows
  );
  */
}

/*******************************************************************************/
void LayerStackWidget::SetDeleteEnabled(bool enabled)
{
  assert(m_UI != NULL);


  assert(m_UI->deleteButton != NULL);

  m_UI->deleteButton->setEnabled(enabled);


  assert(m_UI->deleteAllButton != NULL);

  m_UI->deleteAllButton->setEnabled(enabled);
}

/*******************************************************************************/
void LayerStackWidget::SetMoveEnabled(bool enabled)
{
  assert(m_UI != NULL);

  assert(m_UI->upButton != NULL);
  assert(m_UI->downButton != NULL);
  assert(m_UI->topButton != NULL);
  assert(m_UI->bottomButton != NULL);

  m_UI->upButton->setEnabled(enabled);
  m_UI->downButton->setEnabled(enabled);
  m_UI->topButton->setEnabled(enabled);
  m_UI->bottomButton->setEnabled(enabled);
}

/*******************************************************************************/
void LayerStackWidget::SetProjectionEnabled(bool enabled)
{
  assert(m_UI != NULL);

  assert(m_UI->projectionButton != NULL);

  m_UI->projectionButton->setEnabled(enabled);
}

/*******************************************************************************/
void LayerStackWidget::SetReloadEnabled(bool enabled)
{
  assert(m_UI != NULL);

  assert(m_UI->reloadButton != NULL);

  m_UI->reloadButton->setEnabled(enabled);
}

/*******************************************************************************/
void LayerStackWidget::SetResetEffectsEnabled(bool enabled)
{
  assert(m_UI != NULL);

  assert(m_UI->reloadButton != NULL);

  m_UI->resetEffectsButton->setEnabled(enabled);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void LayerStackWidget::OnCurrentRowChanged(const QModelIndex& current, const QModelIndex&)
{
  // qDebug()
  //   << this
  //   << "::OnCurrentRowChange(" << current.row() << "," << previous.row() << ")";

  emit CurrentChanged(current.row());
}

/*******************************************************************************/
void LayerStackWidget::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&)
{
  // qDebug()
  //   << this
  //   << "::OnSelectionChanged(" << selected << "," << deselected << ")";

  QModelIndexList indexes(selected.indexes());
  // assert( indexes.empty() || indexes.size()==1 );

  emit SelectionChanged(indexes.empty() ? -1 : indexes.front().row());
}

} // end namespace 'mvd'
