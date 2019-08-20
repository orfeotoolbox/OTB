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

#ifndef otb_Shader_h
#define otb_Shader_h

#include "OTBIceExport.h"

#include <itkObject.h>
#include <itkPoint.h>

namespace otb
{

class GlView;

class OTBIce_EXPORT Shader
  : public itk::Object
{
friend class GlView;
public:
  typedef Shader                                  Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef itk::Point<float,2>                             PointType;

  void LoadShader();

  virtual void SetupShader();

  void UnloadShader();

  //~ itkSetMacro(UL,PointType);
  //~ itkSetMacro(UR,PointType);
  //~ itkSetMacro(LL,PointType);
  //~ itkSetMacro(LR,PointType);
  //~ itkGetConstReferenceMacro(UL,PointType);
  //~ itkGetConstReferenceMacro(UR,PointType);
  //~ itkGetConstReferenceMacro(LL,PointType);
  //~ itkGetConstReferenceMacro(LR,PointType);

protected:
  Shader();

  ~Shader() override;

  /** Build the shader sources and fills m_Program */
  void BuildShader();

  virtual std::string GetVertexSource() const = 0;

  virtual std::string GetFragmentSource() const = 0;

  virtual std::string GetName() const = 0;

  /** Program object in which the shader is compiled and linked (0 if no program) */
  unsigned int m_Program;

  /** Projection matrix */
  const float * m_ProjMatrix;

  /** ModelView matrix */
  const float * m_ModelViewMatrix;

private:
  // prevent implementation
  Shader(const Self&);
  void operator=(const Self&);

  //~ PointType m_UL;
  //~ PointType m_UR;
  //~ PointType m_LL;
  //~ PointType m_LR;

}; // End class Shader

} // End namespace otb

#endif
