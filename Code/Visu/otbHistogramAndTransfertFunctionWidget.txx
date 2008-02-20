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
#ifndef _otbHistogramAndTransfertFunctionWidget_txx
#define _otbHistogramAndTransfertFunctionWidget_txx

#include "otbHistogramAndTransfertFunctionWidget.h"
#include "otbGluPolygonDrawingHelper.h"
#include <FL/gl.h>

namespace otb
{

template <class THistogram>
HistogramAndTransfertFunctionWidget<THistogram>
::HistogramAndTransfertFunctionWidget()
  : Fl_Gl_Window(0,0,0,0,0)
{
  m_Label = "Default label";
  m_HistogramColor[0]=0.5;
  m_HistogramColor[1]=0.5;
  m_HistogramColor[2]=0.5;
  m_BackgroundColor[0]=1;
  m_BackgroundColor[1]=1;
  m_BackgroundColor[2]=1;
  m_AxisColor[0]=0;
  m_AxisColor[1]=0;
  m_AxisColor[2]=0;
  m_TextColor[0]=0;
  m_TextColor[1]=0;
  m_TextColor[2]=0;
  m_GridColor[0]=0.75;
  m_GridColor[1]=0.75;
  m_GridColor[2]=0.75;
  m_Histogram = NULL;
  m_MarginX = 25;
  m_MarginY = 30;
  m_GridSizeX=5;
  m_GridSizeY=5;
}


template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::OpenGlSetup()
{
  if (!this->valid())
    {
      valid(1);
      glLoadIdentity();
      glViewport(0,0,w(),h());
      glClearColor(m_BackgroundColor[0],
		   m_BackgroundColor[1],
		   m_BackgroundColor[2],
		   1);     
    }     
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
  glMatrixMode(GL_MODELVIEW);      //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  this->ortho();
  glDisable(GL_BLEND);
}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::GridRendering(double gridXSpacing, double gridYSpacing)
{
  double x,y;
  // Rendering grid
  glBegin(GL_LINES);  
  glColor3d(m_GridColor[0],m_GridColor[1],m_GridColor[2]);
  for(unsigned int i=1;i<m_GridSizeX;++i)
    {
      x=static_cast<double>(i)*gridXSpacing+m_MarginX;
      y=m_MarginY;
      glVertex2d(x,y);
      y = static_cast<double>(this->h())-m_MarginY;
      glVertex2d(x,y);
    }
  
  for(unsigned int i=1;i<m_GridSizeY;++i)
    {
      y=static_cast<double>(i)*gridYSpacing+m_MarginY;
      x=m_MarginX;
      glVertex2d(x,y);
      x = static_cast<double>(this->w())-m_MarginX;
      glVertex2d(x,y);
    }
  glEnd();
}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::AxisRendering(void)
{
  glBegin(GL_LINES);
  glColor3d(m_AxisColor[0],m_AxisColor[1],m_AxisColor[2]);
  glVertex2d(m_MarginX,m_MarginY);
  glVertex2d(m_MarginX,static_cast<double>(this->h())-m_MarginY/2);
  glVertex2d(m_MarginX,m_MarginY);
  glVertex2d(static_cast<double>(this->w())-m_MarginX/2,m_MarginY);
  glEnd();
}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::HistogramRendering(double binWidth, double binHeightRatio, double maxFrequency)
{
  HistogramIteratorType it;
  otb::GluPolygonDrawingHelper drawer;
  double startx = m_MarginX;  
  // Temporary vertex coordinates
  double x,y;
  // Rendering histogram
  for(it=m_Histogram->Begin();it!=m_Histogram->End();++it,startx+=binWidth)
    {
      drawer.Color3d(m_HistogramColor[0],m_HistogramColor[1],m_HistogramColor[2]);
      drawer.BeginPolygon();
      drawer.BeginContour();
      
      x =startx;
      y = m_MarginY;
      drawer.Vertex2d(x,y);
      y += binHeightRatio*static_cast<double>(it.GetFrequency());
      drawer.Vertex2d(x,y);
      x += binWidth;
      drawer.Vertex2d(x,y);
      y=m_MarginY;
      drawer.Vertex2d(x,y);
      drawer.EndContour();
      drawer.EndPolygon();
    } 
}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::LegendRendering(double gridXSpacing, double gridYSpacing, double maxFrequency)
{
  double x,y;
  // rendering axis values
  itk::OStringStream oss;
  glColor3d(m_TextColor[0],m_TextColor[1],m_TextColor[2]);
  gl_font(FL_COURIER_BOLD,10);
  for(unsigned int i=0;i<m_GridSizeX;++i)
    {
      double step = (m_Histogram->Quantile(0,1.)-m_Histogram->Quantile(0,0.))
	/static_cast<double>(m_GridSizeX);
      ValueType value = static_cast<ValueType>(m_Histogram->Quantile(0,0.)
      +static_cast<double>(i)*step);
      std::cout<<step<<std::endl;
      oss<<value;
      x=static_cast<double>(i)*gridXSpacing+m_MarginX;
      y=m_MarginY/2;
      gl_draw(oss.str().c_str(),(float)x,(float)y);
      oss.str("");
    }
 for(unsigned int i=0;i<m_GridSizeY;++i)
    {
      double value = static_cast<double>(i)
	*maxFrequency/static_cast<double>(m_GridSizeY);
      oss<<value;
      y=static_cast<double>(i)*gridYSpacing+m_MarginY;
      x=0;
      gl_draw(oss.str().c_str(),(float)x,(float)y);
      oss.str("");
    }
 //rendering label
 gl_font(FL_COURIER_BOLD,12);
 gl_draw(m_Label.c_str(),static_cast<float>(3*this->w())/8,static_cast<float>(static_cast<double>(this->h())-m_MarginY/2));

}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::draw()
{
  if(!m_Histogram)
    {
      return;
    }
  std::cout<<"Beginning histogram rendering."<<std::endl;
  
  
  double maxFrequency = 0;
  HistogramIteratorType it;
  
  // Computing histogram max frequency
  for(it=m_Histogram->Begin();it!=m_Histogram->End();++it)
    {
      if(it.GetFrequency()>maxFrequency)
	{
	  maxFrequency = it.GetFrequency();
	}
    }
  
  double binWidth = static_cast<double>(this->w()-2*m_MarginX)/static_cast<double>(m_Histogram->GetSize()[0]);
  double binHeightRatio = static_cast<double>(this->h()-2*m_MarginY)/static_cast<double>(maxFrequency);
  double gridXSpacing = (static_cast<double>(this->w())-2*m_MarginX)/static_cast<double>(m_GridSizeX);
  double gridYSpacing = (static_cast<double>(this->h())-2*m_MarginY)/static_cast<double>(m_GridSizeY);
  
  OpenGlSetup();
  GridRendering(gridXSpacing,gridYSpacing);
  AxisRendering();
  HistogramRendering(binWidth,binHeightRatio,maxFrequency);
  LegendRendering(gridXSpacing,gridYSpacing,maxFrequency);
 
 std::cout<<"End of histogram rendering."<<std::endl;
}

template <class THistogram>
void
HistogramAndTransfertFunctionWidget<THistogram>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
}
#endif
