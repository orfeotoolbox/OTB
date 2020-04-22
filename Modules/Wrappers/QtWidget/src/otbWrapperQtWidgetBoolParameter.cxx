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

#include "otbWrapperQtWidgetBoolParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetBoolParameter::QtWidgetBoolParameter(BoolParameter* boolParam, QtWidgetModel* m, QWidget* parent) : QtWidgetParameterBase(boolParam, m, parent)
{
}

QtWidgetBoolParameter::~QtWidgetBoolParameter()
{
}

void QtWidgetBoolParameter::SetValue(bool value)
{
  BoolParameter* paramDown = dynamic_cast<BoolParameter*>(this->GetParam());
  assert(paramDown && "Not a BoolParameter");
  if (paramDown->GetValue() != value)
  {
    paramDown->SetValue(value);

    QString key(paramDown->GetKey());
    Q_EMIT    ParameterChanged(key);
  }
}

void QtWidgetBoolParameter::DoUpdateGUI()
{
  BoolParameter* paramDown = dynamic_cast<BoolParameter*>(this->GetParam());
  assert(paramDown && "Not a BoolParameter");
  if (paramDown->GetValue() != m_Button->isChecked())
  {
    m_Button->setChecked(paramDown->GetValue());
  }
  QString buttonText(paramDown->GetValue() ? "On" : "Off");
  if (m_Button->text() != buttonText)
  {
    m_Button->setText(buttonText);
  }
}

void QtWidgetBoolParameter::DoCreateWidget()
{
  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0, 0, 0, 0);

  m_Button = new QToolButton(this);
  m_Button->setCheckable(true);
  BoolParameter* paramDown = dynamic_cast<BoolParameter*>(this->GetParam());
  assert(paramDown && "Not a BoolParameter");
  if (paramDown->GetValue())
  {
    m_Button->setText("On");
    m_Button->setChecked(true);
  }
  else
  {
    m_Button->setText("Off");
  }

  // Set the tool tip associated to the parameter
  m_Button->setToolTip(QString::fromStdString(paramDown->GetDescription()));

  connect(m_Button, &QToolButton::toggled, this, &QtWidgetBoolParameter::SetValue);
  connect(m_Button, &QToolButton::toggled, GetModel(), &QtWidgetModel::NotifyUpdate);

  hLayout->addWidget(m_Button);
  hLayout->addStretch();

  this->setLayout(hLayout);
}
}
}
