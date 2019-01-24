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

#include "otbWrapperStringListParameter.h"


#include "otbCast.h"


namespace otb
{

namespace Wrapper
{


/*****************************************************************************/
StringListParameter
::StringListParameter()
{
  SetName( "String List" );
  SetKey( "strList" );
}


/*****************************************************************************/
StringListParameter
::~StringListParameter()
{
}


/*****************************************************************************/
void
StringListParameter
::SetValue( const StringListInterface::StringVector & strings )
{
  SetStrings( strings );
}

/*****************************************************************************/
StringListInterface::StringVector
StringListParameter
::GetValue() const
{
  // Should get benefit of C++11 move constructor.
  return GetFileNameList();
}

/*****************************************************************************/
void
StringListParameter
::AddString( const std::string & s )
{
  AddFromFileName( s );
}

/*****************************************************************************/
const std::string &
StringListParameter
::GetNthElement( std::size_t i ) const
{
  return GetNthFileName( i );
}

/*****************************************************************************/
void
StringListParameter
::SetNthElement( std::size_t i, const std::string & string )
{
  SetNthFileName( i, string );
}

/*****************************************************************************/
Role
StringListParameter
::GetDirection() const
{
  return GetRole();
}

/*****************************************************************************/
bool
StringListParameter
::IsFilename() const
{
  return false;
}

/*****************************************************************************/
const std::string &
StringListParameter
::ToString( const ParameterType::Pointer & p ) const
{
  assert( !p.IsNull() );

  return p->GetValue();
}

/*****************************************************************************/
const StringListParameter::ParameterType::Pointer &
StringListParameter
::FromString( const ParameterType::Pointer & p,
	      const std::string & s ) const
{
  assert( !p.IsNull() );

  p->SetValue( s );

  return p;
}


}

}
