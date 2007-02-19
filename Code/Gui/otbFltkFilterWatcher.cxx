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
#include "otbFltkFilterWatcher.h"

namespace otb
{

FltkFilterWatcher
::FltkFilterWatcher(itk::ProcessObject* o,int x, int y, int w, int h,const char *comment)
{
  // Initialize state
  m_Process = o;
  m_Comment = comment;
 
  // Create a series of commands
  m_StartFilterCommand =      CommandType::New();
  m_EndFilterCommand =        CommandType::New();
  m_ProgressFilterCommand =   CommandType::New();

  // Assign the callbacks
  m_StartFilterCommand->SetCallbackFunction(this,
                                        &FltkFilterWatcher::StartFilter);
  m_EndFilterCommand->SetCallbackFunction(this,
                                        &FltkFilterWatcher::EndFilter);
  m_ProgressFilterCommand->SetCallbackFunction(this,
                                        &FltkFilterWatcher::ShowProgress);

  // Add the commands as observers
  m_StartTag = m_Process->AddObserver(itk::StartEvent(), m_StartFilterCommand);
  m_EndTag = m_Process->AddObserver(itk::EndEvent(), m_EndFilterCommand);
  m_ProgressTag
    = m_Process->AddObserver(itk::ProgressEvent(), m_ProgressFilterCommand);

  m_Window = new Fl_Window(x,y,w+10,h+10);
  m_Window->label(m_Comment.c_str());
  m_Window->begin();
  m_Progress = new Fl_Progress(5,5,w,h);
  m_Progress->selection_color(FL_DARK_BLUE);
  m_Progress->minimum(0);
  m_Progress->maximum(1);
  m_Window->end();
  
}

FltkFilterWatcher
::~FltkFilterWatcher()
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
  delete m_Progress;
  delete m_Window;
}

} // end namespace otb

