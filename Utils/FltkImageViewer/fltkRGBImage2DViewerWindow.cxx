/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkRGBImage2DViewerWindow.cxx,v $
  Language:  C++
  Date:      $Date: 2003/07/26 18:34:45 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkRGBImage2DViewerWindow.h"
#include "fltkCommandEvents.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>


namespace fltk {

//------------------------------------------
//
//    Creator
//
//------------------------------------------
RGBImage2DViewerWindow::
RGBImage2DViewerWindow(int x,int y,int w,int h, const char * label)
:GlWindow(x,y,w,h,label) 
{

  m_Background.SetRed( 0.5 );
  m_Background.SetGreen( 0.5 );
  m_Background.SetBlue( 0.5 );
  
  m_Buffer = 0;
  m_Overlay = 0;

  m_ShiftX =   0;
  m_ShiftY =   0;
  m_Zoom   = 1.0;

  m_NumberOfBytesPerPixel = 3 ; // change by 4 in case of RGB

  m_Width = 0;
  m_Height = 0;
}




//------------------------------------------
//
//    Destructor
//
//------------------------------------------
RGBImage2DViewerWindow
::~RGBImage2DViewerWindow() 
{
  if( m_Buffer )
    {
    delete [] m_Buffer;
    m_Buffer = 0;
    }

  if( m_Overlay )
    {
    delete [] m_Overlay;
    m_Overlay = 0;
    }
}




//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
RGBImage2DViewerWindow
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
RGBImage2DViewerWindow
::SetBackground( const ColorType & newcolor ) 
{
  m_Background = newcolor;
}




//------------------------------------------
//
//    Get Background Color
//
//------------------------------------------
const RGBImage2DViewerWindow::ColorType & 
RGBImage2DViewerWindow
::GetBackground(void) const 
{
  return m_Background;
}



//------------------------------------------
//
//    Allocate
//
//------------------------------------------
void
RGBImage2DViewerWindow::
Allocate(unsigned int nx,unsigned int ny)
{

  if( m_Buffer )
    {
    delete [] m_Buffer;
    }

  this->size(nx,ny);

  m_Buffer = new unsigned char[ nx * ny * m_NumberOfBytesPerPixel ]; 
  
  this->SetWidth( nx );
  this->SetHeight( ny );

}

//------------------------------------------
//
//    Allocate Overlay
//
//------------------------------------------
void
RGBImage2DViewerWindow::
AllocateOverlay(void)
{
  if( !m_Buffer )
  {
    return;
  }
  if( m_Overlay )
  {
    delete [] m_Overlay;
  }

  m_Overlay = new unsigned char[ GetWidth() * GetHeight() * 4 ]; // *4 for RGBA


}

//------------------------------------------
//
//    Update the display 
//
//------------------------------------------
void
RGBImage2DViewerWindow
::Update(void) 
{
  this->redraw();
  Fl::check();
}




//------------------------------------------
//
//    Set Height
//
//------------------------------------------
void RGBImage2DViewerWindow::SetHeight(unsigned int height) 
{
  m_Height = height;
  m_ShiftY = -m_Height;
}




//------------------------------------------
//
//    Set Width
//
//------------------------------------------
void RGBImage2DViewerWindow::SetWidth(unsigned int width) 
{
  m_Width = width;
  m_ShiftX = -m_Width;
}



//------------------------------------------
//
//    Get Width
//
//------------------------------------------
unsigned int
RGBImage2DViewerWindow::GetWidth( void ) const
{
  return m_Width;
}




//------------------------------------------
//
//    Get Height
//
//------------------------------------------
unsigned int
RGBImage2DViewerWindow::GetHeight( void ) const
{
  return m_Height;
}



//------------------------------------------
//
//    Fit Image to Window
//
//------------------------------------------
void 
RGBImage2DViewerWindow::FitImageToWindow(void) 
{
  m_Zoom = static_cast<GLdouble>( h() ) /
           static_cast<GLdouble>( m_Height ); 
  m_ShiftY = -m_Height;
  m_ShiftX = -m_Width;
  this->redraw();
  Fl::check();

}



//------------------------------------------
//
//    Fit Window to Image
//
//------------------------------------------
void 
RGBImage2DViewerWindow::FitWindowToImage(void) 
{
  m_Zoom = 1.0;
  m_ShiftX = -m_Width;
  m_ShiftY = -m_Height;
  m_ParentWindow->size( m_Width, m_Height );
}



//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void RGBImage2DViewerWindow::draw(void) 
{

  if( !m_Buffer)
  {
    return;
  }
  
  
  if( !visible_r() ) 
  {
    return;
  }

  if(!valid())
  {
    
    glViewport( 0, 0, m_Width, m_Height );

    glClearColor(
      m_Background.GetRed(),
      m_Background.GetGreen(),
      m_Background.GetBlue(),
      1.0);
  
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
  }


  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  const GLdouble width  = static_cast<GLdouble>( m_Width );
  const GLdouble height = static_cast<GLdouble>( m_Height );

//  glOrtho( -width, width, -height, height, -20000, 10000 );
  gluOrtho2D( -width, width, -height, height );  
  
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glPixelStorei( GL_UNPACK_ALIGNMENT,        1 );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, m_Width );
  glPixelStorei( GL_UNPACK_SKIP_ROWS,        0 );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS,      0 );
  
  glRasterPos2i( m_ShiftX, m_ShiftY );

  glPixelZoom( m_Zoom, m_Zoom );

  glDrawPixels( m_Width, m_Height, 
                GL_RGB, GL_UNSIGNED_BYTE, 
                static_cast<void *>(m_Buffer) );

  glDisable(GL_DEPTH_TEST);

  if(m_Overlay)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawPixels(m_Width, m_Height, GL_RGBA, 
       GL_UNSIGNED_BYTE,  static_cast<void *>(m_Overlay));
    glDisable(GL_BLEND);
  }

  //
  // Prepare for drawing other objects
  //
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



  // Call other drawers
  GetNotifier()->InvokeEvent( fltk::GlDrawEvent() );

}

  

