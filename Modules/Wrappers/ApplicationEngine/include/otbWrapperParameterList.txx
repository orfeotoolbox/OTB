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

#ifndef otbWrapperParameterList_txx
#define otbWrapperParameterList_txx


#include <algorithm>

#include "otbCast.h"
#include "otbWrapperParameterList.h"


namespace otb
{

namespace Wrapper
{


/*****************************************************************************/
template< typename T >
ParameterList< T >
::ParameterList() :
  m_Parameters()
{
}

/*****************************************************************************/
template< typename T >
ParameterList< T >
::~ParameterList()
{
}

/*****************************************************************************/
template< typename T >
typename ParameterList< T >::ParameterVector::const_iterator
ParameterList< T >
::begin() const
{
  return m_Parameters.begin();
}

/*****************************************************************************/
template< typename T >
typename ParameterList< T >::ParameterVector::const_iterator
ParameterList< T >
::end() const
{
  return m_Parameters.end();
}

/*****************************************************************************/
template< typename T >
bool
ParameterList< T >
::HasValue() const
{
  return
    std::all_of(
      begin(),
      end(),
      []( auto p ) -> bool
      {
        return !p && p->HasValue();
      }
    );
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::ClearValue()
{
  m_Parameters.clear();

  Superclass::ClearValue();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::SetListFromFileName( const StringVector & strings )
{
  // First clear previous file chosen
  ClearValue();

  for( const StringVector::value_type & s : strings )
    AddFromFileName( s );

  Modified();

  SetActive( true );
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::InsertNullElement( std::size_t index )
{
  m_Parameters.insert(
    index>=m_Parameters.size()
    ? m_Parameters.end()
    : m_Parameters.begin() + index,
    typename T::Pointer()
  );

  SetActive( false );

  Modified();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::AddFromFileName( const std::string & filename )
{
  assert( !filename.empty() );

  typename T::Pointer p( T::New() );

  FromString( p, filename );

  m_Parameters.push_back( p );

  assert( !m_Parameters.back().IsNull() );
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::Insert( const std::string & filename, std::size_t index )
{
  typename T::Pointer p( T::New() );

  FromString( p, filename );

  m_Parameters.insert( m_Parameters.begin() + index, p );

  assert( !m_Parameters.back().IsNull() );

  SetActive( true );

  Modified();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::SetNthFileName( std::size_t i,
		  const std::string & filename )
{
  assert( i<m_Parameters.size() );
  assert( !m_Parameters[ i ].IsNull() );

  // Should throw exception when failed.
  FromString( m_Parameters[ i ], filename );

  Modified();

  SetActive( true );
}

/*****************************************************************************/
template< typename T >
std::size_t
ParameterList< T >
::GetStrings( StringVector & strings ) const
{
  std::transform(
    begin(),
    end(),
    std::back_inserter( strings ),
    [ this ]( auto p ) -> auto
    {
      return this->ToString( p );
    }
  );

  return m_Parameters.size();
}

/*****************************************************************************/
template< typename T >
StringListInterface::StringVector
ParameterList< T >
::GetFileNameList() const
{
  StringVector filenames;

  GetStrings( filenames );

  return filenames;
}

/*****************************************************************************/
template< typename T >
const std::string &
ParameterList< T >
::GetNthFileName( std::size_t i ) const
{
  assert( i<m_Parameters.size() );

  return ToString( m_Parameters[ i ] );
}

/*****************************************************************************/
template< typename T >
const std::string &
ParameterList< T >
::GetToolTip( std::size_t i ) const
{
  assert( i<m_Parameters.size() );
  assert( !m_Parameters[ i ].IsNull() );

  return m_Parameters[ i ]->GetDescription();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::Erase( std::size_t start, std::size_t count )
{
  assert( start<m_Parameters.size() );
  assert( start+count<=m_Parameters.size() );

  m_Parameters.erase(
    m_Parameters.begin() + start,
    m_Parameters.begin() + start + count
  );

  Modified();
}

/*****************************************************************************/
template< typename T >
std::size_t
ParameterList< T >
::Size() const
{
  return m_Parameters.size();
}

/*****************************************************************************/
template< typename T >
bool
ParameterList< T >
::IsActive( std::size_t i ) const
{
  assert( i<m_Parameters.size() );
  assert( !m_Parameters[ i ].IsNull() );

  return m_Parameters[ i ]->GetActive();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::Swap( std::size_t i1, std::size_t i2 )
{
  assert( !m_Parameters.empty() );

  auto clamp = [ this ]( std::size_t i ) -> std::size_t
  {
    return
      i>=m_Parameters.size()
      ? m_Parameters.size() - 1
      : i;
  };

  std::swap(
    m_Parameters[ clamp( i1 ) ],
    m_Parameters[ clamp( i2 ) ]
  );
}


} // End namespace Wrapper


} // End namespace otb


#endif
