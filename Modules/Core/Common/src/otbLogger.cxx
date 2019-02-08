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

namespace otb
{

Logger::Pointer Logger::CreateInstance()
{
  Logger::Pointer instance = Logger::New();

  // By default, redirect logs to std::cout
  itk::StdStreamLogOutput::Pointer defaultOutput = itk::StdStreamLogOutput::New();
  defaultOutput->SetStream(std::cout);
  
  instance->AddLogOutput(defaultOutput);

  return instance;
}

Logger::Pointer Logger::Instance()
{
  // Static locales are initialized once in a thread-safe way
  static Logger::Pointer instance = CreateInstance();

  return instance;
}

Logger::Logger()
{
  this->SetPriorityLevel(itk::LoggerBase::INFO);

  this->SetLevelForFlushing(itk::LoggerBase::CRITICAL);

  this->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  this->SetHumanReadableFormat("%Y-%m-%d %H:%M:%S");

  m_LogSetupInfoDone = false;
}

Logger::~Logger()
{
}

void Logger::LogSetupInformation()
{
  if (! IsLogSetupInformationDone())
    {
    std::ostringstream oss;

    oss<<"Default RAM limit for OTB is "<<otb::ConfigurationManager::GetMaxRAMHint()<<" MB"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

    oss<<"GDAL maximum cache size is "<<GDALGetCacheMax64()/(1024*1024)<<" MB"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

    oss<<"OTB will use at most "<<itk::MultiThreader::GetGlobalDefaultNumberOfThreads()<<" threads"<<std::endl;
    this->Info(oss.str());
    oss.str("");
    oss.clear();

    // ensure LogSetupInformation is done once per logger, and also that it is
    // skipped by the singleton when it has already been printed by an other instance
    LogSetupInformationDone();
    Instance()->LogSetupInformationDone();
    }
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

bool Logger::IsLogSetupInformationDone()
{
  return m_LogSetupInfoDone;
}

void Logger::LogSetupInformationDone()
{
  m_LogSetupInfoDone = true;
}

} // namespace otb
