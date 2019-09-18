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

#ifndef otbSwigPrintCallbackCallback_h
#define otbSwigPrintCallbackCallback_h

namespace otb
{

/** \class SwigPrintCallback
 *  \brief Interface for a printing callback method
 *
 *  This class defines the Call method, used to write a string, the Flush
 *  method, used to flush a bugger, and the Isatty method used to determine
 *  if the output is interactive. This class is the base class of the Python
 *  class PythonLogOutputCallback, that uses Swig directors to override these
 *  methods.
 */

class SwigPrintCallback
{
public:
  /** Constructor */
  SwigPrintCallback() = default;

  /** Destructor */
  virtual ~SwigPrintCallback() = default;

  /** Write a string to a buffer */
  virtual void Call(std::string const&){};

  /** Flush the buffer */
  virtual void Flush(){};

  /** Determine if the bufer is interactive */
  virtual bool IsInteractive()
  {
    return false;
  };
};

} // namespace otb

#endif // otbPythonLogOutputCallback_h
