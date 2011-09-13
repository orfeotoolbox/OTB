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
: QtWidgetParameterBase(m),
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
        QWidget* label = new QtWidgetParameterLabel( param );
        gridLayout->addWidget(label, i, 0);
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );
        gridLayout->addWidget(specificWidget, i, 1);
        m_WidgetList.push_back(specificWidget);
        }
      else
        {
        QtWidgetParameterBase* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );

        QVBoxLayout* vboxLayout = new QVBoxLayout;
        vboxLayout->addWidget(specificWidget);
        QGroupBox* group = new QGroupBox;
        group->setLayout(vboxLayout);
        group->setTitle(param->GetName());
        gridLayout->addWidget(group, i, 0, 1, -1);

        m_WidgetList.push_back(specificWidget);
        }
      }
    }

  this->setLayout(gridLayout);
}

}
}
