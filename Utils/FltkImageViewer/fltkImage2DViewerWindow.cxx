/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkImage2DViewerWindow.cxx,v $
  Language:  C++
  Date:      $Date: 2002/10/15 15:16:54 $
  Version:   $Revision: 1.20 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkImage2DViewerWindow.h"
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
Image2DViewerWindow::
Image2DViewerWindow(int x,int y,int w,int h, const char * label)
:GlWindow(x,y,w,h,label) 
{

  m_Background.SetRed( 0.5 );
  m_Background.SetGreen( 0.5 );
  m_Background.SetBlue( 0.5 );
  
  m_Buffer = 0;

  m_ShiftX =   0;
  m_ShiftY =   0;
  m_Zoom   = 1.0;

  m_NumberOfBytesPerPixel = 1; // change by 4 in case of RGBA

  m_Width = 0;
  m_Height = 0;

  m_SelectionCallBack = 0;
  m_ClickCallBack = 0;

  m_InteractionMode = SelectMode;

}




//------------------------------------------
//
//    Destructor
//
//------------------------------------------
Image2DViewerWindow
::~Image2DViewerWindow() 
{
  if( m_Buffer )
    {
    delete [] m_Buffer;
    m_Buffer = 0;
    }
}




//------------------------------------------
//
//    Set Background Color
//
//------------------------------------------
void 
Image2DViewerWindow
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
Image2DViewerWindow
::SetBackground( const ColorType & newcolor ) 
{
  m_Background = newcolor;
}




//------------------------------------------
//
//    Get Background Color
//
//------------------------------------------
const Image2DViewerWindow::ColorType & 
Image2DViewerWindow
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
Image2DViewerWindow::
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
//    Update the display 
//
//------------------------------------------
void
Image2DViewerWindow
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
void Image2DViewerWindow::SetHeight(unsigned int height) 
{
  m_Height = height;
  m_ShiftY = -m_Height;
}




//------------------------------------------
//
//    Set Width
//
//------------------------------------------
void Image2DViewerWindow::SetWidth(unsigned int width) 
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
Image2DViewerWindow::GetWidth( void ) const
{
  return m_Width;
}




//------------------------------------------
//
//    Get Height
//
//------------------------------------------
unsigned int
Image2DViewerWindow::GetHeight( void ) const
{
  return m_Height;
}



//------------------------------------------
//
//    Fit Image to Window
//
//------------------------------------------
void 
Image2DViewerWindow::FitImageToWindow(void) 
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
Image2DViewerWindow::FitWindowToImage(void) 
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
void Image2DViewerWindow::draw(void) 
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
  
  glRasterPos2i( m_ShiftX, m_ShiftY ) ;

  glPixelZoom( m_Zoom, m_Zoom );


  if( m_InteractionMode == SelectMode )
    {
    // drawing selection box begin
    int X1 = m_Box.X1 * 2 + m_ShiftX ;
    int X2 = m_Box.X2 * 2 + m_ShiftX ;
    int Y1 = -(m_Box.Y1 * 2 + m_ShiftY) ;
    int Y2 = -(m_Box.Y2 * 2 + m_ShiftY) ;

    if (Y2 >= m_Height - 2 )
      {
      Y2 = m_Height - 2 ;
      }

    glBegin(GL_LINE_STRIP);
    glVertex2i(X1, Y1) ;
    glVertex2i(X2, Y1) ;
    glVertex2i(X2, Y2) ;
    glVertex2i(X1, Y2) ;
    glVertex2i(X1, Y1) ;
    glEnd();
    // end 
    }


  glDrawPixels( m_Width, m_Height, 
                GL_LUMINANCE, GL_UNSIGNED_BYTE, 
                static_cast<void *>(m_Buffer) );

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
Image2DViewerWindow
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
Image2DViewerWindow
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
Image2DViewerWindow
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
Image2DViewerWindow
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
Image2DViewerWindow
::handle(int event) 
{

  static int p1x  = 0;
  static int p1y  = 0;

  switch( event ) 
  {
    case FL_PUSH:
      {
      const int state = Fl::event_state();
      if( state & FL_BUTTON1 )
        {
        p1x = Fl::event_x();
        p1y = Fl::event_y();
        switch( m_InteractionMode )
          {
          case PanningMode:
            break;
          case SelectMode:
            break;
          case ClickMode:
            ClickEventHandling( p1x, p1y );
            break;
          case ZoomingMode:
            break;
          }

        }
      else if( state & FL_BUTTON2 )
        {
        } 
      else if( state & FL_BUTTON3 )
        {
          handlePopUpMenu();
        }
      return 1;
      }
    case FL_RELEASE:
      return 1;
    case FL_DRAG:
      {
      const int state = Fl::event_state();
      if( state == FL_BUTTON1 )
        {
        switch( m_InteractionMode )
          {
          case PanningMode:
            PanningEventHandling( p1x, p1y );
            break;
          case SelectMode:
            SelectEventHandling( p1x, p1y );
            break;
          case ClickMode:
            break;
          case ZoomingMode:
            break;
          }
        }
      else if (state == FL_BUTTON2 )
        {
          ZoomingEventHandling( p1x, p1y );
        }
      return 1;
      }
  }

  return 0;
}



