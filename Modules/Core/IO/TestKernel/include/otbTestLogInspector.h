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

#ifndef otbTestLogInspector_h
#define otbTestLogInspector_h

#include "itkStdStreamLogOutput.h"
#include "otbNewMacro.h"
#include "otbLogger.h"
#include <sstream>
#include <string>
#include <utility>

namespace otb
{

/**
 * Internal type to send logs to memory in a `std::ostringstream`.
 * \warning Once registered to an `itk::Logger`, it cannot be removed.
 */
class LogInspector : public itk::StdStreamLogOutput
{
public:
  using Self         = LogInspector;
  using Superclass   = Object;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<Self const>;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LogInspector, Object);
  otbNewMacro(LogInspector);

  /** Reset the logs currently acquired. */
  void Clear() { m_oss.str(""); }

  /** Return the logs currently acquired. */
  [[nodiscard]] std::string str() const { return m_oss.str(); }

protected:
  LogInspector() {
    this->SetStream(m_oss);
  }

private:
  std::ostringstream m_oss;
};

/**
 * Utility class that permits to catch and watch logs.
 * A typical use-case is to observe things can cannot be tested directly, but through logs.
 * For instance, a function allocates some memory, and logs how much memory is allocated. Thanks to
 * this utility, we can extract the related logs and test the function does as expected.
 *
 * Example:
 * ```
  auto const logs = otb::WatchLogs::Watch(
      [&]() { writer->Update(); }
  );

  std::regex const re_line{R"(.*otbImageFileReader.hxx, line \d+, ALLOCATE temp buffer: (\d+)MB.*, size=\d+x(\d+) @\(\d+, (\d+)\))"};
  std::istringstream iss(logs);
  std::cout << "Matches:\n";
  for (std::string line ; std::getline(iss, line) ;)
  {
    if (std::smatch sub; std::regex_search(line, sub, re_line)) 
    {
      std::cout << "  --> " << sub[1] << "  Δ" << sub[2] << "  @" << sub[3] << "\n";
    }
  }
 * ```
 *
 * \warning This utility shall not be used in production code, only in small examples and tests.
 * Indeed it relies on `itk::Logger::AddLogOutput()` function to register a dedicated logger.
 * Unfortunately, `itk::Logger` (v 5.3) has no `RemoveLogOutput()` function, meaning that memory
 * will grow indefinitely until OTB process terminated.
 */
class WatchLogs
{
public:
  /**
   * Default constructor.
   * While this constructor is public, unless you need to clear the logs regularly, prefer `Watch()`
   * entry point.
   *
   * Saves the initially defined logging level, and then forces the log level to DEBUG (by default)
   * as we are likelly to watch debug level logs.
   */
  explicit WatchLogs(
      itk::LoggerBaseEnums::PriorityLevel level = itk::LoggerBaseEnums::PriorityLevel::DEBUG
  )
  : m_logger(*otb::Logger::Instance())
  , m_saved_log_level(m_logger.GetPriorityLevel())
  {
    m_log_watcher = otb::LogInspector::New();
    m_logger.AddLogOutput(m_log_watcher);
    m_logger.SetPriorityLevel(level);
  }

  /**
   * Destructor.
   * Reset the log level to what it was, à la RAII.
   *
   * \warning Clear what has been captured by the actual watcher. Note However that we cannot remove
   * the watcher, which will continue to store logs in RAM.
   *
   * \throw None
   */
  ~WatchLogs()
  {
    m_logger.SetPriorityLevel(m_saved_log_level);

    // release the RAM currently allocated
    m_log_watcher->Clear();
  }

  /** Reset the logs currently acquired. */
  void Clear() { m_log_watcher->Clear(); }

  /** Return the logs currently acquired. */
  [[nodiscard]] std::string str() const { return m_log_watcher->str(); }

  /**
   * Main entry entry point.
   * Wraps the execution the code to observe. Captures and return its logs.
   *
   * @tparam F        Functor type, no argument expected
   * @tparam Args     Type of extra arguments for `WatchLogs` constructor call
   *
   * @param[in] f     Functor (lambda e.g.) to execute and observe.
   * @param[in] args  Optional extra parameters to instantiate the `WatchLogs` instance. Permits to
   *                  override the log level.
   *
   * @return The logs captured
   */
  template <typename F, typename... Args>
  [[nodiscard]] static std::string Watch(F && f, Args&&... args)
  {
    WatchLogs wl(std::forward<Args>(args)...);
    f();
    return wl.str();
  }

private:
  otb::Logger &                       m_logger; // Needs to be 1st b/c of construction dependencies
  otb::LogInspector::Pointer          m_log_watcher;
  itk::LoggerBaseEnums::PriorityLevel m_saved_log_level;
};

} // otb namespace

#endif // otbTestLogInspector_h
