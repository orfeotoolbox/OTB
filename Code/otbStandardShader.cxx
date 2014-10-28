/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbStandardShader.h"
#include "otbFragmentShaderRegistry.h"
#include "otbGlVersionChecker.h"
#include <GL/glew.h>

namespace otb
{

StandardShader::StandardShader()
  : m_MinRed(0),
    m_MaxRed(255),
    m_MinGreen(0),
    m_MaxGreen(255),
    m_MinBlue(0),
    m_MaxBlue(255),
    m_UseNoData(true),
    m_NoData(0),
    m_Gamma(1.),
    m_Alpha(1.),
    m_CurrentRed(0),
    m_CurrentGreen(0),
    m_CurrentBlue(0),
    m_LocalContrastRange(50),
    m_SpectralAngleRange(10),
    m_Center(),
    m_Radius(200),
    m_ChessboardSize(256),
    m_SliderPosition(500),
    m_VerticalSlider(false),
    m_ShaderType(SHADER_STANDARD)
{
  this->BuildShader();
}

StandardShader::~StandardShader()
{}

std::string StandardShader::GetSource() const
{
  const char * glVersion = NULL;
  const char * glslVersion = NULL;
  if(!otb::GlVersionChecker::CheckGLCapabilities( glVersion, glslVersion))
    {
    itkExceptionMacro(<<" Required GL and GLSL versions were not found (GL version is "<<glVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GL_VERSION<<", GLSL version is "<<glslVersion<<", should be at least "<<otb::GlVersionChecker::REQUIRED_GLSL_VERSION<<")");
    }

  bool isGLSLS130Available  = otb::GlVersionChecker::VerCmp(glslVersion,"1.30")>=0;

  std::string shader_source = "";

  if(isGLSLS130Available)
    {
    shader_source+="#version 130 \n";
    }
  else
    {
    shader_source+="#version 120 \n";
    }

  shader_source = 
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
    "gl_FragColor = clamp(pow((p+shader_b)*shader_a,shader_gamma), 0.0, 1.0);\n" \
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
    "}\n";
  
  if(isGLSLS130Available)
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
// Compute shifts and scales
  double shr,shg,shb,scr,scg,scb;
  shr = -m_MinRed;
  shg = -m_MinGreen;
  shb = -m_MinBlue;
  scr = 1./(m_MaxRed-m_MinRed);
  scg = 1./(m_MaxGreen-m_MinGreen);
  scb = 1./(m_MaxBlue-m_MinBlue);
  
  GLint shader_a = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_a");
  glUniform4f(shader_a,scr,scg,scb,1.);

  GLint shader_b = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_b");
  glUniform4f(shader_b,shr,shg,shb,0);

  GLint shader_use_no_data = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_use_no_data");
  glUniform1i(shader_use_no_data,m_UseNoData);

  GLint shader_no_data = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_no_data");
  glUniform1f(shader_no_data,m_NoData);

  GLint shader_gamma = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_gamma");
  glUniform4f(shader_gamma,m_Gamma,m_Gamma,m_Gamma,m_Gamma);

  GLint shader_alpha = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_alpha");
  glUniform1f(shader_alpha,m_Alpha);

  GLint shader_radius = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_radius");
  glUniform1f(shader_radius,m_Radius);

  GLint shader_center = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_center");
  glUniform2f(shader_center,m_Center[0],m_Center[1]);
  
  GLint shader_type = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_type");
  glUniform1i(shader_type,m_ShaderType);

  GLint shader_current = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_current");
  glUniform3f(shader_current,m_CurrentRed,m_CurrentGreen,m_CurrentBlue);

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

