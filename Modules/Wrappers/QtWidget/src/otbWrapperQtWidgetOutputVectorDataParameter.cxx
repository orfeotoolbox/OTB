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

#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperTypes.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtWidgetOutputVectorDataParameter::QtWidgetOutputVectorDataParameter(OutputVectorDataParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_OutputVectorDataParam(param)
{
}

QtWidgetOutputVectorDataParameter::~QtWidgetOutputVectorDataParameter()
{
}

void QtWidgetOutputVectorDataParameter::DoUpdateGUI()
{
  //update lineedit
  QString text( m_OutputVectorDataParam->GetFileName() );
  if (text != m_Input->text())
    m_Input->setText(text);
}

void QtWidgetOutputVectorDataParameter::DoCreateWidget()
{
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);

  m_Input = new QLineEdit;
  m_Input->setToolTip(
    QString::fromStdString( m_OutputVectorDataParam->GetDescription() )
  );
  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
  m_HLayout->addWidget(m_Input);

  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select output filename...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void
QtWidgetOutputVectorDataParameter
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    otb::GetSaveFilename(
      this,
      QString(),
      m_Input->text(),
      tr( "Vector data files (*)" ),
      NULL
    )
  );

  if( filename.isEmpty() )
    return;

  m_Input->setText( filename );
}


void QtWidgetOutputVectorDataParameter::SetFileName(const QString& value)
{
  // save value
  m_FileName = QFile::encodeName( value ).constData();

  m_OutputVectorDataParam->SetFileName(m_FileName);

  // notify of value change
  QString key( m_OutputVectorDataParam->GetKey() );
  emit ParameterChanged(key);
}


}
}
