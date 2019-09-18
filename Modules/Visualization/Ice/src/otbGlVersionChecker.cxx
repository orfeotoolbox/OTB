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

#include "otbGlVersionChecker.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

#include "itkMacro.h"

namespace otb
{

const char * GlVersionChecker::REQUIRED_GL_VERSION = "2.0.0";
const char * GlVersionChecker::REQUIRED_GLSL_VERSION = "1.20";

const char *
GlVersionChecker
::GLVersion()
{
  const GLubyte * glVersionStr = glGetString( GL_VERSION );

  if( glVersionStr==nullptr )
    {
    std::ostringstream oss;

    oss << "Failed to query OpenGL version string (error: "
        << glGetError()
        << ").";

    throw std::runtime_error( oss.str() );
    }

  return reinterpret_cast< const char * >( glVersionStr );
}


const char *
GlVersionChecker
::GLSLVersion()
{
  const GLubyte * slVersionStr = glGetString( GL_SHADING_LANGUAGE_VERSION );

  if( slVersionStr==nullptr )
    {
    std::ostringstream oss;

    oss << "Failed to query OpenGL shading language version string (error: "
        << glGetError()
        << ").";

    throw std::runtime_error( oss.str() );
    }

  return reinterpret_cast< const char * >( slVersionStr );
}


bool
GlVersionChecker
::CheckGLCapabilities( const char * & glVersion, const char * & glslVersion )
{
  // Get OpenGL version.
  glVersion = GlVersionChecker::GLVersion();

  // If OpenGL version is at least 2.0, get (and return) GLSL version
  // (before checking against OpenGL required version).
  if( GlVersionChecker::VerCmp( glVersion, "2.0" )<0 )
    glslVersion = nullptr;
  else
    glslVersion = GlVersionChecker::GLSLVersion();

  // Now, Check OpenGL version against required version.
  if( GlVersionChecker::VerCmp( glVersion, GlVersionChecker::REQUIRED_GL_VERSION )<0 )
    return false;

  //
  // Then, check OpenGL SL version against required version.
  if( GlVersionChecker::VerCmp( glslVersion, GlVersionChecker::REQUIRED_GLSL_VERSION )<0 )
    return false;

  //
  // Finally, Ok.
  return true;
}


bool
GlVersionChecker
::SplitVersion( const char * version,
                int& major,
                int& minor,
                int& release )
{
  //
  // Special case: empty strings returns 0.0.0 and true.
  if( version==nullptr || strlen( version )==0 )
    {
    major = 0;
    minor = 0;
    release = 0;

    return true;
    }

  //
  // Parse major part.

  major = atoi( version );

  if( major<1 )
    {
    major = 0;

    return false;
    }


  //
  // Parse minor part.

  minor = 0;

  const char * minorStr = strchr( version, '.' );

  if( minorStr==nullptr )
    return false;

  minor = atoi( ++minorStr );

  //
  // Parse release part.

  release = 0;

  const char * releaseStr = strchr( minorStr, '.' );

  // In this case, it is Ok to return because version of the form
  // Major.minor are handled.
  if( releaseStr==nullptr )
    return true;

  release = atoi( ++releaseStr );

  //
  // Ok.
  return true;
}


int
GlVersionChecker
::VerCmp( const char * version, const char * required )
{
  //
  // Split version.

  int verMaj = -1;
  int verMin = -1;
  int verRel = -1;

  if( !GlVersionChecker::SplitVersion( version, verMaj, verMin, verRel ) )
    throw std::invalid_argument( version );

  //
  // Split required version.

  int reqMaj = -1;
  int reqMin = -1;
  int reqRel = -1;

  if( !GlVersionChecker::SplitVersion( required, reqMaj, reqMin, reqRel ) )
    throw std::runtime_error( required );

  //
  // Compare split versions.

  if( verMaj<reqMaj )
    return -1;

  else if( verMaj>reqMaj )
    return 1;

  else // if( verMaj==reqMaj )
    {
    if( verMin<reqMin )
      return -1;

    else if( verMin>reqMin )
      return 1;

    else // if( verMin==reqMin )
      {
      if( verRel<reqRel)
        return -1;

      else if( verRel>reqRel )
        return 1;

      else // if( verRel==reqRel )
        return 0;
      }
    }
}




} // End namespace otb

