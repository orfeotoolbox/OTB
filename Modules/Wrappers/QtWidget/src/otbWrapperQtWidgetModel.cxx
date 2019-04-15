/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperQtWidgetModel.h"

//Use to create command line from the application parameters
#include "otbWrapperOutputProcessXMLParameter.h"

using std::string;

namespace otb
{

namespace Wrapper
{

QtWidgetModel
::QtWidgetModel(Application* app) :
  m_LogOutput(),
  m_taskAppli(nullptr),
  m_Application(app),
  m_IsRunning(false)
{
  // Init only if not already done
  if(!m_Application->IsInitialized())
    {
    m_Application->Init();
    }

  m_LogOutput = QtLogOutput::New();

  // Attach log output to the Application logger
  m_Application->GetLogger()->AddLogOutput(m_LogOutput);
}

QtWidgetModel::~QtWidgetModel()
{
  delete m_taskAppli;
}

void
QtWidgetModel
::NotifyUpdate()
{
  assert(!m_IsRunning && "Cannot update parameters while application is running.");

  // Update the parameters
  try
  {
    m_Application->UpdateParameters();
  }
  catch(otb::ApplicationException& err)
  {
    m_Application->GetLogger()->Debug("Caught otb::ApplicationException during application update:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    emit ExceptionRaised(err.what());
  }
  catch(otb::ImageFileReaderException& err)
  {
    m_Application->GetLogger()->Debug("Caught otb::ImageFileReaderException during application update:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    m_Application->GetLogger()->Fatal(err.GetDescription() + string("\n"));
    emit ExceptionRaised(err.what());
  }
  catch(itk::ExceptionObject& err)
  {
    m_Application->GetLogger()->Debug("Caught itk::ExceptionObject during application update:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    m_Application->GetLogger()->Fatal(string(err.GetDescription()) + "\n");
    emit ExceptionRaised(err.GetDescription());
  }
  catch(std::exception& err)
  {
    m_Application->GetLogger()->Fatal(string("Caught std::exception during application update: ") + err.what() + "\n");
    emit ExceptionRaised(err.what());
  }
  catch(...)
  {
    m_Application->GetLogger()->Fatal("Caught unknown exception during application update.\n");
    emit ExceptionRaised("Unknown exception");
  }

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

  // launch the output image writing
  delete m_taskAppli;
  m_taskAppli = new AppliThread(m_Application);

  QObject::connect( m_taskAppli, &AppliThread::ExceptionRaised,
                    this, &QtWidgetModel::ExceptionRaised );

  QObject::connect( m_taskAppli, &AppliThread::ApplicationExecutionDone,
                    this, &QtWidgetModel::OnApplicationExecutionDone );

  // Tell the Progress Reporter to begin
  emit SetProgressReportBegin();

  m_taskAppli->Execute();

  emit SetApplicationReady(true);
}

void QtWidgetModel::Stop()
{
  if (m_taskAppli && m_IsRunning)
  {
    m_taskAppli->Stop();
    m_taskAppli->wait();
    m_IsRunning = false;
  }
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

  try
  {
    result = m_Application->ExecuteAndWriteOutput();
  }
  catch(otb::ApplicationException& err)
  {
    // These are thrown with otbAppLogFATAL, a macro which logs a user
    // friendly error message before throwing. So log exception details only
    // in debug.
    m_Application->GetLogger()->Debug("Caught otb::ApplicationException during application execution:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    emit ExceptionRaised( err.what() );
  }
  catch(otb::ImageFileReaderException& err)
  {
    m_Application->GetLogger()->Debug("Caught otb::ImageFileReaderException during application execution:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    m_Application->GetLogger()->Fatal(err.GetDescription() + string("\n"));
    emit ExceptionRaised(err.what());
  }
  catch(itk::ProcessAborted& /*err*/)
  {
    m_Application->GetLogger()->Info("Processing aborted\n");
  }
  catch(itk::ExceptionObject& err)
  {
    m_Application->GetLogger()->Debug("Caught itk::ExceptionObject during application execution:\n");
    m_Application->GetLogger()->Debug(string(err.what()) + "\n");
    m_Application->GetLogger()->Fatal(string(err.GetDescription()) + "\n");
    emit ExceptionRaised( err.what() );
  }
  catch(std::exception& err)
  {
    m_Application->GetLogger()->Fatal(string("Caught std::exception during application execution: ") + err.what() + "\n");
    emit ExceptionRaised( err.what() );
  }
  catch(...)
  {
    m_Application->GetLogger()->Fatal("Caught unknown exception during application execution.\n");
    emit ExceptionRaised("Unknown exception.");
  }

  // Signal OTB-application has ended with result status.
  emit ApplicationExecutionDone( result );
}

}

}
