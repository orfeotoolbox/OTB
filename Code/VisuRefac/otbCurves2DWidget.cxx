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
#ifndef __otb2DCurvesWidget_cxx
#define __otb2DCurvesWidget_cxx

#include "otbCurves2DWidget.h"

namespace otb
{
Curves2DWidget::Curves2DWidget() : /**m_Curves(),*/ m_XAxisLabel("X axis"), m_YAxisLabel("Y axis"),
				 m_Margins(), m_AxisOrigin(), m_AxisLength(),
				 m_GridOrigin(), m_GridSpacing(), m_ZeroCrossingAxis(true),
				 m_AxisColor(), m_GridColor()
{
  // Prefer a white background
  ColorType white;
  white.Fill(1);
  this->SetBackgroundColor(white);

  // Default colors
  m_AxisColor.Fill(0.);
  m_GridColor.Fill(0.9);

  // Default margin
  m_Margins.Fill(20);

  // Default values
  m_AxisOrigin.Fill(-100.);
  m_AxisLength.Fill(200);
  m_GridOrigin.Fill(-100);
  m_GridSpacing.Fill(10.);

  // The list of curves
  //m_Curves = CurveListType::New();
}

Curves2DWidget::~Curves2DWidget()
{}
  
void Curves2DWidget::draw()
{
  // Call superclass implementation for gl init
  Superclass::draw();
  // Render the grid
  this->RenderGrid();
  // Render the axis
  this->RenderAxis();
  // Render the curves
  this->RenderCurves();
}

void Curves2DWidget::RenderAxis()
{

  // Check if zero crossing axis is possible
  double xAxisYPos = static_cast<double>(m_Margins[0]);
  double yAxisXPos = static_cast<double>(m_Margins[1]);

  ContinuousIndexType spaceCenter;
  spaceCenter.Fill(0.);
  ContinuousIndexType screenCenter = SpacePositionToScreenPosition(spaceCenter);

  // Update zero crossing coords 
  if(m_ZeroCrossingAxis)
    {

    if(m_AxisOrigin[0] < 0 && m_AxisOrigin[0]+m_AxisLength[0] > 0)
      {
      yAxisXPos = screenCenter[0];
      }
    if(m_AxisOrigin[1] < 0 && m_AxisOrigin[1]+m_AxisLength[1] > 0)
      {
      xAxisYPos = screenCenter[1];
      }
    }
  glBegin(GL_LINES);
  glColor4d(m_AxisColor[0],m_AxisColor[1],m_AxisColor[2],m_AxisColor[3]);
  glVertex2d(m_Margins[0],yAxisXPos);
  glVertex2d(this->w()-m_Margins[0],yAxisXPos);
  glVertex2d(xAxisYPos,m_Margins[1]);
  glVertex2d(xAxisYPos,this->h()-m_Margins[1]);
  glEnd();

  gl_font(FL_COURIER_BOLD,10);
  //Draw the y axis legend
  ContinuousIndexType screenLabelPosition;
  screenLabelPosition[0]=yAxisXPos+10;
  screenLabelPosition[1]=this->h()-m_Margins[1]-10;
  gl_draw(m_YAxisLabel.c_str(),(float)screenLabelPosition[0],(float)screenLabelPosition[1]);

  //Draw the x axis legend
  screenLabelPosition[0]=this->w()-m_Margins[0] - 5*m_XAxisLabel.size();
  screenLabelPosition[1]=xAxisYPos - 10;
  gl_draw(m_XAxisLabel.c_str(),(float)screenLabelPosition[0],(float)screenLabelPosition[1]);
}

void Curves2DWidget::RenderGrid()
{
  ContinuousIndexType screenGridOrigin = SpacePositionToScreenPosition(m_GridOrigin);
  
  ContinuousIndexType screenGridSpacing;
  screenGridSpacing[0] = m_GridSpacing[0]*(this->w()-2*m_Margins[0])/m_AxisLength[0];
  screenGridSpacing[1] = m_GridSpacing[1]*(this->h()-2*m_Margins[1])/m_AxisLength[1];

  glBegin(GL_LINES);
  glColor4d(m_GridColor[0],m_GridColor[1],m_GridColor[2],m_GridColor[3]);
  double pos = screenGridOrigin[0];
  while(pos <= this->w()-m_Margins[0])
    {
    glVertex2d(pos,m_Margins[1]);
    glVertex2d(pos,this->h()-m_Margins[1]);
    pos+=screenGridSpacing[0];
    }

  pos = screenGridOrigin[1];

  while(pos <= this->h()-m_Margins[1])
    {
    glVertex2d(m_Margins[0],pos);
    glVertex2d(this->w()-m_Margins[0],pos);
    pos+=screenGridSpacing[1];
    }

  glEnd();
}

void Curves2DWidget::RenderCurves()
{

}

Curves2DWidget::ContinuousIndexType Curves2DWidget::SpacePositionToScreenPosition(const ContinuousIndexType & pos) const
{
  ContinuousIndexType resp;
  resp[0] = m_Margins[0]+ (pos[0]-m_AxisOrigin[0])*(this->w()-2*m_Margins[0])/m_AxisLength[0];
  resp[1] = m_Margins[1]+ (pos[1]-m_AxisOrigin[1])*(this->h()-2*m_Margins[1])/m_AxisLength[1];
  return resp;
}


Curves2DWidget::ContinuousIndexType Curves2DWidget::ScreenPositionToSpacePosition(const ContinuousIndexType & pos) const
{
  ContinuousIndexType resp;
  resp[0] = (pos[0]-m_Margins[0])*m_AxisLength[0]/(this->w()-2*m_Margins[0])+m_AxisOrigin[0];
  resp[1] = (pos[1]-m_Margins[1])*m_AxisLength[1]/(this->h()-2*m_Margins[1])+m_AxisOrigin[1];
  return resp;
}

void Curves2DWidget::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
} // end namespace otb

#endif


