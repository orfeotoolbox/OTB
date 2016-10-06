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
#include "otbWrapperQtWidgetOutputFilenameParameter.h"

#include <otbQtAdapters.h>

namespace otb
{
namespace Wrapper
{

QtWidgetOutputFilenameParameter::QtWidgetOutputFilenameParameter(OutputFilenameParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_FilenameParam(param)
{
}

QtWidgetOutputFilenameParameter::~QtWidgetOutputFilenameParameter()
{
}

void QtWidgetOutputFilenameParameter::DoUpdateGUI()
{
  // Update the lineEdit
  QString text(
    QFile::decodeName(
      m_FilenameParam->GetValue().c_str()
    )
  );

  if (text != m_Input->text())
    m_Input->setText(text);
}

void QtWidgetOutputFilenameParameter::DoCreateWidget()
{
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_Input = new QLineEdit;
  m_Input->setToolTip( m_FilenameParam->GetDescription() );
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

void
QtWidgetOutputFilenameParameter
::SelectFile()
{
  assert( m_Input!=NULL );

  QString filename(
    GetSaveFileName(
      this,
      QString(),
      m_Input->text(),
      tr( "All files (*)" ),
      NULL )
  );

  if( filename.isEmpty() )
    return;

  SetFileName( filename );

  m_Input->setText( filename  );
}


void QtWidgetOutputFilenameParameter::SetFileName(const QString& value)
{
  // save value
  m_FilenameParam->SetValue(
    QFile::encodeName( value ).constData()
  );

  // notify of value change
  QString key( m_FilenameParam->GetKey() );
  emit ParameterChanged(key);
}

}
}
