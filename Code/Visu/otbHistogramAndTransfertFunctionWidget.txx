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

namespace otb
{

template <class THistogram>
HistogramAndTransfertFunctionWidget<THistogram>
::HistogramAndTransfertFunctionWidget()
  : Fl_Gl_Window(0,0,0,0,0)
{
  m_Label = "Default label";
  m_HistogramColor[0]=0;
  m_HistogramColor[1]=0;
  m_HistogramColor[2]=0;
  m_BackgroundColor[0]=1;
  m_BackgroundColor[1]=1;
  m_BackgroundColor[2]=1;
  m_AxisColor[0]=0;
  m_AxisColor[1]=0;
  m_AxisColor[2]=0;
  m_TextColor[0]=0;
  m_TextColor[1]=0;
  m_TextColor[2]=0;
  m_Histogram = NULL;
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
  
  double maxFrequency = 0;
  HistogramIteratorType it;
  
  for(it=m_Histogram->Begin();it!=m_Histogram->End();++it)
    {
      std::cout<<it.GetFrequency()<<std::endl;
      if(it.GetFrequency()>maxFrequency)
	{
	  maxFrequency = it.GetFrequency();
	}
    }
  
  double binWidth = static_cast<double>(this->w())/static_cast<double>(m_Histogram->GetSize()[0]);
  double binHeightRatio = static_cast<double>(this->h())/static_cast<double>(maxFrequency);
  std::cout<<"Bin width: "<<binWidth<<std::endl;
  std::cout<<"Bin height ratio: "<<binHeightRatio<<std::endl;
  std::cout<<"Max frequency: "<<maxFrequency<<std::endl;
  
  otb::GluPolygonDrawingHelper drawer;
  double startx = 0;
  
  for(it=m_Histogram->Begin();it!=m_Histogram->End();++it,startx+=binWidth)
    {
      double x,y;
      drawer.Color3d(m_HistogramColor[0],m_HistogramColor[1],m_HistogramColor[2]);
      drawer.BeginPolygon();
      drawer.BeginContour();
      
      x =startx;
      y = 0;
      drawer.Vertex2d(x,y);
      y = binHeightRatio*static_cast<double>(it.GetFrequency());
      drawer.Vertex2d(x,y);
      x = startx+binWidth;
      drawer.Vertex2d(x,y);
      y=0;
      drawer.Vertex2d(x,y);
      drawer.EndContour();
      drawer.EndPolygon();
    } 
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
