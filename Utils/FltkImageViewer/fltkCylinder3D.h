/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkCylinder3D.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:53 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltkCylinder3D_h
#define __fltkCylinder3D_h

#include "fltkShape3D.h"



namespace fltk {




/**
 *  \class fltk::Cylinder3D derives from Shape3D and represents a cylinder in 3D.
 */
class ITK_EXPORT Cylinder3D : public Shape3D 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef Cylinder3D   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Cylinder3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Set the Radius
   */
        void SetRadius( double radius );


  /**
   * Set the Height
   */
        void SetHeight( double height );


  /**
   * Set the Number Of Sectors to digitize the base
   */
        void SetNumberOfSectors( unsigned int number );


  /**
   * Get the Radius
   */
        double GetRadius(void) const;


  /**
   * Get the Height
   */
        double GetHeight(void) const;


  /**
   * Get the Number of Sectors
   */
        unsigned int GetNumberOfSectors(void) const;


  /**
   * Draw the Cone using OpenGL commands
   */
        void DrawGeometry(void) const;



protected:
  /**
   * Constructor
   */
        Cylinder3D();

private:
  /**
   *  Radius of the base
   */
        double          m_Radius;

  /**
   *   Height of the cylinder
   */
        double          m_Height;

  /**
   *   Number of Sector in which the circle is digitized
   */
        unsigned int    m_NumberOfSectors;


        };


}   // end namespace li

#endif
