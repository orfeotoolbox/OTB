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

#include "otbWrapperOutputImageParameter.h"

#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

QtWidgetView::QtWidgetView(Application* app)
{

  m_Model = new QtWidgetModel(app);
  m_Application = app;
  //m_MainWindow = new QWidget();
  //m_Application->RegisterListener( this );

}

QtWidgetView::~QtWidgetView()
{

}

void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout();

  mainLayout->addWidget(CreateHeader());
  mainLayout->addWidget(CreateInputWidgets());
  mainLayout->addWidget(CreateFooter());


  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(mainLayout);

  // Put the main group inside a scroll area
  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setWidget(mainGroup);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QVBoxLayout *scrollLayout = new QVBoxLayout();
  scrollLayout->addWidget(scrollArea);

  // Make the scroll layout the main layout
  this->setLayout(scrollLayout);
  this->setWindowIcon(QIcon( ":/otb_small.png" ));
  this->setWindowTitle(QString(m_Model->GetApplication()->GetName()).append(" - version ").append(OTB_VERSION_STRING));

  this->show();
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

  m_ExecButton = new QPushButton(footerGroup);
  m_ExecButton->setDefault(true);
  m_ExecButton->setText(QObject::tr("Execute"));
  connect( m_ExecButton, SIGNAL(clicked()), this, SLOT(ExecuteAndWriteOutputSlot() ) );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect( m_QuitButton, SIGNAL(clicked()), this, SLOT(CloseSlot()) );

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

  QWidget * progWin = new QWidget();
  progWin->setWindowTitle( "Progress reporting..." );

  QVBoxLayout *layout = new QVBoxLayout;  
  
  std::vector< QProgressBar * > barListIntern, barListWriter;
  std::vector< QLabel * > labelListIntern, labelListWriter;
  if( m_Application->GetInternalProcessList().size() != m_Application->GetInternalProcessListName().size())
    {
      itkGenericExceptionMacro ("Internal process list and list name size mismatch...");
    }
  

  // Build the window : First internal process
  for(unsigned int ii=0; ii<m_Application->GetInternalProcessList().size(); ii++)
    {
      QLabel *label = new QLabel(QString(m_Application->GetInternalProcessListName()[ii].c_str()));
      QProgressBar * bar = new QProgressBar();
      layout->addWidget(label);
      layout->addWidget(bar);
      barListIntern.push_back(bar);
      labelListIntern.push_back(label);
    }

  
  // Build the window : then writers
  unsigned int nbOutput = 0;
  std::vector<std::string> paramList = m_Application->GetParametersKeys(true);
  for (std::vector<std::string>::const_iterator it = paramList.begin();
       it != paramList.end();
       ++it)
    {
      if ( m_Application->GetParameterType(*it) == ParameterType_OutputImage)
        {
          itk::OStringStream oss;
          // create the label including the output description
          Parameter* param =  m_Application->GetParameterByKey(*it);
          OutputImageParameter* outputParam = dynamic_cast<OutputImageParameter*>(param);
          oss << "Writer "<< nbOutput << ": ";
          oss << outputParam->GetName() <<".";
          QLabel *label = new QLabel(QString(oss.str().c_str()));
          QProgressBar * bar = new QProgressBar();
          bar->setToolTip( QString( outputParam->GetDescription()) );
          layout->addWidget(label);
          layout->addWidget(bar);
          barListWriter.push_back(bar);
          labelListWriter.push_back(label);
          nbOutput++;
        }
    }
  
  // Display the window
  progWin->setLayout(layout);
  progWin->update();
  progWin->show();
  // PAY ATTENTION : launching a GUI modification in a slot is simple : you have to call the following method 
  // to update the general GUI
  QCoreApplication::processEvents();

  // Watch process
  double curWriterProgress = 0;
  unsigned int curWriter = 0;
  unsigned int countt = 0;
  while( m_Application->GetExecuteAndWriteOutputDone() == false )
    {
      itk::OStringStream oss;
      oss.str("");
      
      // Internal DoExecute process watcher
      std::vector<double> progCount = m_Application->GetDoExecuteProgress();
      for(unsigned int i=0; i<progCount.size(); i++)
        {
          barListIntern[i]->setValue( static_cast<int>(progCount[i]*100 ));
          progWin->update();
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

              barListWriter[curWriter]->setValue( static_cast<int>(curProg*100) );
              curWriterProgress = curProg;
              progWin->update();
              QCoreApplication::processEvents();
            }
        }
      
      itksys::SystemTools::Delay(1000);
      
    }
  progWin->close();
}

void QtWidgetView::CloseSlot()
{
  this->close();
}

}
}
