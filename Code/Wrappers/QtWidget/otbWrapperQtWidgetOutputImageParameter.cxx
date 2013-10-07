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
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

QtWidgetOutputImageParameter::QtWidgetOutputImageParameter(OutputImageParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_OutputImageParam(param)
{
}

QtWidgetOutputImageParameter::~QtWidgetOutputImageParameter()
{
}

void QtWidgetOutputImageParameter::DoUpdateGUI()
{
  // Update the lineEdit
  if (m_Input->text() != m_OutputImageParam->GetFileName())
    {
    m_Input->setText(m_OutputImageParam->GetFileName());
    }
}

void QtWidgetOutputImageParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit();
  m_Input->setToolTip( m_OutputImageParam->GetDescription() );
  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
  m_HLayout->addWidget(m_Input);

  // Set the Output PixelType choice Combobox
  m_ComboBox = new QComboBox;
  m_ComboBox->setToolTip("Output Pixel Type");
  m_ComboBox->addItem( "uint 8");
  m_ComboBox->addItem( "int 16");
  m_ComboBox->addItem( "uint 16");
  m_ComboBox->addItem( "int 32");
  m_ComboBox->addItem( "uint 32");
  m_ComboBox->addItem( "float");
  m_ComboBox->addItem( "double");
  m_ComboBox->setCurrentIndex(m_OutputImageParam->GetPixelType());
  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetPixelType(int)) );
  connect( m_ComboBox, SIGNAL(currentIndexChanged(int)), GetModel(), SLOT(NotifyUpdate()) );
  m_HLayout->addWidget(m_ComboBox);

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select output filename...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

void QtWidgetOutputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    m_Input->setText(fileDialog.selectedFiles().at(0));
    m_OutputImageParam->SetUseXMLValue(false);
    }
}

void QtWidgetOutputImageParameter::SetFileName(const QString& value)
{
  // save value
  m_FileName = value.toAscii().constData();

  m_OutputImageParam->SetFileName(m_FileName);

  // notify of value change
  QString key( m_OutputImageParam->GetKey() );
  emit ParameterChanged(key);
}

void QtWidgetOutputImageParameter::SetPixelType(int  pixelType)
{
  m_OutputImageParam->SetPixelType(static_cast< ImagePixelType >(pixelType));
  m_PixelType = pixelType;
}

}
}
