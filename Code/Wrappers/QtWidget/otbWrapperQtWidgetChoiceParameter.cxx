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

QtWidgetChoiceParameter::QtWidgetChoiceParameter( ChoiceParameter* param,
						  QtWidgetModel* m ) :
  QtWidgetParameterBase(param, m),
  m_ChoiceParam(param),
  m_ComboBox( NULL ),
  m_StackWidget( NULL ),
  m_VLayout( NULL ),
  m_VLayoutGroup( NULL ),
  m_WidgetList()
{
}

QtWidgetChoiceParameter::~QtWidgetChoiceParameter()
{
}

void QtWidgetChoiceParameter::DoUpdateGUI()
{
  // Update the combobox value
  unsigned int value = m_ChoiceParam->GetValue( );
  m_ComboBox->setCurrentIndex(value);

  // Update the choice subparameters
  WidgetListIteratorType it = m_WidgetList.begin();
  for (it = m_WidgetList.begin(); it != m_WidgetList.end(); ++it)
    {
    (*it)->UpdateGUI();
    }
}

void QtWidgetChoiceParameter::DoCreateWidget()
{
  m_ComboBox = new QComboBox;
  m_ComboBox->setToolTip(m_ChoiceParam->GetDescription());

  m_StackWidget = new QStackedWidget;

  for (unsigned int i = 0; i < m_ChoiceParam->GetNbChoices(); ++i)
    {
    QString key = m_ChoiceParam->GetChoiceName(i).c_str();
    m_ComboBox->addItem( key, QVariant(key) );

    ParameterGroup::Pointer param = m_ChoiceParam->GetChoiceParameterGroupByIndex(i);
    if (param.IsNotNull())
      {
      QtWidgetParameterBase* widget =
	QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );

      m_StackWidget->addWidget(widget);

      m_WidgetList.push_back(widget);
      }
    }

  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetValue(int)) );
  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), m_StackWidget, SLOT(setCurrentIndex(int)) );
  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), GetModel(), SLOT(NotifyUpdate()) );

  m_VLayout = new QVBoxLayout;
  m_VLayout->addWidget(m_ComboBox);
  if (!m_WidgetList.empty())
    {
    m_VLayout->addWidget(m_StackWidget);
    }
  m_VLayout->addStretch();

  this->setLayout(m_VLayout);
}

void QtWidgetChoiceParameter::SetValue(int value)
{
  m_ChoiceParam->SetValue( value );
}

}
}
