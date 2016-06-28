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
  const char * glVersion = ITK_NULLPTR;
  const char * glslVersion = ITK_NULLPTR;
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

  shader_source = 
    "#define PI 3.1415926535897932384626433832795\n"			\
    "\n"								\
    "uniform int pixel_type;\n"						\
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
    "\n"								\
    "float decibels( float x )\n"					\
    "{\n"								\
    "  return 10.0f * log( x ) / log( 10.0f );\n"			\
    "}\n"								\
    "\n"								\
    "float phasis( float x, float y )\n"				\
    "{\n"								\
    "  return atan( y / x );\n"						\
    "}\n"								\
    "\n"								\
    "vec3 modulus( vec3 v )\n"						\
    "{\n"								\
    "  v.r = v.g = v.b = length( vec2( v ) );\n"			\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec3 phasis( vec3 v )\n"						\
    "{\n"								\
    "  v.r = v.g = v.b = phasis( v.x, v.y );\n"				\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec4 modulus( vec4 v )\n"						\
    "{\n"								\
    "  v.r = v.g = v.b = length( vec2( v ) );\n"			\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec4 phasis( vec4 v )\n"						\
    "{\n"								\
    "  v.r = v.g = v.b = phasis( v.x, v.y );\n"				\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec3 amplitude_db( vec3 v )\n"					\
    "{\n"								\
    "  v.r = v.g = v.b = decibels( length( vec2( v ) ) );\n"		\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec3 intensity_db( vec3 v )\n"					\
    "{\n"								\
    "  v.r = v.g = v.b = decibels( v.x * v.x + v.y * v.y );\n"		\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec4 amplitude_db( vec4 v )\n"					\
    "{\n"								\
    "  v.r = v.g = v.b = decibels( length( vec2( v ) ) );\n"		\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "vec4 intensity_db( vec4 v )\n"					\
    "{\n"								\
    "  v.r = v.g = v.b = decibels( v.x * v.x + v.y * v.y );\n"		\
    "  return v;\n"							\
    "}\n"								\
    "\n"								\
    "void main (void) {\n"                                              \
    "vec4 p = texture2D(src, gl_TexCoord[0].xy);\n"                     \
    "vec4 p2 = p;\n"							\
    "vec3 p_current = shader_current;\n"				\
    "// Modulus (complex)\n"						\
    "if( pixel_type==1 )\n"						\
    "{\n"								\
    "  p2 = modulus( p );\n"						\
    "\n"								\
    "  p_current = modulus( shader_current );\n"			\
    "}\n"								\
    "// Phasis (complex)\n"						\
    "else if( pixel_type==2 )\n"					\
    "{\n"								\
    "  p2 = phasis( p );\n"						\
    "\n"								\
    "  p_current = phasis( shader_current );\n"				\
    "}\n"								\
    "// Amplitude dB (complex)\n"					\
    "else if( pixel_type==3 )\n"					\
    "{\n"								\
    "  p2 = amplitude_db( p );\n"					\
    "\n"								\
    "  p_current = amplitude_db( shader_current );\n"			\
    "}\n"								\
    "// Intensity dB (complex)\n"					\
    "else if( pixel_type==4 )\n"					\
    "{\n"								\
    "  p2 = intensity_db( p );\n"					\
    "\n"								\
    "  p_current = intensity_db( shader_current );\n"			\
    "}\n"								\
    "// Dynamics\n"							\
    "gl_FragColor =\n"							\
    "  pow( clamp( ( p2 + shader_b ) * shader_a, 0.0, 1.0 ), shader_gamma );\n" \
    "gl_FragColor[3] = clamp(shader_alpha, 0.0, 1.0);\n"		\
    "if(shader_use_no_data > 0 && vec3(p) == vec3(shader_no_data)){\n"  \
    "gl_FragColor[3] = 0.;\n"                                           \
    "}\n"                                                               \
    "float alpha = gl_FragColor[3];\n"                                  \
    "float dist = distance(gl_FragCoord.xy, shader_center);\n" \
    "if(shader_type == 1)\n"                                            \
    "{\n"                                                               \
    "if(dist < shader_radius)\n"                                        \
    "{\n"                                                               \
    "vec3 tmp = clamp((vec3(p2)-vec3(p_current)+vec3(shader_localc_range))/(2.*vec3(shader_localc_range)),0.0,1.0);\n" \
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
    "float angle = acos(clamp(dot(vec3(p2),p_current)/(length(vec3(p2))*length(p_current)),-1.0,1.0));\n" \
    "vec3 tmp = clamp(vec3(1.-shader_spectral_angle_range*abs(angle)/PI), 0.0, 1.0);\n" \
    "gl_FragColor[0] = tmp[0];\n"                                       \
    "gl_FragColor[1] = tmp[1];\n"                                       \
    "gl_FragColor[2] = tmp[2];\n"                                       \
    "gl_FragColor[3] = alpha;\n"                                        \
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
  FragmentShaderRegistry::Pointer registry( otb::FragmentShaderRegistry::Instance() );
  assert( !registry.IsNull() );

  assert( !m_ImageSettings.IsNull() );

#define DECIBEL( x ) ( 10.0 * log10( x ) )

  //
  // Apply logarithmic dB scale.
  double minRGB[] =
    {
      m_ImageSettings->GetMinRed(),
      m_ImageSettings->GetMinGreen(),
      m_ImageSettings->GetMinBlue()
    };

  double maxRGB[] =
    {
      m_ImageSettings->GetMaxRed(),
      m_ImageSettings->GetMaxGreen(),
      m_ImageSettings->GetMaxBlue()
    };

#if 0
  switch( m_ImageSettings->GetPixelType() )
    {
    case ImageSettings::PIXEL_TYPE_AMPLITUDE_DB :
      assert( minRGB[ 0 ]==minRGB[ 1 ] && minRGB[ 1 ]==minRGB[ 2 ] );
      assert( maxRGB[ 0 ]==maxRGB[ 1 ] && maxRGB[ 1 ]==maxRGB[ 2 ] );

      minRGB[ 0 ] =
      minRGB[ 1 ] =
      minRGB[ 2 ] =
	DECIBEL( minRGB[ 0 ] );

      maxRGB[ 0 ] =
      maxRGB[ 1 ] =
      maxRGB[ 2 ] =
	DECIBEL( maxRGB[ 0 ] );
      break;

    case ImageSettings::PIXEL_TYPE_INTENSITY_DB :
      assert( minRGB[ 0 ]==minRGB[ 1 ] && minRGB[ 1 ]==minRGB[ 2 ] );
      assert( maxRGB[ 0 ]==maxRGB[ 1 ] && maxRGB[ 1 ]==maxRGB[ 2 ] );

      minRGB[ 0 ] =
      minRGB[ 1 ] =
      minRGB[ 2 ] =
	DECIBEL( minRGB[ 0 ] * minRGB[ 0 ] );

      maxRGB[ 0 ] =
      maxRGB[ 1 ] =
      maxRGB[ 2 ] =
	DECIBEL( maxRGB[ 0 ] * maxRGB[ 0 ] );
      break;

    default:
      break;
    }
#endif

  //
  // Compute shifts.
  double shr = -minRGB[ 0 ];
  double shg = -minRGB[ 1 ];
  double shb = -minRGB[ 2 ];

  //
  // Compute scales.
  double scr = 1.0 / ( maxRGB[ 0 ] + shr );
  double scg = 1.0 / ( maxRGB[ 1 ] + shg );
  double scb = 1.0 / ( maxRGB[ 2 ] + shb );

  double gamma = m_ImageSettings->GetGamma();

  GLint pixel_type =
    glGetUniformLocation( registry->GetShaderProgram( "StandardShader" ), "pixel_type" );

  glUniform1i( pixel_type, m_ImageSettings->GetPixelType() );

  GLint shader_a = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_a");
  glUniform4f(shader_a,scr,scg,scb,1.);

  GLint shader_b = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_b");
  glUniform4f(shader_b,shr,shg,shb,0);

  GLint shader_use_no_data = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_use_no_data");
  glUniform1i(shader_use_no_data, m_ImageSettings->GetUseNoData()  );

  GLint shader_no_data = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_no_data");
  glUniform1f( shader_no_data, m_ImageSettings->GetNoData() );

  GLint shader_gamma = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_gamma");
  glUniform4f( shader_gamma, gamma, gamma, gamma, gamma );

  GLint shader_alpha = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_alpha");
  glUniform1f( shader_alpha, m_ImageSettings->GetAlpha() );

  GLint shader_radius = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_radius");
  glUniform1f(shader_radius,m_Radius);

  GLint shader_center = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_center");
  glUniform2f(shader_center,m_Center[0],m_Center[1]);
  
  GLint shader_type = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_type");
  glUniform1i(shader_type,m_ShaderType);

  // std::cout
  //   << "r: " << m_ImageSettings->GetCurrentRed()
  //   << " g: " << m_ImageSettings->GetCurrentGreen()
  //   << " b: " << m_ImageSettings->GetCurrentBlue()
  //   << std::endl;

  GLint shader_current = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_current");
  glUniform3f(
    shader_current,
    m_ImageSettings->GetCurrentRed(),
    m_ImageSettings->GetCurrentGreen(),
    m_ImageSettings->GetCurrentBlue()
  );

  GLint shader_localc_range = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_localc_range");
  glUniform1f(shader_localc_range,m_LocalContrastRange);

  GLint shader_spectral_angle_range = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_spectral_angle_range");
  glUniform1f(shader_spectral_angle_range,m_SpectralAngleRange);

  GLint shader_chessboard_size = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_chessboard_size");
  glUniform1f(shader_chessboard_size,m_ChessboardSize);

  GLint shader_slider_pos = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_slider_pos");
  glUniform1f(shader_slider_pos,m_SliderPosition);

  GLint shader_vertical_slider_flag = glGetUniformLocation(registry->GetShaderProgram("StandardShader"), "shader_vertical_slider_flag");
  glUniform1i(shader_vertical_slider_flag,m_VerticalSlider);

}


} // End namespace otb

