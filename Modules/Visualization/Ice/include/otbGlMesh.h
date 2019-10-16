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

#ifndef otb_GlMesh_h
#define otb_GlMesh_h

#include "OTBIceExport.h"


#include "otbGlVertexArrayObject.h"
#include "otbGlBufferObject.h"


namespace otb { namespace gl {

/**
 * @class OpenGL mesh.
 */
struct Mesh
{
  Mesh() = default;

  template< typename VT,
	    typename IT = GLuint >
  Mesh( std::initializer_list< VT > vertices,
	std::size_t components,
	std::initializer_list< IT > indices ) :
    m_VBO( vertices, components ),
    m_IBO( indices, 3 )
  {}

  Mesh( Mesh && ) = default;

  // Mesh & operator = ( Mesh && ) = default;

  // Mesh( Mesh && m ) :
  //   m_VAO( std::move( m.m_VAO ) ),
  //   m_VBO( std::move( m.m_VBO ) ),
  //   m_IBO( std::move( m.m_IBO ) )
  // {}


  void
  Bind( bool isEnabled = true )
  {
    assert( glIsBuffer( m_VBO ) );
    assert( glIsBuffer( m_IBO ) );

    assert( glIsVertexArray( m_VAO ) );

    m_VAO.Bind( isEnabled );
    m_VBO.Bind( isEnabled );
    m_IBO.Bind( isEnabled );
  }

  void
  VertexAttribPointer( VertexBufferObject::Id_t id,
		       std::size_t components,
		       std::ptrdiff_t first = 0 ) const;

  VertexBufferObject const &
  VBO() const noexcept
  {
    return m_VBO;
  }

  IndexBufferObject const &
  IBO() const
  {
    return m_IBO;
  }

private:
  VertexArrayObject m_VAO;
  VertexBufferObject m_VBO;
  IndexBufferObject m_IBO;
};


Mesh MakeTexturedQuad( Id_t xy, Id_t uv );

Mesh MakeInterleavedTexturedQuad( Id_t xy, Id_t uv );

Mesh MakeQuad( Id_t xy);

} // end namespace gl.

} // end namespace otb.


#endif // otb_GlMesh_h
