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

#ifndef otbStandardFilterWatcher_h
#define otbStandardFilterWatcher_h

#include "otbFilterWatcherBase.h"

namespace otb
{

/** \class StandardFilterWatcher
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
 *  StandardFilterWatcher watcher(thresholdFilter, "Threshold");
 *  \endcode
 *
 *  \see itk::SimpleFilterWatcher
 *  \see otb::fltkFilterWatcher
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT StandardFilterWatcher : public FilterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardFilterWatcher(itk::ProcessObject* process,
                        const char *comment = "");

  StandardFilterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");

  /** Default constructor */
  StandardFilterWatcher() : m_StarsCount(0) {};

  /** Copy constructor */
  StandardFilterWatcher(const StandardFilterWatcher&);

  /** operator=  */
  void operator =(const StandardFilterWatcher&);

  /** Get/Set number of stars */
  void SetStars(int count)
  {
    m_StarsCount = count;
  }
  const int& GetStars() const
  {
    return m_StarsCount;
  }

protected:

  /** Callback method to show the ProgressEvent */
  void ShowProgress() override;

  /** Callback method to show the StartEvent */
  void StartFilter() override;

  /** Callback method to show the EndEvent */
  void EndFilter() override;

private:

  /** Stars coutning */
  int m_StarsCount;

  int m_CurrentNbStars;

  bool m_CoutIsConsole;

  std::string m_Buffer;
};

} // end namespace otb

#endif
