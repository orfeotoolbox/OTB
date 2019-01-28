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

#include "otbConfigurationManager.h"

#include "otbMacro.h"
#include "otbLogger.h"

#include "itkMultiThreader.h"
#include "itksys/SystemTools.hxx"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <cstdlib>
#include <algorithm>
#include <string>

namespace otb
{

std::string ConfigurationManager::GetDEMDirectory()
{
  std::string svalue;
  itksys::SystemTools::GetEnv("OTB_DEM_DIRECTORY",svalue);
  return svalue;
}

std::string ConfigurationManager::GetGeoidFile()
{
  std::string svalue;
  itksys::SystemTools::GetEnv("OTB_GEOID_FILE",svalue);
  return svalue;
}

ConfigurationManager::RAMValueType ConfigurationManager::GetMaxRAMHint()
{
  std::string max_ram_hint;
  if (itksys::SystemTools::GetEnv("OTB_MAX_RAM_HINT", max_ram_hint))
  {
    return std::stoul(max_ram_hint);
  }
  else
  {
    // Default value
    return 256;
  }
}

itk::LoggerBase::PriorityLevelType ConfigurationManager::GetLoggerLevel()
{
  std::string svalue;

  // Default value is INFO
  itk::LoggerBase::PriorityLevelType level = itk::LoggerBase::INFO;
  
  if(itksys::SystemTools::GetEnv("OTB_LOGGER_LEVEL",svalue))
    {
    if(svalue.compare("DEBUG") == 0)
      {
      level = itk::LoggerBase::DEBUG;
      }
    else if(svalue.compare("INFO") == 0)
      {
      level = itk::LoggerBase::INFO;
      }
    else if(svalue.compare("WARNING") == 0)
      {
      level = itk::LoggerBase::WARNING;
      }
    else if(svalue.compare("CRITICAL") == 0)
      {
      level = itk::LoggerBase::CRITICAL;
      }
    else
      {
      otbLogMacro(Error,<<"Unknown value for OTB_LOGGER_LEVEL_MACRO. Possible values are DEBUG, INFO, WARNING, CRITICAL.");
      }
    }
  return level;
}

int ConfigurationManager::InitOpenMPThreads()
{
  int ret = 1;
#ifdef _OPENMP
  ret = itk::MultiThreader::GetGlobalDefaultNumberOfThreads();
  omp_set_num_threads(ret);
#endif
  return ret;
}

}
