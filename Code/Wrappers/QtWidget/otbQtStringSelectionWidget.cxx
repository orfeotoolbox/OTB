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
  m_Checkbox->setChecked(true);
  m_HLayout->addWidget(m_Checkbox);

  m_Input = new QLineEdit;
  m_Input->setEnabled( m_Checkbox->isChecked() );

  m_HLayout->addWidget(m_Input);

  QObject::connect(
          m_Checkbox, SIGNAL( toggled( bool ) ),
          m_Input, SLOT( setEnabled( bool ) )
  );

  QObject::connect(
        m_Input,
        SIGNAL( editingFinished( ) ),
        this,
        SLOT( OnEditionFinished() )
        );

  this->setLayout(m_HLayout);
}

void QtStringSelectionWidget::OnEditionFinished()
{
  // used to propagate m_Input (QLineEdit type) editingFinished signal
  emit InternalQLineEditEditionFinished();
}

}

}