//------------------------------------------
//
//    Set Intensity Window
//
//------------------------------------------
void 
RGBImage2DViewerWindow
::SetIntensityWindow( Fl_Window * window )
{
  m_IntensityWindow = window;
}



//------------------------------------------
//
//    Set Parent Window
//
//------------------------------------------
void 
RGBImage2DViewerWindow
::SetParentWindow( Fl_Window * window )
{
  m_ParentWindow = window;
}




//------------------------------------------
//
//    Intensity Windowing
//
//------------------------------------------
void 
RGBImage2DViewerWindow
::IntensityWindowing(void) 
{
  m_IntensityWindow->show();
}


//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
void 
RGBImage2DViewerWindow
::handlePopUpMenu(void) 
{
  static Fl_Menu_Button * popupMenu = 0;
  // need to keep the shift of the first popup creation for redrawing purpose
  static unsigned int popupShift=0; 
  if( !popupMenu )
  {
    popupMenu = new Fl_Menu_Button(m_ParentWindow->x()+Fl::event_x(),
                                   m_ParentWindow->y()-(m_ParentWindow->h()/2)+Fl::event_y(),
                                   100,200);
    popupShift = (m_ParentWindow->h()/2);
    //by using this function this seems to disable position() function (FTLK bug)
      //popupMenu->type( Fl_Menu_Button::POPUP3); 
    popupMenu->add("Fit Image To Window");
    popupMenu->add("Fit Window To Image");
    popupMenu->add("Intensity Windowing");
  }
  else
  {
    popupMenu->position(m_ParentWindow->x()+Fl::event_x(),m_ParentWindow->y()-popupShift+Fl::event_y());
  }
  typedef enum 
    {
    FIT_IMAGE_TO_WINDOW,
    FIT_WINDOW_TO_IMAGE,
    INTENSITY_WINDOWING
    } PopupMenuOptions;

  popupMenu->popup();

  switch( popupMenu->value() )
  {
    case FIT_WINDOW_TO_IMAGE:
      this->FitWindowToImage();
      break;
    case FIT_IMAGE_TO_WINDOW:
      this->FitImageToWindow();
      break;
    case INTENSITY_WINDOWING:
      this->IntensityWindowing();
      break;
  }
}




//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
int 
RGBImage2DViewerWindow
::handle(int event) 
{

  static int p1x  = 0;
  static int p1y  = 0;

  switch( event ) 
  {
    case FL_PUSH:
      {
      const int state = Fl::event_state();
      if( state == FL_BUTTON1 )
        {
        p1x = Fl::event_x();
        p1y = Fl::event_y();
        }
      else if( state == FL_BUTTON2 )
        {
          std::cout << "Button2 " << std::endl;
        } 
      else if( state == FL_BUTTON3 )
        {
          handlePopUpMenu();
        }
      return 1;
      }
    case FL_RELEASE:
      return 1;
    case FL_DRAG:
      {
        return 1;
      }
  }

  return 0;
}

} // end namespace fltk


