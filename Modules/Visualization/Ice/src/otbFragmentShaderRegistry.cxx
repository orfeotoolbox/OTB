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

#include "otbFragmentShaderRegistry.h"
#include <GL/glew.h>

namespace otb
{

FragmentShaderRegistry::Pointer FragmentShaderRegistry::m_Instance;

FragmentShaderRegistry::FragmentShaderRegistry() : m_ShaderMap()
{
}

FragmentShaderRegistry::~FragmentShaderRegistry()
{
  this->ClearShaders();
}

FragmentShaderRegistry::Pointer FragmentShaderRegistry::Instance()
{
  if (m_Instance.IsNull())
  {
    m_Instance = FragmentShaderRegistry::New();
    glewInit();
  }

  return m_Instance;
}

void FragmentShaderRegistry::RegisterShader(const std::string& name, const std::string& source)
{
  if (m_ShaderMap.count(name) != 0)
  {
    itkExceptionMacro(<< "A shader with name " << name << " has already been registered!");
  }

  GLuint program = glCreateProgram();

  GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

  const char* source_cstr = source.c_str();

  glShaderSource(shader, 1, &source_cstr, nullptr);
  glCompileShader(shader);

  GLint compiled;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char* logs = new char[length];
    glGetShaderInfoLog(shader, 1000, &length, logs);

    std::string slogs = logs;

    delete[] logs;
    // For safety!
    logs = nullptr;

    // Shader must be destroyed here!
    glDeleteShader(shader);
    shader = 0;

    // Program must be destroyed here!
    glDeleteProgram(program);
    program = 0;

    itkExceptionMacro(<< "Shader " << name << " with sources " << source << " failed to compile: " << slogs);
  }

  glAttachShader(program, shader);
  glLinkProgram(program);

  m_ShaderMap[name] = std::make_pair(program, shader);
}

bool FragmentShaderRegistry::UnregisterShader(const std::string& name)
{
  if (m_ShaderMap.count(name) != 0)
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
  if (!IsShaderRegistered(name))
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

  for (ShaderMapType::iterator it = m_ShaderMap.begin(); it != m_ShaderMap.end(); ++it)
  {
    keys.push_back(it->first);
  }

  for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); ++it)
  {
    UnregisterShader(*it);
  }

  m_ShaderMap.clear();
}

unsigned int FragmentShaderRegistry::GetShaderProgram(const std::string& name)
{
  if (m_ShaderMap.count(name) == 0)
  {
    itkExceptionMacro(<< "No shader with name " << name << " has been registered.");
  }
  return m_ShaderMap[name].first;
}
}
