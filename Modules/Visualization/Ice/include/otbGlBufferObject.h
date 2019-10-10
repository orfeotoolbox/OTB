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

#ifndef otb_GlBufferObject_h
#define otb_GlBufferObject_h

#include "OTBIceExport.h"


#include "otbGlHandle.h"
#include "otbGlTypeTraits.h"


namespace otb { namespace gl {


/** OpenGL element-type of buffer-object.
 */
enum class element : GLenum
{
  vertex = GL_ARRAY_BUFFER,
  index = GL_ELEMENT_ARRAY_BUFFER,
};


/**
 */
template< element E >
struct BufferObjectPolicy
{
  static
  void
  Generate( Id_t & id )
    {
    glGenBuffers( 1, &id );

#if OTB_DEBUG
  std::cout << "glGenBuffers() -> " << id << std::endl;
#endif
    }

  static
  void
  Bind( Id_t id )
    {
    glBindBuffer( static_cast< GLenum >( E ), id );
    }

  static
  void
  Release( Id_t & id )
    {
#if OTB_DEBUG
      std::cout << "glDeleteBuffers( " << id << " )" << std::endl;
#endif

      glDeleteBuffers( 1, &id );
    }
};


/**
 * @class OpenGL object.
 */
template< element E >
struct BufferObject
{
  /** Buffer identifier type. */
  using Id_t = otb::gl::Id_t;

  /** @return element type. */
  static constexpr
  element
  Element() noexcept
  {
    return E;
  }

  /** Default constructor. */
  BufferObject() = default;

  /** Construct and fill. */
  template< typename T >
  BufferObject( std::initializer_list< T > data,
		std::size_t components = 1 ) :
    m_Id(),
    m_Count( data.size() ),
    m_Size( sizeof( T ) ),
    m_Components( components ),
    m_GlType( TypeTraits< T >::value() )
  {
    assert( data.size() );
    assert( components );

    glBufferData(
      static_cast< GLenum >( E ),
      data.size() * sizeof( T ),
      data.begin(),
      GL_STATIC_DRAW
    );

    CheckError();
  }

  ~BufferObject() = default;

  BufferObject( BufferObject && ) = default;

  BufferObject & operator = ( BufferObject && ) = default;

  /** Cast operator. */
  operator Id_t() const noexcept
  {
    return m_Id;
  }

  void
  Bind( bool isEnabled = true ) const
  {
    m_Id.Bind( isEnabled );
  }

  GLenum
  GlType() const noexcept
  {
    return m_GlType;
  }

  std::size_t
  Size() const noexcept
  {
    return m_Size;
  }

  std::size_t
  Stride( std::size_t components ) const noexcept
  {
    assert( m_Size>0 );
    assert( m_Components>0 );

    assert( components<=m_Components );

    return m_Size * ( m_Components - components );
  }

private:
  Handle< BufferObjectPolicy< E > > m_Id;
  std::size_t m_Count = 0;
  std::size_t m_Size = 0;
  std::size_t m_Components = 0;
  GLenum m_GlType = GL_ZERO;
};


// template<> class BufferObject< element::vertex >;
// template<> class BufferObject< element::index >;


using VertexBufferObject = BufferObject< element::vertex >;
using IndexBufferObject = BufferObject< element::index >;


} // end namespace gl.

} // end namespace otb.


#endif // otb_GlBufferObject_h
