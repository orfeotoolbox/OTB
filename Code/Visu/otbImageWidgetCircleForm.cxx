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
#ifndef __otbImageWidgetCircleForm_cxx
#define __otbImageWidgetCircleForm_cxx

#include "otbImageWidgetCircleForm.h"
#include "otbMath.h"


namespace otb
{
ImageWidgetCircleForm
::ImageWidgetCircleForm()
{
  m_Solid = true;
  m_Center.Fill(0);
  m_Radius = 1;

}

ImageWidgetCircleForm
::~ImageWidgetCircleForm()
{
}
void
ImageWidgetCircleForm
::Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh,unsigned ss_rate)
{
  if(this->GetVisible())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
      }
      if(m_Solid)
      {
        glBegin(GL_POLYGON);
      }
      else
      {
        glBegin(GL_LINE_LOOP);
      }
        for(double angle = 0;angle <= 2*M_PI;angle+=0.01/static_cast<double>(m_Radius))
        {
          double xi = m_Center[0]+static_cast<double>(m_Radius)*vcl_sin(angle);
          double yi = m_Center[1]+static_cast<double>(m_Radius)*vcl_cos(angle);

          double xd = (xi-originx)*openGlZoom*(1/static_cast<double>(ss_rate));
          double yd = windowh+(originy-yi)*openGlZoom*(1/static_cast<double>(ss_rate));
          glVertex2d(xd,yd);
        }
      glEnd();
      glDisable(GL_BLEND);
}

} // end namespace otb

#endif
