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

#ifndef otb_GlError_h
#define otb_GlError_h


#include "OTBIceExport.h"

#ifdef _WIN32
#  include <windows.h>
#endif

#include <GL/glew.h>

#include <cassert>
#include <iostream>
#include <stdexcept>

#include <sstream>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4275) // warning C4275: non-DLL-interface std::runtime_error used as base for DLL-interface Error
#endif

namespace otb { namespace gl {

/**
 * @class OpenGL-specific exception.
 */
struct OTBIce_EXPORT Error : public std::runtime_error
{
  /** Construct an OpenGL exception related to the OpenGL error code.
   */
  Error( GLenum code ):
    std::runtime_error(
      [ code ]()
      {
        std::ostringstream oss;

        oss << "OpenGL error #" << code << ": '" << gluErrorString( code ) << "'";

        return oss.str();
      }()
    ) {}

}; // End class GlError

enum class error : int
{
  clear = 0,
  trace,
  assertion,
  exception,
};


template< error E = error::exception >
GLenum
CheckError()
{
  GLenum glError = glGetError();

  if( E>=error::trace )
    if( glError!=GL_NO_ERROR )
      std::cerr
	<< "OPenGL error #" << glError << ": '" << gluErrorString( glError ) << "'"
	<< std::endl;

  if( E>=error::assertion )
    assert( glError==GL_NO_ERROR );

  // When assert is removed (NDEBUG) and Throw is false, this function
  // body is emtpy and the C++ compiler should optimize the call by
  // removing it.
  //
  // N.B.: equivalent of a scoped (and breakpoint-friendly) macro.

  // Condition is splitted because first is determined at compile time
  // and may be removed (see above) and second is determined at
  // runtime.
  if( E>=error::exception )
    if( glError!=GL_NO_ERROR )
      throw Error( glError );

  return glError;
}

} // End of namespace gl.

} // End namespace otb.


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // otb_GlError_h
