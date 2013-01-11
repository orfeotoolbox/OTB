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
#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

QtWidgetOutputVectorDataParameter::QtWidgetOutputVectorDataParameter(OutputVectorDataParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_OutputVectorDataParam(param)
{
}

QtWidgetOutputVectorDataParameter::~QtWidgetOutputVectorDataParameter()
{
}

void QtWidgetOutputVectorDataParameter::DoUpdateGUI()
{

}

void QtWidgetOutputVectorDataParameter::DoCreateWidget()
{
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);

  m_Input = new QLineEdit;
  m_Input->setToolTip( m_OutputVectorDataParam->GetDescription() );
  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
  m_HLayout->addWidget(m_Input);

  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select output filename...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

void QtWidgetOutputVectorDataParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setNameFilter("Vector data files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    m_Input->setText(fileDialog.selectedFiles().at(0));
    }
}

void QtWidgetOutputVectorDataParameter::SetFileName(const QString& value)
{
  // save value
  m_FileName = value.toStdString();

  m_OutputVectorDataParam->SetFileName(m_FileName);

  // notify of value change
  QString key( m_OutputVectorDataParam->GetKey() );
  emit ParameterChanged(key);
}


}
}
