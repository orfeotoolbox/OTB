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
}

StandardFilterWatcher
::StandardFilterWatcher(const StandardFilterWatcher& watch) : FilterWatcherBase(watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardFilterWatcher
::operator =(const StandardFilterWatcher& watch)
{
  // Initialize state
  FilterWatcherBase::operator=(watch);
  m_StarsCount = watch.m_StarsCount;
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
  m_TimeProbe.Start();
  std::cout << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")
            << " \"" << m_Comment << "\" " << std::endl;
}

void
StandardFilterWatcher
::EndFilter()
{
  m_TimeProbe.Stop();
  std::cout << std::endl << "Filter took "
            << m_TimeProbe.GetMean()
            << " seconds." << std::endl;
}
} // end namespace otb
