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
#include "itkImageRegionConstIteratorWithIndex.h"

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

  #ifdef OTB_GL_USE_ACCEL
  m_UseGlAcceleration = true;
  #else
  m_UseGlAcceleration = false;
  #endif
}

template <class TInputImage>
ImageWidget<TInputImage>
::~ImageWidget()
{
  // Delete OpenGl buffer if needed
  if(m_OpenGlBuffer!=NULL)
    {
    delete [] m_OpenGlBuffer;
    m_OpenGlBuffer = NULL;
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
  #if (not defined OTB_GL_USE_ACCEL)
  os<<indent<<indent<<"OpenGl acceleration is not allowed."<<std::endl;
  #else
  if(m_UseGlAcceleration)
    {
    os<<indent<<indent<<"OpenGl acceleration is allowed and enabled."<<std::endl;
    }
  else
    {
    os<<indent<<indent<<"OpenGl acceleration is allowed but disabled."<<std::endl;
    }
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
  // Before doing anything, check if region is inside the buffered
  // region of image
  if(!image->GetBufferedRegion().IsInside(region))
    {
    itkExceptionMacro(<<"Region to read is oustside of the buffered region.");
    }
  // Check if Gl acceleration mode is correct
  #ifndef OTB_GL_USE_ACCEL
  if(m_UseGlAcceleration)
    {
    itkExceptionMacro(<<"Gl acceleration enabled but not allowed. Consider rebuilding with OTB_USE_GL_ACCEL to ON.");
    }
  #endif

  // Delete previous buffer if needed
  if(m_OpenGlBuffer != NULL)
    {
    delete [] m_OpenGlBuffer;
    m_OpenGlBuffer = NULL;
    }
  // Allocate new memory
  m_OpenGlBuffer = new unsigned char[3*region.GetNumberOfPixels()];

  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex<InputImageType> it(image,region);

  // Go to begin
  it.GoToBegin();

  while(!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    if(!m_UseGlAcceleration)
      {
      // compute the linear index (buffer is flipped around X axis
      // when gl acceleration is disabled
      index = ComputeXAxisFlippedBufferIndex(it.GetIndex(),region);
      }
    else
      {
      // Conpute the linear index
      index = ComputeBufferIndex(it.GetIndex(),region);
      }

    // Fill the buffer
    m_OpenGlBuffer[index]  =it.Get()[0];
    m_OpenGlBuffer[index+1]=it.Get()[1];
    m_OpenGlBuffer[index+2]=it.Get()[2];
    ++it;
    }
  // Last, updating buffer size
  m_OpenGlBufferSize = region.GetSize();
}

template <class TInputImage>
void
ImageWidget<TInputImage>
::draw()
{
  // Check if Gl acceleration mode is correct
  #ifndef OTB_GL_USE_ACCEL
  if(m_UseGlAcceleration)
    {
    itkException(<<"Gl acceleration enabled but not allowed. Consider rebuilding with OTB_USE_GL_ACCEL to ON.");
    }
  #endif

  // Set up Gl environement
  if (!this->valid())
  {
    valid(1);
    glLoadIdentity();
    glViewport(0,0,w(),h());
    glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }

  glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
  glMatrixMode(GL_MODELVIEW);      //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  this->ortho();
  glDisable(GL_BLEND);

  // Check if there is somthing to draw
  if(m_OpenGlBuffer == NULL)
    {
    return;
    }

  // Image extent
  double sizex = m_IsotropicZoom*static_cast<double>(m_OpenGlBufferSize[0]);
  double sizey = m_IsotropicZoom*static_cast<double>(m_OpenGlBufferSize[1]);
  double startx = (static_cast<double>(this->w())-sizex)/2;
  double starty = (static_cast<double>(this->h())-sizey)/2;

  if(!m_UseGlAcceleration)
    {
    // Set the pixel Zoom
    glRasterPos2f(startx,starty);
    glPixelZoom(m_IsotropicZoom,m_IsotropicZoom);
    
    // display the image
    glDrawPixels(m_OpenGlBufferSize[0],
		 m_OpenGlBufferSize[1],
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 m_OpenGlBuffer);
    }
  else
    {
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,0.0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_OpenGlBufferSize[0], m_OpenGlBufferSize[1], 0, GL_RGB, GL_UNSIGNED_BYTE, m_OpenGlBuffer);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  // Linear Filtering
    glBindTexture (GL_TEXTURE_2D, texture);
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (startx,starty, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (startx+sizex, starty, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (startx+sizex,starty+sizey, 0.0);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (startx,starty+sizey, 0.0);
    glEnd ();
    glDisable(GL_TEXTURE_2D);
    }
}

template <class TInputImage>
void
ImageWidget<TInputImage>
::resize(int x, int y, int w, int h)
{
  // First call the superclass implementation
  Fl_Gl_Window::resize(x,y,w,h);
  // If There is a controller
  if(m_Controller.IsNotNull())
    {
    m_Controller->HandleWidgetResize(m_Identifier,x,y,w,h);
    }
}

template <class TInputImage>
int
ImageWidget<TInputImage>
::handle(int event)
{
  // If there is a controller
  if(m_Controller.IsNotNull())
    {
    return m_Controller->HandleWidgetEvent(m_Identifier,event);
    }
  else
    {
    return 0;
    }
}
}
#endif
