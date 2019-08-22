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

#include "otbShaderRegistry.h"
#include <GL/glew.h>

namespace otb
{

ShaderRegistry::Pointer ShaderRegistry::m_Instance;

ShaderRegistry::ShaderRegistry()
  : m_ShaderMap()
{}

ShaderRegistry::~ShaderRegistry()
{
  this->ClearShaders();
}

ShaderRegistry::Pointer  ShaderRegistry::Instance()
{
  if(m_Instance.IsNull())
    {
    m_Instance = ShaderRegistry::New();
    GLenum err = glewInit();
    if (err != GLEW_OK)
      {
      itkGenericExceptionMacro(<< "Failed to initialize GLEW: "<< glewGetErrorString(err));
      }
    }

  return m_Instance;
}

void ShaderRegistry::RegisterShader(const std::string& name, const std::string& vSource, const std::string& fSource)
{
  if(m_ShaderMap.count(name) != 0)
    {
    itkExceptionMacro(<<"A shader with name "<<name<<" has already been registered!");
    }

  if(vSource.empty() && fSource.empty())
    {
    itkExceptionMacro(<<"No shader sources supplied!");
    }

  GLuint vShader = 0;
  if(!vSource.empty())
    {
    vShader = CompileShader(GL_VERTEX_SHADER, name, vSource);
    }

  GLuint fShader = 0;
  if(!fSource.empty())
    {
    fShader = CompileShader(GL_FRAGMENT_SHADER, name, fSource);
    }

  // create a new program for these shader objects
  GLuint program = glCreateProgram();

  // attach the shader objects to the current program, and flag them for
  // deletion, they will be actually destroyed when the program is deleted.
  if(vShader)
    {
    glAttachShader(program,vShader);
    glDeleteShader(vShader);
    }
  if(fShader)
    {
    glAttachShader(program,fShader);
    glDeleteShader(fShader);
    }
  glLinkProgram(program);

  m_ShaderMap[name] = program;
}

bool ShaderRegistry::UnregisterShader(const std::string& name)
{
  if(m_ShaderMap.count(name) != 0)
    {
    glDeleteProgram(m_ShaderMap[name]);
    m_ShaderMap.erase(name);
    return true;
    }
  return false;
}

bool ShaderRegistry::IsShaderRegistered(const std::string& name) const
{
  return (m_ShaderMap.count(name) != 0);
}

bool ShaderRegistry::LoadShader(const std::string& name)
{
  if(!IsShaderRegistered(name))
    {
    return false;
    }
  glUseProgram(m_ShaderMap[name]);
  return true;
}

void ShaderRegistry::UnloadShader()
{
  glUseProgram(0);
}

void ShaderRegistry::ClearShaders()
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

unsigned int ShaderRegistry::GetShaderProgram(const std::string& name)
{
  if(m_ShaderMap.count(name) == 0)
    {
    itkExceptionMacro(<<"No shader with name "<<name<<" has been registered.");
    }
  return m_ShaderMap[name];
}

unsigned int ShaderRegistry::CompileShader(int stype, const std::string& name, const std::string& src)
{
  GLuint shader = glCreateShader(stype);

  // compile the sources
  const char * source_cstr = src.c_str();
  glShaderSource(shader, 1, &source_cstr,nullptr);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if(!compiled)
    {
    // get the logs
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &length);
    char * logs = new char[length];
    glGetShaderInfoLog(shader,1000,&length,logs);
    std::string slogs = logs;
    delete [] logs;
    logs = nullptr;

    // Shader must be destroyed here!
    glDeleteShader( shader );
    shader = 0;

    itkExceptionMacro(<<"Shader "<<name<<" with sources "<<src<<" failed to compile: "<<slogs);
    }
  return shader;
}

}
