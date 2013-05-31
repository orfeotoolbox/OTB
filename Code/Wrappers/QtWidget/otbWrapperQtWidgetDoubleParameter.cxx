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
#include "otbWrapperQtWidgetDoubleParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetDoubleParameter::QtWidgetDoubleParameter(DoubleParameter* doubleParam, QtWidgetModel* m)
: QtWidgetParameterBase(doubleParam, m),
  m_DoubleParam(doubleParam)
{
}

QtWidgetDoubleParameter::~QtWidgetDoubleParameter()
{
}

void QtWidgetDoubleParameter::DoUpdateGUI()
{
  // Update the valid range if updated
  m_QDoubleSpinBox->setRange(m_DoubleParam->GetMinimumValue(),
                             m_DoubleParam->GetMaximumValue());

  bool signalsBlocked = m_QDoubleSpinBox->blockSignals( true );

  if (m_DoubleParam->HasValue())
    {
    m_QDoubleSpinBox->setValue(m_DoubleParam->GetValue());
    }
  m_QDoubleSpinBox->blockSignals( signalsBlocked );

  QFont font = m_QDoubleSpinBox->font();
  if (m_DoubleParam->HasUserValue())
    {
    font.setBold(true);
    }
  else
    {
    font.setBold(false);
    }

  m_QDoubleSpinBox->setFont(font);
}

void QtWidgetDoubleParameter::DoCreateWidget()
{
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QDoubleSpinBox = new QDoubleSpinBox;
  m_QDoubleSpinBox->setDecimals(5);
  m_QDoubleSpinBox->setSingleStep(0.1);
  m_QDoubleSpinBox->setRange(m_DoubleParam->GetMinimumValue(), m_DoubleParam->GetMaximumValue());
  m_QDoubleSpinBox->setToolTip(m_DoubleParam->GetDescription());

  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetValue(double)) );
  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), GetModel(), SLOT(NotifyUpdate()) );

  m_QHBoxLayout->addWidget(m_QDoubleSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

  if (m_DoubleParam->GetRole() == Role_Output)
    {
    m_QDoubleSpinBox->setEnabled( false );
    }
}

void QtWidgetDoubleParameter::SetValue(double value)
{
  m_DoubleParam->SetValue(value);
  m_DoubleParam->SetUserValue(true);
  m_DoubleParam->SetAutomaticValue(false);
}

}
}
