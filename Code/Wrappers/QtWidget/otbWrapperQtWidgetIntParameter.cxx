/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbWrapperQtWidgetIntParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetIntParameter::QtWidgetIntParameter(IntParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_IntParam(param)
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QSpinBox* input = new QSpinBox;
  input->setRange(param->GetMinimumValue(), param->GetMaximumValue());
  input->setToolTip(param->GetDescription());

  connect( input, SIGNAL(valueChanged(int)), this, SLOT(SetValue(int)) );
  connect( input, SIGNAL(valueChanged(int)), GetModel(), SLOT(NotifyUpdate()) );

  hLayout->addWidget(input);
  hLayout->addStretch();

  this->setLayout(hLayout);
}

QtWidgetIntParameter::~QtWidgetIntParameter()
{
}

void QtWidgetIntParameter::SetValue(int value)
{
  std::cout << "QtWidgetIntParameter::SetValue " << value << std::endl;
  m_IntParam->SetValue(value);
}

}
}
