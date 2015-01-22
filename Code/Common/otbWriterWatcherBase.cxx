/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbWriterWatcherBase.h"

namespace otb
{

WriterWatcherBase
::WriterWatcherBase()
{
  // Initialize state
  m_Comment = "Not watching any object";
  m_Process = 0;
}

WriterWatcherBase
::WriterWatcherBase(itk::ProcessObject* process,
                    const char *comment)
{
  // Initialize state
  m_Process = process;
  m_Comment = comment;

  // Create a series of commands
  m_StartWriterCommand = CommandType::New();
  m_EndWriterCommand = CommandType::New();
  m_ProgressWriterCommand = CommandType::New();
  m_StartFilterCommand = CommandType::New();
  m_EndFilterCommand = CommandType::New();
  m_ProgressFilterCommand = CommandType::New();

  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::StartFilter);
  m_EndFilterCommand->SetCallbackFunction(this,
                                          &WriterWatcherBase::EndFilter);
  m_ProgressFilterCommand->SetCallbackFunction(this,
                                               &WriterWatcherBase::ShowFilterProgress);
  m_StartWriterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::StartWriter);
  m_EndWriterCommand->SetCallbackFunction(this,
                                          &WriterWatcherBase::EndWriter);
  m_ProgressWriterCommand->SetCallbackFunction(this,
                                               &WriterWatcherBase::ShowWriterProgress);

  // Add the commands as observers
  m_StartWriterTag = m_Process->AddObserver(itk::StartEvent(),
                                            m_StartWriterCommand);

  m_EndWriterTag = m_Process->AddObserver(itk::EndEvent(),
                                          m_EndWriterCommand);

  m_ProgressWriterTag = m_Process->AddObserver(itk::ProgressEvent(),
                                               m_ProgressWriterCommand);

  m_StartFilterTag = 0;
  m_EndFilterTag = 0;
  m_ProgressFilterTag = 0;

 // Try to get the filter that is wired to m_Process.
  if (m_Process->GetInputs()[0]->GetSource())
    {
    m_SourceProcess = m_Process->GetInputs()[0]->GetSource();

    // Add the commands as observers
    m_StartFilterTag = m_SourceProcess->AddObserver(itk::StartEvent(),
                                                    m_StartFilterCommand);

    m_EndFilterTag = m_SourceProcess->AddObserver(itk::EndEvent(),
                                                  m_EndFilterCommand);

    m_ProgressFilterTag = m_SourceProcess->AddObserver(itk::ProgressEvent(),
                                                       m_ProgressFilterCommand);
    }
}

WriterWatcherBase
::WriterWatcherBase(itk::ProcessObject* process, itk::ProcessObject * source,
                    const char *comment)
{
  // Initialize state
  m_Process = process;
  m_Comment = comment;

  // Create a series of commands
  m_StartWriterCommand = CommandType::New();
  m_EndWriterCommand = CommandType::New();
  m_ProgressWriterCommand = CommandType::New();
  m_StartFilterCommand = CommandType::New();
  m_EndFilterCommand = CommandType::New();
  m_ProgressFilterCommand = CommandType::New();

  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::StartFilter);
  m_EndFilterCommand->SetCallbackFunction(this,
                                          &WriterWatcherBase::EndFilter);
  m_ProgressFilterCommand->SetCallbackFunction(this,
                                               &WriterWatcherBase::ShowFilterProgress);
  m_StartWriterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::StartWriter);
  m_EndWriterCommand->SetCallbackFunction(this,
                                          &WriterWatcherBase::EndWriter);
  m_ProgressWriterCommand->SetCallbackFunction(this,
                                               &WriterWatcherBase::ShowWriterProgress);

  // Add the commands as observers
  m_StartWriterTag = m_Process->AddObserver(itk::StartEvent(),
                                            m_StartWriterCommand);

  m_EndWriterTag = m_Process->AddObserver(itk::EndEvent(),
                                          m_EndWriterCommand);

  m_ProgressWriterTag = m_Process->AddObserver(itk::ProgressEvent(),
                                               m_ProgressWriterCommand);

  m_SourceProcess = source;

  // Add the commands as observers
  m_StartFilterTag = m_SourceProcess->AddObserver(itk::StartEvent(),
                                                  m_StartFilterCommand);

  m_EndFilterTag = m_SourceProcess->AddObserver(itk::EndEvent(),
                                                m_EndFilterCommand);

  m_ProgressFilterTag = m_SourceProcess->AddObserver(itk::ProgressEvent(),
                                                     m_ProgressFilterCommand);
}

