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

#include "otbLogger.h"
#include "itksys/SystemTools.hxx"
#include "otbConfigurationManager.h"
#include "itkStdStreamLogOutput.h"
#include <iostream>
#include "gdal.h"
#include "itkMultiThreader.h"

#include <type_traits>
#include <cassert>

namespace otb
{

namespace
{
  static bool is_logger_singleton_level_set;
}

Logger::Logger() :
  m_LogSetupInfoDone(false)
{
  m_LevelForFlushing = itk::LoggerBase::CRITICAL;
  m_TimeStampFormat = itk::LoggerBase::HUMANREADABLE;
  m_HumanReadableFormat = "%Y-%m-%d %H:%M:%S";
}

Logger * Logger::CreateInstance()
{
  Logger * logger = new Logger;

  // By default, redirect logs to std::cout
  itk::StdStreamLogOutput::Pointer defaultOutput = 
    itk::StdStreamLogOutput::New();
  defaultOutput->SetStream(std::cout);
  
  logger->AddLogOutput(defaultOutput);
  return logger;
}


Logger * Logger::Instance()
{
  static Logger * logger_singleton = CreateInstance();
  if ( !is_logger_singleton_level_set )
  {
    is_logger_singleton_level_set = true;
    logger_singleton->SetPriorityLevel(ConfigurationManager::GetLoggerLevel());
  }
  return logger_singleton;
}

Logger::Pointer Logger::New()
{
  Pointer smartPtr = new Logger;
  smartPtr->UnRegister();
  smartPtr->SetPriorityLevel(Instance()->GetPriorityLevel());
  return smartPtr;
}

void Logger::LogSetupInformation()
{
  if (! IsLogSetupInformationDone())
    {
    std::ostringstream oss;

    oss<<"Default RAM limit for OTB is "<<
      otb::ConfigurationManager::GetMaxRAMHint()<<" MB"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

    oss<<"GDAL maximum cache size is "<<
      GDALGetCacheMax64()/(1024*1024)<<" MB"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

    oss<<"OTB will use at most "<<
      itk::MultiThreader::GetGlobalDefaultNumberOfThreads()<<
      " threads"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

// ensure LogSetupInformation is done once per logger, and also that it is
// skipped by the singleton when it has already been printed by an other instance
    LogSetupInformationDone();
    Instance()->LogSetupInformationDone();
    }
}

bool Logger::IsLogSetupInformationDone()
{
  return m_LogSetupInfoDone;
}

void Logger::LogSetupInformationDone()
{
  m_LogSetupInfoDone = true;
}

std::string Logger::BuildFormattedEntry(itk::Logger::PriorityLevelType level, std::string const& content)
{
  static const std::string levelString[] = {"(MUSTFLUSH)", "(FATAL)", "(CRITICAL)", "(WARNING)", "(INFO)", "(DEBUG)", "(NOTSET)"};

  assert(level <= std::extent<decltype(levelString)>::value);

  return itksys::SystemTools::GetCurrentDateTime("%Y-%m-%d %H:%M:%S") + " " + levelString[level] + ": " + content;
}


} // namespace otb
