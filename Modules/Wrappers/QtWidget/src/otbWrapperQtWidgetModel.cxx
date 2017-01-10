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
#include "otbWrapperQtWidgetModel.h"

//Use to create command line from the application parameters
#include "otbWrapperOutputProcessXMLParameter.h"

namespace otb
{

namespace Wrapper
{

QtWidgetModel
::QtWidgetModel(Application* app) :
  m_Application(app),
  m_LogOutput(),
  m_IsRunning(false)
{
  // Init only if not already done
  if(!m_Application->IsInitialized())
    {
    m_Application->Init();
    }

  m_LogOutput = QtLogOutput::New();

  // Attach log output to the Application logger
  m_Application->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  m_Application->GetLogger()->AddLogOutput(m_LogOutput);

  m_Timer = new QTimer(this);
  m_Timer->setSingleShot(true);
  m_Timer->setInterval(1000);
  QObject::connect(
    m_Timer,
    SIGNAL( timeout() ),
    this,
    SLOT( TimerDone() )
    );
}

QtWidgetModel::~QtWidgetModel()
{
}

void
QtWidgetModel
::NotifyUpdate()
{
  // Update the parameters
  m_Application->UpdateParameters();
  emit UpdateGui();

  // Notify all
  if (!m_IsRunning)
    {
    bool applicationStatus = m_Application->IsApplicationReady();
    emit SetApplicationReady(applicationStatus);
    }
}

void
QtWidgetModel
::ExecuteAndWriteOutputSlot()
{
  // Deactivate the Execute button while processing
  emit SetApplicationReady(false);
  m_IsRunning = true;

  //Buld corresponding command line and display to the Log tab

  //Build XML DOM from m_application
  OutputProcessXMLParameter::Pointer outXMLParam = OutputProcessXMLParameter::New();

  TiXmlElement* XMLAppElement = outXMLParam->ParseApplication(m_Application);
 
  //Create command line from the XML document
  TiXmlElement * pName, *pParam;
  std::ostringstream cmdLine;

  cmdLine << "";
    
  if(XMLAppElement)
    {
    pName = XMLAppElement->FirstChildElement("name");

    cmdLine << "otbcli_" << pName->GetText();
#ifdef _WIN32
    cmdLine << ".bat";
#endif
    cmdLine << " ";

    //Parse application parameters
    pParam = XMLAppElement->FirstChildElement("parameter");
    
    while(pParam)
      {
      //Get pareter key
      cmdLine << "-";
      cmdLine << pParam->FirstChildElement("key")->GetText();
      cmdLine << " ";

      //Some parameters can have multiple values. Test it and handle this
      //specific case
      TiXmlElement * values = pParam->FirstChildElement("values");

      if (values)
        {
        //Loop over value
        TiXmlElement * pValue = values->FirstChildElement("value");
        while(pValue)
          {
          cmdLine << pValue->GetText();
          cmdLine << " ";
            
          pValue = pValue->NextSiblingElement(); // iteration over multiple values 
          }
        }
      else
        {
        //Get parameter value
        cmdLine << pParam->FirstChildElement("value")->GetText();
        cmdLine << " ";

        //In case of OutputImageparameter we need to report output pixel type
        TiXmlElement * pPixType = pParam->FirstChildElement("pixtype");

        if (pPixType)
          {
          cmdLine << pPixType->GetText();
          cmdLine << " ";
          }
        }
        
      pParam = pParam->NextSiblingElement(); // iteration over parameters
      }

    //Insert a new line character at the end of the command line
    cmdLine << std::endl;
    
    //Report the command line string to the application logger
    m_Application->GetLogger()->Write(itk::LoggerBase::INFO, cmdLine.str());
    }

  // launch the output image writing
  AppliThread * taskAppli = new AppliThread( m_Application );

  QObject::connect(
    taskAppli,
    SIGNAL( ExceptionRaised( QString ) ),
    // to:
    this,
    SIGNAL( ExceptionRaised( QString ) )
  );

  QObject::connect(
    taskAppli,
    SIGNAL( ApplicationExecutionDone( int ) ),
    // to:
    this,
    SLOT( OnApplicationExecutionDone( int ) )
  );

  // Tell the Progress Reporter to begin
  emit SetProgressReportBegin();

  taskAppli->Execute();
}

void
QtWidgetModel
::OnApplicationExecutionDone( int status )
{
  // For the progressReport to close the Progress widget
  // and the GUI to update message
  emit SetProgressReportDone( status );

  if (status >= 0)
    {
    std::ostringstream oss;
    oss << "Execution took "<< m_Application->GetLastExecutionTiming() << " sec";
    SendLogINFO(oss.str());
    }

  // start timer
  m_Timer->start();
}

void
QtWidgetModel
::TimerDone()
{
  m_IsRunning = false;
  // Require GUI update.
  NotifyUpdate();
}

void
QtWidgetModel
::SendLogWARNING( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::WARNING, mes );
}

void
QtWidgetModel
::SendLogINFO( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::INFO, mes );
}

void
QtWidgetModel
::SendLogDEBUG( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::DEBUG, mes );
}

}

}

namespace otb
{

namespace Wrapper
{

AppliThread
::~AppliThread()
{
  wait();
}

void
AppliThread
::run()
{
  int result = -1;

  //
  // Try to execute OTB-application.
  try
    {
    result = m_Application->ExecuteAndWriteOutput();
    }
  //
  // Catch standard exceptions.
  catch( std::exception& err )
    {
    std::ostringstream message;

    message
      << "The following error occurred during OTB-application execution: "
      << err.what()
      << std::endl;

    m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, message.str() );

    // Signal exception.
    emit ExceptionRaised( err.what() );
    }
  //
  // Catch other exceptions.
  catch( ... )
    {
    m_Application->GetLogger()->Write(
      itk::LoggerBase::FATAL,
      "An unknown exception has been raised during OTB-application execution"
    );

    // Signal exception.
    emit ExceptionRaised( "Exception raised by OTB-application." );
    }
  //
  // Signal OTB-application has ended with result status.
  emit ApplicationExecutionDone( result );
}

}

}
