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
#include "otbWrapperQtWidgetInputImageParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetInputImageParameter::QtWidgetInputImageParameter(InputImageParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_InputImageParam(param)
{
  this->CreateWidget();
}

QtWidgetInputImageParameter::~QtWidgetInputImageParameter()
{
}

void QtWidgetInputImageParameter::CreateWidget()
{
  // Set up input text edit
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->setSpacing(0);
  hLayout->setContentsMargins(0,0,0,0);
  QLineEdit* input = new QLineEdit;
  input->setToolTip( m_InputImageParam->GetDescription() );
  connect( input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  hLayout->addWidget(input);

  // Set up input text edit
  QPushButton *button = new QPushButton;
  button->setText("...");
  button->setToolTip("Select file...");
  button->setMaximumWidth(button->width());
  connect( button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  hLayout->addWidget(button);

  this->setLayout(hLayout);
}

void QtWidgetInputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    this->SetFileName(fileDialog.selectedFiles().at(0));
    }
}

void QtWidgetInputImageParameter::SetFileName(const QString& value)
{
  // save value
  m_InputImageParam->SetFromFileName(value.toStdString());

  // notify of value change
  QString key( QString::fromStdString(m_InputImageParam->GetKey()) );
  emit ParameterChanged(key);
}

}
}
