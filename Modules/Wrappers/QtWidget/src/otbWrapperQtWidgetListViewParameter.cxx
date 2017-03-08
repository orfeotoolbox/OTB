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

#include "otbWrapperQtWidgetListViewParameter.h"

#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetListViewParameter::QtWidgetListViewParameter(ListViewParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_ListViewParam(param), m_LineHeight(20)
{
}

QtWidgetListViewParameter::~QtWidgetListViewParameter()
{
}

void QtWidgetListViewParameter::DoUpdateGUI()
{
  size_t numSelected = m_ListViewParam->GetSelectedItems().size();

  //Clear m_ListView add re-add choices only if no items selected.
  //Otherwise results in mantis #1025

  if(numSelected < 1)
    {
    while(m_ListView->takeItem(0))
      {
      m_ListView->removeItemWidget( m_ListView->takeItem(0) );
      }

    for (unsigned int i = 0; i < m_ListViewParam->GetNbChoices(); ++i)
      {
      // Add listBox items
      QString key = m_ListViewParam->GetChoiceKey(i).c_str();
      m_ListView->addItem( key);
      }
    }

  //I can't find any reason for calling m_ListView->setCurrentRow(value) in this
  //case because QListWidget is a MultiSelection widget.
  if (m_ListView->selectionMode() == QAbstractItemView::SingleSelection)
    {
    unsigned int value = m_ListViewParam->GetValue( );
    m_ListView->setCurrentRow(value);
    }
}

void QtWidgetListViewParameter::DoCreateWidget()
{
  m_ListView = new QListWidget();
  m_ListView->setToolTip(m_ListViewParam->GetDescription());

  if(m_ListViewParam->GetSingleSelection())
    {
    m_ListView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
  else
    {
    m_ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

  connect( m_ListView, SIGNAL(itemSelectionChanged()), this, SLOT(SelectedItems()) );

  m_VLayout = new QHBoxLayout;
  m_VLayout->setContentsMargins(0, 0, 0, 0);
  m_VLayout->addWidget(m_ListView);
  m_ListView->setMaximumSize(m_ListView->width() ,  4* m_LineHeight);
  m_VLayout->activate();

  this->setLayout(m_VLayout);
}

void QtWidgetListViewParameter::SelectedItems()
{
  // Clear previous item selected
  m_SelectedItems.clear();

  // Item changed (check if selected or not)
  for (int idx = 0; idx < m_ListView->count(); ++idx)
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
}

}
}