WriterWatcherBase
::WriterWatcherBase(const WriterWatcherBase& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
    if (m_StartWriterCommand)
      {
      m_Process->RemoveObserver(m_StartWriterTag);
      }
    if (m_EndWriterCommand)
      {
      m_Process->RemoveObserver(m_EndWriterTag);
      }
    if (m_ProgressWriterCommand)
      {
      m_Process->RemoveObserver(m_ProgressWriterTag);
      }
    }
  if (m_SourceProcess)
    {
    if (m_StartFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_StartFilterTag);
      }
    if (m_EndFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_EndFilterTag);
      }
    if (m_ProgressFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_ProgressFilterTag);
      }
    }

  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_SourceProcess = watch.m_SourceProcess;
  m_Comment = watch.m_Comment;

  m_StartFilterTag = 0;
  m_EndFilterTag = 0;
  m_ProgressFilterTag = 0;
  m_StartWriterTag = 0;
  m_EndWriterTag = 0;
  m_ProgressWriterTag = 0;

  // Create a series of commands
  if (m_Process)
    {
    m_StartWriterCommand = CommandType::New();
    m_EndWriterCommand = CommandType::New();
    m_ProgressWriterCommand = CommandType::New();

    // Assign the callbacks
    m_StartWriterCommand->SetCallbackFunction(this,
                                              &WriterWatcherBase::StartWriter);
    m_EndWriterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::EndWriter);
    m_ProgressWriterCommand->SetCallbackFunction(this,
                                                 &WriterWatcherBase::ShowWriterProgress);

    // Add the commands as observers
    m_StartWriterTag = m_Process->AddObserver(itk::StartEvent(), m_StartWriterCommand);
    m_EndWriterTag = m_Process->AddObserver(itk::EndEvent(), m_EndWriterCommand);
    m_ProgressWriterTag
      = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressWriterCommand);
    }

  if (m_SourceProcess)
    {
    m_StartFilterCommand = CommandType::New();
    m_EndFilterCommand = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this,
                                              &WriterWatcherBase::StartFilter);
    m_EndFilterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::EndFilter);
    m_ProgressFilterCommand->SetCallbackFunction(this,
                                                 &WriterWatcherBase::ShowFilterProgress);

    // Add the commands as observers
    m_StartFilterTag = m_SourceProcess->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndFilterTag = m_SourceProcess->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressFilterTag
      = m_SourceProcess->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

void
WriterWatcherBase
::operator =(const WriterWatcherBase& watch)
{
// Remove any observers we have on the old process object
  if (m_Process)
    {
    if (m_StartWriterCommand)
      {
      m_Process->RemoveObserver(m_StartWriterTag);
      }
    if (m_EndWriterCommand)
      {
      m_Process->RemoveObserver(m_EndWriterTag);
      }
    if (m_ProgressWriterCommand)
      {
      m_Process->RemoveObserver(m_ProgressWriterTag);
      }
    }
  if (m_SourceProcess)
    {
    if (m_StartFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_StartFilterTag);
      }
    if (m_EndFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_EndFilterTag);
      }
    if (m_ProgressFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_ProgressFilterTag);
      }
    }

  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_SourceProcess = watch.m_SourceProcess;
  m_Comment = watch.m_Comment;

  m_StartFilterTag = 0;
  m_EndFilterTag = 0;
  m_ProgressFilterTag = 0;
  m_StartWriterTag = 0;
  m_EndWriterTag = 0;
  m_ProgressWriterTag = 0;

  // Create a series of commands
  if (m_Process)
    {
    m_StartWriterCommand = CommandType::New();
    m_EndWriterCommand = CommandType::New();
    m_ProgressWriterCommand = CommandType::New();

    // Assign the callbacks
    m_StartWriterCommand->SetCallbackFunction(this,
                                              &WriterWatcherBase::StartWriter);
    m_EndWriterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::EndWriter);
    m_ProgressWriterCommand->SetCallbackFunction(this,
                                                 &WriterWatcherBase::ShowWriterProgress);

    // Add the commands as observers
    m_StartWriterTag = m_Process->AddObserver(itk::StartEvent(), m_StartWriterCommand);
    m_EndWriterTag = m_Process->AddObserver(itk::EndEvent(), m_EndWriterCommand);
    m_ProgressWriterTag
      = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressWriterCommand);
    }
  if (m_SourceProcess)
    {
    m_StartFilterCommand = CommandType::New();
    m_EndFilterCommand = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this,
                                              &WriterWatcherBase::StartFilter);
    m_EndFilterCommand->SetCallbackFunction(this,
                                            &WriterWatcherBase::EndFilter);
    m_ProgressFilterCommand->SetCallbackFunction(this,
                                                 &WriterWatcherBase::ShowFilterProgress);

    // Add the commands as observers
    m_StartFilterTag = m_SourceProcess->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndFilterTag = m_SourceProcess->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressFilterTag
      = m_SourceProcess->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

WriterWatcherBase
::~WriterWatcherBase()
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
    if (m_StartWriterCommand)
      {
      m_Process->RemoveObserver(m_StartWriterTag);
      }
    if (m_EndWriterCommand)
      {
      m_Process->RemoveObserver(m_EndWriterTag);
      }
    if (m_ProgressWriterCommand)
      {
      m_Process->RemoveObserver(m_ProgressWriterTag);
      }
    }
  if (m_SourceProcess)
    {
    if (m_StartFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_StartFilterTag);
      }
    if (m_EndFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_EndFilterTag);
      }
    if (m_ProgressFilterCommand)
      {
      m_SourceProcess->RemoveObserver(m_ProgressFilterTag);
      }
    }
}

} // end namespace otb
