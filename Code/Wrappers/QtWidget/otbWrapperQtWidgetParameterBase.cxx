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

  // connect the update signal to this widget
  connect( GetModel(), SIGNAL(UpdateGui()), this, SLOT(UpdateGUI() ) );
}

void QtWidgetParameterBase::UpdateGUI()
{
  // Update the GUI
  this->DoUpdateGUI();

  // Emit State of the Parameter to update this paramter checkbox
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

void QtWidgetParameterBase::ParameterChanged(const QString& key)
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
  m_Param->SetChecked(value);
  m_Param->SetActive(value);
}

// Slot connected to the signal emitted by the Reset Button
void QtWidgetParameterBase::Reset(  )
{
  m_Param->Reset();
  m_Param->SetUserValue(false);
  m_Param->SetAutomaticValue(false);
  this->UpdateGUI();
}
}
}
