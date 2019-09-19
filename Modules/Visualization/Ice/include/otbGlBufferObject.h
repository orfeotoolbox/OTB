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


#include "otbGlError.h"
#include "otbGlTypeTraits.h"


#include <utility>

#ifndef bool_str
#  define bool_str( b ) ( ( b ) ? "true" : "false" )
#endif

namespace otb { namespace gl {


/** OpenGL element-type of buffer-object.
 */
enum class element : GLenum
{
  vertex = GL_ARRAY_BUFFER,
  index = GL_ELEMENT_ARRAY_BUFFER,
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
  BufferObject()
  {
    Generate();
    Bind();
  }

  /** Construct and fill. */
  template< typename T >
  BufferObject( std::initializer_list< T > data,
		std::size_t components = 1 ) :
    m_Count( data.size() ),
    m_Size( sizeof( T ) ),
    m_Components( components ),
    m_GlType( TypeTraits< T >::value() )
  {
    assert( data.size() );
    assert( components );

    Generate();

    Bind();

    std::cerr
      << "BufferObject #" << m_Id << " " << TypeTraits< T >::name()
      << std::endl;

    std::cout << data.size() << " " << sizeof( T ) << std::endl;

    glBufferData(
      static_cast< GLenum >( E ),
      data.size() * sizeof( T ),
      data.begin(),
      GL_STATIC_DRAW
    );

    CheckError();
  }

  ~BufferObject()
  {
    Release();
  }

  BufferObject( BufferObject && rhs ) :
    m_Id( std::exchange( rhs.m_Id, GL_ZERO ) )
  {}

  BufferObject &
  operator = ( BufferObject && rhs )

  {
    Release();

    m_Id = std::exchange( rhs.m_Id, GL_ZERO );

    return *this;
  }

  void
  Bind( bool isEnabled = true ) const
  {
    assert( m_Id );

#if OTB_DEBUG
    std::cerr << "glBindBuffer(" << m_Id << ") " << bool_str( isEnabled ) << std::endl;
#endif // OTB_DEBUG

    glBindBuffer( static_cast< GLenum >( E ), isEnabled ? m_Id : GL_ZERO );
    CheckError();
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
  void
  Generate()
  {
#if OTB_DEBUG
  std::cerr << "glGenBuffers()" << std::endl;
#endif // OTB_DEBUG

  glGenBuffers( 1, &m_Id );
  assert( m_Id );

#if OTB_DEBUG
  std::cerr << "-> " << m_Id << std::endl;
#endif // OTB_DEBUG

  CheckError();
  }

  void
  Release()
  {
    if( m_Id==GL_ZERO )
      return;

#if OTB_DEBUG
    std::cerr << "glDeleteBuffers(" << m_Id << ")" << std::endl;
#endif // OTB_DEBUG

    CheckError< error::clear >();

    glDeleteBuffers( 1, &m_Id );
    CheckError();

    m_Id = GL_ZERO;
  }

  Id_t m_Id = GL_ZERO;
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
