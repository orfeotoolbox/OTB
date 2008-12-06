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
#ifndef __otbImageWidgetPolylineForm_txx
#define __otbImageWidgetPolylineForm_txx

#include "otbImageWidgetPolylineForm.h"


namespace otb
{
template<class TValue>
ImageWidgetPolylineForm<TValue>
::ImageWidgetPolylineForm()
{
  m_Polyline = PolylineType::New();
  m_InternalValueToAlphaChannel = false;
}
template<class TValue>
ImageWidgetPolylineForm<TValue>
::~ImageWidgetPolylineForm()
{
}

template<class TValue>
void
ImageWidgetPolylineForm<TValue>
::Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh,unsigned ss_rate)
{
  if(this->GetVisible())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      if(m_InternalValueToAlphaChannel)
      {
        glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Polyline->GetValue());
      }
      else
      {
        glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
      }
      glBegin(GL_LINE_STRIP);

      VertexListConstIteratorType it =  this->GetPolyline()->GetVertexList()->Begin();

      while(it != this->GetPolyline()->GetVertexList()->End())
	{
	  double x1 = it.Value()[0];
	  double y1 = it.Value()[1];

	  x1 = static_cast<int>((x1-originx)*openGlZoom*(1/static_cast<double>(ss_rate)));
	  y1 = static_cast<int>(windowh+(originy-y1)*openGlZoom*(1/static_cast<double>(ss_rate)));
          glVertex2f(x1,y1);
	  ++it;
	}
      glEnd();
      glDisable(GL_BLEND);
    }
}

} // end namespace otb

#endif
