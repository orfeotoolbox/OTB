/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkWindowInteractive.cxx,v $
  Language:  C++
  Date:      $Date: 2002/04/14 21:52:20 $
  Version:   $Revision: 1.7 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <fltkWindowInteractive.h>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <math.h>


namespace fltk {


//------------------------------------------
//
//    Creator
//
//------------------------------------------
WindowInteractive
::WindowInteractive(int x,int y,int w,int h, const char * label)
  :Fl_Window(x,y,w,h,label) 
{

  m_Zoom     = 1.0f;

  m_Offset.Fill(  itk::NumericTraits<OffsetType::ValueType>::Zero );
  m_OffsetVariation.Fill( itk::NumericTraits<OffsetVariationType::ValueType>::Zero );

  m_Mouse_a.Fill( itk::NumericTraits<ScreenPointType::ValueType>::Zero );
  m_Mouse_b.Fill( itk::NumericTraits<ScreenPointType::ValueType>::Zero );

}






//------------------------------------------
//
//    Destructor
//
//------------------------------------------
WindowInteractive
::~WindowInteractive() 
{
  
}



//------------------------------------------
//
//    Plug Drawer Object
//
//------------------------------------------
void 
WindowInteractive
::AddDrawer( Drawer *dr) 
{
  if( !dr ) 
  {
    return;
  }
  m_Drawers.push_back(dr);
}

  


//------------------------------------------
//
//    Return actual Zoom factor
//
//------------------------------------------
const float &
WindowInteractive
::GetZoom(void) const 
{
  return m_Zoom;
}


//------------------------------------------
//
//    Return actual offset
//
//------------------------------------------
const WindowInteractive::OffsetType &  
WindowInteractive
::GetOffset(void) const 
{
  return m_Offset;
}




//------------------------------------------
//
//    Return variation in offset
//
//------------------------------------------
const WindowInteractive::OffsetVariationType &  
WindowInteractive
::GetOffsetVariation(void) const 
{
  return m_OffsetVariation;
}




//------------------------------------------
//
//    Clear cumulated offset values
//
//------------------------------------------
void 
WindowInteractive
::ClearOffset(void) 
{
  m_Offset.Fill(  itk::NumericTraits<OffsetType::ValueType>::Zero );
  m_OffsetVariation.Fill( itk::NumericTraits<OffsetVariationType::ValueType>::Zero );
}



//------------------------------------------
//
//    Draw Scene
//
//------------------------------------------
void 
WindowInteractive
::draw(void) 
{

  if( !visible_r() ) return;

  std::vector<Drawer *>::iterator it = m_Drawers.begin();

  while( it != m_Drawers.end() )
  {
    Drawer * drawer = *it;
    if( drawer ) 
    {
      drawer->Draw();
    }
    ++it;
  }

}

  



//------------------------------------------
//
//    Event Handling 
//
//------------------------------------------
int 
WindowInteractive
::handle(int event) 
{

  m_OffsetVariation.Fill( 0.0 );

  switch(event) 
  {
    case FL_PUSH:
      m_Mouse_a[0] = Fl::event_x();
      m_Mouse_a[1] = Fl::event_y();
      return 1;
    case FL_RELEASE:
      m_Mouse_a = m_Mouse_b;
      return 1;
    case FL_DRAG:
    {
      m_Mouse_b[0] = Fl::event_x();
      m_Mouse_b[1] = Fl::event_y();
      if( Fl::event_key(FL_Control_L) ) 
      {
        m_Zoom *= (float)exp(2.0*(m_Mouse_b[1] - m_Mouse_a[1])/h());
      }
      else
      {
        m_OffsetVariation[0] = 90.0f*(m_Mouse_b[0] - m_Mouse_a[0])/w();
        m_OffsetVariation[1] = 90.0f*(m_Mouse_b[1] - m_Mouse_a[1])/h();
        m_Offset += m_OffsetVariation;
      }
      m_Mouse_a = m_Mouse_b;
      redraw();
      return 1;
    }
   }

  return 0;

}




} // end namespace fltk


