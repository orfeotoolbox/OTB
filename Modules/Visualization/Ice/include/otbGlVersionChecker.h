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

#ifndef otb_GlVersionChecker_h
#define otb_GlVersionChecker_h

#include "OTBIceExport.h"

namespace otb
{

class OTBIce_EXPORT GlVersionChecker
{
public:

  /**
   * Constant containing required OpenGL version for
   * client-application feedback messages.
   */
  static const char * REQUIRED_GL_VERSION;

  /**
   * Constant containing required shading-language version for
   * client-application feedback messages.
   */
  static const char * REQUIRED_GLSL_VERSION;

  /**
   * \return The OpenGL version used at runtime. The call to this
   * method requires a valid OpenGL rendering context. An
   * std::runtime_error() is thrown when glGetError() is different
   * than GL_NO_ERROR.
   */
  static const char * GLVersion();

  /**
   * \return The OpenGL Shading-Language version used at runtime. The
   * call to this method requires a valid OpenGL rendering context. An
   * std::runtime_error() is thrown when glGetError() is different
   * than GL_NO_ERROR. This method uses GL_SHADING_LANGUAGE_VERSION
   * which is available only if the OpenGL version is greater than
   * 2.0. (@see
   * http://www.opengl.org/sdk/docs/man2/xhtml/glGetString.xml).
   */
  static const char * GLSLVersion();

  /**
   * \brief Check that OpenGL required capabilities have been reached.
   *
   * \return true if OpengGL capabilities of running platform meets
   * the needs of the library.
   */
  static bool CheckGLCapabilities();

  /**
   * \brief Check that OpenGL required capabilities have been reached.
   *
   * \param glVersion The returned OpenGL version or <code>NULL</code>
   * if its query has failed.
   *
   * \param glslVersion The returned OpenGL Shading-Language version
   * or <code>NULL</code> if its query has failed.
   *
   * \return true if OpengGL capabilities of running platform meets
   * the needs of the library.
   */
  static bool CheckGLCapabilities( const char * & glVersion,
                                   const char * & glslVersion );

  /**
   * \brief Compares to version-strings on the model of strcmp().
   *
   * \param Left hand-side version string (runtime). If
   * <code>NULL</code>, version-string is considered 0.0.0. (\see
   * GlVersionChecker::SplitVersion()).
   *
   * \param Right hande-side version string (required). If
   * <code>NULL</code>, version-string is considered 0.0.0. (\see
   * GlVersionChecker::SplitVersion()).
   *
   * \return <code>-1</code> if version<required; <code>0</code> if
   * version==required; <code>+1</code> if version>=required.
   */
  static int VerCmp( const char * version, const char * required );

  /**
   * \brief Split version string into Major.minor.release triplet.
   *
   * \param version String to split into triplet. If <code>NULL</code>
   * or empty, triplet is 0.0.0.
   * \param major Major number of resulting version triplet.
   * \param minor Minor number of resulting version triplet.
   * \param release Release number of resulting version triplet.
   *
   * \return <code>true</code> if operation has succeeded,
   * <code>false</code> otherwise.
   */
  static bool SplitVersion( const char * version,
                            int& major,
                            int& minor,
                            int& release ) noexcept;

private:
  // Prevent instantiation
  GlVersionChecker() = delete;
  ~GlVersionChecker() = delete;
  GlVersionChecker( const GlVersionChecker & ) = delete;
  void operator = ( const GlVersionChecker & ) = delete;
};

} // End namespace otb


#endif
