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
#include "otbWrapperQtWidgetComplexInputImageParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetComplexInputImageParameter::QtWidgetComplexInputImageParameter(ComplexInputImageParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_ComplexInputImageParam(param)
{
}

QtWidgetComplexInputImageParameter::~QtWidgetComplexInputImageParameter()
{
}

void QtWidgetComplexInputImageParameter::DoUpdateGUI()
{
  //update lineedit if HasUserValue flag is set(from xml)
  if(m_ComplexInputImageParam->HasUserValue())
    {
    QString text(
      QFile::decodeName(
        m_ComplexInputImageParam->GetFileName().c_str()
      )
    );

    if (text != m_Input->text())
      m_Input->setText(text);
    }
}

void QtWidgetComplexInputImageParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip( m_ComplexInputImageParam->GetDescription() );
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

void QtWidgetComplexInputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  assert( m_Input!=NULL );

  if( !m_Input->text().isEmpty() )
    {
    QFileInfo finfo( m_Input->text() );

    fileDialog.setDirectory(
      finfo.isDir()
      ? finfo.absoluteFilePath()
      : finfo.absoluteDir()
      );
    }

  if (fileDialog.exec())
    {
    if ( this->SetFileName( fileDialog.selectedFiles().at(0) ) == true )
      m_Input->setText(fileDialog.selectedFiles().at(0));
    else
      {
      std::ostringstream oss;
      oss << "The given file "
          << QFile::encodeName(	fileDialog.selectedFiles().at( 0 ) ).constData()
          << " is not valid.";
      this->GetModel()->SendLogWARNING( oss.str() );
      }
    }
}

bool QtWidgetComplexInputImageParameter::SetFileName(const QString& value)
{
  bool res = false;
  // save value
  if( m_ComplexInputImageParam->SetFromFileName(
    QFile::encodeName( value ).constData()) == true)
    {
    res = true;
    // notify of value change
    QString key( m_ComplexInputImageParam->GetKey() );
    emit ParameterChanged(key);
    }

  return res;
}

}
}
