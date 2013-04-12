/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdWrapperQtWidgetView.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperQtWidgetProgressReport.h"
#include "otbWrapperQtWidgetSimpleProgressReport.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperApplicationHtmlDocGenerator.h"

#include "otbWrapperTypes.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h" // TODO : handle
                                                   // this param to
                                                   // get the outfname

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdWrapperQtWidgetParameterFactory.h"
#include "Core/mvdI18nApplication.h"


namespace mvd
{
namespace Wrapper
{
/*
  TRANSLATOR mvd::ApplicationLauncher

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
QtWidgetView::QtWidgetView(otb::Wrapper::Application* app)
{
  m_Model = new otb::Wrapper::QtWidgetModel(app);
  m_Application = app;
}

/*******************************************************************************/
QtWidgetView::~QtWidgetView()
{

}

/*******************************************************************************/
void QtWidgetView::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QTabWidget *tab = new QTabWidget();
  tab->addTab(CreateInputWidgets(), "Parameters");

  //QTextEdit *log = new QTextEdit();
  //connect( m_Model->GetLogOutput(), SIGNAL(NewContentLog(QString)), log, SLOT(append(QString) ) );
  //tab->addTab(log, "Logs");
  //otb::Wrapper::QtWidgetProgressReport* prog =  new otb::Wrapper::QtWidgetProgressReport(m_Model);
  //prog->SetApplication(m_Application);
  //tab->addTab(prog, "Progress");
  tab->addTab(CreateDoc(), "Documentation");
  mainLayout->addWidget(tab);

  m_Message = new QLabel("<center><font color=\"#FF0000\">Select parameters</font></center>");
  connect( m_Model, SIGNAL(SetApplicationReady(bool)), this, SLOT(UpdateMessageAfterApplicationReady(bool)) );
  mainLayout->addWidget(m_Message);

  otb::Wrapper::QtWidgetSimpleProgressReport * progressReport =  new otb::Wrapper::QtWidgetSimpleProgressReport(m_Model);
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


/*******************************************************************************/
QWidget* QtWidgetView::CreateInputWidgets()
{
  QScrollArea *scrollArea = new QScrollArea;
  // Put the main group inside a scroll area
  QWidget * widgets = 
    mvd::Wrapper::QtWidgetParameterFactory::CreateQtWidget(m_Model->GetApplication()->GetParameterList(),
                                                           m_Model);
  
  scrollArea->setWidget(widgets);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidgetResizable(true);

  //
  // need to be connected to the end of a process
  QObject::connect(m_Model, SIGNAL( SetProgressReportDone() ),
                   this,
                   SLOT ( OnApplicationExecutionDone() )
    );

  //
  // setup the default output in Widgets OutputImageParameter
  FillOTBAppDefaultOutputImageParameter(widgets);
  
  return scrollArea;
}

/*******************************************************************************/
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

/*******************************************************************************/
QWidget* QtWidgetView::CreateDoc()
{
  QTextEdit *text = new QTextEdit;
  text->setReadOnly(true);

  QTextDocument * doc = new QTextDocument();

  std::string docContain;
  otb::Wrapper::ApplicationHtmlDocGenerator::GenerateDoc( m_Application, docContain);

  doc->setHtml(docContain.c_str());

  text->setDocument(doc);

  return text;
}

/*******************************************************************************/
void 
QtWidgetView::FillOTBAppDefaultOutputImageParameter( QWidget * widgets)
{
  //
  // Get the cache dir
  // get the const instance of the I18nApplication
  I18nApplication *  app = I18nApplication::Instance< I18nApplication >();
  QString cacheDir = app->GetCacheDir().absolutePath();

  // default output fname
  QString outfname = cacheDir + "/result/"+ m_Application->GetName()+".tif";

  //
  // get the OTB application widget layout
  QLayout * layout = widgets->layout();

  for (int idx = 0; idx < layout->count(); idx++ )
    {
    QWidget * currentWidget = layout->itemAt(idx)->widget();

    // is it a QtWidgetOutputImageParameter ?
    otb::Wrapper::QtWidgetOutputImageParameter * outParam 
      = qobject_cast<otb::Wrapper::QtWidgetOutputImageParameter *>(currentWidget);
    
    if (outParam)
      {
      outParam->SetFileName(outfname);
      outParam->UpdateGUI();
      } // else is it a {Group/Choice}Parameter Widget containing
        // QtWidgetOutputImageParameters ?
    else
      {
      // 
      QList< otb::Wrapper::QtWidgetOutputImageParameter *> outParameterWidget
        = currentWidget->findChildren<otb::Wrapper::QtWidgetOutputImageParameter*>();
  
      QList<otb::Wrapper::QtWidgetOutputImageParameter *>::iterator  it = outParameterWidget.begin();

      // 
      while(it != outParameterWidget.end())
        {
        if (*it)
          {
          (*it)->SetFileName(outfname);
          (*it)->UpdateGUI();
          }
        ++it;
        }
      }
    }

}


/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void QtWidgetView::CloseSlot()
{
  // Close the widget
  this->close();

  // Emit a signal to close any widget that this gui belonging to
  emit QuitSignal();
}

/*******************************************************************************/
void QtWidgetView::UpdateMessageAfterExcuteClicked()
{
  m_Message->setText("<center><font color=\"#FF0000\">Running</font></center>");
}

/*******************************************************************************/
void QtWidgetView::UpdateMessageAfterApplicationReady( bool val )
{
  if(val == true)
    m_Message->setText("<center><font color=\"#00FF00\">Ready to run</font></center>");
  else
    m_Message->setText("<center><font color=\"#FF0000\">Select parameters</font></center>");
}

/*******************************************************************************/
void QtWidgetView::OnApplicationExecutionDone()
{
  //
  // detect if this application has outputImageParameter. emit
  // the output filenames if any
  std::vector<std::string> paramList = m_Model->GetApplication()->GetParametersKeys(true);
  
  // iterate on the application parameters
  for (std::vector<std::string>::const_iterator it = paramList.begin();
          it != paramList.end();
          ++it)
    {
    // parameter key
    std::string key = *it;

    // get a valid outputParameter
    if (m_Model->GetApplication()->GetParameterType(key) == otb::Wrapper::ParameterType_OutputImage && 
        m_Model->GetApplication()->IsParameterEnabled(key) &&
        m_Model->GetApplication()->HasValue(key) )
        {
        // get the parameter
        otb::Wrapper::Parameter* param = m_Model->GetApplication()->GetParameterByKey(key);
        
        // try to cast it 
        otb::Wrapper::OutputImageParameter* outputParam = 
          dynamic_cast<otb::Wrapper::OutputImageParameter*>(param);

        // emit the output image filename selected
        if (outputParam)
          {
          emit OTBApplicationOutputImageChanged( QString ( m_Model->GetApplication()->GetName() ),// app
                                                 QString ( outputParam->GetFileName() )          // outfname
                                                 
            );
          }
        }
    }
}

}
}
