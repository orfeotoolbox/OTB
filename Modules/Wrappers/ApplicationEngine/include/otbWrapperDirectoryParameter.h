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

#ifndef otbWrapperDirectoryParameter_h
#define otbWrapperDirectoryParameter_h

#include "otbWrapperStringParameter.h"
#include "otbMacro.h"

#include <string>

namespace otb
{
namespace Wrapper
{

/** \class DirectoryParameter
 *  \brief This class represent a string parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT DirectoryParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef DirectoryParameter            Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(DirectoryParameter, Parameter);

  bool HasValue() const override
  {
    return m_StringParam->HasValue();
  }

  // Set/Get Value
  otbSetObjectMemberMacro(StringParam, Value , std::string);
  otbGetObjectMemberConstMacro(StringParam, Value, std::string);

  // Clear Value
  void ClearValue() override
  {
    m_StringParam->ClearValue();
  }

  // Reimplement the SetActive method
  void SetActive( bool value ) override
  {
    Superclass::SetActive( value );
    m_StringParam->SetActive( value );
  }

  // GetActive method
  otbGetObjectMemberConstMacro(StringParam, Active, bool);

  virtual ParameterType GetType() const override
  {
    return ParameterType_Directory;
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
  DirectoryParameter()
  {
    m_StringParam = StringParameter::New();
  }

  /** Destructor */
  ~DirectoryParameter() override
  {}

private:
  DirectoryParameter(const DirectoryParameter &) = delete;
  void operator =(const DirectoryParameter&); //purposely not
                                              //implemented

  StringParameter::Pointer   m_StringParam;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
