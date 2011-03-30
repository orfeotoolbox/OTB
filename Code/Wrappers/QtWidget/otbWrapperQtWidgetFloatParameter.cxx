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
#include "otbWrapperQtWidgetFloatParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetFloatParameter::QtWidgetFloatParameter(FloatParameter* floatParam, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_FloatParam(floatParam)
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QDoubleSpinBox* input = new QDoubleSpinBox;
  input->setDecimals(5);
  input->setRange(floatParam->GetMinimumValue(), floatParam->GetMaximumValue());
  input->setToolTip(floatParam->GetDescription());

  connect( input, SIGNAL(valueChanged(double)), this, SLOT(SetValue(double)) );
  connect( input, SIGNAL(valueChanged(double)), GetModel(), SLOT(NotifyUpdate()) );

  //QString optionID(floatParam->GetName());
  hLayout->addWidget(input);
  hLayout->addStretch();

  this->setLayout(hLayout);
}

QtWidgetFloatParameter::~QtWidgetFloatParameter()
{
}

void QtWidgetFloatParameter::SetValue(double value)
{
  std::cout << "QtWidgetFloatParameter::SetValue " << value << std::endl;
  m_FloatParam->SetValue( static_cast<float>(value) );
}

}
}
