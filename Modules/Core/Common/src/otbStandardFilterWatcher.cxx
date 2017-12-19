/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbStandardFilterWatcher.h"

namespace otb
{

StandardFilterWatcher
::StandardFilterWatcher(itk::ProcessObject* process,
                        const char *comment)
  : FilterWatcherBase(process, comment)
{
  m_StarsCount = 50;
  m_CurrentNbStars = -1;
}

StandardFilterWatcher
::StandardFilterWatcher(itk::ProcessObject* process,
                        const std::string& comment)
  : FilterWatcherBase(process, comment.c_str())
{
  m_StarsCount = 50;
  m_CurrentNbStars = -1;
}

StandardFilterWatcher
::StandardFilterWatcher(const StandardFilterWatcher& watch) : FilterWatcherBase(watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
  m_CurrentNbStars = watch.m_CurrentNbStars;
}

void
StandardFilterWatcher
::operator =(const StandardFilterWatcher& watch)
{
  // Initialize state
  FilterWatcherBase::operator=(watch);
  m_StarsCount = watch.m_StarsCount;
  m_CurrentNbStars = watch.m_CurrentNbStars;
}

void
StandardFilterWatcher
::ShowProgress()
{
  if (m_Process)
    {
    int progressPercent = static_cast<int>(m_Process->GetProgress() * 100);
    int nbStars = static_cast<int>(m_Process->GetProgress() * m_StarsCount);
    int nbBlanks = m_StarsCount - nbStars;

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
      std::cout << "\rProcessing progress: " << progressPercent << "% [" << stars << blanks << "]" << std::flush;
      }

    m_CurrentNbStars = nbStars;
    }
}

void
StandardFilterWatcher
::StartFilter()
{
  m_Stopwatch.Start();
  std::cout << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")
            << " \"" << m_Comment << "\" " << std::endl;
}

void
StandardFilterWatcher
::EndFilter()
{
  m_Stopwatch.Stop();
  std::cout << std::endl << "Filter took "
            << m_Stopwatch.GetElapsedMilliseconds() / 1000
            << " seconds." << std::endl;
}
} // end namespace otb
