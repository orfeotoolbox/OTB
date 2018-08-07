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

#include "otbWrapperQtWidgetInputProcessXMLParameter.h"


#include <otbQtAdapters.h>


namespace otb
{
namespace Wrapper
{

QtWidgetInputProcessXMLParameter::QtWidgetInputProcessXMLParameter(InputProcessXMLParameter* param, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(param, m, parent),
  m_XMLParam(param)
{
}

QtWidgetInputProcessXMLParameter::~QtWidgetInputProcessXMLParameter()
{
}

const QLineEdit*
QtWidgetInputProcessXMLParameter
::GetInput() const
{
  return m_Input;
}

QLineEdit*
QtWidgetInputProcessXMLParameter
::GetInput()
{
  return m_Input;
}

void QtWidgetInputProcessXMLParameter::DoUpdateGUI()
{
  if (m_XMLParam->HasUserValue())
    {
    // Update the lineEdit
    QString text(
      QFile::decodeName( m_XMLParam->GetFileName() )
    );

    if (text != m_Input->text())
      m_Input->setText(text);
    }
}

void QtWidgetInputProcessXMLParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit(this);
  m_Input->setToolTip(
    QString::fromStdString( m_XMLParam->GetDescription() )
  );
  connect( m_Input, &QLineEdit::textChanged, this, &QtWidgetInputProcessXMLParameter::SetFileName );
  connect( m_Input, &QLineEdit::textChanged, GetModel(), &QtWidgetModel::NotifyUpdate );

  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton(this);
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, &QPushButton::clicked, this, &QtWidgetInputProcessXMLParameter::SelectFile );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void
QtWidgetInputProcessXMLParameter
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    otb::GetOpenFilename(
      this,
      QString(),
      m_Input->text(),
      tr( "XML File (*.xml)" ),
      NULL,
      QFileDialog::ReadOnly
    )
  );

  if( filename.isEmpty() )
    return;

  m_Input->setText( filename  );
}


void QtWidgetInputProcessXMLParameter::SetFileName(const QString& value)
{
  // load xml file name
  if( m_XMLParam->SetFileName(
	QFile::encodeName( value ).constData() ) )
    {
    // notify of value change
    QString key( m_XMLParam->GetKey() );
    emit ParameterChanged(key);
    GetModel()->GetApplication()->ForceInXMLParseFlag();
    }
}

}

}
