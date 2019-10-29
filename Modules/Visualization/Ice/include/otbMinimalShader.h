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

#ifndef otb_MinimalShader_h
#define otb_MinimalShader_h

#include <itkObjectFactory.h>
#include "otbShader.h"
#include <string>

namespace otb
{

class OTBIce_EXPORT MinimalShader
  : public Shader
{
public:
  typedef MinimalShader                                   Self;
  typedef Shader                                          Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef itk::Point<float,2>                             PointType;

  void SetupShader() override;

  itkNewMacro(Self);

  void SetColor(const double *rgb, const double *a);

protected:
  MinimalShader();

  ~MinimalShader() override;

  std::string GetVertexSource() const override;

  std::string GetFragmentSource() const override;

  std::string GetName() const override;

private:
  // prevent implementation
  MinimalShader(const Self&);
  void operator=(const Self&);

  const double *m_ColorRGB;
  const double *m_ColorA;
  
  struct UniformLocs
    {
    int proj;
    int modelview;
    int color;
    };

  UniformLocs m_Loc;
}; // End class MinimalShader

} // End namespace otb

#endif

