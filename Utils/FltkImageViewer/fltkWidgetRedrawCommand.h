/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkWidgetRedrawCommand.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:09 $
  Version:   $Revision: 1.6 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltkWidgetRedrawCommand_h
#define __fltkWidgetRedrawCommand_h

#include "itkCommand.h"
#include <FL/Fl.H>


namespace fltk {


/**
 *  Implementation of the Command Pattern 
 *  for redrawing a fltk Widget 
 *
 *  This command will be registered as observer
 *  of other objects. When one of these objects
 *  change, it can invoke a redraw event, and 
 *  hence this command will be notified. The command
 *  will call the redraw method of the fltk Widget.
 *
 */
template<class TWidget>  
class WidgetRedrawCommand : public itk::Command 
{
public:

  /**
   * Type of the widget for which the command is directed
   */
  typedef TWidget    WidgetType;

  
  /**
   * Standard "Self" typedef.
   */
  typedef WidgetRedrawCommand<WidgetType>   Self;


  /**
   * Standard "Superclass" typedef.
   */
  typedef itk::Command  Superclass;


  /**
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Execute method will call redraw in the Widget
   */
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event );


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( WidgetRedrawCommand, ::itk::Command );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);


  /**
   * Set Widget to update
   */
  void SetWidget( WidgetType * widget );

  /**
   * Get Widget to update
   */
  WidgetType * GetWidget( void );



protected:
  /**
   * Constructor
   */
  WidgetRedrawCommand();

private:

  /**
   * Widget to redraw
   */
  WidgetType     *  m_Widget;
 

  
};


} // end namespace  fltk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "fltkWidgetRedrawCommand.txx"
#endif



#endif
