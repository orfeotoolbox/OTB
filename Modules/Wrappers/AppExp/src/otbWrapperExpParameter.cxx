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

#include "otbWrapperExpParameter.h"

namespace otb
{
namespace WrapperExp
{

Parameter::Parameter() :
    m_Name( "" ),
    m_Description( "" ),
    m_Key( "" ),
    m_Mandatory( true ),
    m_Active( false ),
    m_UserValueFlag( false ),
    m_UserLevel( UserLevel_Basic ),
    m_Role( Role_Input ),
    m_Root( nullptr ),
    m_Value("")
{}

bool
Parameter::GetActive( bool recurseParents /*= false*/) const
{
  bool result = m_Active;
  if ( recurseParents && !IsRoot() )
    {
    result = result && GetRoot()->GetActive( recurseParents );
    }
  return result;
}

} // end namespace WrapperExp
} //end namespace otb
