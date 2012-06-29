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
#include "otbCurves2DWidget.h"

namespace otb
{
Curves2DWidget::Curves2DWidget() : m_Curves(), m_XAxisLabel("X axis"), m_YAxisLabel("Y axis"), m_Margins(),
  m_Extent(), m_AxisOrigin(), m_AxisLength(),
  m_GridOrigin(), m_GridSpacing(), m_ZeroCrossingAxis(true),
  m_AutoScale(true), m_AxisColor(), m_GridColor(), m_ScaleAxeYDefault(true),
  m_SpaceToScreenTransform()
{
  // Build the curves list
  m_Curves = CurveListType::New();

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
  m_Curves = CurveListType::New();

  // The space to screen transform
  m_SpaceToScreenTransform = AffineTransformType::New();
}

Curves2DWidget::~Curves2DWidget()
{}

unsigned int Curves2DWidget::AddCurve(CurveType * curve)
{
  m_Curves->PushBack(curve);
  return m_Curves->Size() - 1;
}

void Curves2DWidget::RemoveCurve(unsigned int idx)
{
  m_Curves->Erase(idx);
}

void Curves2DWidget::RemoveCurveByItsID(unsigned int id)
{
  unsigned int size = m_Curves->Size();

  for (unsigned int j = 0; j < size; ++j)
    {
    if (m_Curves->GetNthElement(j)->GetId() == id)
      {
      m_Curves->Erase(j);
      break;
      }
    }
}

Curves2DWidget::CurvePointerType
Curves2DWidget::GetCurveByItsId(unsigned int id)
{
  for (unsigned int j = 0; j < m_Curves->Size(); ++j)
    {
    if (m_Curves->GetNthElement(j)->GetId() == id)
      {
      return m_Curves->GetNthElement(j);
      }
    }

  itkExceptionMacro(<< "No curve found with ID " << id);
}

void Curves2DWidget::ClearAllCurves()
{
  m_Curves->Clear();
}

unsigned int Curves2DWidget::GetNumberOfCurves()
{
  return m_Curves->Size();
}

void Curves2DWidget::draw()
{
  // Call superclass implementation for gl init
  Superclass::draw();

  // Call the before rendering method for each curve
  CurveListType::Iterator it = m_Curves->Begin();
  while (it != m_Curves->End())
    {
    it.Get()->BeforeRendering();
    ++it;
    }
  if (m_AutoScale)
    {
    this->AutoScale();
    }

  // Update the space to screen transform
  this->UpdateSpaceToScreenTransform();

  // Render the grid
  this->RenderGrid();
  // Render the axis
  this->RenderAxis();
  // Render the curves
  this->RenderCurves();
}

void Curves2DWidget::AutoScale()
{

  if (m_Curves->Size() < 1)
    {
    // No curves available, retrun
    return;
    }
  PointType min, max, newMin, newMax;

  // Get min and max from the first curve
  CurveListType::Iterator it = m_Curves->Begin();
  min = it.Get()->GetMinimum();
  max = it.Get()->GetMaximum();

  // Get the absolute min/max
  while (it != m_Curves->End())
    {
    newMin = it.Get()->GetMinimum();
    newMax = it.Get()->GetMaximum();

    if (newMin[0] < min[0]) min[0] = newMin[0];
    if (newMin[1] < min[1]) min[1] = newMin[1];
    if (newMax[0] > max[0]) max[0] = newMax[0];
    if (newMax[1] > max[1]) max[1] = newMax[1];

    ++it;
    }

  // Autoscale
  m_AxisOrigin = min;
  m_AxisLength = max - min;
  m_GridSpacing = m_AxisLength / 10;
  m_GridOrigin = min;
}

void Curves2DWidget::UpdateSpaceToScreenTransform()
{
  // Update Extent
  RegionType::IndexType extentIndex;
  RegionType::SizeType  extentSize;

  // Extent index
  extentIndex[0] = m_Margins[0];
  extentIndex[1] = m_Margins[1];

  // Extent size
  extentSize[0] = this->w() - 2 * m_Margins[0];
  extentSize[1] = this->h() - 2 * m_Margins[1];

  // Set the display extent
  m_Extent.SetIndex(extentIndex);
  m_Extent.SetSize(extentSize);

  // Set the matrix
  AffineTransformType::MatrixType matrix;
  matrix.Fill(0);
  matrix(0, 0) = (static_cast<double>(m_Extent.GetSize()[0])) / m_AxisLength[0];
  matrix(1, 1) = (static_cast<double>(m_Extent.GetSize()[1])) / m_AxisLength[1];
  m_SpaceToScreenTransform->SetMatrix(matrix);

  // Set the translation
  AffineTransformType::OutputVectorType translation;
  translation[0] = m_Extent.GetIndex()[0] - m_AxisOrigin[0];
  translation[1] = m_Extent.GetIndex()[1] - m_AxisOrigin[1];
  m_SpaceToScreenTransform->SetTranslation(translation);

  // Set the center
  m_SpaceToScreenTransform->SetCenter(m_AxisOrigin);
}

void Curves2DWidget::RenderAxis()
{

  // Check if zero crossing axis is possible
  double xAxisYPos = static_cast<double>(m_Margins[0]);
  double yAxisXPos = static_cast<double>(m_Margins[1]);

  PointType spaceCenter;
  spaceCenter.Fill(0.);
  PointType screenCenter = m_SpaceToScreenTransform->TransformPoint(spaceCenter);

  // Update zero crossing coords
  if (m_ZeroCrossingAxis)
    {

    if (m_AxisOrigin[0] < 0 && m_AxisOrigin[0] + m_AxisLength[0] > 0)
      {
      yAxisXPos = screenCenter[0];
      }
    if (m_AxisOrigin[1] < 0 && m_AxisOrigin[1] + m_AxisLength[1] > 0)
      {
      xAxisYPos = screenCenter[1];
      }
    }
  glBegin(GL_LINES);
  glColor4d(m_AxisColor[0], m_AxisColor[1], m_AxisColor[2], m_AxisColor[3]);
  glVertex2d(m_Margins[0], yAxisXPos);
  glVertex2d(this->w() - m_Margins[0], yAxisXPos);
  glVertex2d(xAxisYPos, m_Margins[1]);
  glVertex2d(xAxisYPos, this->h() - m_Margins[1]);
  glEnd();

  gl_font(FL_COURIER_BOLD, 10);
  //Draw the y axis legend
  PointType screenLabelPosition;
  screenLabelPosition[0] = yAxisXPos + 10;
  screenLabelPosition[1] = this->h() - m_Margins[1] - 10;
  gl_draw(m_YAxisLabel.c_str(), (float) screenLabelPosition[0], (float) screenLabelPosition[1]);

  //Draw the x axis legend
  screenLabelPosition[0] = this->w() - m_Margins[0] - 5 * m_XAxisLabel.size();
  screenLabelPosition[1] = xAxisYPos - 10;
  gl_draw(m_XAxisLabel.c_str(), (float) screenLabelPosition[0], (float) screenLabelPosition[1]);

  // Draw the title of the curve
  screenLabelPosition[0] = this->w() - m_Margins[0] - 7 * m_Title.size();
  screenLabelPosition[1] = this->h() - m_Margins[1] - 10;
  gl_draw(m_Title.c_str(), (float) screenLabelPosition[0], (float) screenLabelPosition[1]);
}

void Curves2DWidget::RenderGrid()
{
  // Get the grid screen origin
  PointType screenGridOrigin = m_SpaceToScreenTransform->TransformPoint(m_GridOrigin);
  // Get the grid screen spacing
  VectorType screenGridSpacing = m_SpaceToScreenTransform->TransformVector(m_GridSpacing);

  // Render the grid
  glBegin(GL_LINES);
  glColor4d(m_GridColor[0], m_GridColor[1], m_GridColor[2], m_GridColor[3]);
  double pos = screenGridOrigin[0];
  while (pos <= this->w() - m_Margins[0])
    {
    glVertex2d(pos, m_Margins[1]);
    glVertex2d(pos, this->h() - m_Margins[1]);
    pos += screenGridSpacing[0];
    }

  pos = screenGridOrigin[1];

  while (pos <= this->h() - m_Margins[1])
    {
    glVertex2d(m_Margins[0], pos);
    glVertex2d(this->w() - m_Margins[0], pos);
    pos += screenGridSpacing[1];
    }

  glEnd();

  // Display the vertical grid index (Y axis)
  pos = screenGridOrigin[1];

  int scale;
  if(m_ScaleAxeYDefault)
  {
    scale = 0;
  }
  else
    scale = m_GridOrigin[1];

  std::ostringstream oss;
  gl_font(FL_COURIER_BOLD, 8);
  glColor4d(0, 0, 0, 0.5);

  while(pos <= this->h()-m_Margins[1])
    {
    oss<<scale;

    gl_draw(oss.str().c_str(), (float)m_Margins[0]-20, (float)pos);
    pos+=screenGridSpacing[1];
    scale += m_GridSpacing[1];
    oss.str("");
    }

     // Display the vertical grid index (X axis)
    scale = m_GridOrigin[0];
    pos = screenGridOrigin[0];
    while(pos <= this->w()-m_Margins[0])
    {
    oss<<scale;

    gl_draw(oss.str().c_str(), (float)pos, m_Margins[1]-5);
    pos+=screenGridSpacing[0];
    scale += m_GridSpacing[0];
    oss.str("");
    }


}

void Curves2DWidget::RenderCurves()
{
  for (CurveListType::Iterator it = m_Curves->Begin();
       it != m_Curves->End(); ++it)
    {
    if (it.Get()->GetVisible())
      {
      it.Get()->Render(m_Extent, m_SpaceToScreenTransform);
      }
    }
}

void Curves2DWidget::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