//------------------------------------------
//
//    Panning Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::PanningEventHandling(int & p1x, int & p1y)
{
  int p2x = Fl::event_x();
  int p2y = Fl::event_y();
  const int dx = p2x - p1x;
  const int dy = p2y - p1y;
  m_ShiftX += dx;
  m_ShiftY -= dy; // Mouse Y -> - OpenGl Y
  p1x = p2x;
  p1y = p2y;
  redraw();
  Fl::check();
}


void 
Image2DViewerWindow
::SetSelectionBox(SelectionBoxType* box)
{
  m_Box.X1 = box->X1 ;
  m_Box.Y1 = box->Y1 ;
  m_Box.X2 = box->X2 ;
  m_Box.Y2 = box->Y2 ;
  redraw();
  Fl::check();
}

void 
Image2DViewerWindow
::SetSelectionCallBack(void* ptrObject,
                       void (*selectionCallBack)(void* ptrObject,
                                                 SelectionBoxType* box))
{
  m_SelectionCallBackTargetObject = ptrObject ;
  m_SelectionCallBack = selectionCallBack ;
}


void 
Image2DViewerWindow
::SetClickCallBack(void* ptrObject,
                   void (*clickCallBack)(void* ptrObject,
                                      int & px, int & py ))
{
  m_ClickCallBackTargetObject = ptrObject ;
  m_ClickCallBack = clickCallBack ;
}



//------------------------------------------
//
//    Click Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::ClickEventHandling(int & px, int & py)
{
  
  if (m_ClickCallBack != 0)
    {
    m_ClickCallBack( m_ClickCallBackTargetObject, px, py ) ;
    }
  redraw();
  Fl::check();
}

//------------------------------------------
//
//    Select Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::SelectEventHandling(int & p1x, int & p1y)
{
  int p2x = Fl::event_x();
  int p2y = Fl::event_y();

  if (p2x >= m_Width)
    {
      p2x = m_Width - 1 ;
    }

  if (p2x <= 0)
    {
      p2x = 0 ;
    }

  if (p2y >= m_Height)
    {
      p2y = m_Height ;
    }

  if (p2y <= 0 )
    {
      p2y = 0 ;
    }

  m_Box.X1 = p1x ;
  m_Box.X2 = p2x ;
  m_Box.Y1 = p1y ;
  m_Box.Y2 = p2y ;

  if (m_SelectionCallBack != 0)
    {
      m_SelectionCallBack(m_SelectionCallBackTargetObject, &m_Box) ;
    }

  redraw();
  Fl::check();
}

//------------------------------------------
//
//    Zooming Event Handling 
//
//------------------------------------------
void 
Image2DViewerWindow
::ZoomingEventHandling(int & p1x, int & p1y)
{
  int p2x = Fl::event_x();
  int p2y = Fl::event_y();
  const int dx = p2x - p1x;
  const int dy = p2y - p1y;
  m_ShiftX += dx;
  m_ShiftY -= dy; // Mouse Y -> - OpenGl Y
  p1x = p2x;
  p1y = p2y;
  redraw();
  Fl::check();
}



//------------------------------------------
//
//    Select Interaction Mode
//
//------------------------------------------
void 
Image2DViewerWindow
::SetInteractionMode( InteractionModeType mode )
{
  m_InteractionMode = mode;
}


} // end namespace fltk


