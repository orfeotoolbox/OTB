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
#include "otbWrapperQtWidgetEmptyParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetEmptyParameter::QtWidgetEmptyParameter(EmptyParameter* emptyParam)
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QCheckBox* checkbox = new QCheckBox;
  checkbox->setToolTip(emptyParam->GetDescription());

  QString optionID(emptyParam->GetName());
  hLayout->addWidget(checkbox);
  hLayout->addStretch();

  QGroupBox *paramHGroup = new QGroupBox;
  paramHGroup->setLayout(hLayout);
}

QtWidgetEmptyParameter::~QtWidgetEmptyParameter()
{
}

}
}
