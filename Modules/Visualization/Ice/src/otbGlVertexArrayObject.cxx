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

#include "otbGlVertexArrayObject.h"

#define bool_str( b ) ( ( b ) ? "true" : "false" )

namespace otb { namespace gl {


VertexArrayObject
::VertexArrayObject()
{
#if OTB_DEBUG
  std::cerr << "glGenVertexArrays()" << std::endl;
#endif // OTB_DEBUG

  glGenVertexArrays( 1, &m_Id );
  assert( m_Id );

#if OTB_DEBUG
  std::cerr << "-> " << m_Id << std::endl;
#endif // OTB_DEBUG

  CheckError();

  Bind();
}


VertexArrayObject
::~VertexArrayObject()
{
  Release();
}


VertexArrayObject &
VertexArrayObject
::operator = ( VertexArrayObject && rhs )
{
  Release();

  m_Id = std::exchange( rhs.m_Id, GL_ZERO );

  return *this;
}


void
VertexArrayObject
::Bind( bool isEnabled ) const
{
  assert( m_Id );

#if OTB_DEBUG
  std::cerr
    << "glBindVertexArrays( " << m_Id <<  " ) " << bool_str( isEnabled ) <<
    std::endl;
#endif // OTB_DEBUG

  CheckError< error::clear >();

  glBindVertexArray( isEnabled ? m_Id : GL_ZERO );
  CheckError();
}


void
VertexArrayObject
::Release()
{
  if( m_Id==GL_ZERO )
    return;

#if OTB_DEBUG
  std::cerr << "glDeleteVertexArrays(" << m_Id << ")" << std::endl;
#endif // OTB_DEBUG

  glDeleteVertexArrays( 1, &m_Id );
  CheckError();

  m_Id = GL_ZERO;
}


} // End namespace 'gl'.

} // End namespace 'otb'.
