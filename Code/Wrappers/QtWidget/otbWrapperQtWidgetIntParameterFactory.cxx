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

#include "otbWrapperQtWidgetIntParameterFactory.h"
#include "otbWrapperNumericalParameter.h"


namespace otb
{

namespace Wrapper
{

QtWidgetIntParameterFactory::QtWidgetIntParameterFactory()
{}

QtWidgetIntParameterFactory::~QtWidgetIntParameterFactory()
{}

QWidget * QtWidgetIntParameterFactory::CreateQtWidget(Parameter * param)
{
  // Try to cast to int parameter
  IntParameter * intParam = dynamic_cast<IntParameter *>(param);

  // Check if dynamic cast succeeds
  if(!param)
    {
    return 0;
    }

  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);
  
  QSpinBox* input = new QSpinBox;
  input->setRange(intParam->GetMinimumValue(), intParam->GetMaximumValue());
  input->setToolTip(intParam->GetDescription());
  
  QString optionID(intParam->GetName());

  hLayout->addWidget(input);
  hLayout->addStretch();

  QGroupBox *paramHGroup = new QGroupBox;
  paramHGroup->setLayout(hLayout);
  return paramHGroup;
}
}
}
