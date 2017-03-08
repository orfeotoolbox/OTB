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

#include "otbWrapperQtWidgetIntParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetIntParameter::QtWidgetIntParameter(IntParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_IntParam(param)
{
}

QtWidgetIntParameter::~QtWidgetIntParameter()
{
}

void QtWidgetIntParameter::DoCreateWidget()
{
  // Set up input text edit
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QSpinBox = new QSpinBox;
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(), m_IntParam->GetMaximumValue());
  m_QSpinBox->setToolTip(m_IntParam->GetDescription());

  connect( m_QSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetValue(int)) );
  connect( m_QSpinBox, SIGNAL(valueChanged(int)), GetModel(), SLOT(NotifyUpdate()) );

  m_QHBoxLayout->addWidget(m_QSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_IntParam->GetRole() == Role_Output)
    {
    m_QSpinBox->setEnabled( false );
    }
}

void QtWidgetIntParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(),
                       m_IntParam->GetMaximumValue());

  bool signalsBlocked2 = m_QSpinBox->blockSignals( true );

  if (m_IntParam->HasValue())
    {
    m_QSpinBox->setValue(m_IntParam->GetValue());
    }
  m_QSpinBox->blockSignals( signalsBlocked2 );

  QFont f = m_QSpinBox->font();
  if (m_IntParam->HasUserValue())
    {
    f.setBold(true);
    }
  else
    {
    f.setBold(false);
    }
  m_QSpinBox->setFont(f);
}

void QtWidgetIntParameter::SetValue(int value)
{
  m_IntParam->SetValue(value);

  /** moved to ParameterChanged slot in QtWidgetParameterBase:: **/
  /**m_IntParam->SetUserValue(true); **/

  QString key( m_IntParam->GetKey() );
  emit ParameterChanged(key);

  m_IntParam->SetAutomaticValue(false);
}

}
}
