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

QtWidgetFloatParameter::QtWidgetFloatParameter(FloatParameter* floatParam)
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QDoubleSpinBox* input = new QDoubleSpinBox;
  input->setDecimals(5);
  input->setRange(floatParam->GetMinimumValue(), floatParam->GetMaximumValue());
  input->setToolTip(floatParam->GetDescription());


  //QString optionID(floatParam->GetName());
  hLayout->addWidget(input);
  hLayout->addStretch();

  this->setLayout(hLayout);
}

QtWidgetFloatParameter::~QtWidgetFloatParameter()
{
}

}
}
