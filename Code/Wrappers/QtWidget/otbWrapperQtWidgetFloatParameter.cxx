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
#include "otbWrapperQtWidgetFloatParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetFloatParameter::QtWidgetFloatParameter(FloatParameter* floatParam, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_FloatParam(floatParam)
{
}

QtWidgetFloatParameter::~QtWidgetFloatParameter()
{
}

void QtWidgetFloatParameter::DoUpdateGUI()
{
  bool signalsBlocked = m_QDoubleSpinBox->blockSignals( true );
  m_QDoubleSpinBox->setValue(m_FloatParam->GetValue());
  m_QDoubleSpinBox->blockSignals( signalsBlocked );
}

void QtWidgetFloatParameter::DoCreateWidget()
{
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0,0,0,0);

  m_QDoubleSpinBox = new QDoubleSpinBox;
  m_QDoubleSpinBox->setDecimals(5);
  m_QDoubleSpinBox->setRange(m_FloatParam->GetMinimumValue(), m_FloatParam->GetMaximumValue());
  m_QDoubleSpinBox->setToolTip(m_FloatParam->GetDescription());

  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetValue(double)) );
  connect( m_QDoubleSpinBox, SIGNAL(valueChanged(double)), GetModel(), SLOT(NotifyUpdate()) );

  m_QHBoxLayout->addWidget(m_QDoubleSpinBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);

}

void QtWidgetFloatParameter::SetValue(double value)
{
  m_FloatParam->SetValue( static_cast<float>(value) );
}

}
}
