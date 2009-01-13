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
}

StandardFilterWatcher
::StandardFilterWatcher( const StandardFilterWatcher& watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardFilterWatcher
::operator=(const StandardFilterWatcher &watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardFilterWatcher
::ShowProgress()
{
  if (m_Process)
    {
      int progressPercent = static_cast<int>(m_Process->GetProgress()*100);
      std::string stars(static_cast<int>(m_Process->GetProgress()*m_StarsCount),'*');
      std::string blanks(static_cast<int>(m_StarsCount - m_Process->GetProgress()*m_StarsCount),' ');
      std::cout << "\rProcessing progress:" << progressPercent << "% [" << stars << blanks << "]" << std::flush;
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
      << m_TimeProbe.GetMeanTime()
      << " seconds." << std::endl;
}

} // end namespace otb
