/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkProgressBar.cxx,v $
  Language:  C++
  Date:      $Date: 2002/06/21 20:37:37 $
  Version:   $Revision: 1.5 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkProgressBar.h"
#include "itkProcessObject.h"
#include <FL/Fl.H>


namespace fltk {


/** Constructor */
ProgressBar::ProgressBar(int x, int y, int w, int h, char * label):
    Fl_Slider( x, y, w, h, label ) {
 m_RedrawCommand = RedrawCommandType::New();
 m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ProcessEvent );
 m_RedrawCommand->SetCallbackFunction( this, &ProgressBar::ConstProcessEvent );
}


  
/** Get Command */
ProgressBar::RedrawCommandType * 
ProgressBar::GetRedrawCommand( void ) const
{
  return m_RedrawCommand.GetPointer();
}

  



/** Manage a Progress event */
void 
ProgressBar::ProcessEvent( itk::Object * caller, 
                           const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent )   ==  typeid( event ) )
    {
    ::itk::ProcessObject::Pointer  process = 
             dynamic_cast< itk::ProcessObject *>( caller );
    this->value( process->GetProgress() );
    this->redraw();
    Fl::check();
    }
}




void 
ProgressBar::ConstProcessEvent( const itk::Object * caller, 
                                const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent )   ==  typeid( event ) ) 
    {
    itk::ProcessObject::ConstPointer  process = 
              dynamic_cast< const itk::ProcessObject *>( caller );
    this->value( process->GetProgress() );
    this->redraw();
    Fl::check();
    }
}
 

/** Manage a Progress event */
void 
ProgressBar::Observe( itk::Object *caller )
{
  caller->AddObserver(  itk::ProgressEvent(), m_RedrawCommand.GetPointer() );
}

  

} // end namespace fltk


