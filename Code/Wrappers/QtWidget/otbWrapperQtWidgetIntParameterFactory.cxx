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
  if(!intParam)
    {
    return 0;
    }
  
  // a GridLayout with two colums : option name / option inputs
  QGroupBox *gridGroup = new QGroupBox;
  gridGroup->setFlat(true);

  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setSpacing(1);
  gridLayout->setContentsMargins(0,0,0,0);
  gridGroup->setLayout(gridLayout);
  
  QSpinBox* input = new QSpinBox;
  input->setRange(intParam->GetMinimumValue(), intParam->GetMaximumValue());
  input->setToolTip(intParam->GetDescription());

  // Set up label
  QLabel *label = new QLabel;
  label->setText(intParam->GetName());
  label->setToolTip(intParam->GetDescription());

  gridLayout->addWidget(label,0,0);
  gridLayout->addWidget(input,0,1);

  return gridGroup;
}
}
}
