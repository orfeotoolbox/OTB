/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkGlWindow.h,v $
  Language:  C++
  Date:      $Date: 2002/05/11 17:26:16 $
  Version:   $Revision: 1.10 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltkGLWINDOW
#define fltkGLWINDOW

#include <FL/gl.h> 
#include <FL/Fl_Gl_Window.H>
#include <fltkWidgetRedrawCommand.h>
#include <vector>

namespace fltk {


/**
 *  This class implements an OpenGl Window.
 *  it derives from Fl_Gl_Window and add
 *  the elements needed to communicate with
 *  ITK via Command objects
 *
 */
class GlWindow : public Fl_Gl_Window 
{


public:

  typedef WidgetRedrawCommand< Fl_Gl_Window >  RedrawCommandType;
    
public:

  GlWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~GlWindow();
  void SaveImage( void );
  void SaveImage( const char * filename );
  RedrawCommandType * GetRedrawCommand(void);
  itk::Object * GetNotifier(void);

private:

  RedrawCommandType::Pointer   m_RedrawCommand;
    
  itk::Object::Pointer    m_Notifier;

};

} // end namespace fltk


#endif



