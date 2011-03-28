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
#include "otbWrapperQtWidgetStringParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetStringParameter::QtWidgetStringParameter(StringParameter* param)
: m_StringParam(param)
{
  this->CreateWidget();
}

QtWidgetStringParameter::~QtWidgetStringParameter()
{
}


void QtWidgetStringParameter::CreateWidget()
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QLineEdit* input = new QLineEdit;
  input->setToolTip(m_StringParam->GetDescription());
  hLayout->addWidget(input);

  this->setLayout(hLayout);
}

}
}
