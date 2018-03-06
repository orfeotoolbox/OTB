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
#include "otbConfigurationManager.h"
#include "itkStdStreamLogOutput.h"
#include <iostream>

namespace otb
{

Logger::Pointer Logger::Singleton = ITK_NULLPTR;

Logger::Pointer Logger::Instance()
{
  if(!Logger::Singleton)
    {
    Logger::Singleton = Logger::New();

    // By default, redirect logs to std::cout
    itk::StdStreamLogOutput::Pointer defaultOutput = itk::StdStreamLogOutput::New();
    defaultOutput->SetStream(std::cout);

    Logger::Singleton->AddLogOutput(defaultOutput);
    }
  
  return Logger::Singleton;
}


Logger::Logger()
{
  this->SetPriorityLevel(otb::ConfigurationManager::GetLoggerLevel());

  this->SetLevelForFlushing(itk::LoggerBase::CRITICAL);

  this->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  this->SetHumanReadableFormat("%Y-%m-%d %H:%M:%S");
}

Logger::~Logger()
{
}

std::string Logger::BuildFormattedEntry(itk::Logger::PriorityLevelType level, std::string const & content)
{
 static const std::string levelString[] = { "(MUSTFLUSH)", "(FATAL)", "(CRITICAL)",
                                         "(WARNING)", "(INFO)", "(DEBUG)", "(NOTSET)" };

  // TODO: assert(level <= std::extent<decltype(levelString)>::value);  // requires C++11

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
  s << " " << levelString[level] << ": " << content;

  return s.str();
}

} // namespace otb
