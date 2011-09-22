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

QtWidgetEmptyParameter::QtWidgetEmptyParameter(EmptyParameter* emptyParam, QtWidgetModel* m)
  : QtWidgetParameterBase(m),
    m_EmptyParam(emptyParam)
{
}

QtWidgetEmptyParameter::~QtWidgetEmptyParameter()
{
}

void QtWidgetEmptyParameter::DoUpdateGUI()
{

}

void QtWidgetEmptyParameter::DoCreateWidget()
{
  // Set up input text edit
  m_QHBoxLayout = new QHBoxLayout;
  m_QHBoxLayout->setSpacing(0);
  m_QHBoxLayout->setContentsMargins(0, 0, 0, 0);

  m_QCheckBox = new QCheckBox;
  m_QCheckBox->setToolTip(m_EmptyParam->GetDescription());
  connect(m_QCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SetValue(int)));

  QString optionID(m_EmptyParam->GetName());
  m_QHBoxLayout->addWidget(m_QCheckBox);
  m_QHBoxLayout->addStretch();

  this->setLayout(m_QHBoxLayout);
}

void QtWidgetEmptyParameter::SetValue( int value )
{
  switch(value)
    {
    case Qt::Unchecked:
      m_EmptyParam->SetValue(false);
      break;
    case Qt::PartiallyChecked:
      break;
    case Qt::Checked:
      m_EmptyParam->SetValue(true);
      break;
    }
}

}
}
