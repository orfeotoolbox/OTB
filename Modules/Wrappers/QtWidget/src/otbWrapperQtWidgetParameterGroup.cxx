/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetChoiceParameter.h"
#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterGroup::QtWidgetParameterGroup(ParameterGroup::Pointer paramList, QtWidgetModel* m, QWidget* parent)
  : QtWidgetParameterBase(paramList, m, parent), m_ParamList(paramList)
{
}

QtWidgetParameterGroup::~QtWidgetParameterGroup()
{
}

void QtWidgetParameterGroup::DoUpdateGUI()
{
  // Note that we do not need to call each child widget's UpdateGUI here,
  // because they already each have a signal/slot connection that triggers it
  // when the model updates.
  // It is created in QtWidgetParameterBase::CreateWidget()
}

void QtWidgetParameterGroup::DoCreateWidget()
{
  // a GridLayout with two columns : parameter label / parameter widget
  QGridLayout* gridLayout = new QGridLayout;
  this->setLayout(gridLayout);

  unsigned int nbParams = m_ParamList->GetNumberOfParameters();
  for (unsigned int i = 0; i < nbParams; ++i)
  {
    Parameter* param    = m_ParamList->GetParameterByIndex(i);
    Parameter* rawParam = m_ParamList->GetParameterByIndex(i, false);

    if (param != nullptr)
    {
      ParameterGroup*  paramAsGroup  = dynamic_cast<ParameterGroup*>(param);
      ChoiceParameter* paramAsChoice = dynamic_cast<ChoiceParameter*>(param);

      if (paramAsGroup == nullptr && paramAsChoice == nullptr)
      {
        // Label (col 1)
        QWidget* label = new QtWidgetParameterLabel(rawParam, this);
        gridLayout->addWidget(label, i, 1);

        // Parameter Widget (col 2)
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget(param, GetModel(), this);
        gridLayout->addWidget(specificWidget, i, 2);

        // CheckBox (col 0)
        QCheckBox* checkBox = new QCheckBox(this);
        connect(checkBox, &QCheckBox::clicked, specificWidget, &QtWidgetParameterBase::SetActivationState);
        connect(checkBox, &QCheckBox::clicked, GetModel(), &QtWidgetModel::NotifyUpdate);
        connect(specificWidget, &QtWidgetParameterBase::ParameterActiveStatus, checkBox, &QCheckBox::setChecked);
        connect(specificWidget, &QtWidgetParameterBase::ParameterActiveStatus, specificWidget, &QtWidgetParameterBase::SetActivationState);

        // if Mandatory make the checkbox checked and deactivated
        if (param->GetMandatory())
        {
          checkBox->setCheckState(Qt::Checked);
          checkBox->setEnabled(false);
          specificWidget->setEnabled(true);
        }
        else
        {
          checkBox->setCheckState(Qt::Unchecked);
          checkBox->setEnabled(true);
          specificWidget->setEnabled(false);
        }
        gridLayout->addWidget(checkBox, i, 0);

        m_WidgetList.push_back(specificWidget);
      }
      else
      {
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget(param, GetModel(), this);

        QVBoxLayout* vboxLayout = new QVBoxLayout;
        vboxLayout->addWidget(specificWidget);
        QGroupBox* group = new QGroupBox(this);
        group->setLayout(vboxLayout);

        // Make the parameter Group checkable when it is not mandatory
        if (!param->GetMandatory())
        {
          group->setCheckable(true);
          group->setChecked(false);

          // Update iteratively the children status
          for (auto child : specificWidget->children())
          {
            // deactivate the children tree
            this->ProcessChild(child, false);
          }
        }
        else
        {
          param->SetActive(true);
        }
        connect(group, &QGroupBox::clicked, specificWidget, &QtWidgetParameterBase::SetActivationState);

        group->setTitle(rawParam->GetName());
        gridLayout->addWidget(group, i, 0, 1, -1);

        m_WidgetList.push_back(specificWidget);
      }
    }
  }
}


// Slot connected to the signal emitted the checkBox relative to
// current widget
void QtWidgetParameterGroup::SetActivationState(bool value)
{
  // First call the superclass implementation
  this->QtWidgetParameterBase::SetActivationState(value);

  // Update the Group status
  this->setEnabled(value);

  // Update iteratively the children status
  for (auto child : this->children())
  {
    this->ProcessChild(child, value);
  }
}

// Activate iteratively  the children
void QtWidgetParameterGroup::ProcessChild(QObject* currentNode, bool status)
{
  // Activate the current node if it was checked
  QtWidgetParameterBase* widgetBase = dynamic_cast<QtWidgetParameterBase*>(currentNode);
  if (widgetBase)
  {
    if (widgetBase->IsChecked() && status)
    {
      widgetBase->GetParam()->SetActive(status);
    }

    // If the status is false (deactivating) deactivate all the children
    // tree
    if (!status)
    {
      widgetBase->GetParam()->SetActive(status);
    }
  }

  for (auto child : currentNode->children())
  {
    this->ProcessChild(child, status);
  }
}
}
}
