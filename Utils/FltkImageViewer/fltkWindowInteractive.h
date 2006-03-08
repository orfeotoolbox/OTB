/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkWindowInteractive.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:10 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#ifndef fltkWINDOWINTERCLASS
#define fltkWINDOWINTERCLASS

#include <FL/Fl_Window.H>
#include <fltkDrawer.h>
#include <itkPoint.h>
#include <itkVector.h>
#include <vector>


namespace fltk {


class WindowInteractive: public Fl_Window 
{

public:

  typedef itk::Point<float,2>     OffsetType;
  typedef itk::Vector<float,2>    OffsetVariationType;

  typedef itk::Point<int,2>       ScreenPointType;

public:
  WindowInteractive(int x,int y,int w,int h, const char * label=0);
  virtual ~WindowInteractive();
  virtual void draw(void);
  virtual int  handle(int);

  const float & GetZoom(void) const;
  const OffsetType & GetOffset(void) const;
  const OffsetVariationType & GetOffsetVariation(void) const;

  void  ClearOffset(void);
  void  AddDrawer( Drawer * dr );

private:

  ScreenPointType   m_Mouse_a;
  ScreenPointType   m_Mouse_b;

  float m_Zoom;

  OffsetType            m_Offset;
  OffsetVariationType   m_OffsetVariation;

  std::vector< Drawer * > m_Drawers;
  
};


} // end namespace fltk

#endif   
