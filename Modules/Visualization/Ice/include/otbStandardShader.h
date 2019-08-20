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

#ifndef otb_StandardShader_h
#define otb_StandardShader_h

#include <itkObjectFactory.h>
#include "otbShader.h"
#include "otbImageSettings.h"
#include <string>

namespace otb
{

typedef enum
{
  SHADER_STANDARD,
  SHADER_LOCAL_CONTRAST,
  SHADER_LOCAL_ALPHA,
  SHADER_ALPHA_GRID,
  SHADER_ALPHA_SLIDER,
  SHADER_SPECTRAL_ANGLE,
  SHADER_GRADIENT,
  SHADER_LUT_JET,
  SHADER_LUT_LOCAL_JET,
  SHADER_LUT_HOT,
  SHADER_LUT_LOCAL_HOT,
  SHADER_LUT_WINTER,
  SHADER_LUT_LOCAL_WINTER,
  SHADER_LUT_SUMMER,
  SHADER_LUT_LOCAL_SUMMER,
  SHADER_LUT_COOL,
  SHADER_LUT_LOCAL_COOL
} ShaderType;


class OTBIce_EXPORT StandardShader
  : public Shader
{
public:
  typedef StandardShader                                  Self;
  typedef Shader                                  Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef itk::Point<float,2>                             PointType;

  itkSetObjectMacro( ImageSettings, ImageSettings );
  itkGetObjectMacro( ImageSettings, ImageSettings );
  itkGetConstObjectMacro( ImageSettings, ImageSettings );

  itkSetMacro(LocalContrastRange,double);
  itkGetMacro(LocalContrastRange,double);

  itkSetMacro(SpectralAngleRange,double);
  itkGetMacro(SpectralAngleRange,double);

  itkSetMacro(ShaderType,ShaderType);
  itkGetMacro(ShaderType,ShaderType);

  itkSetMacro(Radius,double);
  itkGetConstReferenceMacro(Radius,double);

  itkSetMacro(ChessboardSize,double);
  itkGetConstReferenceMacro(ChessboardSize,double);

  itkSetMacro(SliderPosition,double);
  itkGetConstReferenceMacro(SliderPosition,double);

  itkSetMacro(VerticalSlider,bool);
  itkGetMacro(VerticalSlider,bool);

  itkSetMacro(Center,PointType);
  itkGetConstReferenceMacro(Center,PointType);

  void SetupShader() override;

  itkNewMacro(Self);

protected:
  StandardShader();

  ~StandardShader() override;

  std::string GetSource() const override;

  std::string GetName() const override;

private:
  // prevent implementation
  StandardShader(const Self&);
  void operator=(const Self&);

  ImageSettings::Pointer m_ImageSettings;

  double m_LocalContrastRange;
  double m_SpectralAngleRange;

  PointType m_Center;

  double m_Radius;

  double m_ChessboardSize;

  double m_SliderPosition;

  bool m_VerticalSlider;

  ShaderType m_ShaderType;
}; // End class StandardShader

} // End namespace otb

#endif
