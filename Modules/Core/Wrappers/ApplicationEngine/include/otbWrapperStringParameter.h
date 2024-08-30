/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperStringParameter_h
#define otbWrapperStringParameter_h

#include <string>
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class StringParameter
 *  \brief This class represent a string parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT StringParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef StringParameter               Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(StringParameter, Parameter);

  /** Set the value */
  void SetValue(const std::string& value)
  {
    m_Value = value;
    SetActive(true);
  }

  /** Get the value */
  const std::string& GetValue() const
  {
    return m_Value;
  }

  bool HasValue() const override
  {
    return !m_Value.empty();
  }

  void ClearValue() override
  {
    m_Value = "";
  }

  ParameterType GetType() const override
  {
    return ParameterType_String;
  }

  std::string ToString() const override
  {
    return m_Value;
  }

  void FromString(const std::string& value) override
  {
    SetValue(value);
  }

protected:
  /** Constructor */
  StringParameter()
  {
  }

  /** Destructor */
  ~StringParameter() override
  {
  }

  std::string m_Value;

private:
  StringParameter(const StringParameter&) = delete;
  void operator=(const StringParameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
