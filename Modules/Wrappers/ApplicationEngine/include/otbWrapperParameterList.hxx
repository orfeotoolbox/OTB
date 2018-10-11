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

#ifndef otbWrapperParameterList_hxx
#define otbWrapperParameterList_hxx


#include <algorithm>
#include <iterator>

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
  AbstractParameterList(),
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
    Size()>0
    &&
    std::all_of(
      begin(),
      end(),
      []( auto p ) -> bool
      {
        assert( p!=nullptr );
        return p && p->HasValue();
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
  this->SetStrings(strings);
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

  p->FromString(filename);

  m_Parameters.push_back( p );

  assert( !m_Parameters.back().IsNull() );

  SetActive( true );

  Modified();
}

/*****************************************************************************/
template< typename T >
void
ParameterList< T >
::Insert( const std::string & filename, std::size_t index )
{
  typename T::Pointer p( T::New() );

  p->FromString(filename);

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

  m_Parameters[i]->FromString(filename);

  SetActive( true );

  Modified();
}

/*****************************************************************************/
template< typename T >
std::size_t
ParameterList< T >
::SetStrings( const StringVector & strings )
{
  // First clear previous file chosen
  ClearValue();

  if ( !strings.empty() )
    {
    std::transform(
      strings.begin(),
      strings.end(),
      std::back_inserter( m_Parameters ),
      []( auto s ) -> auto
      {
        typename T::Pointer parameter(T::New());
        parameter->FromString(s);
        return parameter;
      }
    );

    SetActive( true );
    Modified();
    }
  return strings.size();
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
    []( auto p ) -> auto
    {
      return p->ToString();
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
std::string
ParameterList< T >
::GetNthFileName( std::size_t i ) const
{
  assert( i<m_Parameters.size() );

  return m_Parameters[i]->ToString();
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

/*****************************************************************************/
template< typename T >
template< typename L, typename From, typename Get >
void
ParameterList< T >
::SetObjectList( L & this_list,
		 const L & list,
		 From from,
		 Get get )
{
  // Force update of input-list elements.
  for( std::size_t i=0; i<list.Size(); i++ )
  {
    assert( list.GetNthElement( i )!=nullptr );

    list.GetNthElement( i )->UpdateOutputInformation();
  }

  // Clear previous target list.
  ClearValue();

  for( std::size_t i=0; i<list.Size(); i++ )
    {
    assert( list.GetNthElement( i )!=nullptr );

    typename T::Pointer parameter;

    from( parameter, list.GetNthElement( i ) );

    m_Parameters.push_back( parameter );

    assert( get( parameter )!=nullptr );

    this_list.PushBack( get( parameter ) );
    }

  SetActive( true );

  Modified();
}

/*****************************************************************************/
template< typename T >
template< typename L, typename Get >
typename L::ObjectType *
ParameterList< T >
::GetObjectList( L & this_list, Get get )
{
  assert( this_list );

  this_list->Clear();

  std::for_each(
    begin(),
    end(),
    [ this_list, get ]( typename T::Pointer parameter ) -> void
    {
      assert( parameter );
      assert( parameter==otb::DynamicCast< T >( parameter ) );

      assert( get( DynamicCast< T >( parameter ) ) );

      this_list->PushBack(
	get(
	  DynamicCast< T >( parameter )
	)
      );
    }
  );

  return this_list;
}

/*****************************************************************************/
template< typename T >
template< typename L, typename Get >
const typename L::ObjectType *
ParameterList< T >
::GetObjectList( L & this_list, Get get ) const
{
  return
    const_cast< ParameterList< T > * >( this )
    ->GetObjectList( this_list, get );
}

/*****************************************************************************/
template< typename T >
template< typename D, typename From >
void
ParameterList< T >
::AddData( D * data, From from )
{
  assert( data!=nullptr );

  // Check input availability
  data->UpdateOutputInformation();

  // Build & add parameter.
  m_Parameters.push_back( from( data ) );

  Modified();
}

/*****************************************************************************/
template< typename T >
template< typename D, typename Set >
typename T::Pointer
ParameterList< T >
::FromData( D * data,
	    Set set,
	    const std::string & description )
{
  assert( data!=nullptr );

  typename T::Pointer p;

  return From( p, data, set, description );
}

/*****************************************************************************/
template< typename T >
template< typename D, typename Set >
typename T::Pointer &
ParameterList< T >
::FromData( typename T::Pointer & parameter,
	    D * data,
	    Set set,
	    const std::string & description )
{
  assert( data!=nullptr );

  parameter = T::New();

  set( parameter, data );
  parameter->SetDescription( description );

  return parameter;
}

template <typename T>
std::vector<std::string> ParameterList<T>::ToStringList() const
{
  return GetFileNameList();
}

template <typename T>
void ParameterList<T>::FromStringList(const std::vector<std::string>& value)
{
  SetStrings(value);
}

template <typename T>
std::string ParameterList<T>::ToString() const
{
  std::ostringstream oss;
  oss << std::setprecision(10);
  auto strList = GetFileNameList();
  for (size_t i = 0; i < strList.size(); i++)
  {
    if (i != 0)
    {
      oss << " ";
    }
    oss << strList[i];
  }
  return oss.str();
}

} // End namespace Wrapper


} // End namespace otb


#endif
