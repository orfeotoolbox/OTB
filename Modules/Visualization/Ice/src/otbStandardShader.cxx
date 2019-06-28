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

#include "otbStandardShader.h"

#include <cassert>
#include <GL/glew.h>

#include "otbFragmentShaderRegistry.h"
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

  BuildShader();
}

StandardShader::~StandardShader()
{}

std::string StandardShader::GetSource() const
{
  const char * glVersion = nullptr;
  const char * glslVersion = nullptr;
  if(!otb::GlVersionChecker::CheckGLCapabilities( glVersion, glslVersion))
    {
    itkExceptionMacro(<<" Required GL and GLSL versions were not found (GL version is "<<glVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GL_VERSION<<", GLSL version is "<<glslVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GLSL_VERSION<<")");
    }

  bool isGLSLS140Available  = otb::GlVersionChecker::VerCmp(glslVersion,"1.40")>=0;

  std::string shader_source = "";

  if(isGLSLS140Available)
    {
    shader_source+="#version 140 \n";
    }
  else
    {
    shader_source+="#version 130 \n";
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
    "void main (void) {\n"                                              \
    "vec4 p = texture2D(src, gl_TexCoord[0].xy);\n"                     \
    "vec4 colors = pow( clamp( ( p+shader_b ) * shader_a, 0.0, 1.0 ), shader_gamma );\n" \
    "gl_FragColor = colors;\n"                                          \
    "gl_FragColor[3] = clamp(shader_alpha,0.0,1.0);\n"                  \
    "if(shader_use_no_data > 0 && vec3(p) == vec3(shader_no_data)){\n"  \
    "gl_FragColor[3] = 0.;\n"                                           \
    "}\n"                                                               \
    "float alpha = gl_FragColor[3];\n"                                  \
    "float dist = distance(gl_FragCoord.xy,shader_center);\n" \
    "if(shader_type == 1)\n"                                            \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "vec3 tmp = clamp((vec3(p)-vec3(shader_current)+vec3(shader_localc_range))/(2.*vec3(shader_localc_range)),0.0,1.0);\n" \
    "gl_FragColor[0] = tmp[0];\n"                                       \
    "gl_FragColor[1] = tmp[1];\n"                                       \
    "gl_FragColor[2] = tmp[2];\n"                                       \
    "gl_FragColor[3] = alpha;\n"                                        \
    "}\n"                                                               \
    "}\n"                                                               \
    "else if(shader_type == 2)"                                         \
    "{\n"                                                               \
    "gl_FragColor[3] = dist > shader_radius ? 1.0 : 0.0; \n"            \
    "}\n"                                                               \
    "else if(shader_type == 3)\n"                                       \
    "{\n"                                                               \
    "float alpha = (mod(floor(gl_FragCoord.x / shader_chessboard_size), 2.0) == 0.) != (mod(floor(gl_FragCoord.y / shader_chessboard_size), 2.0) == 1.) ? shader_alpha : 0.0;\n" \
    "gl_FragColor[3] = clamp(alpha,0.0,1.0);\n"                         \
    "}\n"                                                               \
    "else if(shader_type == 4)\n"                                       \
    "{\n"                                                               \
    "float alpha = (shader_vertical_slider_flag == 0 && gl_FragCoord.x > shader_slider_pos) || (shader_vertical_slider_flag == 1 && gl_FragCoord.y > shader_slider_pos) ? 1.0 : 0.0;\n" \
    "gl_FragColor[3] = clamp(alpha,0.0,1.0);\n"                         \
    "}\n"                                                               \
    "else if(shader_type == 5)\n"                                       \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "float angle = acos(clamp(dot(vec3(p),shader_current)/(length(vec3(p))*length(shader_current)),-1.0,1.0));\n" \
    "vec3 tmp = clamp(vec3(1.-shader_spectral_angle_range*abs(angle)/3.142),0.0,1.0);\n" \
    "gl_FragColor[0] = tmp[0];\n"                                       \
    "gl_FragColor[1] = tmp[1];\n"                                       \
    "gl_FragColor[2] = tmp[2];\n"                                       \
    "gl_FragColor[3] = alpha;\n"                                        \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
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
    "gl_FragColor[0] = mapped[0];\n"                                    \
    "gl_FragColor[1] = mapped[1];\n"                                    \
    "gl_FragColor[2] = mapped[2];\n"                                    \
    "}\n"                                                               \
    "}\n";
  
  if(isGLSLS140Available)
    {
    shader_source+=
    "else if(shader_type == 6)\n"                                       \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "vec2 size = vec2(textureSize(src,0));\n"                           \
    "vec2 dx = vec2(gl_TexCoord[0].xy);\n"                              \
    "dx[0]+=1.0/size[0];\n"                                             \
    "vec2 dy = vec2(gl_TexCoord[0].xy);\n"                              \
    "dy[1]+=1.0/size[1];\n"                                             \
    "vec4 pdx = texture2D(src, dx);\n"                                  \
    "vec4 pdy = texture2D(src, dy);\n"                                  \
    "gl_FragColor = clamp(pow(5*shader_a*(0.5*abs((pdx-p))+ 0.5*abs((pdy-p))),shader_gamma),0.0,1.0);\n" \
    "gl_FragColor[3] = alpha;\n"                                        \
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

  GLint shader_a = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_a");
  glUniform4f(shader_a,scr,scg,scb,1.);

  GLint shader_b = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_b");
  glUniform4f(shader_b,shr,shg,shb,0);

  GLint shader_use_no_data = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_use_no_data");
  glUniform1i(shader_use_no_data, m_ImageSettings->GetUseNoData()  );

  GLint shader_no_data = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_no_data");
  glUniform1f( shader_no_data, m_ImageSettings->GetNoData() );

  GLint shader_gamma = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_gamma");
  glUniform4f( shader_gamma, gamma, gamma, gamma, gamma );

  GLint shader_alpha = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_alpha");
  glUniform1f( shader_alpha, m_ImageSettings->GetAlpha() );

  GLint shader_radius = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_radius");
  glUniform1f(shader_radius,m_Radius);

  GLint shader_center = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_center");
  glUniform2f(shader_center,m_Center[0],m_Center[1]);
  
  GLint shader_type = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_type");
  glUniform1i(shader_type,m_ShaderType);

  // std::cout
  //   << "r: " << m_ImageSettings->GetCurrentRed()
  //   << " g: " << m_ImageSettings->GetCurrentGreen()
  //   << " b: " << m_ImageSettings->GetCurrentBlue()
  //   << std::endl;

  GLint shader_current = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_current");
  glUniform3f(
    shader_current,
    m_ImageSettings->GetCurrentRed(),
    m_ImageSettings->GetCurrentGreen(),
    m_ImageSettings->GetCurrentBlue()
  );

  GLint shader_localc_range = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_localc_range");
  glUniform1f(shader_localc_range,m_LocalContrastRange);

  GLint shader_spectral_angle_range = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_spectral_angle_range");
  glUniform1f(shader_spectral_angle_range,m_SpectralAngleRange);

  GLint shader_chessboard_size = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_chessboard_size");
  glUniform1f(shader_chessboard_size,m_ChessboardSize);

  GLint shader_slider_pos = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_slider_pos");
  glUniform1f(shader_slider_pos,m_SliderPosition);

  GLint shader_vertical_slider_flag = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_vertical_slider_flag");
  glUniform1i(shader_vertical_slider_flag,m_VerticalSlider);

}


} // End namespace otb

