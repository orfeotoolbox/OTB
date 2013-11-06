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
#include <stdexcept>

namespace otb
{
namespace Wrapper
{

QtWidgetInputImageParameter::QtWidgetInputImageParameter(InputImageParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_InputImageParam(param)
{
}

QtWidgetInputImageParameter::~QtWidgetInputImageParameter()
{
}

void QtWidgetInputImageParameter::DoUpdateGUI()
{
  //update lineedit if HasUserValue flag is set(from xml)
  if(m_InputImageParam->HasUserValue())
    {
    QString text( m_InputImageParam->GetFileName().c_str() );
    if (text != m_Input->text())
      m_Input->setText(text);
    }
}

void QtWidgetInputImageParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip( m_InputImageParam->GetDescription() );
  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );

  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

void QtWidgetInputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    if ( this->SetFileName(fileDialog.selectedFiles().at(0)) == true )
      m_Input->setText(fileDialog.selectedFiles().at(0));
    else
      {
      std::ostringstream oss;
      oss << "The given file "
          << fileDialog.selectedFiles().at(0).toAscii().constData()
          << " is not valid.";
      this->GetModel()->SendLogWARNING( oss.str() );
      }
    }
}

bool QtWidgetInputImageParameter::SetFileName(const QString& value)
{
  bool res = true;
  // save value
  if(m_InputImageParam->SetFromFileName(value.toAscii().constData()) == true)
    {
    // notify of value change
    QString key( m_InputImageParam->GetKey() );

    emit ParameterChanged(key);
    }
  else
    res = false;

  return res;
}

}
}
