/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkWidgetRedrawCommand.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/02 14:38:08 $
  Version:   $Revision: 1.8 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "fltkWidgetRedrawCommand.h"



namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
template <class TWidget>
WidgetRedrawCommand<TWidget>
::WidgetRedrawCommand(void)  
{
  m_Widget  = 0;
}



//--------------------------------------------------
//
//    Set Widget
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::SetWidget( WidgetType * widget )  
{
  m_Widget = widget;
}



//--------------------------------------------------
//
//    Get Widget
//
//--------------------------------------------------
template <class TWidget>
typename WidgetRedrawCommand<TWidget>::WidgetType *
WidgetRedrawCommand<TWidget>
::GetWidget( void )
{
  return m_Widget;
}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::Execute( itk::Object *, const itk::EventObject & )
{

  if( !m_Widget )
  {
    return;
  }

  m_Widget->redraw();
  Fl::check();

}




//--------------------------------------------------
//
//    Execute
//
//--------------------------------------------------
template <class TWidget>
void
WidgetRedrawCommand<TWidget>
::Execute( const itk::Object * ,  const itk::EventObject & )
{

  if( !m_Widget )
  {
    return;
  }

  m_Widget->redraw();
  Fl::check();

}





} // end namespace fltk


