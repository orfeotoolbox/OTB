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
//*******************************************************************
//  $Id: ossimMatrix3x3.cpp,v 1.13 2005/09/06 16:23:41 gpotts Exp $

#include <base/data_types/ossimMatrix3x3.h>
#include <matrix/newmatap.h>
#include <base/data_types/ossimColumnVector3d.h>
#include <base/common/ossimConstants.h>

NEWMAT::Matrix ossimMatrix3x3::create()
{
   NEWMAT::Matrix m(3, 3);
   
   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = 0.0;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = 0.0;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;

   return m;
}

NEWMAT::Matrix ossimMatrix3x3::create(double v00, double v01, double v02,
                                      double v10, double v11, double v12,
                                      double v20, double v21, double v22)
{
   NEWMAT::Matrix m(3, 3);
   
   m[0][0] = v00;
   m[0][1] = v01;
   m[0][2] = v02;
   
   m[1][0] = v10;
   m[1][1] = v11;
   m[1][2] = v12;
   
   m[2][0] = v20;
   m[2][1] = v21;
   m[2][2] = v22;

   return m;
}

NEWMAT::Matrix ossimMatrix3x3::create(const NEWMAT::Matrix& rhs)
{
   NEWMAT::Matrix m(3, 3);

   if (rhs.Ncols() != 3 || rhs.Nrows() != 3)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "ossimMatrix3x3::create(const NEWMAT::Matrix& rhs) ERROR:"
                                          << "\nMatrix passed to function not a 3x3!"
                                          << "\nnumber of columns:  " << rhs.Ncols()
                                          << "\nnumber of rows:     " << rhs.Nrows()
                                          << "\nReturn blank 3x3 matrix...\n";
      return m;
   }
   
   m[0][0] = rhs[0][0];
   m[0][1] = rhs[0][1];
   m[0][2] = rhs[0][2];
   m[0][3] = rhs[0][3];
   m[1][0] = rhs[1][0];
   m[1][1] = rhs[1][1];
   m[1][2] = rhs[1][2];
   m[1][3] = rhs[1][3];
   m[2][0] = rhs[2][0];
   m[2][1] = rhs[2][1];
   m[2][2] = rhs[2][2];
   m[2][3] = rhs[2][3];

   return m;
}

ossimColumnVector3d ossimMatrix3x3::getEigenValues(const NEWMAT::Matrix& matrix)
{
   if (matrix.Ncols() != 3 || matrix.Nrows() != 3)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: ossimColumnVector3d operator*(const NEWMAT::Matrix& lhs,"
                                          << "\nconst ossimColumnVector3d& rhs), "
                                          << "\nMatrix passed to function not a 3x3!"
                                          << "\nnumber of columns:  " << matrix.Ncols()
                                          << "\nnumber of rows:     " << matrix.Nrows()
                                          << "\nReturn blank ossimColumnVector3d...\n";
      return ossimColumnVector3d();
   }

   NEWMAT::DiagonalMatrix d;
   NEWMAT::SymmetricMatrix s;
   
   s << matrix;
   
   EigenValues(s, d);
   
   return ossimColumnVector3d(d[0], d[1], d[2]);
}

NEWMAT::Matrix ossimMatrix3x3::createIdentity()
{
   NEWMAT::Matrix m(3,3);

   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = 0.0;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = 0.0;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;
   
   return m;
}

NEWMAT::Matrix ossimMatrix3x3::createZero()
{
  NEWMAT::Matrix m(3,3);

  m = 0.0;

  return m;
}

NEWMAT::Matrix ossimMatrix3x3::createTranslationMatrix(double dx,double dy)
{
  NEWMAT::Matrix m(3,3);

   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = dx;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = dy;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;

   return m;
}

NEWMAT::Matrix ossimMatrix3x3::createRotationMatrix(double angleX,
                                                    double angleY,
                                                    double angleZ,
                                                    ossimCoordSysOrientMode orientationMode)
{
   return (createRotationZMatrix(angleZ, orientationMode)*
           createRotationYMatrix(angleY, orientationMode)*
           createRotationXMatrix(angleX, orientationMode));
}

NEWMAT::Matrix ossimMatrix3x3::createRotationXMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
  NEWMAT::Matrix m(3,3);
  double Cosine = cos(angle*RAD_PER_DEG);
  double Sine   = sin(angle*RAD_PER_DEG);

  if(orientationMode == OSSIM_RIGHT_HANDED)
  {
     m << 1 << 0      << 0
       << 0 << Cosine << Sine
       << 0 << -Sine  << Cosine;
  }
  else
  {
     m << 1 << 0      << 0
       << 0 << Cosine << -Sine
       << 0 << Sine  << Cosine;
  }
  return m;
}

NEWMAT::Matrix ossimMatrix3x3::createRotationYMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(3,3);
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
  if(orientationMode == OSSIM_RIGHT_HANDED)
  {
     m[0][0] = Cosine;
     m[0][1] = 0.0;
     m[0][2] = -Sine;
     
     m[1][0] = 0.0;
     m[1][1] = 1.0;
     m[1][2] = 0.0;
     
     m[2][0] = Sine;
     m[2][1] = 0.0;
     m[2][2] = Cosine;
  }
  else
  {
     m[0][0] = Cosine;
     m[0][1] = 0.0;
     m[0][2] = Sine;
     
     m[1][0] = 0.0;
     m[1][1] = 1.0;
     m[1][2] = 0.0;
     
     m[2][0] = -Sine;
     m[2][1] = 0.0;
     m[2][2] = Cosine;
  }
  return m;
}


NEWMAT::Matrix ossimMatrix3x3::createRotationZMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(3,3);
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
  if(orientationMode == OSSIM_RIGHT_HANDED)
  {
     m[0][0] = Cosine;
     m[0][1] = Sine;
     m[0][2] = 0.0;
     
     m[1][0] = -Sine;
     m[1][1] = Cosine;
     m[1][2] = 0.0;
     
     m[2][0] = 0.0;
     m[2][1] = 0.0;
     m[2][2] = 1.0;
  }
  else
  {
     m[0][0] = Cosine;
     m[0][1] = -Sine;
     m[0][2] = 0.0;
     
     m[1][0] = Sine;
     m[1][1] = Cosine;
     m[1][2] = 0.0;
     
     m[2][0] = 0.0;
     m[2][1] = 0.0;
     m[2][2] = 1.0;
  }
  return m;
}

NEWMAT::Matrix ossimMatrix3x3::createScaleMatrix(double x, double y, double z)
{
    NEWMAT::Matrix m = createIdentity();

    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;

    return m;
}
