/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _otbConfigurationManager_h
#define _otbConfigurationManager_h

#include <string>
#include <cstdint>
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
 * Please refer to each function documentation for available
 * configuration values and related environment variables.
 */
namespace OTBCommon_EXPORT ConfigurationManager
{
  using RAMValueType = std::uint64_t ;

  /**
   * DEMDirectory is a directory were DEM tiles are stored.
   *
   * If environment variable OTB_DEM_DIRECTORY is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  std::string GetDEMDirectory();

  /**
   * GeoidFile is path to a geoid file.
   *
   * If environment variable OTB_GEOID_FILE is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  std::string GetGeoidFile();

  /**
   * MaxRAMHint denotes the maximum memory OTB should use for
   * processing, expressed in MegaBytes.
   *
   * If environment variable OTB_MAX_RAM_HINT is defined and could be
   * converted to int, return its content as a 64 bits unsigned int.
   * Else, returns default value, which is 128 Mb
   *
   */
  RAMValueType GetMaxRAMHint();

  /**
   * Returns the maximum number of rows read at once from images.
   *  In some scenarios where multi-bands input images need to be reorganized before they could be
   * processed, OTB may momentarily double the memory it uses, and go twice over
   * :envvar:`OTB_MAX_RAM_HINT` value.
   * For those cases, this option controls the maximum number of lines directly loaded in memory
   * before they are reorganized for processing.
   *
   * This value will be used in `otb::ImageFileReader` to stream the reading of image files.
   * A value of 0 tells to read as many lines as there are, even if there isn't enough RAM available
   * to accommodate the temporary buffer used.
   *
   * By default, this value is assumed to be 0.
   *
   * \return `$OTB_MAX_NUMBER_OF_ROWS_READ_AT_ONCE` or 0 if unset
   * \throw std::invalid_argument if the variable cannot be decode as an int.
   * \throw std::out_of_range if the decoded int doesn't fit in an `unsigned int`.
   */
  unsigned long GetMaxImageRowsReadAtOnce();

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
  itk::LoggerBaseEnums::PriorityLevel GetLoggerLevel();

  /**
   * If OpenMP is enabled, the number of threads for openMP is set to the
   * same number as in ITK (see GetGlobalDefaultNumberOfThreads()). This number
   * of threads is returned.
   * If OpenMP is disabled, this function does nothing
   */
  int InitOpenMPThreads();

};
}

#endif
