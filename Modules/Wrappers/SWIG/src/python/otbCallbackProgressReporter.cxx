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

#include "otbCallbackProgressReporter.h"
#include "otbStopwatch.h"

namespace otb
{

CallbackProgressReporter
::CallbackProgressReporter()
{
}

CallbackProgressReporter
::CallbackProgressReporter(itk::ProcessObject* process,
                        const char *comment)
  : FilterWatcherBase(process, comment)
{
}

CallbackProgressReporter
::CallbackProgressReporter(itk::ProcessObject* process,
                        const std::string& comment)
  : FilterWatcherBase(process, comment.c_str())
{
}

void
CallbackProgressReporter
::ShowProgress()
{
  if (m_Process)
    {
    int progressPercent = static_cast<int>(m_Process->GetProgress() * 100);
    
    if (progressPercent > 100)
      {
      progressPercent = 100;
      }
    m_Callback->Call("\r"+std::to_string(progressPercent)+"%");
    m_Callback->Flush();
    }
}

void
CallbackProgressReporter
::StartFilter()
{
  m_Stopwatch.Start();
}

void
CallbackProgressReporter
::EndFilter()
{
  m_Stopwatch.Stop();
}

}
