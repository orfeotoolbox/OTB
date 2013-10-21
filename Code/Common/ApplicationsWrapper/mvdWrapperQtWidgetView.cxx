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
//
#include "Core/mvdI18nCoreApplication.h"

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
QtWidgetView
::QtWidgetView( otb::Wrapper::Application::Pointer otbApp,
		QWidget* parent,
		Qt::WindowFlags flags ) :
  m_Application( otbApp ),
  m_Model( NULL ),
  m_ExecButton( NULL ),
  m_QuitButton( NULL ),
  m_Message( NULL ),
  m_IsClosable( true )
{
  m_Model = new otb::Wrapper::QtWidgetModel( otbApp );

  QObject::connect(
    m_Model, SIGNAL( SetProgressReportBegin() ),
    this, SLOT( OnProgressReportBegin() )
  );

  QObject::connect(
    m_Model, SIGNAL( SetProgressReportDone( int ) ),
    this, SLOT( OnProgressReportEnd( int ) )
  );

  QObject::connect(
    m_Model, SIGNAL( ExceptionRaised( QString ) ),
    this, SLOT( OnExceptionRaised( QString ) )
  );
}

/*******************************************************************************/
QtWidgetView
::~QtWidgetView()
{
  // m_Application is smart-pointed and will be automatically deleted.

  delete m_Model;
  m_Model = NULL;
}

/*******************************************************************************/
void
QtWidgetView
::CreateGui()
{
  // Create a VBoxLayout with the header, the input widgets, and the footer
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QTabWidget *tab = new QTabWidget();
  tab->addTab(CreateInputWidgets(), "Parameters");

   
  //otb::Wrapper::QtWidgetProgressReport* prog =  new otb::Wrapper::QtWidgetProgressReport(m_Model);
  //prog->SetApplication(m_Application);
  //tab->addTab(prog, "Progress");
  tab->addTab(CreateDoc(), "Documentation");
  mainLayout->addWidget(tab);

  QTextEdit *log = new QTextEdit();
  connect( m_Model->GetLogOutput(), SIGNAL(NewContentLog(QString)), log, SLOT(append(QString) ) );
  tab->addTab(log, "Logs");

  m_Message = new QLabel("<center><font color=\"#FF0000\">Select parameters</font></center>");
  connect(
    m_Model,
    SIGNAL( SetApplicationReady( bool ) ),
    this, SLOT( UpdateMessageAfterApplicationReady( bool ) )
  );
  mainLayout->addWidget(m_Message);

  otb::Wrapper::QtWidgetSimpleProgressReport* progressReport =
    new otb::Wrapper::QtWidgetSimpleProgressReport(m_Model);
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
QWidget*
QtWidgetView
::CreateInputWidgets()
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
  QObject::connect(
    m_Model,
    SIGNAL( SetProgressReportDone( int ) ),
    // to:
    this,
    SLOT ( OnApplicationExecutionDone( int ) )
    );

  //
  // setup the default output in Widgets OutputImageParameter
  FillOTBAppDefaultOutputImageParameter(widgets);
  
  return scrollArea;
}

/*******************************************************************************/
QWidget*
QtWidgetView
::CreateFooter()
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
  connect(
    m_Model, SIGNAL( SetApplicationReady( bool ) ),
    m_ExecButton, SLOT( setEnabled( bool ) )
  );

  QObject::connect(
    m_ExecButton, SIGNAL( clicked() ),
    // to:
    this, SLOT( OnExecButtonClicked() )
  );
  QObject::connect(
    this, SIGNAL( ExecuteAndWriteOutput() ),
    // to:
    m_Model, SLOT( ExecuteAndWriteOutputSlot() )
  );

  m_QuitButton = new QPushButton(footerGroup);
  m_QuitButton->setText(QObject::tr("Quit"));
  connect(
    m_QuitButton, SIGNAL( clicked() ),
    this, SLOT( CloseSlot() )
  );

  // Put the buttons on the right
  footerLayout->addStretch();
  footerLayout->addWidget(m_ExecButton);
  footerLayout->addWidget(m_QuitButton);

  footerGroup->setLayout(footerLayout);

  return footerGroup;
}

/*******************************************************************************/
QWidget*
QtWidgetView
::CreateDoc()
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
QtWidgetView
::FillOTBAppDefaultOutputImageParameter( QWidget * widgets)
{
  //
  // Get the cache dir
  // get the const instance of the application.
  I18nCoreApplication* app = I18nCoreApplication::Instance();

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
      // generate a default output fname for the current
      // OutputImageParameter 
      QString outfname = 
        app->GetResultsDir().absolutePath() 
        + "/"+ m_Application->GetName()
        +"_"+ GenerateIdentifier() +".tif";

      // use it
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
          // generate a default output fname for the current
          // OutputImageParameter 
          QString outfname = 
            app->GetResultsDir().absolutePath() + "/"+ m_Application->GetName()+"_"+GenerateIdentifier()+".tif";

          // use the outfname
          (*it)->SetFileName(outfname);
          (*it)->UpdateGUI();
          }
        ++it;
        }
      }
    }

}


