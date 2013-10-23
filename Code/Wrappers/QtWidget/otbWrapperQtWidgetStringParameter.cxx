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

  // Update the lineEdit only if there is a change and thats not empty or whitespaces
  QString text( m_StringParam->GetValue().c_str() );
  if (!text.trimmed().isEmpty() && text != m_Input->text())
    {
    m_Input->setText(text);
    }
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

  this->setLayout(m_HLayout);

  if (m_StringParam->GetRole() == Role_Output)
    {
    m_Input->setEnabled( false );
    }
}

void QtWidgetStringParameter::SetValue(const QString& value)
{

  m_StringParam->SetValue(value.toAscii().constData());

  if (m_StringParam->GetValue() != value.toAscii().constData())
    {
    QString key( m_StringParam->GetKey() );
    emit ParameterChanged(key);
    }
  /** moved to ParameterChanged slot in QtWidgetParameterBase:: **/
  //m_StringParam->SetUserValue(true);

}

}
}
