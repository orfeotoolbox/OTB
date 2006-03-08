/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSphere3D.cxx,v $
  Language:  C++
  Date:      $Date: 2002/09/19 19:29:58 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkSphere3D.h"


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Sphere3D::Sphere3D(void)  
{
  m_Radius           = 1.0;
  m_NumberOfSlices   =   6;
  m_NumberOfStacks   =   6;
  m_GluQuadric = gluNewQuadric();
}



//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
Sphere3D::~Sphere3D(void)  
{
  gluDeleteQuadric( m_GluQuadric );
}





//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
Sphere3D::SetRadius( double radius ) 
{
  m_Radius = radius;
}



//--------------------------------------------------
//
//    Set Number of Slices
//
//--------------------------------------------------
void 
Sphere3D::SetNumberOfSlices( unsigned int number )
{
  m_NumberOfSlices = number;
}



//--------------------------------------------------
//
//    Set Number of Stacks
//
//--------------------------------------------------
void 
Sphere3D::SetNumberOfStacks( unsigned int number ) 
{
  m_NumberOfStacks = number;
}



//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
Sphere3D::GetRadius( void ) const
{
  return m_Radius;
}




//--------------------------------------------------
//
//    Get Number of Sectors
//
//--------------------------------------------------
unsigned int 
Sphere3D::GetNumberOfSlices( void ) const
{
  return m_NumberOfSlices;
}





//--------------------------------------------------
//
//    Get Number of Stacks
//
//--------------------------------------------------
unsigned int 
Sphere3D::GetNumberOfStacks( void ) const
{
  return m_NumberOfStacks;
}





//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
void 
Sphere3D::DrawGeometry(void) const 
{

  switch( this->GetDrawingMode() )
    {
    case points:
      gluQuadricDrawStyle( m_GluQuadric, GLU_POINT );
      break;
    case lines:
      gluQuadricDrawStyle( m_GluQuadric, GLU_LINE );
      break;
    case triangles:
      gluQuadricDrawStyle( m_GluQuadric, GLU_FILL );
      break;
    case none:
      break;
    case surfacepoints:
      break;
    }
  gluQuadricNormals( m_GluQuadric, GL_SMOOTH );
  gluSphere( m_GluQuadric, 
             m_Radius, 
             m_NumberOfSlices, 
             m_NumberOfStacks   );

}



}  // end namespace li

