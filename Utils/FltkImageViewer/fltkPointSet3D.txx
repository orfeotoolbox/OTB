/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkPointSet3D.txx,v $
  Language:  C++
  Date:      $Date: 2005/12/08 18:20:44 $
  Version:   $Revision: 1.7 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _fltkPointSet3D_txx
#define _fltkPointSet3D_txx

#include "fltkPointSet3D.h"



namespace fltk {



//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
template <class TPointSet>
PointSet3D<TPointSet>
::PointSet3D(void)  
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
typename PointSet3D<TPointSet>::PointSetConstPointer
PointSet3D<TPointSet>
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
PointSet3D<TPointSet>
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
PointSet3D<TPointSet>
::DrawGeometry(void)  const
{

  if( !m_PointSet )
  {
    return;
  }


    if( GetDrawingMode() == triangles ) 
    {
    }

    if( GetDrawingMode()== lines ) 
    {
    }

    if( GetDrawingMode()== points ) 
    {
     
      // Draw the axes for context information
      // red (+ve x ), green (+ve y), blue (+ve z)
      glColor3f( 1, 0, 0 );
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 20.0, 0.0, 0.0 );
      glEnd();
      glColor3f( 1, 1, .3 );
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( -20.0, 0.0, 0.0 );
      glEnd();
      glColor3f( 0, 1, 0 );
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 0.0, 20.0, 0.0 );
      glEnd();
      glColor3f( 0, 1, 1);
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 0.0, -20.0, 0.0 );
      glEnd();
      glColor3f( 0, 0, 1 );
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.0, 20.0 );
      glEnd();
      glColor3f( 1, 0, 1 );
      glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.0, -20.0 );
      glEnd();
      glColor();
      
      
      PointsContainerPointer    meshPoints = m_PointSet->GetPoints();
      PointDataContainerPointer meshData   = m_PointSet->GetPointData();

      if( !meshPoints )
      {
        return;
      }

      typedef typename PointsContainerType::ConstIterator      PointIteratorType;   
      typedef typename PointDataContainerType::ConstIterator   DataIteratorType;   

      glDisable(GL_LIGHTING);
      glBegin(GL_POINTS);

      PointIteratorType p = meshPoints->Begin();

      DataIteratorType  d;
//      bool pointDataValuesExist = false;
      if( meshData )
        {
        d = meshData->Begin();
//        pointDataValuesExist = true;
        }

      while( p != meshPoints->End() )
      {
        const PointType point = p.Value();
        glVertex3d( point[0], point[1], point[2] );
        ++p;
      }

      glEnd();
    }


}



} // end namespace fltk


#endif
