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

#ifndef otb_GlROIActor_h
#define otb_GlROIActor_h

#include "otbGlActor.h"

#include "otbGenericRSTransform.h"
#include "otbImageKeywordlist.h"
#include "itkImageRegion.h"
#include "itkVector.h"
#include <string>

namespace otb
{

namespace gl
{
struct Mesh;
}

class OTBIce_EXPORT GlROIActor 
  : public GlActor
{
public:
  typedef GlROIActor                                Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  typedef ImageKeywordlist                                ImageKeywordlistType;
  typedef itk::ImageRegion<2>                             ImageRegionType;
  typedef otb::GenericRSTransform<>                       RSTransformType;
  typedef RSTransformType::InputPointType                 PointType;
  typedef itk::Vector<double,3>                           ColorType;

  itkNewMacro(Self);

  // Retrieve the full extent of the actor
  void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const override;

  // Update internal actor state with respect to ViewSettings
  void ProcessViewSettings() override;

  // Heavy load/unload operations of data
  void UpdateData() override;

  // Gl rendering of current state
  void Render() override;

  void CreateShader() override;

  void SetUL( const PointType & );

  itkGetConstReferenceMacro(UL,PointType);

  void SetLR( const PointType & );

  itkGetConstReferenceMacro(LR,PointType);

  itkSetStringMacro(Wkt);
  itkGetStringMacro(Wkt);

  void SetKwl(const ImageKeywordlistType & kwl);
  itkGetConstReferenceMacro(Kwl,ImageKeywordlistType);
  
  itkSetMacro(Color,ColorType);
  itkGetConstReferenceMacro(Color,ColorType);
  
  itkSetMacro(Alpha,double);
  itkGetConstReferenceMacro(Alpha,double);

  itkSetMacro(Fill,bool);
  itkGetMacro(Fill,bool);
  itkBooleanMacro(Fill);

protected:
  GlROIActor();
  
  ~GlROIActor() override;
  
private:
  // prevent implementation
  GlROIActor(const Self&);
  void operator=(const Self&);

  void UpdateTransforms();
 
  PointType            m_UL;
  PointType            m_LR;

  std::string          m_Wkt;
  ImageKeywordlistType m_Kwl;

  ColorType            m_Color;
  double               m_Alpha;
  double               m_CurrentAlpha;
  bool                 m_Fill;
  
  RSTransformType::Pointer m_ViewportToImageTransform;
  RSTransformType::Pointer m_ImageToViewportTransform;

  PointType            m_VpUL;
  PointType            m_VpUR;
  PointType            m_VpLL;
  PointType            m_VpLR;

  /** OpenGL quad. */
  std::unique_ptr< gl::Mesh > m_Mesh;

}; // End class GlROIActor

} // End namespace otb

#endif
