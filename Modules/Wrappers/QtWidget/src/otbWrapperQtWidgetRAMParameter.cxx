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

#include "otbWrapperQtWidgetRAMParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetRAMParameter::QtWidgetRAMParameter(RAMParameter* param, QtWidgetModel* m, QWidget* parent) : QtWidgetParameterBase(param, m, parent), m_RAMParam(param)
{
}

QtWidgetRAMParameter::~QtWidgetRAMParameter()
{
}

void QtWidgetRAMParameter::DoCreateWidget()
{
  // Set up input text edit
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QSpinBox = new QSpinBox(this);
  m_QSpinBox->setToolTip(QString::fromStdString(m_RAMParam->GetDescription()));

  connect(m_QSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &QtWidgetRAMParameter::SetValue);
  connect(m_QSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), GetModel(), &QtWidgetModel::NotifyUpdate);

  // Set a valid range
  // Using m_UnsignedIntParam->GetMaximum() to set the maximum range
  // of the SpinBox give a maximum of 0. The SpinBox max is an
  // integer and m_UnsignedIntParam->GetMaximum() returns an unsigned
  // integer which is 2 times the itk::NumericTraits<int>::max().
  // static_cast<int>(m_UnsignedIntParam->GetMaximum()) = 0 then.
  m_QSpinBox->setRange(itk::NumericTraits<int>::Zero, itk::NumericTraits<int>::max());

  // Block mouse wheel events to the QSpinBox
  // this is to avoid grabbing focus when scrolling the parent QScrollArea
  m_QSpinBox->setFocusPolicy(Qt::StrongFocus);
  m_QSpinBox->installEventFilter(this);

  m_QHBoxLayout->addWidget(m_QSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);
}

void QtWidgetRAMParameter::DoUpdateGUI()
{
  bool blocked = m_QSpinBox->blockSignals(true);

  if (m_RAMParam->HasValue())
  {
    m_QSpinBox->setValue(static_cast<int>(m_RAMParam->GetValue()));
  }
  m_QSpinBox->blockSignals(blocked);

  QFont f = m_QSpinBox->font();
  if (m_RAMParam->HasUserValue())
  {
    f.setBold(true);
  }
  else
  {
    f.setBold(false);
  }
  m_QSpinBox->setFont(f);
}

void QtWidgetRAMParameter::SetValue(int value)
{
  m_RAMParam->SetValue(static_cast<unsigned int>(value));
  m_RAMParam->SetUserValue(true);
  m_RAMParam->SetAutomaticValue(false);
}
}
}
