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
#ifndef _otbImageWidgetPolygonForm_txx
#define _otbImageWidgetPolygonForm_txx

#include "otbImageWidgetPolygonForm.h"


namespace otb
{
template<class TValue>
ImageWidgetPolygonForm<TValue>
::ImageWidgetPolygonForm()
{
  m_Polygon = PolygonType::New();
}
template<class TValue>
ImageWidgetPolygonForm<TValue>
::~ImageWidgetPolygonForm()
{
}

template<class TValue>
void
ImageWidgetPolygonForm<TValue>
::Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh)
{
  if(this->GetVisible())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
      glBegin(GL_LINE_LOOP);

      VertexListIteratorType it =  this->GetPolygon()->GetVertexList()->Begin();
      double x1 = it.Value()[0];
      double y1 = it.Value()[1];
      
      x1 = static_cast<unsigned int>((x1-originx)*openGlZoom);
      y1 = static_cast<unsigned int>(windowh+(originy-y1)*openGlZoom);
      
      ++it; 
      while(it != this->GetPolygon()->GetVertexList()->End())
	{  
	  double x2 = it.Value()[0];
	  double y2 = it.Value()[1];

	  x2 = static_cast<unsigned int>((x2-originx)*openGlZoom);
	  y2 = static_cast<unsigned int>(windowh+(originy-y2)*openGlZoom);

	  glVertex2f(x1,y1);
	  glVertex2f(x2,y2);

	  x1 = x2;
	  y1 = y2;
	  ++it;
	}      
      
      glEnd();
      glDisable(GL_BLEND);
    }
}
 
} // end namespace otb

#endif
