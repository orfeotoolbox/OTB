/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkFrustum3D.cxx,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:24:54 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "fltkFrustum3D.h"


namespace fltk {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
Frustum3D::Frustum3D(void)  
{
  m_ApertureAngleX   = 30.0;
  m_ApertureAngleY   = 30.0;
  m_BottomPlane      = 10.0;
  m_TopPlane         =  2.0;
}






//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
void 
Frustum3D::DrawGeometry(void)  const 
{

  const double deg2rad = atan(1.0f)/45.0;

  const GLdouble h1 = m_TopPlane;
  const GLdouble h2 = m_BottomPlane;
  const GLdouble l1 = m_TopPlane    * sin( m_ApertureAngleX * deg2rad );
  const GLdouble w1 = m_TopPlane    * sin( m_ApertureAngleY * deg2rad );
  const GLdouble l2 = m_BottomPlane * sin( m_ApertureAngleX * deg2rad );
  const GLdouble w2 = m_BottomPlane * sin( m_ApertureAngleY * deg2rad );

                if( GetDrawingMode() == triangles ) 
      {
                glBegin(GL_QUADS);

                        glNormal3d(0.0, h2,-w2);
                        glVertex3d(-l1, w1, h1);
                        glVertex3d(-l2, w2, h2);
                        glVertex3d( l2, w2, h2);
                        glVertex3d( l1, w1, h1);

                        glNormal3d(0.0,-h2,-w2);
                        glVertex3d(-l1,-w1, h1);
                        glVertex3d( l1,-w1, h1);
                        glVertex3d( l2,-w2, h2);
                        glVertex3d(-l2,-w2, h2);

                        glNormal3d(-h2,0.0,-l2);
                        glVertex3d(-l1, w1, h1);
                        glVertex3d(-l1,-w1, h1);
                        glVertex3d(-l2,-w2, h2);
                        glVertex3d(-l2, w2, h2);

                        glNormal3d( h2,0.0,-l2);
                        glVertex3d( l1, w1, h1);
                        glVertex3d( l2, w2, h2);
                        glVertex3d( l2,-w2, h2);
                        glVertex3d( l1,-w1, h1);

                        glNormal3d(0.0,0.0,-1.0);
                        glVertex3d(-l1, w1, h1);
                        glVertex3d( l1, w1, h1);
                        glVertex3d( l1,-w1, h1);
                        glVertex3d(-l1,-w1, h1);

                        glNormal3d(0.0,0.0,1.0);
                        glVertex3d(-l2, w2, h2);
                        glVertex3d(-l2,-w2, h2);
                        glVertex3d( l2,-w2, h2);
                        glVertex3d( l2, w2, h2);

      glEnd();
      }

                if( GetDrawingMode() == lines ) 
      {
                        glDisable(GL_LIGHTING);
                        glBegin(GL_LINE_LOOP);
                                glVertex3d(-l2, w2, h2);
                                glVertex3d(-l2,-w2, h2);
                                glVertex3d( l2,-w2, h2);
                                glVertex3d( l2, w2, h2);
                        glEnd();
                        glBegin(GL_LINE_LOOP);
                                glVertex3d(-l1, w1, h1);
                                glVertex3d(-l1,-w1, h1);
                                glVertex3d( l1,-w1, h1);
                                glVertex3d( l1, w1, h1);
                        glEnd();
                        glBegin(GL_LINES);
                                glVertex3d( l1,-w1, h1);
                                glVertex3d( l2,-w2, h2);
                                glVertex3d( l1, w1, h1);
                                glVertex3d( l2, w2, h2);
                                glVertex3d(-l1,-w1, h1);
                                glVertex3d(-l2,-w2, h2);
                                glVertex3d(-l1, w1, h1);
                                glVertex3d(-l2, w2, h2);
                        glEnd();
                        glEnable(GL_LIGHTING);
                }
}




} // end namespace fltk


