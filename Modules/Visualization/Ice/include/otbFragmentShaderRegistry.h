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

#ifndef otb_FragmentShaderRegistry_h
#define otb_FragmentShaderRegistry_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

class FragmentShaderRegistry : public itk::Object
{
public:
  typedef FragmentShaderRegistry        Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  static Pointer Instance();

  void RegisterShader(const std::string& name, const std::string& source);

  bool UnregisterShader(const std::string& name);

  bool IsShaderRegistered(const std::string& name) const;

  bool LoadShader(const std::string& name);

  void UnloadShader();

  void ClearShaders();

  unsigned int GetShaderProgram(const std::string& name);

protected:
  FragmentShaderRegistry();

  ~FragmentShaderRegistry() override;

private:
  typedef std::map<std::string, std::pair<unsigned int, unsigned int>> ShaderMapType;

  itkNewMacro(Self);

  // prevent implementation
  FragmentShaderRegistry(const Self&);
  void operator=(const Self&);

  static Pointer m_Instance;

  ShaderMapType m_ShaderMap;

}; // End class FragmentShaderRegistry

} // End namespace otb

#endif
