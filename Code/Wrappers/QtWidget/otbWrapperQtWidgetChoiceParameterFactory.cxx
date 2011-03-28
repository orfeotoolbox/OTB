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

#include "otbWrapperQtWidgetChoiceParameterFactory.h"
#include "otbWrapperChoiceParameter.h"


namespace otb
{

namespace Wrapper
{

QtWidgetChoiceParameterFactory::QtWidgetChoiceParameterFactory()
{
}

QtWidgetChoiceParameterFactory::~QtWidgetChoiceParameterFactory()
{
}

QWidget * QtWidgetChoiceParameterFactory::CreateQtWidget(Parameter * param)
{
  // Try to cast to choice parameter
  ChoiceParameter * choiceParam = dynamic_cast<ChoiceParameter *>(param);

  // Check if dynamic cast succeeds
  if(!choiceParam)
    {
    return 0;
    }

  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);

  QComboBox* combobox = new QComboBox;
  combobox->setToolTip(choiceParam->GetDescription());


  for (unsigned int i = 0; i < choiceParam->GetNbChoice(); ++i)
    {
    combobox->addItem( "test", QVariant("test") );
    }

  hLayout->addWidget(combobox);
  QGroupBox *paramHGroup = new QGroupBox;
  paramHGroup->setLayout(hLayout);
  return paramHGroup;

}
}
}
