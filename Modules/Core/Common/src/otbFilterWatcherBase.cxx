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
#include "otbFilterWatcherBase.h"

namespace otb
{

FilterWatcherBase
::FilterWatcherBase()
: m_Comment("Not watching an object"),
  m_Process(0),
  m_StartTag(0),
  m_EndTag(0),
  m_ProgressTag(0),
  m_Started(false),
  m_Ended(false)
{
}

FilterWatcherBase
::FilterWatcherBase(itk::ProcessObject* process,
                    const char *comment)
: m_Comment(comment),
  m_Process(process),
  m_StartTag(0),
  m_EndTag(0),
  m_ProgressTag(0),
  m_Started(false),
  m_Ended(false)
{
  // Create a series of commands
  m_StartFilterCommand = CommandType::New();
  m_EndFilterCommand = CommandType::New();
  m_ProgressFilterCommand = CommandType::New();

  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this,
                                            &FilterWatcherBase::StartFilterCallback);
  m_EndFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcherBase::EndFilterCallback);
  m_ProgressFilterCommand->SetCallbackFunction(this,
                                               &FilterWatcherBase::ShowProgressCallback);

  // Add the commands as observers
  m_StartTag = m_Process->AddObserver(itk::StartEvent(),
                                      m_StartFilterCommand);

  m_EndTag = m_Process->AddObserver(itk::EndEvent(),
                                    m_EndFilterCommand);

  m_ProgressTag = m_Process->AddObserver(itk::ProgressEvent(),
                                         m_ProgressFilterCommand);
}

FilterWatcherBase
::FilterWatcherBase(const FilterWatcherBase& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
    m_Process->RemoveAllObservers();
    }

  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_Comment = watch.m_Comment;

  m_StartTag = 0;
  m_EndTag = 0;
  m_ProgressTag = 0;
  m_Started = false;
  m_Ended = false;

  // Create a series of commands
  if (m_Process)
    {
    m_StartFilterCommand = CommandType::New();
    m_EndFilterCommand = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this,
                                              &FilterWatcherBase::StartFilterCallback);
    m_EndFilterCommand->SetCallbackFunction(this,
                                            &FilterWatcherBase::EndFilterCallback);
    m_ProgressFilterCommand->SetCallbackFunction(this,
                                                 &FilterWatcherBase::ShowProgressCallback);

    // Add the commands as observers
    m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressTag
      = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

void
FilterWatcherBase
::operator =(const FilterWatcherBase& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
    m_Process->RemoveAllObservers();
    }

  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_Comment = watch.m_Comment;

  m_StartTag = 0;
  m_EndTag = 0;
  m_ProgressTag = 0;

  // Create a series of commands
  if (m_Process)
    {
    m_StartFilterCommand = CommandType::New();
    m_EndFilterCommand = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this,
                                              &FilterWatcherBase::StartFilterCallback);
    m_EndFilterCommand->SetCallbackFunction(this,
                                            &FilterWatcherBase::EndFilterCallback);
    m_ProgressFilterCommand->SetCallbackFunction(this,
                                                 &FilterWatcherBase::ShowProgressCallback);

    // Add the commands as observers
    m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressTag
      = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

FilterWatcherBase
::~FilterWatcherBase()
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
    m_Process->RemoveAllObservers();
    }
}

} // end namespace otb
