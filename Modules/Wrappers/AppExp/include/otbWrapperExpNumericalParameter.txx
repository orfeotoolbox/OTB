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
#include "otbWrapperExpNumericalParameter.h"

namespace otb
{
namespace WrapperExp
{

template< class T >
NumericalParameter< T >::NumericalParameter() :
    m_HasValue( false ) ,
    m_HasDefaultValue( false ) ,
    m_DefaultValue( 0 ) ,
    m_MinimumValue( std::numeric_limits< T >::max() ) ,
    m_MaximumValue( std::numeric_limits< T >::lowest() ) ,
    m_Value( 0 )
{}

void
template< class T >
NumericalParameter< T >::SetDefaultValue( const ValueType & val )
{
  m_HasDefaultValue = true;
  m_DefaultValue = val;
  Modified();
}

typename T
template< class T >
NumericalParameter< T >::GetDefaultValue() const
{
  if ( !m_HasDefaultValue )
    //log
  return m_DefaultValue;
}

bool
template< class T >
NumericalParameter< T >::HasValue() const
{
  return m_HasValue;
}

const std::string &
template< class T >
NumericalParameter< T >::GetLitteralValue() const
{
  std::ostringstream oss;
  oss << this->GetParameterInt( paramKey );
  return oss.str();
}

float
template< class T >
NumericalParameter< T >::GetFloattingValue() const
{
  return m_Value;
}

int
template< class T >
NumericalParameter< T >::GetIntegerValue() const
{
  return m_Value;
}

void
template< class T >
NumericalParameter< T >::Reset()
{
  if( !m_HasDefaultValue )
    {
    //log
    return;
    }
  m_Value = m_DefaultValue;
  Modified();
}

} // end namespace WrapperExp
} // end namespace otb
