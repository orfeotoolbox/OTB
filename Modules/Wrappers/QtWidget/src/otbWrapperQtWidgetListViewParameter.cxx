/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
  m_ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

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
}

}
}
