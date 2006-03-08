/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkGlWindowInteractive.cxx,v $
  Language:  C++
  Date:      $Date: 2002/10/15 15:16:54 $
  Version:   $Revision: 1.9 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkGlWindowInteractive.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <math.h>
#include <FL/Fl.H>
#include "fltkCommandEvents.h"


namespace fltk {

//------------------------------------------
//
//    Creator
//
//------------------------------------------
GlWindowInteractive::
GlWindowInteractive(int x,int y,int w,int h, const char * label)
:GlWindow(x,y,w,h,label) 
{
  m_Altitude  = 0.0;
  m_Azimuth   = 0.0; 
  m_Zoom      = 1.0;

  m_Center.Fill( 0.0 );
  m_Background.SetRed( 0.5 );
  m_Background.SetGreen( 0.5 );
  m_Background.SetBlue( 0.5 );
  
  m_Sensitivity = 500.0;

}




//------------------------------------------
//
//    Destructor
//
//------------------------------------------
GlWindowInteractive
::~GlWindowInteractive() 
{
}


//------------------------------------------
//
//    Reset Viewing Parameters
//
//------------------------------------------
void
GlWindowInteractive::
ResetViewingParameters(void)
{
  m_Altitude  = 0.0;
  m_Azimuth   = 0.0; 
  m_Zoom      = 1.0;
  m_Center.Fill( 0.0 );
}



//------------------------------------------
//
//    Set viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractive
::SetViewPoint(void) 
{
  glScaled( m_Zoom, m_Zoom, m_Zoom );
  glRotated( m_Altitude, 1.0, 0.0, 0.0 );
  glRotated( m_Azimuth, 0.0, 0.0, 1.0 );
  glTranslated( m_Center[0], m_Center[1], m_Center[2] );
}


 
//------------------------------------------
//
//    Set sensitivity for the mouse
//
//------------------------------------------
void 
GlWindowInteractive
::SetSensitivity( GLdouble sensitivity ) 
{
  m_Sensitivity = sensitivity;
}



    
//------------------------------------------
//
//    Set Center for viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractive
::SetCenter( const Point3DType & center ) 
{
  m_Center  = center ;
}


    
//------------------------------------------
//
//    Set Altitude for viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractive
::SetAltitude( GLdouble altitude ) 
{
  m_Altitude  = altitude ;
}


    
//------------------------------------------
//
//    Set Zoom for viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractive
::SetZoom( GLdouble zoom ) 
{
  m_Zoom  = zoom ;
}



    
//------------------------------------------
//
//    Set Azimuth for viewpoint parameters
//
//------------------------------------------
void 
GlWindowInteractive
::SetAzimuth( GLdouble azimuth ) 
{
  m_Azimuth = azimuth;
}


    
//------------------------------------------
//
//    Get Sensitivity to mouse movements
//
//------------------------------------------
GLdouble 
GlWindowInteractive
::GetSensitivity(void) const
{
  return m_Sensitivity;
}



    
//------------------------------------------
//
//    Get Altitude for viewpoint parameters
//
//------------------------------------------
GLdouble 
GlWindowInteractive
::GetAltitude(void) const
{ 
  return m_Altitude;
}


    
//------------------------------------------
//
//    Get Azimuth for viewpoint parameters
//
//------------------------------------------
GLdouble 
GlWindowInteractive
::GetAzimuth(void) const
{
  return m_Azimuth;
}


 
//------------------------------------------
//
//    Get Zoom for viewpoint parameters
//
//------------------------------------------
GLdouble 
GlWindowInteractive
::GetZoom(void) const
{
  return m_Zoom;
}



 
//------------------------------------------
//
//    Get Center for viewpoint parameters
//
//------------------------------------------
const GlWindowInteractive::Point3DType &
GlWindowInteractive
::GetCenter(void) const
{
  return m_Center;
}








//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
GlWindowInteractive
::SetBackground( GLfloat r, GLfloat g, GLfloat b ) 
{
  m_Background.SetRed( r );
  m_Background.SetGreen( g );
  m_Background.SetBlue( b );
}





//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
GlWindowInteractive
::SetBackground( const ColorType & newcolor ) 
{
  m_Background = newcolor;
}




//------------------------------------------
//
//    Get Background Color
//
//------------------------------------------
const GlWindowInteractive::ColorType & 
GlWindowInteractive
::GetBackground(void) const 
{
  return m_Background;
}



//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void GlWindowInteractive::draw(void) 
{

  if( !visible_r() ) 
  {
    return;
  }

  glViewport( 0, 0, w(), h() );

  glClearColor(
      m_Background.GetRed(),
      m_Background.GetGreen(),
      m_Background.GetBlue(),
      1.0);
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  const GLdouble width = 150.0;
  const GLdouble height = width * h() / w();
  glOrtho( -width, width, -height, height, -20000, 10000 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  SetViewPoint();

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  GLfloat diffuse1[]  = {   1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat diffuse2[]  = {   0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat diffuse3[]  = {   0.5f, 0.5f, 0.5f, 1.0f };

  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse1);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse2);
  glLightfv(GL_LIGHT2,GL_DIFFUSE,diffuse3);
  
  GLfloat position_0[]  = { 200.0, 200.0, 200.0, 0.0};
  glLightfv(GL_LIGHT0,GL_POSITION,position_0);
  
  GLfloat position_1[]  = {-200.0,   0.0, -100.0, 0.0};
  glLightfv(GL_LIGHT1,GL_POSITION,position_1);
  
  GLfloat position_2[]  = {   0.0,-200.0, -100.0, 0.0};
  glLightfv(GL_LIGHT2,GL_POSITION,position_2);

  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);

  GetNotifier()->InvokeEvent( fltk::GlDrawEvent() );

}

  



//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
int 
GlWindowInteractive
::handle(int event) 
{

  switch( event ) 
  {
    case FL_PUSH:
      m_Mouse_a[0] = Fl::event_x();
      m_Mouse_a[1] = Fl::event_y();
      return 1;
    case FL_RELEASE:
      m_Mouse_a = m_Mouse_b;
      return 1;
    case FL_DRAG:
      m_Mouse_b[0] = Fl::event_x();
      m_Mouse_b[1] = Fl::event_y();
      const float factorW = m_Sensitivity / ( w() * m_Zoom );
      const float factorH = m_Sensitivity / ( h() * m_Zoom );
      if( Fl::event_state( FL_CTRL ) ) 
      {
        m_Zoom *= exp(2.0*(m_Mouse_b[1] - m_Mouse_a[1])/h());
      }
      else if( Fl::event_state(FL_SHIFT) ) 
      {
        m_Center[2] += -factorH*(m_Mouse_b[1] - m_Mouse_a[1]);
        if( m_Azimuth > -90.0 && m_Azimuth < 90.0 ) 
        {
          m_Center[0] += factorW * ( m_Mouse_b[0] - m_Mouse_a[0] );
        }
        else 
        {
          m_Center[0] -=  factorW * ( m_Mouse_b[0] - m_Mouse_a[0] );
        }
      }
      else if( Fl::event_state(FL_ALT) ) 
      {
        m_Center[2] += -factorH * ( m_Mouse_b[1] - m_Mouse_a[1] );
        if( m_Azimuth > 0 && m_Azimuth < 180.0 ) 
        {
          m_Center[1] -= factorW * ( m_Mouse_b[1] - m_Mouse_a[1] );
        }
        else 
        {
          m_Center[1] +=  factorW * ( m_Mouse_b[0] - m_Mouse_a[0] );
        }
      }
      else 
      {
         m_Altitude += 90.0 * ( m_Mouse_b[1] - m_Mouse_a[1] ) / h();
         m_Azimuth  += 90.0 * ( m_Mouse_b[0] - m_Mouse_a[0] ) / w();
         if( m_Azimuth > 180.0 ) 
         {
           m_Azimuth -= 360.0;
         }
         else if( m_Azimuth < 180.0 ) 
         {
           m_Azimuth += 360.0;
         }
      }
      
      m_Mouse_a = m_Mouse_b;
      
      redraw();

      return 1;
  }

  return 0;
}




} // end namespace fltk



