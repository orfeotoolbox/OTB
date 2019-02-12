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

#include "otbWrapperQtWidgetStringParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetStringParameter::QtWidgetStringParameter(StringParameter* param, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(param, m, parent),
  m_StringParam(param)
{
}

QtWidgetStringParameter::~QtWidgetStringParameter()
{
}

void QtWidgetStringParameter::DoUpdateGUI()
{
  m_Input->setToolTip(
    QString::fromStdString( m_StringParam->GetDescription() )
  );

  // Update the lineEdit only if there is a change and that's not empty or whitespaces
  QString text( m_StringParam->GetValue().c_str() );
  if (!text.trimmed().isEmpty() && text != m_Input->text())
    {
    m_Input->setText(text);
    }
}

void QtWidgetStringParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);

  m_Input = new QLineEdit(this);
  m_Input->setToolTip(
    QString::fromStdString( m_StringParam->GetDescription() )
  );
  m_HLayout->addWidget(m_Input);

  connect( m_Input, &QLineEdit::textChanged, this, &QtWidgetStringParameter::SetValue );
  connect( m_Input, &QLineEdit::textChanged, GetModel(), &QtWidgetModel::NotifyUpdate );

  this->setLayout(m_HLayout);

  if (m_StringParam->GetRole() == Role_Output)
    {
    m_Input->setEnabled( false );
    }
}

void QtWidgetStringParameter::SetValue(const QString& value)
{
  m_StringParam->SetValue(value.toLatin1().constData());
  // notify of value change
  QString key( m_StringParam->GetKey() );
  emit ParameterChanged(key);
}

}
}
