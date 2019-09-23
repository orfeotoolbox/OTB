/*
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

#ifndef otbProgressReporterManager_h
#define otbProgressReporterManager_h

#include "itkCommand.h"
#include "otbWrapperApplication.h"

#include "otbSwigPrintCallback.h"
#include "otbStandardOneLineFilterWatcher.h"

#include <memory>

namespace otb
{

/** \class ProgressReporterManager
 *  \brief Class to manage progress reporting with callback printing.
 *
 *  This class stores the command used to create CallbackProgressReporter
 *  using an Observer. The created CallbackProgressReporter are also stored
 *  in this class.
 */
class ProgressReporterManager : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ProgressReporterManager       Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkTypeMacro(ProgressReporterManager, itk::Object);

  itkNewMacro(ProgressReporterManager);

  typedef SwigPrintCallback                          CallbackType;
  typedef StandardOneLineFilterWatcher<CallbackType> WatcherType;

  /** Filter watcher list type */
  typedef std::vector<std::unique_ptr<WatcherType>> WatcherListType;

  /** Command Member */
  typedef itk::MemberCommand<Self> AddProcessCommandType;

  /** Set the logger callback function */
  void SetLogOutputCallback(CallbackType* callback)
  {
    m_Callback = callback;
    this->Modified();
  }

  /** Getter to AddProcessCommand */
  AddProcessCommandType* GetAddProcessCommand()
  {
    return m_AddProcessCommand.GetPointer();
  }

  /** Delete all watchers */
  void DeleteWatcherList();

protected:
  /** Default constructor */
  ProgressReporterManager();

  /** Destructor. */
  ~ProgressReporterManager() override;

  /** Load the watchers for internal progress and writing progress report. */
  void LinkWatchers(itk::Object* caller, const itk::EventObject& event);

private:
  /** The LogOutputCallback used for printing */
  CallbackType* m_Callback;

  /** Command associated to the LinkWatchers command */
  AddProcessCommandType::Pointer m_AddProcessCommand;

  /** container storing all the watchers (CallbackProgressReporter) */
  WatcherListType m_WatcherList;
};
}

#endif // otbProgressReporterManager_h
