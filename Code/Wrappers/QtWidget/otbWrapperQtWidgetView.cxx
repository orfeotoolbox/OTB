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

namespace otb
{
namespace Wrapper
{

QtWidgetView::QtWidgetView(Application* app)
{

  m_Model = new QtWidgetModel(app);

}

QtWidgetView::~QtWidgetView()
{

}

void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  mainLayout->addWidget(CreateHeader());
  mainLayout->addWidget(CreateInputWidgets());
  mainLayout->addWidget(CreateFooter());

  QGroupBox *mainGroup = new QGroupBox(this);
  mainGroup->setLayout(mainLayout);

  // Put the main group inside a scroll area
  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setWidget(mainGroup);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QVBoxLayout *scrollLayout = new QVBoxLayout(this);
  scrollLayout->addWidget(scrollArea);

  // Make the scroll layout the main layout
  this->setLayout(scrollLayout);
  this->setWindowIcon(QIcon( ":/otb_small.png" ));
  this->setWindowTitle(QString(m_Model->GetApplication()->GetName()).append(" - version ").append(OTB_VERSION_STRING));

  // Tweak the window size so that it looks ok (TODO : find the way to make it perfectly symmetric)
  this->resize(mainGroup->sizeHint() + scrollArea->verticalScrollBar()->size());
}

QWidget* QtWidgetView::CreateHeader()
{
  // an HLayout with the description of the application, and two icons
  QHBoxLayout *headerLayout = new QHBoxLayout;

  QGroupBox *headerGroup = new QGroupBox;
  headerGroup->setStyleSheet("border: 1px solid gray");

  headerGroup->setFixedHeight(50);
  headerGroup->setContentsMargins(0,0,0,0);
  headerLayout->setContentsMargins(5,5,5,5);

  QLabel *iconOTBLabel = new QLabel;
  iconOTBLabel->setStyleSheet("border-style: none");
  //iconOTBLabel->setPixmap(QIcon( ":/otb_big.png" ).pixmap(32, QIcon::Normal, QIcon::On));

  QLabel *descriptionLabel = new QLabel;
  descriptionLabel->setStyleSheet("border-style: none");
  QString descriptionLabelText(m_Model->GetApplication()->GetDescription());
  descriptionLabel->setText(descriptionLabelText);

  QLabel *iconCNESLabel = new QLabel;
  iconCNESLabel->setStyleSheet("border-style: none");
  //iconCNESLabel->setPixmap(QIcon( ":/cnes.png" ).pixmap(32, QIcon::Normal, QIcon::On));

  headerLayout->addWidget(iconOTBLabel);
  headerLayout->addStretch();
  headerLayout->addWidget(descriptionLabel);
  headerLayout->addStretch();
  headerLayout->addWidget(iconCNESLabel);
  headerGroup->setLayout(headerLayout);

  return headerGroup;
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  QtWidgetParameterBase* params = QtWidgetParameterFactory::CreateQtWidget(m_Model->GetApplication()->GetParameterList(), m_Model);
  return params;
}

QWidget* QtWidgetView::CreateFooter()
{
  // an HLayout with two buttons : Execute and Quit
  QGroupBox *footerGroup = new QGroupBox;
  QHBoxLayout *footerLayout = new QHBoxLayout;

  footerGroup->setFixedHeight(40);
  footerGroup->setContentsMargins(0,0,0,0);
  footerLayout->setContentsMargins(5,5,5,5);
/*
  m_ProgressLabel = new otb::ProgressLabel(footerGroup);
  connect(m_ExecutionThread, SIGNAL(started()),     this, SLOT(StartExecute()) );
  connect(m_ExecutionThread, SIGNAL(finished()),    this, SLOT(AbortExecute()) );
  connect(m_ExecutionThread, SIGNAL(terminated()),  this, SLOT(EndExecute()) );
*/
  m_ExecButton = new QPushButton(footerGroup);
  //m_ExecButton->setStyleSheet("border-style: none");
  m_ExecButton->setDefault(true);
  m_ExecButton->setText(QObject::tr("Execute"));
  connect( m_ExecButton, SIGNAL(clicked()), m_Model, SLOT(Execute()) );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect( m_QuitButton, SIGNAL(clicked()), this, SLOT(close()) );

  // Put the buttons on the right
  //footerLayout->addWidget(m_ProgressLabel);
  footerLayout->addStretch();
  footerLayout->addWidget(m_ExecButton);
  footerLayout->addWidget(m_QuitButton);
  footerGroup->setLayout(footerLayout);

  return footerGroup;
}


}
}
