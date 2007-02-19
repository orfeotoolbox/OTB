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
#ifndef __otbFltkFilterWatcher_h
#define __otbFltkFilterWatcher_h

#include "itkCommand.h"
#include "itkProcessObject.h"
#include "itkTimeProbe.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Progress.H>


namespace otb
{

/** \class FltkFilterWatcher

 */
class ITK_EXPORT FltkFilterWatcher
{
public:
  /** Classes that need access to filter's private data */
  friend class XMLFilterWatcher;

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  FltkFilterWatcher(itk::ProcessObject* o, int x, int y, int w,int h,const char *comment="");

  /** Destructor. */
  virtual ~FltkFilterWatcher();
  
protected:

  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress()
  {
    if (m_Process)
      {
	m_Progress->value(m_Process->GetProgress());
	Fl::check();
      }
  }

  /** Callback method to show the StartEvent */
  virtual void StartFilter()
  {
    m_Window->show();
    m_Progress->show();
  }

  /** Callback method to show the EndEvent */
  virtual void EndFilter()
  {
    m_Window->hide();
  }

private:
  itk::TimeProbe m_TimeProbe;
  std::string m_Comment;
  itk::ProcessObject::Pointer m_Process;

  typedef itk::SimpleMemberCommand<FltkFilterWatcher> CommandType;
  CommandType::Pointer m_StartFilterCommand;
  CommandType::Pointer m_EndFilterCommand;
  CommandType::Pointer m_ProgressFilterCommand;

  unsigned long m_StartTag;
  unsigned long m_EndTag;
  unsigned long m_ProgressTag;

  Fl_Window * m_Window;
  Fl_Progress * m_Progress;
};

} // end namespace otb

#endif
