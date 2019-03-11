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

#ifndef otbStandardOutputPrintCallback_h
#define otbStandardOutputPrintCallback_h

#include "otbSystem.h"

namespace otb
{

/** \class StandardOutputPrintCallback
 *  \brief Class with printing callback methods using the standard output
 *
 *  This class defines the Call method, used to write a string the standard 
 *  output, the Flush method, used to flush it, and the IsInteractive 
 *  method used to determine if the output is the console.
 *  
 *  \see otb::StandardOneLineFilterWatcher
 *
 * \ingroup OTBCommon
 */

class StandardOutputPrintCallback
{
public:
  /** Constructor */
  StandardOutputPrintCallback() :m_IsInteractive( System::IsInteractive(1)) {};

  /** Destructor */
  virtual ~StandardOutputPrintCallback() = default;

  /** Write a string to a buffer */
  void Call(std::string const& content);

  /** Flush the buffer */
  void Flush();

  /** Determine if the output is interactive */
  bool IsInteractive();
  
private:
  /** flag determining if the output is interactive */
  bool m_IsInteractive;
};

} // namespace otb

#endif // otbStandardOutputPrintCallback_h
