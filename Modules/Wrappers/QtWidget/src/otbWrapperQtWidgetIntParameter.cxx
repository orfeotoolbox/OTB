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

#include "otbWrapperQtWidgetIntParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetIntParameter::QtWidgetIntParameter(IntParameter* param, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(param, m, parent),
  m_IntParam(param)
{
}

QtWidgetIntParameter::~QtWidgetIntParameter()
{
}

void QtWidgetIntParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(), m_IntParam->GetMaximumValue());

  // Update the SpinBox value if parameter has been updated
  if (m_IntParam->HasValue())
  {
    m_QSpinBox->SetValueNoSignal(m_IntParam->GetValue());
  }

  // Set style depending on UserValue parameter flag
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

void QtWidgetIntParameter::DoCreateWidget()
{
  // Set up input text edit
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QSpinBox = new QtWidgetSpinBox(this);
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(), m_IntParam->GetMaximumValue());
  m_QSpinBox->setToolTip(QString::fromStdString(m_IntParam->GetDescription()));

  // Block mouse wheel events to the QSpinBox
  // this is to avoid grabbing focus when scrolling the parent QScrollArea
  m_QSpinBox->setFocusPolicy(Qt::StrongFocus);
  m_QSpinBox->installEventFilter(this);

  // Set the SpinBox initial value to the parameter value
  if (m_IntParam->HasValue())
    m_QSpinBox->SetValueNoSignal(m_IntParam->GetValue());

  // What happens when the Reset button is clicked
  connect(m_QSpinBox, &QtWidgetSpinBox::Cleared,
          this, &QtWidgetIntParameter::OnCleared);

  // What happens when the value changed because of user interaction (keyboard or arrows pressed)
  // Note: to avoid calling this when the value changes automatically (reset button, DoParameterUpdate, XML load),
  // calls to QSpinBox::setValue() are wrapped with signal blockers
  connect(m_QSpinBox, static_cast<void (QtWidgetSpinBox::*)(int)>(&QtWidgetSpinBox::valueChanged),
          this, &QtWidgetIntParameter::OnValueChanged);

  // What happens when the SpinBox looses focus, or enter is pressed
  connect(m_QSpinBox, &QtWidgetSpinBox::editingFinished,
          this, &QtWidgetIntParameter::OnEditingFinished);

  m_QHBoxLayout->addWidget(m_QSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_IntParam->GetRole() == Role_Output)
    {
    m_QSpinBox->setEnabled( false );
    }
}

void QtWidgetIntParameter::OnCleared()
{
  // Set the parameter to its default value
  m_IntParam->Reset();

  // Reset the SpinBox value, but avoid triggering its valueChanged signal
  m_QSpinBox->SetValueNoSignal(m_IntParam->GetDefaultValue());

  // Unset user value flag and hide the Reset button
  m_IntParam->SetUserValue(false);
  m_QSpinBox->DisableClearButton();

  // Call the application DoUpdateParameters, then all widgets' DoUpdateGUI (including this one)
  this->GetModel()->NotifyUpdate();
}

void QtWidgetIntParameter::OnValueChanged(int value)
{
  // Set the parameter value, user value flag and show the Reset button
  m_IntParam->SetValue( static_cast<float>(value) );
  m_IntParam->SetUserValue(true);
  m_QSpinBox->EnableClearButton();
}

void QtWidgetIntParameter::OnEditingFinished()
{
  // Call the application DoUpdateParameters, then all widgets' DoUpdateGUI (including this one)
  this->GetModel()->NotifyUpdate();
}

// Filter mouse wheel events to avoid scrolling issues in parent QScrollArea
bool QtWidgetIntParameter::eventFilter( QObject * o, QEvent * e )
{
  if ( e->type() == QEvent::Wheel && qobject_cast<QAbstractSpinBox*>( o ) )
  {
    e->ignore();
    return true;
  }
  return QWidget::eventFilter( o, e );
}


}
}
