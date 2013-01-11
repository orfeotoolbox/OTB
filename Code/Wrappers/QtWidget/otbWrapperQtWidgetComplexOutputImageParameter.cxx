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
#include "otbWrapperQtWidgetComplexOutputImageParameter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

QtWidgetComplexOutputImageParameter::QtWidgetComplexOutputImageParameter(ComplexOutputImageParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_OutputImageParam(param)
{
}

QtWidgetComplexOutputImageParameter::~QtWidgetComplexOutputImageParameter()
{
}

void QtWidgetComplexOutputImageParameter::DoUpdateGUI()
{

}

void QtWidgetComplexOutputImageParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip( m_OutputImageParam->GetDescription() );
  connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
  m_HLayout->addWidget(m_Input);

  // Set the Output PixelType choice Combobox
  m_ComboBox = new QComboBox;
  m_ComboBox->setToolTip("Complex Output Pixel Type");
  m_ComboBox->addItem( "cfloat");
  m_ComboBox->addItem( "cdouble");
  m_ComboBox->setCurrentIndex(m_OutputImageParam->GetComplexPixelType());
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

void QtWidgetComplexOutputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    m_Input->setText(fileDialog.selectedFiles().at(0));
    }
}

void QtWidgetComplexOutputImageParameter::SetFileName(const QString& value)
{
  // save value
  m_FileName = value.toStdString();

  m_OutputImageParam->SetFileName(m_FileName);

  // notify of value change
  QString key( m_OutputImageParam->GetKey() );
  emit ParameterChanged(key);
}

void QtWidgetComplexOutputImageParameter::SetPixelType(int  pixelType)
{
  m_OutputImageParam->SetComplexPixelType(static_cast< ComplexImagePixelType >(pixelType));
  m_ComplexPixelType = pixelType;
}

}
}
