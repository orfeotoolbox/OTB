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

#ifndef _otbConfigurationManager_h
#define _otbConfigurationManager_h

#include "itkVersion.h"

#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
#include "itksys/FundamentalType.h"
#else
#include "itk_kwiml.h"
#endif

#include <string>
#include <boost/cstdint.hpp>
#include "itkLoggerBase.h"
#include "OTBCommonExport.h"

namespace otb
{
/**
 * \brief Retrieve configuration values from env var or default values
 *
 * This is a simple helper class to retrieve configuration values from
 * environment variables if they are set, or from default values if
 * not.
 *
 * Please refer to each static method documentation for available
 * configuration values and related environment variables.
 */
class OTBCommon_EXPORT ConfigurationManager
{
public:
#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
  typedef ::itksysFundamentalType_UInt64 RAMValueType;
#else
  typedef KWIML_INT_uint64_t RAMValueType;
#endif

  /**
   * DEMDirectory is a directory were DEM tiles are stored.
   *
   * If environment variable OTB_DEM_DIRECTORY is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  static std::string GetDEMDirectory();

  /**
   * GeoidFile is path to a geoid file.
   *
   * If environment variable OTB_GEOID_FILE is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  static std::string GetGeoidFile();

  /**
   * MaxRAMHint denotes the maximum memory OTB should use for
   * processing, expressed in MegaBytes.
   *
   * If environment variable OTB_MAX_RAM_HINT is defined and could be
   * converted to int, return its content as a 64 bits unsigned int.
   * Else, returns default value, which is 128 Mb
   *
   */
  static RAMValueType GetMaxRAMHint();

  /**
   * Logger level controls the level of logging that OTB will output.
   * 
   * This is used to set-up the otb::Logger class.
   *
   * If OTB_LOGGER_LEVEL environment variable is set to one of DEBUG,
   * INFO, WARNING, CRITICAL or FATAL, the logger level will be
   * set accordingly.
   *
   * Priority is DEBUG < INFO < WARNING < CRITICAL < FATAL.
   * 
   * Only messages with a higher priority than the logger level will
   * be displayed.
   *
   * By default (if OTB_LOGGER_LEVEL is not set or can not be
   * decoded), level is INFO.
   * 
   */
  static itk::LoggerBase::PriorityLevelType GetLoggerLevel();

  /**
   * If OpenMP is enabled, the number of threads for openMP is set to the
   * same number as in ITK (see GetGlobalDefaultNumberOfThreads()). This number
   * of threads is returned.
   * If OpenMP is disabled, this function does nothing
   */
  static int InitOpenMPThreads();

private:
  ConfigurationManager() = delete;
  ~ConfigurationManager() = delete;
  ConfigurationManager(const ConfigurationManager&) = delete;
  void operator =(const ConfigurationManager&) = delete;


};
}

#endif
