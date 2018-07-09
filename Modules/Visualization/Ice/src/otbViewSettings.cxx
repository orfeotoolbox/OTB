/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbViewSettings.h"
#include "otbGenericRSTransform.h"
#include "otbImageToGenericRSOutputParameters.h"
#include "otbMapProjections.h"

namespace otb
{
ViewSettings::ViewSettings() :
  m_Wkt( "" ),
  m_RotationAngle( 0.0 ),
  m_UseProjection( true ),
  m_GeometryChanged( false )
{
  m_Origin.Fill( 0 );
  m_Spacing.Fill( 1 );
  m_ViewportSize.Fill( 0 );
  m_RotationCenter.Fill( 0 );
}

ViewSettings::~ViewSettings()
{}

void
ViewSettings
::Reset()
{
  m_Origin.Fill( 0 );
  m_Spacing.Fill( 1 );
  m_ViewportSize.Fill( 0 );

  m_Wkt = "";

  m_KeywordList = KeywordListType();

  m_UseProjection = true;
  m_GeometryChanged = false;

  m_RotationCenter.Fill( 0 );

  m_RotationAngle = 0.0;
}

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

void ViewSettings::UpdateRotation(const PointType & newCenter, double newAngle)
{
    std::complex<double> theta1 = std::polar(1.0,-m_RotationAngle);
    std::complex<double> theta2 = std::polar(1.0,-newAngle+m_RotationAngle);

    m_RotationAngle = -std::arg(theta1*theta2);

    if(std::abs(m_RotationAngle) > 1e-9)
      {
      // Compute new center
      std::complex<double> c1(m_RotationCenter[0],m_RotationCenter[1]);
      std::complex<double> c2(newCenter[0],newCenter[1]);
      std::complex<double> one(1.0,0.0);
    
      std::complex<double> finalCenter = (theta2*(c1-c2)+c2-theta1*theta2*c1)/(one-theta1*theta2);
    
      m_RotationCenter[0] = finalCenter.real();
      m_RotationCenter[1] = finalCenter.imag();
      }
}


void ViewSettings::SetPersepectiveAngle()
{
  typedef otb::GenericRSTransform<double,3,3> RSTransformType;
  // Build the RS transform
  RSTransformType::Pointer forwardTransform = RSTransformType::New();
  forwardTransform->SetInputKeywordList(m_KeywordList);
  forwardTransform->SetInputProjectionRef(m_Wkt);
  forwardTransform->InstantiateTransform();

  RSTransformType::Pointer inverseTransform = RSTransformType::New();
  inverseTransform->SetOutputProjectionRef(m_Wkt);
  inverseTransform->SetOutputKeywordList(m_KeywordList);
  inverseTransform->InstantiateTransform();

  PointType centerPoint = GetViewportCenter();

  // Find the rotation to apply
  RSTransformType::InputPointType centerPoint3d;
  centerPoint3d[0] = centerPoint[0];
  centerPoint3d[1] = centerPoint[1];
  centerPoint3d[2] = 0;

  RSTransformType::OutputPointType groundPoint3d1 = forwardTransform->TransformPoint(centerPoint3d);

  groundPoint3d1[2] = 1000;

  RSTransformType::InputPointType centerPoint3d2 = inverseTransform->TransformPoint(groundPoint3d1);

  double angle = -std::atan2(centerPoint3d2[1]-centerPoint3d[1],centerPoint3d2[0]-centerPoint3d[0]);
  
  this->UpdateRotation(centerPoint,M_PI/2 - angle);
}


void ViewSettings::SetNorthUpAngle()
{
  typedef otb::GenericRSTransform<double,3,3> RSTransformType;
  // Build the RS transform
  RSTransformType::Pointer forwardTransform = RSTransformType::New();
  forwardTransform->SetInputKeywordList(m_KeywordList);
  forwardTransform->SetInputProjectionRef(m_Wkt);
  forwardTransform->InstantiateTransform();

  RSTransformType::Pointer inverseTransform = RSTransformType::New();
  inverseTransform->SetOutputProjectionRef(m_Wkt);
  inverseTransform->SetOutputKeywordList(m_KeywordList);
  inverseTransform->InstantiateTransform();
  
  PointType centerPoint = GetViewportCenter();
  
  // Find the rotation to apply
  RSTransformType::InputPointType centerPoint3d;
  centerPoint3d[0] = centerPoint[0];
  centerPoint3d[1] = centerPoint[1];
  centerPoint3d[2] = 0;
  
  RSTransformType::OutputPointType groundPoint3d1 = forwardTransform->TransformPoint(centerPoint3d);
  
  groundPoint3d1[1] += 0.01;
  
  RSTransformType::InputPointType centerPoint3d2 = inverseTransform->TransformPoint(groundPoint3d1);
  
  double angle = -std::atan2(centerPoint3d2[1]-centerPoint3d[1],centerPoint3d2[0]-centerPoint3d[0]);

  this->UpdateRotation(centerPoint, M_PI/2 - angle);

}


double
ViewSettings
::GetScale( const PointType & origin, const PointType & extent, bool isSigned ) const
{
  SpacingType spacing;

  assert( m_ViewportSize[ 0 ]!=0 );
  assert( m_ViewportSize[ 1 ]!=0 );

  spacing[ 0 ] = ( extent[ 0 ] - origin[ 0 ] ) / m_ViewportSize[ 0 ];
  spacing[ 1 ] = ( extent[ 1 ] - origin[ 1 ] ) / m_ViewportSize[ 1 ];

  SpacingType scale( spacing );

  scale[ 0 ] = std::abs( scale[ 0 ] );
  scale[ 1 ] = std::abs( scale[ 1 ] );

  if( scale[ 0 ]>scale[ 1 ] )
    return
      isSigned
      ? spacing[ 0 ]
      : scale[ 0 ];

  return
    isSigned
    ? spacing[ 1 ]
    : scale[ 1 ];
}


} // end of 'otb' namespace.
