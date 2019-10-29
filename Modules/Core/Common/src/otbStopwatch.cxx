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

#include <chrono>
#include <sstream>
#include <iomanip>

#include "otbStopwatch.h"

namespace otb
{

Stopwatch::Stopwatch() : m_StartTime(), m_ElapsedMilliseconds(), m_IsRunning()
{
}

void Stopwatch::Start()
{
  if (!this->m_IsRunning)
  {
    this->m_IsRunning = true;
    this->m_StartTime = this->GetTimestamp();
  }
}

void Stopwatch::Stop()
{
  if (this->m_IsRunning)
  {
    this->m_ElapsedMilliseconds += GetRunningElapsedTime();
    this->m_IsRunning = false;
  }
}

void Stopwatch::Reset()
{
  this->m_ElapsedMilliseconds = 0;
  this->m_IsRunning           = false;
}

void Stopwatch::Restart()
{
  this->m_ElapsedMilliseconds = 0;
  this->m_IsRunning           = true;
  this->m_StartTime           = this->GetTimestamp();
}

Stopwatch::DurationType Stopwatch::GetElapsedMilliseconds() const
{
  auto result = this->m_ElapsedMilliseconds;

  if (this->m_IsRunning)
    result += this->GetRunningElapsedTime();

  return result;
}

void Stopwatch::GetElapsedHumanReadableTime(std::ostream& oss) const
{
  auto         result  = this->GetElapsedMilliseconds();
  DurationType seconds = result / 1000;
  DurationType hours   = seconds / 3600;
  seconds -= hours * 3600;
  DurationType minutes = seconds / 60;
  seconds -= minutes * 60;
  std::ostringstream os;
  if (hours > 0)
    oss << hours << "h " << std::setfill('0') << std::setw(2);
  if (minutes > 0 || hours > 0)
    oss << minutes << "m " << std::setfill('0') << std::setw(2);
  oss << seconds << "s";
}

std::string Stopwatch::GetElapsedHumanReadableTime() const
{
  std::ostringstream oss;
  GetElapsedHumanReadableTime(oss);
  return oss.str();
}

bool Stopwatch::IsRunning() const
{
  return this->m_IsRunning;
}

Stopwatch Stopwatch::StartNew()
{
  Stopwatch sw;
  sw.Start();

  return sw;
}

inline Stopwatch::TimepointType Stopwatch::GetTimestamp() const
{
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

inline Stopwatch::DurationType Stopwatch::GetRunningElapsedTime() const
{
  return this->GetTimestamp() - this->m_StartTime;
}
}
