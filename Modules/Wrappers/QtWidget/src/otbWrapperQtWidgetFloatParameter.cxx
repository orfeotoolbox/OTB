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

#include "otbWrapperQtWidgetFloatParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetFloatParameter::QtWidgetFloatParameter(FloatParameter* floatParam, QtWidgetModel* m)
: QtWidgetParameterBase(floatParam, m),
  m_FloatParam(floatParam)
{
}

QtWidgetFloatParameter::~QtWidgetFloatParameter()
{
}

void QtWidgetFloatParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QDoubleSpinBox->setRange(m_FloatParam->GetMinimumValue(),
                             m_FloatParam->GetMaximumValue());

  bool signalsBlocked2 = m_QDoubleSpinBox->blockSignals( true );

  if (m_FloatParam->HasValue())
    {
    m_QDoubleSpinBox->setValue(m_FloatParam->GetValue());
    }
  m_QDoubleSpinBox->blockSignals( signalsBlocked2 );

  QFont f = m_QDoubleSpinBox->font();
  if (m_FloatParam->HasUserValue())
    {
    f.setBold(true);
    }
  else
    {
    f.setBold(false);
    }

  m_QDoubleSpinBox->setFont(f);
}

void QtWidgetFloatParameter::DoCreateWidget()
{
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QDoubleSpinBox = new QDoubleSpinBox;
  m_QDoubleSpinBox->setDecimals(5);
  m_QDoubleSpinBox->setSingleStep(0.1);
  m_QDoubleSpinBox->setRange(m_FloatParam->GetMinimumValue(), m_FloatParam->GetMaximumValue());
  m_QDoubleSpinBox->setToolTip(m_FloatParam->GetDescription());

  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetValue(double)) );
  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), GetModel(), SLOT(NotifyUpdate()) );

  m_QHBoxLayout->addWidget(m_QDoubleSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_FloatParam->GetRole() == Role_Output)
    {
    m_QDoubleSpinBox->setEnabled( false );
    }
}

void QtWidgetFloatParameter::SetValue(double value)
{
  m_FloatParam->SetValue( static_cast<float>(value) );
  /** moved to ParameterChanged slot in QtWidgetParameterBase:: **/
  /** m_FloatParam->SetUserValue(true); **/

  QString key( m_FloatParam->GetKey() );
  emit ParameterChanged(key);

  m_FloatParam->SetAutomaticValue(false);
}

}
}
