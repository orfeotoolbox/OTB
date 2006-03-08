/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkCone3D.cxx,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:52 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "fltkCone3D.h"
#include <math.h>


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Cone3D::Cone3D(void)  
{
  m_Radius           = 1.0;
  m_Height           = 1.0;
  m_NumberOfSectors  =   6;
}



//--------------------------------------------------
//
//    Destructor
//
//--------------------------------------------------
Cone3D::~Cone3D(void)  
{
}



//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
Cone3D::SetRadius( double radius ) 
{
  m_Radius = radius;
}



//--------------------------------------------------
//
//    Set Height
//
//--------------------------------------------------
void 
Cone3D::SetHeight( double height ) 
{
  m_Height = height;
}



//--------------------------------------------------
//
//    Set Number of Sectors
//
//--------------------------------------------------
void 
Cone3D::SetNumberOfSectors( unsigned int number ) 
{
  m_NumberOfSectors = number;
}



//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
Cone3D::GetRadius( void ) const
{
  return m_Radius;
}


//--------------------------------------------------
//
//    Get Height
//
//--------------------------------------------------
double 
Cone3D::GetHeight( void ) const
{
  return m_Height;
}


//--------------------------------------------------
//
//    Get Length
//
//--------------------------------------------------
unsigned int 
Cone3D::GetNumberOfSectors( void ) const
{
  return m_NumberOfSectors;
}






//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
void 
Cone3D::DrawGeometry(void) const 
{

  const double factor = 8.0 * atan( 1.0 ) / m_NumberOfSectors;

  glBegin(GL_TRIANGLE_FAN); 
  {
    glVertex3d( 0.0, 0.0, m_Height );
    for(unsigned int i=0; i<m_NumberOfSectors+1; i++ )
    {
      const GLdouble th = i * factor;
      const GLdouble x  = m_Radius * cos( th );
      const GLdouble y  = m_Radius * sin( th );
      const GLdouble xn = m_Height * cos( th );
      const GLdouble yn = m_Height * sin( th );
      glNormal3d( xn, yn, m_Radius );
      glVertex3d(  x,  y, 0.0 );
    }
  }
  glEnd();



  glBegin(GL_TRIANGLE_FAN); 
  {
    glNormal3d( 0.0, 0.0, -1.0 );
    glVertex3d( 0.0, 0.0,  0.0 );
    for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
    {
      const GLdouble th =  i * factor;
      const GLdouble x  =  m_Radius * cos( th );
      const GLdouble y  = -m_Radius * sin( th );
      glVertex3d(x,y,0.0);
    }
  }  
  glEnd();

}



}  // end namespace li

