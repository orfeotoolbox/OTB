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
#include "otbWrapperApplicationHtmlDocGenerator.h"
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
  m_IconPathDone(""),
  m_IconPathFailed(""),
  m_Model( NULL ),
  m_ExecButton( NULL ),
  m_QuitButton( NULL ),
  m_Message( NULL ),
  m_LogText( NULL ),
  m_TabWidget( NULL ),
  m_IsClosable( true ),
  m_IsRunning(false)
{
  setObjectName( QtWidgetView::OBJECT_NAME );

  m_Model = new otb::Wrapper::QtWidgetModel( otbApp );
  m_QuitShortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);

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
  m_TabWidget->addTab(CreateDoc(), tr("Documentation"));
  mainLayout->addWidget(m_TabWidget);

  m_Message = new QLabel("<center><font color=\"#FF0000\">"+tr("Select parameters")+"</font></center>", this);
  connect( m_Model, &QtWidgetModel::SetApplicationReady, this, &QtWidgetView::UpdateMessageAfterApplicationReady );
  connect( m_Model, &QtWidgetModel::SetProgressReportDone, this, &QtWidgetView::UpdateMessageAfterExecution );
  mainLayout->addWidget(m_Message);

  otb::Wrapper::QtWidgetSimpleProgressReport * progressReport = new otb::Wrapper::QtWidgetSimpleProgressReport(m_Model, this);
  progressReport->SetApplication(m_Model->GetApplication());

  QWidget* footer = CreateFooter();
   
  QHBoxLayout *footLayout = new QHBoxLayout;
  footLayout->addWidget(progressReport);
  footLayout->addWidget(footer);
  mainLayout->addLayout(footLayout);

  footLayout->setAlignment(footer, Qt::AlignBottom);

  QGroupBox *mainGroup = new QGroupBox(this);
  mainGroup->setLayout(mainLayout);

  QVBoxLayout  *finalLayout = new QVBoxLayout;
  finalLayout->addWidget(mainGroup);

  // Make the final layout to the widget
  this->setLayout(finalLayout);
}

void QtWidgetView::UpdateMessageAfterExecution(int status)
{
  if (status >= 0)
    {
    m_Message->setText("<center>"+QString(m_IconPathDone.c_str())+
      "<font color=\"#00A000\">"+tr("Done")+"</font></center>");
    }
  else
    {
    m_Message->setText("<center>"+QString(m_IconPathFailed.c_str())+
      "<font color=\"#FF0000\">"+tr("Failed")+"</font></center>");
    }
  m_ExecButton->setText(QObject::tr("Execute"));
  m_IsRunning = false;
}

void QtWidgetView::UpdateMessageAfterApplicationReady( bool val )
{
  if(!m_IsRunning)
    {
    if(val == true)
      m_Message->setText("<center><font color=\"#00A000\">"+tr("Ready to run")+"</font></center>");
    else
      m_Message->setText("<center><font color=\"#FF0000\">"+tr("Select parameters")+"</font></center>");
    }
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  QScrollArea *scrollArea = new QScrollArea(this);

  scrollArea->setWidget( otb::Wrapper::QtWidgetParameterFactory::CreateQtWidget(
      m_Model->GetApplication()->GetParameterList(),
      m_Model,
      this));
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidgetResizable(true);

  return scrollArea;
}


QWidget* QtWidgetView::CreateFooter()
{
  // an HLayout with two buttons : Execute and Quit
  QGroupBox *footerGroup = new QGroupBox(this);
  QHBoxLayout *footerLayout = new QHBoxLayout;
 
  footerGroup->setFixedHeight(40);
  footerGroup->setContentsMargins(0, 0, 0, 0);
  footerLayout->setContentsMargins(5, 5, 5, 5);

  m_ExecButton = new QPushButton(footerGroup);
  m_ExecButton->setDefault(true);
  m_ExecButton->setEnabled(false);
  m_ExecButton->setText(QObject::tr("Execute"));
  connect( m_Model, &QtWidgetModel::SetApplicationReady, m_ExecButton, &QPushButton::setEnabled );
  connect( m_ExecButton, &QPushButton::clicked, this, &QtWidgetView::OnExecButtonClicked );
  connect( this, &QtWidgetView::ExecuteAndWriteOutput, m_Model, &QtWidgetModel::ExecuteAndWriteOutputSlot );
  connect( this, &QtWidgetView::Stop, m_Model, &QtWidgetModel::Stop );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect(m_QuitButton, &QPushButton::clicked, this, &QtWidgetView::close );

  // Add Ctrl-Q shortcut to quit
  connect( m_QuitShortcut, &QShortcut::activated, this, &QtWidgetView::close );

  // Put the buttons on the right
  footerLayout->addStretch();
  footerLayout->addWidget(m_ExecButton);
  footerLayout->addWidget(m_QuitButton);

  footerGroup->setLayout(footerLayout);

  return footerGroup;
}

QWidget* QtWidgetView::CreateDoc()
{
  QTextEdit *text = new QTextEdit(this);
  text->setReadOnly(true);

  QTextDocument * doc = new QTextDocument(this);

  std::string docContain;
  otb::Wrapper::ApplicationHtmlDocGenerator::GenerateDoc( GetModel()->GetApplication(), docContain);

  doc->setHtml(docContain.c_str());

  text->setDocument(doc);

  return text;
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

void
QtWidgetView
::OnExecButtonClicked()
{
  if (m_IsRunning)
    {
    m_Message->setText("<center><font color=\"#FF0000\">"+tr("Cancelling")+"...</font></center>");
    emit Stop();
    }
  else
    {
    m_IsRunning =  true;
    m_Message->setText("<center><font color=\"#FF0000\">"+tr("Running")+"</font></center>");
    m_ExecButton->setText(QObject::tr("Cancel"));
    emit ExecuteAndWriteOutput();
    }
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

  if( m_QuitButton!=NULL )
    m_QuitButton->setEnabled( m_IsClosable );
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

