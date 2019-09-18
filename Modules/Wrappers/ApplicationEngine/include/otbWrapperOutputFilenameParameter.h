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

#ifndef otbWrapperOutputFilenameParameter_h
#define otbWrapperOutputFilenameParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class OutputFilenameParameter
 *  \brief This class represent a string parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT OutputFilenameParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputFilenameParameter       Self;
  typedef StringParameter               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputFilenameParameter, Parameter);


  bool HasValue() const override
  {
    return m_StringParam->HasValue();
  }

  // Get Value
  otbGetObjectMemberConstMacro(StringParam, Value, std::string);

  // Set Value
  virtual void SetValue(const std::string value)
  {
    itkDebugMacro("setting member m_StringParam to " << value);
    this->m_StringParam->SetValue(value);
    SetActive(true);
    this->Modified();
  }

  // Clear Value
  void ClearValue() override
  {
    m_StringParam->ClearValue();
  }

  ParameterType GetType() const override
  {
    return ParameterType_OutputFilename;
  }

  std::string ToString() const override
  {
    return GetValue();
  }

  void FromString(const std::string& value) override
  {
    SetValue(value);
  }

protected:
  /** Constructor */
  OutputFilenameParameter()
  {
    // Initialize the String Parameter
    m_StringParam = StringParameter::New();
  }

  /** Destructor */
  ~OutputFilenameParameter() override
  {
  }

private:
  OutputFilenameParameter(const OutputFilenameParameter&) = delete;
  void operator                                           =(const OutputFilenameParameter&); // purposely not
  // implemented

  StringParameter::Pointer m_StringParam;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
