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

  QtWidgetSimpleProgressReport * progressReport =  new QtWidgetSimpleProgressReport(m_Model);
  progressReport->SetApplication(m_Application);
  QHBoxLayout *footLayout = new QHBoxLayout;
  footLayout->addWidget(progressReport);
  footLayout->addWidget(CreateFooter());
  mainLayout->addLayout(footLayout);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(mainLayout);

  // Put the main group inside a scroll area
  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setWidget(mainGroup);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidgetResizable(true);

  QVBoxLayout  *scrollLayout = new QVBoxLayout();
  scrollLayout->addWidget(scrollArea);

  // Make the scroll layout the main layout
  this->setLayout(scrollLayout);
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
  footerGroup->setContentsMargins(0, 0, 0, 0);
  footerLayout->setContentsMargins(5, 5, 5, 5);

  m_ExecButton = new QPushButton(footerGroup);
  m_ExecButton->setDefault(true);
  m_ExecButton->setEnabled(false);
  m_ExecButton->setText(QObject::tr("Execute"));
  connect( m_ExecButton, SIGNAL(clicked()), m_Model, SLOT(ExecuteAndWriteOutputSlot() ) );
  connect( m_Model, SIGNAL(SetApplicationReady(bool)), m_ExecButton, SLOT(setEnabled(bool)) );

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


  //std::cout<<text->toHtml().toStdString()<<std::endl;

  return text;
}

void QtWidgetView::SetDocParameters( std::string & val )
{
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys( false ); //true );
  const unsigned int nbOfParam = appKeyList.size();
    
  itk::OStringStream oss;
  oss << "<h3>Parameters</h3>";
  
  // Mandatory parameters
  oss << "<h4>Mandatory parameters</h4>";
  
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    Parameter::Pointer param =  m_Application->GetParameterByKey( appKeyList[i] );
    // Check if mandatory parameter are present and have value
    if( param->GetMandatory() == true )
      {
      if( m_Application->GetParameterType(appKeyList[i]) !=  ParameterType_Group && param->IsRoot() )
        {
        oss << "<i>" << param->GetName() << ":</i><br />";
        oss << param->GetDescription()<< "<br />";
        oss << "<br />";
        }
      else if( m_Application->GetParameterType(appKeyList[i]) ==  ParameterType_Group )
        {
        oss << param->GetName()<<"<br />";
        oss << param->GetDescription()<<"<br />";
        oss << "Parameters:<<br />";
        std::string grDoc;
        GetDocParameterGroup( grDoc, appKeyList[i]);
        oss<<grDoc;
        }
      }
    }
  
  // Optional parameters
  oss << "<h4>Optional parameters</h4>";
  oss << "<body><li>";
  bool found = false;
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    Parameter::Pointer param =  m_Application->GetParameterByKey( appKeyList[i] );
    // Check if mandatory parameter are present and have value
    if( param->GetMandatory() == false )
      {
      if( m_Application->GetParameterType(appKeyList[i]) !=  ParameterType_Group && param->IsRoot() )
        {
        oss << "<i>" << param->GetName() << ":</i><br />";
        oss << param->GetDescription()<< "<br />";
        oss << "<br />";
        
        found = true;
        }
      else if( m_Application->GetParameterType(appKeyList[i]) ==  ParameterType_Group )
        {
        oss << "<b><i>"<<param->GetName()<<"</b></i> ("<<param->GetDescription()<<"):<br />";
        std::string grDoc;
        GetDocParameterGroup( grDoc, appKeyList[i]);
        oss<<grDoc;
        
        found = true;
        }
      }
    }
  if( !found )
    oss << "None";
  
  
  val.append(oss.str());
}

void QtWidgetView::GetDocParameterGroup( std::string & val, const std::string & key )
{
  Parameter * paramGr  = m_Application->GetParameterByKey( key );
  if( !dynamic_cast<ParameterGroup *>(paramGr) )
    {
    itkGenericExceptionMacro("Invlaid parameter type for key "<<key<<", wait for ParameterGroup...");
    }

  ParameterGroup * group = dynamic_cast<ParameterGroup *>(paramGr);
  const std::vector<std::string> appKeyList = group->GetParametersKeys( false );
  unsigned int nbOfParam = appKeyList.size();
  itk::OStringStream oss;
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    const std::string fullKey(std::string(key).append(".").append(appKeyList[i]));
    Parameter::Pointer param =  m_Application->GetParameterByKey( fullKey );
    if( m_Application->GetParameterType(fullKey) !=  ParameterType_Group )
      {
      oss << "<i>" << param->GetName()<< ":</i><br />";
      oss << param->GetDescription()<<"<br>";
      //oss << "<br />";
      }
    else
      {
      oss << "<b><i>"<<param->GetName()<<"</b></i> ("<<param->GetDescription()<<"):<br />";
      std::string grDoc;
      GetDocParameterGroup( grDoc, fullKey);
      oss<<grDoc;
      }
    }
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
