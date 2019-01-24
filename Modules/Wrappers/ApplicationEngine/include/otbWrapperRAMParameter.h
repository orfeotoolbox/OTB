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

#ifndef otbWrapperRAMParameter_h
#define otbWrapperRAMParameter_h

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class RAMParameter
 *  \brief This class represent a RAM parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT RAMParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef RAMParameter                  Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(RAMParameter, Parameter);

  /** typedef */
  typedef NumericalParameter<unsigned int>     UnsignedIntParameter;
  typedef UnsignedIntParameter::ScalarType     ScalarType;

  bool HasValue() const override
  {
    return m_UnsignedIntParam->HasValue();
  }

  // Set/Get Value
  otbSetObjectMemberMacro(UnsignedIntParam, Value , ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam, Value , std::string);
  otbGetObjectMemberMacro(UnsignedIntParam, Value , ScalarType);

  // Set/Get extremums
  otbSetObjectMemberMacro(UnsignedIntParam,  DefaultValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  DefaultValue, ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam,  MinimumValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  MinimumValue, ScalarType);
  otbSetObjectMemberMacro(UnsignedIntParam,  MaximumValue, ScalarType);
  otbGetObjectMemberMacro(UnsignedIntParam,  MaximumValue, ScalarType);

  // Clear Value
  void ClearValue() override
  {
    m_UnsignedIntParam->ClearValue();
  }

  // Reset
  void Reset() override
  {
    m_UnsignedIntParam->Reset();
  }

  // Reimplement the SetActive method
  void SetActive( bool value ) override
  {
    Superclass::SetActive( value );
    m_UnsignedIntParam->SetActive( value );
  }

  otbGetObjectMemberConstMacro(UnsignedIntParam, Active, bool);

protected:
  /** Constructor */
  RAMParameter()
  {
    this->SetName("RAM");
    this->SetDescription("Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default).");
    this->SetKey("ram");

    // Initialize the unsigned int NumericalParam
    m_UnsignedIntParam = UnsignedIntParameter::New();

    // 0 RAM is not allowed, make the minimum to 1 by default
    this->SetMinimumValue(1);
  }

  /** Destructor */
  ~RAMParameter() override
  {}

private:
  RAMParameter(const RAMParameter &) = delete;
  void operator =(const RAMParameter&) = delete;

  UnsignedIntParameter::Pointer         m_UnsignedIntParam;
};

} // End namespace Wrapper
} // End namespace otb

#endif
