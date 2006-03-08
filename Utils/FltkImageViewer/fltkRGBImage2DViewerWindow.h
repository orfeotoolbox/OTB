/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkRGBImage2DViewerWindow.h,v $
  Language:  C++
  Date:      $Date: 2003/07/26 18:34:45 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltk_RGBImage2D_Viewer_h
#define fltk_RGBImage2D_Viewer_h

#include <fltkGlWindow.h>
#include <itkRGBPixel.h>


namespace fltk {


class RGBImage2DViewerWindow : public GlWindow 
{

public:

  typedef     unsigned char    ValueType;

  typedef itk::RGBPixel<float>  ColorType;

  RGBImage2DViewerWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~RGBImage2DViewerWindow();

  virtual void draw(void);  // overload fltk methods
  virtual int  handle(int);
  virtual void  handlePopUpMenu( void );
  
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  void Allocate( unsigned int nx, unsigned int ny );
  void Update( void );
  
  const ColorType & GetBackground(void) const;

  ValueType * GetBuffer(void) 
  { return m_Buffer; }

  unsigned int GetNumberOfBytesPerPixel(void) const
  { return m_NumberOfBytesPerPixel; }

  unsigned int GetWidth(void) const;
  unsigned int GetHeight(void) const;
  void SetWidth(unsigned int);
  void SetHeight(unsigned int);
  void FitWindowToImage(void);
  void FitImageToWindow(void);
  void IntensityWindowing(void);
  void SetIntensityWindow( Fl_Window * window );
  void SetParentWindow( Fl_Window * window );

  void AllocateOverlay(void);
  ValueType * GetOverlay(void) 
  { return m_Overlay; }

private:

  ColorType          m_Background;
  ValueType       *  m_Buffer;
  ValueType       *  m_Overlay;
  int                m_Width;
  int                m_Height;
  double             m_Zoom;
  int                m_ShiftX;
  int                m_ShiftY;
  Fl_Window       *  m_IntensityWindow;
  Fl_Window       *  m_ParentWindow;
  unsigned int       m_NumberOfBytesPerPixel;
};


} // end namespace fltk

#endif   
