/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkFrustum3D.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:54 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltkFrustum3D_h
#define __fltkFrustum3D_h

#include "fltkShape3D.h"



namespace fltk {




/**
 *  \class fltk::Frustum3D derives from Shape3D and represents a parallepiped in 3D.
 */
class ITK_EXPORT Frustum3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef Frustum3D   Self;

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
  itkTypeMacro( Frustum3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    
  

  /**
   * Get/Set the Top plane position
   */
  itkGetMacro( TopPlane, double );
  itkSetMacro( TopPlane, double );


  /**
   * Get/Set the Bottom plane position
   */
  itkGetMacro( BottomPlane, double );
  itkSetMacro( BottomPlane, double );


  /**
   * Get/Set Aperture Angle along X
   */
  itkGetMacro( ApertureAngleX, double );
  itkSetMacro( ApertureAngleX, double );



  /**
   * Get/Set Aperture Angle along Y
   */
  itkGetMacro( ApertureAngleY, double );
  itkSetMacro( ApertureAngleY, double );



  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



protected:

  /**
   * Constructor
   */
  Frustum3D();


  /**
   * Copy Constructor
   */
  Frustum3D( const Frustum3D & box );


  /**
   * Assignment Operator
   */
  const Frustum3D & operator=( const Frustum3D & box );


private:

  /**
   * Aperture angle of the pyramid along the X direction
   */
  double m_ApertureAngleX;

  /**
   * Aperture angle of the pyramid along the Y direction
   */
  double m_ApertureAngleY;


  /**
   * Distance from the Apex to the top plane
   */
  double m_TopPlane;

 
  /**
   * Distance from the Apex to the bottom plane
   */
  double m_BottomPlane;

 
 
};


} // end namespace fltk


#endif
