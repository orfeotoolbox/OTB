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

#include "otbWrapperExpNumericalParameterInterface.h"

namespace otb
{
namespace WrapperExp
{

NumericalParameterInterface::NumericalParameterInterface() :
m_Boundaries({ any_numeric( std::numeric_limits< double >::lowest() ),
               any_numeric::DefaultInit(),
               any_numeric( std::numeric_limits< double >::max() ) })
{
}

void 
NumericalParameterInterface::
SetDefaultValue( any_numeric val )
{
  assert ( val <= m_Boundaries[2] );
  assert ( val >= m_Boundaries[0] );
  m_Boundaries[1] = val ;
}

void 
NumericalParameterInterface::
SetMinimumValue( any_numeric val )
{
  assert ( val <= m_Boundaries[2] );
  m_Boundaries[1] = ( val <= m_Boundaries[1] ) ? m_Boundaries[1] : val ; 
  m_Boundaries[0] = val ;
}

void 
NumericalParameterInterface::
SetMaximumValue( any_numeric val )
{
  assert ( val >= m_Boundaries[0] );
  m_Boundaries[1] = ( val >= m_Boundaries[1] ) ? m_Boundaries[1] : val ; 
  m_Boundaries[2] = val ;
}

} // end namespace WrapperExp
} //end namespace otb
