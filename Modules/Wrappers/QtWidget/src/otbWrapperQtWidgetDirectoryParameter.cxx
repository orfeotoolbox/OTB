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

#include "otbWrapperQtWidgetDirectoryParameter.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtWidgetDirectoryParameter::QtWidgetDirectoryParameter(DirectoryParameter* param, QtWidgetModel* m, QWidget* parent)
  : QtWidgetParameterBase(param, m, parent), m_DirectoryParam(param)
{
}

QtWidgetDirectoryParameter::~QtWidgetDirectoryParameter()
{
}

void QtWidgetDirectoryParameter::DoUpdateGUI()
{
  // Update the lineEdit
  QString text(QFile::decodeName(m_DirectoryParam->GetValue().c_str()));

  m_Input->setText(text);
}

void QtWidgetDirectoryParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit(this);
  m_Input->setToolTip(QString::fromStdString(m_DirectoryParam->GetDescription()));
  connect(m_Input, &QLineEdit::textChanged, this, &QtWidgetDirectoryParameter::SetFileName);
  connect(m_Input, &QLineEdit::textChanged, GetModel(), &QtWidgetModel::NotifyUpdate);

  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton(this);
  m_Button->setText("...");
  m_Button->setToolTip("Select a directory...");
  m_Button->setMaximumWidth(m_Button->width());
  connect(m_Button, &QPushButton::clicked, this, &QtWidgetDirectoryParameter::SelectFile);
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void QtWidgetDirectoryParameter::SelectFile()
{
  assert(m_Input != NULL);

  QString dir(GetExistingDirectory(this, QString(), m_Input->text()));

  if (dir.isEmpty())
    return;

  m_Input->setText(dir);
}


void QtWidgetDirectoryParameter::SetFileName(const QString& value)
{
  // save value
  m_DirectoryParam->SetValue(QFile::encodeName(value).constData());

  // notify of value change
  QString key(m_DirectoryParam->GetKey());
  Q_EMIT    ParameterChanged(key);
}
}
}
