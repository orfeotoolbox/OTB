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
: QtWidgetParameterBase(param, m),
  m_StringParam(param)
{
}

QtWidgetStringParameter::~QtWidgetStringParameter()
{
}


void QtWidgetStringParameter::DoUpdateGUI()
{
  m_Input->setToolTip(m_StringParam->GetDescription());
}

void QtWidgetStringParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);

  m_Input = new QLineEdit;
  m_Input->setToolTip(m_StringParam->GetDescription());
  m_HLayout->addWidget(m_Input);

  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetValue(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
  connect( GetModel(), SIGNAL(UpdateGui()), this, SLOT(UpdateGUI() ) );

  this->setLayout(m_HLayout);
}

void QtWidgetStringParameter::SetValue(const QString& value)
{
  m_StringParam->SetValue(value.toStdString());
  m_StringParam->SetUserValue(true);
  QString key( QString::fromStdString(m_StringParam->GetKey()) );
  emit ParameterChanged(key);
}

}
}
