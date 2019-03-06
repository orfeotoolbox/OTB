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

CallbackProgressReporter::CallbackProgressReporter()
{
}

CallbackProgressReporter::CallbackProgressReporter(itk::ProcessObject* process, const char* comment)
  : FilterWatcherBase(process, comment), m_StarsCount(50), m_CurrentNbStars(-1)
{
}

CallbackProgressReporter::CallbackProgressReporter(itk::ProcessObject* process, const std::string& comment)
  : FilterWatcherBase(process, comment.c_str()), m_StarsCount(50), m_CurrentNbStars(-1)
{
}

void CallbackProgressReporter::ShowProgress()
{
  if (m_Process)
  {
    int progressPercent = static_cast<int>(m_Process->GetProgress() * 100);
    int nbStars         = static_cast<int>(m_Process->GetProgress() * m_StarsCount);
    int nbBlanks        = m_StarsCount - nbStars;

    if (nbBlanks < 0)
    {
      nbBlanks = 0;
    }

    if (nbStars > m_StarsCount)
    {
      nbStars = m_StarsCount;
    }

    if (progressPercent > 100)
    {
      progressPercent = 100;
    }

    if (nbStars > m_CurrentNbStars)
    {
      std::string stars(nbStars, '*');
      std::string blanks(nbBlanks, ' ');

      if (m_Callback->Isatty())
      {
        m_Callback->Call("\r" + m_Comment + ": " + std::to_string(progressPercent) + "% [" + stars + blanks + "]");
        m_Callback->Flush();
      }
      else
      {
        m_Buffer = "\r" + m_Comment + ": " + std::to_string(progressPercent) + "% [" + stars + blanks + "]";
      }
    }

    m_CurrentNbStars = nbStars;
  }
}

void CallbackProgressReporter::StartFilter()
{
  m_Stopwatch.Start();
}

void CallbackProgressReporter::EndFilter()
{
  m_Stopwatch.Stop();

  if (m_Process && !(m_Callback->Isatty()))
  {
    m_Callback->Call(m_Buffer);
    m_Buffer = std::string("");
  }

  m_Callback->Call(" (" + m_Stopwatch.GetElapsedHumanReadableTime() + ")\n");
}
}
