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
#include "otbViewSettings.h"
#include "otbGenericRSTransform.h"
#include "otbImageToGenericRSOutputParameters.h"
#include "otbMapProjections.h"

namespace otb
{
ViewSettings::ViewSettings()
{
  m_Origin.Fill(0);
  m_Spacing.Fill(1);
  m_ViewportSize.Fill(0);
  m_Wkt="";
  m_UseProjection = false;
  m_GeometryChanged = false;
  m_RotationCenter.Fill(0);
  m_RotationAngle = 0;
}

ViewSettings::~ViewSettings()
{}

void ViewSettings::GetViewportExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  ulx = m_Origin[0];
  uly = m_Origin[1];
  lrx = m_Origin[0] + m_Spacing[0]*m_ViewportSize[0];
  lry = m_Origin[1] + m_Spacing[1]*m_ViewportSize[1];
}

void ViewSettings::ScreenToViewPortTransform(const double & xscreen, const double & yscreen, double & xvp, double & yvp) const
{
  xvp = m_Origin[0] + xscreen * m_Spacing[0];
  yvp = m_Origin[1] + yscreen * m_Spacing[1];
}

void ViewSettings::ViewportToScreenTransform(const double & xvp, const double & yvp, double & xscreen, double & yscreen) const
{
  xscreen = (xvp - m_Origin[0]) / m_Spacing[0];
  yscreen = (yvp - m_Origin[1]) / m_Spacing[1];
}

void ViewSettings::Zoom(const PointType & zoomCenter,double scale)
{ 
  m_Origin[0] = m_Origin[0] + (zoomCenter[0]-m_Origin[0])*(1-scale);
  m_Origin[1] = m_Origin[1] + (zoomCenter[1]-m_Origin[1])*(1-scale);
  
  m_Spacing[0]*=scale;
  m_Spacing[1]*=scale;
}

void ViewSettings::Center(const PointType & center)
{
  m_Origin[0] = center[0] - 0.5*m_Spacing[0]*m_ViewportSize[0];
  m_Origin[1] = center[1] - 0.5*m_Spacing[1]*m_ViewportSize[1];
}

ViewSettings::PointType ViewSettings::GetViewportCenter() const
{
  PointType center = m_Origin;
  center[0]+=0.5*m_Spacing[0]*m_ViewportSize[0];
  center[1]+=0.5*m_Spacing[1]*m_ViewportSize[1];
  return center;
}

void ViewSettings::SetPersepectiveAngle()
{
  typedef otb::GenericRSTransform<double,3,3> RSTransformType;
  // Build the RS transform
  RSTransformType::Pointer forwardTransform = RSTransformType::New();
  forwardTransform->SetInputKeywordList(m_KeywordList);
  forwardTransform->SetOutputProjectionRef(m_Wkt);
  forwardTransform->InstanciateTransform();

  RSTransformType::Pointer inverseTransform = RSTransformType::New();
  inverseTransform->SetInputProjectionRef(m_Wkt);
  inverseTransform->SetOutputKeywordList(m_KeywordList);
  inverseTransform->InstanciateTransform();

  PointType centerPoint = GetViewportCenter();

  // Find the rotation to apply
  RSTransformType::InputPointType centerPoint3d;
  centerPoint3d[0] = centerPoint[0];
  centerPoint3d[1] = centerPoint[1];
  centerPoint3d[2] = 0;

  RSTransformType::OutputPointType groundPoint3d1 = forwardTransform->TransformPoint(centerPoint3d);

  groundPoint3d1[2] = 1000;

  RSTransformType::InputPointType centerPoint3d2 = inverseTransform->TransformPoint(groundPoint3d1);

  double angle = -vcl_atan2(centerPoint3d2[1]-centerPoint3d[1],centerPoint3d2[0]-centerPoint3d[0]);
  
  m_RotationAngle = M_PI/2 - angle;
}


void ViewSettings::SetNorthUpAngle()
{
  typedef otb::GenericRSTransform<double,3,3> RSTransformType;
  // Build the RS transform
  RSTransformType::Pointer forwardTransform = RSTransformType::New();
  forwardTransform->SetInputKeywordList(m_KeywordList);
  forwardTransform->SetOutputProjectionRef(m_Wkt);
  forwardTransform->InstanciateTransform();

  RSTransformType::Pointer inverseTransform = RSTransformType::New();
  inverseTransform->SetInputProjectionRef(m_Wkt);
  inverseTransform->SetOutputKeywordList(m_KeywordList);
  inverseTransform->InstanciateTransform();
  
  PointType centerPoint = GetViewportCenter();
  
  // Find the rotation to apply
  RSTransformType::InputPointType centerPoint3d;
  centerPoint3d[0] = centerPoint[0];
  centerPoint3d[1] = centerPoint[1];
  centerPoint3d[2] = 0;
  
  RSTransformType::OutputPointType groundPoint3d1 = forwardTransform->TransformPoint(centerPoint3d);
  
  groundPoint3d1[0] = 1000;
  
  RSTransformType::InputPointType centerPoint3d2 = inverseTransform->TransformPoint(groundPoint3d1);
  
  double angle = -vcl_atan2(centerPoint3d2[1]-centerPoint3d[1],centerPoint3d2[0]-centerPoint3d[0]);
  
  m_RotationAngle = angle - M_PI/2;
}
}
