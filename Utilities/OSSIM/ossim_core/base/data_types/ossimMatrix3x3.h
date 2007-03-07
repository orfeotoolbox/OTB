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
// Helper class for using NEWMAT::Matrix's.
//
//*******************************************************************
//  $Id: ossimMatrix3x3.h,v 1.14 2005/09/06 16:23:41 gpotts Exp $

#ifndef ossimMatrix3x3_HEADER
#define ossimMatrix3x3_HEADER

#include <iostream>

#include "matrix/newmat.h"
#include "matrix/newmatio.h"
#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimColumnVector3d;

class OSSIMDLLEXPORT ossimMatrix3x3
{
 public:

   
   /*!
    *  Uses the matrix package to compute the eigenvalues for this
    *  matrix
    */ 
   ossimColumnVector3d getEigenValues(const NEWMAT::Matrix& rhs);

   /*!
    *  Returns a blank 3x3 matrix.
    */
   static NEWMAT::Matrix create();

   /*!
    *  Returns a copy of "m".
    *  Note:
    *  If "m" is not a 3x3 that's an error and a blank matrix is returned.
    */
   static NEWMAT::Matrix create(const NEWMAT::Matrix& m);

   /*!
    *  Create a 3x3 with assigned values.
    */
   static NEWMAT::Matrix create(double v00, double v01, double v02,
                                double v10, double v11, double v12,
                                double v20, double v21, double v22);
   
   static NEWMAT::Matrix createIdentity();
   
   static NEWMAT::Matrix createZero();
   static NEWMAT::Matrix createRotationMatrix(double angleX,
                                              double angleY,
                                              double angleZ,
                                              ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationXMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationYMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationZMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);

   static NEWMAT::Matrix createScaleMatrix(double X, double Y, double Z);

   static NEWMAT::Matrix createTranslationMatrix(double dx,double dy);
   
protected:
   ossimMatrix3x3(){}  // To squash un-warranted compiler warnings...
   
private:
   ossimMatrix3x3(const ossimMatrix3x3& rhs){}
   const ossimMatrix3x3& operator=(const ossimMatrix3x3& rhs){ return rhs; }
   
};

#endif
