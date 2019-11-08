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

#include <iostream>

#include "otbStandardWriterWatcher.h"
#include "otbStopwatch.h"
#include "otbSystem.h"

namespace otb
{

StandardWriterWatcher::StandardWriterWatcher(itk::ProcessObject* process, const char* comment)
  : WriterWatcherBase(process, comment), m_StarsCount(50), m_CoutIsConsole(System::IsInteractive(1))
{
}

StandardWriterWatcher::StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject* source, const char* comment)
  : WriterWatcherBase(process, source, comment), m_StarsCount(50), m_CoutIsConsole(System::IsInteractive(1))
{
}

StandardWriterWatcher::StandardWriterWatcher(itk::ProcessObject* process, const std::string& comment)
  : WriterWatcherBase(process, comment.c_str()), m_StarsCount(50), m_CoutIsConsole(System::IsInteractive(1))
{
}

StandardWriterWatcher::StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject* source, const std::string& comment)
  : WriterWatcherBase(process, source, comment.c_str()), m_StarsCount(50), m_CoutIsConsole(System::IsInteractive(1))
{
}

StandardWriterWatcher::StandardWriterWatcher(const StandardWriterWatcher& watch)
  : WriterWatcherBase(watch), m_StarsCount(watch.m_StarsCount), m_CoutIsConsole(System::IsInteractive(1))
{
}

void StandardWriterWatcher::operator=(const StandardWriterWatcher& watch)
{
  // Initialize state
  WriterWatcherBase::operator=(watch);
  m_StarsCount               = watch.m_StarsCount;
}

void StandardWriterWatcher::ShowWriterProgress()
{
  this->ShowProgress();
}

void StandardWriterWatcher::ShowFilterProgress()
{
  this->ShowProgress();
}

void StandardWriterWatcher::ShowProgress()
{
  std::ostringstream oss;
  oss.str("");

  if (m_SourceProcess)
  {
    double       progress        = m_SourceProcess->GetProgress();
    int          progressPercent = static_cast<int>(progress * 100);
    unsigned int nbStars         = static_cast<int>(progress * m_StarsCount);
    if (nbStars > m_StarsCount)
    {
      nbStars = m_StarsCount;
    }

    std::string stars(nbStars, '*');
    std::string blanks(m_StarsCount - nbStars, ' ');
    oss << "Current Tile: ";
    if (progressPercent < 10)
    {
      oss << " ";
    }
    if (progressPercent < 100)
    {
      oss << " ";
    }
    oss << progressPercent << "% [" << stars << blanks << "]  ";
  }

  if (m_Process)
  {
    double       progress        = m_Process->GetProgress();
    int          progressPercent = static_cast<int>(progress * 100);
    unsigned int nbStars         = static_cast<int>(progress * m_StarsCount);
    if (nbStars > m_StarsCount)
    {
      nbStars = m_StarsCount;
    }
    std::string stars(nbStars, '*');
    std::string blanks(m_StarsCount - nbStars, ' ');
    oss << "Writing: ";
    if (progressPercent < 10)
    {
      oss << " ";
    }
    if (progressPercent < 100)
    {
      oss << " ";
    }
    oss << progressPercent << "% [" << stars << blanks << "]";
  }

  if (m_CoutIsConsole)
  {
    std::cout << "\r" << oss.str() << std::flush;
  }
  else
  {
    m_Buffer = oss.str();
  }
}

void StandardWriterWatcher::StartWriter()
{
  m_Stopwatch.Start();
  std::cout << "Writing task: "
            << " \"" << m_Comment << "\" " << std::endl;
  std::cout << "Writer type: " << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None") << std::endl;
  std::cout << "Filter type: " << (m_SourceProcess.GetPointer() ? m_SourceProcess->GetNameOfClass() : "None") << std::endl;
}

void StandardWriterWatcher::EndWriter()
{
  m_Stopwatch.Stop();
  if (!m_CoutIsConsole)
  {
    std::cout << m_Buffer;
    m_Buffer = std::string("");
  }
  std::cout << std::endl << "Writing task took ";
  m_Stopwatch.GetElapsedHumanReadableTime(std::cout);
  std::cout << std::endl;
}

} // end namespace otb
