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

#ifndef otbPythonLogOutput_h
#define otbPythonLogOutput_h

#include "itkLogOutput.h"
#include "otbSwigPrintCallback.h"

namespace otb
{

/** \class LogOutputCallback
 *  \brief A itk::LogOutput using a LogOutputCallback
 *
 *  This class delegates the Write and Flush methods to a LogOutputCallback
 */
class PythonLogOutput : public itk::LogOutput
{
public:
  typedef PythonLogOutput               Self;
  typedef itk::LogOutput                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef SwigPrintCallback CallbackType;
  itkTypeMacro(PythonLogOutput, itk::LogOutput);

  itkNewMacro(PythonLogOutput);

  /** Set the callback method */
  void SetCallback(CallbackType* callback)
  {
    m_Callback = callback;
    this->Modified();
  }

  /** flush a buffer */
  virtual void Flush() override;

  /** Write to a buffer */
  virtual void Write(double timestamp) override;

  /** Write to a buffer */
  virtual void Write(std::string const& content) override;

  /** Write to a buffer */
  virtual void Write(std::string const& content, double timestamp) override;

protected:
  /** Constructor */
  PythonLogOutput() = default;

  /** Destructor */
  ~PythonLogOutput() override = default;

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  CallbackType* m_Callback;
};

} // namespace otb

#endif // otbPythonLogOutput_h
