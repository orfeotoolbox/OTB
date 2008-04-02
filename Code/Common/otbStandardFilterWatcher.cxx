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
{
  // Initialize state
  m_Process = process;
  m_Comment = comment;
  m_StarsCount = 50;
  
  // Create a series of commands
  m_StartFilterCommand = CommandType::New();
  m_EndFilterCommand = CommandType::New();
  m_ProgressFilterCommand = CommandType::New();
  
  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this,
                                        &StandardFilterWatcher::StartFilter);
  m_EndFilterCommand->SetCallbackFunction(this,
                                        &StandardFilterWatcher::EndFilter);
  m_ProgressFilterCommand->SetCallbackFunction(this,
                                        &StandardFilterWatcher::ShowProgress);
  
  // Add the commands as observers
  m_StartTag = m_Process->AddObserver(itk::StartEvent(),
				      m_StartFilterCommand);
  
  m_EndTag = m_Process->AddObserver(itk::EndEvent(),
				    m_EndFilterCommand);
  
  m_ProgressTag = m_Process->AddObserver(itk::ProgressEvent(),
					 m_ProgressFilterCommand);
}

StandardFilterWatcher
::StandardFilterWatcher( const StandardFilterWatcher& watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
      if (m_StartFilterCommand)
	{
	  m_Process->RemoveObserver(m_StartTag);
	}
      if (m_EndFilterCommand)
	{
	  m_Process->RemoveObserver(m_EndTag);
	}
      if (m_ProgressFilterCommand)
	{
	  m_Process->RemoveObserver(m_ProgressTag);
	}
    }
  
  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_Comment = watch.m_Comment;
  m_StarsCount = watch.m_StarsCount;
  
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
						&StandardFilterWatcher::StartFilter);
      m_EndFilterCommand->SetCallbackFunction(this,
					      &StandardFilterWatcher::EndFilter);
      m_ProgressFilterCommand->SetCallbackFunction(this,
						   &StandardFilterWatcher::ShowProgress);
      
      // Add the commands as observers
      m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
      m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
      m_ProgressTag
	= m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

void
StandardFilterWatcher
::operator=(const StandardFilterWatcher &watch)
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
      if (m_StartFilterCommand)
	{
	  m_Process->RemoveObserver(m_StartTag);
	}
      if (m_EndFilterCommand)
	{
	  m_Process->RemoveObserver(m_EndTag);
	}
      if (m_ProgressFilterCommand)
	{
	  m_Process->RemoveObserver(m_ProgressTag);
	}
    }
  
  // Initialize state
  m_TimeProbe = watch.m_TimeProbe;
  m_Process = watch.m_Process;
  m_Comment = watch.m_Comment;
  m_StarsCount = watch.m_StarsCount;
  
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
						&StandardFilterWatcher::StartFilter);
      m_EndFilterCommand->SetCallbackFunction(this,
					      &StandardFilterWatcher::EndFilter);
      m_ProgressFilterCommand->SetCallbackFunction(this,
						   &StandardFilterWatcher::ShowProgress);
      
      // Add the commands as observers
      m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
      m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
      m_ProgressTag
	= m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);
    }
}

StandardFilterWatcher
::~StandardFilterWatcher()
{
  // Remove any observers we have on the old process object
  if (m_Process)
    {
      if (m_StartTag)
	{
	  m_Process->RemoveObserver(m_StartTag);
	}
      if (m_EndTag)
	{
	  m_Process->RemoveObserver(m_EndTag);
	}
      if (m_ProgressTag)
	{
	  m_Process->RemoveObserver(m_ProgressTag);
	}
    }
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
      std::cout << "\r" << progressPercent << "% [" << stars << blanks << "]" << std::flush;
    }
}

void
StandardFilterWatcher
::StartFilter()
{
  m_TimeProbe.Start();
  std::cout << (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None")
	    << " \"" << m_Comment << "\" " << std::endl;
  std::cout << "Prgress task : " << std::flush;
}

void
StandardFilterWatcher
::EndFilter()
{
  m_TimeProbe.Stop();
  std::cout << std::endl << "Filter took "
	    << m_TimeProbe.GetMeanTime()
	    << " seconds.";
}

} // end namespace otb
