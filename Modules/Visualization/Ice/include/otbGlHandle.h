/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otb_GlHandle_h
#define otb_GlHandle_h

#include "OTBIceExport.h"


#ifdef _WIN32
#  include <windows.h>
#endif

#include <GL/glew.h>
#include "otbGlError.h"

#include <cassert>
#include <utility>


namespace otb { namespace gl {

using Id_t = GLuint;

/**
 * @class OpenGL handle.
 */
template< typename Policy >
class OTBIce_EXPORT Handle
{
  using Policy_t = Policy;

public:
  /** OpenGL handle identifier type */
  using Id_t = otb::gl::Id_t;

  /** Default constructor */
  Handle()
  {
    Policy::Generate( m_Id );
    assert( m_Id );

    CheckError();

    Policy::Bind( m_Id );

    CheckError();
  }

  /** Destructor */
  ~Handle()
  {
    Release();
  }

  /** Copy constructor (disabled). */
  Handle( Handle const & ) = delete;

  /** Assignement operator (disabled). */
  Handle & operator = ( Handle const & ) = delete;

  /** Move constructor. */
  Handle( Handle && rhs ) noexcept :
    m_Id( std::exchange( rhs.m_Id, 0 ) ) {}

  /** Move assignment operator. */
  Handle &
    operator = ( Handle && rhs )
    {
      Release();

      m_Id = std::exchange( rhs.m_Id, GL_ZERO );

      return *this;
    }

  /** Cast operator. */
  operator Id_t() const noexcept
  {
    return m_Id;
  }

  /** OpenGL handle binding */
  void
    Bind( bool isEnabled = true ) const
  {
    assert( m_Id );

    Policy::Bind( isEnabled ? m_Id : GL_ZERO );

    CheckError();
  }

protected:
  /** OpenGL handle identifier (handler) */
  Id_t m_Id = GL_ZERO;

private:
  void
  Release()
  {
    CheckError< error::clear >();

    Policy::Release( m_Id );

    m_Id = GL_ZERO;

    CheckError();
  }
};


} // end namespace gl.

} // end namespace otb.


#endif // otb_GlHandle_h
