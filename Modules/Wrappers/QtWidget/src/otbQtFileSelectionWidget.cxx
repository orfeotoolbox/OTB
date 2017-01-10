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
#include "otbQtFileSelectionWidget.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtFileSelectionWidget
::QtFileSelectionWidget() :
  QWidget(),
  m_HLayout( NULL ),
  m_Input( NULL ),
  m_Button( NULL ),
  m_Checkbox( NULL ),
  m_IOMode( IO_MODE_INPUT )
{
  this->DoCreateWidget();
}

QtFileSelectionWidget::~QtFileSelectionWidget()
{
}

void
QtFileSelectionWidget
::SetIOMode( IOMode mode )
{
  m_IOMode = mode;
}

QtFileSelectionWidget::IOMode
QtFileSelectionWidget
::GetIOMode() const
{
  return m_IOMode;
}

void QtFileSelectionWidget::DoUpdateGUI()
{

}

void QtFileSelectionWidget::DoCreateWidget()
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

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setFixedWidth(30);

  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}


void
QtFileSelectionWidget
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    m_IOMode == IO_MODE_INPUT
    ? GetOpenFileName(
        this,
	QString(),
	m_Input->text(),
	tr( "All files (*)" ),
	NULL,
	QFileDialog::ReadOnly )
    : GetSaveFileName(
        this,
	QString(),
	m_Input->text(),
	tr( "All files (*)" ),
	NULL )
  );

  if( filename.isEmpty() )
    return;

  m_Input->setText( filename  );
}


}

}
