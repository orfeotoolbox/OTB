/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkProgressBar.h,v $
  Language:  C++
  Date:      $Date: 2002/05/10 22:56:16 $
  Version:   $Revision: 1.10 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltk_ProgressBar_h
#define __fltk_ProgressBar_h

#include "itkCommand.h"
#include "fltkCommandEvents.h"
#include <FL/Fl_Slider.H>


namespace fltk {

/**
 *  GUI object that derives from a FLTK Slider and
 *  implements a progress barr.
 *
 *  It contains an Update Command object compatible
 *  with ITK Observer Pattern
 */
class ProgressBar : public Fl_Slider
{
public:

  /** Command Class invoked for button redraw */
  typedef itk::MemberCommand< ProgressBar >  RedrawCommandType;


  /** Constructor */
  ProgressBar(int x, int y, int w, int h, char * label=0);

  
  /** Get Command */
  RedrawCommandType * GetRedrawCommand( void ) const;

  
  /** Manage a Progress event */
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event );
  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event );
 

  /** Manage a Progress event */
  void Observe( itk::Object *caller );

private:

  RedrawCommandType::Pointer m_RedrawCommand;
  
};


} // end namespace fltk


#endif
