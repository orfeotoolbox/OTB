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

#include "otbQtStringSelectionWidget.h"

namespace otb
{
namespace Wrapper
{

QtStringSelectionWidget::QtStringSelectionWidget() : QWidget()
{
  this->DoCreateWidget();
}

QtStringSelectionWidget::~QtStringSelectionWidget()
{
}

bool QtStringSelectionWidget::IsChecked() const
{
  return m_Checkbox->isChecked();
}

void QtStringSelectionWidget::SetChecked(bool val)
{
  return m_Checkbox->setChecked(val);
}

const QString QtStringSelectionWidget::GetText() const
{
  return m_Input->text();
}

void QtStringSelectionWidget::SetText(const QString& qString)
{
  m_Input->setText(qString);
}

std::string QtStringSelectionWidget::ToStdString()
{
  return m_Input->text().toLatin1().constData();
}

void QtStringSelectionWidget::ClearText()
{
  m_Input->clear();
}

void QtStringSelectionWidget::DoUpdateGUI()
{
}

void QtStringSelectionWidget::DoCreateWidget()
{
  unsigned int sp = 2;
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(sp);
  m_HLayout->setContentsMargins(sp, sp, sp, sp);

  m_Checkbox = new QCheckBox();
  m_Checkbox->setChecked(true);
  m_HLayout->addWidget(m_Checkbox);

  m_Input = new QLineEdit;
  m_Input->setEnabled(m_Checkbox->isChecked());

  m_HLayout->addWidget(m_Input);

  connect(m_Checkbox, &QCheckBox::toggled, m_Input, &QLineEdit::setEnabled);

  connect(m_Input, &QLineEdit::editingFinished, this, &QtStringSelectionWidget::OnEditionFinished);

  this->setLayout(m_HLayout);
}

void QtStringSelectionWidget::OnEditionFinished()
{
  // used to propagate m_Input (QLineEdit type) editingFinished signal
  emit InternalQLineEditEditionFinished();
}
}
}
