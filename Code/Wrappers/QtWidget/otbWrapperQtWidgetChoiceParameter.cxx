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
#include "otbWrapperQtWidgetChoiceParameter.h"

#include "otbWrapperQtWidgetParameterLabel.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetChoiceParameter::QtWidgetChoiceParameter(ChoiceParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_ChoiceParam(param)
{
  this->CreateWidget();
}

QtWidgetChoiceParameter::~QtWidgetChoiceParameter()
{
}

void QtWidgetChoiceParameter::CreateWidget()
{
  m_ComboBox = new QComboBox;
  m_ComboBox->setToolTip(m_ChoiceParam->GetDescription());

  m_StackWidget = new QStackedWidget;

  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetValue(int)) );
  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), m_StackWidget, SLOT(setCurrentIndex(int)) );


  for (unsigned int i = 0; i < m_ChoiceParam->GetNbChoices(); ++i)
    {
    QString key = QString::fromStdString( m_ChoiceParam->GetChoiceKey(i) );
    m_ComboBox->addItem( key, QVariant(key) );

    Parameter::Pointer param = m_ChoiceParam->GetChoiceAssociatedParameter(i);
    if (param.IsNotNull())
      {
      std::cout << param->GetName() << std::endl;
      QWidget* label = new QtWidgetParameterLabel( param );
      QWidget* specificWidget = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );
      QHBoxLayout* hbox = new QHBoxLayout;
      hbox->addWidget(label);
      hbox->addWidget(specificWidget);
      QGroupBox* group = new QGroupBox;
      group->setLayout(hbox);
      m_StackWidget->addWidget(group);
      }
    }

  m_VLayout = new QVBoxLayout;
  m_VLayout->addWidget(m_ComboBox);
  m_VLayout->addWidget(m_StackWidget);

  m_VLayoutGroup = new QGroupBox;
  m_VLayoutGroup->setLayout(m_VLayout);
  m_VLayoutGroup->setFlat(true);

  m_MainHLayout = new QHBoxLayout;
  m_MainHLayout->setSpacing(0);
  m_MainHLayout->setContentsMargins(0,0,0,0);
  m_MainHLayout->addWidget(m_VLayoutGroup);
  this->setLayout(m_MainHLayout);
}

void QtWidgetChoiceParameter::SetValue(int value)
{
  m_ChoiceParam->SetValue( value );
}

}
}
