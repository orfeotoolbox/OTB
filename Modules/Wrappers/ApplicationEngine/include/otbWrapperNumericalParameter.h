/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
 *  \brief This class represents a numerical parameter with an optional default value
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

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(NumericalParameter, Parameter);

  /** Typedef of the scalar type */
  typedef T ScalarType;

  /** Implement the reset method (replace value by default value) */
  void Reset() override
  {
    m_Value = m_DefaultValue;
  }

  /** Set the value */
  void SetValue( ScalarType value)
  {
    m_Value = ( value < m_MinimumValue ) ? m_MinimumValue :
              ( value < m_MaximumValue ) ? value : m_MaximumValue ;

    // Set Active only if the parameter is not automatically set
    if (!GetAutomaticValue())
      {
      SetActive(true);
      }
  }

  void SetValue( const std::string & valueStr )
  {
    ScalarType value = static_cast<ScalarType>(atof(valueStr.c_str()));
    SetValue(value);
  }

  /** Get the value */
  ScalarType GetValue() const
  {
    if (!HasValue())
      {
      itkGenericExceptionMacro(<< "Parameter " << this->GetKey() << " has no value yet.");
      }
    return m_Value.value();
  }

  ScalarType GetDefaultValue() const
  {
    if (!HasDefaultValue())
      {
      itkGenericExceptionMacro(<< "Parameter " << this->GetKey() << " has no default value.");
      }
    return m_DefaultValue.value();
  }

  bool HasValue() const override
  {
    return m_Value != boost::none;
  }

  bool HasDefaultValue() const
  {
    return m_DefaultValue != boost::none;
  }

  void ClearValue() override
  {
    m_Value = boost::none;
  }

  void ClearDefaultValue()
  {
    m_DefaultValue = boost::none;
  }

  /** Set the default value */
  itkSetMacro(DefaultValue, ScalarType);

  /** Get/Set the minimum value */
  itkGetMacro(MinimumValue, ScalarType);
  itkSetMacro(MinimumValue, ScalarType);

  /** Get/Set the maximum value */
  itkGetMacro(MaximumValue, ScalarType);
  itkSetMacro(MaximumValue, ScalarType);

protected:
  /** Constructor */
  NumericalParameter()
    : m_Value(boost::none),
      m_DefaultValue(boost::none),
      m_MinimumValue(itk::NumericTraits<T>::NonpositiveMin()),
      m_MaximumValue(itk::NumericTraits<T>::max())
  {}

  /** Destructor */
  ~NumericalParameter() override
  {}

  /** Value or boost::none */
  boost::optional<T> m_Value;

  /** Default value or boost::none **/
  boost::optional<T> m_DefaultValue;

  ScalarType m_MinimumValue;
  ScalarType m_MaximumValue;

private:
  NumericalParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class Numerical Parameter

// Helper typedef for float
typedef NumericalParameter<float> FloatParameter;

// Helper typedef for int
typedef NumericalParameter<int> IntParameter;

} // End namespace Wrapper
} // End namespace otb

#endif
