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

#include "otbShader.h"
#include "otbShaderRegistry.h"

namespace otb
{
Shader::Shader()
  : m_Program(0)
  , m_ProjMatrix(nullptr)
  , m_ModelViewMatrix(nullptr)
{}

Shader::~Shader()
{}

void Shader::BuildShader()
{
  std::string vSource = this->GetVertexSource();
  std::string fSource = this->GetFragmentSource();
  std::string name = this->GetName();
  
  try
    {
    // Assumption here is that each shader has its unique name
    if(!otb::ShaderRegistry::Instance()->IsShaderRegistered(name))
      {
      otb::ShaderRegistry::Instance()->RegisterShader(name,vSource,fSource);
      }
    m_Program = otb::ShaderRegistry::Instance()->GetShaderProgram(name);
    }
  catch(itk::ExceptionObject& err)
    {
    // Log compilation errors if any
    std::cerr<<err<<std::endl;
    }
}

void Shader::LoadShader()
{
  otb::ShaderRegistry::Instance()->LoadShader(GetName());
}

void Shader::UnloadShader()
{
  otb::ShaderRegistry::Instance()->UnloadShader();
}

void Shader::SetupShader()
{
  // Default does nothing
}

const std::vector<int> &
Shader::GetAttribIdx()
{
  return m_AttribIdx;
}

}
