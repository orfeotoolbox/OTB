/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkCylinder3D.cxx,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:53 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "fltkCylinder3D.h"
#include <math.h>


namespace fltk {


//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Cylinder3D::Cylinder3D(void)  
{
  m_Radius           = 1.0;
  m_Height           = 1.0;
  m_NumberOfSectors  =   6;
}





//--------------------------------------------------
//
//    Set Radius
//
//--------------------------------------------------
void 
Cylinder3D::SetRadius( double radius ) 
{
  m_Radius = radius;
}



//--------------------------------------------------
//
//    Set Height
//
//--------------------------------------------------
void 
Cylinder3D::SetHeight( double height ) 
{
  m_Height = height;
}



//--------------------------------------------------
//
//    Set Number of Sectors
//
//--------------------------------------------------
void 
Cylinder3D::SetNumberOfSectors( unsigned int number ) 
{
  m_NumberOfSectors = number;
}



//--------------------------------------------------
//
//    Get Radius
//
//--------------------------------------------------
double 
Cylinder3D::GetRadius( void ) const
{
  return m_Radius;
}


//--------------------------------------------------
//
//    Get Height
//
//--------------------------------------------------
double 
Cylinder3D::GetHeight( void ) const
{
  return m_Height;
}


//--------------------------------------------------
//
//    Get Length
//
//--------------------------------------------------
unsigned int 
Cylinder3D::GetNumberOfSectors( void ) const
{
  return m_NumberOfSectors;
}






//--------------------------------------------------
//
//    SmartPointer Constructor
//
//--------------------------------------------------
void 
Cylinder3D::DrawGeometry(void) const 
{

  if( GetDrawingMode() == none ) 
  {
    return;
  }

  // Angle subtended by each sector
  const double factor = 8.0 * atan( 1.0f ) / m_NumberOfSectors;

  if( GetDrawingMode() == points ) 
  {
    // Draw the points
    glBegin( GL_POINTS ); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }



  if( GetDrawingMode() == lines ) 
  {
    // Draw the bottom circle
    glBegin(GL_LINE_STRIP); 
    {
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, 0.0 );
      }
    }  
    glEnd();

    // Draw the top circle
    glBegin(GL_LINE_STRIP); 
    {
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
      }
    }  
    glEnd();

    // Draw the side
    glBegin(GL_LINES); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }

  if( GetDrawingMode() == triangles ) 
  {
    // Draw the bottom circle
    glBegin(GL_TRIANGLE_FAN); 
    {
      glNormal3d( 0.0, 0.0, -1.0 );
      glVertex3d( 0.0, 0.0,  0.0 );
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, 0.0 );
      }
    }  
    glEnd();

    // Draw the top circle
    glBegin(GL_TRIANGLE_FAN); 
    {
      glNormal3d( 0.0, 0.0, 1.0 );
      glVertex3d( 0.0, 0.0, m_Height );
      for( unsigned int i=0; i<m_NumberOfSectors+1; i++ ) 
      {
        const GLdouble th =  i * factor;
        const GLdouble x  =  m_Radius * cos( th );
        const GLdouble y  = -m_Radius * sin( th );
        glVertex3d( x, y, m_Height );
      }
    }  
    glEnd();

    // Draw the side
    glBegin(GL_QUAD_STRIP); 
    {
      for(unsigned int i=0; i<m_NumberOfSectors+1; i++) 
      {
        const GLdouble th = i * factor;
        const GLdouble x = m_Radius * cos( th );
        const GLdouble y = m_Radius * sin( th );
        glNormal3d( x, y, 0.0 );
        glVertex3d( x, y, m_Height );
        glVertex3d( x, y, 0.0 );
        }
    }
    glEnd();
  }

}



}  // end namespace li

