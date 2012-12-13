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
#include "otbGlWidget.h"

namespace otb
{

GlWidget
::GlWidget() : Fl_Gl_Window(0, 0, 0, 0), m_Identifier(), m_UseGlAcceleration(false),
  m_BackgroundColor()
{
  m_Identifier = "Default";

#if 0
  // OTB_GL_USE_ACCEL
  m_UseGlAcceleration = true;
#endif

  // Default background color
  m_BackgroundColor.Fill(0);
}

GlWidget::~GlWidget()
{
  // Clear registered controller
  m_Controller = NULL;
}

void GlWidget::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os, indent);
  // Display information about the widget
  os << indent << "Widget " << m_Identifier << ": " << std::endl;

#if 0
  #ifndef OTB_GL_USE_ACCEL
  os << indent << indent << "OpenGl acceleration is not allowed." << std::endl;
  #else
  if (m_UseGlAcceleration)
    {
    os << indent << indent << "OpenGl acceleration is allowed and enabled." << std::endl;
    }
  else
    {
    os << indent << indent << "OpenGl acceleration is allowed but disabled." << std::endl;
    }
  #endif
#endif
}

void GlWidget::draw()
{
#if 0
  // Check if Gl acceleration mode is correct
  #ifndef OTB_GL_USE_ACCEL
  if (m_UseGlAcceleration)
    {
    itkWarningMacro(
      <<
      "Gl acceleration enabled but not allowed. Consider rebuilding with OTB_USE_GL_ACCEL to ON. For now, disabling Gl acceleration.");
    m_UseGlAcceleration = false;
    }
  #endif
#endif

  // Set up Gl environement
  if (!this->valid())
    {
    valid(1);
    glLoadIdentity();
    glViewport(0, 0, w(), h());
    glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
  glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
  glMatrixMode(GL_MODELVIEW);      //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  this->ortho();
  glDisable(GL_BLEND);
}

void GlWidget::resize(int x, int y, int w, int h)
{
  // Distinguish between resize, move and not changed events
  // (The system window manager may generate multiple resizing events,
  // so we'd rather avoid event flooding here)
  bool reportMove   = false;
  bool reportResize = false;
  if (this->x() != x || this->y() != y)
    {
    reportMove = true;
    }

  if (this->w() != w || this->h() != h)
    {
    reportResize = true;
    }

  // First call the superclass implementation
  Fl_Gl_Window::resize(x, y, w, h);
  // If There is a controller
  if (m_Controller.IsNotNull())
    {
    if (reportMove)
      {
      m_Controller->HandleWidgetMove(m_Identifier, x, y);
      }
    if (reportResize)
      {
      m_Controller->HandleWidgetResize(m_Identifier, w, h);
      }
    }
}

int GlWidget::handle(int event)
{
  // Call superclass implementation
  int resp = Fl_Widget::handle(event);

  // Check if there is a controller
  // Avoid processing hide events, since it causes segfault (the
  // destructor of the Fl class generates hide events).
  if (m_Controller.IsNotNull() && event != FL_HIDE)
    {
    resp = m_Controller->HandleWidgetEvent(m_Identifier, event);
    }
  return resp;
}

GlWidget::PointType GlWidget::GetMousePosition()
{
  // Get the cursor position
  PointType index;
  index[0] = Fl::event_x();
  index[1] = Fl::event_y();
  // Flip the y axis
  index[1] = this->h() - index[1];
  return index;
}
}
