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

#include "otbWrapperQtWidgetComplexInputImageParameter.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtWidgetComplexInputImageParameter::QtWidgetComplexInputImageParameter(ComplexInputImageParameter* param, QtWidgetModel* m, QWidget * parent)
: QtWidgetParameterBase(param, m, parent),
  m_ComplexInputImageParam(param)
{
}

QtWidgetComplexInputImageParameter::~QtWidgetComplexInputImageParameter()
{
}
const QLineEdit*
QtWidgetComplexInputImageParameter
::GetInput() const
{
  return m_Input;
}

QLineEdit*
QtWidgetComplexInputImageParameter
::GetInput()
{
  return m_Input;
}

void QtWidgetComplexInputImageParameter::DoUpdateGUI()
{
  //update lineedit if HasUserValue flag is set(from xml)
  if(m_ComplexInputImageParam->HasUserValue())
    {
    QString text(
      QFile::decodeName(
        m_ComplexInputImageParam->GetFileName().c_str()
      )
    );

    if (text != m_Input->text())
      m_Input->setText(text);
    }
}

void QtWidgetComplexInputImageParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip(
    QString::fromStdString( m_ComplexInputImageParam->GetDescription() )
  );
  connect( m_Input, &QLineEdit::textChanged, this, &QtWidgetComplexInputImageParameter::SetFileName );
  connect( m_Input, &QLineEdit::textChanged, GetModel(), &QtWidgetModel::NotifyUpdate );

  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, &QPushButton::clicked, this, &QtWidgetComplexInputImageParameter::SelectFile );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

void
QtWidgetComplexInputImageParameter
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    otb::GetOpenFilename(
      this,
      QString(),
      m_Input->text(),
      tr( "Raster files (*)" ),
      NULL,
      QFileDialog::ReadOnly
    )
  );

  if( filename.isEmpty() )
    return;

  if( !SetFileName( filename ) )
    {
    std::ostringstream oss;

    oss << "Invalid filename: '"
	<< QFile::encodeName( filename ).constData()
	<< "'";

    assert( GetModel()!=NULL );

    GetModel()->SendLogWARNING( oss.str() );

    return;
    }

  m_Input->setText( filename  );
}

bool QtWidgetComplexInputImageParameter::SetFileName(const QString& value)
{
  bool res = false;
  // save value
  if( m_ComplexInputImageParam->SetFromFileName(
    QFile::encodeName( value ).constData()) == true)
    {
    res = true;
    // notify of value change
    QString key( m_ComplexInputImageParam->GetKey() );
    emit ParameterChanged(key);
    }

  return res;
}

}
}
