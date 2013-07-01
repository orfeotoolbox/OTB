/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdWrapperQtWidgetChoiceParameter.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperQtWidgetParameterLabel.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdWrapperQtWidgetParameterFactory.h"

namespace mvd
{
namespace Wrapper
{

QtWidgetChoiceParameter
::QtWidgetChoiceParameter( otb::Wrapper::ChoiceParameter* param, 
			   otb::Wrapper::QtWidgetModel* m) :
  otb::Wrapper::QtWidgetParameterBase(param, m),
  m_ChoiceParam(param),
  m_MainHLayout( NULL ),
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

    otb::Wrapper::ParameterGroup::Pointer param = m_ChoiceParam->GetChoiceParameterGroupByIndex(i);
    if (param.IsNotNull())
      {
      otb::Wrapper::QtWidgetParameterBase* widget 
        = QtWidgetParameterFactory::CreateQtWidget( param, GetModel() );
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
