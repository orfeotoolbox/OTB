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
  QTabWidget *tab = new QTabWidget();
 
  tab->addTab(CreateInputWidgets(), "Parameters");
  QTextEdit *log = new QTextEdit();
  connect( m_Model->GetLogOutput(), SIGNAL(NewContentLog(QString)), log, SLOT(append(QString) ) );
  tab->addTab(log, "Logs");
  QtWidgetProgressReport* prog =  new QtWidgetProgressReport(m_Model);
  prog->SetApplication(m_Application);
  tab->addTab(prog, "Progress");
  tab->addTab(CreateDoc(), "Documentation");
  mainLayout->addWidget(tab);

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
  itk::OStringStream oss;
  oss << "<center><h2>" << m_Application->GetDocName() << "</center></h2>";
  oss << "<h3>Brief Description</h3>";
  oss << "<body>" << m_Application->GetDescription() << "</body>";
  oss << "<h3>Tags</h3>";
  oss << "<body>";
  if (m_Application->GetDocTags().size() > 0)
    {
    for (unsigned int i = 0; i < m_Application->GetDocTags().size() - 1; i++)
      {
      oss << m_Application->GetDocTags()[i] << ", ";
;
      }
    oss << m_Application->GetDocTags()[m_Application->GetDocTags().size() - 1];
    }
  oss << "</body>";

  oss << "<h3>Long Description</h3>";
  oss << "<body>" << m_Application->GetDocLongDescription() << "</body>";

  std::string val;
  this->SetDocParameters(val);
  oss << val;

  oss << "<h3>Limitations</h3>";
  oss << "<body>" << m_Application->GetDocLimitations() << "</body>";
  oss << "<h3>Authors</h3>";
  oss << "<body>" << m_Application->GetDocAuthors() << "</body>";
  oss << "<h3>See also</h3>";
  oss << "<body>" << m_Application->GetDocSeeAlso() << "</body>";
  oss << "<h3>Command line example</h3>";
  oss << "<code>" << m_Application->GetDocCLExample() << "</code>";

  doc->setHtml(oss.str().c_str());

  text->setDocument(doc);


  std::cout<<text->toHtml().toStdString()<<std::endl;

  return text;
}

void QtWidgetView::SetDocParameters( std::string & val )
{
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys( false );
    
  itk::OStringStream oss;
  
  oss << "<h3>Parameters</h3>";
  oss<<"<ul>";
  std::string paramDocs("");

  this->GetDocParameters( paramDocs );
  oss<<paramDocs;
  oss<<"</ul>";

  val.append(oss.str());
}

void QtWidgetView::GetDocParameters( std::string & val )
{
  itk::OStringStream oss;
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys( false );
  const unsigned int nbOfParam = appKeyList.size();
  
  std::string paramDocs("");
   for( unsigned int i=0; i<nbOfParam; i++ )
     {
     const std::string key(appKeyList[i]);
     Parameter::Pointer param =  m_Application->GetParameterByKey( key );
     if( m_Application->GetParameterType(key) ==  ParameterType_Group)
       {
       oss << "<li><b>[group] "<<param->GetName()<<": </b>";
       if(std::string(param->GetDescription()).size()!=0)
         {
         oss<<param->GetDescription();
         }
       std::string grDoc;
       GetDocParameterGroup( grDoc, key );
       oss<<grDoc;
       oss<<"</li><br />";
       }
     else if( m_Application->GetParameterType(key) ==  ParameterType_Choice )
       {
       oss << "<li><b> [choice] "<<param->GetName()<<": </b>";
       if(std::string(param->GetDescription()).size()!=0)
         {
         oss<<param->GetDescription();
         }
       std::string grDoc;
       GetDocParameterChoice(grDoc, key);
       oss<<grDoc;
       oss<<"</li><br />";
       }
     else
       {
       oss << "<li><b><code>[param] " << param->GetName() << ": </code></b>";
       oss << param->GetDescription()<< "</li>";
       }
     }
   
   if( oss.str() == "" )
     oss << "None";

   val = oss.str();
}


void QtWidgetView::GetDocParameterGroup( std::string & val, const std::string & key )
{
  Parameter * paramGr  = m_Application->GetParameterByKey( key );
  if( !dynamic_cast<ParameterGroup *>(paramGr))
    {
    itkGenericExceptionMacro("Invalid parameter type for key "<<key<<", wait for ParameterGroup...");
    }
    ParameterGroup * group = dynamic_cast<ParameterGroup *>(paramGr);
    const std::vector<std::string> appKeyList = group->GetParametersKeys( false );
    unsigned int nbOfParam = appKeyList.size();
  itk::OStringStream oss;
  oss<<"<ul>";
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    const std::string fullKey(std::string(key).append(".").append(appKeyList[i]));
    Parameter::Pointer param =  m_Application->GetParameterByKey( fullKey );
    if( m_Application->GetParameterType(fullKey) ==  ParameterType_Group)
      {
      oss << "<li><b><code>[group] "<< param->GetName()<<": </code></b>";
      if(std::string(param->GetDescription()).size()!=0)
        {
        oss<<param->GetDescription();
        }
      std::string grDoc;
      GetDocParameterGroup( grDoc, fullKey );
      oss<<grDoc;
      oss<<"</li>";
      }
    else if( m_Application->GetParameterType(fullKey) ==  ParameterType_Choice )
      {
      oss << "<li><b><code>[choice] "<<param->GetName()<<": </code></b>";
      if(std::string(param->GetDescription()).size()!=0)
        {
        oss<<param->GetDescription();
        }
      std::string grDoc;
      GetDocParameterChoice(grDoc, fullKey );
      oss<<grDoc;
      oss<<"</li>";
      }
    else
      {
      oss << "<li><b><code>[param] "<< param->GetName()<< ": </code></b>";
      oss  << param->GetDescription()<<"</li>";
      }
    }
  oss<<"</ul>";
  val.append(oss.str());
}


void QtWidgetView::GetDocParameterChoice( std::string & val, const std::string & key )
{
  Parameter * paramCh  = m_Application->GetParameterByKey( key );
  if( !dynamic_cast<ChoiceParameter *>(paramCh))
    {
    itkGenericExceptionMacro("Invalid parameter type for key "<<key<<", wait for ChoiceParameter...");
    }
    ChoiceParameter * choice = dynamic_cast<ChoiceParameter *>(paramCh);
    const std::vector<std::string> appKeyList = choice->GetChoiceKeys();
    unsigned int nbOfParam = choice->GetNbChoices();
    itk::OStringStream oss;
    oss<<"<ul>";
  
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    const std::string fullKey(std::string(key).append(".").append(appKeyList[i]));
    ParameterGroup * group = choice->GetChoiceParameterGroupByIndex(i);
    std::string grDoc;
    oss << "<li><b><code>[group] "<< group->GetName()<<": </code></b>";
      if(std::string(group->GetDescription()).size()!=0)
        {
        oss<<group->GetDescription();
        }

    GetDocParameterGroup( grDoc, fullKey );
    oss<<grDoc;
    oss<<"</li>";
    }
  oss<<"</ul>";
  val.append(oss.str());
}


void QtWidgetView::CloseSlot()
{
  // Close the widget
  this->close();

  // Emit a signal to close any widget that this gui belonging to
  emit QuitSignal();
}

}
}
