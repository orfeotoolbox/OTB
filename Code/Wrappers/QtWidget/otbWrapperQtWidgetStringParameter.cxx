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

QtWidgetStringParameter::QtWidgetStringParameter(StringParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_StringParam(param)
{
}

QtWidgetStringParameter::~QtWidgetStringParameter()
{
}


void QtWidgetStringParameter::DoUpdateGUI()
{

}

void QtWidgetStringParameter::DoCreateWidget()
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QLineEdit* input = new QLineEdit;
  input->setToolTip(m_StringParam->GetDescription());
  hLayout->addWidget(input);

  connect( input, SIGNAL(textChanged(const QString&)), this, SLOT(SetValue(const QString&)) );

  this->setLayout(hLayout);
}

void QtWidgetStringParameter::SetValue(const QString& value)
{
  std::cout << "QtWidgetStringParameter::SetValue " << m_StringParam->GetKey() << " " << value.toStdString() << std::endl;
  m_StringParam->SetValue(value.toStdString());

  QString key( QString::fromStdString(m_StringParam->GetKey()) );
  emit ParameterChanged(key);
}

}
}