/*******************************************************************************/
QString 
QtWidgetView
::GenerateIdentifier()
{
  //
  // get an unique identifier  and remove braces
  QString  identifier = QUuid::createUuid().toString();
  identifier.replace("{", "");
  identifier.replace("}", "");

  return identifier;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
QtWidgetView
::CloseSlot()
{
  // Close the widget
  this->close();

  // Emit a signal to close any widget that this gui belonging to
  emit QuitSignal();
}

/*******************************************************************************/
#if 0
void
QtWidgetView
::UpdateMessageAfterExcuteClicked()
{
  m_Message->setText("<center><font color=\"#FF0000\">Running</font></center>");
}
#endif

/*******************************************************************************/
void
QtWidgetView
::OnExecButtonClicked()
{
  assert( m_Model!=NULL );
  assert( m_Model->GetApplication()!=NULL );

  otb::Wrapper::Application::Pointer otbApp( m_Model->GetApplication() );

  StringVector paramKeys( otbApp->GetParametersKeys() );

  bool isSure = true;

  /*
  typedef QVector< QFileInfo > FileInfoVector;

  FileInfoVector fileInfos;
  */

  for( StringVector::const_iterator it( paramKeys.begin() );
       it!=paramKeys.end() && isSure;
       ++it )
    {
    if( otbApp->GetParameterType( *it )==
	otb::Wrapper::ParameterType_OutputImage &&
        otbApp->IsParameterEnabled( *it ) &&
        otbApp->HasValue( *it ) )
      {
      otb::Wrapper::Parameter::Pointer param( otbApp->GetParameterByKey( *it ) );

      otb::Wrapper::OutputImageParameter::Pointer outImgParam(
	otb::DynamicCast< otb::Wrapper::OutputImageParameter >( param )
      );

      QFileInfo fileInfo( outImgParam->GetFileName() );

      if( fileInfo.exists() )
	{
	QMessageBox::StandardButton questionButton =
	  QMessageBox::question(
	    this,
	    tr( PROJECT_NAME ),
	    tr( "Are you sure you want to overwrite file '%1'?" )
	    .arg( outImgParam->GetFileName() ),
	    QMessageBox::Yes | QMessageBox::No,
	    QMessageBox::No
	  );

	if( questionButton==QMessageBox::Yes )
	  {
	  /*
	  fileInfos.push_back( fileInfo );
	  */
	  }
	else
	  isSure = false;
	}
      }
    }

  if( !isSure )
    return;

  /* U N S A F E
  // BUGFIX: Mantis-750
  //
  // Remove files which will be overwritten in order to use
  // file-existence to check whether to emit the OutputImageChanged
  // signal (see ::OnApplicationExecutionDone()).
  for( FileInfoVector::const_iterator it( fileInfos.begin() );
       it!=fileInfos.end();
       ++it )
    {
    qDebug() << "Removing:" << it->filePath();

    it->dir().remove( it->fileName() );
    }
  */

  m_Message->setText("<center><font color=\"#FF0000\">Running</font></center>");

  emit ExecuteAndWriteOutput();
}

/*******************************************************************************/
void
QtWidgetView
::UpdateMessageAfterApplicationReady( bool val )
{
  if(val == true)
    m_Message->setText("<center><font color=\"#00FF00\">Ready to run</font></center>");
  else
    m_Message->setText("<center><font color=\"#FF0000\">Select parameters</font></center>");
}

/*******************************************************************************/
void
QtWidgetView
::OnExceptionRaised( QString what  )
{
  qWarning() << what;

  QMessageBox::warning(
    this,
    PROJECT_NAME,
    what,
    QMessageBox::Ok
  );
}

/*******************************************************************************/
void
QtWidgetView
::OnApplicationExecutionDone( int status )
{
  otb::Wrapper::Application::Pointer otbApp( m_Model->GetApplication() );

  if( status!=0 )
    {
    QMessageBox::information(
      this,
      PROJECT_NAME,
      tr( "'%1' has failed with return status %2.\n"
	  "Please refer to '%1' documentation."
      )
      .arg( otbApp->GetName() )
      .arg( status ),
      QMessageBox::Ok
    );

    emit ExecutionDone( status );

    return;
    }

#if 1
  QMessageBox::information(
    this,
    PROJECT_NAME,
    tr( "'%1' has succeeded with return status %2.\n"
	"Result(s) will be imported as dataset(s).")
    .arg( otbApp->GetName() )
    .arg( status ),
    QMessageBox::Ok
  );

#else
  QMessageBox::StandardButton button =
    QMessageBox::information(
      this,
      PROJECT_NAME,
      tr( "'%1' has succeeded with return status %2.\n"
	  "Would you like to import results?")
      .arg( otbApp->GetName() )
      .arg( status ),
      QMessageBox::Yes | QMessageBox::No,
      QMessageBox::Yes
    );

  if( button==QMessageBox::No )
    {
    return;
    }
#endif

  CountType count = 0;

  //
  // detect if this application has outputImageParameter. emit
  // the output filenames if any
  StringVector paramList( otbApp->GetParametersKeys( true ) );

  // iterate on the application parameters
  for ( StringVector::const_iterator it( paramList.begin() );
	it!=paramList.end();
	++it )
    {
    // parameter key
    const std::string& key = *it;

    // get a valid outputParameter
    if( otbApp->GetParameterType( key )
	==otb::Wrapper::ParameterType_OutputImage && 
	otbApp->IsParameterEnabled (key ) &&
	otbApp->HasValue( key ) )
      {
      // get the parameter
      otb::Wrapper::Parameter* param = otbApp->GetParameterByKey( key );

      // try to cast it 
      otb::Wrapper::OutputImageParameter* outputParam = 
	dynamic_cast< otb::Wrapper::OutputImageParameter* >( param );

      // emit the output image filename selected
      if( outputParam!=NULL )
	{
	QFileInfo fileInfo( outputParam->GetFileName() );

	/* U N S A F E
	// BUGFIX: Mantis-750
	//
	// If output image-exists, it's sure that it has been output
	// from the OTB-application process because overwritten
	// files are first deleted (see OnExecButtonClicked()).
	if( fileInfo.exists() )
	{
	*/
	++ count;

	emit OTBApplicationOutputImageChanged(
	  QString( otbApp->GetName() ),
	  QString( outputParam->GetFileName() )
	);
	/*
	}
	*/
	}
      }
    }

  emit ExecutionDone( status );
}

}

}
