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
}
