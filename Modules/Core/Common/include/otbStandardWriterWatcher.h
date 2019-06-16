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

#ifndef otbStandardWriterWatcher_h
#define otbStandardWriterWatcher_h

#include <iosfwd>

#include "otbWriterWatcherBase.h"

#include "OTBCommonExport.h"

namespace otb
{

/** \class StandardWriterWatcher
 *  \brief This class shows the percentage progress execution
 *         of the pipeline filtering process
 *
 *  This class is based on Observer design pattern
 *  Abstract class ProcessObject is the subject
 *  Event are observers
 *
 *  Usage example:
 *
 *  \code
 *  typedef itk::BinaryThresholdImageWriter<ImageType> WriterType;
 *  WriterType::Pointer thresholdWriter = WriterType::New();
 *
 *  StandardWriterWatcher watcher(thresholdWriter, "Threshold");
 *  \endcode
 *
 *  \see itk::SimpleWriterWatcher
 *  \see otb::fltkWriterWatcher
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT StandardWriterWatcher : public WriterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardWriterWatcher(itk::ProcessObject* process,
                        const char *comment = "");
  StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const char *comment = "");

  StandardWriterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");
  StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const std::string& comment = "");

  /** Default constructor */
  StandardWriterWatcher() : m_StarsCount(0) {};

  /** Copy constructor */
  StandardWriterWatcher(const StandardWriterWatcher&);

  /** operator=  */
  void operator =(const StandardWriterWatcher&);

  /** Get/Set number of stars */
  void SetStars(int count)
  {
    m_StarsCount = count;
  }
  const unsigned int& GetStars() const
  {
    return m_StarsCount;
  }

protected:

  /** Callback method to show the ProgressEvent */
  void ShowWriterProgress() override;

  /** Callback method to show the StartEvent */
  void StartWriter() override;

  /** Callback method to show the EndEvent */
  void EndWriter() override;

  /** Callback method to show the ProgressEvent */
  void ShowFilterProgress() override;

  /** Callback method to show the StartEvent */
  void StartFilter() override {}

  /** Callback method to show the EndEvent */
  void EndFilter() override {}

  /** This is the method invoked by ShowFilterProgress() and ShowWriterProgress() */
  virtual void ShowProgress();

private:

  /** Stars coutning */
  unsigned int m_StarsCount;

  bool m_CoutIsConsole;

  std::string m_Buffer;
};

} // end namespace otb

#endif
