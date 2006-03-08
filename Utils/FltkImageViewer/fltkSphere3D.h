/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSphere3D.h,v $
  Language:  C++
  Date:      $Date: 2002/10/15 15:16:54 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __fltkSphere3D_h
#define __fltkSphere3D_h

#include "fltkShape3D.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif




namespace fltk {




/**
 *  \class fltk::Sphere3D derives from Shape3D and represents a sphere in 3D.
 */
class ITK_EXPORT Sphere3D : public Shape3D 
{

public:
  /**
   * Standard "Self" typedef.
   */
  typedef Sphere3D   Self;

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
   * Define the PointType using the Superclass
   */
  typedef Superclass::PointType     PointType;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Sphere3D, Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    

  /**
   * Set the Radius
   */
        void SetRadius( double radius );


  /**
   * Set the Center
   */
        void SetCenter( const PointType & center );


  /**
   * Set the Number Of Slices to digitize the base
   */
        void SetNumberOfSlices( unsigned int number );


  /**
   * Set the Number Of Stacks to digitize the base
   */
        void SetNumberOfStacks( unsigned int number );


  /**
   * Get the Radius
   */
        double GetRadius(void) const;


  /**
   * Get the Center
   */
        const PointType & GetCenter(void) const;


  /**
   * Get the Number of Slices
   */
        unsigned int GetNumberOfSlices(void) const;


  /**
   * Get the Number of Stacks
   */
        unsigned int GetNumberOfStacks(void) const;


  /**
   * Draw the Cone using OpenGL commands
   */
        void DrawGeometry(void) const;



protected:
  /**
   * Constructor
   */
        Sphere3D();

  /**
   * Destructor
   */
        ~Sphere3D();

private:
  /**
   *  Radius of the base
   */
        double          m_Radius;

  /**
   *   Height of the cone
   */
        double          m_Height;

  /**
   *   Number of Slices in which the circle is digitized
   */
        unsigned int    m_NumberOfSlices;

  /**
   *   Number of Stacks in which the circle is digitized
   */
        unsigned int    m_NumberOfStacks;

  /**
   *   Sphere using GLU support for quadrics.
   */
  GLUquadricObj   * m_GluQuadric;  

        };


}   // end namespace li

#endif
