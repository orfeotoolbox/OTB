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
::StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const char *comment)
  : WriterWatcherBase(process, source, comment)
{
  m_StarsCount = 50;
}

StandardWriterWatcher
::StandardWriterWatcher(itk::ProcessObject* process,
                        const std::string& comment)
  : WriterWatcherBase(process, comment.c_str())
{
  m_StarsCount = 50;
}

StandardWriterWatcher
::StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const std::string& comment)
  : WriterWatcherBase(process, source, comment.c_str())
{
  m_StarsCount = 50;
}

StandardWriterWatcher
::StandardWriterWatcher(const StandardWriterWatcher& watch)  : WriterWatcherBase(watch)
{
  // Initialize state
  m_StarsCount = watch.m_StarsCount;
}

void
StandardWriterWatcher
::operator =(const StandardWriterWatcher& watch)
{
  // Initialize state
  WriterWatcherBase::operator=(watch);
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
  std::ostringstream oss;
  oss.str("");
  oss << "\r";

  if (m_SourceProcess)
    {
    double      progress = m_SourceProcess->GetProgress();
    int         progressPercent = static_cast<int>(progress * 100);
    unsigned int nbStars = static_cast<int>(progress * m_StarsCount);
    if(nbStars>m_StarsCount)
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
    double      progress = m_Process->GetProgress();
    int         progressPercent = static_cast<int>(progress * 100);
    unsigned int nbStars = static_cast<int>(progress * m_StarsCount);
    if(nbStars>m_StarsCount)
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
    oss << progressPercent << "% [" << stars << blanks << "]" << std::flush;
    }
  std::cout << oss.str();
}

void
StandardWriterWatcher
::StartWriter()
{
  m_TimeProbe.Start();
  std::cout << "Writing task: " << " \"" << m_Comment << "\" " << std::endl;
  std::cout << "Writer type: " << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None") << std::endl;
  std::cout << "Filter type: " << (m_SourceProcess.GetPointer() ? m_SourceProcess->GetNameOfClass() : "None") <<
  std::endl;
}

void
StandardWriterWatcher
::EndWriter()
{
  m_TimeProbe.Stop();
  std::cout << std::endl << "Writing task took "
            << m_TimeProbe.GetMean()
            << " seconds." << std::endl;
}

} // end namespace otb
