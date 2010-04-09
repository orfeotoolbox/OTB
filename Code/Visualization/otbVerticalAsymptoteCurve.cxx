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
#include "otbVerticalAsymptoteCurve.h"

namespace otb
{

VerticalAsymptoteCurve::VerticalAsymptoteCurve()
{
  // Default histogram color
  m_VerticalAsymptoteColor.Fill(0.5);
  m_Abcisse = 0.;
}

// VerticalAsymptoteCurve::~VerticalAsymptoteCurve()
// {}

void VerticalAsymptoteCurve::Render(const RegionType& extent, const AffineTransformType * space2ScreenTransform)
{
  PointType spacePoint, screenPoint;

  // Rendering bounds
  glColor3d(m_VerticalAsymptoteColor[0], m_VerticalAsymptoteColor[1], m_VerticalAsymptoteColor[2]);
  glBegin(GL_LINES);
  // UL
  spacePoint[0] = m_Abcisse;
  spacePoint[1] = extent.GetIndex()[1];
  screenPoint   = space2ScreenTransform->TransformPoint(spacePoint);
  glVertex2d(screenPoint[0], spacePoint[1]);

  // LL
  spacePoint[1] = extent.GetIndex()[1] + extent.GetSize()[1];
  screenPoint   = space2ScreenTransform->TransformPoint(spacePoint);
  glVertex2d(screenPoint[0], spacePoint[1]);
  glEnd();
}

void
VerticalAsymptoteCurve::BeforeRendering()
{
  // Initialize
  m_Minimum[0] = 100;
  m_Minimum[1] = 100;

  m_Maximum[1] = -10;
  m_Maximum[0] = -10;
}

VerticalAsymptoteCurve::PointType
VerticalAsymptoteCurve
::GetMinimum()
{
  return m_Minimum;
}

VerticalAsymptoteCurve::PointType
VerticalAsymptoteCurve
::GetMaximum()
{
  return m_Maximum;
}

} // end namespace otb
