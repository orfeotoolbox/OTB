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
#include <GL/glew.h>

namespace otb
{

StandardShader::StandardShader()
  : m_MinRed(0),
    m_MinGreen(0),
    m_MinBlue(0),
    m_MaxRed(255),
    m_MaxGreen(255),
    m_MaxBlue(255),
    m_Gamma(1.),
    m_Alpha(1.)
{
  this->BuildShader();
}

StandardShader::~StandardShader()
{}

std::string StandardShader::GetSource() const
{
  return "#version 120 \n"                      \
    "uniform sampler2D src;\n"                  \
    "uniform vec4 shader_a;\n"                  \
    "uniform vec4 shader_b;\n"                  \
    "uniform vec4 shader_gamma;\n"              \
    "uniform float shader_alpha;\n"                     \
    "void main (void) {\n"                              \
    "vec4 p = texture2D(src, gl_TexCoord[0].xy);\n"                     \
    "gl_FragColor = clamp(pow((p + shader_b)*shader_a,shader_gamma), 0.0, 1.0);\n" \
    "gl_FragColor[3] = clamp(shader_alpha,0.0,1.0);\n"                                            \
    "}";
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
  GLint shader_gamma = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_gamma");
  glUniform4f(shader_gamma,m_Gamma,m_Gamma,m_Gamma,m_Gamma);

  GLint shader_alpha = glGetUniformLocation(otb::FragmentShaderRegistry::Instance()->GetShaderProgram("StandardShader"), "shader_alpha");
  glUniform1f(shader_alpha,m_Alpha);

}


} // End namespace otb

