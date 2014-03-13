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
#include "otbFragmentShaderRegistry.h"
#include <GL/glew.h>

namespace otb
{

FragmentShaderRegistry::Pointer FragmentShaderRegistry::m_Instance;

FragmentShaderRegistry::FragmentShaderRegistry()
  : m_ShaderMap()
{}

FragmentShaderRegistry::~FragmentShaderRegistry()
{
  this->ClearShaders();
}

FragmentShaderRegistry::Pointer  FragmentShaderRegistry::Instance()
{
  if(m_Instance.IsNull())
    {
    m_Instance = FragmentShaderRegistry::New();
    glewInit();
    }

  return m_Instance;
}

void FragmentShaderRegistry::RegisterShader(const std::string& name, const std::string& source)
{
  if(m_ShaderMap.count(name) != 0)
    {
    itkExceptionMacro(<<"A shader with name "<<name<<" has already been registered!");
    }

  GLint source_length = static_cast<GLint>(source.size());

  GLuint program = glCreateProgram();

  GLuint shader  = glCreateShader(GL_FRAGMENT_SHADER);
  
  const char * source_cstr = source.c_str();
  
  glShaderSource(shader, 1, &source_cstr,NULL);
  glCompileShader(shader);

  GLint compiled;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  
  if(!compiled)
    {    
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &length);
    
    char * logs = new char[length];
    glGetShaderInfoLog(shader,1000,&length,logs);

    itkExceptionMacro(<<"Shader "<<name<<" with sources "<<source<<" failed to compile: "<<logs);

    delete [] logs;
    // For safety!
    logs = NULL;

    // Shader must be destroyed here!
    glDeleteShader( shader );
    shader = 0;

    // Program must be destroyed here!
    glDeleteProgram( program );
    program = 0;
    }

  glAttachShader(program,shader);
  glLinkProgram(program);

  m_ShaderMap[name] = std::make_pair(program,shader);
}

bool FragmentShaderRegistry::UnregisterShader(const std::string& name)
{
  if(m_ShaderMap.count(name) != 0)
    {
    glDeleteProgram(m_ShaderMap[name].first);
    glDeleteShader(m_ShaderMap[name].second);
    m_ShaderMap.erase(name);
    return true;
    }
  return false;
}

bool FragmentShaderRegistry::IsShaderRegistered(const std::string& name) const
{
  return (m_ShaderMap.count(name) != 0);
}

bool FragmentShaderRegistry::LoadShader(const std::string& name)
{
  if(!IsShaderRegistered(name))
    {
    return false;
    }
  glUseProgramObjectARB(m_ShaderMap[name].first);
  return true;
}

void FragmentShaderRegistry::UnloadShader()
{
  glUseProgramObjectARB(0);
}

void FragmentShaderRegistry::ClearShaders()
{
  std::vector<std::string> keys;

  for(ShaderMapType::iterator it = m_ShaderMap.begin();
      it!=m_ShaderMap.end();++it)
    {
    keys.push_back(it->first);
    }

  for(std::vector<std::string>::iterator it = keys.begin();
      it!=keys.end();++it)
    {
    UnregisterShader(*it);
    }

  m_ShaderMap.clear();
}

unsigned int FragmentShaderRegistry::GetShaderProgram(const std::string& name)
{
  if(m_ShaderMap.count(name) == 0)
    {
    itkExceptionMacro(<<"No shader with name "<<name<<" has been registered.");
    }
  return m_ShaderMap[name].first;
}

}
