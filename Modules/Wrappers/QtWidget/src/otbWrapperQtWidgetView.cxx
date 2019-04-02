/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperQtWidgetView.h"

#include <functional>

#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetParameterFactory.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetSimpleProgressReport.h"

namespace otb
{
namespace Wrapper
{
/*
  TRANSLATOR mvd::Wrapper::QtWidgetView

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

char const * const
QtWidgetView
::OBJECT_NAME = "otb::Wrapper::QtWidgetView";

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
QtWidgetView::QtWidgetView( const otb::Wrapper::Application::Pointer & otbApp,
		QWidget* parent,
		Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_Model( NULL ),
  m_LogText( NULL ),
  m_TabWidget( NULL ),
  m_IsClosable( true ),
  m_IsRunning(false)
{
  setObjectName( QtWidgetView::OBJECT_NAME );

  m_Model = new otb::Wrapper::QtWidgetModel( otbApp );

  QObject::connect(
    m_Model, &QtWidgetModel::SetProgressReportBegin,
    this, &QtWidgetView::OnProgressReportBegin
  );

  QObject::connect(
    m_Model, &QtWidgetModel::SetProgressReportDone,
    this, &QtWidgetView::OnProgressReportEnd
  );

  QObject::connect(
    m_Model, &QtWidgetModel::ExceptionRaised,
    this, &QtWidgetView::OnExceptionRaised
  );
}

QtWidgetView::~QtWidgetView()
{
  delete m_Model;
  m_Model = NULL;
}

void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout;
  m_TabWidget = new QTabWidget(this);

  m_TabWidget->addTab(CreateInputWidgets(), tr("Parameters"));
  m_LogText = new QTextEdit(this);
  connect( m_Model->GetLogOutput(), &QtLogOutput::NewContentLog, m_LogText, &QTextEdit::append );
  m_TabWidget->addTab(m_LogText, tr("Logs"));
  mainLayout->addWidget(m_TabWidget);

  // Footer: progress bar
  QHBoxLayout *footLayout = new QHBoxLayout;
  mainLayout->addLayout(footLayout);

  QGroupBox *mainGroup = new QGroupBox(this);
  mainGroup->setLayout(mainLayout);

  QVBoxLayout  *finalLayout = new QVBoxLayout;
  finalLayout->addWidget(mainGroup);

  // Make the final layout to the widget
  this->setLayout(finalLayout);
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  return otb::Wrapper::QtWidgetParameterFactory::CreateQtWidget(
      m_Model->GetApplication()->GetParameterList(),
      m_Model,
      this);
}


void QtWidgetView::closeEvent( QCloseEvent * e )
{
  assert( e!=NULL );

  if( !IsClosable() )
    {
    assert( GetModel()->GetApplication() );

    QMessageBox::warning(
      this,
      tr( "Warning!" ),
      tr( "OTB-Application '%1' cannot be closed while running!")
      .arg( GetModel()->GetApplication()->GetDocName() )
    );

    e->ignore();

    return;
    }

  QWidget::closeEvent( e );

  emit QuitSignal();

  deleteLater();
}

void QtWidgetView::UnhandledException(QString message)
{
  this->OnExceptionRaised(message);
  m_LogText->append(message);
}

void QtWidgetView::OnExceptionRaised( QString /*message*/)
{
  m_TabWidget->setCurrentIndex(1);
}

bool QtWidgetView::IsRunning() const
{
  return m_IsRunning;
}

QtWidgetModel* QtWidgetView::GetModel() const
{
return m_Model;
}

bool QtWidgetView::IsClosable() const
{
  return m_IsClosable;
}

void QtWidgetView::SetClosable( bool enabled )
{
  m_IsClosable = enabled;

  setEnabled( true );
}

void QtWidgetView::OnProgressReportBegin()
{
  SetClosable( false );
}

void QtWidgetView::OnProgressReportEnd( int )
{
  SetClosable( true );
}

} // end of namespace Wrapper
} // end of namespace otb

