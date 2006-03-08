/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkPointSet2D.txx,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:03 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _fltkPointSet2D_txx
#define _fltkPointSet2D_txx

#include "fltkPointSet2D.h"



namespace fltk {



//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
template <class TPointSet>
PointSet2D<TPointSet>
::PointSet2D(void)  
{
  m_PointSet = PointSetType::New();
  SetDrawingMode( points );
}




//--------------------------------------------------
//
//    Get PointSet
//
//--------------------------------------------------
template <class TPointSet>
typename PointSet2D<TPointSet>::PointSetConstPointer
PointSet2D<TPointSet>
::GetPointSet(void) const 
{
  return m_PointSet.GetPointer(); 
}



//--------------------------------------------------
//
//    Set PointSet
//
//--------------------------------------------------
template <class TPointSet>
void
PointSet2D<TPointSet>
::SetPointSet( PointSetType * mesh)  
{
  m_PointSet = mesh; 
}




//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
template <class TPointSet>
void 
PointSet2D<TPointSet>
::DrawGeometry(void)  const
{

    if( !m_PointSet )
    {
      return;
    }

    GLfloat viewport[4];
    glGetFloatv( GL_VIEWPORT, viewport );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, viewport[2], viewport[3], 0.0 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
   
    if( GetDrawingMode() == triangles ) 
    {
    }

    if( GetDrawingMode()== lines ) 
    {
    }

    if( GetDrawingMode()== points ) 
    {

      PointsContainerPointer    meshPoints = m_PointSet->GetPoints();
      PointDataContainerPointer meshData   = m_PointSet->GetPointData();

      if( !meshPoints )
      {
        return;
      }

      typedef typename PointsContainerType::ConstIterator      PointIteratorType;   
      typedef typename PointDataContainerType::ConstIterator   DataIteratorType;   

      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glBegin(GL_POINTS);

      PointIteratorType p = meshPoints->Begin();

      DataIteratorType  d;
      bool pointDataValuesExist = false;
      if( meshData )
        {
        d = meshData->Begin();
        pointDataValuesExist = true;
        }

      while( p != meshPoints->End() )
      {
        
        const PointType point = p.Value();
       
        if( pointDataValuesExist )
        {
        //  if( d.Value() > 0 ) 
        //  { // color in blue
        //    glColor3f( (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0 ); 
        //  }
        //  else 
        //  { // color in red
        //   glColor3f( (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0 ); 
        //  }
        // ++d;
        }
        
        glVertex2d( point[0], point[1] );
        ++p;
      }

      glEnd();
    }

    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );


}



} // end namespace fltk


#endif
