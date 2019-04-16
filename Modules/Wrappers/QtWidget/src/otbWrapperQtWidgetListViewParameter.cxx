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

#include "otbWrapperQtWidgetListViewParameter.h"

#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetListViewParameter::QtWidgetListViewParameter(ListViewParameter* param, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(param, m, parent),
  m_ListViewParam(param), m_LineHeight(20)
{
}

QtWidgetListViewParameter::~QtWidgetListViewParameter()
{
}

void QtWidgetListViewParameter::DoUpdateGUI()
{
  bool resetNeeded = false;
  if (m_ListViewParam->GetNbChoices() != (unsigned int) m_ListView->count())
    {
    resetNeeded = true;
    }
  else
    {
    for (unsigned int i = 0; i < m_ListViewParam->GetNbChoices(); ++i)
      {
      QString key = m_ListViewParam->GetChoiceName(i).c_str();
      if (key != m_ListView->item(i)->text() )
        {
        resetNeeded = true;
        break;
        }
      }
    }

  // save selected choices
  std::vector<int> selected = m_ListViewParam->GetSelectedItems();

  //Beware of mantis #1025 and #1403
  if(resetNeeded)
    {
    m_SelectedItems.clear();
    while(m_ListView->takeItem(0))
      {
      m_ListView->removeItemWidget( m_ListView->takeItem(0) );
      }

    for (unsigned int i = 0; i < m_ListViewParam->GetNbChoices(); ++i)
      {
      // Add listBox items
      QString key = m_ListViewParam->GetChoiceName(i).c_str();
      m_ListView->addItem( key);
      }
    }

  // test if selection has to be updated
  if (selected != m_SelectedItems)
    {
    for (int idx = 0; idx < m_ListView->count(); ++idx)
      {
      // check if this item is selected
      bool isSelected = false;
      for (unsigned int k = 0 ; k < selected.size() ; ++k)
        {
        if (selected[k] == idx)
          {
          isSelected = true;
          break;
          }
        }
      m_ListView->item(idx)->setSelected(isSelected);
      }
    }

}

void QtWidgetListViewParameter::DoCreateWidget()
{
  m_ListView = new QListWidget(this);
  m_ListView->setToolTip(
    QString::fromStdString( m_ListViewParam->GetDescription() )
  );

  if(m_ListViewParam->GetSingleSelection())
    {
    m_ListView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
  else
    {
    m_ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

  connect( m_ListView, &QListWidget::itemSelectionChanged, this, &QtWidgetListViewParameter::SelectedItems );

  m_VLayout = new QHBoxLayout;
  m_VLayout->setContentsMargins(0, 0, 0, 0);
  m_VLayout->addWidget(m_ListView);
  m_VLayout->activate();

  this->setLayout(m_VLayout);
}

void QtWidgetListViewParameter::SelectedItems()
{
  // Clear previous item selected
  m_SelectedItems.clear();

  // Item changed (check if selected or not)
  for (int idx = 0; idx < m_ListView->count() && idx < (int) m_ListViewParam->GetNbChoices(); ++idx)
    {
    if (m_ListView->item(idx)->isSelected())
      {
      m_SelectedItems.push_back(idx);
      }
    }
  m_ListViewParam->SetSelectedItems(m_SelectedItems);

  // make sure parameter is enabled
  m_ListViewParam->SetActive(true);
  m_ListViewParam->SetUserValue(true);

  // Call the application DoUpdateParameters, then all widgets' DoUpdateGUI (including this one)
  this->GetModel()->NotifyUpdate();
}

}
}
