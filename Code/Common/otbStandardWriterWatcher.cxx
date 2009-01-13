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
#include "otbStandardWriterWatcher.h"

namespace otb
{

StandardWriterWatcher
::StandardWriterWatcher(itk::ProcessObject* process,
      const char *comment)
  : WriterWatcherBase(process, comment)
{
  m_StarsCount = 50;
}

StandardWriterWatcher
::StandardWriterWatcher(itk::ProcessObject* process,itk::ProcessObject * source,
      const char *comment)
  : WriterWatcherBase(process,source,comment)
{
  m_StarsCount = 50;
}

StandardWriterWatcher
::StandardWriterWatcher( const StandardWriterWatcher& watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardWriterWatcher
::operator=(const StandardWriterWatcher &watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardWriterWatcher
::ShowWriterProgress()
{
  this->ShowProgress();
}

void
StandardWriterWatcher
::ShowFilterProgress()
{
  this->ShowProgress();
}

void
StandardWriterWatcher
::ShowProgress()
{
  itk::OStringStream oss;
  oss.str("");
  oss<<"\r";

  if (m_SourceProcess)
    {
      double progress = m_SourceProcess->GetProgress();
      int progressPercent = static_cast<int>(progress*100);
      std::string stars(static_cast<int>(progress*m_StarsCount),'*');
      std::string blanks(m_StarsCount - stars.length(),' ');
      oss << "Current Tile: ";
      if(progressPercent<10)
  {
    oss<<" ";
  }
      if(progressPercent<100)
  {
    oss<<" ";
  }
      oss<<progressPercent << "% [" << stars << blanks << "]  ";
    }

  if (m_Process)
    {
      double progress = m_Process->GetProgress();
      int progressPercent = static_cast<int>(progress*100);
      std::string stars(static_cast<int>(progress*m_StarsCount),'*');
      std::string blanks(m_StarsCount - stars.length(),' ');
      oss << "Writing: ";
      if(progressPercent<10)
  {
    oss<<" ";
  }
      if(progressPercent<100)
  {
    oss<<" ";
  }
      oss<< progressPercent << "% [" << stars << blanks << "]" << std::flush;
    }
  std::cout<<oss.str();
}



void
StandardWriterWatcher
::StartWriter()
{
  m_TimeProbe.Start();
  std::cout<<"Writing task: "<< " \"" << m_Comment << "\" " << std::endl;
  std::cout << "Writer type: "<<(m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")<< std::endl;
  std::cout << "Filter type: "<<(m_SourceProcess.GetPointer() ? m_SourceProcess->GetNameOfClass() : "None")<< std::endl;
}

void
StandardWriterWatcher
::EndWriter()
{
  m_TimeProbe.Stop();
  std::cout << std::endl << "Writing task took "
      << m_TimeProbe.GetMeanTime()
      << " seconds." << std::endl;
}

} // end namespace otb
