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

#include "otbWrapperQtWidgetInputVectorDataParameter.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtWidgetInputVectorDataParameter::QtWidgetInputVectorDataParameter(InputVectorDataParameter* param, QtWidgetModel* m, QWidget* parent)
  : QtWidgetParameterBase(param, m, parent), m_InputVectorDataParam(param), m_HLayout(nullptr), m_Input(nullptr), m_Button(nullptr)
{
}

QtWidgetInputVectorDataParameter::~QtWidgetInputVectorDataParameter()
{
}

const QLineEdit* QtWidgetInputVectorDataParameter::GetInput() const
{
  return m_Input;
}

QLineEdit* QtWidgetInputVectorDataParameter::GetInput()
{
  return m_Input;
}

void QtWidgetInputVectorDataParameter::DoUpdateGUI()
{
  // update lineedit
  if (m_InputVectorDataParam->HasUserValue())
  {
    QString text(QFile::decodeName(m_InputVectorDataParam->GetFileName().c_str()));

    if (text != m_Input->text())
      m_Input->setText(text);
  }
}

void QtWidgetInputVectorDataParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit(this);
  m_Input->setToolTip(QString::fromStdString(m_InputVectorDataParam->GetDescription()));
  connect(m_Input, &QLineEdit::textChanged, this, &QtWidgetInputVectorDataParameter::SetFileName);
  connect(m_Input, &QLineEdit::textChanged, GetModel(), &QtWidgetModel::NotifyUpdate);

  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton(this);
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setMaximumWidth(m_Button->width());
  connect(m_Button, &QPushButton::clicked, this, &QtWidgetInputVectorDataParameter::SelectFile);
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void QtWidgetInputVectorDataParameter::SelectFile()
{
  assert(m_Input != NULL);

  QString filename(otb::GetOpenFilename(this, QString(), m_Input->text(), tr("Vector data files (*)"), NULL, QFileDialog::ReadOnly));

  if (filename.isEmpty())
    return;

  if (!SetFileName(filename))
  {
    std::ostringstream oss;

    oss << "Invalid filename: '" << QFile::encodeName(filename).constData() << "'";

    assert(GetModel() != NULL);

    GetModel()->SendLogWARNING(oss.str());

    return;
  }

  m_Input->setText(filename);
}


bool QtWidgetInputVectorDataParameter::SetFileName(const QString& value)
{
  bool res = true;
  // save value
  if (m_InputVectorDataParam->SetFromFileName(QFile::encodeName(value).constData()) == true)
  {
    // notify of value change
    QString key(m_InputVectorDataParam->GetKey());
    Q_EMIT    ParameterChanged(key);
  }
  else
    res = false;

  return res;
}
}
}
