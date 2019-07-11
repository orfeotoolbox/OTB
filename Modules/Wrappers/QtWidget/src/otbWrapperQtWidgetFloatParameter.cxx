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

#include "otbWrapperQtWidgetFloatParameter.h"

#include <limits>

namespace otb
{
namespace Wrapper
{

QtWidgetFloatParameter::QtWidgetFloatParameter(FloatParameter* floatParam, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(floatParam, m, parent),
  m_FloatParam(floatParam)
{
}

QtWidgetFloatParameter::~QtWidgetFloatParameter()
{
}

void QtWidgetFloatParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QDoubleSpinBox->setRange(m_FloatParam->GetMinimumValue(), m_FloatParam->GetMaximumValue());

  // Update the SpinBox value if parameter has been updated
  if (m_FloatParam->HasValue())
  {
    m_QDoubleSpinBox->SetValueNoSignal(m_FloatParam->GetValue());
  }

  // Set style depending on UserValue parameter flag
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

  m_QDoubleSpinBox = new QtWidgetDoubleSpinBox(this);
  m_QDoubleSpinBox->setSingleStep(0.1);
  m_QDoubleSpinBox->setDecimals(std::numeric_limits<float>::digits10); // max precision, this is 6 for IEEE float
  m_QDoubleSpinBox->setRange(m_FloatParam->GetMinimumValue(), m_FloatParam->GetMaximumValue());
  m_QDoubleSpinBox->setToolTip( QString::fromStdString( m_FloatParam->GetDescription()));

  // Block mouse wheel events to the QSpinBox
  // this is to avoid grabbing focus when scrolling the parent QScrollArea
  m_QDoubleSpinBox->setFocusPolicy(Qt::StrongFocus);
  m_QDoubleSpinBox->installEventFilter(this);

  // Set the SpinBox initial value to the parameter value
  if (m_FloatParam->HasValue())
    m_QDoubleSpinBox->SetValueNoSignal(m_FloatParam->GetValue());

  // What happens when the Reset button is clicked
  connect(m_QDoubleSpinBox, &QtWidgetDoubleSpinBox::Cleared,
          this, &QtWidgetFloatParameter::OnCleared);

  // What happens when the value changed because of user interaction (keyboard or arrows pressed)
  // Note: to avoid calling this when the value changes automatically (reset button, DoParameterUpdate, XML load),
  // calls to QSpinBox::setValue() are wrapped with signal blockers
  connect(m_QDoubleSpinBox, static_cast<void (QtWidgetDoubleSpinBox::*)(double)>(&QtWidgetDoubleSpinBox::valueChanged),
          this, &QtWidgetFloatParameter::OnValueChanged);

  // What happens when the SpinBox looses focus, or enter is pressed
  connect(m_QDoubleSpinBox, &QtWidgetDoubleSpinBox::editingFinished,
          this, &QtWidgetFloatParameter::OnEditingFinished);

  m_QHBoxLayout->addWidget(m_QDoubleSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_FloatParam->GetRole() == Role_Output)
    {
    m_QDoubleSpinBox->setEnabled( false );
    }
}

void QtWidgetFloatParameter::OnCleared()
{
  // Set the parameter to its default value
  m_FloatParam->Reset();

  // Reset the SpinBox value, but avoid triggering its valueChanged signal
  m_QDoubleSpinBox->SetValueNoSignal(m_FloatParam->GetDefaultValue());

  // Unset user value flag and hide the Reset button
  m_FloatParam->SetUserValue(false);
  m_QDoubleSpinBox->DisableClearButton();

  // Call the application DoUpdateParameters, then all widgets' DoUpdateGUI (including this one)
  this->GetModel()->NotifyUpdate();
}

void QtWidgetFloatParameter::OnValueChanged(double value)
{
  // Set the parameter value, user value flag and show the Reset button
  m_FloatParam->SetValue( static_cast<float>(value) );
  m_FloatParam->SetUserValue(true);
  m_QDoubleSpinBox->EnableClearButton();
}

void QtWidgetFloatParameter::OnEditingFinished()
{
  // Call the application DoUpdateParameters, then all widgets' DoUpdateGUI (including this one)
  this->GetModel()->NotifyUpdate();
}

}
}
