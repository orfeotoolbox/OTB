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

QtWidgetIntParameter::QtWidgetIntParameter(IntParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_IntParam(param)
{
}

QtWidgetIntParameter::~QtWidgetIntParameter()
{
}

void QtWidgetIntParameter::DoCreateWidget()
{
  // Set up input text edit
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QSpinBox = new QSpinBox;
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(), m_IntParam->GetMaximumValue());
  m_QSpinBox->setToolTip(m_IntParam->GetDescription());

  connect( m_QSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetValue(int)) );
  connect( m_QSpinBox, SIGNAL(valueChanged(int)), GetModel(), SLOT(NotifyUpdate()) );

  m_QHBoxLayout->addWidget(m_QSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_IntParam->GetRole() == Role_Output)
    {
    m_QSpinBox->setEnabled( false );
    }
}

void QtWidgetIntParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QSpinBox->setRange(m_IntParam->GetMinimumValue(),
                       m_IntParam->GetMaximumValue());

  bool signalsBlocked = m_QSpinBox->blockSignals( true );

  if (m_IntParam->HasValue())
    {
    m_QSpinBox->setValue(m_IntParam->GetValue());
    }
  m_QSpinBox->blockSignals( signalsBlocked );

  QFont font = m_QSpinBox->font();
  if (m_IntParam->HasUserValue())
    {
    font.setBold(true);
    }
  else
    {
    font.setBold(false);
    }
  m_QSpinBox->setFont(font);
}

void QtWidgetIntParameter::SetValue(int value)
{
  m_IntParam->SetValue(value);

  /** moved to ParameterChanged slot in QtWidgetParameterBase:: **/
  /**m_IntParam->SetUserValue(true); **/

  QString key( m_IntParam->GetKey() );
  emit ParameterChanged(key);

  m_IntParam->SetAutomaticValue(false);
}

}
}
