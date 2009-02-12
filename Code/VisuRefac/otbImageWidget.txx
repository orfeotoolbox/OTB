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
#ifndef __otbImageWidget_txx
#define __otbImageWidget_txx

#include "otbImageWidget.h"

namespace otb
{
template <class TInputImage>
ImageWidget<TInputImage>
::ImageWidget() : Fl_Gl_Window(0,0,0,0)
{
  // Setting up default values
  m_IsotropicZoom = 1.0;
  m_OpenGlBuffer  = NULL;
  m_OpenGlBufferSize.Fill(0);
  m_Identifier    = "Default";
}

template <class TInputImage>
ImageWidget<TInputImage>
::~ImageWidget()
{
  if(m_OpenGlBuffer!=NULL)
    {
    delete [] m_OpenGlBuffer;
    }
}

template <class TInputImage>
void
ImageWidget<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os,indent);
  // Display information about the widget
  os<<indent<<"Widget "<<m_Identifier<<": "<<std::endl;
  #ifdef OTB_GL_USE_ACCEL
  os<<indent<<indent<<"OpenGl acceleration is enabled."<<std::endl;
  #else
   os<<indent<<indent<<"OpenGl acceleration is disabled."<<std::endl;
  #endif
  if(m_OpenGlBuffer == NULL)
    {
    os<<indent<<indent<<"OpenGl buffer is not allocated."<<std::endl;
    }
  else
    {
    os<<indent<<indent<<"OpenGl buffer is allocated with size "<<m_OpenGlBufferSize<<"."<<std::endl;
    }
  os<<indent<<indent<<"OpenGl isotropic zoom is "<<m_IsotropicZoom<<"."<<std::endl;
}



template <class TInputImage>
void
ImageWidget<TInputImage>
::ReadBuffer(InputImageType * image, RegionType & region)
{

}

template <class TInputImage>
void
ImageWidget<TInputImage>
::draw()
{

}

template <class TInputImage>
void
ImageWidget<TInputImage>
::resize(int x, int y, int w, int h)
{

}

template <class TInputImage>
int
ImageWidget<TInputImage>
::handle(int event)
{

}
}
#endif
