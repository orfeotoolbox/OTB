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
#include "otbCrossGlComponent.h"


namespace otb
{
CrossGlComponent
::CrossGlComponent() : m_IndexList(),m_Spacing(), m_Origin(), m_GluTesselator(),
                            m_ColorList(), m_LineWidth(1.5),m_CrossWidth(10), m_RedColor()
{
  // Default color is red
  m_RedColor.Fill(0);
  m_RedColor[0]=1.;
  m_RedColor[3]=0.75;


  // Intialize origin and spacing
  m_Origin.Fill(0.);
  m_Spacing.Fill(1.);
  
  // Create the tesselator
  m_GluTesselator = gluNewTess();
}

CrossGlComponent
::~CrossGlComponent()
{
  // Delete the tesselator
  gluDeleteTess(m_GluTesselator);
}


void
CrossGlComponent
::Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform)
{
  if(m_IndexList.size() == 0)
    {
    // nothing to render, return
    return;
    }
  
  // Set up blending and color
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4d(m_RedColor[0],m_RedColor[1],m_RedColor[2],m_RedColor[3]);
  
  // Set up line width
  double previousWidth = 0.;
  glGetDoublev(GL_LINE_WIDTH,&previousWidth);

  // convert line width to screen line width
  VectorType imageLineWidth;
  imageLineWidth.Fill(m_LineWidth);
  VectorType screenLineWidth = space2ScreenTransform->TransformVector(imageLineWidth);
  glLineWidth(screenLineWidth[0]);
  // Do we need to render boundaries only (for polygons)
  gluTessProperty(m_GluTesselator,GLU_TESS_BOUNDARY_ONLY,false);//m_RenderPolygonBoundariesOnly);

  // Enabling line antialiasing
  glEnable(GL_LINE_SMOOTH);
  
  for(unsigned int i=0; i<m_IndexList.size(); i++)
  {
    this->Render(i,extent,space2ScreenTransform);
  }

  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
  glLineWidth(previousWidth);
}
 
  
void
CrossGlComponent
::Render(unsigned int id, const RegionType & /*extent*/, const AffineTransformType * space2ScreenTransform)
{
  glColor4d(m_ColorList[id][0],m_ColorList[id][1],m_ColorList[id][2],m_ColorList[id][3]);

  // Take into account pixel spacing and origin
  //PointType spacePoint = dataNode->GetPoint();
  PointType spacePoint;
  spacePoint[0]*= m_Spacing[0];
  spacePoint[1]*= m_Spacing[1];
  spacePoint[0] = m_IndexList[id][0] + m_Origin[0];
  spacePoint[1] = m_IndexList[id][1] + m_Origin[1];

  // Transform to a screen point
  PointType screenPoint = space2ScreenTransform->TransformPoint(spacePoint);
    
  glBegin(GL_LINES);
  // Draw a cross
  glVertex2d(screenPoint[0]-m_CrossWidth,screenPoint[1]);
  glVertex2d(screenPoint[0]+m_CrossWidth,screenPoint[1]);
  glVertex2d(screenPoint[0],screenPoint[1]-m_CrossWidth);
  glVertex2d(screenPoint[0],screenPoint[1]+m_CrossWidth);
  glEnd();
}

}
