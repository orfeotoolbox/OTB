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

QtWidgetModel::QtWidgetModel(Application* app) :
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

void QtWidgetModel::NotifyUpdate()
{
  // Update the parameters
  m_Application->UpdateParameters();
  emit UpdateGui();

  // Notify all
  bool applicationStatus = m_Application->IsApplicationReady();
  emit SetApplicationReady(applicationStatus);
}

void QtWidgetModel::ExecuteAndWriteOutputSlot()
{
  // Deactivate the Execute button while processing
  emit SetApplicationReady(false);

  // launch the output image writing
  AppliThread * taskAppli = new AppliThread( m_Application );
  connect(taskAppli, SIGNAL(ApplicationExecutionDone()), this, SLOT(NotifyUpdate()));
  connect(taskAppli, SIGNAL(ApplicationExecutionDone()), this, SLOT(ActivateExecuteButton()));

  taskAppli->Execute();

  // Tell the Progress Reporter to begin
  emit SetProgressReportBegin();
}

void QtWidgetModel::ActivateExecuteButton()
{
  // For the view to activate the button "Execute"
  emit SetApplicationReady(true);

  // For the progressReport to close the Progress widget
  emit SetProgressReportDone();
}

void QtWidgetModel::SendLogWARNING( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::WARNING, mes );
}

void QtWidgetModel::SendLogINFO( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::INFO, mes );
}

void QtWidgetModel::SendLogDEBUG( const std::string & mes )
{
  m_Application->GetLogger()->Write( itk::LoggerBase::DEBUG, mes );
}


}
}
