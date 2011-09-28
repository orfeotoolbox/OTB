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
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterBase::QtWidgetParameterBase(Parameter * param, QtWidgetModel* m)
  : m_Model(m), m_Param(param)
{
}

QtWidgetParameterBase::~QtWidgetParameterBase()
{
}

void QtWidgetParameterBase::CreateWidget()
{
  this->DoCreateWidget();
}

void QtWidgetParameterBase::UpdateGUI()
{
  this->DoUpdateGUI();
}

void QtWidgetParameterBase::ParameterChanged(const QString& key)
{

}

QtWidgetModel* QtWidgetParameterBase::GetModel()
{
  return m_Model;
}

// Slot connected to the signal emitted the checkBox relative to
// current widget
void QtWidgetParameterBase::SetActivationState( bool value )
{
  this->setEnabled(value);
  m_Param->SetChecked(value);
  m_Param->SetActive(value);
}
}
