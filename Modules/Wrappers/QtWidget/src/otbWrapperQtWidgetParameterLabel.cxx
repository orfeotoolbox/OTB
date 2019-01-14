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

#include "otbWrapperQtWidgetParameterLabel.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterLabel::QtWidgetParameterLabel(Parameter* param, QWidget * parent)
  : QWidget(parent)
{
  // Set up label
  QLabel *label = new QLabel(this);

  label->setText(param->GetName());
  label->setToolTip(param->GetKey());

  QVBoxLayout *labelLayout = new QVBoxLayout;
  labelLayout->setSpacing(0);
  labelLayout->setContentsMargins(0, 0, 0, 0);
  labelLayout->addWidget(label, 0);

  this->setLayout(labelLayout);
}

QtWidgetParameterLabel::~QtWidgetParameterLabel()
{
}

}
}
