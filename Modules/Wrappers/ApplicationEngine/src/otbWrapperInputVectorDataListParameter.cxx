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

#include "otbWrapperInputVectorDataListParameter.h"


namespace otb
{


namespace Wrapper
{


const std::string
VECTOR_DATA_FILTER(
  "All files (*);;"
  "Shape file (*shp)"
);


/*****************************************************************************/
InputVectorDataParameter::Pointer
InputVectorDataListParameter
::FromVectorData( VectorDataType * data )
{
  assert( data!=nullptr );

  InputVectorDataParameter::Pointer p;

  return FromVectorData( p, data );
}

/*****************************************************************************/
InputVectorDataParameter::Pointer &
InputVectorDataListParameter
::FromVectorData( InputVectorDataParameter::Pointer & parameter,
		  VectorDataType * data )
{
  return
    FromData(
      parameter,
      data,
      []( auto p, auto d ) -> void
      {
        assert( p );

	p->SetVectorData( d );
      },
      "Vector-data filename"
    );
}

/*****************************************************************************/
InputVectorDataListParameter
::InputVectorDataListParameter() :
  m_VectorDataList( VectorDataListType::New() )
{
  SetName( "Input VectorData List" );
  SetKey( "vdList" );
}

/*****************************************************************************/
InputVectorDataListParameter
::~InputVectorDataListParameter()
{
}

/*****************************************************************************/
const VectorDataListType *
InputVectorDataListParameter
::GetVectorDataList() const
{
  return
    const_cast< InputVectorDataListParameter * >( this )
    ->GetVectorDataList();
}

/*****************************************************************************/
VectorDataListType *
InputVectorDataListParameter
::GetVectorDataList()
{
  return
    GetObjectList(
      m_VectorDataList,
      []( auto param ) -> auto
      {
        assert( param );

	return param->GetVectorData();
      }
    );
}

/*****************************************************************************/
const VectorDataType *
InputVectorDataListParameter
::GetNthVectorData( std::size_t i )
{
  assert( i<m_Parameters.size() );
  assert( !m_Parameters[ i ].IsNull() );
  assert( m_Parameters[ i ]->GetVectorData()!=nullptr );

  return m_Parameters[ i ]->GetVectorData();
}

/*****************************************************************************/
void
InputVectorDataListParameter
::SetVectorDataList( VectorDataListType * vdList )
{
  assert( vdList!=nullptr );
  assert( !m_VectorDataList.IsNull() );

  SetObjectList(
    *m_VectorDataList,
    *vdList,
    //
    [ this ]( auto p, auto vd ) -> auto
    {
      this->FromVectorData( p, vd );
    },
    //
    []( auto p ) -> auto
    {
      assert( p );

      return p->GetVectorData();
    }
  );
}

/*****************************************************************************/
void
InputVectorDataListParameter
::AddVectorData( VectorDataType * vectorData )
{
  AddData(
    vectorData,
    [ this ]( auto d ) -> auto
    {
      return this->FromVectorData( d );
    }
  );
}

/*****************************************************************************/
void
InputVectorDataListParameter
::ClearValue()
{
  Superclass::ClearValue();

  assert( m_VectorDataList );

  m_VectorDataList->Clear();
}

/*****************************************************************************/
Role
InputVectorDataListParameter
::GetDirection() const
{
  return Role_Input;
}

/*****************************************************************************/
const std::string &
InputVectorDataListParameter
::GetFilenameFilter() const
{
  return VECTOR_DATA_FILTER;
}

}

}
