/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#include "otbFilterWatcherBase.h"

namespace otb
{

FilterWatcherBase::FilterWatcherBase()
  : m_Comment("Not watching an object"), m_Process(nullptr), m_StartTag(0), m_EndTag(0), m_ProgressTag(0), m_Started(false), m_Ended(false)
{
}

FilterWatcherBase::FilterWatcherBase(itk::ProcessObject* process, const char* comment)
  : m_Comment(comment), m_Process(process), m_StartTag(0), m_EndTag(0), m_ProgressTag(0), m_Started(false), m_Ended(false)
{
  // Create a series of commands
  m_StartFilterCommand    = CommandType::New();
  m_EndFilterCommand      = CommandType::New();
  m_ProgressFilterCommand = CommandType::New();

  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::StartFilterCallback);
  m_EndFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::EndFilterCallback);
  m_ProgressFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::ShowProgressCallback);

  // Add the commands as observers
  m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);

  m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);

  m_ProgressTag = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
}

FilterWatcherBase::FilterWatcherBase(const FilterWatcherBase& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
  {
    m_Process->RemoveAllObservers();
  }

  // Initialize state
  m_Stopwatch = watch.m_Stopwatch;
  m_Process   = watch.m_Process;
  m_Comment   = watch.m_Comment;

  m_StartTag    = 0;
  m_EndTag      = 0;
  m_ProgressTag = 0;
  m_Started     = false;
  m_Ended       = false;

  // Create a series of commands
  if (m_Process)
  {
    m_StartFilterCommand    = CommandType::New();
    m_EndFilterCommand      = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::StartFilterCallback);
    m_EndFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::EndFilterCallback);
    m_ProgressFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::ShowProgressCallback);

    // Add the commands as observers
    m_StartTag    = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndTag      = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressTag = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
  }
}

void FilterWatcherBase::operator=(const FilterWatcherBase& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
  {
    m_Process->RemoveAllObservers();
  }

  // Initialize state
  m_Stopwatch = watch.m_Stopwatch;
  m_Process   = watch.m_Process;
  m_Comment   = watch.m_Comment;

  m_StartTag    = 0;
  m_EndTag      = 0;
  m_ProgressTag = 0;

  // Create a series of commands
  if (m_Process)
  {
    m_StartFilterCommand    = CommandType::New();
    m_EndFilterCommand      = CommandType::New();
    m_ProgressFilterCommand = CommandType::New();

    // Assign the callbacks
    m_StartFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::StartFilterCallback);
    m_EndFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::EndFilterCallback);
    m_ProgressFilterCommand->SetCallbackFunction(this, &FilterWatcherBase::ShowProgressCallback);

    // Add the commands as observers
    m_StartTag    = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
    m_EndTag      = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
    m_ProgressTag = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
  }
}

FilterWatcherBase::~FilterWatcherBase()
{
  // Remove any observers we have on the old process object
  if (m_Process)
  {
    m_Process->RemoveAllObservers();
  }
}

} // end namespace otb
