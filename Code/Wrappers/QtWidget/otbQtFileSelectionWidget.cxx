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
  : QWidget(), m_Index(0), m_AsValue(false)
{
  m_InputList = InputImageListParameter::New();
  this->DoCreateWidget();
}

QtFileSelectionWidget::QtFileSelectionWidget( InputImageListParameter * il )
  : QWidget(), m_Index(0), m_AsValue(false)
{
  m_InputList = il;
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

  //connect( m_Input, SIGNAL(textChanged(const QString&)), this, SLOT(SetFileName(const QString&)) );
  //connect( m_Input, SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );

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

void QtFileSelectionWidget::SelectFile()
{
  std::cout<<this<<std::endl;
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    QString filemane(fileDialog.selectedFiles().at(0));
    m_Input->setText(filemane);

    if( m_AsValue == false )
      {
      m_InputList->AddFromFileName(filemane.toStdString());
      m_Index = m_InputList->GetImageList()->Size()-1;
      m_AsValue = true;
      }
    else
      {
      m_InputList->SetNthFileName( m_Index, filemane.toStdString());
      }

    }
}

/*
void QtFileSelectionWidget::SetFileName(const QString& value)
{
  // save value
  m_InputImageParam->SetFromFileName(value.toStdString());

  // notify of value change
  QString key( QString::fromStdString(m_InputImageParam->GetKey()) );
  emit ParameterChanged(key);
}
*/

}
}
