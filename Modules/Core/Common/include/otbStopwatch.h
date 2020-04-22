/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStopwatch_h
#define otbStopwatch_h

#include <cstdint>
#include <string>
#include <iosfwd>

#include "OTBCommonExport.h"

namespace otb
{

/** \class Stopwatch
 * \brief Stopwatch timer.
 *
 * A simple class for measuring elapsed time.
 *
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT Stopwatch final
{
public:
  /** Standard class typedefs. */
  typedef Stopwatch Self;

  /** Represents a duration measured in milliseconds */
  typedef uint64_t DurationType;

  /** Constructs a timer instance */
  Stopwatch();

  /** Start the timer if not already running */
  void Start();

  /** Stop the timer if running */
  void Stop();

  /** Reset the timer */
  void Reset();

  /** Reset and restart the timer */
  void Restart();

  /** Get the total duration, excluding the current iteration */
  DurationType GetElapsedMilliseconds() const;

  /** Get the total duration in human readable format (ostream
    * version) */
  void GetElapsedHumanReadableTime(std::ostream& oss) const;

  /** Get the total duration in human readable format */
  std::string GetElapsedHumanReadableTime() const;

  /** Returns whether the stopwatch is running */
  bool IsRunning() const;

  /** Creates and starts a new stopwatch instance */
  static Stopwatch StartNew();

private:
  typedef uint64_t TimepointType;

  TimepointType GetTimestamp() const;
  DurationType  GetRunningElapsedTime() const;

  TimepointType m_StartTime;
  DurationType  m_ElapsedMilliseconds;
  bool          m_IsRunning;
};

} // namespace otb

#endif
