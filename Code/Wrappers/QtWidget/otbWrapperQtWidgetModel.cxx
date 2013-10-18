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

namespace otb
{

namespace Wrapper
{

QtWidgetModel
::QtWidgetModel(Application* app) :
  m_Application(app),
  m_LogOutput()
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
  bool applicationStatus = m_Application->IsApplicationReady();
  emit SetApplicationReady(applicationStatus);
}

void
QtWidgetModel
::ExecuteAndWriteOutputSlot()
{
  // Deactivate the Execute button while processing
  emit SetApplicationReady(false);

  // launch the output image writing
  AppliThread * taskAppli = new AppliThread( m_Application );

  QObject::connect(
    taskAppli,
    SIGNAL( ExceptionRaised( QString ) ),
    // to:
    this,
    SLOT( ExceptionRaised( QString ) )
  );

  QObject::connect(
    taskAppli,
    SIGNAL( ApplicationExecutionDone( int ) ),
    // to:
    this,
    SLOT( OnApplicationExecutionDone( int ) )
  );

  taskAppli->Execute();

  // Tell the Progress Reporter to begin
  emit SetProgressReportBegin();
}

void
QtWidgetModel
::OnApplicationExecutionDone( int status )
{
  // Require GUI update.
  NotifyUpdate();

  // For the view to activate the button "Execute"
  emit SetApplicationReady( true );

  // For the progressReport to close the Progress widget
  emit SetProgressReportDone( status );
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
