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
      OutputProcessXMLParameter* paramAsInXML = dynamic_cast<OutputProcessXMLParameter*>(param);
      InputProcessXMLParameter* paramAsOutXML = dynamic_cast<InputProcessXMLParameter*>(param);

      bool paramIsXML = false;
      if(paramAsInXML != 0 || paramAsOutXML != 0)
        paramIsXML = true;

      if (paramAsGroup == 0 && paramAsChoice == 0 && !paramIsXML)
        {
        // Label (col 1)
        QWidget* label = new QtWidgetParameterLabel( param );
        gridLayout->addWidget(label, i, 1);

        // Parameter Widget (col 2)
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );
        gridLayout->addWidget(specificWidget, i, 2 );

        // CheckBox (col 0)
        QCheckBox * checkBox = new QCheckBox;
        connect( checkBox, SIGNAL(clicked(bool)), specificWidget, SLOT(SetActivationState(bool)));
        connect( checkBox, SIGNAL(clicked(bool)), GetModel(), SLOT(NotifyUpdate()) );
        connect( specificWidget, SIGNAL(ParameterActiveStatus(bool)), checkBox, SLOT(setChecked(bool)));
        connect( specificWidget, SIGNAL(ParameterActiveStatus(bool)), specificWidget, SLOT(SetActivationState(bool)));

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
        gridLayout->addWidget(checkBox, i, 0);

        // Reset Button
        // Make sense only for NumericalParameter
        if (dynamic_cast<IntParameter*>(param)
            || dynamic_cast<FloatParameter*>(param)
            || dynamic_cast<RadiusParameter*>(param)
            /*|| dynamic_cast<RAMParameter*>(param)*/)
          {
          if( param->GetRole() != Role_Output )
            {
            QPushButton* resetButton = new QPushButton;
            resetButton->setText("Reset");
            resetButton->setToolTip("Reset the value of this parameter");
            gridLayout->addWidget(resetButton, i, 3);
            
            // Slots to connect to the reset button
            connect( resetButton, SIGNAL(clicked()), specificWidget, SLOT(Reset()) );
            connect( resetButton, SIGNAL(clicked()), GetModel(), SLOT(NotifyUpdate()) );
            }
          }

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
          group->setChecked(false);

          // Update iteratively the children status
          for (unsigned int idx = 0; idx < param->GetChildrenList().size(); ++idx)
            {
            // deactivate the children tree
            this->ProcessChild(param->GetChildrenList()[idx], false);
            }
          }
        else
          {
          param->SetActive(true);
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
