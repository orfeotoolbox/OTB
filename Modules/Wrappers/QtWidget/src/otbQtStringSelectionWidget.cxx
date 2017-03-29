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

#include "otbQtStringSelectionWidget.h"

namespace otb
{
namespace Wrapper
{

QtStringSelectionWidget::QtStringSelectionWidget()
  : QWidget()
{
  this->DoCreateWidget();
}

QtStringSelectionWidget::~QtStringSelectionWidget()
{
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
  m_Input->setEnabled( m_Checkbox->isChecked() );

  m_HLayout->addWidget(m_Input);

  QObject::connect(
          m_Checkbox, SIGNAL( toggled( bool ) ),
          m_Input, SLOT( setEnabled( bool ) )
  );

  QObject::connect(
        m_Input,
        SIGNAL( editingFinished( ) ),
        this,
        SLOT( OnEditionFinished() )
        );

  this->setLayout(m_HLayout);
}

void QtStringSelectionWidget::OnEditionFinished()
{
  // used to propagate m_Input (QLineEdit type) editingFinished signal
  emit InternalQLineEditEditionFinished();
}

}

}
