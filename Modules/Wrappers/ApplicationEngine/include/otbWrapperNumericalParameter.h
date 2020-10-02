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

#ifndef otbWrapperNumericalParameter_h
#define otbWrapperNumericalParameter_h

#include "otbWrapperParameter.h"
#include "itkNumericTraits.h"
#include <boost/optional.hpp>

namespace otb
{
namespace Wrapper
{
/** \class NumericalParameter
 *  \brief This class represents a numerical parameter
 *
 * \ingroup OTBApplicationEngine
 */
template <class T>
class ITK_ABI_EXPORT NumericalParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef NumericalParameter            Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Typedef of the scalar type */
  typedef T ScalarType;

  /** Implement the reset method (replace value by default value) */
  void Reset() override
  {
    m_Value = m_DefaultValue;
  }

  /** Set the value */
  void SetValue(ScalarType value)
  {
    m_Value = (value < m_MinimumValue) ? m_MinimumValue : (value < m_MaximumValue) ? value : m_MaximumValue;

    // Set Active only if the parameter is not automatically set
    if (!GetAutomaticValue())
    {
      SetActive(true);
    }
  }

  void SetValue(const std::string& valueStr)
  {
    ScalarType value = static_cast<ScalarType>(atof(valueStr.c_str()));
    SetValue(value);
  }

  ScalarType GetValue() const
  {
    if (!HasValue())
    {
      itkGenericExceptionMacro(<< "Parameter " << this->GetKey() << " has no value yet.");
    }
    return static_cast<ScalarType>(*m_Value);
  }

  bool HasValue() const override
  {
    return m_Value != boost::none;
  }

  void ClearValue() override
  {
    m_Value.reset();
  }

  /** Set the default value */
  itkSetMacro(DefaultValue, ScalarType);

  /** Get the default value */
  itkGetMacro(DefaultValue, ScalarType);

  /** Set the minimum value */
  itkSetMacro(MinimumValue, ScalarType);

  /** Get the minimum value */
  itkGetMacro(MinimumValue, ScalarType);

  /** Set the maximum value */
  itkSetMacro(MaximumValue, ScalarType);

  /** Get the maximum value */
  itkGetMacro(MaximumValue, ScalarType);

  // TODO move to hxx
  int ToInt() const override
  {
    if (!HasValue())
    {
      itkExceptionMacro("Cannot convert parameter " << GetKey() << " to int (no value).");
    }
    return static_cast<int>(*m_Value);
  }

  float ToFloat() const override
  {
    if (!HasValue())
    {
      itkExceptionMacro("Cannot convert parameter " << GetKey() << " to float (no value).");
    }
    return static_cast<float>(*m_Value);
  }

  double ToDouble() const override
  {
    if (!HasValue())
    {
      itkExceptionMacro("Cannot convert parameter " << GetKey() << " to double (no value).");
    }
    return static_cast<double>(*m_Value);
  }

  void FromInt(int value) override
  {
    SetValue(value);
  }

  void FromString(const std::string& value) override
  {
    SetValue(value);
  }

  std::string ToString() const override
  {
    std::ostringstream oss;
    oss << this->GetValue();
    return oss.str();
  }

protected:
  /** Constructor */
  NumericalParameter()
    : m_DefaultValue(itk::NumericTraits<T>::Zero), m_MinimumValue(itk::NumericTraits<T>::NonpositiveMin()), m_MaximumValue(itk::NumericTraits<T>::max())
  {
  }

  /** Destructor */
  ~NumericalParameter() override
  {
  }

  /** Value */
  boost::optional<T> m_Value;

  /** Default value (when appliable) */
  ScalarType m_DefaultValue;

  /** Minimum value */
  ScalarType m_MinimumValue;

  /** Maximum value */
  ScalarType m_MaximumValue;

private:
  NumericalParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

}; // End class Numerical Parameter

class OTBApplicationEngine_EXPORT FloatParameter : public NumericalParameter<float>
{
public:
  /** Standard class typedef */
  typedef FloatParameter                Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(NumericalParameter, Parameter);

  ParameterType GetType() const override
  {
    return ParameterType_Float;
  }

  void FromFloat(float value) override
  {
    SetValue(value);
  }
};

class OTBApplicationEngine_EXPORT DoubleParameter : public NumericalParameter<double>
{
public:
  /** Standard class typedef */
  typedef DoubleParameter                Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(NumericalParameter, Parameter);

  ParameterType GetType() const override
  {
    return ParameterType_Double;
  }

  void FromDouble(double value) override
  {
    SetValue(value);
  }
};

class OTBApplicationEngine_EXPORT IntParameter : public NumericalParameter<int>
{
public:
  /** Standard class typedef */
  typedef IntParameter                  Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(NumericalParameter, Parameter);

  ParameterType GetType() const override
  {
    return ParameterType_Int;
  }
};

class OTBApplicationEngine_EXPORT RAMParameter : public NumericalParameter<unsigned int>
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

  ParameterType GetType() const override
  {
    return ParameterType_RAM;
  }

  /** Constructor */
  RAMParameter() : NumericalParameter<unsigned int>()
  {
    this->SetName("RAM");
    this->SetDescription("Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default).");
    this->SetKey("ram");

    // 0 RAM is not allowed, make the minimum to 1 by default
    this->SetMinimumValue(1);
  }
};

class OTBApplicationEngine_EXPORT RadiusParameter : public IntParameter
{
public:
  typedef RadiusParameter               Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(RadiusParameter, Parameter);

  bool HasValue() const override
  {
    return true;
  }

  ParameterType GetType() const override
  {
    return ParameterType_Radius;
  }

protected:
  RadiusParameter()
  {
    this->SetName("Radius");
    this->SetKey("r");
    this->SetDescription("Radius in pixels");
  }
};

} // End namespace Wrapper
} // End namespace otb

#endif
