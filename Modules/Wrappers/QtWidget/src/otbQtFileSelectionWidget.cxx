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

#include "otbQtFileSelectionWidget.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtFileSelectionWidget
::QtFileSelectionWidget() :
  QWidget(),
  m_HLayout( NULL ),
  m_Input( NULL ),
  m_Button( NULL ),
  m_Checkbox( NULL ),
  m_IOMode( IO_MODE_INPUT )
{
  this->DoCreateWidget();
}

QtFileSelectionWidget::~QtFileSelectionWidget()
{
}

void
QtFileSelectionWidget
::SetIOMode( IOMode mode )
{
  m_IOMode = mode;
}

QtFileSelectionWidget::IOMode
QtFileSelectionWidget
::GetIOMode() const
{
  return m_IOMode;
}

void QtFileSelectionWidget::DoUpdateGUI()
{

}

void QtFileSelectionWidget::DoCreateWidget()
{
  unsigned int sp = 2;
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(sp);
  m_HLayout->setContentsMargins(sp, sp, sp, sp);

  m_Checkbox = new QCheckBox();
  m_HLayout->addWidget(m_Checkbox);

  m_Input = new QLineEdit;
  connect( m_Input, &QLineEdit::editingFinished, this, &QtFileSelectionWidget::CallFilenameChanged );
  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setFixedWidth(30);

  connect( m_Button, &QPushButton::clicked, this, &QtFileSelectionWidget::SelectFile );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void
QtFileSelectionWidget
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    m_IOMode == IO_MODE_INPUT
    ? otb::GetOpenFilename(
        this,
	QString(),
	m_Input->text(),
	tr( "All files (*)" ),
	NULL,
	QFileDialog::ReadOnly )
    : otb::GetSaveFilename(
        this,
	QString(),
	m_Input->text(),
	tr( "All files (*)" ),
	NULL )
  );

  if( filename.isEmpty() )
    return;

  SetFileName(filename);
}

void
QtFileSelectionWidget
::CallFilenameChanged()
{
  emit FilenameChanged();
}

void
QtFileSelectionWidget
::SetFileName(const QString & filename)
{
  m_Input->setText( filename  );

  emit FilenameChanged();
}

}

}
