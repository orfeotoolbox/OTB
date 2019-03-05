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

#ifndef otbCallbackProgressReporter_h
#define otbCallbackProgressReporter_h

#include "otbFilterWatcherBase.h"
#include "otbLogOutputCallback.h"

namespace otb
{

class CallbackProgressReporter : public FilterWatcherBase
{
public:
  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  CallbackProgressReporter(itk::ProcessObject* process,
                        const char *comment = "");

  CallbackProgressReporter(itk::ProcessObject* process,
                        const std::string& comment = "");

  /** Default constructor */
  CallbackProgressReporter();

  /** Destructor. */
  virtual ~CallbackProgressReporter() = default;

  typedef LogOutputCallback CallbackType;
  
  /** Set the callback method */
  void SetCallback(CallbackType * callback)
    {
    m_Callback = callback;
    }
  
protected:
  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress() override;

  /** Callback method to show the StartEvent */
  virtual void StartFilter() override;

  /** Callback method to show the EndEvent */
  virtual void EndFilter() override;

private:

  /** Total number of stars in the progress bar */
  int m_StarsCount;

  /** Current number of stars in the progress bar */
  int m_CurrentNbStars;
  
  CallbackType * m_Callback; 
  
  
  /** buffer used when sys.stdout.isatty() == false */
  std::string m_Buffer;
};
}

#endif //otbCallbackProgressReporter_h
