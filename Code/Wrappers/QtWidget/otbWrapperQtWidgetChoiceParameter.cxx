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
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QComboBox* combobox = new QComboBox;
  combobox->setToolTip(m_ChoiceParam->GetDescription());

  for (unsigned int i = 0; i < m_ChoiceParam->GetNbChoices(); ++i)
    {
    QString key = QString::fromStdString( m_ChoiceParam->GetChoiceKey(i) );
    combobox->addItem( key, QVariant(key) );
    }

  connect( combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetValue(int)) );

  hLayout->addWidget(combobox);
  this->setLayout(hLayout);
}

void QtWidgetChoiceParameter::SetValue(int value)
{
  m_ChoiceParam->SetValue( value );
}


}
}
