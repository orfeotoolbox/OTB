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
#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetChoiceParameter.h"
#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterGroup::QtWidgetParameterGroup(ParameterGroup::Pointer paramList, QtWidgetModel* m)
: QtWidgetParameterBase(paramList, m),
  m_ParamList(paramList)
{
}

QtWidgetParameterGroup::~QtWidgetParameterGroup()
{
}

void QtWidgetParameterGroup::DoUpdateGUI()
{
  WidgetListIteratorType it = m_WidgetList.begin();
  for (it = m_WidgetList.begin(); it != m_WidgetList.end(); ++it)
    {
    (*it)->UpdateGUI();
    }
}

void QtWidgetParameterGroup::DoCreateWidget()
{
  // a GridLayout with two columns : parameter label / parameter widget
  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setSpacing(1);
  gridLayout->setContentsMargins(0, 0, 0, 0);

  unsigned int nbParams = m_ParamList->GetNumberOfParameters();
  for (unsigned int i = 0; i < nbParams; ++i)
    {
    Parameter* param = m_ParamList->GetParameterByIndex(i);

    if (param != 0)
      {
      ParameterGroup* paramAsGroup = dynamic_cast<ParameterGroup*>(param);
      ChoiceParameter* paramAsChoice = dynamic_cast<ChoiceParameter*>(param);

      if (paramAsGroup == 0 && paramAsChoice == 0)
        {
        // Label (col 0)
        QWidget* label = new QtWidgetParameterLabel( param );
        gridLayout->addWidget(label, i, 1);

        // Parameter Widget (col 2)
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );
        gridLayout->addWidget(specificWidget, i, 2 );

        // CheckBox (col 1)
        QCheckBox * checkBox = new QCheckBox;
        connect(checkBox, SIGNAL(clicked(bool)), specificWidget, SLOT(SetActivationState(bool)));
        connect( checkBox, SIGNAL(clicked(bool)), GetModel(), SLOT(NotifyUpdate()) );
        
        if (param->IsRoot())
           {
          // if Mandatory make the checkbox checked and deactivated
          if (param->GetMandatory())
            {
            checkBox->setCheckState(Qt::Checked);
            checkBox->setEnabled(false);
            specificWidget->setEnabled(true);
            }
          else
            {
            checkBox->setCheckState(Qt::Unchecked);
            checkBox->setEnabled(true);
            specificWidget->setEnabled(false);
            }
          }
        else
          {
          // If this widget belongs to a Group, make it disabled by
          // defaut
          specificWidget->setEnabled(false);
          }
        gridLayout->addWidget(checkBox, i, 0);

        m_WidgetList.push_back(specificWidget);
        }
      else
        {
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );

        QVBoxLayout* vboxLayout = new QVBoxLayout;
        vboxLayout->addWidget(specificWidget);
        QGroupBox* group = new QGroupBox;
        group->setLayout(vboxLayout);

        // Make the paramter Group checkable when it is not mandatory
        if (!param->GetMandatory() )
          {
          group->setCheckable(true);
          }
        connect(group, SIGNAL(clicked(bool)), specificWidget, SLOT(SetActivationState(bool)));

        group->setTitle(param->GetName());
        gridLayout->addWidget(group, i, 0, 1, -1);

        m_WidgetList.push_back(specificWidget);
        }
      }
    }

  this->setLayout(gridLayout);
}


// Slot connected to the signal emitted the checkBox relative to
// current widget
void QtWidgetParameterGroup::SetActivationState( bool value )
{
  // First call the superclass implementation
  this->QtWidgetParameterBase::SetActivationState(value);

  // Update the Group status
  this->setEnabled(value);

  // Update iteratively the children status
  for (unsigned int idx = 0; idx < m_ParamList->GetChildrenList().size(); ++idx)
    {
    this->ProcessChild(m_ParamList->GetChildrenList()[idx], value);
    }
}

// Activate iteratively  the children
void QtWidgetParameterGroup::ProcessChild(Parameter* currentNode, bool status)
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
