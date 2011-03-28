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

QtWidgetIntParameter::QtWidgetIntParameter(IntParameter* param)
: m_IntParam(param)
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QSpinBox* input = new QSpinBox;
  input->setRange(param->GetMinimumValue(), param->GetMaximumValue());
  input->setToolTip(param->GetDescription());

  hLayout->addWidget(input);
  hLayout->addStretch();

  this->setLayout(hLayout);
}

QtWidgetIntParameter::~QtWidgetIntParameter()
{
}

}
}
