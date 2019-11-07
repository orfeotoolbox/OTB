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

#ifndef otbFilterWatcherBase_h
#define otbFilterWatcherBase_h

#include <string>

#include "otbStopwatch.h"
#include "itkCommand.h"
#include "itkProcessObject.h"

#include "OTBCommonExport.h"

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
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT FilterWatcherBase
{
public:
  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  FilterWatcherBase(itk::ProcessObject* process, const char* comment = "");

  /** Default constructor */
  FilterWatcherBase();

  /** Copy constructor */
  FilterWatcherBase(const FilterWatcherBase&);

  /** operator=  */
  void operator=(const FilterWatcherBase&);

  /** Destructor. */
  virtual ~FilterWatcherBase();

  const char* GetNameOfClass()
  {
    return (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None");
  }

  /** Methods to access member data */
  /** Get a pointer to the process object being watched. */
  itk::ProcessObject* GetProcess()
  {
    return m_Process.GetPointer();
  }

  /** Get the comment for the watcher. */
  std::string GetComment()
  {
    return m_Comment;
  }

  /** Get a reference to the Stopwatch */
  otb::Stopwatch& GetStopwatch()
  {
    return m_Stopwatch;
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
  otb::Stopwatch m_Stopwatch;

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
