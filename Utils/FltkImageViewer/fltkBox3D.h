/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkBox3D.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:50 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltkBox3D_h
#define __fltkBox3D_h

#include "fltkShape3D.h"



namespace fltk {




/**
 *  \class fltk::Box3D derives from Shape3D and represents a parallepiped in 3D.
 */
class ITK_EXPORT Box3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Box3D   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Box3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Set Width
   */
  void SetWidth( double width );

  
  /**
   * Set Height
   */
  void SetHeight( double height);

 
  /**
   * Set Length
   */
  void SetLength( double length );


  /**
   * Return Width
   */
  double GetWidth(void) const;


  /**
   * Return Height
   */
  double GetHeight(void) const;

  /**
   * Return Length
   */
  double GetLength(void) const;


  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;

protected:
  /**
   * Constructor
   */
  Box3D();

  /**
   * Constructor with values
   */
  Box3D( double length, double width, double height );


  /**
   * Copy Constructor
   */
  Box3D( const Box3D & box );


  /**
   * Assignment Operator
   */
  const Box3D & operator=( const Box3D & box );


private:

  /**
   * Length of the Box, by default is aligned with X axis
   */
  double m_Length;
 
  /**
   * Width of the Box, by default is aligned with Y axis
   */
  double m_Width;

  /**
   * Width of the Box, by default is aligned with Z axis
   */
  double m_Height;

 
};


} // end namespace fltk


#endif
