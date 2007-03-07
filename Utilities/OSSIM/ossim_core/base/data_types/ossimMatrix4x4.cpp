//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for locpt.  This uses geotrans
// local cartesian implementation.
//*******************************************************************
//  $Id: ossimMatrix4x4.cpp,v 1.6 2005/09/06 16:23:41 gpotts Exp $

#include "ossimMatrix4x4.h"
#include "matrix/newmatap.h"
#include "base/common/ossimConstants.h"

ossimMatrix4x4::ossimMatrix4x4(const NEWMAT::Matrix& m)
   :theData(4,4)
{
   if((m.Nrows() == 4) &&
      (m.Ncols() == 4))
   {
      theData = m;
   }
   else if((m.Nrows()==3)&&
           (m.Ncols()==3))
   {
      theData[0][0] = m[0][0];
      theData[0][1] = m[0][1];
      theData[0][2] = m[0][2];
      theData[0][3] = 0.0;
      theData[1][0] = m[1][0];
      theData[1][1] = m[1][1];
      theData[1][2] = m[1][2];
      theData[1][3] = 0.0;
      theData[2][0] = m[2][0];
      theData[2][1] = m[2][1];
      theData[2][2] = m[2][2];
      theData[2][3] = 0.0;
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
   else
   {
      theData[0][0] = 1.0;
      theData[0][1] = 0.0;
      theData[0][2] = 0.0;
      theData[0][3] = 0.0;
      
      theData[1][0] = 0.0;
      theData[1][1] = 1.0;
      theData[1][2] = 0.0;
      theData[1][3] = 0.0;
      
      theData[2][0] = 0.0;
      theData[2][1] = 0.0;
      theData[2][2] = 1.0;
      theData[2][3] = 0.0;
      
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
}

ossimMatrix4x4::ossimMatrix4x4()
  :theData(4, 4)
{
  theData[0][0] = 1.0;
  theData[0][1] = 0.0;
  theData[0][2] = 0.0;
  theData[0][3] = 0.0;

  theData[1][0] = 0.0;
  theData[1][1] = 1.0;
  theData[1][2] = 0.0;
  theData[1][3] = 0.0;

  theData[2][0] = 0.0;
  theData[2][1] = 0.0;
  theData[2][2] = 1.0;
  theData[2][3] = 0.0;

  theData[3][0] = 0.0;
  theData[3][1] = 0.0;
  theData[3][2] = 0.0;
  theData[3][3] = 1.0;
}

ossimMatrix4x4::ossimMatrix4x4(double v00, double v01, double v02, double v03,
                               double v10, double v11, double v12, double v13,
                               double v20, double v21, double v22, double v23,
                               double v30, double v31, double v32, double v33)
  :theData(4, 4)
{
  theData[0][0] = v00;
  theData[0][1] = v01;
  theData[0][2] = v02;
  theData[0][3] = v03;

  theData[1][0] = v10;
  theData[1][1] = v11;
  theData[1][2] = v12;
  theData[1][3] = v13;

  theData[2][0] = v20;
  theData[2][1] = v21;
  theData[2][2] = v22;
  theData[2][3] = v23;

  theData[3][0] = v30;
  theData[3][1] = v31;
  theData[3][2] = v32;
  theData[3][3] = v33;
}

ossimColumnVector3d ossimMatrix4x4::getEigenValues()const
{
  NEWMAT::DiagonalMatrix d;
  NEWMAT::SymmetricMatrix s;

  s << theData;

  EigenValues(s, d);

  return ossimColumnVector3d(d[0], d[1], d[2]);
}

NEWMAT::Matrix ossimMatrix4x4::createIdentity()
{
  ossimMatrix4x4 m(1.0, 0.0, 0.0, 0.0, 
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createRotateOnly(const ossimMatrix4x4 &aMatrix)
{
  ossimMatrix4x4 m = aMatrix;

  m.theData[0][3] = 0;
  m.theData[1][3] = 0;
  m.theData[2][3] = 0;
  m.theData[3][3] = 1.0;

  m.theData[3][0] = 0.0;
  m.theData[3][1] = 0.0;
  m.theData[3][2] = 0.0;
  m.theData[3][3] = 1.0;

  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createZero()
{
  ossimMatrix4x4 m;

  m.getData() = 0.0;

  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createTranslationMatrix(double x,
                                                       double y,
                                                       double z) 
{
   ossimMatrix4x4 m(1.0, 0.0, 0.0, x,
                    0.0, 1.0, 0.0, y,
                    0.0, 0.0, 1.0, z,
                    0.0, 0.0, 0.0, 1.0);
   
   return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createRotationMatrix(double angleX,
                                                    double angleY,
                                                    double angleZ,
                                                    ossimCoordSysOrientMode orientationMode)
{
  return (createRotationZMatrix(angleZ,
                                orientationMode)*
	  createRotationYMatrix(angleY,
                                orientationMode)*
	  createRotationXMatrix(angleX,
                                orientationMode));
}

NEWMAT::Matrix ossimMatrix4x4::createRotationXMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << Sine << 0.0
        << 0.0 << -Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
   else
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << -Sine << 0.0
        << 0.0 << Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
  
   return m;
}

NEWMAT::Matrix ossimMatrix4x4::createRotationYMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << Cosine << 0.0 << -Sine  << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << Sine   << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   else
   {
      m << Cosine << 0.0 << Sine   << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << -Sine  << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   
  return m;
}


NEWMAT::Matrix ossimMatrix4x4::createRotationZMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << Cosine << Sine   << 0.0 << 0.0
        << -Sine  << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   else
   {
      m << Cosine << -Sine   << 0.0 << 0.0
        << Sine   << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   
   return m;
}


NEWMAT::Matrix ossimMatrix4x4::createScaleMatrix(double x, double y, double z)
{
   NEWMAT::Matrix m(4, 4);
   
      m << x   << 0.0 << 0.0 << 0.0
        << 0.0 << y   << 0.0 << 0.0
        << 0.0 << 0.0 << z   << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;

    return m;
}
