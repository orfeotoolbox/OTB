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
  : m_Param(param), m_Model(m)
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
void QtWidgetParameterBase::SetValue( int value )
{
  switch(value)
    {
    case Qt::Unchecked:
      this->setEnabled(false);
      m_Param->SetChecked(false);
      m_Param->SetActive(false);
      break;
    case Qt::PartiallyChecked:
      break;
    case Qt::Checked:
      this->setEnabled(true);
      m_Param->SetChecked(true);
      m_Param->SetActive(true);
      break;
    }
}

// A slot connected to a signal emitted by a ParameterGroup
// we need to modify the children availabitily status
void QtWidgetParameterBase::SetValue( bool v)
{
  // Update the Group status
  this->setEnabled(v);

  // Update iteratively the children status
  for (unsigned int idx = 0; idx < m_Param->GetChildrenList().size(); ++idx)
    {
    this->ProcessChild(m_Param->GetChildrenList()[idx], v);
    }
}

// Activate iteratively  the children
void QtWidgetParameterBase::ProcessChild(Parameter* currentNode, bool status)
{
  // Activate the current node if it was checked
  if ( currentNode->IsChecked() && status)
    {
    currentNode->SetActive(status);
    }

  // If the status is false (deactivating) deactivate all the children
  // tree
  if (!status)
    {
    currentNode->SetActive(status);
    }

  unsigned int counter = 0;
  while(counter < currentNode->GetChildrenList().size())
    {
    this->ProcessChild(currentNode->GetChildrenList()[counter], status);
    ++counter;
    }
}

}
}
