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

#ifndef otb_GlTypeTraits_h
#define otb_GlTypeTraits_h

#include "OTBIceExport.h"


#ifdef _WIN32
#  include <windows.h>
#endif

#include <GL/glew.h>


namespace otb { namespace gl {


using Id_t = GLuint;


/**
 * @class traits to associate (OpenGL) type to its GLenum value.
 */
template< typename T >
struct TypeTraits
{
  /** OpenGL type */
  using Type = T;

  /** OpenGL type enum */
  static constexpr GLenum value() noexcept;

  static char const * name() noexcept;
};


/** Define OpenGL type to its GLenum value */
#define OTB_GL_TYPE_TRAITS( type, glenum, glname )			\
  /** OpenGL type to GLenum association. */				\
  template<>								\
  struct TypeTraits< type >						\
  {									\
    static constexpr GLenum value() noexcept { return glenum; }		\
									\
    static constexpr char const * name() noexcept { return glname; }	\
  }

OTB_GL_TYPE_TRAITS( GLfloat, GL_FLOAT, "GLfloat" );
OTB_GL_TYPE_TRAITS( GLdouble, GL_DOUBLE, "GLdouble" );
OTB_GL_TYPE_TRAITS( GLint, GL_INT, "GLint" );
OTB_GL_TYPE_TRAITS( GLuint, GL_UNSIGNED_INT, "GLuint" );


} // end namespace gl.

} // end namespace otb.


#endif // otb_TypeTraits_h
