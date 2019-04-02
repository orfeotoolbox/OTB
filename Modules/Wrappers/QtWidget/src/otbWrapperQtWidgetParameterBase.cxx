/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterBase::QtWidgetParameterBase(Parameter * param, QtWidgetModel* m, QWidget * parent)
  : QWidget(parent)
  , m_Model(m)
  , m_Param(param)
  , m_IsChecked( false )
{

}

QtWidgetParameterBase::~QtWidgetParameterBase()
{
}


void QtWidgetParameterBase::CreateWidget()
{
  // Connect the model update gui signal to this widget update gui slot
  connect( GetModel(), &QtWidgetModel::UpdateGui, this, &QtWidgetParameterBase::UpdateGUI );

  this->DoCreateWidget();
}

void QtWidgetParameterBase::UpdateGUI()
{
  // Update the GUI
  this->DoUpdateGUI();

  // Emit State of the Parameter to update this parameter checkbox
  if (!m_Param->GetMandatory())
    {
    bool state = m_Param->GetActive();
    emit ParameterActiveStatus(state);
   }
  else
    {
    // Activate the Mandatory param and the Mandatory subparam in an
    // activated Group
   if (m_Param->GetRoot()->GetActive())
     {
     emit ParameterActiveStatus(true);
     }
    }
}

void QtWidgetParameterBase::ParameterChanged(const QString& itkNotUsed(key))
{
  m_Param->SetUserValue(true);
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
  this->SetChecked(value);
  m_Param->SetActive(value);
}

const Parameter *
QtWidgetParameterBase
::GetParam() const
{
  return m_Param;
}

Parameter *
QtWidgetParameterBase
::GetParam()
{
  return m_Param;
}

// Used to block mouse wheel events to avoid conflict with scrolling in the parent QScrollArea
bool QtWidgetParameterBase::eventFilter(QObject* o, QEvent* e)
{
  if (e->type() == QEvent::Wheel)
  {
    std::cout << "ignoring mouse wheel event\n";
    e->ignore();
    return true;
  }
  return QWidget::eventFilter(o, e);
}


}

}
