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
#ifndef __otbFilterWatcherBase_h
#define __otbFilterWatcherBase_h

#include "itkCommand.h"
#include "itkProcessObject.h"
#include "itkTimeProbe.h"

namespace otb
{

/** \class FilterWatcherBase
 *  \brief This class is an abstract class
 *         Provides an interface to progress task mechanic
 *
 *  Implement your callbacks
 *  \li ShowProgress : callback called for each completed pixel
 *  \li StartFilter  : callback called at the begin of process
 *  \li EndFilter    : callback called at the end of process
 *
 */
class /*ITK_EXPORT*/ FilterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  FilterWatcherBase(itk::ProcessObject* process,
                    const char *comment = "");

  /** Default constructor */
  FilterWatcherBase();

  /** Copy constructor */
  FilterWatcherBase(const FilterWatcherBase&);

  /** operator=  */
  void operator =(const FilterWatcherBase&);

  /** Destructor. */
  virtual ~FilterWatcherBase();

  const char *GetNameOfClass()
    {
    return (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None");
    }

  /** Methods to access member data */
  /** Get a pointer to the process object being watched. */
  itk::ProcessObject *GetProcess()
  {
    return m_Process.GetPointer();
  }

  /** Get the comment for the watcher. */
  std::string GetComment()
  {
    return m_Comment;
  }

  /** Get a reference to the TimeProbe */
  itk::TimeProbe& GetTimeProbe()
  {
    return m_TimeProbe;
  }

protected:

  /** Callback method to show the ProgressEvent */
  virtual void ShowProgressCallback()
  {
    this->ShowProgress();
  }

  /** Callback method to show the StartEvent */
  virtual void StartFilterCallback()
  {
    if (!m_Started)
      {
      this->StartFilter();
      m_Started = true;
      }
  }

  /** Callback method to show the EndEvent */
  virtual void EndFilterCallback()
  {
    if (!m_Ended)
      {
      this->EndFilter();
      m_Ended = true;
      }
  }

  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress() = 0;

  /** Callback method to show the StartEvent */
  virtual void StartFilter() = 0;

  /** Callback method to show the EndEvent */
  virtual void EndFilter() = 0;

  /** Computing time */
  itk::TimeProbe m_TimeProbe;

  /** Associated comment */
  std::string m_Comment;

  /** Abstract process object */
  itk::ProcessObject::Pointer m_Process;

  /** Internal type */
  typedef itk::SimpleMemberCommand<FilterWatcherBase> CommandType;

  /** Start filter callback */
  CommandType::Pointer m_StartFilterCommand;

  /** End filter callback */
  CommandType::Pointer m_EndFilterCommand;

  /** Progress filter callback */
  CommandType::Pointer m_ProgressFilterCommand;

  /** Start oberserver */
  unsigned long m_StartTag;

  /** End observer */
  unsigned long m_EndTag;

  /** Progress observer */
  unsigned long m_ProgressTag;

  /** Some filters (for ex. persistents) invoke the StartEvent several times
   *  Work around this by remembering if it was called or not */
  bool m_Started;

  /** Some filters (for ex. persistents) invoke the EndEvent several times
   *  Work around this by remembering if it was called or not */
  bool m_Ended;

private:

};

} // end namespace otb

#endif
