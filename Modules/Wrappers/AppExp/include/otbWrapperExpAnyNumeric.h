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

#ifndef otbWrapperExpAnyNumeric_h
#define otbWrapperExpAnyNumeric_h

#include <typeinfo>
#include <algorithm>
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

class any_numeric
{
public:
  any_numeric() : m_Numeric ( nullptr ) {};

  ~any_numeric(){ delete m_Numeric ;} ;

  any_numeric( const any_numeric & other)
  : m_Numeric( other.m_Numeric ? other.m_Numeric->clone() : nullptr )
  {
  }

  any_numeric( any_numeric&& other)
  : m_Numeric( other.m_Numeric ? std::move(other.m_Numeric) : nullptr )
  {
  }

  template< typename T >
  any_numeric( const T & val)
  : m_Numeric( new numeric< T >( val ) )
  {
  }

  static any_numeric DefaultInit()
  {
     return any_numeric( std::numeric_limits<double>::quiet_NaN() );
  }

  any_numeric & swap( any_numeric & rhs )
    {
    std::swap( m_Numeric , rhs.m_Numeric );
    return *this;
    }

  any_numeric & operator=( any_numeric & rhs )
    {
    return swap( rhs );
    }

  template< typename T >
  any_numeric & operator=( const T & rhs )
    {
    auto temp = any_numeric( rhs );
    return swap(  temp );
    }

  const std::type_info & type_info() const
    {
    return m_Numeric ? m_Numeric->type_info() : typeid(void) ;
    }

  bool operator<( const any_numeric & rhs )
    {
    return as<double>() < rhs.as<double>() ;
    }

  bool operator>( const any_numeric & rhs )
    {
    return rhs < this ;
    }

  bool operator<=( const any_numeric & rhs )
    {
    return !(rhs > this) ;
    }

  bool operator>=( const any_numeric & rhs )
    {
    return !(rhs < this) ;
    }

  bool operator==( const any_numeric & rhs )
    {
    return as<double>() == rhs.as<double>() ;
    }

  bool operator!=( const any_numeric & rhs )
    {
    return !((*this) == rhs ) ;
    }

  template < typename T >
  T as() const
    {
      return dynamic_cast<numeric<T>*>(m_Numeric)?
      dynamic_cast<numeric<T>*>(m_Numeric)->m_Number
      : 0;
    // return m_Numeric ? static_cast< T > ( m_Numeric->m_Number ) : T(0);
    }

    operator const void *() const
    {
    return m_Numeric;
    }

private:

  class numeric_holder
  {
  public:
    virtual ~numeric_holder() = default ;
    numeric_holder() = default;
    virtual const std::type_info & type_info() const = 0 ;
    virtual numeric_holder * clone() const = 0 ;
  };

  template < typename T >
  class numeric : public numeric_holder
  {
  public:
    numeric( const T & val ) : m_Number(val) {};
    ~numeric() = default ;

    virtual const std::type_info & type_info() const
    {
      return typeid(T);
    }

    virtual numeric_holder * clone() const
    {
    return new numeric( m_Number );
    }

    const T m_Number;
  };

  numeric_holder * m_Numeric;

};

} // end namespace wrapperexp
} // end namespace otb

#endif
