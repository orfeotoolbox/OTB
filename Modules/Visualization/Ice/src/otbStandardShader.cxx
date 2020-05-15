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

#include "otbStandardShader.h"

#include <cassert>
#include <GL/glew.h>

#include "otbShaderRegistry.h"
#include "otbGlVersionChecker.h"

namespace otb
{

StandardShader::StandardShader() :
  m_LocalContrastRange( 50 ),
  m_SpectralAngleRange( 10 ),
  m_Center(),
  m_Radius( 200 ),
  m_ChessboardSize( 256 ),
  m_SliderPosition( 500 ),
  m_VerticalSlider( false ),
  m_ShaderType( SHADER_STANDARD )
{
  m_Center.Fill( 0 );

  const char * glVersion = nullptr;
  const char * glslVersion = nullptr;
  if(!otb::GlVersionChecker::CheckGLCapabilities( glVersion, glslVersion))
    {
    itkExceptionMacro(<<" Required GL and GLSL versions were not found (GL version is "<<glVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GL_VERSION<<", GLSL version is "<<glslVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GLSL_VERSION<<")");
    }
  m_HasGLSL140 = otb::GlVersionChecker::VerCmp(glslVersion,"1.40")>=0;

  // Register the shader in the ShaderRegistry
  BuildShader();

  // Reserve variable locations
  //  - for vertex shader
  m_Loc.proj = glGetUniformLocation(m_Program, "in_proj");
  m_Loc.modelview = glGetUniformLocation(m_Program, "in_mv");
  //  - for fragment shader
  m_Loc.a = glGetUniformLocation(m_Program, "shader_a");
  m_Loc.b = glGetUniformLocation(m_Program, "shader_b");
  m_Loc.use_no_data = glGetUniformLocation(m_Program, "shader_use_no_data");
  m_Loc.no_data = glGetUniformLocation(m_Program, "shader_no_data");
  m_Loc.gamma = glGetUniformLocation(m_Program, "shader_gamma");
  m_Loc.alpha = glGetUniformLocation(m_Program, "shader_alpha");
  m_Loc.radius = glGetUniformLocation(m_Program, "shader_radius");
  m_Loc.center = glGetUniformLocation(m_Program, "shader_center");
  m_Loc.type = glGetUniformLocation(m_Program, "shader_type");
  m_Loc.current = glGetUniformLocation(m_Program, "shader_current");
  m_Loc.localc_range = glGetUniformLocation(m_Program, "shader_localc_range");
  m_Loc.spectral_angle_range = glGetUniformLocation(m_Program, "shader_spectral_angle_range");
  m_Loc.chessboard_size = glGetUniformLocation(m_Program, "shader_chessboard_size");
  m_Loc.slider_pos = glGetUniformLocation(m_Program, "shader_slider_pos");
  m_Loc.vertical_slider_flag = glGetUniformLocation(m_Program, "shader_vertical_slider_flag");

  m_AttribIdx.push_back( glGetAttribLocation(m_Program, "position") );
  m_AttribIdx.push_back( glGetAttribLocation(m_Program , "in_coord") );
}

StandardShader::~StandardShader()
{}

std::string
StandardShader
::GetVertexSource() const
{
  return
    "#version 130\n"
    "in vec4 position;\n"
    "in vec2 in_coord;\n"
    "out vec2 tex_coord;\n"
    "uniform mat4 in_proj;\n"
    "uniform mat4 in_mv;\n"
    "void main()\n"
    "{\n"
    "tex_coord = in_coord;\n"
    "gl_Position = in_proj * in_mv * position;\n"
    "}";
}

std::string StandardShader::GetFragmentSource() const
{
  std::string shader_source = "";

  if(m_HasGLSL140)
    {
    shader_source+="#version 140\n";
    }
  else
    {
    shader_source+="#version 130\n";
    }

  shader_source +=
    "uniform sampler2D src;\n"                                          \
    "uniform vec4 shader_a;\n"                                          \
    "uniform vec4 shader_b;\n"                                          \
    "uniform int shader_use_no_data;\n"                                 \
    "uniform float shader_no_data;\n"                                   \
    "uniform vec3 shader_current;\n"                                    \
    "uniform vec4 shader_gamma;\n"                                      \
    "uniform float shader_alpha;\n"                                     \
    "uniform vec2 shader_center;\n"                                     \
    "uniform int shader_type;\n"                                        \
    "uniform float shader_radius;\n"                                    \
    "uniform float shader_localc_range;\n"                              \
    "uniform float shader_spectral_angle_range;\n"                      \
    "uniform float shader_chessboard_size;\n"                           \
    "uniform float shader_slider_pos;\n"                                \
    "uniform int shader_vertical_slider_flag;\n"                        \
    "in vec2 tex_coord;\n"                                              \
    "out vec4 out_color;\n"                                             \
    "void main (void) {\n"                                              \
    "vec4 p = texture(src, tex_coord);\n"                     \
    "vec4 colors = pow( clamp( ( p+shader_b ) * shader_a, 0.0, 1.0 ), shader_gamma );\n" \
    "out_color = colors;\n"                                          \
    "out_color[3] = clamp(shader_alpha,0.0,1.0);\n"                  \
    "if(shader_use_no_data > 0 && vec3(p) == vec3(shader_no_data)){\n"  \
    "out_color[3] = 0.;\n"                                           \
    "}\n"                                                               \
    "float alpha = out_color[3];\n"                                  \
    "float dist = distance(gl_FragCoord.xy,shader_center);\n" \
    "if(shader_type == 1)\n"                                            \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "vec3 tmp = clamp((vec3(p)-vec3(shader_current)+vec3(shader_localc_range))/(2.*vec3(shader_localc_range)),0.0,1.0);\n" \
    "out_color[0] = tmp[0];\n"                                       \
    "out_color[1] = tmp[1];\n"                                       \
    "out_color[2] = tmp[2];\n"                                       \
    "out_color[3] = alpha;\n"                                        \
    "}\n"                                                               \
    "}\n"                                                               \
    "else if(shader_type == 2)"                                         \
    "{\n"                                                               \
    "out_color[3] = dist > shader_radius ? 1.0 : 0.0; \n"            \
    "}\n"                                                               \
    "else if(shader_type == 3)\n"                                       \
    "{\n"                                                               \
    "float alpha = (mod(floor(gl_FragCoord.x / shader_chessboard_size), 2.0) == 0.) != (mod(floor(gl_FragCoord.y / shader_chessboard_size), 2.0) == 1.) ? shader_alpha : 0.0;\n" \
    "out_color[3] = clamp(alpha,0.0,1.0);\n"                         \
    "}\n"                                                               \
    "else if(shader_type == 4)\n"                                       \
    "{\n"                                                               \
    "float alpha = (shader_vertical_slider_flag == 0 && gl_FragCoord.x > shader_slider_pos) || (shader_vertical_slider_flag == 1 && gl_FragCoord.y > shader_slider_pos) ? 1.0 : 0.0;\n" \
    "out_color[3] = clamp(alpha,0.0,1.0);\n"                         \
    "}\n"                                                               \
    "else if(shader_type == 5)\n"                                       \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float angle = acos(clamp(dot(vec3(p),shader_current)/(length(vec3(p))*length(shader_current)),-1.0,1.0));\n" \
    "vec3 tmp = clamp(vec3(1.-shader_spectral_angle_range*abs(angle)/3.142),0.0,1.0);\n" \
    "out_color[0] = tmp[0];\n"                                       \
    "out_color[1] = tmp[1];\n"                                       \
    "out_color[2] = tmp[2];\n"                                       \
    "out_color[3] = alpha;\n"                                        \
    "}\n"                                                               \
    "}\n"                                                               \
    "else if(shader_type == 7)\n"                                       \
    "{\n"                                                               \
    "float color = colors[0];\n"                                              \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = -abs( 3.95 * (color - 0.7460)) + 1.5;\n"              \
    "mapped[1] = -abs( 3.95 * (color - 0.492)) + 1.5;\n"               \
    "mapped[2] = -abs( 3.95 * (color - 0.2385)) + 1.5;\n"              \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
  "else if(shader_type == 8)\n"                                         \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float color=clamp((p[0]-shader_current[0]+shader_localc_range)/(2.*shader_localc_range),0.0,1.0);\n" \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = -abs( 3.95 * (color - 0.7460)) + 1.5;\n"              \
    "mapped[1] = -abs( 3.95 * (color - 0.492)) + 1.5;\n"               \
    "mapped[2] = -abs( 3.95 * (color - 0.2385)) + 1.5;\n"              \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n"                                                               \
  "else if(shader_type == 9)\n"                                         \
    "{\n"                                                               \
    "float color = colors[0];\n"                                        \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = 63.0 / 26.0 * color - 1.0 / 13.0;\n"                   \
    "mapped[1] = 63.0 / 26.0 * color - 11.0 / 13.0;\n"                  \
    "mapped[2] = 4.5 * color - 3.5;\n"                                  \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
  "else if(shader_type == 10)\n"                                         \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float color=clamp((p[0]-shader_current[0]+shader_localc_range)/(2.*shader_localc_range),0.0,1.0);\n" \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = 63.0 / 26.0 * color - 1.0 / 13.0;\n"                   \
    "mapped[1] = 63.0 / 26.0 * color - 11.0 / 13.0;\n"                  \
    "mapped[2] = 4.5 * color - 3.5;\n"                                  \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n"                                                               \
  "else if(shader_type == 11)\n"                                         \
    "{\n"                                                               \
    "float color = colors[0];\n"                                        \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = 0.0;\n"                                                \
    "mapped[1] = color;\n"                                              \
    "mapped[2] = 1.0 - 0.5 * color;\n"                                  \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "else if(shader_type == 12)\n"                                      \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float color=clamp((p[0]-shader_current[0]+shader_localc_range)/(2.*shader_localc_range),0.0,1.0);\n" \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = 0.0;\n"                                                \
    "mapped[1] = color;\n"                                              \
    "mapped[2] = 1.0 - 0.5 * color;\n"                                  \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n"                                                               \
  "else if(shader_type == 13)\n"                                        \
    "{\n"                                                               \
    "float color = colors[0];\n"                                        \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = color;\n"                                              \
    "mapped[1] = 0.5*color+0.5;\n"                                      \
    "mapped[2] = 0.4;\n"                                                \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "else if(shader_type == 14)\n"                                      \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float color=clamp((p[0]-shader_current[0]+shader_localc_range)/(2.*shader_localc_range),0.0,1.0);\n" \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = color;\n"                                              \
    "mapped[1] = 0.5*color+0.5;\n"                                      \
    "mapped[2] = 0.4;\n"                                                \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n"                                                               \
  "else if(shader_type == 15)\n"                                        \
    "{\n"                                                               \
    "float color = colors[0];\n"                                        \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = color;\n"                                              \
    "mapped[1] = 1.0-color;\n"                                          \
    "mapped[2] = 1.0;\n"                                                \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "else if(shader_type == 16)\n"                                      \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float color=clamp((p[0]-shader_current[0]+shader_localc_range)/(2.*shader_localc_range),0.0,1.0);\n" \
    "vec3 mapped;\n"                                                    \
    "mapped[0] = color;\n"                                              \
    "mapped[1] = 1.0-color;\n"                                          \
    "mapped[2] = 1.0;\n"                                                \
    "mapped = clamp(mapped,0.0,1.0);\n"                                 \
    "out_color[0] = mapped[0];\n"                                    \
    "out_color[1] = mapped[1];\n"                                    \
    "out_color[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n";

  if( m_HasGLSL140 )
  {
    shader_source+=
      "else if(shader_type == 6)\n"                                       \
      "{\n"                                                               \
      "if(dist < shader_radius)\n"                                        \
      "{\n"                                                               \
      "vec2 size = vec2(textureSize(src,0));\n"                           \
      "vec2 dx = tex_coord;\n"                              \
      "dx[0]+=1.0/size[0];\n"                                             \
      "vec2 dy = tex_coord;\n"                              \
      "dy[1]+=1.0/size[1];\n"                                             \
      "vec4 pdx = texture2D(src, dx);\n"                                  \
      "vec4 pdy = texture2D(src, dy);\n"                                  \
      "out_color = clamp(pow(5*shader_a*(0.5*abs((pdx-p))+ 0.5*abs((pdy-p))),shader_gamma),0.0,1.0);\n" \
      "out_color[3] = alpha;\n"                                        \
      "}\n"                                                               \
      "}\n";
  }
  shader_source+="}";
  return shader_source;
}

std::string StandardShader::GetName() const
{
  return "StandardShader";
}

void StandardShader::SetupShader()
{
  assert( !m_ImageSettings.IsNull() );

  glUniformMatrix4fv(m_Loc.proj,1, GL_FALSE, m_ProjMatrix);
  glUniformMatrix4fv(m_Loc.modelview,1, GL_FALSE, m_ModelViewMatrix);

  //
  // Compute shifts.
  double shr = -m_ImageSettings->GetMinRed();
  double shg = -m_ImageSettings->GetMinGreen();
  double shb = -m_ImageSettings->GetMinBlue();
  //
  // Compute scales.
  double scr = 1.0 / ( m_ImageSettings->GetMaxRed()+shr );
  double scg = 1.0 / ( m_ImageSettings->GetMaxGreen()+shg );
  double scb = 1.0 / ( m_ImageSettings->GetMaxBlue()+shb );

  double gamma = m_ImageSettings->GetGamma();

  glUniform4f(m_Loc.a,scr,scg,scb,1.);
  glUniform4f(m_Loc.b,shr,shg,shb,0);
  glUniform1i(m_Loc.use_no_data, m_ImageSettings->GetUseNoData()  );
  glUniform1f( m_Loc.no_data, m_ImageSettings->GetNoData() );
  glUniform4f( m_Loc.gamma, gamma, gamma, gamma, gamma );
  glUniform1f( m_Loc.alpha, m_ImageSettings->GetAlpha() );
  glUniform1f(m_Loc.radius,m_Radius);
  glUniform2f(m_Loc.center,m_Center[0],m_Center[1]);
  glUniform1i(m_Loc.type,m_ShaderType);
  glUniform3f(
    m_Loc.current,
    m_ImageSettings->GetCurrentRed(),
    m_ImageSettings->GetCurrentGreen(),
    m_ImageSettings->GetCurrentBlue()
  );
  glUniform1f(m_Loc.localc_range,m_LocalContrastRange);
  glUniform1f(m_Loc.spectral_angle_range,m_SpectralAngleRange);
  glUniform1f(m_Loc.chessboard_size,m_ChessboardSize);
  glUniform1f(m_Loc.slider_pos,m_SliderPosition);
  glUniform1i(m_Loc.vertical_slider_flag,m_VerticalSlider);
}

} // End namespace otb
