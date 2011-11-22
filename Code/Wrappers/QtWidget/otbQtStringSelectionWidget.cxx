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
#include "otbQtStringSelectionWidget.h"

namespace otb
{
namespace Wrapper
{

QtStringSelectionWidget::QtStringSelectionWidget()
  : QWidget()
{
  this->DoCreateWidget();
}

QtStringSelectionWidget::~QtStringSelectionWidget()
{
}

void QtStringSelectionWidget::DoUpdateGUI()
{

}

void QtStringSelectionWidget::DoCreateWidget()
{
  unsigned int sp = 2;
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(sp);
  m_HLayout->setContentsMargins(sp, sp, sp, sp);

  m_Checkbox = new QCheckBox();
  m_HLayout->addWidget(m_Checkbox);

  m_Input = new QLineEdit;

  m_HLayout->addWidget(m_Input);

  this->setLayout(m_HLayout);
}



}
}
