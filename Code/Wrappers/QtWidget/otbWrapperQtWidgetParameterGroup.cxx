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
#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterGroup::QtWidgetParameterGroup(ParameterList::Pointer paramList)
: m_ParamList(paramList)
{
  this->CreateWidget();
}

QtWidgetParameterGroup::~QtWidgetParameterGroup()
{
}

void QtWidgetParameterGroup::CreateWidget()
{
  // a GridLayout with two colums : parameter label / parameter widget
  QGridLayout *gridLayout = new QGridLayout;
  gridLayout->setSpacing(1);
  gridLayout->setContentsMargins(0,0,0,0);

  unsigned int nbParams = m_ParamList->GetNumberOfParameters();
  for (unsigned int i = 0; i < nbParams; ++i)
    {
    Parameter* param = m_ParamList->GetParameter(i);
    QWidget* label = new QtWidgetParameterLabel( param );
    gridLayout->addWidget(label, i, 0);
    QWidget* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param );
    gridLayout->addWidget(specificWidget, i, 1);
    }

  this->setLayout(gridLayout);
}

}
}
