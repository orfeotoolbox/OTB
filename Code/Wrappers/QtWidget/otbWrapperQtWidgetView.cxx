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
  m_Application = app;
  m_MainWindow = new QWidget();
  m_ProgressWindow = new QWidget(); 
  //m_Application->RegisterListener( this );

}

QtWidgetView::~QtWidgetView()
{

}

void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout(m_MainWindow);//this);

  mainLayout->addWidget(CreateHeader());
  mainLayout->addWidget(CreateInputWidgets());
  mainLayout->addWidget(CreateFooter());

  QGroupBox *mainGroup = new QGroupBox(m_MainWindow);//this);
  mainGroup->setLayout(mainLayout);

  // Put the main group inside a scroll area
  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setWidget(mainGroup);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QVBoxLayout *scrollLayout = new QVBoxLayout(m_MainWindow);//this);
  scrollLayout->addWidget(scrollArea);

  // Make the scroll layout the main layout
  m_MainWindow->setLayout(scrollLayout);
  m_MainWindow->setWindowIcon(QIcon( ":/otb_small.png" ));
  m_MainWindow->setWindowTitle(QString(m_Model->GetApplication()->GetName()).append(" - version ").append(OTB_VERSION_STRING));

  // Tweak the window size so that it looks ok (TODO : find the way to make it perfectly symmetric)
  m_MainWindow->resize(mainGroup->sizeHint() + scrollArea->verticalScrollBar()->size());
  //m_ProgressWindow->show();
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
  //connect( m_ExecButton, SIGNAL(clicked()), m_Model, SLOT(ExecuteAndWriteOutput() ) );
  connect( m_ExecButton, SIGNAL(clicked()), this, SLOT(ExecuteAndWriteOutputSlot() ) );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect( m_QuitButton, SIGNAL(clicked()), this, SLOT(m_MainWindow->close()) );

  // Put the buttons on the right
  //footerLayout->addWidget(m_ProgressLabel);
  footerLayout->addStretch();
  footerLayout->addWidget(m_ExecButton);
  footerLayout->addWidget(m_QuitButton);
  footerGroup->setLayout(footerLayout);

  return footerGroup;
}

void QtWidgetView::ExecuteAndWriteOutputSlot()
{
  m_Model->ExecuteAndWriteOutput();

  //QWidget * progWin = new QWidget();
  //QTextEdit * progWin = new QTextEdit();
  //progWin->setWindowTitle( "Progress reporting..." );
  //progWin->resize(700, 700);
  m_ProgressWindow->setWindowTitle( "Progress reporting..." );
  //m_ProgressWindow->resize(700, 700);

  QVBoxLayout *layout = new QVBoxLayout;  
  
  std::vector< QProgressBar * > barListIntern, barListWriter;
  std::vector< QLabel * > labelListIntern, labelListWriter;
  if( m_Application->GetInternalProcessList().size() != m_Application->GetInternalProcessListName().size())
    {
      itkGenericExceptionMacro ("Internal process list and list name size mismatch...");
    }
  
  for(unsigned int ii=0; ii<m_Application->GetInternalProcessList().size(); ii++)
    {
      QLabel *label = new QLabel(QString(m_Application->GetInternalProcessListName()[ii].c_str()));
      QProgressBar * bar = new QProgressBar();
      layout->addWidget(label);
      layout->addWidget(bar);
      barListIntern.push_back(bar);
      labelListIntern.push_back(label);
      std::cout<<"********************************* progress added "<<std::endl;
    }
  
  unsigned int nbOutput = 0;
  std::vector<std::string> paramList = m_Application->GetParametersKeys(true);
  for (std::vector<std::string>::const_iterator it = paramList.begin();
       it != paramList.end();
       ++it)
    {
      if ( m_Application->GetParameterType(*it) == ParameterType_OutputImage)
        {
          itk::OStringStream oss;
          oss << "Writer "<<nbOutput;
          QLabel *label = new QLabel(QString(oss.str().c_str()));
          QProgressBar * bar = new QProgressBar();
          layout->addWidget(label);
          layout->addWidget(bar);
          barListWriter.push_back(bar);
          labelListWriter.push_back(label);
          nbOutput++;
          std::cout<<"********************************* wrietr added "<<std::endl;
        }
    }
  
  //progWin->setLayout(layout);
  //progWin->update();
  //progWin->show();
  m_ProgressWindow->setLayout(layout);
  m_ProgressWindow->update();
 std::cout<<"********************************* ExecuteAndWriteOutput window will be displayed *********************************"<<std::endl;
  m_ProgressWindow->show();
  QCoreApplication::processEvents();
  std::cout<<"********************************* ExecuteAndWriteOutput window displayed *********************************"<<std::endl;
  double curWriterProgress = 0;
  unsigned int curWriter = 0;
  unsigned int countt = 0;
  while( m_Application->GetExecuteAndWriteOutputDone() == false )
    {
      //progWin->setText( QString(countt) );
      //progWin->repaint();
      
      itk::OStringStream oss;
      oss.str("");
      
      // Internal DoExecute process watcher
      std::vector<double> progCount = m_Application->GetDoExecuteProgress();
      for(unsigned int i=0; i<progCount.size(); i++)
        {
          //oss<< "doexe "<<i<<": "<<m_Application->GetDoExecuteProgress()[i] <<std::flush;
          barListIntern[i]->setValue( static_cast<int>(progCount[i]*100 ));
          //progWin->update();
          m_ProgressWindow->update();
 QCoreApplication::processEvents();
        }
      
      // Writer watcher
      if(  nbOutput > 0)
        {
          double curProg = m_Application->GetExecuteProgress();
          
          if( curProg > -1 )
            {
              if( curWriterProgress > curProg )
                {
                  curWriter++;
                }
            
              std::cout<<curProg<<"."<<std::flush;
              barListWriter[curWriter]->setValue( static_cast<int>(curProg*100) );
              curWriterProgress = curProg;
              //progWin->update();
              m_ProgressWindow->update();
 QCoreApplication::processEvents();
            }
        }
      
      sleep(1);
      
    }
  //progWin->close();
  m_ProgressWindow->close();
}


}
}
