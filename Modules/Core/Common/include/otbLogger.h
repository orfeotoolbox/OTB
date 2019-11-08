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

#ifndef otbLogger_h
#define otbLogger_h

#include <string>

#include "itkLoggerBase.h"
#include "itkLogger.h"
#include "OTBCommonExport.h"

namespace otb
{

/** \class Logger
 *  \brief An ITK logger specialized for OTB
 *
 *  Sets OTB wide settings in its constructor
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT Logger : public itk::Logger
{
public:
  typedef Logger                        Self;
  typedef itk::Logger                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkTypeMacro(Logger, itk::Logger);

  /**
   * If Logger crashes when called from the destructor of a static
   * or global object, the singleton might have already been destroyed.
   * You can prolong its lifetime by calling Logger::Instance()
   * from that object's constructor.
   *
   * See https://stackoverflow.com/questions/335369/finding-c-static-initialization-order-problems#335746
   */
  static Logger* Instance();

  void ResetOutputs();

  static Pointer New();
  itkCreateAnotherMacro(Logger) itkCloneMacro(Logger)

      /** Output logs about the RAM, caching and multi-threading settings */
      void LogSetupInformation();

  /** Return true if the LogSetupInformation has already been called*/
  bool IsLogSetupInformationDone();

  /** Set the flag m_LogSetupInfoDone to true */
  void LogSetupInformationDone();

  // Overwrite this to provide custom formatting of log entries
  std::string BuildFormattedEntry(itk::Logger::PriorityLevelType, std::string const&) override;

protected:
  Logger();
  virtual ~Logger() override = default;

private:
  Logger(const Self&) = delete;
  void operator=(const Self&) = delete;

  static Logger* CreateInstance();


  bool m_LogSetupInfoDone;

}; // class Logger

} // namespace otb

#endif // otbLogger_h
