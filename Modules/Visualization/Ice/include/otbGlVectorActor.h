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

#ifndef otb_GlVectorActor_h
#define otb_GlVectorActor_h


#include "otbGenericRSTransform.h"
#include "otbGeoInterface.h"
#include "otbGlActor.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbOGRLayerWrapper.h"

#include "itkCenteredRigid2DTransform.h"
#include "itkVector.h"
#include <string>


// Forward declaration of GLUtesselator
class GLUtesselator;

namespace otb
{

class OTBIce_EXPORT GlVectorActor 
  : public GlActor, public GeoInterface
{
public:
  typedef GlVectorActor                                   Self;
  typedef GlActor                                         Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  typedef otb::GenericRSTransform<>                       RSTransformType;
  typedef RSTransformType::InputPointType                 PointType;
  typedef itk::Vector<double,3>                           ColorType;
  typedef itk::CenteredRigid2DTransform<>                 RigidTransformType;

  itkNewMacro(Self);
  
  // Initialize with a new file
  void Initialize(const std::string & filename, const std::string & layername = "");

  // Get all available layers
  std::vector<std::string> GetAvailableLayers() const;

  // Get layer name
  std::string GetCurrentLayer() const;

  // Set layer name
  void SetCurrentLayer(const std::string & layername);

  // Retrieve the full extent of the actor
  void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const override;

  // Return actor extent in its own geometry
  void GetBoundingBox(double & ulx, double & uly, double & lrx, double & lry) const;

  // Update internal actor state with respect to ViewSettings
  void ProcessViewSettings() override;

  // Heavy load/unload operations of data
  void UpdateData() override;

  // Gl rendering of current state
  void Render() override;

  void CreateShader() override;

  PointType ViewportToVectorTransform(const PointType & point) const;

  itkSetMacro(Color,ColorType);
  itkGetConstReferenceMacro(Color,ColorType);
  
  itkSetMacro(Alpha,double);
  itkGetConstReferenceMacro(Alpha,double);

  void SetFill(bool flag);
  itkGetMacro(Fill,bool);
  itkBooleanMacro(Fill);

  void SetSolidBorder(bool flag);
  itkGetMacro(SolidBorder,bool);
  itkBooleanMacro(SolidBorder);

  void SetOptimizedRendering(bool flag);
  itkGetMacro(OptimizedRendering,bool);
  itkBooleanMacro(OptimizedRendering);
  
  itkGetMacro(OptimizedRenderingActive,bool);

  itkSetMacro(LineWidth,double);
  itkGetConstReferenceMacro(LineWidth,double);

  std::string GetWkt() const override;

  //
  // otb::GlActor overloads.
  //

  bool TransformFromViewport( Point2d & out,
                                      const Point2d & in,
                                      bool isPhysical = true ) const override;


protected:
  GlVectorActor();
  
  ~GlVectorActor() override;


  // Internal class to hold tiles
  class InternalFeature
  {
  public:
    InternalFeature(OGRFeatureDefn & definition)
      : m_SourceFeature(definition),
        m_RenderedFeature(definition)
    {}

    otb::ogr::Feature m_SourceFeature;
    otb::ogr::Feature m_RenderedFeature;
  };
  
private:
  // prevent implementation
  GlVectorActor(const Self&);
  void operator=(const Self&);

  void InternalFeaturesTransform();

  void UpdateTransforms();

  void UpdateDisplayList();

  ColorType            m_Color;
  double               m_Alpha;
  bool                 m_Fill;
  bool                 m_SolidBorder;
  double               m_LineWidth;
  
  RSTransformType::Pointer m_ViewportToVectorTransform;
  RSTransformType::Pointer m_VectorToViewportTransform;

  otb::ogr::DataSource::Pointer m_OGRDataSource;
  std::string                   m_CurrentLayer;

  std::vector<InternalFeature> m_InternalFeatures;

  GLUtesselator * m_GluTesselator;

  unsigned int m_DisplayList;
  bool m_DisplayListNeedsRebuild;

  double m_ExtentULX;
  double m_ExtentULY;
  double m_ExtentLRX;
  double m_ExtentLRY;

  bool m_OptimizedRendering;
  bool m_OptimizedRenderingActive;

  unsigned int m_PointMarkerSize;

  RigidTransformType::Pointer m_ViewportForwardRotationTransform;
  RigidTransformType::Pointer m_ViewportBackwardRotationTransform;

  int m_ColorIdx;

}; // End class GlVectorActor

} // End namespace otb

#endif
