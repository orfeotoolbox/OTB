/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkClippingPlane3DDrawer.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:51 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __fltkClippingPlane3DDrawer_h
#define __fltkClippingPlane3DDrawer_h

#include "fltkClippingPlane3DDrawerGUI.h"
#include "itkImage.h"
#include "itkCommand.h"
#include <FL/gl.h>



namespace fltk {


class ClippingPlane3DDrawer : public fltkClippingPlane3DDrawerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef  ClippingPlane3DDrawer       Self;

 

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltkClippingPlane3DDrawerGUI  Superclass;


  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(ClippingPlane3DDrawer, fltkClippingPlane3DDrawerGUI);


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  



  /**
   * Command that will draw the object
   */
  typedef itk::SimpleConstMemberCommand< Self >   DrawCommandType;
  typedef DrawCommandType::Pointer                DrawCommandPointer;


  /**
   * Update internal data from the GUI
   */
   void UpdateFromGUI(void);


  /**
   * Update the GUI from the internal data 
   */
   void UpdateTheGUI(void);


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void) const;


  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  /**
   * Default constructor
   */
   ClippingPlane3DDrawer();


  /**
   * Destructor
   */
  ~ClippingPlane3DDrawer();



private:

  double                 m_X;  // Plane orientation
  double                 m_Y;
  double                 m_Z;

  double                 m_R;  // Distance from the center

  bool                   m_Enabled;
   
  unsigned int           m_Id;

  static unsigned int    counter;
  
  DrawCommandPointer     m_DrawCommand;

};


} // end namespace fltk





#endif




