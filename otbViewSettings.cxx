#include "otbViewSettings.h"

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
}

ViewSettings::~ViewSettings()
{}

void ViewSettings::GetViewportExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  ulx = std::min(m_Origin[0],m_Origin[0] + m_Spacing[0]*m_ViewportSize[0]);
  uly = std::min(m_Origin[1],m_Origin[1] + m_Spacing[1]*m_ViewportSize[1]);
  lrx = std::max(m_Origin[0],m_Origin[0] + m_Spacing[0]*m_ViewportSize[0]);
  lry = std::max(m_Origin[1],m_Origin[1] + m_Spacing[1]*m_ViewportSize[1]);
}

void ViewSettings::ScreenToViewPortTransform(const double & xscreen, const double & yscreen, double & xvp, double & yvp) const
{
  xvp = m_Origin[0] + xscreen * m_Spacing[0];
  yvp = m_Origin[1] + (m_ViewportSize[1] - yscreen) * m_Spacing[1];
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
  m_Origin[0] = center[0] - m_Spacing[0]*m_ViewportSize[0];
  m_Origin[1] = center[1] - m_Spacing[1]*m_ViewportSize[1];
}

}
