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

#include "otbMinimalShader.h"

#include <cassert>
#include <GL/glew.h>

#include "otbShaderRegistry.h"

namespace otb
{

MinimalShader::MinimalShader()
{
  // Register the shader in the ShaderRegistry
  BuildShader();

  // Reserve variable locations
  //  - for vertex shader
  m_Loc.proj = glGetUniformLocation(m_Program, "in_proj");
  m_Loc.modelview = glGetUniformLocation(m_Program, "in_mv");
  //  - for fragment shader

}

MinimalShader::~MinimalShader()
{}

std::string MinimalShader::GetVertexSource() const
{
  std::string shader_source =
    "#version 130 \n"                                           \
    "in vec4 in_color;\n"                                       \
    "out vec4 v_color;\n"                                       \
    "uniform mat4 in_proj;\n"                                   \
    "uniform mat4 in_mv;\n"                                     \
    "void main()\n"                                             \
    "{\n"                                                       \
    "v_color = in_color;\n"                                     \
    "gl_Position = in_proj * in_mv * gl_Vertex;\n"              \
    "}";

  return shader_source;
}

std::string MinimalShader::GetFragmentSource() const
{
  std::string shader_source =
    "#version 130 \n"                                       \
    "in vec4 v_color;\n"                                    \
    "out vec4 out_color;\n"                                 \
    "void main (void) {\n"                                  \
    "out_color = v_color;\n"                                \
    "}";

  return shader_source;
}

std::string MinimalShader::GetName() const
{
  return "MinimalShader";
}

void MinimalShader::SetupShader()
{
  glUniformMatrix4fv(m_Loc.proj,1, GL_FALSE, m_ProjMatrix);
  glUniformMatrix4fv(m_Loc.modelview,1, GL_FALSE, m_ModelViewMatrix);
}

} // End namespace otb


