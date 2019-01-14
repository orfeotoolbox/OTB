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

#include "otbWrapperQtWidgetChoiceParameter.h"

#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetChoiceParameter::QtWidgetChoiceParameter( ChoiceParameter* param,
                                            QtWidgetModel* m , QWidget * parent) :
  QtWidgetParameterBase(param, m, parent),
  m_ChoiceParam(param),
  m_ComboBox( nullptr ),
  m_StackWidget( nullptr ),
  m_VLayout( nullptr ),
  m_VLayoutGroup( nullptr ),
  m_WidgetList()
{
}

QtWidgetChoiceParameter::~QtWidgetChoiceParameter()
{
}

void QtWidgetChoiceParameter::DoUpdateGUI()
{
  // Update the combobox value
  unsigned int value = m_ChoiceParam->GetValue( );
  m_ComboBox->setCurrentIndex(value);

  // Update the choice subparameters
  WidgetListIteratorType it = m_WidgetList.begin();
  for (it = m_WidgetList.begin(); it != m_WidgetList.end(); ++it)
    {
    (*it)->UpdateGUI();
    }
}

void QtWidgetChoiceParameter::DoCreateWidget()
{
  m_ComboBox = new QComboBox(this);
  m_ComboBox->setToolTip(
    QString::fromStdString( m_ChoiceParam->GetDescription() )
  );

  m_StackWidget = new QStackedWidget(this);

  for (unsigned int i = 0; i < m_ChoiceParam->GetNbChoices(); ++i)
    {
    QString key = m_ChoiceParam->GetChoiceName(i).c_str();
    m_ComboBox->addItem( key, QVariant(key) );

    ParameterGroup::Pointer param = m_ChoiceParam->GetChoiceParameterGroupByIndex(i);
    if (param.IsNotNull())
      {
      QtWidgetParameterBase* widget =
       QtWidgetParameterFactory::CreateQtWidget( param, GetModel(), this);

      m_StackWidget->addWidget(widget);

      m_WidgetList.push_back(widget);
      }
    }

  connect( m_ComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtWidgetChoiceParameter::SetValue);
  connect( m_ComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), m_StackWidget, &QStackedWidget::setCurrentIndex);
  connect( m_ComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), GetModel(), &QtWidgetModel::NotifyUpdate );

  m_VLayout = new QVBoxLayout;
  m_VLayout->addWidget(m_ComboBox);
  if (!m_WidgetList.empty())
    {
    m_VLayout->addWidget(m_StackWidget);
    }
  m_VLayout->addStretch();

  this->setLayout(m_VLayout);
}

void QtWidgetChoiceParameter::SetValue(int value)
{
  m_ChoiceParam->SetValue( value );

  QString key( m_ChoiceParam->GetKey() );
  emit ParameterChanged(key);
}

}
}
