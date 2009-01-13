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
#ifndef __otbImageWidgetPolygonForm_txx
#define __otbImageWidgetPolygonForm_txx

#include "otbImageWidgetPolygonForm.h"
#include "otbGluPolygonDrawingHelper.h"

namespace otb
{
template<class TValue>
ImageWidgetPolygonForm<TValue>
::ImageWidgetPolygonForm()
{
  m_Polygon = PolygonType::New();
  m_Solid = true;
  m_InternalValueToAlphaChannel = false;
}
template<class TValue>
ImageWidgetPolygonForm<TValue>
::~ImageWidgetPolygonForm()
{
}

template<class TValue>
void
ImageWidgetPolygonForm<TValue>
::Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh,unsigned ss_rate)
{
  if(this->GetVisible())
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      GluPolygonDrawingHelper::Pointer polygonDrawer = GluPolygonDrawingHelper::New();


      if(m_InternalValueToAlphaChannel)
      {
        polygonDrawer->Color4d(m_Color[0],m_Color[1],m_Color[2],m_Polygon->GetValue());
      }
      else
      {
  polygonDrawer->Color4d(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
      }
      polygonDrawer->SetBoundaryOnly(!m_Solid);
      VertexListConstIteratorType it =  this->GetPolygon()->GetVertexList()->Begin();

      while(it != this->GetPolygon()->GetVertexList()->End())
  {
    double x1 = it.Value()[0];
    double y1 = it.Value()[1];

    x1 = static_cast<int>((x1-originx)*openGlZoom*(1/static_cast<double>(ss_rate)));
    y1 = static_cast<int>(windowh+(originy-y1)*openGlZoom*(1/static_cast<double>(ss_rate)));
    polygonDrawer->Vertex2d(x1,y1);
    ++it;
  }
      polygonDrawer->RenderPolygon();
      glDisable(GL_BLEND);
    }
}

} // end namespace otb

#endif
