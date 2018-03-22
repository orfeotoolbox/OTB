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

#ifndef otbWrapperExpNumericalParameter_txx
#define otbWrapperExpNumericalParameter_txx

#include "otbWrapperExpNumericalParameter.h"

namespace otb
{
namespace WrapperExp
{

template< typename T >
NumericalParameter< T >::NumericalParameter() :
    m_NaN( InternalNaN< std::numeric_limits< ValueType >::has_quiet_NaN , ValueType >::value ), 
    m_HasValue( false ) ,
    m_HasDefaultValue( false ) ,
    m_DefaultValue( m_NaN ) ,
    m_MinimumValue( std::numeric_limits< T >::lowest() ) ,
    m_MaximumValue( std::numeric_limits< T >::max() ) ,
    m_Value( m_NaN )
{
}

template< typename T >
void
NumericalParameter< T >::SetDefaultValue( const ValueType & val )
{
  m_HasDefaultValue = true;
  if ( m_DefaultValue != val )
    {
    bool max = val <= m_MaximumValue ;
    bool min = val >= m_MinimumValue ;
    m_DefaultValue = (min && max) ? val : 
      ( max ? m_MinimumValue : m_MaximumValue );
    }
  
}

template< typename T >
void
NumericalParameter< T >::SetMinimumValue( const ValueType & val )
{
  if ( m_MinimumValue != val )
    {
    m_MinimumValue = val;
    bool max = m_MinimumValue <= m_MaximumValue ;
    bool def =  m_MinimumValue <= m_DefaultValue ;
    m_MaximumValue = max ? m_MaximumValue : m_MinimumValue ;
    m_DefaultValue = def ? m_DefaultValue : m_MinimumValue ;
    // log

    }
}

template< typename T >
void
NumericalParameter< T >::SetMaximumValue( const ValueType & val )
{
  if ( m_MaximumValue != val )
    {
    m_MaximumValue = val;
    bool min = m_MaximumValue >= m_MinimumValue ;
    bool def =  m_MaximumValue >= m_DefaultValue ;
    m_MinimumValue = min ? m_MinimumValue : m_MaximumValue ;
    m_DefaultValue = def ? m_DefaultValue : m_MaximumValue ;
    // log

    }
}

template< typename T >
T
NumericalParameter< T >::GetDefaultValue() const
{
  if ( !m_HasDefaultValue )
  {
    // log
    return m_NaN;
  }

  return m_DefaultValue;
}

template< typename T >
bool
NumericalParameter< T >::HasValue() const
{
  return m_HasValue;
}

template <typename T >
T
NumericalParameter< T >::GetInternalValue() const
{
  if ( !m_HasValue )
    return m_NaN;
    // log

  return m_Value;
}


template< typename T >
std::string
NumericalParameter< T >::GetLitteralValue() const
{
  std::ostringstream oss;
  oss << GetInternalValue();
  return oss.str();
}

template< typename T >
float
NumericalParameter< T >::GetFloattingValue() const
{
  return GetInternalValue();
}

template< typename T >
int
NumericalParameter< T >::GetIntegerValue() const
{
  return GetInternalValue();
}

template< typename T >
void
NumericalParameter< T >::Reset()
{
  if( !m_HasDefaultValue )
    {
    //log
    return;
    }
  m_Value = m_DefaultValue;

}

} // end namespace WrapperExp
} // end namespace otb

#endif
