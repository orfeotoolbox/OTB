/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbLogger.h"
#include "itksys/SystemTools.hxx"

namespace otb
{

Logger::Logger() :
    itk::Logger::Logger()
{
#if OTB_DEBUG
  this->SetPriorityLevel(itk::LoggerBase::DEBUG);
#else
  this->SetPriorityLevel(itk::LoggerBase::INFO);
#endif

  this->SetLevelForFlushing(itk::LoggerBase::CRITICAL);

  this->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  this->SetHumanReadableFormat("%Y-%m-%d %H:%M:%S");
}

Logger::~Logger()
{
}

std::string Logger::BuildFormattedEntry(itk::Logger::PriorityLevelType level, std::string const & content)
{
 static std::string m_LevelString[] = { "(MUSTFLUSH)", "(FATAL)", "(CRITICAL)",
                                         "(WARNING)", "(INFO)", "(DEBUG)", "(NOTSET)" };
  std::ostringstream s;

  switch ( this->m_TimeStampFormat )
  {
    case REALVALUE:
    {
      s.precision(30);
      s << m_Clock->GetTimeInSeconds();
      break;
    }
    case HUMANREADABLE:
    {
      s << itksys::SystemTools::GetCurrentDateTime( this->m_HumanReadableFormat.c_str() );
      break;
    }
  }
  s << " " << m_LevelString[level] << ": " << content;

  return s.str();
}

} // namespace otb
