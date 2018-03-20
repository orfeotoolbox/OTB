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

#ifndef otbWrapperExpNumericalParameter_h
#define otbWrapperExpNumericalParameter_h

#include <limits>
#include <type_traits>
// #include "otbWrapperExpTypes.h"
#include "otbWrapperExpSingleParameter.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

template < typename T >
class OTBApplicationEngine_EXPORT NumericalParameter : 
public SingleParameter
{
public:
  typedef NumericalParameter             Self;
  typedef SingleParameter                Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef T                              ValueType;

  itkTypeMacro( NumericalParameter , SingleParameter );

  itkNewMacro( Self );

  /** Set the default value */
  // itkSetMacro( DefaultValue , ValueType );
  virtual void SetDefaultValue( const ValueType & val );

  /** Get the default value */
  // itkGetMacro( DefaultValue , ValueType );
  virtual ValueType GetDefaultValue() const ;

  /** Set the minimum value */
  // itkSetMacro( MinimumValue , ValueType );
  virtual void SetMinimumValue( const ValueType & val );

  /** Get the minimum value */
  itkGetMacro( MinimumValue , ValueType );

  /** Set the maximum value */
  // itkSetMacro( MaximumValue , ValueType );
  virtual void SetMaximumValue( const ValueType & val );

  /** Get the maximum value */
  itkGetMacro( MaximumValue , ValueType );

  virtual bool HasValue() const override ;

  /*
  Warning! The following functon need work in order to allow 
  complex instantiation of numerical parameter
  */
  virtual std::string GetLitteralValue() const override ;

  virtual float GetFloattingValue() const override ;

  virtual int GetIntegerValue() const override ;

  virtual void Reset() override ;

  // NaN
  template < bool B , typename Val>
  struct InternalNaN
  {
    static constexpr Val value = 0 ;
  };

  template < typename Val>
  struct InternalNaN< true , Val>
  {
    static constexpr Val value = std::numeric_limits< Val>::quiet_NaN() ;
  };


protected:
  NumericalParameter();
  ~NumericalParameter() override = default ;

  ValueType GetInternalValue() const ;

  ValueType m_NaN;

  bool m_HasValue, m_HasDefaultValue ;

  /** Default value (when appliable) */
  ValueType m_DefaultValue;

  /** Minimum value */
  ValueType m_MinimumValue;

  /** Maximum value */
  ValueType m_MaximumValue;

  /** Value */
  ValueType m_Value;

private:
};

} // end namespace WrapperExp
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperExpNumericalParameter.txx"
#endif

#endif
