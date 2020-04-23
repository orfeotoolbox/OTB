/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperMacros_h
#define otbWrapperMacros_h


#define otbAppLogFATAL(x)                                                              \
  {                                                                                    \
    std::ostringstream msg;                                                            \
    msg << "" x << std::endl;                                                          \
    this->GetLogger()->Fatal(msg.str());                                               \
    {                                                                                  \
      std::ostringstream message;                                                      \
      message << "otb::ApplicationException " x;                                       \
      ::otb::ApplicationException e_(__FILE__, __LINE__, message.str(), ITK_LOCATION); \
      throw e_; /* Explicit naming to work around Intel compiler bug.  */              \
    }                                                                                  \
  }

#define otbAppLogCRITICAL(x)                                        \
  {                                                                 \
    std::ostringstream msg;                                         \
    msg << "" x << std::endl;                                       \
    this->GetLogger()->Write(itk::LoggerBase::CRITICAL, msg.str()); \
  }

#define otbAppLogWARNING(x)                                        \
  {                                                                \
    std::ostringstream msg;                                        \
    msg << "" x << std::endl;                                      \
    this->GetLogger()->Write(itk::LoggerBase::WARNING, msg.str()); \
  }

#define otbAppLogINFO(x)                                        \
  {                                                             \
    std::ostringstream msg;                                     \
    msg << "" x << std::endl;                                   \
    this->GetLogger()->Write(itk::LoggerBase::INFO, msg.str()); \
  }

#define otbAppLogDEBUG(x)                                        \
  {                                                              \
    std::ostringstream msg;                                      \
    msg << "" x << std::endl;                                    \
    this->GetLogger()->Write(itk::LoggerBase::DEBUG, msg.str()); \
  }

#endif
