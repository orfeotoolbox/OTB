/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkGlWindowInteractive.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:58 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltkGLWINDOWINTERCLASS
#define fltkGLWINDOWINTERCLASS

#include <fltkGlWindow.h>
#include <itkPoint.h>
#include <itkRGBPixel.h>


namespace fltk {


class GlWindowInteractive : public GlWindow 
{

public:

  typedef itk::Point<GLdouble,3>  Point3DType;

  typedef itk::Point<int,2>       ScreenPointType;

  typedef itk::RGBPixel<GLdouble>   ColorType;


  GlWindowInteractive(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindowInteractive();

  virtual void draw(void);  // overload fltk methods
  virtual int  handle(int);
  
  virtual void SetViewPoint(void);

  void SetSensitivity(GLdouble newSensitivity);
  void SetAltitude(GLdouble newAltitude);
  void SetAzimuth(GLdouble newAzimuth);
  void SetZoom(GLdouble newZoom);
  void SetCenter(const Point3DType & center);
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  void ResetViewingParameters(void);
  
  const ColorType & GetBackground(void) const;
  GLdouble GetSensitivity(void) const;
  GLdouble GetAltitude(void) const;
  GLdouble GetAzimuth(void) const;
  GLdouble GetZoom(void) const ;
  const Point3DType & GetCenter(void) const;

private:

  ScreenPointType   m_Mouse_a;
  ScreenPointType   m_Mouse_b;
  ColorType         m_Background;


protected:

  GLdouble          m_Altitude;
  GLdouble          m_Azimuth;
  GLdouble          m_Zoom;
  Point3DType       m_Center;

  GLdouble          m_Sensitivity;

};


} // end namespace fltk

#endif   
