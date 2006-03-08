/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkVectorImage3D.txx,v $
  Language:  C++
  Date:      $Date: 2002/01/22 04:27:48 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _fltkVectorImage3D_txx
#define _fltkVectorImage3D_txx

#include "fltkVectorImage3D.h"



namespace fltk {



//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
template <class TVectorImage>
VectorImage3D<TVectorImage>
::VectorImage3D(void)  
{
  m_VectorImage = VectorImageType::New();
  SetDrawingMode( lines );
  m_VectorLengthScale = 1.0f;
}




//--------------------------------------------------
//
//    Get VectorImage
//
//--------------------------------------------------
template <class TVectorImage>
typename VectorImage3D<TVectorImage>::VectorImageConstPointer
VectorImage3D<TVectorImage>
::GetVectorImage(void) const 
{
  return m_VectorImage.GetPointer(); 
}



//--------------------------------------------------
//
//    Set VectorImage
//
//--------------------------------------------------
template <class TVectorImage>
void
VectorImage3D<TVectorImage>
::SetVectorImage( VectorImageType * vectorImage )  
{
  m_VectorImage = vectorImage ; 
}




//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
template <class TVectorImage>
void 
VectorImage3D<TVectorImage>
::DrawGeometry(void)  const
{

  if( !m_VectorImage )
  {
    return;
  }


  RegionType region = m_VectorImage->GetRequestedRegion();

  itk::ImageRegionIteratorWithIndex< VectorImageType > 
                                             it( m_VectorImage, region );


  IndexType index = region.GetIndex();
  SizeType  size  = region.GetSize();

  IndexType centerIndex;
  for( unsigned int i=0; i<VectorImageType::ImageDimension; i++ )
    {
    centerIndex[i] = index[i] + size[i] / 2;
    }

  itk::Point< double, 3>   center;

  m_VectorImage->TransformIndexToPhysicalPoint( centerIndex, center );
  
  glPushMatrix();

  glTranslated( -center[0], -center[1], -center[2] );


  if( GetDrawingMode() == triangles ) 
    {
    }

  if( GetDrawingMode()== points ) 
    {
    }

  if( GetDrawingMode()== lines ) 
    {

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    VectorType gradient;
    itk::Point< double, 3 >   point;

    it.GoToBegin();
    while( !it.IsAtEnd() )
      {
      IndexType index = it.GetIndex();
      m_VectorImage->TransformIndexToPhysicalPoint( index, point );
      glVertex3d( point[0], point[1], point[2] );
      gradient = it.Get();
      gradient *= m_VectorLengthScale;
      glVertex3d( point[0] + gradient[0], 
                  point[1] + gradient[1], 
                  point[2] + gradient[2] );

      ++it;
      }

    glEnd();
    }

  glPopMatrix();

}




} // end namespace fltk


#endif
