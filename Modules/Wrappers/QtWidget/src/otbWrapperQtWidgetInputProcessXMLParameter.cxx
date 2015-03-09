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
#include "otbWrapperQtWidgetInputProcessXMLParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetInputProcessXMLParameter::QtWidgetInputProcessXMLParameter(InputProcessXMLParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_XMLParam(param)
{
}

QtWidgetInputProcessXMLParameter::~QtWidgetInputProcessXMLParameter()
{
}

void QtWidgetInputProcessXMLParameter::DoUpdateGUI()
{
  // Update the lineEdit
  QString text( m_XMLParam->GetFileName() );
  if (text != m_Input->text())
    m_Input->setText(text);
}

void QtWidgetInputProcessXMLParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip( m_XMLParam->GetDescription() );
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

void QtWidgetInputProcessXMLParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setNameFilter("XML File (*.xml)");

  if (fileDialog.exec())
    {
    QString name = fileDialog.selectedFiles().at(0);
    if( !name.isEmpty() )
      {
      this->SetFileName(name);
      m_Input->setText(name);
      }
    }
}

void QtWidgetInputProcessXMLParameter::SetFileName(const QString& value)
{
  // load xml file name
  m_XMLParam->SetValue(value.toAscii().constData());

  // notify of value change
  QString key( m_XMLParam->GetKey() );

  emit ParameterChanged(key);

  GetModel()->UpdateAllWidgets();

}

}

}
