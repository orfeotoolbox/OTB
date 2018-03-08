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

#include "otbConfigurationManager.h"

#include "itksys/SystemTools.hxx"

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
  std::string svalue;
  
  RAMValueType value = 128;
  
  if(itksys::SystemTools::GetEnv("OTB_MAX_RAM_HINT",svalue))
    {

    unsigned long int tmp = strtoul(svalue.c_str(),ITK_NULLPTR,10);
    
    if(tmp)
      {
      value = static_cast<RAMValueType>(tmp);
      }
    }
  
  return value;
}


#define level_macro(x) \
  {                                                                     \
    std::string target = #x;                                            \
    if(std::equal(svalue.begin(),svalue.end(),target.begin(),target.end())) \
      level = itk::LoggerBase::x;                                       \
  }                                                                     \


itk::LoggerBase::PriorityLevelType ConfigurationManager::GetLoggerLevel()
{
  std::string svalue;

  #ifndef NDEBUG
  itk::LoggerBase::PriorityLevelType level = itk::LoggerBase::DEBUG;
  #else
  itk::LoggerBase::PriorityLevelType level = itk::LoggerBase::INFO;
  #endif

  
  if(itksys::SystemTools::GetEnv("OTB_LOGGER_LEVEL",svalue))
    {
    level_macro(DEBUG);
    level_macro(WARNING);
    level_macro(INFO);
    level_macro(CRITICAL);
    level_macro(FATAL);
    }
  return level;
}

}
