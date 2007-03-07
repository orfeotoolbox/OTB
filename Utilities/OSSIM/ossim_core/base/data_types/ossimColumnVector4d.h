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
// Description: This is your 3-D vector where the 4-th dimension is
//              for the homogenious coordinate.
//
//*******************************************************************
//  $Id: ossimColumnVector4d.h,v 1.3 2004/09/10 15:45:34 gpotts Exp $
#ifndef ossimColumnVector4d_HEADER
#define ossimColumnVector4d_HEADER
#include <base/common/ossimConstants.h>
#include <base/common/ossimCommon.h>
#include <float.h> // for FLT_EPSILON
#include <iomanip>
#include <iostream>
#include <cmath>
using namespace std;

class OSSIMDLLEXPORT ossimColumnVector4d
{
public:
   ossimColumnVector4d()
      {
         data[0]=0;
         data[1]=0;
         data[2]=0;
         data[3]=1;
      }

   ossimColumnVector4d(double x, double y, double z, double w=1.0)
      {
         data[0]=x;
         data[1]=y;
         data[2]=z;
         data[3]=w;
      }

   ossimColumnVector4d(const ossimColumnVector4d &rhs)
      {
         data[0] = rhs.data[0];
         data[1] = rhs.data[1];
         data[2] = rhs.data[2];
         data[3] = rhs.data[3];
      }
   friend ostream& operator <<(ostream& out, const ossimColumnVector4d& v)
      {
         return out << setiosflags(ios::fixed) << setprecision(15)
                    << v[0] << " " << v[1] <<" "
                    << v[2] << " " << v[3];
      }

   friend ossimColumnVector4d operator*(double scalar, const ossimColumnVector4d &v)
      {
         return ossimColumnVector4d(v.data[0]*scalar,
                              v.data[1]*scalar,
                              v.data[2]*scalar);
      }

   double& operator [](int index)
      {
         return data[index];
      }

   const double& operator [](int index)const
      {
         return data[index];
      }

   ossimColumnVector4d operator*(double scalar)const
      {
         return ossimColumnVector4d(data[0]*scalar,
                         data[1]*scalar,
                         data[2]*scalar);
      }

   ossimColumnVector4d operator +(const ossimColumnVector4d &rhs)const
      {
         return ossimColumnVector4d( data[0]+rhs[0],
                               data[1]+rhs[1],
                               data[2]+rhs[2]);
      }

   ossimColumnVector4d operator +=(const ossimColumnVector4d &rhs)
      {
         data[0] += rhs[0];
         data[1] += rhs[1];
         data[2] += rhs[2];

         return *this;
      }

   ossimColumnVector4d operator - ()const
      {
         return ossimColumnVector4d(-data[0],
                         -data[1],
                         -data[2]);
      }

   ossimColumnVector4d operator /(double scalar)const
      {
         return ossimColumnVector4d( data[0]/scalar,
                               data[1]/scalar,
                               data[2]/scalar);
      }

   ossimColumnVector4d& operator /=(double scalar)
      {
         data[0]/=scalar;
         data[1]/=scalar;
         data[2]/=scalar;

         return *this;
      }

   ossimColumnVector4d operator -(const ossimColumnVector4d &rhs)const
      {
         return ossimColumnVector4d( data[0]-rhs[0],
                               data[1]-rhs[1],
                               data[2]-rhs[2]);
      }

   ossimColumnVector4d operator -=(const ossimColumnVector4d &rhs)
      {
         data[0] -= rhs[0];
         data[1] -= rhs[1];
         data[2] -= rhs[2];

         return *this;
      }

   double magnitude()const
      {
         return std::sqrt(data[0]*data[0] +
                          data[1]*data[1] +
                          data[2]*data[2]);
      }
   double dot(const ossimColumnVector4d &rhs)const
      {
         return (data[0]*rhs[0]+
                 data[1]*rhs[1]+
                 data[2]*rhs[2]);
      }

   ossimColumnVector4d unit()const
   {
      double mag = magnitude();

      if(ossimAbs(mag) > DBL_EPSILON)
      {
         mag = 1.0/mag;
         return (*this *mag);
      }

      return *this;
   }

   ossimColumnVector4d cross(const ossimColumnVector4d &rhs)
      {
         return ossimColumnVector4d( data[1]*rhs[2] - data[2]*rhs[1],
                               data[2]*rhs[0] - data[0]*rhs[2],
                               data[0]*rhs[1] - data[1]*rhs[0]);
      }

   ossimColumnVector4d& xAligned()// creates 4-D homogeneous vectors
      {
         data[0] = 1;
         data[1] = 0;
         data[2] = 0;
         data[3] = 1;

         return *this;
      }
   ossimColumnVector4d& yAligned()//
      {
         data[0] = 0;
         data[1] = 1;
         data[2] = 0;
         data[3] = 1;

         return *this;
      }
   ossimColumnVector4d& zAligned() //
      {
         data[0] = 0;
         data[1] = 0;
         data[2] = 1;
         data[3] = 1;

         return *this;
      }

private:
   double data[4];
};

#endif
