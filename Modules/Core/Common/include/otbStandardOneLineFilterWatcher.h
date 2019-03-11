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

#ifndef otbStandardOneLineFilterWatcher_h
#define otbStandardOneLineFilterWatcher_h

#include <iosfwd>

#include "otbFilterWatcherBase.h"
#include "otbStandardOutputPrintCallback.h"

namespace otb
{

/** \class StandardOneLineFilterWatcher
 *  \brief This class shows the percentage progress execution
 *         of the pipeline filtering process
 *
 *  This class is based on Observer design patter
 *  Abstract class ProcessObject is the subject
 *  Event are observers
 *
 *  Usage example:
 *
 *  \code
 *  typedef itk::BinaryThresholdImageFilter<ImageType> FilterType;
 *  FilterType::Pointer thresholdFilter = FilterType::New();
 *
 *  StandardOneLineFilterWatcher watcher(thresholdFilter, "Threshold");
 *  \endcode
 *
 *  \see otb::StandardOutputPrintCallback
 *  \see itk::SimpleFilterWatcher
 *  \see otb::fltkFilterWatcher
 *
 * \ingroup OTBCommon
 */
template <class PrintCallbackType = StandardOutputPrintCallback>
class OTBCommon_EXPORT StandardOneLineFilterWatcher : public FilterWatcherBase
{
public:
  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardOneLineFilterWatcher(itk::ProcessObject* process,
                        const char *comment = "");

  StandardOneLineFilterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");

  /** Default constructor */
  StandardOneLineFilterWatcher();

  /** Destrucotr */
  virtual ~StandardOneLineFilterWatcher() =default;

  /** Get/Set number of stars */
  void SetStars(int count)
  {
    m_StarsCount = count;
  }
  const int& GetStars() const
  {
    return m_StarsCount;
  }

  /** Set the callback class */
  void SetCallback(PrintCallbackType * callback)
  {
    m_Callback = callback;
  }

protected:

  /** Callback method to show the ProgressEvent */
  void ShowProgress() override;

  /** Callback method to show the StartEvent */
  void StartFilter() override;

  /** Callback method to show the EndEvent */
  void EndFilter() override;

private:

  /** Stars counting */
  int m_StarsCount;

  /** Current number of stars, we keep track of this to avoid reprinting the
   * progress if it hasn't changed */
  int m_CurrentNbStars;

  bool m_CoutIsConsole;

  /** If the output is not interactive (e.g. it is redirected to a file), it
   * is buffered and only written at the end of the processing */
  std::string m_Buffer;
  
  /** The point to the callback used for printing. It is set to the default
   * callback on construction and can be changed later using the setter.
   * Delete will not be called on this pointer. */
  PrintCallbackType * m_Callback;
  
  /** A default callback created in the constructor and deleted in the 
   * destructor. */ 
  std::shared_ptr<PrintCallbackType> m_DefaultCallback;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardOneLineFilterWatcher.hxx"
#endif

#endif
