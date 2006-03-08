/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkCommandEvents.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:51 $
  Version:   $Revision: 1.5 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltkCOMMANDEVENTS
#define fltkCOMMANDEVENTS

#include "itkEventObject.h"


namespace fltk {

/**
 *  List of the events expected in this domain
 */
itkEventMacro( RedrawEvent,            itk::UserEvent );
itkEventMacro( GlDrawEvent,            itk::UserEvent );
itkEventMacro( VolumeReslicedEvent,    itk::UserEvent );
itkEventMacro( ClippingPlaneEvent,     itk::UserEvent );


  
} // end namespace fltk


#endif
