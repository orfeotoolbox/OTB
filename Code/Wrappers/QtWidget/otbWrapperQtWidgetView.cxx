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
#include "otbWrapperQtWidgetView.h"

#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetParameterFactory.h"
#include "otbWrapperQtWidgetProgressReport.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperQtWidgetSimpleProgressReport.h"
#include "otbWrapperApplicationHtmlDocGenerator.h"

#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

QtWidgetView::QtWidgetView(Application* app)
{
  m_Model = new QtWidgetModel(app);
  m_Application = app;
}

QtWidgetView::~QtWidgetView()
{

}

void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout();
  m_TabWidget = new QTabWidget();

  m_TabWidget->addTab(CreateInputWidgets(), "Parameters");
  m_LogText = new QTextEdit();
  connect( m_Model->GetLogOutput(), SIGNAL(NewContentLog(QString)), m_LogText, SLOT(append(QString) ) );
  m_TabWidget->addTab(m_LogText, "Logs");
  QtWidgetProgressReport* prog =  new QtWidgetProgressReport(m_Model);
  prog->SetApplication(m_Application);
  m_TabWidget->addTab(prog, "Progress");
  m_TabWidget->addTab(CreateDoc(), "Documentation");
  mainLayout->addWidget(m_TabWidget);

  m_Message = new QLabel("<center><font color=\"#FF0000\">Select parameters</font></center>");
  connect( m_Model, SIGNAL(SetApplicationReady(bool)), this, SLOT(UpdateMessageAfterApplicationReady(bool)) );
  mainLayout->addWidget(m_Message);

  QtWidgetSimpleProgressReport * progressReport =  new QtWidgetSimpleProgressReport(m_Model);
  progressReport->SetApplication(m_Application);

  QHBoxLayout *footLayout = new QHBoxLayout;
  footLayout->addWidget(progressReport);
  footLayout->addWidget(CreateFooter());
  mainLayout->addLayout(footLayout);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(mainLayout);

  QVBoxLayout  *finalLayout = new QVBoxLayout();
  finalLayout->addWidget(mainGroup);

  // Make the final layout to the widget
  this->setLayout(finalLayout);
}

void QtWidgetView::UpdateMessageAfterExcuteClicked()
{
  m_Message->setText("<center><font color=\"#FF0000\">Running</font></center>");
}

void QtWidgetView::UpdateMessageAfterApplicationReady( bool val )
{
  if(val == true)
    m_Message->setText("<center><font color=\"#00FF00\">Ready to run</font></center>");
  else
    m_Message->setText("<center><font color=\"#FF0000\">Select parameters</font></center>");
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  QScrollArea *scrollArea = new QScrollArea;
  // Put the main group inside a scroll area
  scrollArea->setWidget(QtWidgetParameterFactory::CreateQtWidget(m_Model->GetApplication()->GetParameterList(), m_Model));
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidgetResizable(true);

  return scrollArea;
}


QWidget* QtWidgetView::CreateFooter()
{
  // an HLayout with two buttons : Execute and Quit
  QGroupBox *footerGroup = new QGroupBox;
  QHBoxLayout *footerLayout = new QHBoxLayout;

  footerGroup->setFixedHeight(40);
  footerGroup->setContentsMargins(0, 0, 0, 0);
  footerLayout->setContentsMargins(5, 5, 5, 5);

  m_ExecButton = new QPushButton(footerGroup);
  m_ExecButton->setDefault(true);
  m_ExecButton->setEnabled(false);
  m_ExecButton->setText(QObject::tr("Execute"));
  connect( m_ExecButton, SIGNAL(clicked()), m_Model, SLOT(ExecuteAndWriteOutputSlot() ) );
  connect( m_Model, SIGNAL(SetApplicationReady(bool)), m_ExecButton, SLOT(setEnabled(bool)) );
  connect( m_ExecButton, SIGNAL(clicked()), this, SLOT(UpdateMessageAfterExcuteClicked() ) );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect( m_QuitButton, SIGNAL(clicked()), this, SLOT(CloseSlot()) );

  // Put the buttons on the right
  footerLayout->addStretch();
  footerLayout->addWidget(m_ExecButton);
  footerLayout->addWidget(m_QuitButton);

  footerGroup->setLayout(footerLayout);

  return footerGroup;
}

QWidget* QtWidgetView::CreateDoc()
{
  QTextEdit *text = new QTextEdit;
  text->setReadOnly(true);

  QTextDocument * doc = new QTextDocument();

  std::string docContain;
  ApplicationHtmlDocGenerator::GenerateDoc( m_Application, docContain);

  doc->setHtml(docContain.c_str());

  text->setDocument(doc);

  return text;
}

void QtWidgetView::CloseSlot()
{
  // Close the widget
  this->close();

  // Emit a signal to close any widget that this gui belonging to
  emit QuitSignal();
}

void QtWidgetView::UnhandledException(QString message)
{
  m_TabWidget->setCurrentIndex(1);
  m_LogText->append(message);
}

}
}
