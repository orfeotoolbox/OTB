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

namespace otb
{
namespace Wrapper
{

QtFileSelectionWidget::QtFileSelectionWidget()
  : QWidget()
{
  this->DoCreateWidget();
}

QtFileSelectionWidget::~QtFileSelectionWidget()
{
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

void QtFileSelectionWidget::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("All files (*)");

  if (fileDialog.exec())
    {
    QString filemane(fileDialog.selectedFiles().at(0));
    m_Input->setText(filemane);
    }
}


}
}
